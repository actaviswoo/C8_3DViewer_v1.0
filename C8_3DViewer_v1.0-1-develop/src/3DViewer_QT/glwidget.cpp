#include "glwidget.h"
#include <stdlib.h>

#include <QColor>
#include <QColorDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QMainWindow>
#include <QSettings>
#include <QTimer>
#include <cstring>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { initSettings(); }

GLWidget::~GLWidget() { rm_data(&data); }

void GLWidget::initSettings() {
  backgroundColor.setRgb(0, 0, 0);
  lineColor.setRgb(255, 255, 255);
  dotColor.setRgb(255, 255, 255);
}

void GLWidget::setutProjection() {
  if (projectionMode == 0) {
      glOrtho(-data.max_c * 1.1, data.max_c * 1.1, -data.max_c * 1.1, data.max_c * 1.1, -data.max_c * 1.1, data.max_c * 5);
  } else if (projectionMode == 1) {
      double fov = 60.0 * M_PI / 180;  // 60 угол в градусах
      double heapHeight = data.max_c / (2 * tan(fov / 2));
      glFrustum(-data.max_c, data.max_c, -data.max_c, data.max_c, heapHeight, heapHeight * 20);
      glTranslated(0, 0, -heapHeight * 3);
  }
}

void GLWidget::initializeGL() {
  glClearColor(0.0, 0.0, 0.0, 1.0);  // Заливка черным цветом
  glClearDepth(1.0);  // Разрешить очистку буфера глубины
  glEnable(GL_DEPTH_TEST);  // Разрешить тест глубины
}

void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);  // Установка точки опоры
}

void GLWidget::paintGL() {
  if (data.array_polygons != NULL && data.matrix_3d != NULL) {
      glClearColor(backgroundColor.redF(), backgroundColor.greenF(), backgroundColor.blueF(), backgroundColor.alphaF());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glMatrixMode(GL_PROJECTION);  // Инициализация матрицы проекции
      glLoadIdentity();  // Сброс матрицы проекции
      setutProjection();
      glMatrixMode(GL_MODELVIEW);  // Инициализация матрицы модели
      glLoadIdentity();  // Сброс матрицы вида модели
      glVertexPointer(3, GL_DOUBLE, 0, data.matrix_3d[0]);
      glEnableClientState(GL_VERTEX_ARRAY);
      if (typeVertcies != 2) {
        glColor3d(dotColor.redF(), dotColor.greenF(), dotColor.blueF());
        glPointSize(pSize);
        if (typeVertcies == 0) {
          glEnable(GL_POINT_SMOOTH);
        }
        glDrawArrays(GL_POINTS, 0, data.count_of_vertexes);
        if (typeVertcies == 0) {
          glDisable(GL_POINT_SMOOTH);
        }
      }
      glColor3f(lineColor.redF(), lineColor.greenF(), lineColor.blueF());
      glLineWidth(lwSize);
      if (typeEdges == 1) {
        glEnable(GL_LINE_STIPPLE);
        glLineStipple(2, 0x00F0);
      }
      glDrawElements(GL_LINES, data.array_polygons_size, GL_UNSIGNED_INT, data.array_polygons);
      if (typeEdges == 1) {
        glDisable(GL_LINE_STIPPLE);
      }
      glDisableClientState(GL_VERTEX_ARRAY);
  } else {
    rm_data(&data);
  }
}

