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
    rec_queue_.erase(&session);
}

void
shared_state::
broadcast(std::string message)
{
    auto const ss = std::make_shared<std::string const>(std::move(message));

    for(auto session : sessions_)
        session->send(ss);
}

void
shared_state::
process(websocket_session& session, std::string ss){
    rec_queue_.insert(std::make_pair(&session, ss));
    this->broadcast("Simon says: "+ss);
    
}