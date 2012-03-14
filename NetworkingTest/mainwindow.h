#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNetwork/qhostaddress.h"
#include "QtNetwork/qudpsocket.h"
#include "QtNetwork/qabstractsocket.h"
//#include "QStandardItem"
#include "QNetworkInterface"
#include "QHostAddress"
#include "QtNetwork"


namespace Ui {
    class MainWindow;
}

class QUdpSocket;
//class QHostInfo;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_sendButton_clicked();
    void findHostIP();
    void on_configButton_clicked();
    void processPendingDatagrams();
private:
    Ui::MainWindow *ui;
    QUdpSocket *udpSocket;
    QUdpSocket *rxUdpSocket;
    int rxPort;
    int listenPort;
    QHostAddress txIP;
    bool userIP;
    //QHostAddress hostIP;
    //QString hostStrIP;
    QString rxString;
};

#endif // MAINWINDOW_H
