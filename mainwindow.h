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
    Ui::MainWindow *ui;
    ChatServer *server;
    ChatPresenter *presenter;
    QThread *serverThread;
};

#endif // MAINWINDOW_H
