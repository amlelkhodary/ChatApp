#include "chatpresenter.h"
#include <QDebug>

ChatPresenter::ChatPresenter(QObject *parent) : QObject(parent)
{
    clientSocket = new QTcpSocket(this);  // Initialize the QTcpSocket
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatPresenter::onMessageReceived);  // Connect signal
}

void ChatPresenter::handleConnect(const QString &ip, int port)
{
    clientSocket->connectToHost(ip, port);  // Attempt to connect to the server
}

void ChatPresenter::sendMessage(const QString &message)
{
    if (clientSocket->state() == QTcpSocket::ConnectedState) {
        // Prefix the message with "User:" to identify as a client message
        QString formattedMessage = "User: " + message;
        clientSocket->write(message.toUtf8());  // Send the message to the server
    }
}

void ChatPresenter::onMessageReceived()
{
    QByteArray data = clientSocket->readAll();  // Read incoming data from the socket
    emit newMessageReceived(QString::fromUtf8(data));  // Emit the message to the main window
}
