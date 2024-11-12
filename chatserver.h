#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class ChatServer : public QObject
{
    Q_OBJECT

public:
    explicit ChatServer(QObject *parent = nullptr);
    void startServer();

signals:
    void serverStarted();
    void newMessageReceived(const QString &message);

private slots:
    void onNewConnection();
    void onMessageReceived();
    void onClientDisconnected();

private:
    QTcpServer *serverSocket;   // The server socket
    QList<QTcpSocket*> clients; // List of connected clients
};

#endif // CHATSERVER_H
