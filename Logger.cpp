//
// Created by yalavrinenko on 13.04.18.
//

#include "Logger.hpp"

std::mutex& io_lock(){
    static std::mutex _mut;

    return _mut;
}

thread_local std::string Logger::m_ModuleName{"OUT"};
thread_local bool Logger::isSuppressed = false;

thread_local std::ofstream Logger::file_out;