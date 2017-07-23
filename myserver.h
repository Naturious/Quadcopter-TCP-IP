#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

class MyServer : public QObject
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    bool start(); //Server starts listening for connections from any IP address on port 55
    void close(); //Server stops listening for incoming connections
    bool listening(); //Returns true if server started listening for connections
    bool isConnected(); //Returns true if server is connected to a client, false if not
    bool sendData(const int &data); //Send a byte of data through the current connection if connected
signals:
    void newConnectionSignal();
    void endedConnectionSignal();
public slots:
    void newConnection();
    void endedConnection();
private:
    QTcpServer *server;
    QTcpSocket *socket;
    bool connection_established;
    bool started_listening;

};

#endif // MYSERVER_H
