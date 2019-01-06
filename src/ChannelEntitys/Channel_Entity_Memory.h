/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entity_Memory.h
 * Author: julian
 *
 * Created on 6. Januar 2019, 20:05
 */

#ifndef CHANNEL_ENTITY_MEMORY_H
#define CHANNEL_ENTITY_MEMORY_H

#include "Channel_Entity.h"


class Channel_Entity_Memory : public Channel_Entity {
public:
    Channel_Entity_Memory();
    Channel_Entity_Memory(const Channel_Entity_Memory& orig);
    virtual ~Channel_Entity_Memory();
    
    virtual uint8_t read_pin(uint8_t bit_num) override;
    virtual void   write_pin(bool data, uint8_t bit_num) override;
    
    virtual uint8_t read_all() override;
    virtual void    write_all(uint8_t data) override;
private:
    uint8_t data;
};

#endif /* CHANNEL_ENTITY_MEMORY_H */

