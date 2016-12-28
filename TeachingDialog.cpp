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
    TeachArg args;
    args.inputsAmount  = ui->lnEditInputs->text().toInt();
    args.outputsAmount = ui->lnEditOutputs->text().toInt();
    args.hidLayAmount  = ui->lnEditHiddenLayCount->text().toInt();
    args.eraAmount     = ui->lnEditEraCount->text().toInt();
    args.nu            = ui->lnEditNu->text().toDouble();
    args.minMistake    = ui->lnEditMinMistake->text().toDouble();

    QString hidNeuronsCount = ui->txtEditHiddenNeurons->toPlainText();
    IntVector hidV;
    QTextStream in(&hidNeuronsCount);
    for(int i = 0; i < args.hidLayAmount; i++)
    {
        int temp;
        in >> temp;
        hidV.push_back(temp);
    }
    args.hidV = hidV;

    if(ui->rB_Tanh->isChecked())
        args.actFunc = HYPERBOLIC_TANGENT;
    if(ui->rB_Logsigm->isChecked())
        args.actFunc = LOG_SIGMOID;
    if(ui->rB_Softplus->isChecked())
        args.actFunc = SOFTPLUS;
    if(ui->rB_ReLU->isChecked())
        args.actFunc = RELU;
    if(ui->rB_RReLU->isChecked())
        args.actFunc = RANDOMIZED_RELU;
    this->close();
    emit signalBtnTeachClicked(args);
}

void TeachingDialog::on_btnCancel_clicked()
{
    this->close();
}
