/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual
 * Author: julian
 *  
 * 
 * Created on December 18, 2018, 6:48 PM
 */

#ifndef IO_CHANNEL_VIRTUAL_H
#define IO_CHANNEL_VIRTUAL_H
#include "IO_Channel.h"

class IO_Channel_Virtual : public IO_Channel {
public:
    IO_Channel_Virtual();
    IO_Channel_Virtual(const IO_Channel_Virtual& orig);
    virtual ~IO_Channel_Virtual();
private:

};

#endif /* IO_CHANNEL_VIRTUAL_H*/

