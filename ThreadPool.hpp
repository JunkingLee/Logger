//
// Created by cqg on 2018/3/20.
//

#ifndef THREAD4_THREADPOOL_HPP
#define THREAD4_THREADPOOL_HPP
#include "ConcurrentQueue.hpp"
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#define MIN_THREADS 10
template <class Type>
class ThreadPool{
    ThreadPool&operator=(const ThreadPool&)= delete;
    ThreadPool(const ThreadPool& )= delete;
public:
    ThreadPool(int32_t threads,std::function<void(Type& record)> handler):_threads(threads),_handler(handler),_shutdown(false),_workers(),_taks(){
        if(threads<=MIN_THREADS)
            _threads=MIN_THREADS;
        for(int32_t i=0;i<_threads;++i){
            _workers.emplace_back(
                    [this]{
                            while(!_shutdown){
                                Type record;
                                _taks.Pop(record,true);
                                _handler(record);
                            }
                    }
            );
        }
    }
    virtual ~ThreadPool(){
        for(std::thread &worker:_workers)
            worker.join();
    }
    void Submit(Type record){
        _taks.Push(record);
    }
private:
    bool _shutdown;
    int32_t _threads;
    std::function<void(Type& record)> _handler;
    std::vector<std::thread>    _workers;
    ConcurrentQueue<Type>   _taks;
};

#endif //THREAD4_THREADPOOL_HPP
