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

void MainWindow::slot_buttonTeach_pressed(const TeachArg& args)
{
    presenter->teach(args);
}

void MainWindow::on_actionReadWeight_triggered()
{
    presenter->readWeightFromFile();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}
