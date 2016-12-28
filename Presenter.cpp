#include "Presenter.h"

Presenter::Presenter(MainWindow* view, NeuralNet* model) :
    view_MainWidg(view), model_Net(model)
{

}

void Presenter::openSymbolFile()
{
    //Показываем диалоговое окно выбора файла символа
    QImage image;
    QString sampleDir = TESTING_ROOT;
    QString filename = QFileDialog::getOpenFileName(view_MainWidg, "Выберите изображение", sampleDir, "*.bmp");
    if(!filename.isEmpty())
        if(image.load(filename))
            view_MainWidg->setSymbolImage(image);
        else
            qDebug() << "Can't load image file!";
    else
        qDebug() << "Can't open file!";
}

void Presenter::readWeightFromFile()
{
    //Чтение весов из файла
    QString weightsDir = WEIGHTS_ROOT;
    QString filename = QFileDialog::getOpenFileName(view_MainWidg, "Выберите файл весов", weightsDir, "*.txt");
    if(!filename.isEmpty())
    {
        model_Net->readWeightsFromFile(filename);
        QMessageBox msg;
        msg.setText("Чтение весов было успешно завершено");
        msg.exec();
    }
    else
    {
        qDebug() << "Can't open file for reading weights!";
    }
}

QChar Presenter::recognizeSymbol()
{
    //Распознавание символа

    //Преобразуем изображение в бинарный вектор
    DoubleVector x = this->pixelsToBinVector(view_MainWidg->getImage());

    //Подаем этот вектор на вход нейронной сети
    DoubleVector recognised = model_Net->recognize(x, Neuron::getCurrentActFunc());

    //Преобразуем результат нейронной сети в символ
    QChar recSymbol = this->vectorToSymbol(recognised);
    return recSymbol;
}

void Presenter::teach(const TeachArg &args)
{
    QDir dir(TRAINING_ROOT);
    QStringList filters = (QStringList() << "*.bmp");
    QStringList fileList = dir.entryList(filters, QDir::Files);
    int filesCount = fileList.size();
    DoubleMatrix X, Y;
    for(int i = 0; i < filesCount; i++)
    {
        QString filename = fileList[i];
        QImage curImage(TRAINING_ROOT + filename);

        DoubleVector x = this->pixelsToBinVector(curImage);
        X.push_back(x);

        QChar symbol = filename[0];
        DoubleVector y = this->symbolToVector(symbol);
        Y.push_back(y);
    }

    model_Net->teach(args, X, Y);

    /*double accuracy = net->recognitionAccuracy();
    qDebug() << "Accuracy: " << accuracy;
    qDebug() << "Inputs: " << inputsCount;
    qDebug() << "Outputs: " << outputsCount;
    qDebug() << "Hidden Layer Count: " << hidLayCount;
    qDebug() << "Activation func: " << aFuncV[i];
    qDebug() << "Era count: " << eraCount;
    qDebug() << "Nu: " << nu;

    net->writeWeightsToFile(inputsCount, outputsCount, hidLayCount, hidV, aFuncV.at(i), eraCount, nu, accuracy);*/
}

DoubleVector Presenter::pixelsToBinVector(const QImage &image)
{
    //Преобразование изображения в бинарный вектор
    DoubleVector v;
    for(int i = 0; i < IMAGE_SZ; i++)
    {
        for(int j = 0; j < IMAGE_SZ; j++)
        {
            QColor col = image.pixel(j, i);
            int r = col.red();
            int g = col.green();
            int b = col.blue();
            (r > 240 && g > 240 && b > 240) ?  v.push_back(0.0) : v.push_back(1.0);
        }
    }
    return v;
}

DoubleVector Presenter::symbolToVector(const QChar& symbol)
{
    //Преобразование символа в бинарный вектор
    DoubleVector y(model_Net->getOutputsAmount());
    if(symbol.unicode() == 45)  //-
    {
        y[0] = 1.0;
        return y;
    }
    if(symbol.unicode() >= 48 && symbol.unicode() <= 57)    //0..9
    {
        int i = symbol.unicode() - 48 + 1;
        y[i] = 1.0;
        return y;
    }
    if(symbol.unicode() >= 1040 && symbol.unicode() <= 1071)    //а..я
    {
        int i = symbol.unicode() - 1040 + 10 + 1;
        y[i] = 1.0;
        return y;
    }
    std::fill(y.begin(), y.end(), -666);
    return y;
}

QChar Presenter::vectorToSymbol(const DoubleVector &y)
{
    //Преобразование бинарного вектора в символ
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

    if(index == 0)  //-
    {
        symbol = 45;
        return symbol;
    }
    if(index >= 1 && index <= 10) //0..9
    {
        symbol = 48 + index - 1;
        return symbol;
    }
    if(index >= 11 && index <= 42)  //а..я
    {
        symbol = 1040 + index - 10 - 1;
        return symbol;
    }
    return QChar(100);
}
