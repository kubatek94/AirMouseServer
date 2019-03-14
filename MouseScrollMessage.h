/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MouseScrollMessage.h
 * Author: kubatek94
 *
 * Created on 21 March 2016, 11:40
 */

#ifndef MOUSESCROLLMESSAGE_H
#define MOUSESCROLLMESSAGE_H

#include "Message.h"

namespace Message {
    class MouseScrollMessage : public Message {
    public:
        MouseScrollMessage() : Message(Message::Type::MOUSE_SCROLL, 1) { }
        void action(VirtualInput& mouse) {
            jbyte val = buffer.get(2);
            mouse.scroll(val);
        }
    };   
}

#endif /* MOUSESCROLLMESSAGE_H */

