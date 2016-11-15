#include "NeuralNet.h"

NeuralNet::NeuralNet() : inputsCount(0), outputsCount(0), hiddenLayCount(0), mistake(0.0)
{

}

NeuralNet::NeuralNet(int _inputsCount, int _outputsCount, int _hiddenLayCount, const IntVector& _hiddenNeuronCount) :
    inputsCount(_inputsCount), outputsCount(_outputsCount), hiddenLayCount(_hiddenLayCount), hiddenNeuronCount(_hiddenNeuronCount)
{
    //Создание скрытых слоев
    int tempInputs = inputsCount;
    for(int i = 0; i < hiddenLayCount; i++)
    {
        l.push_back(new NeuronLay(tempInputs, hiddenNeuronCount.at(i)));
        tempInputs = hiddenNeuronCount.at(i);
    }

    //Создание выходного слоя
    l.push_back(new NeuronLay(tempInputs, outputsCount));

    //Объедининение выходов одного слоя со входами следующего
    for(int i = 0; i < hiddenLayCount; i++)
        l[i]->setY(l[i+1]->x());
}

NeuralNet::~NeuralNet()
{
    for(int i = 0; i < hiddenLayCount + 1; i++)
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
        in >> inputsCount >> outputsCount >> aFunc >> hiddenLayCount;
        Neuron::setCurrentActFunc(aFunc);
        //Считываем количество нейронов в скрытых слоях
        for(int i = 0; i < hiddenLayCount; i++)
        {
            int temp;
            in >> temp;
            hidN.push_back(temp);
        }
        hiddenNeuronCount = hidN;
        setHiddenLayCount(hiddenLayCount);

        //Считываем веса
        for(int i = 0; i < hiddenLayCount + 1; i++)
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

void NeuralNet::writeWeightsToFile(int inputsCount, int outputsCount, int hidLayCount, const IntVector& hidV, const ActivationFunc& aFunc, int eraCount, double nu, double accuracy)
{
    /*Формируем имя файла (количество входов, количество выходов, количество скрытых слоев,
      количество нейронов в каждом скрытом слое, функция-активации, количество эпох, скорость обучения, точность распознавания)*/
    QString fileName = QString::number(inputsCount) + "_" + QString::number(outputsCount)+ "_" + QString::number(hidLayCount) + "_";
    for(int i = 0; i < hidLayCount; i++)
        fileName += QString::number(hidV.at(i)) + "_";
    fileName += QString::number(aFunc) + "_" + QString::number(eraCount) + "_" + QString::number(nu) + "_" + QString::number((int)accuracy) + ".txt";

    //Запись весов в файл
    QFile file(fileName);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        //Записываем параметры сети
        out << inputsCount << " " << outputsCount << " " << Neuron::getCurrentActFunc() << " " << hiddenLayCount << " ";
        for(int i = 0; i < hiddenLayCount; i++)
            out << hidV[i] << " ";

        for(int i = 0; i < hiddenLayCount + 1; i++)
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
    QTime time;
    time.start();
    for(int era = 0; era < eraCount; era++)
    {
        mistake = 0.0;
        qDebug() << era+1 << " era";
        for(int k = 0; k < sampleSize; k++)
        {
            //Подаем сигналы на вход
            for(int i = 0; i < inputsCount; i++)
                l[0]->setX(i, X[k][i]);

            for(int i = 0; i < hiddenLayCount + 1; i++)
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
            mistake += E(*(l.at(hiddenLayCount)->y()), Y[k]);
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
        writeWeightsToFile(inputsCount, outputsCount, hiddenLayCount, hiddenNeuronCount, aFunc, eraCount, nu, currentAccuracy);
    }
    double elapsedTime = (double)time.elapsed();
    qDebug() << "Teaching time (hours): " << elapsedTime/1000.0/60.0/60.0;
}

void NeuralNet::findDeltas(const ActivationFunc& aFunc, const DoubleVector& y)
{
    //Ошибки нейронов выходного слоя
    for(int i = 0; i < outputsCount; i++)
    {
        double tempFact = l[hiddenLayCount]->n(i)->getY();
        double tempDelta;
        switch(aFunc)
        {
        case HYPERBOLIC_TANGENT:
            tempDelta = l[hiddenLayCount]->n(i)->deltaOutGipTang(y[i], tempFact);
            break;
        case LOG_SIGMOID:
            tempDelta = l[hiddenLayCount]->n(i)->deltaOutLogSigm(y[i], tempFact);
            break;
        case SOFTPLUS:
            tempDelta = l[hiddenLayCount]->n(i)->deltaOutSoftplus(y[i], tempFact);
            break;
        case RELU:
            tempDelta = l[hiddenLayCount]->n(i)->deltaOutRelu(y[i], tempFact);
            break;
        case RANDOMIZED_RELU:
            tempDelta = l[hiddenLayCount]->n(i)->deltaOutRandomizedRelu(y[i], tempFact);
            break;
        default:
            break;
        }
        l[hiddenLayCount]->n(i)->setDelta(tempDelta);
    }
    //Ошибки нейронов скрытых слоев
    for(int i = hiddenLayCount - 1; i >= 0; i--)
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
    for(int i = hiddenLayCount; i >= 0; i--)
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

DoubleVector NeuralNet::recognize(const DoubleVector& x, ActivationFunc aFunc)
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

    for(int i = 0; i < l[hiddenLayCount]->getOutCount(); i++)
    {
        output.push_back(l[hiddenLayCount]->n(i)->getY());
    }

    return output;
}

double NeuralNet::recognitionAccuracy()
{
    QDir dir("D:/Projects/TextRecognition/TextRecognition/TestingSample");
    QStringList filters = (QStringList() << "*.bmp");
    QStringList fileList = dir.entryList(filters, QDir::Files);
    int recocnizedCount = 0;
    for(int j = 0; j < fileList.size(); j++)
    {
        QString filename = fileList[j];
        QImage curImage("D:/Projects/TextRecognition/TextRecognition/TestingSample/" + filename);
        DoubleVector x = pixelsToBin(curImage);
        DoubleVector recognized = recognize(x, Neuron::getCurrentActFunc());
        QChar recSymbol = vectorToSymbol(recognized);
        QChar symbol = filename[0];
        if(recSymbol.unicode() == symbol.unicode())
            recocnizedCount++;
    }
    double accuracy = ((double)recocnizedCount / (double)dir.count()) * 100.0;
    return accuracy;
}

double NeuralNet::E(const DoubleVector& factOut, const DoubleVector& realOut)
{
    double tempSum = 0.0;
    for(int i = 0; i < factOut.size(); i++)
        tempSum += (factOut[i] - realOut[i])*(factOut[i] - realOut[i]);
    return 0.5*tempSum;
}

DoubleVector NeuralNet::symbolToVector(const QChar& symbol)
{
    DoubleVector y(outputsCount);
    if(symbol.unicode() == 45)
    {
        y[0] = 1.0;
        return y;
    }
    if(symbol.unicode() >= 48 && symbol.unicode() <= 57)
    {
        int i = symbol.unicode() - 48 + 1;
        y[i] = 1.0;
        return y;
    }
    if(symbol.unicode() >= 1040 && symbol.unicode() <= 1071)
    {
        int i = symbol.unicode() - 1040 + 10 + 1;
        y[i] = 1.0;
        return y;
    }
    std::fill(y.begin(), y.end(), -666);
    return y;
}

QChar NeuralNet::vectorToSymbol(const DoubleVector &y)
{
    QChar symbol;
    int index;
    double max = -1.0;

    for(int i = 0; i < y.size(); i++)
    {
        if(y[i] > max)
        {
            max = y[i];
            index = i;
        }
    }

    if(index == 0)
    {
        symbol = 45;
        return symbol;
    }
    if(index >= 1 && index <= 10)
    {
        symbol = 48 + index - 1;
        return symbol;
    }
    if(index >= 11 && index <= 42)
    {
        symbol = 1040 + index - 10 - 1;
        return symbol;
    }
    return QChar(100);
}

DoubleVector NeuralNet::pixelsToBin(const QImage &im)
{
    DoubleVector v;
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            QColor col = im.pixel(j, i);
            int r = col.red();
            int g = col.green();
            int b = col.blue();
            if(r > 240 && g > 240 && b > 240)
            {
                v.push_back(0.0);
            }
            else
            {
                v.push_back(1.0);
            }
        }
    }
    return v;
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

IntVector &NeuralNet::getHiddenNeuronCount()
{
    return hiddenNeuronCount;
}

void NeuralNet::setInputsCount(int inputsCount)
{
    this->inputsCount = inputsCount;
}

void NeuralNet::setOutputsCount(int outputsCount)
{
    this->outputsCount = outputsCount;
}

void NeuralNet::setHiddenLayCount(int hiddenLayCount)
{
    this->hiddenLayCount = hiddenLayCount;

    //Создание скрытых слоев
    int tempInputs = inputsCount;
    for(int i = 0; i < hiddenLayCount; i++)
    {
        l.push_back(new NeuronLay(tempInputs, hiddenNeuronCount.at(i)));
        tempInputs = hiddenNeuronCount.at(i);
    }

    //Создание выходного слоя
    l.push_back(new NeuronLay(tempInputs, outputsCount));

    //Объедининение выходов одного слоя со входами следующего
    for(int i = 0; i < hiddenLayCount; i++)
        l[i]->setY(l[i+1]->x());
}

void NeuralNet::setHiddenNeuronCount(const IntVector &hiddenNeuronCount)
{
    this->hiddenNeuronCount = hiddenNeuronCount;
}

void NeuralNet::setMistake(double mistake)
{
    this->mistake = mistake;
}

double NeuralNet::getMistake() const
{
    return mistake;
}
