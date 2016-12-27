#include "NeuralNet.h"

NeuralNet::NeuralNet() : inputsAmount(0), outputsAmount(0), hidLayAmount(0), mistake(0.0)
{

}

NeuralNet::NeuralNet(int _inputsAmount, int _outputsAmount, int _hidLayAmount, const IntVector& _hidNeuronAmount) :
    inputsAmount(_inputsAmount), outputsAmount(_outputsAmount), hidLayAmount(_hidLayAmount), hidNeuronAmount(_hidNeuronAmount)
{
    //Создание скрытых слоев
    int tempInputs = inputsAmount;
    for(int i = 0; i < hidLayAmount; i++)
    {
        l.push_back(new NeuronLay(tempInputs, hidNeuronAmount.at(i)));
        tempInputs = hidNeuronAmount.at(i);
    }

    //Создание выходного слоя
    l.push_back(new NeuronLay(tempInputs, outputsAmount));

    //Объедининение выходов одного слоя со входами следующего
    for(int i = 0; i < hidLayAmount; i++)
        l[i]->setY(l[i+1]->x());
}

NeuralNet::~NeuralNet()
{
    for(int i = 0; i < hidLayAmount + 1; i++)
        delete l[i];
    l.erase(l.begin(), l.end());
}

void NeuralNet::readWeightsFromFile(const QString& fileName)
{
    //Считывание весов с файла
    QFile file(fileName);
    QTextStream in(&file);
    if(file.open(QIODevice::ReadOnly))
    {
        //Считываем параметры сети (входы, выходы, функция, количество скрытых слоев)
        ActivationFunc aFunc;
        IntVector hidN;
        in >> inputsAmount >> outputsAmount >> aFunc >> hidLayAmount;
        Neuron::setCurrentActFunc(aFunc);
        //Считываем количество нейронов в скрытых слоях
        for(int i = 0; i < hidLayAmount; i++)
        {
            int temp;
            in >> temp;
            hidN.push_back(temp);
        }
        hidNeuronAmount = hidN;
        sethidLayAmount(hidLayAmount);

        //Считываем веса
        for(int i = 0; i < hidLayAmount + 1; i++)
        {
            int tempNeuronCount = l[i]->getOutCount();
            for(int j = 0; j < tempNeuronCount; j++)
            {
                //Считываем смещение
                double tempBias;
                in >> tempBias;
                l[i]->n(j)->setBias(tempBias);
                //Считываем все веса
                for(int k = 0; k < l[i]->n(j)->w().size(); k++)
                {
                    double tempWeight;
                    in >> tempWeight;
                    l[i]->n(j)->setW(k, tempWeight);
                }
            }
        }
    }
    else
    {
        qDebug() << "Can't open file for reading!";
    }
    file.close();
}

void NeuralNet::writeWeightsToFile(int inputsAmount, int outputsAmount, int hidLayCount, const IntVector& hidV, const ActivationFunc& aFunc, int eraCount, double nu, double accuracy)
{
    /*Формируем имя файла (количество входов, количество выходов, количество скрытых слоев,
      количество нейронов в каждом скрытом слое, функция-активации, количество эпох, скорость обучения, точность распознавания)*/
    QString fileName = QString::number(inputsAmount) + "_" + QString::number(outputsAmount)+ "_" + QString::number(hidLayCount) + "_";
    for(int i = 0; i < hidLayCount; i++)
        fileName += QString::number(hidV.at(i)) + "_";
    fileName += QString::number(aFunc) + "_" + QString::number(eraCount) + "_" + QString::number(nu) + "_" + QString::number((int)accuracy) + ".txt";

    //Запись весов в файл
    QFile file(fileName);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        //Записываем параметры сети
        out << inputsAmount << " " << outputsAmount << " " << Neuron::getCurrentActFunc() << " " << hidLayAmount << " ";
        for(int i = 0; i < hidLayAmount; i++)
            out << hidV[i] << " ";

        for(int i = 0; i < hidLayAmount + 1; i++)
        {
            int tempNeuronCount = l[i]->getOutCount();
            for(int j = 0; j < tempNeuronCount; j++)
            {
                double tempBias = l[i]->n(j)->getBias();
                out << tempBias << " ";
                for(int k = 0; k < l[i]->n(j)->w().size(); k++)
                {
                    double tempWeight = l[i]->n(j)->w(k);
                    out << tempWeight << " ";
                }
            }
        }
    }
    else
    {
        qDebug() << "Can't open file for write!";
    }
    file.close();
}

