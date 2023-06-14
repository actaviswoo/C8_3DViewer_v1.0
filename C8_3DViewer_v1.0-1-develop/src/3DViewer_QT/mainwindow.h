#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdlib.h>

#include <QBuffer>
#include <QCloseEvent>
#include <QColor>
#include <QColorDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QMainWindow>
#include <QMessageBox>
#include <QMovie>
#include <QSettings>
#include <QTimer>
#include <cstring>
#include <fstream>
#include <string>

#include "glwidget.h"
#include "qgifimage.h"

extern "C" {
#include "../Project/3DViewer.h"
}

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_action_triggered();

  void on_open_button_clicked();

  void on_rollBar_translation_x_valueChanged(int value);

  void on_rollBar_translation_y_valueChanged(int value);

  void on_rollBar_translation_z_valueChanged(int value);

  void on_dial_x_valueChanged(int value);

  void on_dial_y_valueChanged(int value);

  void on_dial_z_valueChanged(int value);

  void on_spinBox_translation_x_valueChanged(int arg1);

  void on_spinBox_translation_y_valueChanged(int arg1);

  void on_spinBox_translation_z_valueChanged(int arg1);

  void on_spinBox_rotate_x_valueChanged(int arg1);

  void on_spinBox_rotate_y_valueChanged(int arg1);

  void on_spinBox_rotate_z_valueChanged(int arg1);

  void on_rollBar_scale_valueChanged(int value);

  void on_spinBox_scale_valueChanged(int arg1);

  void on_perspective_clicked();

  void on_orthogonal_clicked();

  void on_default_button_clicked();

  void on_comboBox_1_currentIndexChanged(int index);

  void on_comboBox_2_currentIndexChanged(int index);

  void on_rollBar_vertcies_valueChanged(int value);

  void on_rollBar_edges_valueChanged(int value);

  void on_spinBox_vertcies_valueChanged(int arg1);

  void on_spinBox_edges_valueChanged(int arg1);

  void on_BgButton_clicked();

  void on_EdgeButton_clicked();

  void on_VertexButton_clicked();

  void closeEvent(QCloseEvent *event);

  bool CheckConfig();

  void RestoreFromConfig();

  void on_saveButton_clicked();

  void SaveImage();

  void on_RecordButton_clicked();

  void TimerAction();

 private:
  Ui::MainWindow *ui;
  QString settingFile;
  void connectSetup();
  string projection_status = "orthogonal\n";
  string edgetype_status = "solid\n";
  string edgeColor_status = "white\n";
  string edgeThickness_status = "1\n";
  string vertexType_status = "disable\n";
  string vertexColor_status = "white\n";
  string vertexSize_status = "0\n";
  string backgroundColor_status = "black\n";
  QImage *gifImage;
  QTimer *timer;
  QGifImage *gif;
  int giftime = 0;
};

#endif  // MAINWINDOW_H
