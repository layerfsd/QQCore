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
        std::map<int,FriendGroup> friendGroups_;
        std::map<u_int64_t,FriendInfo> friendInfos_;
        std::map<u_int64_t ,GI> groupInfo_;
        std::map<u_int64_t ,DI> discusInfo_;
        std::map<u_int64_t ,RI> recentList_;
    public:
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
        /**
         * 加入一个朋友分组信息
         * @param friendGroup
         */
        void AddFriendGroup(const FriendGroup friendGroup);
        /**
         * 加入一个朋友的信息
         * @param friendInfo
         */
        void AddFriendInfo(const FriendInfo friendInfo);
        /**
         * 通过索引获取信息
         * @param index
         * @return
         */
        FriendGroup GetFriendGroupByIndex(int index);
        /**
         * 通过QQ号获取好友基本信息
         * @param id
         * @return
         */
        FriendInfo GetFriendInfoById(u_int64_t id);
        /**
         * 加入所有的朋友信息
         * @param friendInfos
         * @return
         */
        void AddAllFriendsInfo(const std::map<u_int64_t,FriendInfo> &friendInfos);
        /**
         * 加入一个qq群基本信息
         * @param groupInfo
         */
        void AddGroupInfo(const GroupInfo groupInfo);
        /**
         * 加入一个好友信息
         * @param groupInfo
         */
        void AddDiscusnfo(const DiscusInfo discusInfo);
        /**
         * 加入最近聊天列表
         * @param ri
         */
        void AddRecent(const RI ri);

    };
};



#endif //QQCORE_QQ_CONTACT_H
