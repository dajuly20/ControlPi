/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Timer.cpp
 * Author: julian
 * 
 * Created on December 18, 2018, 6:58 PM
 */

#include "../ChannelEntitys/Channel_Entity_TimerOutput.h"
#include "../ChannelEntitys/Channel_Entity_TimerTrigger.h"

#include "IO_Channel_Virtual_Timer.h"
//#include "timercpp.h"
#include <string>

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer() {
     Channel_Entity_TimerTrigger* triggerEntityT = new Channel_Entity_TimerTrigger();
    
     ChannelEntitySP triggerEntity( triggerEntityT);
     ChannelEntitySP outputEntity ( new Channel_Entity_TimerOutput());
    
     chEntities.insert ( std::make_pair('t',triggerEntity) );
     chEntities.insert ( std::make_pair('o',outputEntity) );
  
     
     
        triggerEntityT->registerTrigger(this);
}

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(const IO_Channel_Virtual_Timer& orig) {
}

IO_Channel_Virtual_Timer::~IO_Channel_Virtual_Timer() {
}



void IO_Channel_Virtual_Timer::trigger(bool _bdata, uint8_t bit_num){
    
    std::string val = _bdata ? " true" : " false";
    std::string res = "Triggered pin "+ std::to_string(bit_num)+" with " +val;
    std::cout << res;

/*     t.setInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, 1000); 
*/
 
        // TODO for  now copy value from the "t" = trigger entity to the "o" = output entity. 
    }
