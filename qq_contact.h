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
        ~QQContact();
    private:
        HttpClient *client_;
        map<string,Header> need_;
    public:
        /**
         * 获取好友列表，分组信息
         * @return
         */
        bool GetUserFriends(std::map<int,FriendGroup> &friendGroups,std::map<u_int64_t,FriendInfo> &friendInfos);
        /**
         * 获取群列表
         * @return
         */
        bool GetGroupList(std::map<u_int64_t ,GI> &groupList);
        /**
         * 获取讨论组列表
         * @return
         */
        bool GetDicusList(std::map<u_int64_t ,DI> &discusList);


        /**
         * 获取历史聊天记录列表
         * @return
         */
        bool GetRecentList(std::map<u_int64_t ,RI> &recentList);

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
        bool PaserUserFriendsJson(const string &json,std::map<int,FriendGroup> &friendGroups,std::map<u_int64_t,FriendInfo> &friendList);

        /**
         * 解析获取群列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserGroupInfoJson(const string &json,std::map<u_int64_t ,GI> &groupInfos);
        /**
         * 解析获取讨论组列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserDiscusInfoJson(const string &json,std::map<u_int64_t ,DI> &discusInfos);

        /**
         * 解析获取历史聊天列表时得到的Json数据
         * @param json 响应的Json数据
         * @return 是否解析成功
         */
        bool PaserRecentListJson(const string &json,std::map<u_int64_t ,RI> &recentList);
    };
};



#endif //QQCORE_QQ_CONTACT_H
