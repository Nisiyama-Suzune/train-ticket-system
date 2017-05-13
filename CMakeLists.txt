cmake_minimum_required(VERSION 3.6)
project(tts)

set(CMAKE_CXX_STANDARD 11)

include_directories(header)

set(SOURCE_FILES
        main.cpp

        tts_server/header/account_manager.h
        tts_server/header/train_manager.h
        tts_server/header/server.h

        tts_server/source/account_manager.cpp
        tts_server/source/train_manager.cpp
        tts_server/source/server.cpp


        vector.hpp
        list.hpp
        utility.hpp
        exceptions.hpp
        map.hpp
        memory.hpp
        smart_ptr.hpp
        tts_server/source/memory_pool.cpp tts_server/header/memory_pool.h)
add_executable(tts ${SOURCE_FILES})