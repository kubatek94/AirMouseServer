/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MouseMoveMessage.h
 * Author: kubatek94
 *
 * Created on 19 March 2016, 15:40
 */

#ifndef MOUSEMOVEMESSAGE_H
#define MOUSEMOVEMESSAGE_H

#include <iostream>
#include "Message.h"

namespace Message {
    class MouseMoveMessage : public Message {
    public:
        MouseMoveMessage() : Message(Message::Type::MOUSE_MOVE, 2) { }

        MouseMoveMessage(jbyte dx, jbyte dy) : Message(Message::Type::MOUSE_MOVE, 2) {
            setDelta(dx, dy);
        }

        void setDelta(jbyte dx, jbyte dy) {
            buffer.put(dx);
            buffer.put(dy);
        }

        void action(VirtualInput& mouse) {
            jbyte dx = buffer.get(2);
            jbyte dy = buffer.get(3);

            mouse.move(dy, dx);
        }
    };
}

#endif /* MOUSEMOVEMESSAGE_H */

