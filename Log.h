//
// Created by lan on 16-9-19.
//

#ifndef QQCORE_LOG_H
#define QQCORE_LOG_H

#include <fstream>

namespace qq_core{
    class Log{
    public:
        static Log GetSystemOutLog(){
            return Log::log;
        }
        static Log GetFileLog(std::string filePath){

        }
    public:
    private:
        static std::fstream fstream;
        static bool isOpen = true;
        static Log log;
    };
}

#endif //QQCORE_LOG_H
