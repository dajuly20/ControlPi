/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_AccesWrapper.h
 * Author: julian
 *
 * Created on 25. Dezember 2018, 21:46
 */

#ifndef IO_CHANNEL_ACCESWRAPPER_H
#define IO_CHANNEL_ACCESWRAPPER_H
#include <memory>   // shared_ptr
#include <vector>
#include <map>
#include "src/IOChannels/IO_Channel.h"
#include "src/ChannelEntitys/Channel_Entity.h"

 typedef std::unique_ptr<IO_Channel> IOChannelPtr;
     

class IO_Channel_AccesWrapper{
public:
    IO_Channel_AccesWrapper& operator[](char a);
      uint8_t operator[](int a);
    ChannelEntitySP operator->();
    std::vector<char> options;
    std::map<char,IOChannelPtr> io_channels;
    IO_Channel* getIOChnl();
};


#endif /* IO_CHANNEL_ACCESWRAPPER_H */

