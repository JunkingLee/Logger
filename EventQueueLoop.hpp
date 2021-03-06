//
// Created by cqg on 2018/3/20.
//

#ifndef THREAD4_EVENTQUEUELOOP_HPP
#define THREAD4_EVENTQUEUELOOP_HPP

#include <bits/shared_ptr.h>

class Loop{
    virtual ~Loop(){}
};

class EventQueue;
class BaseEvent;
class EventQueueLoop:public Loop{
public:
    EventQueueLoop(EventQueue* queue);

protected:
    virtual void _Run();
    virtual void OnEvent(std::shared_ptr<BaseEvent> event)=0;
private:
    EventQueue* _queue;
};
#endif //THREAD4_EVENTQUEUELOOP_HPP
