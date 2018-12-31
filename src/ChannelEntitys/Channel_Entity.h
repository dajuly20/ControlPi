/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity.h
 * Author: julian
 *
 * Created on December 18, 2018, 7:16 PM
 */

#ifndef CHANNEL_ENTITY_H
#define CHANNEL_ENTITY_H
#include "stdint.h"
#include <stdexcept>

class Channel_Entity {
public:
    Channel_Entity();
    Channel_Entity(const Channel_Entity& orig);
    virtual ~Channel_Entity();
    virtual uint8_t read_pin(uint8_t bit_num){                  throw std::invalid_argument("AAA must override read_pin() ");}
    virtual void   write_pin(bool data, uint8_t bit_num){       throw std::invalid_argument("AAA must override write_pin()");}
    
    virtual uint8_t read_all(){                                 throw std::invalid_argument("AAA must override read_all() ");};
    virtual void    write_all(uint8_t data){                    throw std::invalid_argument("AAA must override write_all()");};
    
  
protected:
    bool read_only;
    uint8_t width;
    char name;
    

};

#endif /* CHANNEL_ENTITY_H */
