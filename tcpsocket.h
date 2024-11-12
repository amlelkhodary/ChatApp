#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QtSerialPort/QSerialPort>
#include <iostream>
#include <Windows.h>

class SerialPort:public QObject
{
    Q_OBJECT
    HANDLE m_serial;
    QString m_portName;
    bool m_isClosed;
public:
    SerialPort(std::string _portName);
    void close();
    void writeData(const QByteArray &data);
    QByteArray readData();
    ~SerialPort();
};
