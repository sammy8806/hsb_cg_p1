#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T23:07:51
#
#-------------------------------------------------

QT       += core gui opengl

win32 {
    LIBS    += -lOpengl32
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cg_p1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    oglwidget.cpp \
    vertex.cpp \
    triangle.cpp \
    quad.cpp \
    neighbor.cpp

HEADERS  += mainwindow.h \
    oglwidget.h \
    vertex.h \
    triangle.h \
    quad.h \
    neighbor.h

FORMS    += mainwindow.ui
