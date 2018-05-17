//
// Created by yalavrinenko on 13.04.18.
//

#include "Logger.hpp"

std::string Logger::m_ModuleName="OUT";
bool Logger::isSuppressed = false;

std::ofstream Logger::file_out;

std::mutex& io_lock(){
    static std::mutex _mut;

    return _mut;
}