void NeuralNet::teach(int eraCount, double nu, double minMistake, const ActivationFunc& aFunc, const DoubleMatrix& x, const DoubleMatrix& y)
{
    QDir dir("D:/Projects/TextRecognition/TextRecognition/TrainingSample");
    Neuron::setNu(nu);
    Neuron::setCurrentActFunc(aFunc);
    X = x;
    Y = y;
    int sampleSize = X.size();
    QDateTime startTime = QDateTime::currentDateTime();
    for(int era = 0; era < eraCount; era++)
    {
        mistake = 0.0;
        qDebug() << era+1 << " era";
        for(int k = 0; k < sampleSize; k++)
        {
            //Подаем сигналы на вход
            for(int i = 0; i < inputsAmount; i++)
                l[0]->setX(i, X[k][i]);

            for(int i = 0; i < hidLayAmount + 1; i++)
            {
                int tempNeuronCount = l[i]->getOutCount();
                for(int j = 0; j < tempNeuronCount; j++)
                {
                    double tempNeuronOutput;
                    switch(aFunc)
                    {
                    case HYPERBOLIC_TANGENT:
                        tempNeuronOutput = l[i]->n(j)->gipTang();
                        break;
                    case LOG_SIGMOID:
                        tempNeuronOutput = l[i]->n(j)->logSigm();
                        break;
                    case SOFTPLUS:
                        tempNeuronOutput = l[i]->n(j)->softplus();
                        break;
                    case RELU:
                        tempNeuronOutput = l[i]->n(j)->relu();
                        break;
                    case RANDOMIZED_RELU:
                        tempNeuronOutput = l[i]->n(j)->randomizedReluTeaching();
                        break;
                    default:
                        break;
                    }
                    l[i]->n(j)->setY(tempNeuronOutput);
                }
            }
            //Находим ошибки нейронов
            findDeltas(aFunc, Y[k]);
            correctWeights();
            mistake += E(*(l.at(hidLayAmount)->y()), Y[k]);
        }
        mistake = mistake / dir.count();
        qDebug() << "Current mistake: " << mistake;
        if(mistake < minMistake)
        {
            qDebug() << "Minimal - " << minMistake << "; Current - " << mistake;
            return;
        }
        double currentAccuracy = recognitionAccuracy();
        qDebug() << "Current accuracy: " << currentAccuracy;
        QDateTime curTime = QDateTime::currentDateTime();
        double curElapsTime = (double)startTime.secsTo(curTime);
        qDebug() << "Current teaching time (hours): " << curElapsTime / 3600.0;
        writeWeightsToFile(inputsAmount, outputsAmount, hidLayAmount, hidNeuronAmount, aFunc, eraCount, nu, currentAccuracy);
    }
    QDateTime finishTime = QDateTime::currentDateTime();
    double time = (double)startTime.secsTo(finishTime);
    qDebug() << "Teaching time (hours): " << time / 3600.0;
}

