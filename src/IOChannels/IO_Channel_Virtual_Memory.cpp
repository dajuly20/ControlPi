/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Memory.cpp
 * Author: julian
 * 
 * Created on December 18, 2018, 6:52 PM
 */

#include "IO_Channel_Virtual_Memory.h"
#include "../ChannelEntitys/Channel_Entity_Memory.h"

IO_Channel_Virtual_Memory::IO_Channel_Virtual_Memory() {
    ChannelEntitySP memoryEntity_a ( new Channel_Entity_Memory());
     chEntities.insert ( std::make_pair('a',memoryEntity_a) );
    
}

IO_Channel_Virtual_Memory::IO_Channel_Virtual_Memory(const IO_Channel_Virtual_Memory& orig) {
}

IO_Channel_Virtual_Memory::~IO_Channel_Virtual_Memory() {
}

