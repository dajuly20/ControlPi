#ifndef TIMERCPPNEW
#define TIMERCPPNEW

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

using namespace std::chrono_literals;

class Timer {
    std::mutex clear_mtx;
    
    std::mutex cv_mutex;
    std::condition_variable cv;
    
    bool clear = false;
    
    public:
        template<typename Function>
        void setTimeout(Function function, int delay);
        template<typename Function>
        void setInterval(Function function, int interval);
        
        inline void stop(){  
            {
            std::unique_lock<std::mutex> lock{this->clear_mtx};
            this->clear = true;
            }
            cv.notify_one();
        };
};

template<typename Function>
void Timer::setTimeout(Function function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        pthread_setname_np(pthread_self(), "Timer-setTimeout");
        
        {
        std::unique_lock<std::mutex> lock{this->clear_mtx};
        std::cout << "Before" << std::endl;
        if(this->clear) return;
        }
        
        {
        std::unique_lock<std::mutex> lock{this->clear_mtx};
        cv.wait_for(lock, delay*1ms,  [this] { std::cout << "In Cond" << std::endl; return this->clear;});  // Waiting for change of itCondSwitch (that is set by another thread..) 
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        {
        std::unique_lock<std::mutex> lock{this->clear_mtx};
        std::cout << "After" << std::endl;
        if(this->clear) return;
        }
        function();
    });
    t.detach();
}

template<typename Function>
void Timer::setInterval(Function function, int interval) {
    this->clear = false;
    std::thread t([&]() {
        pthread_setname_np(pthread_self(), "Timer-setInterval");
        while(true) {
            {
                std::unique_lock<std::mutex> lock{this->clear_mtx};
                if(this->clear) return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            {
                std::unique_lock<std::mutex> lock{this->clear_mtx};
                if(this->clear) return;
            }
            function();
        }
    });
    t.detach();
}



#endif
