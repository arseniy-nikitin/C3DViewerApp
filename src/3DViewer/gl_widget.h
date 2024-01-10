#ifndef GL_WIDGET_H
#define GL_WIDGET_H

#define GL_SILENCE_DEPRECATION

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QWidget>
#include <QtOpenGL>

#include "settings.h"

class GL_Widget : public QOpenGLWidget, protected QOpenGLExtraFunctions {
  Q_OBJECT

 public:
  GL_Widget(QWidget* parent = nullptr);
  ~GL_Widget();

  // Process new obj
  void initObj(QString qFileDir);

  // Affine transformations
  void moveX(float val) { move_x(&vertexesData, val); };
  void moveY(float val) { move_y(&vertexesData, val); };
  void moveZ(float val) { move_z(&vertexesData, val); };
  void rotateX(float val) { rotate_x(&vertexesData, val); };
  void rotateY(float val) { rotate_y(&vertexesData, val); };
  void rotateZ(float val) { rotate_z(&vertexesData, val); };

 protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

 private:
  // Get settings data
  Settings& settings = Settings::getInstance();

  // Declare structs
  vertexes_data vertexesData{};
  lines_data linesData{};
  count objEdgeValues{};

  // Auxiliary methods for render
  void updateProjectionMatrix(int w, int h);
  int screenWidth{}, screenHeight{};
  void drawLines();
  void drawPoints();
  float getObjAbs();
};
#endif  // GL_WIDGET_H
