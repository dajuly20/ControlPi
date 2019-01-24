/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Channel_Entities_PiFace.h
 * Author: julian
 *
 * Created on December 18, 2018, 7:31 PM
 */

#ifndef CHANNEL_ENTITIES_PIFACE_H
#define CHANNEL_ENTITIES_PIFACE_H
#include "pifacedigitalcpp.h"
#include "../IOChannels/IO_Channel_Hw_PiFace.h"
#include "Channel_Entity.h"
#include <memory>   // shared_ptr

typedef std::shared_ptr<PiFaceDigital> PiFacePtr;

 
class Channel_Entity_PiFace_Outputs : public Channel_Entity {
    
private:
    PiFacePtr& pfd;

    public:
    Channel_Entity_PiFace_Outputs(PiFacePtr& pfd_init) : pfd(pfd_init){};
//    Channel_Entity_PiFace_Outputs(const Channel_Entity_PiFace_Outputs& orig);
    virtual ~Channel_Entity_PiFace_Outputs();
    
     uint8_t read_pin(uint8_t bit_num) override;
     void   write_pin(bool data, uint8_t bit_num) override;
    
    uint8_t read_all() override;
    void    write_all(uint8_t data) override;
    

};


class Channel_Entity_PiFace_Inputs : public Channel_Entity {

private:
    PiFacePtr& pfd;
public:
    Channel_Entity_PiFace_Inputs(PiFacePtr& pfd_init) : pfd(pfd_init){};
//    Channel_Entity_PiFace_Inputs(const Channel_Entity_PiFace_Inputs& orig);
    virtual ~Channel_Entity_PiFace_Inputs();
    
     uint8_t read_pin(uint8_t bit_num) override {return pfd->read_pin(bit_num, PiFaceDigital::IN);}
     uint8_t read_all() override {return pfd->read_byte(PiFaceDigital::IN);}
    
    
};





#endif /* CHANNEL_ENTITIES_PIFACE_H */

