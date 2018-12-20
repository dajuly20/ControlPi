/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Hw_PiFace.h
 * Author: julian
 *
 * Created on December 19, 2018, 11:39 PM
 */

#ifndef IO_CHANNEL_HW_PIFACE_H
#define IO_CHANNEL_HW_PIFACE_H
#include "pifacedigitalcpp.h"
#include "../ChannelEntitys/Channel_Entities_PiFace.h"
#include "IO_Channel_Hw.h"


class IO_Channel_Hw_PiFace : public IO_Channel_Hw {
public:
    IO_Channel_Hw_PiFace(PiFaceDigital* pfd_init) ;
    IO_Channel_Hw_PiFace(const IO_Channel_Hw_PiFace& orig);
    virtual ~IO_Channel_Hw_PiFace();
private:
    PiFaceDigital* pfd;
};

#endif /* IO_CHANNEL_HW_PIFACE_H */

