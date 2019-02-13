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
#include <mutex>

class Channel_Entity {
public:
    Channel_Entity();
    Channel_Entity(const Channel_Entity& orig);
    virtual ~Channel_Entity();
    virtual uint8_t read_pin(uint8_t bit_num){                  throw std::invalid_argument("Must override read_pin() ");}
    virtual void   write_pin(bool data, uint8_t bit_num){       throw std::invalid_argument("Must override write_pin()");}
    virtual void   write_pin_force(bool data, uint8_t bit_num){ write_pin(data, bit_num);} // Delegated to write pin, unless overridden
    virtual uint8_t read_all(){                                 throw std::invalid_argument("Must override read_all() ");};
    virtual void    write_all(uint8_t data){                    throw std::invalid_argument("Must override write_all()");};
    virtual void    write_all_force(uint8_t data){              write_all(data);}; // Delegated to write pin, unless overridden
    
    
    void check_range( int bit_num){
 
        if( 
                (bit_num >= width) ||    //width is max_bit +1
                (bit_num <  0   )
           )    throw std::invalid_argument("Bit " + std::to_string(bit_num)  + " exceeds bit range of 0 - " +  std::to_string(width-1));
    };
  
protected:
    const bool read_only  = false;
    const uint8_t width   = 8;
    char name;
    std::mutex entity_mtx;

   
};

#endif /* CHANNEL_ENTITY_H */

