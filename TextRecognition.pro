#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T21:48:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextRecognition
TEMPLATE = app


SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    NeuralNet.cpp \
    NeuronLay.cpp \
    Neuron.cpp \
    TeachingDialog.cpp \
    OperatorOverloading.cpp \
    Presenter.cpp

HEADERS  += MainWindow.h \
    NeuralNet.h \
    NeuronLay.h \
    Neuron.h \
    TeachingDialog.h \
    MyDefenitions.h \
    Presenter.h

FORMS    += MainWindow.ui \
    TeachingDialog.ui

RESOURCES +=
