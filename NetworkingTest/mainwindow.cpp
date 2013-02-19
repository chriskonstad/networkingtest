/*
    Copyright (C) 2012  Chris Konstad (chriskon149@gmail.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "flickcharm.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);   //create UDP Socket(tx)
    rxUdpSocket = new QUdpSocket(this); //create UDP Socket(rx)
    rxUdpSocket->bind(12345, QUdpSocket::ShareAddress); //temporarily bind to port
    findHostIP();   //find out the local host's IP

    autoSender = new QTimer(this);

    ui->packetLE->setInputMethodHints(Qt::ImhNoPredictiveText); //fixes bug that keeps qlineedit from recording keystrokes

    //Set Tab order for UI
    setTabOrder(ui->rxIPLE, ui->lePortTX);
    setTabOrder(ui->lePortTX, ui->lePortRX);
    setTabOrder(ui->lePortRX, ui->configButton);
    setTabOrder(ui->configButton, ui->packetLE);
    setTabOrder(ui->packetLE, ui->sendButton);

    logFlick.activateOn(ui->teLog);
    ipListFlick.activateOn(ui->listWidget);

    //Connect signals and slots
    connect(rxUdpSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    connect(autoSender, SIGNAL(timeout()), this, SLOT(on_sendButton_clicked()));    //connect the timer to the send button

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

    autoSender->setInterval(ui->sbTime->value()*1000.0);    //convert from seconds to milliseconds
    if(ui->cbEnable->isChecked())
    {
        autoSender->start();
    }
    else
    {
        autoSender->stop();
    }

    rxUdpSocket->close();

    rxUdpSocket->bind(listenPort, QUdpSocket::ShareAddress); //found in book

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

void MainWindow::on_cbEnable_toggled(bool checked)
{
    if(checked)
        ui->cbEnable->setText("Disable");
    else
        ui->cbEnable->setText("Enable");
}
