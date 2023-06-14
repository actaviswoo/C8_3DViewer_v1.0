#include "mainwindow.h"

#include "glwidget.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("3DViewer");
  if (CheckConfig()) {
    RestoreFromConfig();
  } else {
    ui->BgFrame->setStyleSheet("QFrame {background-color: black;}");
    ui->EdgeFrame->setStyleSheet("QFrame {background-color: white;}");
    ui->VertexFrame->setStyleSheet("QFrame {background-color: white;}");
  }
  ui->saveButton->setEnabled(0);
  ui->RecordButton->setEnabled(0);
  timer = new QTimer;
  connect(timer, SIGNAL(timeout()), this, SLOT(TimerAction()));
  gifImage = new QImage[50]{};
}

MainWindow::~MainWindow() {
  delete ui;
  delete[] gifImage;
  delete timer;
}

void MainWindow::connectSetup() {
  //   Open file
  connect(ui->open_button, &QPushButton::pressed, (this),
          &MainWindow::on_action_triggered);
}

bool MainWindow::CheckConfig() {
  bool res = false;
  ifstream myfile;
  char c = 0;
  int n = 0;
  myfile.open("../../../../config/settings.config");
  if (myfile.is_open()) {
    while (myfile.get(c)) {
      n++;
    }
    myfile.close();
  }
  if (n > 0) {
    res = true;
  }
  return res;
}

void MainWindow::RestoreFromConfig() {
  ifstream myfile;
  string line;
  myfile.open("../../../../config/settings.config");
  getline(myfile, line);
  projection_status = line + '\n';
  if (line.find("orthogonal") != std::string::npos) {
    on_orthogonal_clicked();
    ui->orthogonal->setChecked(1);
  } else {
    on_perspective_clicked();
    ui->perspective->setChecked(1);
  }
  getline(myfile, line);
  edgetype_status = line + '\n';
  if (line.find("solid") != std::string::npos) {
    on_comboBox_2_currentIndexChanged(0);
  } else {
    on_comboBox_2_currentIndexChanged(1);
  }
  ui->comboBox_2->setCurrentText(QString::fromLocal8Bit(line.data()));
  getline(myfile, line);
  edgeColor_status = line + '\n';
  QColor color;
  color.setNamedColor(QString::fromLocal8Bit(line.data()));
  QString style = QString("background-color: %1").arg(color.name());
  ui->EdgeFrame->setStyleSheet(style);
  ui->OGLWidget->lineColor.setRgb(color.red(), color.green(), color.blue());
  getline(myfile, line);
  edgeThickness_status = line + '\n';
  on_rollBar_edges_valueChanged(stoi(line));
  getline(myfile, line);
  vertexType_status = line + '\n';
  if (line.find("circle") != std::string::npos) {
    on_comboBox_1_currentIndexChanged(0);
  } else if (line.find("square") != std::string::npos) {
    on_comboBox_1_currentIndexChanged(1);
  } else {
    on_comboBox_1_currentIndexChanged(2);
  }
  ui->comboBox_1->setCurrentText(QString::fromLocal8Bit(line.data()));
  getline(myfile, line);
  vertexColor_status = line + '\n';
  color.setNamedColor(QString::fromLocal8Bit(line.data()));
  style = QString("background-color: %1").arg(color.name());
  ui->VertexFrame->setStyleSheet(style);
  ui->OGLWidget->dotColor.setRgb(color.red(), color.green(), color.blue());
  getline(myfile, line);
  vertexSize_status = line + '\n';
  on_rollBar_vertcies_valueChanged(stoi(line));
  getline(myfile, line);
  backgroundColor_status = line;
  color.setNamedColor(QString::fromLocal8Bit(line.data()));
  style = QString("background-color: %1").arg(color.name());
  ui->BgFrame->setStyleSheet(style);
  ui->OGLWidget->backgroundColor.setRgb(color.red(), color.green(),
                                        color.blue());
}

