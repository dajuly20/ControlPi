/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_TimerTrigger.h
 * Author: julian
 *
 * Created on 7. Januar 2019, 22:59
 */

#ifndef CHANNEL_ENTITY_TIMERTRIGGER_H
#define CHANNEL_ENTITY_TIMERTRIGGER_H

#include "Channel_Entity.h"
#include "Channel_Entity_Memory.h"
#include "../IOChannels/IO_Channel_Virtual_Timer.h"

#include <iostream> // cout
#include <string>   // std::string



class Channel_Entity_TimerTrigger : public Channel_Entity_Memory{
public:
    Channel_Entity_TimerTrigger();
    Channel_Entity_TimerTrigger(const Channel_Entity_TimerTrigger& orig);
    virtual ~Channel_Entity_TimerTrigger();

     
    
    void registerTrigger(IO_Channel_Virtual_Timer* _timer){
        timer = _timer;
    }
    
uint8_t read_pin(uint8_t bit_num){
   
    //  return the Value of the pin by invoking the parent's method. 
    return Channel_Entity_Memory::read_pin(bit_num);
}

void  write_pin(bool _bdata, uint8_t bit_num){   
   // invoke only if this bit has changed its value.
    if(read_pin(bit_num) != _bdata){
        std::cout << "####\n\nBIt number :" << std::to_string(bit_num) << " was triggered! \n\n###" << std::endl;
        timer->trigger( _bdata, bit_num);
        // invoke parent write pin method to really save the pin value
        Channel_Entity_Memory::write_pin(_bdata, bit_num);
    }
}

private:
    IO_Channel_Virtual_Timer* timer;
};

#endif /* CHANNEL_ENTITY_TIMERTRIGGER_H */

