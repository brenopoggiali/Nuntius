#ifndef NICKDIALOG_H
#define NICKDIALOG_H

#include <QMainWindow>
#include <QStyle>
#include <QDesktopWidget>
#include "clientdialog.h"

namespace Ui {
class NickDialog;
}

class NickDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit NickDialog(QWidget *parent = nullptr);
    ~NickDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_3_clicked();

private:
    Ui::NickDialog *ui;
    ClientDialog *clientDialog;
};

#endif // NICKDIALOG_H
