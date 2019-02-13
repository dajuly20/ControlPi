/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_Memory.cpp
 * Author: julian
 * 
 * Created on 6. Januar 2019, 20:05
 */

#include "Channel_Entity_Memory.h"

Channel_Entity_Memory::Channel_Entity_Memory() {
    
}

Channel_Entity_Memory::Channel_Entity_Memory(const Channel_Entity_Memory& orig) {
}

Channel_Entity_Memory::~Channel_Entity_Memory() {
}


uint8_t Channel_Entity_Memory::read_pin(uint8_t bit_num){
    std::unique_lock<std::mutex> lock{entity_mtx}; 
    return (data >> bit_num) & 1;
}

void    Channel_Entity_Memory::write_pin(bool _bdata, uint8_t bit_num){   
    std::unique_lock<std::mutex> lock{entity_mtx}; 
    if (_bdata) {
        data |= 1 << bit_num; // set
    } else {
        data &= 0xff ^ (1 << bit_num); // clear
    }
}


uint8_t Channel_Entity_Memory::read_all(){
    std::unique_lock<std::mutex> lock{entity_mtx}; 
    return data;
}

void    Channel_Entity_Memory::write_all(uint8_t _data){
    std::unique_lock<std::mutex> lock{entity_mtx}; 
    data = _data;   
}

