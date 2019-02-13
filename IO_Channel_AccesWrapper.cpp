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

IO_Channel* IO_Channel_AccesWrapper::getIOChnl(){
    options.clear();
    return &(*IO_Channel_AccesWrapper::io_channels[options[0]]);
}