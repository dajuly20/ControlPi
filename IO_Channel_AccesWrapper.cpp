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

IO_Channel_AccesWrapper::IO_Channel_AccesWrapper(iterationSwitchGuard* _isg){
    isg = _isg;
    
}

IO_Channel_AccesWrapper& IO_Channel_AccesWrapper::operator[](char a){
        //cout << "Value is " << a;
        options.push_back(a);
        return *this;
    }

  uint8_t IO_Channel_AccesWrapper::operator[](int a){
        //cout << "THE PIN Value is " << a << endl;
        uint8_t ret =  (*io_channels[options[0]])[options[1]]->read_pin(a);
        options.clear();
        return ret;
    }

ChannelEntitySP IO_Channel_AccesWrapper::operator->(){
    if(options.size() != 2){
         throw std::invalid_argument("Error: Array must have two dimensions. (dim= "+std::to_string(options.size())+") ..."); 
    }
    auto ret = (*io_channels[options[0]])[options[1]];
    options.clear();
    return ret;
}

void IO_Channel_AccesWrapper::insert(std::pair<char, IOChannelPtr> pair){
    
    pair.second->assignIsg(isg);
    io_channels.insert(std::move(pair));
}

IO_Channel* IO_Channel_AccesWrapper::getIOChnl(){
    options.clear();
    return &(*io_channels[options[0]]);
}