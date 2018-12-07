#include "nickdialog.h"
#include "ui_nickdialog.h"
#include "mainwindow.h"
#include "clientdialog.h"
#include <sstream>
#include <iostream>
#include <string>
#include <QPixmap>
#include <QMessageBox>
#include <QPalette>
#include <iostream>
#include <cstdlib>

NickDialog::NickDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NickDialog)
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

    QPixmap pix(":/img/img/icon_c.png");
    int w = ui->label_2->width();
    int h = ui->label_2->height();
    ui->label_2->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    ui->label_3->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->lineEdit->setText("Principal");

    ui->label_4->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_2->setText("127.0.0.1");

    ui->label_5->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_3->setText("15050");

    ui->label_6->setVisible(false);
    ui->pushButton_3->setVisible(false);

}

NickDialog::~NickDialog()
{
    delete ui;
}

void NickDialog::on_pushButton_2_clicked()
{

        QString nick2 = ui->line_nick->text();
        std::string nick = nick2.toStdString();
        std::cout << nick << std::endl;


        QString canal= ui->lineEdit->text();
        std::string canal2 = canal.toStdString();


        QString end = ui->lineEdit_2->text();
        std::string end2 = end.toStdString();

        QString porta = ui->lineEdit_3->text();
        std::string porta2 = porta.toStdString();
        int porta3 = stoi(porta2);

        std::string aux = nick;
        aux.erase(remove(aux.begin(), aux.end(), ' '), aux.end());



        if(aux == ""){

            ui->statusbar->showMessage("O seu nickname não pode estar vazio ou conter apenas espaços!",5000);


        }

        else{






        clientDialog = new ClientDialog(end2, porta3, nick, canal2, this);
        int e  = clientDialog->connect_serv();
        if(e == -1){


            //QMessageBox::warning(this,"Status","Não foi possível conectar-se ao servidor");
            ui->statusbar->showMessage("Não foi possível conectar-se ao servidor.Tente novamente.",5000);

        }else{


            clientDialog->handler();
            hide();
            clientDialog->show();




        }

    }

}

void NickDialog::on_pushButton_clicked()
{
    hide();
    MainWindow *newmain= new MainWindow();
    newmain->show();
}

void NickDialog::on_checkBox_stateChanged(int arg1)
{

    if(arg1){

        ui->label_3->setVisible(true);
        ui->lineEdit->setVisible(true);

        ui->label_4->setVisible(true);
        ui->lineEdit_2->setVisible(true);

        ui->label_5->setVisible(true);
        ui->lineEdit_3->setVisible(true);

        ui->label_6->setVisible(true);
        ui->pushButton_3->setVisible(true);

        ui->statusbar->showMessage("Mudar essas configurações pode fazer com que não seja possível conectar-se ao servidor.",4000);



    }else{

        ui->label_3->setVisible(false);
        ui->lineEdit->setVisible(false);

        ui->label_4->setVisible(false);
        ui->lineEdit_2->setVisible(false);

        ui->label_5->setVisible(false);
        ui->lineEdit_3->setVisible(false);

        ui->label_6->setVisible(false);
        ui->pushButton_3->setVisible(false);



    }





}

void NickDialog::on_pushButton_3_clicked()
{

    ui->lineEdit->setText("Principal");
    ui->lineEdit_2->setText("127.0.0.1");
    ui->lineEdit_3->setText("15050");

    ui->statusbar->showMessage("Padrões restaurados. Pronto para ir!",3000);

}
