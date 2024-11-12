#include "chatmodel.h"

ChatModel::ChatModel(QObject *parent) : QObject(parent), socket(new QTcpSocket(this)) {
    connect(socket, &QTcpSocket::readyRead, this, &ChatModel::onReadyRead);
}

void ChatModel::connectToServer(const QString &host, int port) {
    socket->connectToHost(host, port);
}

void ChatModel::sendMessage(const QString &message) {
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
    }
}

void ChatModel::onReadyRead() {
    QString message = socket->readAll();
    emit messageReceived("User: " + message);
}
