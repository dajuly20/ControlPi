/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_TimerOutput.h
 * Author: julian
 *
 * Created on 7. Januar 2019, 23:45
 */

#ifndef CHANNEL_ENTITY_TIMEROUTPUT_H
#define CHANNEL_ENTITY_TIMEROUTPUT_H

#include <iostream> // cout
#include <string>   // std::string


#include "Channel_Entity.h"
#include "Channel_Entity_Memory.h"


class Channel_Entity_TimerOutput : public Channel_Entity_Memory{
public:
    Channel_Entity_TimerOutput();
    Channel_Entity_TimerOutput(const Channel_Entity_TimerOutput& orig);
    virtual ~Channel_Entity_TimerOutput();
    
    uint8_t read_pin(uint8_t bit_num){
    
    //  return the Value of the pin by invoking the parent's method. 
    return Channel_Entity_Memory::read_pin(bit_num);
}

void    write_pin(bool _bdata, uint8_t bit_num) override;

private:

};

#endif /* CHANNEL_ENTITY_TIMEROUTPUT_H */

