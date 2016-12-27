#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TeachingDialog.h"
#include "NeuralNet.h"
#include <QXmlStreamReader>
#include <QDomDocument>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void listElements(QDomElement root, QString tagname, QString attribute);
    void readXMLFile(const QString& filename);
    void writeMatrixToFile(const DoubleMatrix& m, const QString& filename);

public slots:
    void on_actionOpen_triggered();
    void on_actionRecognize_triggered();
    void on_btn_recognize_clicked();
    void on_btn_clear_clicked();
    void on_actionTeach_triggered();
    void slot_buttonTeach_pressed(int inputsCount, int outputsCount, int hidLayCount,
                                  const IntVector& hidV, const AFuncVector& aFuncV,
                                  int eraCount, double nu, double minMistake);

private slots:
    void on_actionReadWeight_triggered();
    void on_actionRecognizePercent_triggered();
    void on_actionExit_triggered();
    void on_actionClean_triggered();

private:
    Ui::MainWindow *ui;
    QImage image;
    TeachingDialog *teachDialog;
    NeuralNet *net;

};

#endif // MAINWINDOW_H
