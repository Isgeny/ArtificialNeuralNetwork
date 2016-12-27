#ifndef NEURALNET_H
#define NEURALNET_H

#include "NeuronLay.h"
#include "Neuron.h"
#include "MyDefenitions.h"

class NeuralNet
{
public:
    NeuralNet();
    NeuralNet(int inputsAmount, int outputsAmount, int hidLayAmount, const IntVector& hidNeuronAmount);
    ~NeuralNet();
    void readWeightsFromFile(const QString& fileName);
    void writeWeightsToFile(int inputsAmount, int outputsAmount, int hidLayCount, const IntVector& hidV, const ActivationFunc& aFunc, int eraCount, double nu, double accuracy);
    void teach(int eraCount, double nu, double minMistake, const ActivationFunc& aFunc, const DoubleMatrix& x, const DoubleMatrix& y);
    void findDeltas(const ActivationFunc& aFunc, const DoubleVector& y);
    void correctWeights();
    DoubleVector recognize(const DoubleVector& input, const ActivationFunc& aFunc);
    double recognitionAccuracy();
    double E(const DoubleVector& factOut, const DoubleVector& realOut);
    void setX(const DoubleMatrix& x);
    void setY(const DoubleMatrix& y);
    NeuronLayVector &getL();
    NeuronLay* getL(int i) const;
    IntVector& gethidNeuronAmount();
    int getInputsAmount() const;
    int getOutputsAmount() const;

    void setinputsAmount(int inputsAmount);
    void setoutputsAmount(int outputsAmount);
    void sethidLayAmount(int hidLayAmount);
    void sethidNeuronAmount(const IntVector& hidNeuronAmount);
    void setMistake(double mistake);
    double getMistake() const;

private:
    int inputsAmount;           //Количество входов сети
    int outputsAmount;          //Количетво выходов сети
    int hidLayAmount;           //Количество скрытых слоев
    IntVector hidNeuronAmount;  //Вектор количеств нейронов в каждом скрытом слое
    NeuronLayVector l;
    DoubleMatrix X;
    DoubleMatrix Y;
    double mistake;
};

#endif // NEURALNET_H
