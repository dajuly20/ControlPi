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

#include "../ChannelEntitys/Channel_Entity.h"
#include "../ChannelEntitys/Channel_Entity_TimerOutput.h"
#include "../ChannelEntitys/Channel_Entity_TimerTrigger.h"

#include "IO_Channel_Virtual_Timer.h"
#include "../../timercppNew.h"
#include <string>

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer() {
     Channel_Entity_TimerTrigger* triggerEntityT = new Channel_Entity_TimerTrigger();
    
     ChannelEntitySP triggerEntity( triggerEntityT);
     ChannelEntitySP outputEntity ( new Channel_Entity_TimerOutput());
    
     chEntities.insert ( std::make_pair('t',triggerEntity) );
     chEntities.insert ( std::make_pair('o',outputEntity) );
  
     
     
    triggerEntityT->registerTrigger(this);
        
    t = Timer();
}

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(const IO_Channel_Virtual_Timer& orig) {
}

IO_Channel_Virtual_Timer::~IO_Channel_Virtual_Timer() {
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
    
    //Channel_Entity t = chEntities['t'];
    //Channel_Entity o  = chEntities['o'];
    
    ChannelEntitySP o = chEntities['o'];
    //Channel_Entity_TimerOutput o = 
    
    //bool t_val = chEntities['t'].read_pin(bit_num);
    bool t_val = _tvalu;
    bool o_val = o->read_pin(bit_num);
   
    
    if(!t_val){
        // Reset powe-on delay timer. 
    }
    
    if(t_val){
        // reset power-off delay timer
    }
    
    
    if(!t_val && !o_val ){
    // Trigger = 0  | Output = 0     
        
    }
    else if(!t_val && o_val){
    // Triffer = 0 | Output = 1    
        
    }
    else if(t_val && !o_val){
    // Trigger = 1 | Output = 0    
    // Power-on delay
        
        
        t.setTimeout([&]() {
            std::cout << "Hey.. After 1s. But I put the output to 1!" << std::endl;
            chEntities['o']->write_pin(1,bit_num);
            //t.stop();
        }, 1000); 
      
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
