/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Pipe.cpp
 * Author: julian
 * 
 * Created on December 18, 2018, 6:57 PM
 */
#include <stdexcept>
#include "../../globals.h"
#include "IO_Channel_Virtual_Pipe.h"
#include "../ChannelEntitys/Channel_Entity.h"
#include "../ChannelEntitys/Channel_Entity_Pipe_RX.h"
#include "../ChannelEntitys/Channel_Entity_Pipe_TX.h"

/*
 permission: 2 digit hex
 * e.g.  
 * 1st digit Receive enttity 'i' => R 
 * 2nd digit Receive entity      => W
 * 3rd digit Transmit entity 'o' => R 
 * 4rd digit Transmit entity 'o' => W 
 * 
 *   Each digit is  0b00, 0b01 or 0b10
 *   static const int exp_public  = 2;
 *   static const int exp_private = 1;
 *   static const int exp_none    = 0;
 *
 *  
 * 
 */



//IO_Channel_Virtual_Pipe::IO_Channel_Virtual_Pipe(std::string _token, int _permission) { //: token(_token), permission(_permission){
//    
//    token = _token;
//    permission = _permission;
//    // Perspective: This program so Receive are "switches" on the Web-Frontend and Transmit are "LED"s on the other side.
//    ChannelEntitySP receiveEntity  ( new Channel_Entity_Pipe_RX(Channel_Entity::exp_public, Channel_Entity::exp_private));
//    ChannelEntitySP transmitEntity ( new Channel_Entity_Pipe_TX(Channel_Entity::exp_public, Channel_Entity::exp_none));
//   
//    chEntities.insert ( std::make_pair('i',receiveEntity) );
//    chEntities.insert ( std::make_pair('o',transmitEntity) );
//}


IO_Channel_Virtual_Pipe::IO_Channel_Virtual_Pipe(configEntity* _conf){
    conf =  _conf; 
    token = conf->private_token;
    
    for(auto const& x  : conf->entity_detail){
        std::cout << x.first << ": Read:" << x.second->perm_read << " Write:" << x.second->perm_write << " Kind: " << x.second->entityKind << std::endl;
        int kind = x.second->entityKind;
        //ChannelEntitySP entity;
        switch(kind){
            case (Channel_Entity::ENTITY_INPUT):
                //entity  ( new Channel_Entity_Pipe_RX(Channel_Entity::exp_public, Channel_Entity::exp_private));
                //chEntities.insert ( std::make_pair(x.first,entity) );
                chEntities.insert ( std::make_pair(x.first, ChannelEntitySP( new Channel_Entity_Pipe_RX(x.second->perm_read, x.second->perm_write))) );   
            break;
                
            case (Channel_Entity::ENTITY_OUTPUT):
                
                chEntities.insert ( std::make_pair(x.first, ChannelEntitySP( new Channel_Entity_Pipe_TX(x.second->perm_read, x.second->perm_write))) );   
            break;
                
            case (Channel_Entity::ENTITY_DUPLEX): // fallthrough
            case (Channel_Entity::ENTITY_ERROR):  // fallthrough
            default: 
                throw std::invalid_argument("Err: Invalid Config for Pipe. Use inputEntityKey or outputEntityKey");    
            break;
                
        }
         
            
        
    }
    
    
    permission = -1;
    
    // Perspective: This program so Receive are "switches" on the Web-Frontend and Transmit are "LED"s on the other side.
    //    
    //    ChannelEntitySP receiveEntity  ( new Channel_Entity_Pipe_RX(Channel_Entity::exp_public, Channel_Entity::exp_private));
    //    ChannelEntitySP transmitEntity ( new Channel_Entity_Pipe_TX(Channel_Entity::exp_public, Channel_Entity::exp_none));
    //   
    //    chEntities.insert ( std::make_pair('i',receiveEntity) );
    //    chEntities.insert ( std::make_pair('o',transmitEntity) );
    
}
//
//IO_Channel_Virtual_Pipe::IO_Channel_Virtual_Pipe(const IO_Channel_Virtual_Pipe& orig) {
//}

IO_Channel_Virtual_Pipe::~IO_Channel_Virtual_Pipe() {
}

