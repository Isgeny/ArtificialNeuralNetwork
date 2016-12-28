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
#include <QRadioButton>

#define IMAGE_SZ 32
#define TRAINING_ROOT "D:/Projects/TextRecognition/TextRecognition/TeachingSample"
#define TESTING_ROOT "D:/Projects/TextRecognition/TextRecognition/TestingSample"
#define WEIGHTS_ROOT "D:/Projects/TextRecognition/TextRecognition/Weights"

class MainWindow;
class TeachingDialog;
class Presenter;
class NeuralNet;
class NeuronLay;
class Neuron;

enum ActivationFunc
{
    HYPERBOLIC_TANGENT = 0,
    LOG_SIGMOID,
    SOFTPLUS,
    RELU,
    RANDOMIZED_RELU
};

typedef std::vector<std::vector<double>> DoubleMatrix;
typedef std::vector<double> DoubleVector;
typedef std::vector<int> IntVector;
typedef std::vector<ActivationFunc> AFuncVector;
typedef std::vector<Neuron*> NeuronVector;
typedef std::vector<NeuronLay*> NeuronLayVector;

struct TeachArg
{
    int inputsAmount;
    int outputsAmount;
    int hidLayAmount;
    IntVector hidV;
    ActivationFunc actFunc;
    int eraAmount;
    double nu;
    double minMistake;
};

QTextStream& operator >> (QTextStream& in,  ActivationFunc &aFunc);
QTextStream& operator << (QTextStream& out, ActivationFunc &aFunc);

#endif // MYDEFENITIONS_H
