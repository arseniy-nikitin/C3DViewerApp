#ifndef WIDGET_H
#define WIDGET_H

#include <qgifimage.h>

#include <QFileDialog>
#include <QMouseEvent>
#include <QWidget>

#include "gl_widget.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(QWidget *parent = nullptr);
  ~Widget();

 public slots:
  void clearObjSettings();
  void refreshAppSettingsElementsData();
  void importObj();
  void exportImg();
  void exportGif();
  void createGif();
  void changePositionVector(double val);
  void changeRotationVector(double val);
  void changeScale(double val);
  void changeSizes(double val);
  void changeProjectionType();
  void changeEdgesType();
  void changeVertexesType();
  void changeColors();

 public:
  void refreshObjInfo();

 private:
  Ui::Widget *ui;

  // Get settings data
  Settings &settings = Settings::getInstance();

  // Gif local params
  QTimer *gifTimer = new QTimer(0);
  QGifImage *gifFrame;
  QString gifName;
  int frameCounter = 50;
};
#endif  // WIDGET_H
