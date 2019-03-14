/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   KeyUpMessage.h
 * Author: kubatek94
 *
 * Created on 22 March 2016, 17:41
 */

#ifndef KEYUPMESSAGE_H
#define KEYUPMESSAGE_H

#include "Message.h"

namespace Message {
    class KeyUpMessage : public Message {
    public:
        KeyUpMessage() : Message(Message::Type::KEYUP, 1) { }

        void action(VirtualInput& keyboard) {
            jbyte key = buffer.get(2);

            jbyte inputKey = VirtualInput::javaKeyToInputKey(key);

            keyboard.onKeyUp(inputKey);
        }
    };   
}

#endif /* KEYUPMESSAGE_H */

