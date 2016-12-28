#ifndef NEURON_H
#define NEURON_H

#include "MyDefenitions.h"

class Neuron
{
public:
    Neuron(int inCount, DoubleVector* x, double* yPntr, double* delta);
    ~Neuron();
    void setX(DoubleVector* x);
    void setX(int i, double value);
    DoubleVector* x();
    double& x(int i);
    void setW(DoubleVector& W);
    void setW(int i, double value);
    DoubleVector& w();
    double& w(int i);
    void setBias(double bias);
    double getBias() const;
    static void setNu(double nu);
    static double getNu();
    static void setCurrentActFunc(ActivationFunc aFunc);
    static ActivationFunc getCurrentActFunc();
    void setY(double* y);
    void setY(double y);
    double getY() const;
    void setDelta(double* delta);
    void setDelta(double delta);
    double getDelta() const;

    double sum() const;
    double logSigm() const;
    double gipTang() const;
    double softplus() const;
    double relu() const;
    double randomizedReluTeaching() const;
    double randomizedReluRecognition() const;

    double deltaW(int indexInput) const;
    double deltaWBias() const;
    double deltaOutLogSigm(double real, double fact) const;
    double deltaOutGipTang(double real, double fact) const;
    double deltaOutSoftplus(double real, double fact) const;
    double deltaOutRelu(double real, double fact) const;
    double deltaOutRandomizedRelu(double real, double fact) const;
    double deltaHiddenLogSigm(DoubleVector& weight, DoubleVector& delta1) const;
    double deltaHiddenGipTang(DoubleVector& weight, DoubleVector& delta1) const;
    double deltaHiddenSoftplus(DoubleVector& weight, DoubleVector& delta1) const;
    double deltaHiddenRelu(DoubleVector& weight, DoubleVector& delta1) const;
    double deltaHiddenRandomizedRelu(DoubleVector& weight, DoubleVector& delta1) const;

private:
    void setRandomWeight();

private:
    DoubleVector* X;
    DoubleVector W;
    double bias;
    static double nu;
    static ActivationFunc currentActFunc;
    double *yPntr;
    double *delta;

};

#endif // NEURON_H