void NeuralNet::findDeltas(const ActivationFunc& aFunc, const DoubleVector& y)
{
    //Ошибки нейронов выходного слоя
    for(int i = 0; i < outputsAmount; i++)
    {
        double tempFact = l[hidLayAmount]->n(i)->getY();
        double tempDelta;
        switch(aFunc)
        {
        case HYPERBOLIC_TANGENT:
            tempDelta = l[hidLayAmount]->n(i)->deltaOutGipTang(y[i], tempFact);
            break;
        case LOG_SIGMOID:
            tempDelta = l[hidLayAmount]->n(i)->deltaOutLogSigm(y[i], tempFact);
            break;
        case SOFTPLUS:
            tempDelta = l[hidLayAmount]->n(i)->deltaOutSoftplus(y[i], tempFact);
            break;
        case RELU:
            tempDelta = l[hidLayAmount]->n(i)->deltaOutRelu(y[i], tempFact);
            break;
        case RANDOMIZED_RELU:
            tempDelta = l[hidLayAmount]->n(i)->deltaOutRandomizedRelu(y[i], tempFact);
            break;
        default:
            break;
        }
        l[hidLayAmount]->n(i)->setDelta(tempDelta);
    }
    //Ошибки нейронов скрытых слоев
    for(int i = hidLayAmount - 1; i >= 0; i--)
    {
        int tempNeuronCount = l[i]->getOutCount();
        for(int j = 0; j < tempNeuronCount; j++)
        {
            //Связанные веса
            DoubleVector connectedWeights;
            int tempNeuronCountNextLay = l[i + 1]->getOutCount();
            for(int k = 0; k < tempNeuronCountNextLay; k++)
            {
                connectedWeights.push_back(l[i + 1]->n(k)->w(j));
            }

            double tempDelta;
            switch(aFunc)
            {
            case HYPERBOLIC_TANGENT:
                tempDelta = l[i]->n(j)->deltaHiddenGipTang(connectedWeights, l[i+1]->delta());
                break;
            case LOG_SIGMOID:
                tempDelta = l[i]->n(j)->deltaHiddenLogSigm(connectedWeights, l[i+1]->delta());
                break;
            case SOFTPLUS:
                tempDelta = l[i]->n(j)->deltaHiddenSoftplus(connectedWeights, l[i+1]->delta());
                break;
            case RELU:
                tempDelta = l[i]->n(j)->deltaHiddenRelu(connectedWeights, l[i+1]->delta());
                break;
            case RANDOMIZED_RELU:
                tempDelta = l[i]->n(j)->deltaHiddenRandomizedRelu(connectedWeights, l[i + 1]->delta());
                break;
            default:
                break;
            }
            l[i]->n(j)->setDelta(tempDelta);
        }
    }
}

void NeuralNet::correctWeights()
{
    for(int i = hidLayAmount; i >= 0; i--)
    {
        int tempNeuronCount = l[i]->getOutCount();
        for(int j = 0; j < tempNeuronCount; j++)
        {
            //Корректируем веса
            int tempCountWeights = l[i]->n(j)->w().size();
            for(int k = 0; k < tempCountWeights; k++)
            {
                double tempDeltaWeight = l[i]->n(j)->deltaW(k);
                double tempNewWeight = l[i]->n(j)->w(k) + tempDeltaWeight;
                l[i]->n(j)->w(k) = tempNewWeight;
            }
            //Корректируем смещение
            double tempDeltaBias = l[i]->n(j)->deltaWBias();
            double currBias = l[i]->n(j)->getBias();
            double tempNewBias = tempDeltaBias + currBias;
            l[i]->n(j)->setBias(tempNewBias);
        }
    }
}

DoubleVector NeuralNet::recognize(const DoubleVector& x, const ActivationFunc& aFunc)
{
    DoubleVector output;

    for(int i = 0; i < l[0]->getInCount(); i++)
        l[0]->setX(i, x[i]);

    for(uint i = 0; i < l.size(); i++)
    {
        for(int j = 0; j < l[i]->getOutCount(); j++)
        {
            double tempNeuronOutput = 0.0;
            switch(aFunc)
            {
            case HYPERBOLIC_TANGENT:
                tempNeuronOutput = l[i]->n(j)->gipTang();
                break;
            case LOG_SIGMOID:
                tempNeuronOutput = l[i]->n(j)->logSigm();
                break;
            case SOFTPLUS:
                tempNeuronOutput = l[i]->n(j)->softplus();
                break;
            case RELU:
                tempNeuronOutput = l[i]->n(j)->relu();
                break;
            case RANDOMIZED_RELU:
                tempNeuronOutput = l[i]->n(j)->randomizedReluRecognition();
                break;
            default:
                break;
            }
            l[i]->n(j)->setY(tempNeuronOutput);
        }
    }

    for(int i = 0; i < l[hidLayAmount]->getOutCount(); i++)
        output.push_back(l[hidLayAmount]->n(i)->getY());

    return output;
}

