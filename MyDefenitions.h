#ifndef MYDEFENITIONS_H
#define MYDEFENITIONS_H

#include <QVector>
#include <fstream>
#include <iostream>
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QChar>
#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QTime>

#define IMAGE_SZ 32
#define TEACHING_ROOT "D:/Projects/TextRecognition/TextRecognition/TeachingSample"
#define TESTING_ROOT "D:/Projects/TextRecognition/TextRecognition/TestingSample"
#define WEIGHTS_ROOT "D:/Projects/TextRecognition/TextRecognition/Weights"

enum ActivationFunc
{
    HYPERBOLIC_TANGENT = 0,
    LOG_SIGMOID,
    SOFTPLUS,
    RELU,
    RANDOMIZED_RELU
};

typedef QVector<QVector<double>> DoubleMatrix;
typedef QVector<double> DoubleVector;
typedef QVector<int> IntVector;
typedef QVector<ActivationFunc> AFuncVector;

QTextStream& operator >> (QTextStream& in,  ActivationFunc &aFunc);
QTextStream& operator << (QTextStream& out, ActivationFunc &aFunc);

#endif // MYDEFENITIONS_H
