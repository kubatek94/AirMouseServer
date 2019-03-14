/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpSocket.h
 * Author: kubatek94
 *
 * Created on 16 March 2016, 14:22
 */

#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <cstring>
#include <string>
#include <stdexcept>
#include <iostream>

#include "Message.h"
#include "MessageFactory.h"
#include <thread>
#include <memory>
#include <functional>

//c networking
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class TcpSocket {
public:
    enum class Type { ASYNC, SYNC };
    
    TcpSocket(int socket, struct sockaddr_in address);
    virtual ~TcpSocket();
    
    void start(Type type = Type::ASYNC);
    void stop();
    
    void onMessage(std::function<void(TcpSocket*, std::unique_ptr<Message::Message>)> onMessageCallback);
    
    std::string getAddress();
    
private:
    bool socketOpen = true;
    bool runServer = false;
    void startServer();
    
    std::string ipAddress;
    
    std::thread server;
    int socket;
    struct sockaddr_in address;
    
    std::unique_ptr<Message::Message> getMessage();
    void closeSocket();
    
    std::function<void(TcpSocket*, std::unique_ptr<Message::Message>)> onMessageCallback;
};

#endif /* TCPSOCKET_H */

