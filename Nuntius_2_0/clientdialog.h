#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QMainWindow>
#include <QTextCursor>
#include <QMetaObject>
#include <QStyle>
#include <QDesktopWidget>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <thread>
#include <errno.h>

#include "exception.h"

#define MAXPACKETSIZE 4096
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

using namespace std;

class Message;

namespace Ui {
class ClientDialog;
}

class ClientDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientDialog(std::string addr, int port, std::string nickname, std::string channel_name, QWidget *parent = 0);
    ~ClientDialog();
    int connect_serv();
    void handler();
    void exit_server(int error_code, bool should_exit);
    void send_msg(std::string& msg);
    std::string recv_msg();
    bool connect_channel();
    void set_nickname(std::string &nickname);
    void set_channel_name(std::string &channel_name);
    void special_input_handler(std::string &input);

    void *msg_receiver();
    void handler_msg(std::string buffer);

private slots:
    void on_pushButton_clicked();
    void handler_msg2();

    void on_pushButton_2_clicked();

signals:
    void valueChanged();


private:
    Ui::ClientDialog *ui;
    int _socket;
    int _port;
    std::string _server_ip;
    std::string _nickname;
    char _buffer[MAXPACKETSIZE];
    std::string _channel_name;
    struct sockaddr_in _server_addr;
    QTextCursor _cursor;
    std::string _msgrec;
    Message *_message;
};

#endif // CLIENTDIALOG_H
