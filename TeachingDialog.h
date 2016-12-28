#ifndef TEACHINGDIALOG_H
#define TEACHINGDIALOG_H

#include "MyDefenitions.h"

namespace Ui {
class TeachingDialog;
}

class TeachingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeachingDialog(QWidget *parent = 0);
    ~TeachingDialog();

public slots:
    void on_btn_teach_clicked();
    void on_btnCancel_clicked();

signals:
    void signalBtnTeachClicked(const TeachArg& args);

private:
    Ui::TeachingDialog* ui;

};

#endif // TEACHINGDIALOG_H
