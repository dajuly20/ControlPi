/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timercpp.cpp
 * Author: https://github.com/shalithasuranga/timercpp
 * 
 * Created on 8. Januar 2019, 20:35
 */

#include "timercpp.h"

 TimerCpp::TimerCpp(){
     clear = false;
 }

void TimerCpp::setTimeout(auto function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if(this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if(this->clear) return;
        function();
    });
    t.detach();
}

void TimerCpp::setInterval(auto function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while(true) {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if(this->clear) return;
            function();
        }
    });
    t.detach();
}

void TimerCpp::stop(){
    this->clear = true;
}
 
