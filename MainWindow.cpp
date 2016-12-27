#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    net = new NeuralNet;
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    teachDialog = new TeachingDialog(this);
    connect(teachDialog, &TeachingDialog::signalBtnTeachClicked, this, &MainWindow::slot_buttonTeach_pressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::writeMatrixToFile(const DoubleMatrix &m, const QString& filename)
{
    QFile file(filename);
    QTextStream out(&file);
    if(file.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < m.size(); i++)
        {
            for(int j = 0; j < m[i].size(); j++)
                out << m[i][j] << " ";
            out << "\n";
        }
        file.close();
    }
}

void MainWindow::on_actionOpen_triggered()
{
    ui->lbl_recognized->clear();
    QString sampleDir = "D:/Projects/TextRecognition/TextRecognition/TestingSample";
    QString filename = QFileDialog::getOpenFileName(this, "Выберите изображение", sampleDir, "*.bmp");
    if(!filename.isEmpty())
    {
        if(image.load(filename))
        {
            ui->lbl_orig->setPixmap(QPixmap::fromImage(image));

            QImage tempIm = image;
            tempIm = tempIm.scaledToWidth(ui->lbl_image->width());
            ui->lbl_image->setPixmap(QPixmap::fromImage(tempIm));
        }
    }
    else
    {
        qDebug() << "Can't open file!";
    }
}

void MainWindow::on_btn_recognize_clicked()
{
    if(!image.isNull() && !ui->lbl_image->pixmap()->isNull())
    {
        DoubleVector x = net->pixelsToBinVector(image);
        DoubleVector recognised = net->recognize(x, Neuron::getCurrentActFunc());
        QChar recSymbol = net->vectorToSymbol(recognised);
        ui->lbl_recognized->setText(recSymbol);
    }
}

void MainWindow::on_btn_clear_clicked()
{
    ui->lbl_image->clear();
    ui->lbl_orig->clear();
    ui->lbl_recognized->clear();
}

void MainWindow::on_actionTeach_triggered()
{
    teachDialog->show();
}

void MainWindow::slot_buttonTeach_pressed(int inputsCount, int outputsCount, int hidLayCount,
                                          const IntVector& hidV, const AFuncVector& aFuncV,
                                          int eraCount, double nu, double minMistake)
{
    net->setInputsCount(inputsCount);
    net->setOutputsCount(outputsCount);
    net->setHiddenNeuronCount(hidV);
    net->setHiddenLayCount(hidLayCount);

    QDir dir("D:/Projects/TextRecognition/TextRecognition/TrainingSample");
    QStringList filters = (QStringList() << "*.bmp");
    QStringList fileList = dir.entryList(filters, QDir::Files);
    int filesCount = fileList.size();
    DoubleMatrix X, Y;
    //DoubleVector nums {5, 7, 9, 11, 13, 17, 21};
    DoubleVector nums {3, 7, 11, 15, 19, 23, 27};
    for(int i = 0; i < filesCount; i++)
    {
        QString filename = fileList[i];
        QImage curImage("D:/Projects/TextRecognition/TextRecognition/TrainingSample/" + filename);

        DoubleMatrix x = net->pixelsToBinMatrix(curImage);

        DoubleVector crosses = net->crosses(x, nums);

        QChar symbol = filename[0];
        DoubleVector y = net->symbolToVector(symbol);

        X.push_back(crosses);
        Y.push_back(y);
    }

    writeMatrixToFile(X ,"X.txt");
    writeMatrixToFile(Y, "Y.txt");

    for(int i = 0; i < aFuncV.size(); i++)
    {
        net->teach(eraCount, nu, minMistake, aFuncV.at(i), X, Y);

        double accuracy = net->recognitionAccuracy();
        qDebug() << "Accuracy: " << accuracy;
        qDebug() << "Inputs: " << inputsCount;
        qDebug() << "Outputs: " << outputsCount;
        qDebug() << "Hidden Layer Count: " << hidLayCount;
        qDebug() << "Activation func: " << aFuncV[i];
        qDebug() << "Era count: " << eraCount;
        qDebug() << "Nu: " << nu;

        net->writeWeightsToFile(inputsCount, outputsCount, hidLayCount, hidV, aFuncV.at(i), eraCount, nu, accuracy);
    }
}

void MainWindow::on_actionReadWeight_triggered()
{
    QString weightsDir = "D:/Projects/TextRecognition/TextRecognition/Weights";
    QString filename = QFileDialog::getOpenFileName(this, "Выберите файл весов", weightsDir, "*.txt");
    if(!filename.isEmpty())
    {
        net->readWeightsFromFile(filename);
        QMessageBox msg;
        msg.setText("Чтение весов было успешно завершено");
        msg.exec();
    }
    else
    {
        qDebug() << "Can't open file for reading weights!";
    }
}

void MainWindow::on_actionRecognizePercent_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("Точность распознавания: " + QString::number(net->recognitionAccuracy()) + "%");
    msgBox.setWindowTitle("Точность распознавания");
    msgBox.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
