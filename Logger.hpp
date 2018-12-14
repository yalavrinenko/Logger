//
// Created by yalavrinenko on 13.04.18.
//

#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
std::mutex& io_lock();

class Logger{
private:
    thread_local static std::string m_ModuleName;
    thread_local static bool isSuppressed;

    thread_local static std::ofstream file_out;

    static void f_out(std::ofstream& out){
        out << std::endl;
    }

    template <class FirstArgT, class ... OtherArgsT>
    static void f_out(std::ofstream& out, FirstArgT first, OtherArgsT ... otherArgsT){
        out << first << " ";
        f_out(out, otherArgsT...);
    };

    template <typename ArgT>
    static void m_Out(std::ostream& out, ArgT info_message){
        out << info_message << " ";
    }

    template <class FirstArgT, class ... OtherArgsT>
    static void m_Out(std::ostream& out, FirstArgT info_message, OtherArgsT ... other_args){
        m_Out(out, info_message);
        m_Out(out, other_args...);
    }

public:
    static void init(std::string const &info_path="Log.txt"){
        file_out.open(info_path);
    }

    static void SuppressOutput(bool value){
        isSuppressed = value;
    }

    static void ModuleName(std::string const &module_name){
        m_ModuleName = std::string(module_name);
    }

    static std::string ModuleName(){
        return m_ModuleName;
    }

    static std::string start_line(std::string const &type){
        return "["+m_ModuleName+" "+type+"]:";
    }

    template <class FirstArgT, class ... OtherArgsT>
    static void Info(FirstArgT info_message, OtherArgsT ... other_args){
        std::lock_guard<std::mutex>  guard(io_lock());
        file_out << start_line("INFO");
        f_out(file_out, info_message, other_args...);
        if (isSuppressed)
            return;

        std::cout << "["<< m_ModuleName << " INFO]:";
        m_Out(std::cout, info_message, other_args...);
        std::cout << std::endl;
    }

    template <class FirstArgT, class ... OtherArgsT>
    static void Error(FirstArgT error_message, OtherArgsT ... other_args){
        std::lock_guard<std::mutex>  guard(io_lock());
        std::cerr << "\x1B[31m" << "["<< m_ModuleName << " ERROR]:";
        m_Out(std::cerr, error_message, other_args...);
        std::cerr << "\033[0m" << std::endl;

        file_out << start_line("ERROR");
        f_out(file_out, error_message, other_args...);
    }

    template <class FirstArgT, class ... OtherArgsT>
    static void Warning(FirstArgT warning_message, OtherArgsT ... other_args){
        std::lock_guard<std::mutex>  guard(io_lock());
        std::cout << "\x1B[34m" << "["<< m_ModuleName << " WARNING]:";
        m_Out(std::cout, warning_message, other_args...);
        std::cout << "\033[0m" << std::endl;

        file_out << start_line("WARNING");
        f_out(file_out, warning_message, other_args...);
    }

};

class LoggerGuard{
public:
    explicit LoggerGuard(std::string const& func_name){
        old_func = Logger::ModuleName();
        Logger::ModuleName(func_name);
    }

    ~LoggerGuard(){
        Logger::ModuleName(old_func);
    }
private:
    std::string old_func;
};
#endif

