#ifndef NEURONLAY_H
#define NEURONLAY_H

#include "Neuron.h"

class NeuronLay
{
public:
    NeuronLay(int inCount, int outCount);
    ~NeuronLay();
    NeuronVector n();
    Neuron* n(int i);
    void setX(DoubleVector* x);
    void setX(int i, double value);
    DoubleVector* x();
    double& x(int i);
    void setY(DoubleVector* y);
    DoubleVector* y();
    double& y(int i);
    void setInCount(int inCount);
    int getInCount() const;
    void setOutCount(int outCount);
    int getOutCount() const;
    void setDelta(DoubleVector& Delta);
    DoubleVector& delta();
    double& delta(int i);

private:
    NeuronVector N;
    DoubleVector *X, *Y;
    int inCount, outCount;
    DoubleVector Delta;

};

#endif // NEURONLAY_H
