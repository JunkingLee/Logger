cmake_minimum_required(VERSION 3.9)
project(thread4)

set(CMAKE_CXX_STANDARD 11)

add_executable(thread4 main.cpp ConcurrentQueue.hpp ThreadPool.hpp BaseEvent.hpp ByteArray.h EventQueueLoop.hpp Logger.hpp)