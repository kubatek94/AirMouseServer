/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Message.cpp
 * Author: kubatek94
 * 
 * Created on 16 March 2016, 19:25
 */

#include "Message.h"

namespace Message {
    Message::Message(Message::Type type, int dataLength) : type(type), buffer(dataOffset + dataLength) {
        buffer.put(static_cast<int>(type));
        buffer.put(dataOffset + dataLength);
    }

    Message::Type Message::getType() {
        return type;
    }

    int Message::getLength() {
        return buffer.get(1);
    }

    int Message::getDataLength() {
        return getLength() - dataOffset;
    }

    MessageBuffer* Message::getBuffer() {
        return &buffer;
    }

    void Message::action(VirtualInput& virtualInput) { }   
}