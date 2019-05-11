/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CommandProcessor.h
 * Author: julian
 *
 * Created on 18. Februar 2019, 10:45
 */

#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H

#include <vector>
#include <memory>   // shared ptr
#include <string>   // std::string
#include <mutex>
#include <iostream>
#include <string>   // std::string
#include "../IO_Channel_AccesWrapper.h"
#include "../iterationSwitchGuard.h"
#include "WebSocket/websocket_session.hpp"
#include "WebSocket/shared_state.hpp"

#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split

class commandProcessor{
    iterationSwitchGuard& isg;
    IO_Channel_AccesWrapper chnl;
    std::shared_ptr<shared_state>& webSocketSessions;
    
     std::string command_auth(std::vector<std::string>& crumbs, websocket_session* session){
         if(crumbs.size() != 3)
             return this->errText(1);
                     
         char IO_Channel      = crumbs[1][0]; // Anything but the first character is ignored, so they can write "Hardware" if they like.
         std::string token_is = crumbs[2]; 
         
         if(!chnl.is_valid(IO_Channel)) 
             return this->errText(2);
         
         if(!chnl[IO_Channel].getIOChnl()->checkToken(token_is)) 
             return this->errText(4);
         
         chnl[IO_Channel].getIOChnl()->add_authorized_session(session); 
         return "{\"auth\":\"ok.\"}";
         
     }
    
    std::string command_set(std::vector<std::string>& crumbs, websocket_session* session){
        // Check for parts "number of : "
        if(crumbs.size() != 3)
            return this->errText(1);
  
        // Check for count of letters to be 2 or 3
        if(!(crumbs[1].size() == 2 || crumbs[1].size() == 3))
            return this->errText(2);
        
            
        char IO_Channel  = (crumbs[1])[0];
        char IO_Entity   = (crumbs[1])[1];
    
        std::string valstr    = crumbs[2];
        std::cout << "IO Chnl:" << IO_Channel << " IOEntity: " << IO_Entity << std::endl;

        if(!chnl.is_valid(IO_Channel, IO_Entity)){
             std::cout << " Command seems to be INvaild! \n";
            std::string out =  "Channel ";
                        out +=IO_Channel;
                        out +=IO_Entity;
                        out +="invalid!";
            return out;
        }
        
        bool authorized = chnl[IO_Channel].getIOChnl()->is_session_autorized(session);
           
      
        if(!chnl[IO_Channel][IO_Entity]->checkPermission(Channel_Entity::op_write, authorized))
            return this->errText(3, authorized);
        
        
        std::cout << " Command seems to be vaild! \n";

        // e.g. Pi0 so the value needs to be boolean
        if     (crumbs[1].length() == 3){
            char IO_bitnum  = (crumbs[1])[2];
            uint8_t bin_num = (uint8_t) IO_bitnum - '0';
            
            if(bin_num < 0 || bin_num > 7) return "Illegal pin "+std::to_string(bin_num)+" range 0-7";
            
            bool    val_bool; 
            
            if     (valstr == "1" || valstr == "true" || valstr == "high"){
                val_bool = true;
            }
            else if(valstr == "0" || valstr == "false" || valstr == "low"){
                val_bool = false;
            }
            else{
                return "Value is not boolean!";
            }
            
            // Todo: Write to "write_pin" or "write_pin_force?"
            chnl[IO_Channel][IO_Entity]->write_pin_force(val_bool, bin_num);
            std::cout << "three digit expecting boolean value value is: " << crumbs[2] << std::endl;
            return "ok";
        }

        // e.g. Pi then we assign the whole byte
        else if(crumbs[1].length() == 2){
            int intval = std::stoi(valstr);
            if(intval < 0x00 && intval > 0xFF) return "Value not in range 0x0 - 0xFF ";
              chnl[IO_Channel][IO_Entity]->write_all(intval);
              std::cout << "two digit expecting uint value is: " << crumbs[2] << std::endl;
              return "ok";
        }


        /*
        if(crumbs[1] == "Pi0"){

          if(crumbs[2] == "high"){
            chnl['P']['i']->write_pin_force(1,0);
            message = "Set Pi0 to high";
          }
          else{
            chnl['P']['i']->write_pin_force(0,0);  
            message = "Set Pi0 to low";
          }

          

        }
        else{
            message = "Command not understood(2) "+command.second;
        }
         * */

    }

public:
    
