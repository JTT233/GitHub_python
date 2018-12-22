#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T14:43:51
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PackgeManagement
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    form.cpp \
    mymesgebox.cpp

HEADERS  += widget.h \
    form.h \
    mymesgebox.h

FORMS    += widget.ui \
    form.ui \
    mymesgebox.ui

RESOURCES += \
    images.qrc
