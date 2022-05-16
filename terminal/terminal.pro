TEMPLATE = app
TARGET   = terminal

CONFIG += debug
CONFIG += warn_on

QMAKE_CXXFLAGS += -Werror

QT += charts

MOC_DIR = ./.moc
UI_DIR  = ./.ui
RCC_DIR = ./.rcc
INCLUDEPATH += ./.ui
OBJECTS_DIR = ./.o


HEADERS = terminal.h \
          chart.h


SOURCES = terminal.cpp \
          chart.cpp \
          main.cpp

          
FORMS =  terminal.ui 
