/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Memory.h
 * Author: julian
 *
 * Created on December 18, 2018, 6:52 PM
 */

#ifndef IO_CHANNEL_VIRTUAL_MEMORY_H
#define IO_CHANNEL_VIRTUAL_MEMORY_H

#include "IO_Channel_Virtual.h"


class IO_Channel_Virtual_Memory : public IO_Channel_Virtual {
public:
    IO_Channel_Virtual_Memory();
    IO_Channel_Virtual_Memory(const IO_Channel_Virtual_Memory& orig);
    virtual ~IO_Channel_Virtual_Memory();
private:

};

#endif /* IO_CHANNEL_VIRTUAL_MEMORY_H */

