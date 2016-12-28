#ifndef PRESENTER_H
#define PRESENTER_H

#include "MainWindow.h"
#include "NeuralNet.h"
#include "MyDefenitions.h"

class Presenter
{
public:
    Presenter(MainWindow* view, NeuralNet* model);
    void openSymbolFile();
    void readWeightFromFile();
    QChar recognizeSymbol();
    void teach(const TeachArg& args);

private:
    DoubleVector pixelsToBinVector(const QImage &image);
    DoubleVector symbolToVector(const QChar& symbol);
    QChar vectorToSymbol(const DoubleVector& y);

private:
    MainWindow* view_MainWidg;
    NeuralNet* model_Net;

};

#endif // PRESENTER_H
