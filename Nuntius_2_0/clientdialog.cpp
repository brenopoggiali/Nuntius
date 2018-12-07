#include "clientdialog.h"
#include "ui_clientdialog.h"
#include "message.h"
#include "mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QMessageBox>
#include <QPixmap>


using namespace std;

ClientDialog::ClientDialog(std::string serv_addr, int port, std::string nickname,std::string channel_name, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientDialog)
{
    ui->setupUi(this);
    setFixedSize(QSize(1161,654));
    this->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            this->size(),
            qApp->desktop()->availableGeometry()
        )
    );

//    QPixmap pix2(":/img/img/icon_b.png");
//    int w2 = ui->label_logo_2->width();
//    int h2 = ui->label_logo_2->height();
//    ui->label_logo_2->setPixmap(pix2.scaled(w2,h2,Qt::KeepAspectRatio));



    if (port < 1 || port > 65535)
    {
      // TODO: tenho q adicionar outra validacao para caso a porta ja esteja em uso por outro servico
      //throw Exception("Porta invalida [1 ~ 65535]");
    }
    this->_port = port;
    this->_nickname = nickname;
    this->_server_ip = serv_addr;
    this->_message = new Message(this);
    this->_channel_name = channel_name;

    memset(&_server_addr, 0, sizeof(_server_addr));
    _server_addr.sin_family = AF_INET;
    _server_addr.sin_addr.s_addr = inet_addr(this->_server_ip.c_str());
    _server_addr.sin_port = htons(_port);

    this->_cursor = ui->textBrowser->textCursor();
    QObject::connect(this, &ClientDialog::valueChanged,
                       this, &ClientDialog::handler_msg2);


}

ClientDialog::~ClientDialog()
{
    delete ui;
    this->exit_server(-1,true);
}

int ClientDialog::connect_serv()
{

  this->_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(this->_socket == INVALID_SOCKET){
    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
  }

  std::cout << "[*] Trying to connect to the server at " << this->_server_ip << ":" << this->_port << std::endl;


  if (::connect(_socket, (struct sockaddr *)&_server_addr, sizeof(_server_addr)) == SOCKET_ERROR)
  {
    // TODO: melhorar mensagens de erro
    std::cout << "[*] ERROR: Could not connect to the server" << std::endl;
    std::cout << strerror(errno) << " ERROR num: " << errno << std::endl;
    return -1;
  }

  std::cout << "[*] Connection established, waiting for incoming messages..." << std::endl << std::endl;
}

void ClientDialog::send_msg(std::string &msg)
{
  int n = send(this->_socket, msg.c_str(), msg.length(), 0);
  if (n < 0)
  {

    this->exit_server(-1,true);

    ui->statusbar->showMessage("Falha ao enviar a mensagem. Tente novamente.",5000);
    throw Exception("Fail sending message");

  }
}

std::string ClientDialog::recv_msg()
{
  memset(this->_buffer, 0, MAXPACKETSIZE);
  int n = recv(this->_socket, this->_buffer, MAXPACKETSIZE, 0);

  if (n <= 0)
  {
    std::cout << strerror(errno) << "ERROR num: " << errno << std::endl;
    ui->statusbar->showMessage("Falha ao receber mensagem.",5000);
    this->exit_server(-1,true);
    throw Exception("Fail receiving message");
  }
  return std::string(this->_buffer);
}

bool ClientDialog::connect_channel()
{
   std::string channel_is_full = this->_message->map_input_string(Message::CHANNEL_IS_FULL);
   std::string nickname_in_use = this->_message->map_input_string(Message::NICKNAME_IN_USE);


   //std::string FULL = "FULL";
  std::string client_info = this->_nickname + ";" + this->_channel_name;

  this->send_msg(client_info);
  std::string received_msg = this->recv_msg();
  //colocar em um enum depois
  if (received_msg == channel_is_full)
  {
    ui->statusbar->showMessage("O canal está cheio",5000);
    throw Exception("Channel is Full");

  }
  if (received_msg == nickname_in_use)
  {
    ui->statusbar->showMessage("Este nickname já está sendo usado!",5000);
    throw Exception("Nickname already in use");

  }
}


void ClientDialog::handler()
{

  try
  {
     this->connect_channel();
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    this->exit_server(-1,true);
  }

  std::cout << "[*] Connected on channel " << this->_channel_name << std::endl << std::endl;
  ui->statusbar->showMessage("Conectado no canal",5000);
  QString c = QString::fromStdString(this->_channel_name);


  ui->label_channel->setText(c);

  std::thread t(&ClientDialog::msg_receiver, this);
  t.detach();

}
 void ClientDialog::handler_msg(std::string buffer)
 {


    if (buffer.length() >= MAXPACKETSIZE)
    {
      std::cout << "ERROR message must be less than " << MAXPACKETSIZE << " bytes!" << std::endl;
    }
    else
    {
      //aq tenho q implementar sair do server dps e nao sair do programa
      if (buffer == "#EXIT")
      {
        this->exit_server(-1, true);
      }
      else if (buffer[0] == '#')
      {
        this->special_input_handler(buffer);
      }
      else
      {
        this->send_msg(buffer);
      }
    }
  }


 void ClientDialog::exit_server(int error_code, bool should_exit)
 {
   if (error_code != -1)
   {
     std::cout << strerror(error_code) << " ERROR num: " << error_code << std::endl;
   }

   ::close(this->_socket);
   delete this->_message;
   if (should_exit)
   {
     exit(EXIT_SUCCESS);

   }
 }


