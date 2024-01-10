#include "widget.h"

#include "ui_widget.h"

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);

  // Load settings
  settings.loadSettings();
  refreshAppSettingsElementsData();

  // Obj button signal
  connect(ui->pushButton_import_obj, &QPushButton::clicked, this,
          &Widget::importObj);

  // Img button signal
  connect(ui->pushButton_export_img, &QPushButton::clicked, this,
          &Widget::exportImg);

  // Gif button signals
  connect(ui->pushButton_export_gif, &QPushButton::clicked, this,
          &Widget::exportGif);
  connect(gifTimer, SIGNAL(timeout()), this, SLOT(createGif()));

  // Vector spinboxes signals
  connect(ui->doubleSpinBox_camera_position_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changePositionVector);
  connect(ui->doubleSpinBox_camera_position_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changePositionVector);
  connect(ui->doubleSpinBox_camera_position_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changePositionVector);
  connect(ui->doubleSpinBox_camera_rotation_x,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeRotationVector);
  connect(ui->doubleSpinBox_camera_rotation_y,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeRotationVector);
  connect(ui->doubleSpinBox_camera_rotation_z,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeRotationVector);

  // Scale spinbox signal
  connect(ui->doubleSpinBox_camera_scale,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeScale);

  // Type buttons signals
  connect(ui->pushButton_scene_projection_type, &QPushButton::clicked, this,
          &Widget::changeProjectionType);
  connect(ui->pushButton_edges_type, &QPushButton::clicked, this,
          &Widget::changeEdgesType);
  connect(ui->pushButton_vertexes_type, &QPushButton::clicked, this,
          &Widget::changeVertexesType);

  // Color buttons signals
  connect(ui->colorPushButton_scene_background_color, &ColorPushButton::clicked,
          this, &Widget::changeColors);
  connect(ui->colorPushButton_edges_color, &ColorPushButton::clicked, this,
          &Widget::changeColors);
  connect(ui->colorPushButton_vertexes_color, &ColorPushButton::clicked, this,
          &Widget::changeColors);

  // Size spinboxes signals
  connect(ui->doubleSpinBox_edges_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeSizes);
  connect(ui->doubleSpinBox_vertexes_size,
          QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
          &Widget::changeSizes);
}

Widget::~Widget() { delete ui; }

void Widget::refreshAppSettingsElementsData() {
  // Refresh type buttons data
  if (settings.getProjectionType() == 0)
    ui->pushButton_scene_projection_type->setText("PARALLEL");
  else if (settings.getProjectionType() == 1)
    ui->pushButton_scene_projection_type->setText("CENTRAL");
  if (settings.getEdgesType() == 0)
    ui->pushButton_edges_type->setText("SOLID");
  else if (settings.getEdgesType() == 1)
    ui->pushButton_edges_type->setText("DASHED");
  if (settings.getVertexesType() == 0)
    ui->pushButton_vertexes_type->setText("NONE");
  else if (settings.getVertexesType() == 1)
    ui->pushButton_vertexes_type->setText("CIRCLE");
  else if (settings.getVertexesType() == 2)
    ui->pushButton_vertexes_type->setText("SQUARE");

  // Refresh color buttons data
  ui->colorPushButton_scene_background_color->updateData(
      settings.getBackgroundColor());
  ui->colorPushButton_edges_color->updateData(settings.getEdgesColor());
  ui->colorPushButton_vertexes_color->updateData(settings.getVertexesColor());

  // Refresh spinboxes data
  ui->doubleSpinBox_edges_size->setValue(
      static_cast<double>(settings.getEdgesSize()));
  ui->doubleSpinBox_vertexes_size->setValue(
      static_cast<double>(settings.getVertexesSize()));
}

void Widget::clearObjSettings() {
  ui->doubleSpinBox_camera_position_x->setValue(0.0);
  settings.setPositionX(0.0);
  ui->doubleSpinBox_camera_position_y->setValue(0.0);
  settings.setPositionY(0.0);
  ui->doubleSpinBox_camera_position_z->setValue(0.0);
  settings.setPositionZ(0.0);
  ui->doubleSpinBox_camera_rotation_x->setValue(0.0);
  settings.setRotationX(0.0);
  ui->doubleSpinBox_camera_rotation_y->setValue(0.0);
  settings.setRotationY(0.0);
  ui->doubleSpinBox_camera_rotation_z->setValue(0.0);
  settings.setRotationZ(0.0);
}

void Widget::refreshObjInfo() {
  QFileInfo fileInfo(settings.getObjDirectory());
  ui->label_changable_file_name->setText(fileInfo.fileName());
  ui->label_changable_vertexes_num->setText(
      QString::number(settings.getObjVertexes()));
  ui->label_changable_polygons_num->setText(
      QString::number(settings.getObjEdges()));
}

void Widget::importObj() {
  settings.setObjDirectory((QFileDialog::getOpenFileName(
      this, tr("Pick an obj file"), tr("obj (*.obj)"))));
  if (!settings.getObjDirectory().isEmpty()) {
    clearObjSettings();
    ui->openGLWidget->initObj(settings.getObjDirectory());
    refreshObjInfo();
  }
}

void Widget::exportImg() {
  QFileDialog dialogConnectImage(this);
  QDateTime currentDate = QDateTime::currentDateTime();
  QString formattedTime = currentDate.toString("yyyy-MM-dd-hh.mm.ss");
  QString namePattern = "screenshot-" + formattedTime + ".jpeg";
  QString imgName = dialogConnectImage.getSaveFileName(
      this, tr("Save a screenshot"), namePattern, tr("image (*.jpeg *.bmp)"));
  if (imgName != "") {
    QImage img = ui->openGLWidget->grabFramebuffer();
    img.save(imgName);
  }
}

