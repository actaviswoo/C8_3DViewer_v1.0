#ifndef GLWIDGET_H
#define GLWIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QColor>
#include <QColorDialog>
#include <QOpenGLWidget>
#include <QPushButton>
#include <algorithm>
#include <cstring>

using namespace std;
extern "C" {
#include "../Project/3DViewer.h"
}

class GLWidget : public QOpenGLWidget {
  Q_OBJECT
 public:
  GLWidget(QWidget *parent = nullptr);
  ~GLWidget();
  QColor backgroundColor;
  QColor lineColor;
  QColor dotColor;

  int projectionMode = 0;
  int typeVertcies = 0;
  int typeEdges = 0;
  int pSize = 1;
  int lwSize = 1;
  data_t data = { 0, 0, NULL, NULL, NULL, 0, NULL, 0 };
  void initSettings();
  void setutProjection();

 protected:

 void initializeGL() override;
 void resizeGL(int w, int h) override;
 void paintGL() override;
};

#endif // GLWIDGET_H
