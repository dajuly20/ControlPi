/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   iterationSwitchGuard.h
 * Author: julian
 *
 * Created on 17. Januar 2019, 10:34
 */

#ifndef ITERATIONSWITCHGUARD_H
#define ITERATIONSWITCHGUARD_H

#include <mutex>
#include <condition_variable>

class iterationSwitchGuard {
public:
    std::condition_variable itCond;
    std::mutex              itCondMutex;
    bool                    itCondSwitch;
    
    iterationSwitchGuard();
    virtual ~iterationSwitchGuard();
private:

};

#endif /* ITERATIONSWITCHGUARD_H */