void Widget::exportGif() {
  QDateTime currentDate = QDateTime::currentDateTime();
  QString formattedTime = currentDate.toString("yyyy-MM-dd-hh.mm.ss");
  QString namePattern = "screencast-" + formattedTime + ".gif";
  gifName = QFileDialog::getSaveFileName(this, tr("Save a screencast"),
                                         namePattern, tr("gif (*.gif)"));
  if (gifName != "") {
    ui->pushButton_export_gif->setDisabled(true);
    gifFrame = new QGifImage;
    gifFrame->setDefaultDelay(settings.gifDelayMS);
    gifTimer->setInterval(settings.gifFrameIntervalMS);
    gifTimer->start();
  }
}

void Widget::createGif() {
  QImage img = ui->openGLWidget->grabFramebuffer();
  QSize imgSize(settings.gifWidth, settings.gifHeight);
  QImage scaledImg = img.scaled(imgSize);
  gifFrame->addFrame(scaledImg);
  if (frameCounter <= 0) {
    gifTimer->stop();
    gifFrame->save(gifName);
    frameCounter = settings.gifTotalFrames;
    delete gifFrame;
    ui->pushButton_export_gif->setText("GIF");
    ui->pushButton_export_gif->setEnabled(true);
  }
  frameCounter--;
  if (!ui->pushButton_export_gif->isEnabled()) {
    ui->pushButton_export_gif->setText(QString::number(frameCounter / 10 + 1) +
                                       "S");
  }
}

void Widget::changePositionVector(double val) {
  float floatVal = static_cast<float>(val);
  if (QObject::sender() == ui->doubleSpinBox_camera_position_x) {
    ui->openGLWidget->moveX(floatVal - settings.getPositionX());
    settings.setPositionX(floatVal);
  } else if (QObject::sender() == ui->doubleSpinBox_camera_position_y) {
    ui->openGLWidget->moveY(floatVal - settings.getPositionY());
    settings.setPositionY(floatVal);
  } else if (QObject::sender() == ui->doubleSpinBox_camera_position_z) {
    ui->openGLWidget->moveZ(floatVal - settings.getPositionZ());
    settings.setPositionZ(floatVal);
  }
  ui->openGLWidget->update();
}

void Widget::changeRotationVector(double val) {
  float floatVal = static_cast<float>(val);
  if (QObject::sender() == ui->doubleSpinBox_camera_rotation_x) {
    ui->openGLWidget->rotateX(floatVal - settings.getRotationX());
    settings.setRotationX(floatVal);
  } else if (QObject::sender() == ui->doubleSpinBox_camera_rotation_y) {
    ui->openGLWidget->rotateY(floatVal - settings.getRotationY());
    settings.setRotationY(floatVal);
  } else if (QObject::sender() == ui->doubleSpinBox_camera_rotation_z) {
    ui->openGLWidget->rotateZ(floatVal - settings.getRotationZ());
    settings.setRotationZ(floatVal);
  }
  ui->openGLWidget->update();
}

void Widget::changeScale(double val) {
  float floatVal = static_cast<float>(val);
  settings.setScale(floatVal);
  ui->openGLWidget->update();
}

void Widget::changeProjectionType() {
  QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
  if (senderButton) {
    QString projectionType = senderButton->text();
    if (projectionType == "PARALLEL") {
      settings.setProjectionType(1);
      senderButton->setText("CENTRAL");
    } else if (projectionType == "CENTRAL") {
      settings.setProjectionType(0);
      senderButton->setText("PARALLEL");
    }
  }
  settings.saveSettings();
  ui->openGLWidget->update();
}

void Widget::changeEdgesType() {
  QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
  if (senderButton) {
    QString projectionType = senderButton->text();
    if (projectionType == "SOLID") {
      settings.setEdgesType(1);
      senderButton->setText("DASHED");
    } else if (projectionType == "DASHED") {
      settings.setEdgesType(0);
      senderButton->setText("SOLID");
    }
  }
  settings.saveSettings();
  ui->openGLWidget->update();
}

void Widget::changeVertexesType() {
  QPushButton* senderButton = qobject_cast<QPushButton*>(sender());
  if (senderButton) {
    QString projectionType = senderButton->text();
    if (projectionType == "NONE") {
      settings.setVertexesType(1);
      senderButton->setText("CIRCLE");
    } else if (projectionType == "CIRCLE") {
      settings.setVertexesType(2);
      senderButton->setText("SQUARE");
    } else if (projectionType == "SQUARE") {
      settings.setVertexesType(0);
      senderButton->setText("NONE");
    }
  }
  settings.saveSettings();
  ui->openGLWidget->update();
}

void Widget::changeColors() {
  ColorPushButton* senderButton = qobject_cast<ColorPushButton*>(sender());
  if (senderButton) {
    QString hexCode = senderButton->text();
    if (senderButton == ui->colorPushButton_scene_background_color) {
      settings.setBackgroundColor(QColor(hexCode));
    } else if (senderButton == ui->colorPushButton_edges_color) {
      settings.setEdgesColor(QColor(hexCode));
    } else if (senderButton == ui->colorPushButton_vertexes_color) {
      settings.setVertexesColor(QColor(hexCode));
    }
  }
  settings.saveSettings();
  ui->openGLWidget->update();
}

void Widget::changeSizes(double val) {
  float floatVal = static_cast<float>(val);
  if (QObject::sender() == ui->doubleSpinBox_edges_size) {
    settings.setEdgesSize(floatVal);
  } else if (QObject::sender() == ui->doubleSpinBox_vertexes_size) {
    settings.setVertexesSize(floatVal);
  }
  settings.saveSettings();
  ui->openGLWidget->update();
}
