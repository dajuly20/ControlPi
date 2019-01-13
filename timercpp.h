/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timercpp.h
 * Author: julian
 *
 * Created on 8. Januar 2019, 20:35
 */

#ifndef TIMERCPP_H
#define TIMERCPP_H

#include <iostream>
#include <thread>
#include <chrono>

class TimerCpp {
    bool clear;
    
    public:
        TimerCpp();
        void setTimeout(auto function, int delay);
        void setInterval(auto function, int interval);
        void stop();
};


#endif /* TIMERCPP_H */

