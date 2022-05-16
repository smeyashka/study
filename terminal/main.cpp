#include <QApplication>
#include <QMainWindow>
#include <QDebug>

#include "terminal.h"


int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  try {
    Terminal gui;
    gui.show();

    app.exec();
  }
  catch(const std::bad_alloc&) {
    qDebug() << QObject::tr("Not enough memory to run the application");
  }
  
  return 0;
}
