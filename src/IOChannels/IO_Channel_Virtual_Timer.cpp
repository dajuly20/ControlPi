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
#include "../../globals.h"
#include "../ChannelEntitys/Channel_Entity.h"
#include "../ChannelEntitys/Channel_Entity_TimerOutput.h"
#include "../ChannelEntitys/Channel_Entity_TimerTrigger.h"

#include <thread>
#include <mutex>
#include <condition_variable>

#include "IO_Channel_Virtual_Timer.h"
#include "../../timercppNew.h"
#include <string>

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(){
    Channel_Entity_TimerTrigger* triggerEntityT = new Channel_Entity_TimerTrigger();
    
    ChannelEntitySP triggerEntity( triggerEntityT);
    ChannelEntitySP outputEntity ( new Channel_Entity_TimerOutput());
    
    chEntities.insert ( std::make_pair('t',triggerEntity) );
    chEntities.insert ( std::make_pair('o',outputEntity) );
  
     
     
    triggerEntityT->registerTrigger(this);
        
    t = new Timer();
}

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(const IO_Channel_Virtual_Timer& orig) {
}

IO_Channel_Virtual_Timer::~IO_Channel_Virtual_Timer() {
    delete t;
}




    /*  
     * 
     *  Power-on delay: ( ∆t = 3s = 3*_ )
     *               ________      __
     *  Trigger  ___|        |____|  |___
     *                             < 3
     *               ∆t  ____
     *  Output   _______|    |___________
     *
     * 
     * 
     * 
     *  Power-off delay: ( ∆t = 3s = 3*_ )
     *               ________      __
     *  Trigger  ___|        |____|  |___
     *                             < 3
     *               ___________   _____
     *  Output   ___|           |_|     |___
     * 
     * 
     * 
     *  Power-on and power off delay: ( ∆t = 3s = 3*_ )
     *               ________      __     ___
     *  Trigger  ___|        |____|  |___|   |_____
     *                 > 3        < 3     = 3
     *                  ________              ___
     *  Output   ______|        |____________|   |__
     * 
     * 
     */

void IO_Channel_Virtual_Timer::trigger(bool _tvalu, uint8_t bit_num){
    
    std::string val = _tvalu ? " true" : " false";
    std::string res = "Triggered pin "+ std::to_string(bit_num)+" with " +val;
    std::cout << res;

    ChannelEntitySP o = chEntities['o'];
    
    bool t_val = _tvalu;
    bool o_val = o->read_pin(bit_num);
   
    
    if(!t_val){
        t->stop();
        chEntities['o']->write_pin(0,bit_num);
    }
    
    if(t_val){
        // reset power-off delay timer
    }
   
    if(!t_val && !o_val ){
    // Trigger = 0  | Output = 0     
        std::cout << "Value T=0 O=0" << std::endl;
        
    }
    else if(!t_val && o_val){
    // Triffer = 0 | Output = 1  
        std::cout << "Value went from 1 to zeri, stopping timer." << std::endl;
      
    }
    else if(t_val && !o_val){
    // Trigger = 1 | Output = 0    
    // Power-on delay   
        
        t->setTimeout([this, bit_num]() {
            std::cout << "Hey.. After 1s. But I put the output to 1! BITNUM IS: " << std::to_string( (int)bit_num) << std::endl;
            
            chEntities['o']->write_pin(1,bit_num);
            {
                std::unique_lock<std::mutex> lock{isg->itCondMutex};
                isg->itCondSwitch = true;
                
           }
            isg->itCond.notify_one();
           
        }, 2000); 
      
    }
    else if(t_val && o_val){
    // Trigger = 1 | Output = 1    
        
    }
    
    
//    t.setInterval([&]() {
//        std::cout << "Hey.. After each 1s..." << std::endl;
//    }, 1000); 

    
/*     t.setInterval([&]() {
        cout << "Hey.. After each 1s..." << endl;
    }, 1000); 
*/
 
        // TODO for  now copy value from the "t" = trigger entity to the "o" = output entity. 
    }
