/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   VirtualInput.h
 * Author: kubatek94
 *
 * Created on 17 March 2016, 13:29
 */

#ifndef VIRTUALINPUT_H
#define VIRTUALINPUT_H

#include <stdexcept>
#include <cstring>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <iostream>

typedef signed char jbyte;

class VirtualInput {
public:
    enum class Button { LEFT, RIGHT };
    
    VirtualInput();
    virtual ~VirtualInput();
    
    bool openDevice();
    void closeDevice();
    
    bool isOpen();
    void move(int dy, int dx);
    void scroll(int val);
    void click(Button button);
    
    void onKeyDown(jbyte key);
    void onKeyUp(jbyte key);
    
    static jbyte javaKeyToInputKey(jbyte key);
    
private:    
    bool _isOpen = false; 
    int fd;
    struct uinput_user_dev uidev;
    struct input_event ev[3];
    
    static jbyte keyMap[117];
};

#endif /* VIRTUALINPUT_H */

