/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HW_Channel.h
 * Author: julian
 *
 * Created on December 18, 2018, 6:45 PM
 */

#ifndef HW_CHANNEL_H
#define HW_CHANNEL_H

#include "IO_Channel.h"

class IO_Channel_Hw : public IO_Channel {
public:
    IO_Channel_Hw();
    IO_Channel_Hw(const IO_Channel_Hw& orig);
    virtual ~IO_Channel_Hw();
private:

};

#endif /* HW_CHANNEL_H */

