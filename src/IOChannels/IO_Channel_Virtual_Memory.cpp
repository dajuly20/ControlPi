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
#include "../ConfigParser.h"

IO_Channel_Virtual_Memory::IO_Channel_Virtual_Memory(configEntity* _conf) {
    conf = _conf;
    token = _conf->private_token;
    
    for(auto const& x  : conf->entity_detail){
        std::cout << x.first << ": Read:" << x.second->perm_read << " Write:" << x.second->perm_write << " Kind: " << x.second->entityKind << std::endl;
        int kind = x.second->entityKind;
        //ChannelEntitySP entity;
        switch(kind){
            case (Channel_Entity::ENTITY_DUPLEX):
                {
                ChannelEntitySP memoryEntity_a ( new Channel_Entity_Memory());
                chEntities.insert ( std::make_pair(x.first,memoryEntity_a) );

                }
            break;
                
            case (Channel_Entity::ENTITY_OUTPUT):
            case (Channel_Entity::ENTITY_INPUT): // fallthrough
            case (Channel_Entity::ENTITY_ERROR):  // fallthrough
            default: 
                throw std::invalid_argument("Err: Invalid Config for HardwarePiface. Use inputEntityKey or outputEntityKey");    
            break;
                
        }
       
    }
//    ChannelEntitySP memoryEntity_a ( new Channel_Entity_Memory());
//     chEntities.insert ( std::make_pair('a',memoryEntity_a) );
//    
}

IO_Channel_Virtual_Memory::IO_Channel_Virtual_Memory(const IO_Channel_Virtual_Memory& orig) {
}

IO_Channel_Virtual_Memory::~IO_Channel_Virtual_Memory() {
}

