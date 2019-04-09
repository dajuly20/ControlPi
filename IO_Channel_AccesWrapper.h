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
#include <mutex>
#include "src/IOChannels/IO_Channel.h"
#include "src/ChannelEntitys/Channel_Entity.h"

 typedef std::unique_ptr<IO_Channel> IOChannelPtr;
     
 
class IO_Channel_AccesWrapper{
public:
    std::mutex raceMutex;
    iterationSwitchGuard* isg;
    void insert(std::pair<char, IOChannelPtr> pair);
    IO_Channel_AccesWrapper(iterationSwitchGuard* _isg);
    
    IO_Channel_AccesWrapper(const IO_Channel_AccesWrapper& obj);
    IO_Channel_AccesWrapper& operator=(const IO_Channel_AccesWrapper& obj);
    IO_Channel_AccesWrapper& operator[](char a);
    bool is_valid(char& io_Chnl, char& io_Entity);
    bool is_valid(char& io_Chnl);
    uint8_t operator[](int a);
    ChannelEntitySP operator->();
    IO_Channel* getIOChnl();
    IO_Channel_AccesWrapper();
     
    const std::map<char,IOChannelPtr>& getAllChannels(){
        return io_channels;
    }
//    
//    ChannelEntitySP IO_Channel_AccesWrapper::getFirstInput();
//  
    std::map<char,IOChannelPtr>::const_iterator begin(){
        return io_channels.begin();
    }
    
    std::map<char,IOChannelPtr>::const_iterator end(){
        return io_channels.end();
    }
    
private:
    
    // Race on static ressource? 
    static std::map<char,IOChannelPtr> io_channels; // io_channels must not be leaked! (isg needs to be assigned!)
    std::vector<char> options;
    
    
};


#endif /* IO_CHANNEL_ACCESWRAPPER_H */

