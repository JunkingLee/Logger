//
// Created by cqg on 2018/3/20.
//

#ifndef THREAD4_BASEEVENT_HPP
#define THREAD4_BASEEVENT_HPP
#include "ByteArray.h"
#include <string>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <memory.h>
#include <vector>
class IStream;
class BaseEvent{
public:
    BaseEvent(){}
    BaseEvent(const std::string& type,const ByteArray& data,IStream* stream):
            _type(type),_data(data),_stream(stream){}
    void SetData(const ByteArray& data){
        _data=data;
    }
    const ByteArray& GetData()const{
        return _data;
    }
    void SetType(const std::string& type){
        _type=type;
    }
    const std::string& GetType()const{
        return _type;
    }
    void SetStream(IStream* stream){
        _stream=stream;
    }

    IStream* GetStream(){
        return _stream;
    }
private:
    std::string _type;
    ByteArray _data;
    IStream* _stream;
};

class EventQueue{
public:
    EventQueue(int timeout=0):_timeout(timeout){}
    void PostEvent(BaseEvent* event){
        std::unique_lock<std::mutex> lock(_mutex);
        _events.push_back(std::shared_ptr<BaseEvent>(event));
    }
    std::shared_ptr<BaseEvent> GetEvent(){
        std::unique_lock<std::mutex> lock(_mutex);
        if(_events.empty()){
            if(_timeout==0)
                return nullptr;
            _waitCondtion.wait_for(lock,std::chrono::milliseconds(_timeout));
        }
        if(!_events.empty()){
            std::shared_ptr<BaseEvent> event=_events.front();
            _events.erase(_events.begin());
            return event;
        }
        return nullptr;
    }
private:
    std::vector<std::shared_ptr<BaseEvent>> _events;
    std::mutex  _mutex;
    std::condition_variable _waitCondtion;
    int _timeout;
};

#endif //THREAD4_BASEEVENT_HPP
