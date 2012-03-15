#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);   //create UDP Socket(tx)
    rxUdpSocket = new QUdpSocket(this); //create UDP Socket(rx)
    rxUdpSocket->bind(12345); //temporarily bind to port
    findHostIP();   //find out the local host's IP

    //Set Tab order for UI
    setTabOrder(ui->rxIPLE, ui->lePortTX);
    setTabOrder(ui->lePortTX, ui->lePortRX);
    setTabOrder(ui->lePortRX, ui->configButton);
    setTabOrder(ui->configButton, ui->packetLE);
    setTabOrder(ui->packetLE, ui->sendButton);

    //Connect signals and slots
    connect(rxUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
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
                                 txIP, txPort);
    }
    else
    {
        udpSocket->writeDatagram(myDatagram.data(), myDatagram.size(),
                                     QHostAddress::Broadcast, txPort);
    }
}

void MainWindow::on_configButton_clicked()
{
    txPort = ui->lePortTX->text().toInt();    //set port
    listenPort = ui->lePortRX->text().toInt();  //set port to listen on

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
    ui->rxPacketLabel->setText(rxString);

    //Add datagram to the log
    QString timeString;
    QTime *time = new QTime;
    time->start();  //have the QTime get the current time
    timeString = time->toString("hh:mm:ss.zzz");    //return the current time as a string
    QString packetString;
    packetString.append("(");
    packetString.append(timeString);    //add the current time
    packetString.append(")");
    packetString.append('\t');  //add a tab
    packetString.append(rxString);  //add the packet
    ui->teLog->append(packetString);    //add the packet with timestamp to the packet log
}
