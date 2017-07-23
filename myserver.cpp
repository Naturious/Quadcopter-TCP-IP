#include "myserver.h"

MyServer::MyServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    connection_established = false;

}

bool MyServer::start()
{
    if(server -> listen(QHostAddress::Any,55)) //Start listening for any IP address on port 55
    {
        started_listening = true;
        return true;
    }
    else
    {
        return false;
    }
}

void MyServer::close()
{
    server->close();
    if(isConnected())
        socket->close();
    started_listening = false;
}

bool MyServer::listening()
{
    return started_listening;
}

void MyServer::newConnection()
{
    socket = server->nextPendingConnection();

    connect(socket,SIGNAL(disconnected()),this,SLOT(endedConnection()));

    qDebug() << "Connected!";
    connection_established = true;
    emit newConnectionSignal();
}

bool MyServer::isConnected()
{
    return connection_established;
}

bool MyServer::sendData(const int &data)
{
    const char toSend = data;
    bool res = (socket->write(&toSend,1))>-1;
    socket->flush();
    return res;
}

void MyServer::endedConnection()
{
    connection_established = false;
    qDebug() << "Disconnected!";
    emit endedConnectionSignal();
}
