/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity.cpp
 * Author: julian
 * 
 * Created on December 18, 2018, 7:16 PM
 */

#include "Channel_Entity.h"

Channel_Entity::Channel_Entity( int perm_read_ ,  int perm_write_ ) {
    
    perm_read  = perm_read_;
    perm_write = perm_write_; 
}

Channel_Entity::Channel_Entity(const Channel_Entity& orig) {
}

Channel_Entity::~Channel_Entity() {
}

