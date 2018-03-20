//
// Created by cqg on 2018/3/20.
//

#ifndef THREAD4_CONCURRENTQUEUE_HPP
#define THREAD4_CONCURRENTQUEUE_HPP
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename Type>
class ConcurrentQueue{
    ConcurrentQueue&operator=(const ConcurrentQueue& )=delete;
    ConcurrentQueue(const ConcurrentQueue& )=delete;
public:
    ConcurrentQueue():_queue(),_mutex(),_condition(){}
    virtual ~ConcurrentQueue(){}
    void Push(Type type){
        std::lock_guard<std::mutex> lock(_mutex);
        _queue.push(type);
        _condition.notify_one();
    }
    bool Pop(Type& record,bool isBlocked=true){
        if(isBlocked){
            std::unique_lock<std::mutex> lock(_mutex);
            while(_queue.empty()){
                _condition.wait(lock);/*为空则阻塞*/
            }
        }
        else{
            std::lock_guard<std::mutex> lock(_mutex);
            if(_queue.empty()){
                return false;
            }
        }
        record=std::move(_queue.front());
        _queue.pop();
        return true;
    }

    int32_t Size(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.size();
    }

    bool Empty(){
        std::lock_guard<std::mutex> lock(_mutex);
        return _queue.empty();
    }
private:
    std::queue<Type> _queue;
    mutable std::mutex _mutex;
    std::condition_variable _condition;
};
#endif //THREAD4_CONCURRENTQUEUE_HPP
