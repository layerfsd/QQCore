//
// Created by lan on 16-9-5.
//

#ifndef QQCORE_QQ_CONTACT_H
#define QQCORE_QQ_CONTACT_H

#include "qq_login.h"
#include "qq_set/qq_friend.h"

namespace qq_core{
    class QQContact {
    public:
        QQContact(HttpClient &client, map<string,Header> need);
        QQContact();
        ~QQContact();
    private:
        HttpClient *client_;
        map<string,Header> need_;
        QQFriend *qqFriends_;

    public:
        /**
         * 设置请求端必须
         * @param client
         */
        void set_client(HttpClient &client);
        /**
         * 设置登陆时得到的信息
         * @param need
         */
        void set_need(map<string,Header> need);
        /**
         * 获取好友列表，分组信息
         * @return
         */
        bool GetUserFriends();
        /**
         * 获取群列表
         * @return
         */
        bool GetGroupList();
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList();
        /**
         * 获取自己的信息
         * @return
         */
        bool GetSelfInfo(QI &qi);

        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList();

    private:
        /**
         * 获取hash
         * @param uin 传入qq号码
         * @param ptwebqq 传入ptwebqq
         * @return
         */
        const string GetHash(const long & uin,const string &ptwebqq);
        /**
         * 解析获取好友列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserUserFriendsJson(const string &json);

        /**
         * 解析获取群列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserGroupInfoJson(const string &json);
        /**
         * 解析获取讨论组列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserDiscusInfoJson(const string &json);

        /**
         * 解析获取历史聊天列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserRecentListJson(const string &json);
    protected:
        bool PaserQQInfo(const string &json,QI &qi);
    };
};



#endif //QQCORE_QQ_CONTACT_H
