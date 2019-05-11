/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_Pipe_RX.cpp
 * Author: julian
 * 
 * Created on 10. Februar 2019, 16:17
 */

#include "Channel_Entity_Pipe_RX.h"


Channel_Entity_Pipe_RX::Channel_Entity_Pipe_RX(const Channel_Entity_Pipe_RX& orig) {
}

Channel_Entity_Pipe_RX::~Channel_Entity_Pipe_RX() {
            

}



void Channel_Entity_Pipe_RX::write_pin(bool _bdata, uint8_t bit_num){   
   bool dbg = false;
    if(dbg) std::cout << " **************************** \n\nWrite pin invoked  " << _bdata << " on bit number" << (int) bit_num << " \n\n*******************" << std::endl;

    // I will crash the program!
     throw std::invalid_argument("Error: Cannot assign readOnly entity with value! (e.g. Pi0=1; in logic.conf  ");
}

void Channel_Entity_Pipe_RX::write_pin_force(bool _bdata, uint8_t bit_num){   
  
    // Call parent write method
    Channel_Entity_Memory::write_pin(_bdata, bit_num);
}
/*
 * 
 * Now defined in Chanel_Entity Baseclass ... See if that works as expected.
void Channel_Entity_Pipe_RX::write_pin_force(bool _bdata, uint8_t bit_num){   
    std::cout << " **************************** \n\nWrite pin invoked  " << _bdata << " on bit number" << (int) bit_num << " \n\n*******************" << std::endl;
    // invoke parent write pin method to really save the pin value
    Channel_Entity_Memory::write_pin(_bdata, bit_num);
}
*/
