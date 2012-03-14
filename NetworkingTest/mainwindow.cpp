#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);   //create UDP Socket(tx)
    rxUdpSocket = new QUdpSocket(this); //create UDP Socket(rx)
    //rxUdpSocket->bind(12345, QUdpSocket::ShareAddress);
     rxUdpSocket->bind(12345); //found in book
    connect(rxUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    findHostIP();
    //Set Tab order for UI
    setTabOrder(ui->rxIPLE, ui->portLE);
    setTabOrder(ui->portLE, ui->rxPortLE);
    setTabOrder(ui->rxPortLE, ui->configButton);
    setTabOrder(ui->configButton, ui->packetLE);
    setTabOrder(ui->packetLE, ui->sendButton);
}

MainWindow::~MainWindow()
{
    rxUdpSocket->close();
    udpSocket->close();
    delete ui;
}

void MainWindow::on_sendButton_clicked()
{
    QString packet = ui->packetLE->text();  //create packet
    QByteArray myDatagram = packet.toUtf8();    //convert to datagram

    if(userIP == true)
    {
        udpSocket->writeDatagram(myDatagram.data(), myDatagram.size(),
                                 txIP, rxPort);
    }
    else
    {
        udpSocket->writeDatagram(myDatagram.data(), myDatagram.size(),
                                     QHostAddress::Broadcast, rxPort);
    }
    //udpSocket->writeDatagram(myDatagram.data(), myDatagram.size(),
                             //QHostAddress::Broadcast, 12345); //working sample code
}

void MainWindow::on_configButton_clicked()
{
    rxPort = ui->portLE->text().toInt();    //set port
    listenPort = ui->rxPortLE->text().toInt();  //set port to listen on

    rxUdpSocket->close();

    rxUdpSocket->bind(listenPort); //found in book


    if(ui->rxIPLE->text() != NULL)  //if user added IP address
    {
    QString txStrIp = ui->rxIPLE->text();  //create RX IP string
    txIP = txStrIp; //Convert to IP Address
    userIP = true;
    }
    else
    {
        userIP = false;
    }

}
void MainWindow::findHostIP()   //found this code online, modified it to only show IP addresses (local)
{
    QStringList items;
    ui->listWidget->clear();
    foreach(QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
       if (interface.flags().testFlag(QNetworkInterface::IsRunning))
         foreach (QNetworkAddressEntry entry, interface.addressEntries())
         {
           if ( interface.hardwareAddress() != "00:00:00:00:00:00" &&     entry.ip().toString().contains("."))
               items << interface.name() + " " + entry.ip().toString();
         }
    }
    ui->listWidget->addItems(items);
}

void MainWindow::processPendingDatagrams()
{
    //int senderPort;   //not needed
    //QHostAddress senderAddress;   //not needed
    QByteArray rxDatagram;
    QString rxString;
    do
    {
        rxDatagram.resize(rxUdpSocket->pendingDatagramSize());
        rxUdpSocket->readDatagram(rxDatagram.data(), rxDatagram.size());
    }
    while(rxUdpSocket->hasPendingDatagrams());
    rxString = (tr("\"%1\"").arg(rxDatagram.data()));   //turn datagram into string
    rxString.remove(QChar('"'), Qt::CaseInsensitive);   //remove quotation marks
    //ui->rxPacketLabel->setText(tr("\"%1\"").arg(rxDatagram.data()));
    ui->rxPacketLabel->setText(rxString);
}
