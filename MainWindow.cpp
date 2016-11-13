#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QTime::currentTime().msecsSinceStartOfDay());
    net = new NeuralNet;
    teachDialog = new TeachingDialog(this);
    connect(teachDialog, &TeachingDialog::signalBtnTeachClicked, this, &MainWindow::slot_buttonTeach_pressed);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
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

void MainWindow::on_actionRecognize_triggered()
{
    on_btn_recognize_clicked();
}

void MainWindow::on_btn_recognize_clicked()
{
    if(!image.isNull() && !ui->lbl_image->pixmap()->isNull())
    {
        DoubleVector x = net->pixelsToBin(image);
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
    for(int i = 0; i < filesCount; i++)
    {
        QString filename = fileList[i];
        QImage curImage("D:/Projects/TextRecognition/TextRecognition/TrainingSample/" + filename);

        DoubleVector x = net->pixelsToBin(curImage);

        QChar symbol = filename[0];
        DoubleVector y = net->symbolToVector(symbol);

        X.push_back(x);
        Y.push_back(y);
    }


    for(int i = 0; i < aFuncV.size(); i++)
    {
        net->teach(eraCount, nu, minMistake, aFuncV.at(i), X, Y);

        double accuracy = net->recognitionAccuracy();

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


QTextStream &operator >>(QTextStream &in, ActivationFunc& aFunc)
{
    in >> (quint32&)aFunc;
    return in;
}

QTextStream &operator <<(QTextStream &out, ActivationFunc& aFunc)
{
    out << (quint32&)aFunc;
    return out;
}
