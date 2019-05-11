/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_TimerOutput.cpp
 * Author: julian
 * 
 * Created on 7. Januar 2019, 23:45
 */

#include "Channel_Entity_TimerOutput.h"

Channel_Entity_TimerOutput::Channel_Entity_TimerOutput(int perm_read_,  int perm_write_ )
{
    entityType = Channel_Entity::ENTITY_OUTPUT;
    perm_read  = perm_read_;
    perm_write = perm_write_; 
};

Channel_Entity_TimerOutput::Channel_Entity_TimerOutput(const Channel_Entity_TimerOutput& orig) {
}

Channel_Entity_TimerOutput::~Channel_Entity_TimerOutput() {
}


void Channel_Entity_TimerOutput::write_pin(bool _bdata, uint8_t bit_num){   
    std::cout << " **************************** \n\nWrite pin invoked  " << _bdata << " on bit number" << (int) bit_num << " \n\n*******************" << std::endl;
    // invoke parent write pin method to really save the pin value
    Channel_Entity_Memory::write_pin(_bdata, bit_num);
}