    // Called from main thread...so needs its own copy of chnl, brought in from main thread.
    void iterationTriggered(IO_Channel_AccesWrapper& chnl){
        
        bool is_first = true;
        std::string json_resp;
        json_resp = "{\"status\" : {";
        for(auto&& [chkey, channel] : chnl.getAllChannels()){
            for(auto&& [entitykey, entity] : channel->chEntities){
                if(entity->perm_read == Channel_Entity::exp_public){
                    if(!is_first) json_resp += ','; 
                    uint8_t value = entity->read_all();
                    json_resp +=  '"';
                    json_resp += chkey;
                    json_resp += entitykey;
                    json_resp += "\": \"";
                    json_resp += std::to_string(value);
                    json_resp += '"';
                    is_first = false;
                    
                    //std::cout << "Child is public readable! " << entitykey << " Permission is: " << entity->perm_read << " ~~ " << std::endl;
                }
                else if(entity->perm_read == Channel_Entity::exp_private){
                    {
                    uint8_t value = entity->read_all();
                    std::string json_resp = "{\"status\" : {";
                    json_resp +=  '"';
                    json_resp += chkey;
                    json_resp += entitykey;
                    json_resp += "\": \"";
                    json_resp += std::to_string(value);
                    json_resp += "}}";
                    
                    for (auto it = channel->auth_session_begin(); it != channel->auth_session_end(); it++ ){
                        
                        (*it)->send(std::make_shared<std::string const>(json_resp));
                        
                    }
                    }
                    /*
                     1* Loop Authorized sessions of channel
                     2* session->send(message)   auto const ss = std::make_shared<std::string const>(std::move(message)); 
                     */
                }
            }
            //std::cout <<  "Key is: " << chkey <<  std::endl;
            
        }
        
        // Get Autorized sessions... but first check if sessions are removed if invalid. 
        //chnl[IO_Channel].getIOChnl()->add_authorized_session(session); 
        
        json_resp += "}}";
        
        //std::cout << "json is: " << json_resp.str();
        
//        uint8_t Ho = chnl['H']['o']->read_all();
//        uint8_t Hi = chnl['H']['i']->read_all();
//        
//        uint8_t Po = chnl['P']['o']->read_all();
//        uint8_t Pi = chnl['P']['i']->read_all();
//        
//        std::string message = "{\"Ho\":" + std::to_string(Ho) + ",\"Hi\":"+std::to_string(Hi)+",\"Po\":"+std::to_string(Po)+",\"Pi\":"+std::to_string(Pi)+"}";
        //std::cout << "Hardware Outputs are : " << Ho << std::endl;
        webSocketSessions->broadcast(json_resp);
        
        //std::cout << "Active Sessions hier "  << std::endl;
        
        //for (auto it = webSocketSessions->begin(); it != webSocketSessions->end(); it++ )
        //{
        // std::cout << "Active Session: " <<  (*it)  << std::endl;
        //}
        
                
        
    }
                                                // COPY !!!
    commandProcessor(iterationSwitchGuard& _isg, IO_Channel_AccesWrapper _chnl, std::shared_ptr<shared_state>& _webSocketSessions) : isg(_isg), chnl(_chnl), webSocketSessions(_webSocketSessions) {}
    
    std::string errText(int what, bool authorized = false){
        std::string authstr = authorized ? "(authorized)" : "(not authorized)";
        
        switch(what){
            case 1: return "{\"error\":\"Command needs to consist out of 3 parts! e.g. set:Hi0\"}";
            case 2: return "{\"error\":\"Channel is invalid\"}";
            case 3: return "{\"error\":\"insufficent permission to write that Channel " +authstr+"\nUse command 'auth:<IO-Channel>:<token>' to authorize for a channel.\"}";
            case 4: return "{\"auth\":\"token incorrect.\"}";
            
        }
        
        
        
    }
    
    
    
    std::string processCommand(std::pair<websocket_session*, std::string>& command){
  
        std::cout << "Received command: " << command.second << std::endl;
        std::string message = "Processing command: " + command.second;

        std::vector<std::string> crumbs;
        boost::split(crumbs, command.second, boost::is_any_of(":"), boost::token_compress_on);

        if(crumbs[0] == "set"){
            message = this->command_set(crumbs, command.first);
            if(message == "ok"){
                { // Scope for lock
                std::unique_lock<std::mutex> lock{isg.itCondMutex};    
                std::cout << "Locked in signal thread " << std::endl;
                isg.itCondSwitch = true;  
                }
                isg.itCond.notify_one();
            }
        }
        else if(crumbs[0] == "auth"){
            message = command_auth(crumbs, command.first);
            
        }
        else if(crumbs[0] == "show")
        {
            if(crumbs[1] == "channels"){
                std::vector<std::string> lines = chnl.permissionStatus(command.first);
                message = "{\"channels\": [";
                bool isFirst = true;
                for (auto const& line : lines){
                    if(!isFirst) message +=", ";
                    message += "\"" + line  + "\"";
                    isFirst = false;
                }
                
                message += "]}";
            }
            else if(crumbs[1] == "status"){
                iterationTriggered(chnl);
            }
            else{
                message = "SAY THAT AGAIN!";
            }
        
        }
        else{
            message = "Command not understood(1) "+command.second;
        }
        
        return message;

    }
};


#endif /* COMMANDPROCESSOR_H */

