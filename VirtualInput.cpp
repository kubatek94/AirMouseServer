/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VirtualInput.cpp
 * Author: kubatek94
 * 
 * Created on 17 March 2016, 13:29
 */

#include "VirtualInput.h"

VirtualInput::VirtualInput() {
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "virtual-input");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;
}

VirtualInput::~VirtualInput() {
    closeDevice();
}

bool VirtualInput::openDevice() {
    fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        return false;
    }
    
    ioctl(fd, UI_SET_EVBIT, EV_KEY);
    //allow keys from the keyMap
    for (unsigned int i = 0; i < sizeof(keyMap); i++) {
        jbyte key = keyMap[i];
        if (key != 0) {
            ioctl(fd, UI_SET_KEYBIT, key);
        }
    }
    
    ioctl(fd, UI_SET_EVBIT, EV_REL);
    ioctl(fd, UI_SET_EVBIT, EV_SYN);

    ioctl(fd, UI_SET_RELBIT, REL_X);
    ioctl(fd, UI_SET_RELBIT, REL_Y);
    ioctl(fd, UI_SET_RELBIT, REL_WHEEL);

    ioctl(fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(fd, UI_SET_KEYBIT, BTN_MIDDLE);

    if(write(fd, &uidev, sizeof(uidev)) < 0) {
        return false;
    }

    if(ioctl(fd, UI_DEV_CREATE) < 0) {
        return false;
    }
    
    _isOpen = true;
    return _isOpen;
}

void VirtualInput::closeDevice() {
    if (_isOpen) {
        ioctl(fd, UI_DEV_DESTROY);
        close(fd);
        
        _isOpen = false;
    }
}

bool VirtualInput::isOpen() {
    return _isOpen;
}

void VirtualInput::move(int dy, int dx) {
    memset(ev, 0, sizeof(ev));
    
    ev[0].type = EV_REL;
    ev[0].code = REL_Y;
    ev[0].value = dy;

    ev[1].type = EV_REL;
    ev[1].code = REL_X;
    ev[1].value = dx;
        
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;
    
    if(write(fd, &ev, sizeof(ev)) < 0)
        throw std::runtime_error("Cannot move mouse");
}

void VirtualInput::scroll(int val) {
    memset(ev, 0, sizeof(ev));
    
    ev[0].type = EV_REL;
    ev[0].code = REL_WHEEL;
    ev[0].value = val;

    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    
    if(write(fd, &ev, sizeof(struct input_event) * 2) < 0)
        throw std::runtime_error("Cannot scroll mouse");
}

void VirtualInput::click(Button button) {
    memset(ev, 0, sizeof(ev));
    
    ev[0].type = EV_KEY;
    ev[0].code = (button == Button::LEFT) ? BTN_LEFT : BTN_RIGHT;
    ev[0].value = 1;
    
    ev[1].type = EV_KEY;
    ev[1].code = (button == Button::LEFT) ? BTN_LEFT : BTN_RIGHT;
    ev[1].value = 0;
    
    ev[2].type = EV_SYN;
    ev[2].code = SYN_REPORT;
    ev[2].value = 0;
    
    if(write(fd, &ev, sizeof(ev)) < 0)
        throw std::runtime_error("Cannot click mouse");
}

void VirtualInput::onKeyDown(jbyte key) {
    memset(ev, 0, sizeof(ev));
    
    ev[0].type = EV_KEY;
    ev[0].code = key;
    ev[0].value = 1;
    
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    
    if(write(fd, &ev, sizeof(struct input_event) * 2) < 0)
        throw std::runtime_error("Cannot press down a key");
}

void VirtualInput::onKeyUp(jbyte key) {
    memset(ev, 0, sizeof(ev));
    
    ev[0].type = EV_KEY;
    ev[0].code = key;
    ev[0].value = 0;
    
    ev[1].type = EV_SYN;
    ev[1].code = SYN_REPORT;
    ev[1].value = 0;
    
    if(write(fd, &ev, sizeof(struct input_event) * 2) < 0)
        throw std::runtime_error("Cannot release a key");
}

jbyte VirtualInput::javaKeyToInputKey(jbyte key) {
    signed int keyMapSize = (signed int) sizeof(keyMap);
    if (key >= 0 && key < keyMapSize) {
        return keyMap[key];
    }
    return 0;
}

//KEY -> index in keyMap
//maps android KeyEvent keycodes to input key codes, using index from the keyMap
jbyte VirtualInput::keyMap[] = {
    0, 0, 0, 0, 0, 0, 0,
    KEY_0, //7
    KEY_1, //8
    KEY_2, //9
    KEY_3, //10
    KEY_4, //11
    KEY_5, //12
    KEY_6, //13
    KEY_7, //14
    KEY_8, //15
    KEY_9, //16
    KEY_KPASTERISK, //17 KEYCODE_START
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    KEY_A, //29
    KEY_B, //30
    KEY_C, //31
    KEY_D, //32
    KEY_E, //33
    KEY_F, //34
    KEY_G, //35
    KEY_H, //36
    KEY_I, //37
    KEY_J, //38
    KEY_K, //39
    KEY_L, //40
    KEY_M, //41
    KEY_N, //42
    KEY_O, //43
    KEY_P, //44
    KEY_Q, //45
    KEY_R, //46
    KEY_S, //47
    KEY_T, //48
    KEY_U, //49
    KEY_V, //50
    KEY_W, //51
    KEY_X, //52
    KEY_Y, //53
    KEY_Z, //54
    KEY_COMMA, //55
    KEY_DOT, //56
    KEY_LEFTALT, //57
    KEY_RIGHTALT, //58
    KEY_LEFTSHIFT, //59
    KEY_RIGHTSHIFT, //60
    KEY_TAB, //61
    KEY_SPACE, //62
    0, 0, 0,
    KEY_ENTER, //66
    KEY_BACKSPACE, //67
    KEY_GRAVE, //68
    KEY_MINUS, //69
    KEY_EQUAL, //70
    KEY_LEFTBRACE, //71 [
    KEY_RIGHTBRACE, //72 ]
    KEY_BACKSLASH, //73
    KEY_SEMICOLON, //74
    KEY_APOSTROPHE, //75
    KEY_SLASH, //76
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    KEY_ESC, //111
    KEY_DELETE, //112
    KEY_LEFTCTRL, //113
    KEY_RIGHTCTRL, //114
    KEY_CAPSLOCK, //115
    KEY_SCROLLLOCK //116
};