void *ClientDialog::msg_receiver()
{

  std::string msg;

  while (true)
  {

    try
    {
      msg = this->recv_msg();
    }
    catch (exception &e)
    {
      std::cout << e.what() << std::endl;
      this->exit_server(-1,true);
    }
    std::cout << msg << std::endl;
    this->_msgrec = msg;

    emit valueChanged();


  }
  return 0;
}

void ClientDialog::on_pushButton_clicked()
{
     QString s = ui->lineEdit->text();
     std::string msg = s.toStdString();
     std::string aux = msg;
     aux.erase(remove(aux.begin(), aux.end(), ' '), aux.end());

     if(msg == "#HELP"){

        this->_cursor.insertHtml("<font color=\"Grey\">Use:<br>#JOIN: Para mudar de canal<br>#NICK: Para mudar seu nickname<br>#EXIT: Para deixar a sala e voltar ao menu principal</font>");
        this->_cursor.insertHtml("<br>");
        this->_cursor.movePosition(QTextCursor::End);
        ui->textBrowser->setTextCursor(this->_cursor);
        ui->lineEdit->setText("");

     }

     else if(aux == ""){

         this->_cursor.insertHtml("<font color=\"Grey\">Não é permitido enviar mensagens vazias!</font>");
         this->_cursor.insertHtml("<br>");
         this->_cursor.movePosition(QTextCursor::End);
         ui->textBrowser->setTextCursor(this->_cursor);
         ui->lineEdit->setText("");


     }

     else if(msg == "#EXIT"){

         QMessageBox::StandardButton reply;


         reply = QMessageBox::question(this, "Sair", "Tem certeza que deseja sair da sala e retornar ao menu principal?",
                                       QMessageBox::Yes|QMessageBox::No);
         if (reply == QMessageBox::Yes) {

             this->exit_server(-1,false);
             qApp->quit();
             QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

         } else {

            ui->lineEdit->setText("");
         }

     }

     else{

     QString n = QString::fromStdString(this->_nickname);
     s = ": " + s;
     //s = n + s;

     this->_cursor.insertHtml("<font color=\"DeepPink\">" + n +"</font>");
     this->_cursor.insertHtml("<font color=\"Black\">" + s +"</font>");
     this->_cursor.insertHtml("<br>");
     this->_cursor.movePosition(QTextCursor::End);
     ui->textBrowser->setTextCursor(this->_cursor);



     //ui->textBrowser->append(s);
     this->handler_msg(msg);
     ui->lineEdit->setText("");
     }
}

void ClientDialog::handler_msg2()
{
    std::string msg = this->_msgrec;
    size_t operator_position = msg.find_first_of(": ");
    std::string n = msg.substr(0, operator_position);
    std::string m = msg.substr(operator_position);
    //n = n +":";
    //QString qs = QString::fromStdString(msg);
    QString n1 = QString::fromStdString(n);
    QString m1 = QString::fromStdString(m);
    //ui->textBrowser->append(qs);



    this->_cursor.insertHtml("<font color=\"DeepPink\">" + n1 +"</font>");
    this->_cursor.insertHtml("<font color=\"Black\">" + m1 +"</font>");
    this->_cursor.insertHtml("<br>");
    this->_cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(this->_cursor);





}

void ClientDialog::set_channel_name(std::string &channel_name)
{
  this->_channel_name = channel_name;
}

void ClientDialog::set_nickname(std::string &nickname)
{
  this->_nickname = nickname;
}

void ClientDialog::special_input_handler(std::string &input)
{
  try
  {
    this->_message->input_handler(input);
  }
  catch (InputException &e)
  {
    std::cout << e.what() << std::endl;
  }
  catch (ServerResponseException &e)
  {
    std::cout << e.what() << std::endl;
  }
}

void ClientDialog::on_pushButton_2_clicked()
{

      QMessageBox::StandardButton reply;


      reply = QMessageBox::question(this, "Sair", "Tem certeza que deseja sair da sala e retornar ao menu principal?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {

          this->exit_server(-1,false);
          qApp->quit();
          QProcess::startDetached(qApp->arguments()[0], qApp->arguments());

      } else {


      }



//    this->exit_server(-1,false);
//    qApp->quit();
//    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
//    hide();
//    MainWindow *newmain= new MainWindow();
//    newmain->show();
}
