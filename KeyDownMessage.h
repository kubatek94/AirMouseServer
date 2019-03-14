/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KeyDownMessage.h
 * Author: kubatek94
 *
 * Created on 22 March 2016, 17:41
 */

#ifndef KEYDOWNMESSAGE_H
#define KEYDOWNMESSAGE_H

#include "Message.h"

namespace Message {
    class KeyDownMessage : public Message {
    public:
        KeyDownMessage() : Message(Message::Type::KEYDOWN, 1) { }

        void action(VirtualInput& keyboard) {
            jbyte key = buffer.get(2);

            jbyte inputKey = VirtualInput::javaKeyToInputKey(key);

            keyboard.onKeyDown(inputKey);
        }
    };
}

#endif /* KEYDOWNMESSAGE_H */

