#include "NeuronLay.h"

NeuronLay::NeuronLay(int _inCount, int _outCount) : inCount(_inCount), outCount(_outCount)
{
    X = new DoubleVector(_inCount);
    Y = new DoubleVector(_outCount);
    Delta.resize(_outCount);
    for(int i = 0; i < _outCount; i++)
        N.push_back(new Neuron(_inCount, X, &((*Y)[i]), &(Delta[i])));
}

NeuronLay::~NeuronLay()
{
    for(int i = 0; i < N.size(); i++)
        delete N[i];
    N.erase(N.begin(), N.end());
    if(X->size())
        delete X;
    if(Y->size())
        delete Y;
}

NeuronVector NeuronLay::n()
{
    return N;
}

Neuron* NeuronLay::n(int i)
{
    return N[i];
}

void NeuronLay::setX(DoubleVector* x)
{
    X = x;
    for(int i = 0; i < N.size(); i++)
        N[i]->setX(x);
}

void NeuronLay::setX(int i, double value)
{
    (*X)[i] = value;
}

DoubleVector* NeuronLay::x()
{
    return X;
}

double& NeuronLay::x(int i)
{
    return (*X)[i];
}

void NeuronLay::setY(DoubleVector* y)
{
    Y = y;
    for(int i = 0; i < N.size(); i++)
        N[i]->setY(&((*Y)[i]));
}

DoubleVector* NeuronLay::y()
{
    return Y;
}

double& NeuronLay::y(int i)
{
    return (*Y)[i];
}

void NeuronLay::setInCount(int inCount)
{
    this->inCount = inCount;
}

int NeuronLay::getInCount() const
{
    return inCount;
}

void NeuronLay::setOutCount(int outCount)
{
    this->outCount = outCount;
}

int NeuronLay::getOutCount() const
{
    return outCount;
}

void NeuronLay::setDelta(DoubleVector& Delta)
{
    this->Delta = Delta;
}

DoubleVector& NeuronLay::delta()
{
    return Delta;
}

double& NeuronLay::delta(int i)
{
    return Delta[i];
}
