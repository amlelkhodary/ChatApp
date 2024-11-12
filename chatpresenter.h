#ifndef CHATPRESENTER_H
#define CHATPRESENTER_H

#include <QObject>
#include <QTcpSocket>

class ChatPresenter : public QObject
{
    Q_OBJECT

public:
    explicit ChatPresenter(QObject *parent = nullptr);
    void handleConnect(const QString &ip, int port);
    void sendMessage(const QString &message);

signals:
    void newMessageReceived(const QString &message);  // Signal for new messages

private slots:
    void onMessageReceived();  // Slot for handling incoming messages

private:
    QTcpSocket *clientSocket;  // Declare the QTcpSocket
};

#endif // CHATPRESENTER_H
