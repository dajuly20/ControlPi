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
#include <algorithm>

IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(){
    Channel_Entity_TimerTrigger* triggerEntityT = new Channel_Entity_TimerTrigger();
    
    ChannelEntitySP triggerEntity( triggerEntityT);
    ChannelEntitySP outputEntity ( new Channel_Entity_TimerOutput());
   
    chEntities.insert ( std::make_pair('t',triggerEntity) );
    chEntities.insert ( std::make_pair('o',outputEntity) );
  
    for(int i=0; i<=7; i++){
        powerOnTimers   .insert( std::make_pair(i,new Timer()));
        powerOffTimers  .insert( std::make_pair(i,new Timer()));
    }
            
    triggerEntityT->registerTrigger(this);
        
}

/*
#                                                                                                                  
# timer = T0                                                                                                                     
# powerOnDelay  = 1000
# powerOffDelay = 0      
*/             
bool IO_Channel_Virtual_Timer::setTimersCfg(std::vector<std::string> const* cfgLines){
    
    std::string delimiter   = "=";
    size_t      found;
     
    int         timer_context = -1;
    int         cfglineNr = 0;
    
    // Zero initialise timer configs.
    for(int i; i<=7; i++){
        powerOffTimersCfg[i]  =  0;
        powerOnTimersCfg[i]   =  0;
    }
    

   for (std::string ln: (*cfgLines)){ 
   cfglineNr++; // Count up before iteration, so first line=1    
       // make all lowercase
       std::transform(ln.begin(), ln.end(), ln.begin(), ::tolower);
       
       if ((found = ln.find('=')) != std::string::npos){ 
           std::string key    = ln.substr(0,found);
           std::string value  = ln.substr(found+1, std::string::npos);
           
           if(key == "timer"){
               if(value.substr(0,1) == "t"){
                   timer_context = std::stoi(value.substr(1,std::string::npos));
                   if(timer_context < 0 || timer_context > 7){
                       throw std::invalid_argument("Malformed timer-config on line: #" + std::to_string(cfglineNr) + " Timer out of range (0-7) "); 
                   }
               }
               else{
                  throw std::invalid_argument("Malformed timer-config on line: #" + std::to_string(cfglineNr) + "line timer= must start with a T folowed by 1 Digit e.g. timer=T0 "); 
               }
           }
           else if(key == "powerondelay"){
               if(timer_context < 0) throw std::invalid_argument("Malformed timer-config on line: #" + std::to_string(cfglineNr) + "no timer number specified"); 
               unsigned long intval = std::stoul(value); // MAX = 2147483647
               powerOnTimersCfg[timer_context]  =  intval;
           }
           else if(key == "poweroffdelay"){
               if(timer_context < 0) throw std::invalid_argument("Malformed timer-config on line: #" + std::to_string(cfglineNr) + "no timer number specified"); 
               unsigned long intval = std::stoul(value); // MAX = 2147483647
               powerOffTimersCfg[timer_context]  =  intval;
           }
           else{
              throw std::invalid_argument("Malformed timer-config on line: #" + std::to_string(cfglineNr) + " Key " + key + " unknown. Candidates are timer, powerOnDelay, powerOffDelay");  
           }
       }
   }
    return true;
};



IO_Channel_Virtual_Timer::IO_Channel_Virtual_Timer(const IO_Channel_Virtual_Timer& orig) {
}

IO_Channel_Virtual_Timer::~IO_Channel_Virtual_Timer() {
    //delete powerOnDelay;
    //delete powerOffDelay;
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
    
    // If the Output was 0 already, the power-on timer not yet expired and will be stopped.
    // Output is 0 already, no need to re-set to zero.
    // Also as the output has not yet gone to 1, we don't want to have a power off delay.
    if(!t_val && !o_val ){
            powerOnTimers[bit_num]->stop();
    }
    
    // If Output is still 1 at this point, the power on timer is expired already
    // and does not need to be stopped. But Trigger went to zero. So trigger power-off delay!
    else if(!t_val && o_val){
         if(powerOffTimersCfg[bit_num] == 0){ // If delay is 0 make direct write. 
            chEntities['o']->write_pin(0,bit_num);
        }
        else{
            // Todo: Power off delay here!
            std::cout << "\n\n\nPOWER OFF TIMER# " << bit_num << " is :" << powerOffTimersCfg[bit_num] << std::endl;
            powerOffTimers[bit_num]->setTimeout([this, bit_num]() {
                pthread_setname_np(pthread_self(), "PowerOffTimer");
                std::cout << "Hey.. After 2s. I power off the bit./ BITNUM IS: " << std::to_string( (int)bit_num) << std::endl;

                chEntities['o']->write_pin(0,bit_num);
                {
                    std::unique_lock<std::mutex> lock{isg->itCondMutex};
                    std::cout << "Timer Power off " << bit_num << " was fired" << std::endl;
                    isg->itCondSwitch = true;

               }
                isg->itCond.notify_all();

            }, std::chrono::milliseconds{powerOffTimersCfg[bit_num]}); 


            }
    }
    else if(t_val && !o_val){
    // Trigger = 1 | Output = 0    
    // Power-on delay   
    if(powerOnTimersCfg[bit_num] == 0){ // If delay is 0 make direct write. 
        chEntities['o']->write_pin(1,bit_num);
    }
    else{
        
    
            std::cout << "\n\n\nPOWER ON TIMER# " << bit_num << " is :" << powerOnTimersCfg[bit_num] << std::endl;
            powerOnTimers[bit_num]->setTimeout([this, bit_num]() {
                pthread_setname_np(pthread_self(), "PowerOnTimer");
                std::cout << "Hey.. After 1s. But I put the output to 1! BITNUM IS: " << std::to_string( (int)bit_num) << std::endl;

                chEntities['o']->write_pin(1,bit_num);
                {
                    std::unique_lock<std::mutex> lock{isg->itCondMutex};
                    std::cout << "Timer Power on " << bit_num << " was fired" << std::endl;
                    isg->itCondSwitch = true;

               }
                isg->itCond.notify_all();

            }, std::chrono::milliseconds{powerOnTimersCfg[bit_num]}); 

        }
    }
    // Trigger Value went from 0 -> 1 while Output's still 1.
    // Smells like power-off delay's still not run out... 
    // But as Trigger is 1 again, we can now stop it..
    else if(t_val && o_val){
        powerOffTimers[bit_num]->stop();
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
