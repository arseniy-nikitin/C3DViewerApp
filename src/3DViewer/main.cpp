#include <QApplication>

#include "widget.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QFont defaultFont("Helvetica", 14);
  a.setFont(defaultFont);
  a.setStyleSheet("QWidget{}");
  Widget w;
  w.show();
  return a.exec();
}
