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

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(PiFaceDigital* pfd_init) : pfd(pfd_init) {
    this->init_pfd_object();
}

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace() {
   // Create Instance of pfd
    
    // Parameters for PiFace Object.. will soon be  emoved because initialising piface will be moved into IO_Channel_PiFaceDigital.
    static int hw_addr = 0;
    static int enable_interrupts = 1;

    PiFaceDigital* _pfd = new PiFaceDigital(hw_addr, enable_interrupts, PiFaceDigital::EXITVAL_ZERO);
    this->pfd = _pfd;
    
    if(!pfd->init_success()){
        throw std::invalid_argument("Error: Could not open PiFaceDigital. Is the device properly attached?");
        
    }
    
    this->init_pfd_object();      
}

void IO_Channel_Hw_PiFace::init_pfd_object(){
    ChannelEntitySP output ( new Channel_Entity_PiFace_Outputs(pfd));
    ChannelEntitySP input  ( new Channel_Entity_PiFace_Inputs (pfd));
    
    chEntities.insert ( std::make_pair('o',output) );
    chEntities.insert ( std::make_pair('i',input)  );
 
}

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(const IO_Channel_Hw_PiFace& orig) {
}

IO_Channel_Hw_PiFace::~IO_Channel_Hw_PiFace() {
    delete this->pfd;
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
        return (bool) pfd->interrupts_enabled();
    }
   
   bool IO_Channel_Hw_PiFace::wait_for_interrupt(){
        
        //if (pifacedigital_wait_for_input(&inputs, -1, hw_addr) > 0)
        bool ret = (pfd->wait_for_input(&inputs, -1) > 0);
        // printf("Input values are: "+inputs);
	//pfd->flush();        
        return ret;
    }
   
   void IO_Channel_Hw_PiFace::caching_enable(){ printf("Caching enable called"); pfd->caching_enable(); }
   void IO_Channel_Hw_PiFace::caching_disable(){ printf("Caching disable called"); pfd->caching_disable();}
   void IO_Channel_Hw_PiFace::flush(){ pfd->flush();}