double NeuralNet::recognitionAccuracy()
{
    /*QDir dir("D:/Projects/TextRecognition/TextRecognition/TestingSample");
    QStringList filters = (QStringList() << "*.bmp");
    QStringList fileList = dir.entryList(filters, QDir::Files);
    int recocnizedCount = 0;
    //DoubleVector nums {5, 7, 9, 11, 13, 17, 21};
    DoubleVector nums {3, 7, 11, 15, 19, 23, 27};
    for(int j = 0; j < fileList.size(); j++)
    {
        QString filename = fileList[j];
        QImage curImage("D:/Projects/TextRecognition/TextRecognition/TestingSample/" + filename);
        DoubleMatrix binMatrix = pixelsToBinMatrix(curImage);

        DoubleVector crosses = this->crosses(binMatrix, nums);

        DoubleVector recognized = recognize(crosses, Neuron::getCurrentActFunc());
        QChar recSymbol = vectorToSymbol(recognized);
        QChar symbol = filename[0];
        if(recSymbol.unicode() == symbol.unicode())
            recocnizedCount++;
    }
    double accuracy = ((double)recocnizedCount / (double)dir.count()) * 100.0;
    return accuracy;*/
}

double NeuralNet::E(const DoubleVector& factOut, const DoubleVector& realOut)
{
    double tempSum = 0.0;
    for(int i = 0; i < factOut.size(); i++)
        tempSum += (factOut[i] - realOut[i])*(factOut[i] - realOut[i]);
    return 0.5*tempSum;
}

void NeuralNet::setX(const DoubleMatrix &x)
{
    this->X = x;
}

void NeuralNet::setY(const DoubleMatrix &y)
{
    this->Y = y;
}

NeuronLayVector& NeuralNet::getL()
{
    return l;
}

NeuronLay* NeuralNet::getL(int i) const
{
    return l[i];
}

IntVector &NeuralNet::gethidNeuronAmount()
{
    return hidNeuronAmount;
}

int NeuralNet::getInputsAmount() const
{
    return inputsAmount;
}

int NeuralNet::getOutputsAmount() const
{
    return outputsAmount;
}

void NeuralNet::setinputsAmount(int inputsAmount)
{
    this->inputsAmount = inputsAmount;
}

void NeuralNet::setoutputsAmount(int outputsAmount)
{
    this->outputsAmount = outputsAmount;
}

void NeuralNet::sethidLayAmount(int hidLayAmount)
{
    this->hidLayAmount = hidLayAmount;

    //Создание скрытых слоев
    int tempInputs = inputsAmount;
    for(int i = 0; i < hidLayAmount; i++)
    {
        l.push_back(new NeuronLay(tempInputs, hidNeuronAmount.at(i)));
        tempInputs = hidNeuronAmount.at(i);
    }

    //Создание выходного слоя
    l.push_back(new NeuronLay(tempInputs, outputsAmount));

    //Объедининение выходов одного слоя со входами следующего
    for(int i = 0; i < hidLayAmount; i++)
        l[i]->setY(l[i+1]->x());
}

void NeuralNet::sethidNeuronAmount(const IntVector &hidNeuronAmount)
{
    this->hidNeuronAmount = hidNeuronAmount;
}

void NeuralNet::setMistake(double mistake)
{
    this->mistake = mistake;
}

double NeuralNet::getMistake() const
{
    return mistake;
}
