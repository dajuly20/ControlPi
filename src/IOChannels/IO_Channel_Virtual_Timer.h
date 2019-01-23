/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Timer.h
 * Author: julian
 *
 * Created on December 18, 2018, 6:58 PM
 */

#ifndef IO_CHANNEL_VIRTUAL_TIMER_H
#define IO_CHANNEL_VIRTUAL_TIMER_H
#include "../../timercppNew.h"
#include "IO_Channel_Virtual.h"
#include <memory>
typedef std::unique_ptr<Timer> TimerPtr;

class IO_Channel_Virtual_Timer : public IO_Channel_Virtual {
public:
    IO_Channel_Virtual_Timer();
    IO_Channel_Virtual_Timer(const IO_Channel_Virtual_Timer& orig);
    virtual ~IO_Channel_Virtual_Timer();
    void trigger(bool _bdata, uint8_t bit_num);
    void test();
    
     
       std::map<int,TimerPtr> powerOnTimers;
       std::map<int,TimerPtr> powerOffTimers;
       
       
    Timer* powerOnDelay ;
    Timer* powerOffDelay;
private:
    //TimerCpp t = TimerCpp();
 

};

#endif /* IO_CHANNEL_VIRTUAL_TIMER_H */

