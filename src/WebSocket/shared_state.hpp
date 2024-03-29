/*
 Copyright (c) 2018 Vinnie Falco (vinnie dot falco at gmail dot com)

 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Official repository: https://github.com/vinniefalco/CppCon2018

 */

#ifndef CPPCON2018_SHARED_STATE_HPP
#define CPPCON2018_SHARED_STATE_HPP

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>   // shared_ptr
#include <map>


// Forward declaration
class websocket_session;
class IO_Channel;
typedef std::unique_ptr<IO_Channel> IOChannelPtr;
     
class IO_Channel_AccesWrapper;
//const std::map<char,IOChannelPtr>& IO_Channel_AccesWrapper::getAllChannels();




// Represents the shared server state
class shared_state
{
    std::string doc_root_;
    IO_Channel_AccesWrapper* chnl;
   //  This simple method of tracking
   //  sessions only works with an implicit
   //  strand (i.e. a single-threaded server)
    std::unordered_set<websocket_session*>                   sessions_;
   //std::unordered_multimap<websocket_session*, std::string>
    std::queue<std::pair<websocket_session*, std::string>> commandQueue;
    
    std::mutex              commandQueue_mutex;
    
public:
    std::condition_variable commandQueue_ready;
    
    std::unordered_set<websocket_session*>::const_iterator begin();
    std::unordered_set<websocket_session*>::const_iterator end();
    
    explicit
    shared_state(std::string doc_root);
    
    
    std::string const&
    doc_root() const noexcept
    {
        return doc_root_;
    }

    void reg_iochannels(IO_Channel_AccesWrapper* _chnl);
    
    void join  (websocket_session& session);
    void leave (websocket_session& session);
    void broadcast (std::string message); 
    
    void notify_shutdown();
    void commandQueue_wait_for_data();
    void commandQueue_wait_and_pop(std::pair<websocket_session*, std::string>& popedValue);
    std::pair<websocket_session*, std::string> commandQueue_pop();
    void commandQueue_push(websocket_session& session, std::string message);
};

#endif
