/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_Pipe_RX.h
 * Author: julian
 *
 * Created on 10. Februar 2019, 16:17
 */



#ifndef CHANNEL_ENTITY_PIPE_RX_H
#define CHANNEL_ENTITY_PIPE_RX_H
#include "Channel_Entity_Memory.h"
#include <stdexcept> // throw
#include <iostream> // cout

class Channel_Entity_Pipe_RX : public Channel_Entity_Memory {
public:
    Channel_Entity_Pipe_RX();
    Channel_Entity_Pipe_RX(const Channel_Entity_Pipe_RX& orig);
    virtual ~Channel_Entity_Pipe_RX();
    void write_pin(bool _bdata, uint8_t bit_num) override; 
    void write_pin_force(bool _bdata, uint8_t bit_num);
private:

};

#endif /* CHANNEL_ENTITY_PIPE_RX_H */

