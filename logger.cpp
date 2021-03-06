#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <mutex>
#include <thread>
#include "logger.hpp"

using namespace std;
using namespace logger;

void Log::Write(string msg){
        msg = "[EPOCH] " + to_string(time(0)) + ", " + msg;
        /* while both cout and ofstream in Windows and Linux
        // are thread safe in most implementations
        // mtx will ensure this and that streams do not interrupt each other
        */
        mtx_.lock();
        switch(output_stream_) {
            case BOTH :
                cout << msg << endl;
                outFile_ << msg << endl;
                break;
            case TERM :
                cout << msg << endl;
                break;
            case FILE :
                outFile_ << msg << endl;
                break;
        }
        mtx_.unlock();
    }
//constructor
Log::Log(Level level, OutputStream os, string output_file_name){
    SetLevel(level);
    SetOutputStream(os);
    SetFileName(output_file_name);
    output_file_name_ = output_file_name;
}
Log::~Log(){
    if(outFile_.is_open()){
        outFile_.close();
    }
}
//setting functions
void Log::SetLevel(Level level){
    log_level_ = level;
}
void Log::SetOutputStream(OutputStream os){
    output_stream_ = os;
}
void Log::SetFileName(string fname){
    if(outFile_.is_open()){
        outFile_.close();
    }
    outFile_.open(fname);
    output_file_name_ = fname;
}
//getting functions
Log::Level Log::GetLevel(){
    return log_level_;
}
Log::OutputStream Log::GetOutputStream(){
    return output_stream_;
}
string Log::GetFileName(){
    return output_file_name_;
}

// logging functions
void Log::info(string msg)
{
    if(log_level_ >= INFO){
        string log_msg = "[INFO] ";
        log_msg += msg;
        Write(log_msg);
    }
}
void Log::warn(string msg)
{
    if(log_level_ >= WARN){
        string log_msg = "[WARN] ";
        log_msg += msg;
        Write(log_msg);
    }
}
void Log::error(string msg)
{
    if(log_level_ >= ERROR){
        string log_msg = "[ERROR] ";
        log_msg += msg;
        Write(log_msg);
    }
}
