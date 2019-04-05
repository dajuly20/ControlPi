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
#include "../../iterationSwitchGuard.h"
#include <iostream> //cout
#include <memory>   // shared_ptr
#include <map>
#include <vector>      // map
#include <string>   // std::string
#include "../WebSocket/websocket_session.hpp"
#include "../ConfigParser.h"

typedef std::shared_ptr<Channel_Entity> ChannelEntitySP;

class IO_Channel {
public:
    std::map<char, ChannelEntitySP> chEntities;
    configEntity* conf;
    //configEntity* conf;
    iterationSwitchGuard* isg;
    void assignIsg(iterationSwitchGuard* _isg){
        isg = _isg;
    }
    IO_Channel();
    //IO_Channel(configEntity* _conf);
    
    IO_Channel(std::string _token, int _permission) : token(_token), permission(_permission){ }

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
    
    virtual bool interrupts_enabled(){
        return false;
    }
   
    virtual bool wait_for_interrupt(){
        return false;
    }
    
    virtual void caching_enable(){};
    virtual void caching_disable(){};
    virtual void flush(){};
    
    const int& getPermission(bool authorized){
        return permission;
    }
    
    bool checkToken(std::string& token_is){
        if(this->token == Config::NO_TOKEN) return false;
        return (token_is == this->token);
    }
    
    bool is_session_autorized(websocket_session* session){
        //std::cout << "Sessions authorized:" << authorized_sessions.size() << "Session: " << (int) session <<  std::endl;
        return (std::find(authorized_sessions.begin(), authorized_sessions.end(), session) != authorized_sessions.end());
        
        for(websocket_session* authorized_session : authorized_sessions){
            if(authorized_session == session){
                std::cout << "FOUND! " << std::endl;
                return true;
            }
            else{
                std::cout << "NOT found given session " << session << " != " << authorized_session << std::endl;
            }
        }
    }
    
    void add_authorized_session(websocket_session* session){
        authorized_sessions.push_back(session);
        //std::cout << "Sessions authorized:" << authorized_sessions.size() << "Session: " << (int) session <<  std::endl;
    }
    
    const int u_r  = 0x400;
    const int u_w  = 0x200;
    const int u_x  = 0x100;
    
    const int g_r  = 0x40;
    const int g_w  = 0x20;
    const int g_x  = 0x10;
    
    const int o_r = 0x4;
    const int o_w = 0x2;
    const int o_x = 0x1;
    
    
 protected:   
  
     std::vector<websocket_session*> authorized_sessions;
     
    /* Like Unix File-rights, but only two digits, and no execute bit is ignored-
     * Having token       = user (owner)
     * Having global key  = group 
     * Not having token   = other (world) 
     * 
     * read           = allowed to read
     * write          = allowed to write
     * execute Other  = broadcast value automatically to all sessions.
     *
     *  rwx = 7
     * rw- = 6
     * r   = 4
     * 
     */
    
    //int right = u_r & u_w & o_r; // User read, User write and other read.
    
    std::string token = "";
    int permission = 0;

};

#endif /* IO_CHANNEL_H */

