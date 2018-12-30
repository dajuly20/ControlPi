/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel.h
 * Author: julian
 *
 * Created on December 18, 2018, 6:42 PM
 */

#ifndef IO_CHANNEL_H
#define IO_CHANNEL_H
//#include "src/Channel_Entities/Channel_Entity.h"
#include "../ChannelEntitys/Channel_Entity.h"
#include <iostream> //cout
#include <memory>   // shared_ptr
#include <map>      // map

typedef std::shared_ptr<Channel_Entity> ChannelEntitySP;

class IO_Channel {
public:
    IO_Channel();
    IO_Channel(const IO_Channel& orig);
    virtual ~IO_Channel();

//    Channel_Entity* operator[](char identifier){
//        Channel_Entity* ent = chEntitiesMap[identifier];
//        std::cout << "TEEEESTLE" << std::endl;
//        return ent;
//        //return chEntitiesMap.operator [](identifier);
//    };
    ChannelEntitySP ge(char identifier){
        return chEntities[identifier];
    }
    
    ChannelEntitySP operator[](char identifier){
        return chEntities[identifier];
    }
    
    bool interrupts_enabled(){
        return false;
    }
   
    int wait_for_interrupt(){
        return 0;
    }
    
    void caching_enable(){};
    void caching_disable(){};
    void flush(){};
    
    
    std::map<char, ChannelEntitySP> chEntities;
 protected:   

};

#endif /* IO_CHANNEL_H */

