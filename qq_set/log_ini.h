//
// Created by lan on 16-9-20.
//

#ifndef QQCORE_LOG_INI_H
#define QQCORE_LOG_INI_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

namespace qq_core{
    class Log{
        ~Log(){
            if(Log::stream_opened){
                fstream.flush();
                fstream.close();
            }
        }
    public:
        /**
         * 输出Log日志
         * @param str
         * @return
         */
        template <typename T>
        static void O(T log){
            if(Log::teminal_log_open_){
                std::cout << Log::GetNowTime() << ":\t"<<log << std::endl;
            }
            if(stream_opened){
                std::stringstream ssm;
                ssm << << Log::GetNowTime() << ":\t" << log << std::endl;
                fstream.write(ssm.str().c_str(),ssm.str().length());
            }
        }

        /**
         * 设置是否开启控制台日志输出
         * @param isOpen
         */
        static void setTeminalLogOpen(bool teminal_log_open){
            Log::teminal_log_open_ = teminal_log_open;
        }
        /**
         * 设置是否开启文件日志输出
         * @param isFileLogOpen
         */
        static void setFileLogOpen(bool file_log_open){
            Log::file_log_open_ = file_log_open;
        }
        /**
         * 设置输出文件位置
         * @param log_file_path
         * @return 只能设置一次，设置多次后面都将返回false
         */
        static bool setLogFilePath(std::string log_file_path){
            if("" == Log::log_file_path_){
                Log::log_file_path_ = log_file_path;
                fstream.open(log_file_path,std::ios::out|std::ios::app);
                if(fstream.is_open()){
                    stream_opened = true;
                }
                return true;
            } else{
                false;
            }
        }

        static std::string GetNowTime(){
            time_t now = time(0);
            tm *tm = localtime(&now);
            char buf[80];
            //格式化输出
            tm->tm_mon++;
            strftime(buf, sizeof(buf), "%Y年%m月%d日%H时%M分%S秒", tm);
            return std::string(buf);
        }
    private:
        static bool teminal_log_open_ = true;
        static bool file_log_open_ = false;
        static std::string log_file_path_ = "";
        static std::fstream fstream;
        static bool stream_opened = false;
    };
};
#endif //QQCORE_LOG_INI_H
