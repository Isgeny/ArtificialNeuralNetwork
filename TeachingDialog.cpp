#include "TeachingDialog.h"
#include "ui_TeachingDialog.h"

TeachingDialog::TeachingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeachingDialog)
{
    ui->setupUi(this);
}

TeachingDialog::~TeachingDialog()
{
    delete ui;
}

void TeachingDialog::on_btn_teach_clicked()
{
    int inputs = ui->lnEditInputs->text().toInt();
    int outputs = ui->lnEditOutputs->text().toInt();
    int hiddenLayCount = ui->lnEditHiddenLayCount->text().toInt();
    QString hidNeuronsCount = ui->txtEditHiddenNeurons->toPlainText();
    int eraCount = ui->lnEditEraCount->text().toInt();
    double nu = ui->lnEditNu->text().toDouble();
    double minMistake = ui->lnEditMinMistake->text().toDouble();

    IntVector hidV;
    QTextStream in(&hidNeuronsCount);
    for(int i = 0; i < hiddenLayCount; i++)
    {
        int temp;
        in >> temp;
        hidV.push_back(temp);
    }

    AFuncVector aFuncV;
    if(ui->chBox_Tanh->isChecked())
        aFuncV.push_back(HYPERBOLIC_TANGENT);
    if(ui->chBox_Logsigm->isChecked())
        aFuncV.push_back(LOG_SIGMOID);
    if(ui->chBox_Softplus->isChecked())
        aFuncV.push_back(SOFTPLUS);
    if(ui->chBox_ReLU->isChecked())
        aFuncV.push_back(RELU);
    if(ui->chBox_RReLU->isChecked())
        aFuncV.push_back(RANDOMIZED_RELU);
    this->close();
    emit signalBtnTeachClicked(inputs, outputs, hiddenLayCount, hidV, aFuncV, eraCount, nu, minMistake);
}

void TeachingDialog::on_btnCancel_clicked()
{
    this->close();
}
