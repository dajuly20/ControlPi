/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_TimerTrigger.cpp
 * Author: julian
 * 
 * Created on 7. Januar 2019, 22:59
 */

#include "Channel_Entity_TimerTrigger.h"

Channel_Entity_TimerTrigger::Channel_Entity_TimerTrigger(int perm_read_,  int perm_write_ )
{
    perm_read  = perm_read_;
    perm_write = perm_write_; 
};

Channel_Entity_TimerTrigger::Channel_Entity_TimerTrigger(const Channel_Entity_TimerTrigger& orig) {
}

Channel_Entity_TimerTrigger::~Channel_Entity_TimerTrigger() {
}

