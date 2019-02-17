/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_Pipe_TX.h
 * Author: julian
 *
 * Created on 10. Februar 2019, 16:17
 */



#ifndef CHANNEL_ENTITY_PIPE_TX_H
#define CHANNEL_ENTITY_PIPE_TX_H
#include "Channel_Entity_Memory.h"

class Channel_Entity_Pipe_TX : public Channel_Entity_Memory {
public:
    Channel_Entity_Pipe_TX( int perm_read_ = Channel_Entity::exp_none,  int perm_write_ = Channel_Entity::exp_none){
    
    perm_read  = perm_read_;
    perm_write = perm_write_; 
}
    Channel_Entity_Pipe_TX(const Channel_Entity_Pipe_TX& orig);
    virtual ~Channel_Entity_Pipe_TX();
private:

};

#endif /* CHANNEL_ENTITY_PIPE_TX_H */

