#include "chatserver.h"
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent)
{
    serverSocket = new QTcpServer(this);
    connect(serverSocket, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}

void ChatServer::startServer()
{
    if (!serverSocket->listen(QHostAddress::Any, 1235)) {
        qCritical() << "Server failed to start!";
        return;
    }
    emit serverStarted();
    qDebug() << "Server started on port 1235";
}

void ChatServer::onNewConnection()
{
    QTcpSocket *clientSocket = serverSocket->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &ChatServer::onMessageReceived);
    connect(clientSocket, &QTcpSocket::disconnected, this, &ChatServer::onClientDisconnected);

    // Add the new client socket to the clients list
    clients.append(clientSocket);
    qDebug() << "New client connected";
}

void ChatServer::onMessageReceived()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        QByteArray data = clientSocket->readAll();
        // Broadcast the message to all connected clients
        for (QTcpSocket *socket : clients) {
            if (socket != clientSocket && socket->state() == QTcpSocket::ConnectedState) {
                socket->write(data);
            }
        }
        emit newMessageReceived(QString::fromUtf8(data));
    }
}

void ChatServer::onClientDisconnected()
{
    QTcpSocket *clientSocket = qobject_cast<QTcpSocket *>(sender());
    if (clientSocket) {
        // Remove the client from the clients list when it disconnects
        clients.removeAll(clientSocket);
        clientSocket->deleteLater();
        qDebug() << "Client disconnected";
    }
}
