#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatserver.h"
#include "chatpresenter.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set placeholder text for the input field
    ui->inputField->setPlaceholderText("Type something...");

    // Set the chat area (QTextEdit) to read-only so users can't edit it
    ui->chatArea->setReadOnly(true);

    // Ask the user whether to start as server or client
    QStringList options;
    options << "Server and Client" << "Client Only";
    bool ok;
    QString choice = QInputDialog::getItem(this, "Start Mode", "Select mode:", options, 0, false, &ok);

    ui->connectButton->setEnabled(true);
    ui->sendButton->setEnabled(false);
    ui->inputField->setEnabled(false);

    if (ok && choice == "Server and Client") {
        // Start server in a separate thread
        serverThread = new QThread(this);
        server = new ChatServer();
        server->moveToThread(serverThread);
        connect(serverThread, &QThread::started, server, &ChatServer::startServer);
        connect(server, &ChatServer::newMessageReceived, this, &MainWindow::onNewMessageReceived);
        connect(server, &ChatServer::serverStarted, this, &MainWindow::onServerStarted);
        serverThread->start();

        // Start client automatically and connect to the same server
        presenter = new ChatPresenter(this);
        connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
        connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
        connect(ui->inputField, &QLineEdit::returnPressed, this, &MainWindow::onSendButtonClicked);
        connect(ui->inputField, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    } else if (ok && choice == "Client Only") {
        // Start only client mode, connect to a running server
        presenter = new ChatPresenter(this);
        connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked);
        connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
        connect(ui->inputField, &QLineEdit::returnPressed, this, &MainWindow::onSendButtonClicked);
        connect(ui->inputField, &QLineEdit::textChanged, this, &MainWindow::onTextChanged);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onServerStarted()
{
    QMessageBox::information(this, "Server Started", "Server is now running on port 1235");
    ui->connectButton->setEnabled(true);
}

void MainWindow::onNewMessageReceived(const QString &message)
{
    // Do not display the message sent by the current client
    if (message.startsWith("Me:")) {
        return;  // Ignore the current user's own messages
    }

    // If the message is from another client or server, display it with "User: "
    ui->chatArea->append("User: " + message);
}

void MainWindow::onConnectButtonClicked()
{
    QString ip = "127.0.0.1";  // Server IP
    int port = 1235;           // Server Port
    presenter->handleConnect(ip, port);  // Try to connect to the server

    // Enable UI elements after connecting
    ui->inputField->setEnabled(true);
    // ui->sendButton->setEnabled(true);
}

void MainWindow::onSendButtonClicked()
{
    QString message = ui->inputField->text();
    if (!message.isEmpty()) {
        // Only send message with "Me: "
        presenter->sendMessage(message);
        ui->chatArea->append("Me: " + message);  // Append message with "Me: "
        ui->inputField->clear();  // Clear the input field after sending the message
    }
}

void MainWindow::onTextChanged(const QString &text)
{
    // Enable the send button only if there is text in the input field
    ui->sendButton->setEnabled(!text.isEmpty());
}
