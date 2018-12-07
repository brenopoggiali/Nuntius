#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    setFixedSize(QSize(774,436));
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );
    QPixmap pix(":/img/img/icon_a.png");
    int w = ui->label_logo->width();
    int h = ui->label_logo->height();
    ui->label_logo->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    QPixmap pix2(":/img/img/icon_b.png");
    int w2 = ui->label_logo_2->width();
    int h2 = ui->label_logo_2->height();
    ui->label_logo_2->setPixmap(pix2.scaled(w2,h2,Qt::KeepAspectRatio));

//    QPixmap pix3("C:/Users/Luiz Felipe/Documents/Nuntius_Beta/img/icon_c.png");
//    int w3 = ui->label_logo_3->width();
//    int h3 = ui->label_logo_3->height();
//    ui->label_logo_3->setPixmap(pix3.scaled(w3,h3,Qt::KeepAspectRatio));

    //this->setStyleSheet("background-image: url(C:/Users/Luiz Felipe/Documents/Nuntius_Beta/img/NM2.png);");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

//   SecDialog secDeialog;
//   secDeialog.setModal(true);
//   secDeialog.exec();

    hide();
    nickDialog = new NickDialog(this);
    nickDialog->show();

}
