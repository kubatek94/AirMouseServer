/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiscoveryServer.h
 * Author: kubatek94
 *
 * Created on 16 March 2016, 10:30
 */

#ifndef DISCOVERYSERVER_H
#define DISCOVERYSERVER_H

#include <string>
#include <sstream> 
#include <cstring>
#include <exception>
#include <stdexcept>
#include <iostream>
#include <thread>

//c networking
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class DiscoveryServer {
public:
    enum class Type { ASYNC, SYNC };
    
    DiscoveryServer(const std::string& multicastGroup="239.0.0.1", int port = 5555, int advertisePort = 5550);
    virtual ~DiscoveryServer();
    
    void start(Type type);
    void stop();
private:
    bool runServer = false;
    std::thread server;
    void startServer();
    
    std::string multicastGroup;
    int port;
    int advertisePort;
    
    struct sockaddr_in addr;
    struct ip_mreq mreq;
    int addrlen, cnt, sock = -1;
    
    char buffer[27];
};

#endif /* DISCOVERYSERVER_H */

