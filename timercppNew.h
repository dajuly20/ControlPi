#ifndef TIMERCPPNEW
#define TIMERCPPNEW

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

class Timer {

  using clock = std::chrono::steady_clock;
    std::mutex clear_mtx;

    std::condition_variable cv;
    
  bool clear;
    
  std::size_t th_count;

    public:
  Timer() : clear{false}, th_count{0} {};
        
  ~Timer(){   stop();  }


  void stop(){

  std::unique_lock<std::mutex> lock{clear_mtx};
  clear = true;

  lock.unlock();
  cv.notify_all();
  lock.lock();

  cv.wait(lock, [this]() { return th_count == 0; });
}

  template <typename Function, typename Rep, typename Period>
  void setTimeout(Function &&function,
                  const std::chrono::duration<Rep, Period> &delay);

  template <typename Function, typename Rep, typename Period>
  void setInterval(Function &&function,
                   const std::chrono::duration<Rep, Period> &delay);
        };
        
// " Timer.cpp "      
//Timer::Timer() : clear{false}, th_count{0} {}

//Timer::~Timer() { stop(); }

//void Timer::stop() {
//
//  std::unique_lock<std::mutex> lock{clear_mtx};
//  clear = true;
//
//  lock.unlock();
//  cv.notify_all();
//  lock.lock();
//
//  cv.wait(lock, [this]() { return th_count == 0; });
//}
// Timer.cpp
        
        
        
        

template <typename Function, typename Rep, typename Period>
void Timer::setTimeout(Function &&function,
                       const std::chrono::duration<Rep, Period> &delay) {
        {
    std::lock_guard<std::mutex> lock{clear_mtx};
    clear = false;
    ++th_count;
        }
        
  std::thread t{[this, delay, function]() {
      pthread_setname_np(pthread_self(), "Timer-setTimeout");
      auto deadline = clock::now() + delay;
    while (clock::now() < deadline) {
      std::unique_lock<std::mutex> lock{clear_mtx};
      cv.wait_until(lock, deadline);
      if (clear) {
        break;
      }
    }
        
    std::unique_lock<std::mutex> lock{clear_mtx};
    if (!clear) {
      lock.unlock();
      try {
        function();
      } catch (...) {
          // Todo Add error reporting for failed function!
        }
      lock.lock();
        }

    --th_count;
    cv.notify_all(); // wichtig nicht vorher unlocken sonst race mit dem
                     // destructor
  }};
    t.detach();
}


//template<typename Function>
//void Timer::setInterval(Function function, int interval) {
    
    
template <typename Function, typename Rep, typename Period>
void Timer::setInterval(Function &&function,
                       const std::chrono::duration<Rep, Period> &delay) {
    
     {
    std::lock_guard<std::mutex> lock{clear_mtx};
    clear = false;
    ++th_count;
        }
        
  std::thread t{[this, delay, function]() {
        pthread_setname_np(pthread_self(), "Timer-setInterval");
        while(true) {
            {
                std::unique_lock<std::mutex> lock{this->clear_mtx};
                if(this->clear) return;
            }
            
            
            auto deadline = clock::now() + delay;
            while (clock::now() < deadline) {
              std::unique_lock<std::mutex> lock{clear_mtx};
              cv.wait_until(lock, deadline);
              if (clear) {
                break;
              }
            }
            
            
            std::unique_lock<std::mutex> lock{clear_mtx};
            if (!clear) {
              lock.unlock();
              try {
                function();
              } catch (...) {
                  //Todo... function threw an err
              }
            }
            lock.lock();
            
            --th_count;
            cv.notify_all(); // wichtig nicht vorher unlocken sonst race mit dem
                             // destructor
            
        }
            
    }};
    t.detach();
}



#endif