void MainWindow::on_action_triggered() {
  QFileDialog *fileDialog = new QFileDialog(this);
  if (fileDialog) {
    fileDialog->setWindowTitle(tr("Choose .obj-file"));
    fileDialog->setNameFilter(tr("(*.obj)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    QStringList fileNames;
    if (fileDialog->exec()) {
      fileNames = fileDialog->selectedFiles();
      if (fileNames.size() > 0) {
        QString fileName = fileNames.at(0);
        QByteArray ba = fileName.toLocal8Bit();
        char *input = ba.data();
        rm_data(&ui->OGLWidget->data);
        if (parse(input, &ui->OGLWidget->data)) {
          setWindowTitle(fileName);
          ui->statusBar->showMessage("Path: " + fileName);
        } else {
          rm_data(&ui->OGLWidget->data);
        }
      }
    }
    delete fileDialog;
  } else {
  }
}

void MainWindow::on_open_button_clicked() {
  QFileDialog *fileDialog = new QFileDialog(this);
  if (fileDialog) {
    fileDialog->setWindowTitle(tr("Choose .obj-file"));
    fileDialog->setNameFilter(tr("(*.obj)"));
    fileDialog->setViewMode(QFileDialog::Detail);
    fileDialog->setFileMode(QFileDialog::ExistingFile);
    QStringList fileNames;
    if (fileDialog->exec()) {
      fileNames = fileDialog->selectedFiles();
      if (fileNames.size() > 0) {
        QString fileName = fileNames.at(0);
        QByteArray ba = fileName.toLocal8Bit();
        char *input = ba.data();
        rm_data(&ui->OGLWidget->data);
        if (parse(input, &ui->OGLWidget->data) == 0) {
          on_default_button_clicked();
          setWindowTitle(fileName);
          ui->Full_path->setText("Full path: " + fileName);
          ui->Name->setText("Name: " + QFileInfo(fileName).fileName());
          ui->Vertcies->setText(
              "Vertcies: " +
              QString::number(ui->OGLWidget->data.count_of_vertexes));
          ui->Edges->setText(
              "Edges: " + QString::number(ui->OGLWidget->data.count_of_facets));
          ui->saveButton->setEnabled(1);
          ui->RecordButton->setEnabled(1);
        } else {
          rm_data(&ui->OGLWidget->data);
        }
      }
    }
    delete fileDialog;
  } else {
  }
}

void MainWindow::on_rollBar_translation_x_valueChanged(int value) {
  double old_val = ui->spinBox_translation_x->value();
  ui->spinBox_translation_x->setValue(value);
  move_x(&ui->OGLWidget->data,
         (value - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_rollBar_translation_y_valueChanged(int value) {
  double old_val = ui->spinBox_translation_y->value();
  ui->spinBox_translation_y->setValue(value);
  move_y(&ui->OGLWidget->data,
         (value - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_rollBar_translation_z_valueChanged(int value) {
  double old_val = ui->spinBox_translation_z->value();
  ui->spinBox_translation_z->setValue(value);
  move_z(&ui->OGLWidget->data,
         (value - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_dial_x_valueChanged(int value) {
  double old_val = ui->spinBox_rotate_x->value();
  ui->spinBox_rotate_x->setValue(value);
  rotation_by_ox(&ui->OGLWidget->data, old_val, value);
  ui->OGLWidget->update();
}

void MainWindow::on_dial_y_valueChanged(int value) {
  double old_val = ui->spinBox_rotate_y->value();
  ui->spinBox_rotate_y->setValue(value);
  rotation_by_oy(&ui->OGLWidget->data, old_val, value);
  ui->OGLWidget->update();
}

void MainWindow::on_dial_z_valueChanged(int value) {
  double old_val = ui->spinBox_rotate_z->value();
  ui->spinBox_rotate_z->setValue(value);
  rotation_by_oz(&ui->OGLWidget->data, old_val, value);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_translation_x_valueChanged(int arg1) {
  double old_val = ui->rollBar_translation_x->value();
  ui->rollBar_translation_x->setValue(arg1);
  move_x(&ui->OGLWidget->data,
         (arg1 - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_translation_y_valueChanged(int arg1) {
  double old_val = ui->rollBar_translation_y->value();
  ui->rollBar_translation_y->setValue(arg1);
  move_y(&ui->OGLWidget->data,
         (arg1 - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_translation_z_valueChanged(int arg1) {
  double old_val = ui->rollBar_translation_z->value();
  ui->rollBar_translation_z->setValue(arg1);
  move_z(&ui->OGLWidget->data,
         (arg1 - old_val) * ui->rollBar_scale->value() / 1500);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_rotate_x_valueChanged(int arg1) {
  double old_val = ui->dial_x->value();
  ui->dial_x->setValue(arg1);
  rotation_by_ox(&ui->OGLWidget->data, old_val, arg1);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_rotate_y_valueChanged(int arg1) {
  double old_val = ui->dial_y->value();
  ui->dial_y->setValue(arg1);
  rotation_by_oy(&ui->OGLWidget->data, old_val, arg1);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_rotate_z_valueChanged(int arg1) {
  double old_val = ui->dial_z->value();
  ui->dial_z->setValue(arg1);
  rotation_by_oz(&ui->OGLWidget->data, old_val, arg1);
  ui->OGLWidget->update();
}

void MainWindow::on_rollBar_scale_valueChanged(int value) {
  double val = (double)value / 100;
  double old_val = ui->spinBox_scale->value() / 100.0;
  ui->spinBox_scale->setValue(value);
  scale_x(&ui->OGLWidget->data, val / old_val);
  scale_y(&ui->OGLWidget->data, val / old_val);
  scale_z(&ui->OGLWidget->data, val / old_val);
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_scale_valueChanged(int arg1) {
  double val = (double)arg1 / 100;
  double old_val = ui->rollBar_scale->value() / 100.0;
  ui->rollBar_scale->setValue(arg1);
  scale_x(&ui->OGLWidget->data, val / old_val);
  scale_y(&ui->OGLWidget->data, val / old_val);
  scale_z(&ui->OGLWidget->data, val / old_val);
  ui->OGLWidget->update();
}

void MainWindow::on_perspective_clicked() {
  ui->OGLWidget->projectionMode = 1;
  projection_status = "perspective\n";
  ui->OGLWidget->update();
}

void MainWindow::on_orthogonal_clicked() {
  ui->OGLWidget->projectionMode = 0;
  projection_status = "orthogonal\n";
  ui->OGLWidget->update();
}

void MainWindow::on_default_button_clicked() {
  ui->spinBox_translation_x->setValue(0);
  ui->spinBox_translation_y->setValue(0);
  ui->spinBox_translation_z->setValue(0);
  ui->spinBox_rotate_x->setValue(0);
  ui->spinBox_rotate_y->setValue(0);
  ui->spinBox_rotate_z->setValue(0);
  ui->rollBar_translation_x->setValue(0);
  ui->rollBar_translation_y->setValue(0);
  ui->rollBar_translation_z->setValue(0);
  ui->dial_x->setValue(0);
  ui->dial_y->setValue(0);
  ui->dial_z->setValue(0);
  ui->spinBox_scale->setValue(0);
  ui->rollBar_scale->setValue(100);
  ui->perspective->setChecked(0);
  ui->OGLWidget->projectionMode = 0;
  ui->orthogonal->setChecked(1);
  ui->comboBox_1->setCurrentIndex(0);
  ui->OGLWidget->typeVertcies = 0;
  ui->comboBox_2->setCurrentIndex(0);
  ui->OGLWidget->typeEdges = 0;
  ui->rollBar_vertcies->setMinimum(1);
  ui->rollBar_vertcies->setMaximum(1);
  ui->comboBox_1->setCurrentIndex(2);
  ui->OGLWidget->typeVertcies = 2;
  for (int i = 0; i < ui->OGLWidget->data.count_of_vertexes; ++i)
    for (int j = 0; j < COORDINATES_COUNT; ++j)
      ui->OGLWidget->data.matrix_3d[i][j] =
          ui->OGLWidget->data.matrix_3d_original[i][j];
  ui->OGLWidget->update();
}

void MainWindow::on_comboBox_1_currentIndexChanged(int index) {
  if (index == 0) {
    ui->OGLWidget->typeVertcies = 0;
    ui->rollBar_vertcies->setMinimum(1);
    ui->rollBar_vertcies->setMaximum(10);
    ui->spinBox_vertcies->setMinimum(1);
    ui->spinBox_vertcies->setMaximum(10);
    vertexType_status = "circle\n";
  } else if (index == 1) {
    ui->OGLWidget->typeVertcies = 1;
    ui->rollBar_vertcies->setMinimum(1);
    ui->rollBar_vertcies->setMaximum(10);
    ui->spinBox_vertcies->setMinimum(1);
    ui->spinBox_vertcies->setMaximum(10);
    vertexType_status = "square\n";
  } else if (index == 2) {
    ui->OGLWidget->typeVertcies = 2;
    ui->rollBar_vertcies->setMinimum(1);
    ui->rollBar_vertcies->setMaximum(1);
    vertexType_status = "disable\n";
  }
  ui->OGLWidget->update();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int index) {
  if (index == 0) {
    ui->OGLWidget->typeEdges = 0;
    edgetype_status = "solid\n";
  } else if (index == 1) {
    ui->OGLWidget->typeEdges = 1;
    edgetype_status = "dashed\n";
  }
  ui->OGLWidget->update();
}

void MainWindow::on_rollBar_vertcies_valueChanged(int value) {
  ui->spinBox_vertcies->setValue(value);
  ui->OGLWidget->pSize = ui->rollBar_vertcies->value();
  vertexSize_status = QString::number(value).toStdString() + '\n';
  ui->OGLWidget->update();
}

void MainWindow::on_rollBar_edges_valueChanged(int value) {
  ui->spinBox_edges->setValue(value);
  ui->OGLWidget->lwSize = ui->rollBar_edges->value();
  edgeThickness_status = QString::number(value).toStdString() + '\n';
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_vertcies_valueChanged(int arg1) {
  ui->rollBar_vertcies->setValue(arg1);
  ui->OGLWidget->pSize = ui->spinBox_vertcies->value();
  vertexSize_status = QString::number(arg1).toStdString() + '\n';
  ui->OGLWidget->update();
}

void MainWindow::on_spinBox_edges_valueChanged(int arg1) {
  ui->rollBar_edges->setValue(arg1);
  ui->OGLWidget->lwSize = ui->spinBox_edges->value();
  edgeThickness_status = QString::number(arg1).toStdString() + '\n';
  ui->OGLWidget->update();
}

void MainWindow::on_BgButton_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor(Qt::white, this, tr("Select color"));
  if (color.isValid()) {
    QString style = QString("background-color: %1").arg(color.name());
    ui->BgFrame->setStyleSheet(style);
    ui->OGLWidget->backgroundColor.setRgb(color.red(), color.green(),
                                          color.blue());
    backgroundColor_status = color.name().toStdString() + '\n';
    ui->OGLWidget->update();
  }
}

void MainWindow::on_EdgeButton_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor(Qt::white, this, tr("Select color"));
  if (color.isValid()) {
    QString style = QString("background-color: %1").arg(color.name());
    ui->EdgeFrame->setStyleSheet(style);
    ui->OGLWidget->lineColor.setRgb(color.red(), color.green(), color.blue());
    edgeColor_status = color.name().toStdString() + '\n';
    ui->OGLWidget->update();
  }
}

void MainWindow::on_VertexButton_clicked() {
  QColorDialog dialog(this);
  QColor color = dialog.getColor(Qt::white, this, tr("Select color"));
  if (color.isValid()) {
    QString style = QString("background-color: %1").arg(color.name());
    ui->VertexFrame->setStyleSheet(style);
    ui->OGLWidget->dotColor.setRgb(color.red(), color.green(), color.blue());
    vertexColor_status = color.name().toStdString() + '\n';
    ui->OGLWidget->update();
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  ofstream myfile("../../../../config/settings.config");
  if (myfile.is_open()) {
    myfile << projection_status + edgetype_status + edgeColor_status +
                  edgeThickness_status + vertexType_status +
                  vertexColor_status + vertexSize_status +
                  backgroundColor_status;
    myfile.close();
  }
  event->accept();
}

void MainWindow::on_saveButton_clicked() { SaveImage(); }

void MainWindow::SaveImage() {
  ifstream file;
  int screennumb = 0;
  QString format = ui->formatBox->currentText();
  file.open("../../../../Images/screenshot" +
            QString::number(screennumb).toStdString() + format.toStdString());
  while (file.is_open()) {
    file.close();
    screennumb++;
    file.open("../../../../Images/screenshot" +
              QString::number(screennumb).toStdString() + format.toStdString());
  }
  file.close();
  QImage image = ui->OGLWidget->grabFramebuffer();
  QString filePath =
      "../../../../Images/screenshot" + QString::number(screennumb) + format;
  QString fm;
  if (format == "jpeg") {
    fm = "JPG";
  } else {
    fm = "BMP";
  }
  bool success = image.save(filePath, fm.toStdString().data(), 80);
  if (!success) {
    qDebug() << "Failed to save image to file!";
  } else {
    QMessageBox::information(this, "Screenshot", "Screenshot saved",
                             QMessageBox::Button::Ok);
  }
}

void MainWindow::on_RecordButton_clicked() {
  timer->start(100);
  ui->RecordButton->setEnabled(0);
}

void MainWindow::TimerAction() {
  if (giftime < 50) {
    gifImage[giftime] = ui->OGLWidget->grab().toImage();
    giftime++;
  } else {
    gif = new QGifImage;
    for (int i = 0; i < giftime; i++) {
      gif->addFrame(gifImage[i], 0);
    }
    ifstream file;
    int screennumb = 0;
    file.open("../../../../Images/animation" +
              QString::number(screennumb).toStdString() + ".gif");
    while (file.is_open()) {
      file.close();
      screennumb++;
      file.open("../../../../Images/animation" +
                QString::number(screennumb).toStdString() + ".gif");
    }
    file.close();
    gif->save("../../../../Images/animation" + QString::number(screennumb) +
              ".gif");
    timer->stop();
    ui->RecordButton->setEnabled(1);
    delete gif;
    giftime = 0;
  }
}
