#include "chatserver.h"
#include <QDebug>

ChatServer::ChatServer(QObject *parent) : QObject(parent)
{
    serverSocket = new QTcpServer(this);
    connect(serverSocket, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
    if (!serverSocket->listen(QHostAddress::Any, 1235)) {
        qCritical() << "Server failed to start!";
        return;
    }
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
    // Identify which client sent the message
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    if (!senderSocket) return;

    QByteArray data = senderSocket->readAll();

    // Broadcast the message to all clients except the sender
    for (QTcpSocket *client : clients) {
        if (client != senderSocket && client->state() == QTcpSocket::ConnectedState) {
            client->write(data); // Send the message to the client
        }
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
