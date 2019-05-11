/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_AccesWrapper.cpp
 * Author: julian
 * 
 * Created on 25. Dezember 2018, 21:46
 */

#include "IO_Channel_AccesWrapper.h"
#include <stdexcept> // throw
#include "src/IOChannels/IO_Channel.h"
#include "src/ChannelEntitys/Channel_Entity.h"


#include <iostream> // cout
#include <string>   // std::string
#include <vector>

std::map<char,IOChannelPtr> IO_Channel_AccesWrapper::io_channels;

IO_Channel_AccesWrapper::IO_Channel_AccesWrapper(){};

IO_Channel_AccesWrapper::IO_Channel_AccesWrapper(iterationSwitchGuard* _isg){
    isg = _isg;
    
}

IO_Channel_AccesWrapper::IO_Channel_AccesWrapper(const IO_Channel_AccesWrapper& obj){
    this->isg = obj.isg;
}

IO_Channel_AccesWrapper& IO_Channel_AccesWrapper::operator=(const IO_Channel_AccesWrapper& obj){
    this->isg = obj.isg;
}


// Throws on invalid option. Do check with is_valid() first!
IO_Channel_AccesWrapper& IO_Channel_AccesWrapper::operator[](char a){
        //cout << "Value is " << a;
    if(options.size() == 0 ){
        // New identifier is Channel , check if exists.
        if(IO_Channel_AccesWrapper::io_channels.count(a)){
            options.push_back(a);
            return *this;
        }
    }
    else if(options.size() == 1){
        if((*IO_Channel_AccesWrapper::io_channels[options[0]]).chEntities.count(a)){
            options.push_back(a);
            return *this;
        }
    }
    
    // TODO Throw error 
    throw std::invalid_argument("Error: IO_Channel_AccessWrapper invalid option "+a);
     
    }

  uint8_t IO_Channel_AccesWrapper::operator[](int a){
        //cout << "THE PIN Value is " << a << endl;
        uint8_t ret =  (*IO_Channel_AccesWrapper::io_channels[options[0]])[options[1]]->read_pin(a);
        options.clear();
        return ret;
    }
  
  bool IO_Channel_AccesWrapper::is_valid(char& io_Chnl, char& io_Entity){
      if(!IO_Channel_AccesWrapper::io_channels                       .count(io_Chnl))   return false;
      if(!(*IO_Channel_AccesWrapper::io_channels[io_Chnl]).chEntities.count(io_Entity)) return false;
       
     
      return true;
  }

  bool IO_Channel_AccesWrapper::is_valid(char& io_Chnl){
      if(!IO_Channel_AccesWrapper::io_channels                       .count(io_Chnl))   return false;
      return true;
  }
  
ChannelEntitySP IO_Channel_AccesWrapper::operator->(){
   
    if(options.size() != 2){
        std::cerr << "Size is:" << options.size();
        throw std::invalid_argument("Error: Array must have two Dimensions."); 
    }
    
    auto ret = (*IO_Channel_AccesWrapper::io_channels[options[0]])[options[1]];
    options.clear();
    return ret;
}

void IO_Channel_AccesWrapper::insert(std::pair<char, IOChannelPtr> pair){
    
    pair.second->assignIsg(isg);
    IO_Channel_AccesWrapper::io_channels.insert(std::move(pair));
}


// Loops over all Channels and sets them to zero. 
void IO_Channel_AccesWrapper::setZero(){
    for (auto const& channel : IO_Channel_AccesWrapper::io_channels)
    {
        for (auto const& entity : channel.second->chEntities)
        {
            entity.second->write_all_force(0);
        }
    }

}


std::vector<std::string> IO_Channel_AccesWrapper::permissionStatus(websocket_session* session){
    std::stringstream outbuf;
    std::vector<std::string> out;
    for (auto const& channel : IO_Channel_AccesWrapper::io_channels)
    {
        
        for (auto const& entity : channel.second->chEntities)
        {
            outbuf.str("");
            // Format:  H:i:read:write:authorized
            outbuf 
                    <<  channel.first 
                    << ":" << entity.first 
                    << ":" << entity.second->get_perm_read()
                    << ":" << entity.second->get_perm_write()
                    << ":" << (channel.second->is_session_autorized(session) ? "true" : "false");
            out.push_back(outbuf.str());
        }
    }
    
    return out;

}
//ChannelEntitySP IO_Channel_AccesWrapper::getFirstInput(){
//    if(options.size() != 1){
//       std::cerr << "Size is:" << options.size();
//       throw std::invalid_argument("Error: Array must have one dimen"); 
//    }
//       
//    for (auto const& entity : this->getIOChnl()->chEntities){
//        if(entity->entityType == Channel_Entity::ENTITY_INPUT){
//            return entity;
//        }
//    }
//
//}

IO_Channel* IO_Channel_AccesWrapper::getIOChnl(){
    options.clear();
    return &(*IO_Channel_AccesWrapper::io_channels[options[0]]);
}