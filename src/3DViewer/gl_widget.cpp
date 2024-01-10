#include "gl_widget.h"

GL_Widget::GL_Widget(QWidget *parent) : QOpenGLWidget(parent) {}

GL_Widget::~GL_Widget() {
  delete_struct(&vertexesData, &linesData, &objEdgeValues);
}

void GL_Widget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
}

void GL_Widget::resizeGL(int w, int h) {
  screenWidth = w;
  screenHeight = h;
  glViewport(0, 0, w, h);
  updateProjectionMatrix(w, h);
}

void GL_Widget::updateProjectionMatrix(int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  float aspectRatio = static_cast<float>(w) / static_cast<float>(h);
  float objAbsVal = getObjAbs();
  float halfObjSize = objAbsVal * 0.5f;
  float scaleFactor = settings.getScale();

  if (settings.getProjectionType() == 0) {
    float frustumFactor = scaleFactor * 100;
    float nearPlane = 0.1f;
    float farPlane = 100000.0f;
    float fov = 45.0f;
    float top = halfObjSize * tan(fov * 0.5f * (M_PI / 180.0f)) / frustumFactor;
    float right = top * aspectRatio;

    glFrustum(-right, right, -top, top, nearPlane, farPlane);
  } else if (settings.getProjectionType() == 1) {
    float ortoFactor = halfObjSize / (scaleFactor / 62);
    if (w <= h) {
      glOrtho(-ortoFactor * aspectRatio, ortoFactor * aspectRatio, -ortoFactor,
              ortoFactor, 0.1f, 1000000.0f);
    } else {
      glOrtho(-ortoFactor, ortoFactor, -ortoFactor / aspectRatio,
              ortoFactor / aspectRatio, 0.1f, 1000000.0f);
    }
  }

  glTranslatef(0, 0, -1000);

  glMatrixMode(GL_MODELVIEW);
}

void GL_Widget::paintGL() {
  QColor backgroundColor = settings.getBackgroundColor();
  glClearColor(backgroundColor.redF(), backgroundColor.greenF(),
               backgroundColor.blueF(), 1.0f);

  updateProjectionMatrix(screenWidth, screenHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glVertexPointer(3, GL_FLOAT, 0, vertexesData.vertexes_list);

  glEnableClientState(GL_VERTEX_ARRAY);

  if (settings.getVertexesType()) {
    drawPoints();
  }
  drawLines();

  glDisableClientState(GL_VERTEX_ARRAY);
}

void GL_Widget::drawLines() {
  if (settings.getEdgesType() == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  glLineWidth(settings.getEdgesSize());
  QColor edgesColor = settings.getEdgesColor();
  glColor3f(edgesColor.redF(), edgesColor.greenF(), edgesColor.blueF());
  glDrawElements(GL_LINES, linesData.line_count, GL_UNSIGNED_INT,
                 linesData.lines_list);
  if (settings.getEdgesType() == 1) {
    glDisable(GL_LINE_STIPPLE);
  }
}

void GL_Widget::drawPoints() {
  if (settings.getVertexesType() == 1) {
    glEnable(GL_POINT_SMOOTH);
  }
  glPointSize(settings.getVertexesSize());
  QColor vertexesColor = settings.getVertexesColor();
  glColor3f(vertexesColor.redF(), vertexesColor.greenF(),
            vertexesColor.blueF());
  glDrawArrays(GL_POINTS, 0, vertexesData.vertex_count);
  if (settings.getVertexesType() == 1) {
    glDisable(GL_POINT_SMOOTH);
  }
}

void GL_Widget::initObj(QString qFileDir) {
  delete_struct(&vertexesData, &linesData, &objEdgeValues);
  QByteArray formatedFileDir = qFileDir.toUtf8();
  if (!read_file(formatedFileDir.data(), &vertexesData, &linesData,
                 &objEdgeValues)) {
    settings.setObjVertexes(vertexesData.vertex_count);
    settings.setObjEdges(linesData.line_count);
  }
  update();
}

float GL_Widget::getObjAbs() {
  float abs = objEdgeValues.xmax;
  if (abs < objEdgeValues.ymax) {
    abs = objEdgeValues.ymax;
  } else if (abs < objEdgeValues.zmax) {
    abs = objEdgeValues.zmax;
  }
  return abs;
}
