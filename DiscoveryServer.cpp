/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DiscoveryServer.cpp
 * Author: kubatek94
 * 
 * Created on 16 March 2016, 10:30
 */

#include "DiscoveryServer.h"

DiscoveryServer::DiscoveryServer(const std::string& multicastGroup, int port, int advertisePort) 
: multicastGroup(multicastGroup), port(port), advertisePort(advertisePort) {
    memset((char*)&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    addrlen = sizeof(addr);
    
    mreq.imr_multiaddr.s_addr = inet_addr(this->multicastGroup.c_str());         
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    
    memset(&buffer, 0, sizeof(buffer));
    gethostname(&(buffer[2]), sizeof(buffer) - 3);
}

DiscoveryServer::~DiscoveryServer() {
    if (sock > 0) {
        close(sock);
    }
}

void DiscoveryServer::startServer() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        throw std::runtime_error("Cannot create a socket");
    }
    
    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        throw std::runtime_error("Cannot bind the socket to the address");
    }
    
    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        throw std::runtime_error("Cannot join multicast group");
    }
    
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        throw std::runtime_error("Cannot set timeout value for socket");
    }
    
    while (runServer) {
        cnt = recvfrom(sock, buffer, 2, 0, (struct sockaddr *) &addr, (socklen_t*)&addrlen);

        if (cnt > 0) {
            if (buffer[0] == 0x5) {
                cnt = sendto(sock, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, addrlen);
                if (cnt < 0) {
                    throw std::runtime_error("Cannot reply to socket");
                }
            }
        }
    }
}

void DiscoveryServer::start(Type type) {
    if (!runServer) {
        runServer = true;
        
        if (type == Type::ASYNC) {
            server = std::thread(&DiscoveryServer::startServer, this);
        } else {
            startServer();
        }
    }
}
      
void DiscoveryServer::stop() {
    if (runServer) {
        runServer = false;
        
        if (server.joinable()) {
            server.join();
        }
        
        close(sock);
    }
}

