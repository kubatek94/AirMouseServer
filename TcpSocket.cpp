/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TcpSocket.cpp
 * Author: kubatek94
 * 
 * Created on 16 March 2016, 14:22
 */

#include "TcpSocket.h"

TcpSocket::TcpSocket(int socket, struct sockaddr_in address) : socket(socket), address(address) {}

TcpSocket::~TcpSocket() {
    stop();
}

void TcpSocket::start(Type type) {
    if (!runServer) {
        runServer = true;
        
        if (type == Type::ASYNC) {
            server = std::thread(&TcpSocket::startServer, this);
        } else {
            startServer();
        }   
    }
}

void TcpSocket::stop() {
    if (runServer) {
        runServer = false;
        
        if (server.joinable()) {
            server.join();
        }
        
        closeSocket();
    }
}

void TcpSocket::startServer() {
    while (runServer) {
        //read message
        try {
            std::unique_ptr<Message::Message> message = getMessage();
            if (onMessageCallback) {
                onMessageCallback(this, std::move(message));
            }
        } catch (std::runtime_error e) {
            runServer = false;
            closeSocket();
            
            std::cout << "Client " << getAddress() << " disconnected because: " << e.what() << std::endl;
        }
    }
}

std::unique_ptr<Message::Message> TcpSocket::getMessage() {
    int readBytes = 0;
    jbyte header[2];
    
    //read the header
    readBytes = read(socket, &header, 2);
    if (readBytes < 0 || readBytes != 2) {
        throw std::runtime_error("Cannot read message header");
    }
    
    std::unique_ptr<Message::Message> message = Message::MessageFactory::makeMessage(static_cast<Message::Message::Type>((int)header[0]), (int)header[1]);
    if (message->getDataLength() > 0) {
        MessageBuffer* buffer = message->getBuffer();
                
        buffer->resize(message->getLength());
        readBytes = read(socket, &(buffer->getBuffer().data()[2]), message->getDataLength());
        buffer->setWritePos(message->getLength());
        
        if (readBytes < 0 || readBytes != message->getDataLength()) {
            throw std::runtime_error("Cannot read message body");
        }
    }
    
    return message;
}

void TcpSocket::closeSocket() {
    if (socketOpen) {
        socketOpen = false;
        close(socket);
    }
}

void TcpSocket::onMessage(std::function<void(TcpSocket*,std::unique_ptr<Message::Message>)> onMessageCallback) {
    this->onMessageCallback = onMessageCallback;
}

std::string TcpSocket::getAddress() {
    if (ipAddress.empty()) {
        char* ip = inet_ntoa(address.sin_addr);
        ipAddress = std::string(ip);
    }
    
    return ipAddress;
}
