#include "TcpSocket.h"
#include <windows.h>
using namespace std;
TCPSocket::TCPSocket(string _addr,uint16_t _port):m_isConnected(false)
{
    // Initialize Winsock
    int ret = WSAStartup(MAKEWORD(2,2), &m_winsSocketInfo);
    if (ret != 0)
    {
        throw std::runtime_error("WSAStartup failed with error" +to_string(WSAGetLastError()));
    }
    ZeroMemory( &m_addr, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    inet_pton(m_addr.sin_family,_addr.c_str(), & m_addr.sin_addr.s_addr);
    m_addr.sin_port = htons(_port);

    m_socket = socket(m_addr.sin_family,SOCK_STREAM ,IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        throw std::runtime_error("socket failed with error" +to_string(WSAGetLastError()));
    }
}

void TCPSocket::Connect()
{
    int ret=connect(m_socket,(struct sockaddr *)&m_addr,sizeof(m_addr));
    if(ret==SOCKET_ERROR)
    {
        throw std::runtime_error("Unable to connect to the server" +to_string(WSAGetLastError()));
    }
    m_isConnected=true;
    DWORD timeout =50;
    ret=setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    BOOL yes = 1;
    ret = setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&yes, sizeof(BOOL));
}

void TCPSocket::Disconnect()
{
    if(m_isConnected==false)
        return;
    int ret=shutdown(m_socket,SD_SEND);
    if(ret==SOCKET_ERROR)
    {
        cout<<("shutdown failed with error" +to_string(WSAGetLastError()));
    }
    ret=closesocket(m_socket);
    if(ret==SOCKET_ERROR)
    {
        cout<<("closesocket failed with error" +to_string(WSAGetLastError()));
    }
    m_isConnected=false;
}

void TCPSocket::Send(QByteArray _data)
{
    int ret=send(m_socket, _data.data(),_data.size(),0);
    if(ret<0)
    {
        throw std::runtime_error("Unable to send the data");
    }

}

bool TCPSocket::IsConnected()
{
#define DEFAULT_BUFLEN 800
    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN] = "";
    int ret=recv(m_socket,recvbuf, recvbuflen,0);
    if(ret==0)
    {
        return false;
    }
    return true;
}

QByteArray  TCPSocket::Receive()
{
#define DEFAULT_BUFLEN 800
    int recvbuflen = DEFAULT_BUFLEN;
    char recvbuf[DEFAULT_BUFLEN] = "";
    int ret=recv(m_socket,recvbuf, recvbuflen,0);
    if(ret==0)
    {
        throw std::runtime_error("0");
    }
    else if(ret<0)
    {
        throw std::runtime_error("Unable to read the data");
    }
    else
    {
        QByteArray arr(recvbuf,ret);
        return   arr;
    }
}

TCPSocket::~TCPSocket()
{
    WSACleanup();
}

