#include "Neuron.h"

double Neuron::nu = 0.8;
ActivationFunc Neuron::currentActFunc = HYPERBOLIC_TANGENT;

Neuron::Neuron(int inCount, DoubleVector* x, double* yPntr, double* delta) : bias(0.0)
{
    this->X = x;
    this->yPntr = yPntr;
    this->delta = delta;
    W.resize(inCount);
    setRandomWeight();
}

Neuron::~Neuron()
{

}

void Neuron::setX(DoubleVector* x)
{
    this->X = x;
}

void Neuron::setX(int i, double value)
{
    (*X)[i] = value;
}

DoubleVector* Neuron::x()
{
    return X;
}

double & Neuron::x(int i)
{
    return (*X)[i];
}

void Neuron::setW(DoubleVector& W)
{
    this->W = W;
}

void Neuron::setW(int i, double value)
{
    W[i] = value;
}

DoubleVector& Neuron::w()
{
    return W;
}

double& Neuron::w(int i)
{
    return W[i];
}

void Neuron::setBias(double bias)
{
    this->bias = bias;
}

double Neuron::getBias() const
{
    return bias;
}

void Neuron::setNu(double _nu)
{
    nu = _nu;
}

double Neuron::getNu()
{
    return nu;
}

void Neuron::setCurrentActFunc(ActivationFunc aFunc)
{
    currentActFunc = aFunc;
}

ActivationFunc Neuron::getCurrentActFunc()
{
    return currentActFunc;
}

void Neuron::setY(double* y)
{
    yPntr = y;
}

void Neuron::setY(double y)
{
    (*yPntr) = y;
}

double Neuron::getY() const
{
    return *yPntr;
}

void Neuron::setDelta(double* delta)
{
    this->delta = delta;
}

void Neuron::setDelta(double delta)
{
    (*this->delta) = delta;
}

double Neuron::getDelta() const
{
    return *delta;
}

double Neuron::sum() const
{
    double tempSum = 0.0;
    for(int i = 0; i < W.size(); i++)
    {
        tempSum += W[i] * (*X)[i];
    }
    tempSum += bias;
    return tempSum;
}

double Neuron::logSigm() const
{
    return (1.0)/(1.0 + exp(-sum()));
}

double Neuron::gipTang() const
{
    return tanh(sum());
}

double Neuron::softplus() const
{
    return log(1 + exp(sum()));
}

double Neuron::relu() const
{
    if(sum() <= 0.0)
        return 0.0;
    return sum();
}

double Neuron::randomizedReluTeaching() const
{
    if(sum() < 0.0)
        return ((1.0 / ((double)(rand() % 6) + 3.0))*sum());
    return sum();
}

double Neuron::randomizedReluRecognition() const
{
    if(sum() < 0.0)
        return ((2.0 / 11.0) * sum());
    return sum();
}

double Neuron::deltaW(int i) const
{
    return -nu*(*delta) * (*X)[i];
}

double Neuron::deltaWBias() const
{
    return -nu*(*delta);
}

double Neuron::deltaOutLogSigm(double real, double fact) const
{
    return logSigm()*(1 - logSigm())*(fact - real);
}

double Neuron::deltaOutGipTang(double real, double fact) const
{
    return (1.0 / (cosh(sum()*cosh(sum()))))*(fact - real);
}

double Neuron::deltaOutSoftplus(double real, double fact) const
{
    return (1.0 / (1.0 + exp(-sum())))*(fact - real);
}

double Neuron::deltaOutRelu(double real, double fact) const
{
    if(sum() <= 0.0)
        return 0.0;
    return (fact - real);
}

double Neuron::deltaOutRandomizedRelu(double real, double fact) const
{
    if(sum() < 0.0)
        return ((1.0 / ((double)(rand() % 6) + 3.0)) * (fact - real));
    return (fact - real);
}

double Neuron::deltaHiddenLogSigm(DoubleVector& weight, DoubleVector& delta1) const
{
    double sum = 0.0;
    for(int i = 0; i < delta1.size(); i++)
    {
        sum += weight[i] * delta1[i];
    }
    return (logSigm()*(1 - logSigm()))*sum;
}

double Neuron::deltaHiddenGipTang(DoubleVector& weight, DoubleVector& delta1) const
{
    double s = 0.0;
    for(int i = 0; i < delta1.size(); i++)
    {
        s += weight[i] * delta1[i];
    }
    return  (1.0 / (cosh(sum()*cosh(sum())))) * s;
}

double Neuron::deltaHiddenSoftplus(DoubleVector& weight, DoubleVector& delta1) const
{
    double s = 0.0;
    for(int i = 0; i < delta1.size(); i++)
    {
        s += weight[i] * delta1[i];
    }
    return (1.0 / (1 + exp(-sum()))) * s;
}

double Neuron::deltaHiddenRelu(DoubleVector& weight, DoubleVector& delta1) const
{
    if(sum() <= 0.0)
        return 0.0;
    double s = 0.0;
    for(int i = 0; i < delta1.size(); i++)
    {
        s += weight[i] * delta1[i];
    }
    return s;
}

double Neuron::deltaHiddenRandomizedRelu(DoubleVector& weight, DoubleVector& delta1) const
{
    double s = 0.0;
    for(int i = 0; i < delta1.size(); i++)
    {
        s += weight[i] * delta1[i];
    }
    if(sum() < 0.0)
        return (1.0 / ((double)(rand() % 6) + 3.0)) * s;
    return s;
}

void Neuron::setRandomWeight()
{
    for(int i = 0; i < W.size(); i++)
    {
        W[i] = (double)(rand()) / RAND_MAX*(0.01 - (-0.01)) - 0.01;
    }
    bias = (double)(rand()) / RAND_MAX*(0.01 - (-0.01)) - 0.01;
}
