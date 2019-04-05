/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Hw_PiFace.h
 * Author: julian
 *
 * Created on December 19, 2018, 11:39 PM
 */

#ifndef IO_CHANNEL_HW_PIFACE_H
#define IO_CHANNEL_HW_PIFACE_H
#include "pifacedigitalcpp.h"
#include "../ChannelEntitys/Channel_Entities_PiFace.h"
#include "IO_Channel_Hw.h"
#include "../ConfigParser.h"
#include <memory>   // shared_ptr
typedef std::shared_ptr<PiFaceDigital> PiFacePtr;

class IO_Channel_Hw_PiFace : public IO_Channel_Hw {
 private:
    void init_pfd_object();
     PiFacePtr pfdsp;
     //configEntity* conf = 0;
 
    //IO_Channel_Hw_PiFace();
public:
   
    
    IO_Channel_Hw_PiFace(configEntity* _conf);
        
      
    
    
    //IO_Channel_Hw_PiFace(std::string _token, int _permission) : IO_Channel_Hw_PiFace(){ 
    //    token      = _token;
   //    }
     
    //IO_Channel_Hw_PiFace(PiFaceDigital* pfd_init) ;
    
    //IO_Channel_Hw_PiFace(const IO_Channel_Hw_PiFace& orig);
    virtual ~IO_Channel_Hw_PiFace();
   
    //PiFaceDigital* pfd;
    
     
    bool interrupts_enabled() override;
    bool wait_for_interrupt() override;
    
    void caching_enable() override;
    void caching_disable() override;
    void flush() override; 
    

};

#endif /* IO_CHANNEL_HW_PIFACE_H */

