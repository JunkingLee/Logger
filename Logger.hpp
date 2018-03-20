//
// Created by cqg on 2018/3/20.
//

#ifndef THREAD4_LOGGER_HPP
#define THREAD4_LOGGER_HPP
#include "ConcurrentQueue.hpp"
#include <memory>
#include <thread>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
enum Priority{
    DEBUG,
    STATE,
    INFO,
    WARNING,
    FAULT
};
const std::string PRIORITY_STRING[]=
        {
                "DEBUG",
                "CONFIG",
                "INFO",
                "WARNING",
                "ERROR"
        };
class Logger{
    Logger &operator=(const Logger&)= delete;
    Logger(const Logger&)=delete;

public:
    static Logger *Get(){
        static Logger logger(DEBUG);
        return &logger;
    }
    void SetPriority(Priority priority){
        _priority=priority;
    }
    Priority GetPriority(){
        return _priority;
    }
    void WriteLog(Priority priority,const std::string& log){
        if(priority<_priority)
            return ;
        std::stringstream   stream;
        stream<<"["<<PRIORITY_STRING[priority]<<"]"<<log;
        _queue.Push(stream.str());
    }

private:
    Logger(Priority priority):_queue(),_fileStream(nullptr),_shutdown(false){
        _priority=priority;
        _InitializeFileStram();
        auto func=std::bind(&Logger::_WriteThread,this);
        std::thread writeThread(func);
        writeThread.detach();
    }
    virtual ~Logger(){
        _shutdown=true;
        if(nullptr!=_fileStream){
            _fileStream->close();
            delete _fileStream;
            _fileStream= nullptr;
        }
    }
    void _InitializeFileStram(){
        std::string fileName="logs/Hurricane_log.log";
        _fileStream=new std::ofstream();
        std::ios_base::openmode mode=std::ios_base::out;
        mode|=std::ios_base::trunc;
        _fileStream->open(fileName,mode);
        if(!_fileStream->is_open()){
            std::ostringstream ss_error;
            ss_error<<"FAULT ERROR: could not open log file:["<<fileName;
            ss_error<<"\n\t\t std::ios_base state="<<_fileStream->rdstate();
            std::cerr<<ss_error.str().c_str()<<std::endl<<std::flush;

            _fileStream->close();
            delete _fileStream;
            _fileStream= nullptr;
        }
    }
    void _WriteThread(){
        while(!_shutdown){
            std::string log;
            _queue.Pop(log,true);
            std::cout<<log<<std::endl;
            if(_fileStream)
                *_fileStream<<log<<std::endl;
        }
    }
private:
    ConcurrentQueue<std::string> _queue;
    std::ofstream* _fileStream;
    Priority    _priority;
    bool _shutdown;
};

#define TRACE_DEBUG(LOG_CONTENT) Logger::Get()->WriteLog(DEBUG,LOG_CONTENT);
#define TRACE_STATE(LOG_CONTENT) Logger::Get()->WriteLog(STATE,LOG_CONTENT);
#define TRACE_INFO(LOG_CONTENT) Logger::Get()->WriteLog(INFO,LOG_CONTENT);
#define TRACE_WARNING(LOG_CONTENT) Logger::Get()->WriteLog(WARNING,LOG_CONTENT);
#define TRACE_ERROR(LOG_CONTENT) Logger::Get()->WriteLog(FAULT,LOG_CONTENT);
#endif //THREAD4_LOGGER_HPP
