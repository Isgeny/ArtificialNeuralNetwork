#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TeachingDialog.h"
#include "NeuralNet.h"
#include "Presenter.h"

namespace Ui {
class MainWindow;
}

class Presenter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSymbolImage(const QImage& image);
    QImage& getImage();

public slots:
    void slot_buttonTeach_pressed(int inputsCount, int outputsCount, int hidLayCount,
                                  const IntVector& hidV, const AFuncVector& aFuncV,
                                  int eraCount, double nu, double minMistake);

private slots:
    void on_actionOpenSymbolFile_triggered();
    void on_actionReadWeight_triggered();
    void on_actionExit_triggered();
    void on_actionTeach_triggered();
    void on_btn_recognize_clicked();
    void on_btn_clear_clicked();

private:
    Ui::MainWindow* ui;
    QImage image;
    Presenter* presenter;
    TeachingDialog* teachDialog;

};

#endif // MAINWINDOW_H
