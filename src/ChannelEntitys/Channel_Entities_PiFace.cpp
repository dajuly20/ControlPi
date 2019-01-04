/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entities_PiFace.cpp
 * Author: julian
 * 
 * Created on December 18, 2018, 7:31 PM
 */

#include <pifacedigitalcpp.h>

#include "Channel_Entities_PiFace.h"
// First Channel OUTPUTS
//Channel_Entity_PiFace_Outputs::Channel_Entity_PiFace_Outputs(PiFaceDigital& pfd) {
//    
//}

Channel_Entity_PiFace_Outputs::Channel_Entity_PiFace_Outputs(const Channel_Entity_PiFace_Outputs& orig) {
}

Channel_Entity_PiFace_Outputs::~Channel_Entity_PiFace_Outputs() {
}
   void     Channel_Entity_PiFace_Outputs::write_pin(bool data, uint8_t bit_num){  check_range(bit_num); pfd->write_pin(data, bit_num);}
    uint8_t Channel_Entity_PiFace_Outputs::read_pin(uint8_t bit_num){ check_range(bit_num); return      pfd->read_pin(bit_num, PiFaceDigital::OUT);} 
    uint8_t Channel_Entity_PiFace_Outputs::read_all(){ return pfd->read_byte(PiFaceDigital::OUT);} ;
    void    Channel_Entity_PiFace_Outputs::write_all(uint8_t data){pfd->write_byte(data, PiFaceDigital::OUT);} 
    

// Then Channel INPUTS

//Channel_Entity_PiFace_Inputs::Channel_Entity_PiFace_Inputs() {
//}

Channel_Entity_PiFace_Inputs::Channel_Entity_PiFace_Inputs(const Channel_Entity_PiFace_Inputs& orig) {
}

Channel_Entity_PiFace_Inputs::~Channel_Entity_PiFace_Inputs() {
}



    
//     uint8_t Channel_Entity_PiFace_Inputs::read_pin(uint8_t bit_num){} ;
//     uint8_t Channel_Entity_PiFace_Inputs::read_all(){} ;
//







