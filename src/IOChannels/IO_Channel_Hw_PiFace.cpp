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

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(PiFaceDigital* pfd_init) : pfd(pfd_init) {
    ChannelEntitySP output ( new Channel_Entity_PiFace_Outputs(pfd));
    ChannelEntitySP input (  new Channel_Entity_PiFace_Inputs (pfd));
    
     chEntities.insert ( std::make_pair('o',output) );
     chEntities.insert ( std::make_pair('i',input) );
    
}

IO_Channel_Hw_PiFace::IO_Channel_Hw_PiFace(const IO_Channel_Hw_PiFace& orig) {
}

IO_Channel_Hw_PiFace::~IO_Channel_Hw_PiFace() {
//    for (auto chEntity : chEntities) {
//        delete chEntity;
//    }
//    
//    for (std::pair<char,Channel_Entity*> chEntity : chEntities) {
//        delete chEntity.second;
//    }
}

