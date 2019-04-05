/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Hw_PiFace.cpp
 * Author: julian
 * 
 * Created on December 19, 2018, 11:39 PM
 */

#include "IO_Channel_Hw_PiFace.h"
#include <utility>
#include <iostream>
#include <functional>
#include "pifacedigitalcpp.h"

uint8_t inputs;


//IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(PiFaceDigital* pfd_init) : pfd(pfd_init) {
//    this->init_pfd_object();
//}

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(configEntity* _conf) {
   // Create Instance of pfd
    conf = _conf;
    token = _conf->private_token;
    // Parameters for PiFace Object.. will soon be  emoved because initialising piface will be moved into IO_Channel_PiFaceDigital.
    static int hw_addr = 0;
    static int enable_interrupts = 1;

    pfdsp = PiFacePtr(new PiFaceDigital(hw_addr, enable_interrupts, PiFaceDigital::EXITVAL_ZERO) );
    
    
    if(!pfdsp->init_success()){
        throw std::invalid_argument("Error: Could not open PiFaceDigital. Is the device properly attached?");
        
    }
    
    this->init_pfd_object();      
}

void IO_Channel_Hw_PiFace::init_pfd_object(){
    
    
     for(auto const& x  : conf->entity_detail){
        std::cout << x.first << ": Read:" << x.second->perm_read << " Write:" << x.second->perm_write << " Kind: " << x.second->entityKind << std::endl;
        int kind = x.second->entityKind;
        //ChannelEntitySP entity;
        switch(kind){
            case (EntityDetails::ENTITY_INPUT):
                {
                ChannelEntitySP input  ( new Channel_Entity_PiFace_Inputs (pfdsp, /*Channel_Entity::exp_none, Channel_Entity::exp_none*/ x.second->perm_read, x.second->perm_write));
                chEntities.insert ( std::make_pair(x.first ,input)  );
                }
            break;
                
            case (EntityDetails::ENTITY_OUTPUT):
                {
                ChannelEntitySP output ( new Channel_Entity_PiFace_Outputs(pfdsp, /*Channel_Entity::exp_none, Channel_Entity::exp_none*/ x.second->perm_read, x.second->perm_write));
                chEntities.insert ( std::make_pair(x.first ,output) );
                }
            break;
                
            case (EntityDetails::ENTITY_DUPLEX): // fallthrough
            case (EntityDetails::ENTITY_ERROR):  // fallthrough
            default: 
                throw std::invalid_argument("Err: Invalid Config for HardwarePiface. Use inputEntityKey or outputEntityKey");    
            break;
                
        }
       
    }
    
//    ChannelEntitySP output ( new Channel_Entity_PiFace_Outputs(pfdsp, Channel_Entity::exp_public, Channel_Entity::exp_none));
//    ChannelEntitySP input  ( new Channel_Entity_PiFace_Inputs (pfdsp, Channel_Entity::exp_public, Channel_Entity::exp_none));
////    
//    chEntities.insert ( std::make_pair('o',output) );
//    chEntities.insert ( std::make_pair('i',input)  );

}

//IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(const IO_Channel_Hw_PiFace& orig) {}

IO_Channel_Hw_PiFace::~IO_Channel_Hw_PiFace() {
    //delete this->pfd;
//    for (auto chEntity : chEntities) {
//        delete chEntity;
//    }
//    
//    for (std::pair<char,Channel_Entity*> chEntity : chEntities) {
//        delete chEntity.second;
//    }
}

   bool IO_Channel_Hw_PiFace::interrupts_enabled(){
        // cast to bool
        return (bool) pfdsp->interrupts_enabled();
    }
   
   bool IO_Channel_Hw_PiFace::wait_for_interrupt(){
        
        //if (pifacedigital_wait_for_input(&inputs, -1, hw_addr) > 0)
        bool ret = (pfdsp->wait_for_input(&inputs, -1) > 0);
        // printf("Input values are: "+inputs);
	//pfd->flush();        
        return ret;
    }
   
   void IO_Channel_Hw_PiFace::caching_enable(){ printf("Caching enable called"); pfdsp->caching_enable(); }
   void IO_Channel_Hw_PiFace::caching_disable(){ printf("Caching disable called"); pfdsp->caching_disable();}
   void IO_Channel_Hw_PiFace::flush(){ pfdsp->flush();}
