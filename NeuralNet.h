#ifndef NEURALNET_H
#define NEURALNET_H

#include "NeuronLay.h"
#include "Neuron.h"
#include "MyDefenitions.h"

class NeuralNet
{
private:
    int inputsCount, outputsCount, hiddenLayCount;
    IntVector hiddenNeuronCount;
    NeuronLayVector l;
    DoubleMatrix X;
    DoubleMatrix Y;
    double mistake;

public:
    NeuralNet();
    NeuralNet(int inputsCount, int outputsCount, int hiddenLayCount, const IntVector& hiddenNeuronCount);
    ~NeuralNet();
    void readWeightsFromFile(const QString& fileName);
    void writeWeightsToFile(int inputsCount, int outputsCount, int hidLayCount, const IntVector& hidV, const ActivationFunc& aFunc, int eraCount, double nu, double accuracy);
    void teach(int eraCount, double nu, double minMistake, const ActivationFunc& aFunc, const DoubleMatrix& x, const DoubleMatrix& y);
    void findDeltas(const ActivationFunc& aFunc, const DoubleVector& y);
    void correctWeights();
    DoubleVector recognize(const DoubleVector& input, ActivationFunc aFunc);
    double recognitionAccuracy();
    double E(const DoubleVector& factOut, const DoubleVector& realOut);
    DoubleVector symbolToVector(const QChar& symbol);
    QChar vectorToSymbol(const DoubleVector& y);
    DoubleVector pixelsToBinVector(const QImage &im);
    DoubleMatrix pixelsToBinMatrix(const QImage &im);
    DoubleVector crosses(const DoubleMatrix& m, const DoubleVector& v);
    void setX(const DoubleMatrix& x);
    void setY(const DoubleMatrix& y);
    NeuronLayVector &getL();
    NeuronLay* getL(int i) const;
    IntVector& getHiddenNeuronCount();

    void setInputsCount(int inputsCount);
    void setOutputsCount(int outputsCount);
    void setHiddenLayCount(int hiddenLayCount);
    void setHiddenNeuronCount(const IntVector& hiddenNeuronCount);
    void setMistake(double mistake);
    double getMistake() const;
};

#endif // NEURALNET_H
