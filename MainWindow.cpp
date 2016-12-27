#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Устанавливаем зерно рандома
    srand(QDateTime::currentMSecsSinceEpoch());

    //Создаем презентер
    presenter = new Presenter(this, new NeuralNet());

    teachDialog = new TeachingDialog(this);
    connect(teachDialog, &TeachingDialog::signalBtnTeachClicked, this, &MainWindow::slot_buttonTeach_pressed);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete presenter;
}

void MainWindow::setSymbolImage(const QImage& image)
{
    //Установка оригинального и увеличенного изображений символов в окошки
    this->image = image;
    ui->lbl_orig->setPixmap(QPixmap::fromImage(image));
    ui->lbl_image_scaled->setPixmap(QPixmap::fromImage(image.scaledToWidth(ui->lbl_image_scaled->height())));
}

QImage &MainWindow::getImage()
{
    return image;
}

void MainWindow::on_actionOpenSymbolFile_triggered()
{
    //По нажатию кнопки из панелю меню "Открыть файл символа" показываем диалоговое окно выбора файла
    presenter->openSymbolFile();

    //Стираем текущий символ
    ui->lbl_recognized->clear();
}

void MainWindow::on_btn_recognize_clicked()
{
    if(!image.isNull() && !ui->lbl_image_scaled->pixmap()->isNull())
    {
        QChar recogSymbol = presenter->recognizeSymbol();
        ui->lbl_recognized->setText(recogSymbol);
    }
}

void MainWindow::on_btn_clear_clicked()
{
    ui->lbl_image_scaled->clear();
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
    /*net->setInputsCount(inputsCount);
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
    }*/
}

void MainWindow::on_actionReadWeight_triggered()
{
    presenter->readWeightFromFile();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
