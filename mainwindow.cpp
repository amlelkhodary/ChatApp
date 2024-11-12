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
    ui->inputField->setPlaceholderText("Type something..."); // Set placeholder text for the input field
    ui->chatArea->setReadOnly(true); // Set the chat area (QTextEdit) to read-only so users can't edit it

    // Ask the user whether to start as server or client
    QStringList options;
    options << "Server and Client" << "Client Only";
    bool ok;
    QString choice = QInputDialog::getItem(this, "Dialog Window", "Select mode:", options, 0, false, &ok);

    if(ok){ //if ok in the input dialog is pressed
        ui->connectButton->setEnabled(true);
        ui->sendButton->setEnabled(false);
        ui->inputField->setEnabled(false);

        presenter = new ChatPresenter(this);
        connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::onConnectButtonClicked); //if connect button clicked, connect the user to server and activate the line edit to start typing
        connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked); //if send button is clicked while typing, send the message
        connect(ui->inputField, &QLineEdit::returnPressed, this, &MainWindow::onSendButtonClicked); //if Enter is pressed while typing, send the message
        connect(ui->inputField, &QLineEdit::textChanged, this, &MainWindow::onTextChanged); //if user started typing, activate send button
        connect(presenter, &ChatPresenter::newMessageReceived, this, &MainWindow::onNewMessageReceived); //if new message received, send it to the other user

        if (choice == "Server and Client") { //if the choice contais the server also
            serverThread = new QThread(this); // Start server in a separate thread
            server = new ChatServer(); //the server constuctor creates a server socket and make it listening for any clients
            server->moveToThread(serverThread);
            connect(serverThread, &QThread::started, this, &MainWindow::onServerStarted);  //if the server thread started, create a message box and enable the connect button for clients
            connect(server, &ChatServer::newMessageReceived, this, &MainWindow::onNewMessageReceived); //When new message arrived to the server, then forward it to the other client

            serverThread->start(); //Like joining the thread with the main thread
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onServerStarted()
{
    QMessageBox::information(this, "Server Started", "Server is now running on port 1235"); //when server started, create a message box that shows information about the server
    ui->connectButton->setEnabled(true); //Enable the connect button
}

void MainWindow::onNewMessageReceived(const QString &message)
{
    // Check if the message is not empty and does not start with "Me:"
    if (!message.isEmpty() && !message.startsWith("Me:")) {
        ui->chatArea->append(message); // Display the message in the chat area as "User: <message>"
    }
}

void MainWindow::onConnectButtonClicked()
{
    QString ip = "127.0.0.1";  // Server IP
    int port = 1235;           // Server Port
    presenter->handleConnect(ip, port);  // Try to connect to the server
    ui->inputField->setEnabled(true); //Enable the input field to start typing
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
