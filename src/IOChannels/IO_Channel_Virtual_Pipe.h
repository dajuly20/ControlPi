/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IO_Channel_Virtual_Pipe.h
 * Author: julian
 *
 * Created on December 18, 2018, 6:57 PM
 */

#ifndef IO_CHANNEL_VIRTUAL_PIPE_H
#define IO_CHANNEL_VIRTUAL_PIPE_H

#include "IO_Channel_Virtual.h"


class IO_Channel_Virtual_Pipe : public IO_Channel_Virtual {
public:
    IO_Channel_Virtual_Pipe(std::string _token, int _permission);
    IO_Channel_Virtual_Pipe(const IO_Channel_Virtual_Pipe& orig);
    virtual ~IO_Channel_Virtual_Pipe();
    
private:
  
};

#endif /* IO_CHANNEL_VIRTUAL_PIPE_H */

