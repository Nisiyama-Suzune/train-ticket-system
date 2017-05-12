cmake_minimum_required(VERSION 3.6)
project(train_ticket_system)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        train-ticket-system-server/account_manager.cpp
        train-ticket-system-server/account_manager.h
        train-ticket-system-server/exceptions.hpp
        train-ticket-system-server/io_utilities.hpp
        train-ticket-system-server/list.hpp
        train-ticket-system-server/main.cpp
        train-ticket-system-server/server.cpp
        train-ticket-system-server/server.h
        train-ticket-system-server/train_manager.cpp
        train-ticket-system-server/train_manager.h
        train-ticket-system-server/utility.hpp
        train-ticket-system-server/map.hpp
        train-ticket-system-server/deque.hpp
        train-ticket-system-server/vector.hpp
        train-ticket-system-server/memory.hpp)

add_executable(train_ticket_system ${SOURCE_FILES})