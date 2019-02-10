//
// Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/vinniefalco/CppCon2018
//

#include <unordered_map>

#include "shared_state.hpp"
#include "websocket_session.hpp"
#include <mutex>

shared_state::
shared_state(std::string doc_root)
    : doc_root_(std::move(doc_root))
{
}

void
shared_state::
join(websocket_session& session)
{
    sessions_.insert(&session);
}

void
shared_state::
leave(websocket_session& session)
{
    sessions_.erase(&session);
    //commandQueue.erase(&session); // Commands are still executed!
}

void
shared_state::
broadcast(std::string message)
{
    /*TODO insert mutex to protect input queue!*/
    auto const ss = std::make_shared<std::string const>(std::move(message));

    for(auto session : sessions_)
        session->send(ss);
}


void shared_state::commandQueue_wait_for_data(){
    std::unique_lock<std::mutex> lock{commandQueue_mutex};    
    while(commandQueue.empty()){
        commandQueue_ready.wait(lock, [this] { return !commandQueue.empty();});  // Waiting for change of itCondSwitch (that is set by another thread..) 
    }   
}

void shared_state::notify_shutdown(){
    commandQueue_ready.notify_one();
}


void shared_state::commandQueue_wait_and_pop(std::pair<websocket_session*, std::string>& popedValue){
    std::unique_lock<std::mutex> lock{commandQueue_mutex};    
    
    if(commandQueue.empty()){
        commandQueue_ready.wait(lock, [this]{ return !commandQueue.empty(); });  // Waiting for change of itCondSwitch (that is set by another thread..) 
    }
    
    // re-check condition. Is notified on shutdown also!
    if(!commandQueue.empty()){
        popedValue = commandQueue.front();
        commandQueue.pop();
    }
    
}

std::pair<websocket_session*, std::string>
shared_state::
commandQueue_pop(){
      {
          std::unique_lock<std::mutex> lock{commandQueue_mutex};    
          auto lastOne = commandQueue.front();
          commandQueue.pop();
          return lastOne;
       }
}

void
shared_state::
commandQueue_push(websocket_session& session, std::string ss){
    bool wasEmpty;
            {
        
                std::unique_lock<std::mutex> lock{commandQueue_mutex};   
                wasEmpty = commandQueue.empty();
                commandQueue.push(std::make_pair(&session, ss));
                
            }
            //if(wasEmpty){
                commandQueue_ready.notify_one();
           // }
    //this->broadcast("Simon says: "+ss);
    
}