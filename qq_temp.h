//
// Created by lan on 16-9-7.
//

#ifndef QQCORE_QQ_TEMP_H
#define QQCORE_QQ_TEMP_H

#include "http_client.h"
#include "qq_set/qq_friend.h"
#include "qq_set/qq_message.h"
#include <iostream>
#include <map>

namespace qq_core{
    class QQTemp {
    public:
        QQTemp(Log &log);
        ~QQTemp();

    private:
        Log *log_ = NULL;
    public:
        /**
         * 获取所有在线好友列表
         * @param onlines
         * @return
         */
        bool GetOnLineBuddies(map<string,Header> &need,list<FriendOnLine> &onlines);
        /**
         * 获取QQ号码
         * @param id 传入uin
         * @return 失败返回-1
         */
        bool GetFriendQQNum(map<string,Header> &need,u_int64_t uin , u_int64_t &qq_num);
        /**
         * 获取好友签名
         * @param uin
         * @return
         */
        bool GetSingleLongNick(map<string,Header> &need,u_int64_t uin ,std::string &long_nick);
        /**
         * 获取自己的信息
         * @return
         */
        bool GetSelfInfo(map<string,Header> &need,QI &qi);
        /**
         * 获取qq号码信息
         * @param qi
         * @return
         */
        bool GetQQInfo(map<string,Header> &need,u_int64_t uin,QI &qi);
        /**
         * 获取一个群的详细信息
         * @param uin
         * @param groupDetailInfo
         * @return
         */
        bool GetGroupDetailInfo(map<string, Header> &need, u_int64_t uin, GroupDetailInfo &groupDetailInfo);
        /**
         * 获取讨论组的详细信息
         * @param did
         * @param dm
         * @return
         */
        bool GetDiscusDetailInfo(map<string,Header> &need,u_int64_t did,DDI & ddi);
        /**
         * 获取用户的头像
         * @param uin 传入uin
         * @param size 返回数据大小
         * @return 返回数据指针，为空则不成功
         */
        bool GetUserFace(map<string,Header> &need,u_int64_t uin,char * data,int &size);
        /**
         * 更换qq状态
         * @param status
         * @return 是否更换成功
         */
        bool ChangeStatus(map<string,Header> &need,QQStatus status);
        /**
         * 轮询消息
         * @return
         */
        bool Poll(map<string,Header> &need,bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage));
        /**
         * 发送消息
         * @param sendMessage
         * @return
         */
        bool SendOneMessage(map<string,Header> &need,SendMessage sendMessage);
    private:
        /**
         * 解析在线好友列表
         * @param onlines
         * @return
         */
        bool ParseOnLineBuddies(string &json,list<FriendOnLine> &onlines);

        /**
          * 解析一个群的详细信息
          * @param uin
          * @param groupDetailInfo
          * @return
          */
        bool ParseGroupDetailInfo(string &json, GroupDetailInfo &groupDetailInfo);
        /**
         * 解析一个讨论组的详细信息
         * @param json
         * @param dm
         * @return
         */
        bool ParseDiscusDetailInfo(string &json,DDI &ddi);
    protected:
        /**
         * 解析一个好友的详细信息
         * @param json
         * @param qi
         * @return
         */
        bool ParseQQInfo(const string &json,QI &qi);
    };
};



#endif //QQCORE_QQ_TEMP_H
