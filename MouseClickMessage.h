/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MouseClickMessage.h
 * Author: kubatek94
 *
 * Created on 21 March 2016, 12:42
 */

#ifndef MOUSECLICKMESSAGE_H
#define MOUSECLICKMESSAGE_H

#include "Message.h"

namespace Message {
    class MouseClickMessage : public Message {
    public:
        MouseClickMessage() : Message(Message::Type::MOUSE_CLICK, 1) { }

        void action(VirtualInput& mouse) {
            VirtualInput::Button button = static_cast<VirtualInput::Button>(buffer.get(2));
            mouse.click(button);
        }
    };
}

#endif /* MOUSECLICKMESSAGE_H */

