#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QTcpSocket>
#include <QObject>

class ChatModel : public QObject {
    Q_OBJECT
public:
    explicit ChatModel(QObject *parent = nullptr);
    void connectToServer(const QString &host, int port);
    void sendMessage(const QString &message);

signals:
    void messageReceived(const QString &message);

private slots:
    void onReadyRead();

private:
    QTcpSocket *socket;
};

#endif // CHATMODEL_H
