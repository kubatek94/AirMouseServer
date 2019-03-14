/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KeepAliveMessage.h
 * Author: kubatek94
 *
 * Created on 21 March 2016, 07:36
 */

#ifndef KEEPALIVEMESSAGE_H
#define KEEPALIVEMESSAGE_H

#include "Message.h"

namespace Message {
    class KeepAliveMessage : public Message {
    public:
        KeepAliveMessage() : Message(Message::Type::KEEP_ALIVE, 0) { }
    };   
}

#endif /* KEEPALIVEMESSAGE_H */

