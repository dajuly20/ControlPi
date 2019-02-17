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
#include "../../globals.h"
#include "IO_Channel_Virtual_Pipe.h"
#include "../ChannelEntitys/Channel_Entity.h"
#include "../ChannelEntitys/Channel_Entity_Pipe_RX.h"
#include "../ChannelEntitys/Channel_Entity_Pipe_TX.h"

IO_Channel_Virtual_Pipe::IO_Channel_Virtual_Pipe(std::string _token, int _permission) { //: token(_token), permission(_permission){
    
    token = _token;
    permission = _permission;
    // Perspective: This program so Receive are "switches" on the Web-Frontend and Transmit are "LED"s on the other side.
    ChannelEntitySP receiveEntity  ( new Channel_Entity_Pipe_RX(Channel_Entity::exp_public, Channel_Entity::exp_private));
    ChannelEntitySP transmitEntity ( new Channel_Entity_Pipe_TX(Channel_Entity::exp_public, Channel_Entity::exp_none));
   
    chEntities.insert ( std::make_pair('i',receiveEntity) );
    chEntities.insert ( std::make_pair('o',transmitEntity) );
}

IO_Channel_Virtual_Pipe::IO_Channel_Virtual_Pipe(const IO_Channel_Virtual_Pipe& orig) {
}

IO_Channel_Virtual_Pipe::~IO_Channel_Virtual_Pipe() {
}

