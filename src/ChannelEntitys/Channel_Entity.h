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
#include <iostream>


class Channel_Entity {
public:
    
    int entityType; 
    
    static const int ENTITY_INPUT = 1;
    static const int ENTITY_OUTPUT = 2;
    static const int ENTITY_DUPLEX = 3;
    static const int ENTITY_ERROR = -1;
    
    static const int exp_public  = 2;
    static const int exp_private = 1;
    static const int exp_none    = 0;

    static const int op_write = 2;
    static const int op_read  = 4;
    
    
    Channel_Entity( int perm_read_ = Channel_Entity::exp_none,  int perm_write_ = Channel_Entity::exp_none);
    Channel_Entity(const Channel_Entity& orig);
    virtual ~Channel_Entity();
    virtual uint8_t read_pin(uint8_t bit_num){                  throw std::invalid_argument("Must override read_pin() ");}
    virtual void   write_pin(bool data, uint8_t bit_num){       throw std::invalid_argument("Must override write_pin()");}
    virtual void   write_pin_force(bool data, uint8_t bit_num){ write_pin(data, bit_num);} // Delegated to write pin, unless overridden
    virtual uint8_t read_all(){                                 throw std::invalid_argument("Must override read_all() ");};
    virtual void    write_all(uint8_t data){                    throw std::invalid_argument("Must override write_all()");};
    virtual void    write_all_force(uint8_t data){              write_all(data);}; // Delegated to write pin, unless overridden
    
    
    int get_perm_read(){
        return perm_read;
    }
    
    int get_perm_write(){
        return perm_write;
    }
   
    bool checkPermission(int operation, bool authorized){
        
        std::cout << "Operation: " << operation << " perm_read: " << perm_read << " perm_write: " << perm_write << std::endl;
        std::cout << (authorized ? " authorized " : " not authorized ") << std::endl;
      
        
        switch(operation){
        
            case op_read: 
                return       (perm_read == exp_public
                        || ((perm_read == exp_private) && authorized));
            break;
                
            case op_write:
                std::cout << "Write " << std::endl;
                return       (perm_write == exp_public
                        || ((perm_write == exp_private) && authorized));
            break;
                
            default:
                throw std::invalid_argument("Error: checkPermission unrecognized operation.");
                return false;
                // Error
            
        }
        
       
    }
    
    
    
    void check_range( int bit_num){
 
        if( 
                (bit_num >= width) ||    //width is max_bit +1
                (bit_num <  0   )
           )    throw std::invalid_argument("Bit " + std::to_string(bit_num)  + " exceeds bit range of 0 - " +  std::to_string(width-1));
    };
  

    int perm_read;
    int perm_write;
protected:

    
    const bool read_only  = false;
    const uint8_t width   = 8;
    char name;
    std::mutex entity_mtx;

   
};

#endif /* CHANNEL_ENTITY_H */

