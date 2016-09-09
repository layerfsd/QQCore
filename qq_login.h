//
// Created by lan on 16-9-2.
//

#ifndef QQCORE_QQ_LOGIN_H
#define QQCORE_QQ_LOGIN_H

#include "http_client.h"
#include "json/json.h"

#include <iostream>

#include <map>
#include <fstream>

#include <unistd.h>
namespace qq_core{
    class QQLogin {
    public:
        QQLogin(HttpClient &client);
        ~QQLogin();;
    public:
        enum QRC_Code{SUCCESS=0,INVALID= 65,VALID = 66,SCAN=67,UNKNOW_ERROR = 248};
    private:
        HttpClient *client_;
        map<string,Header> useful_;
    public:

        /**
         * 获取登陆信令
         * @return
         */
        bool GetLoginSig();
        /**
         * 获取二维码图片
         * @param size 数据大小
         * @return 二维码图片数据，为空：NULL
         */
        const char * GetQRC(int &size);
        /**
         * 将数据保存到文件中
         * @param filePath 文件路径
         * @param data 保存的数据
         * @param size 数据大小
         * @return 是否保存成功
         */
        bool SaveImg(string filePath,const char * data,int size);
        /**
         * 检查二维码
         * @param listener 回调函数 string msg:响应消息，响应码
         * @return 是否登陆成功
         */
        bool CheckQRC(void listener(QRC_Code code,string msg));
        /**
         * 检查登陆信令
         * @param url 将上一步得到的msg地址传入
         * @return
         */
        bool CheckSig(string url);
        /**
         * 获取vfwebqq cookie
         * @return
         */
        bool GetVFWebqq();
        /**
         * 以上正确才可执行此函数
         * @return
         */
        bool Login();
        /**
         * 获取登陆后得到的有用信息
         * 包括p_skey,uin,psessionid,cip
         * @return
         */
        map<string,Header> getUseful();
    private:
        /**
         * 解析检查二维码响应获取到的数据
         * @param msg 要解析的消息
         * @return returnMsg 返回消息
         * @return 返回二维码状态
         */
        QRC_Code ParseCheckQRC(string &msg,string &returnMsg);
    };
};

#endif //QQCORE_QQ_LOGIN_H
