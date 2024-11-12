#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QThread>
#include "chatserver.h"
#include "chatpresenter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onServerStarted();
    void onNewMessageReceived(const QString &message);
    void onConnectButtonClicked();
    void onSendButtonClicked();
    void onTextChanged(const QString &text);

private:
    Ui::MainWindow *ui; //To access the widgets inside the ui
    ChatServer *server; //instance from the chat server class
    ChatPresenter *presenter; //instance from the presenter class
    QThread *serverThread; //To start a new thread for the server
};

#endif // MAINWINDOW_H
