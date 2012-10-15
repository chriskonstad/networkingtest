#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/*
  Written by Chris Konstad
  Started in July/August of 2011

  As far as licensing goes...
  Clause 0.) You can use this program for anything legal
  Clause 1.) It's not my fault if something goes wrong and I am not liable for anything done by this program or with this program
  Clause 2.) If you want to distribute this program, you have to distribute everything that _you_ got with it
  Clause 3.) If you want to copy something, you've got to give me credit. If you modified it, you have to give me credit for the base code.
   -License modified by "gjsmo"'s proposed license from here: http://www.codinghorror.com/blog/2007/04/pick-a-license-any-license.html
  */

#include <QMainWindow>
#include <QtNetwork/qhostaddress.h>
#include <QtNetwork/qudpsocket.h>
#include <QtNetwork/qabstractsocket.h>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QtNetwork>
#include <QTime>
#include <QTimer>
#include "flickcharm.h"

namespace Ui {
    class MainWindow;
}

class QUdpSocket;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();   //!< Send the datagram
    void findHostIP();  //!< Find out the local host's IP
    void on_configButton_clicked(); //!< Configure the ports
    void processPendingDatagrams(); //!< Display recieved packets
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;  //socket used for sending
    QUdpSocket *rxUdpSocket;    //socket used for receiving
    int txPort;
    int listenPort;
    QHostAddress txIP;
    bool userIP;
    QTimer *autoSender;
    FlickCharm flickCharm;
};

#endif // MAINWINDOW_H
