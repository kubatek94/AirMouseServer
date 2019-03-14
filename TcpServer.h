/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpServer.h
 * Author: kubatek94
 *
 * Created on 16 March 2016, 13:45
 */

#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdexcept>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>

//c networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "TcpSocket.h"

class TcpServer {
public:
    TcpServer(int port = 5550);
    virtual ~TcpServer();
    void start();
    
    void onClientConnected(std::function<void(std::unique_ptr<TcpSocket>)> clientConnectedCallback);
    
private:
    bool runningServer;
    int server_socket;
    struct sockaddr_in server_addr;
    
    std::function<void(std::unique_ptr<TcpSocket>)> clientConnectedCallback;
};

#endif /* TCPSERVER_H */

