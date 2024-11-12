#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include <QObject>

class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);
    void connectToServer(const QString &host, quint16 port);
    void sendMessage(const QString &message);

private slots:
    void onMessageReceived();

private:
    QTcpSocket *clientSocket;
};

#endif // CHATCLIENT_H
