//
// Created by lan on 16-9-6.
//

#ifndef QQCORE_QQ_FRIEND_H
#define QQCORE_QQ_FRIEND_H

#include <string>
#include <map>

namespace qq_core {
    /**
     * 朋友分组信息
     */
    typedef struct FriendGroup {
        int index = 0;//显示序号
        int sort = 0;//内部序号
        std::string name = "";//分组名称
    } FQ;
    /**
     * 单个朋友信息
     */
    typedef struct FriendInfo {
        u_int64_t id = 0;//qq号码
        int group_index = 0;//所在分组序号
        int face = 0;//头像id
        std::string nick_name = "";//qq昵称
        std::string mark_name = "";//备注名称
        bool is_vip = false;//是否为qq会员
        int vip_level = 0;//vip等级
    } FI;
    /**
     * 群的基本信息
     */
    typedef struct GroupInfo{
       std::string name = "";
        u_int64_t id = 0;
    }GI;
    /**
     * 群的基本信息
     */
    typedef struct DiscusInfo{
        std::string name = "";
        u_int64_t id = 0;
    }DI;
    /**
     * 一个QQ的详细信息
     */
    typedef struct QQInfo{
        u_int64_t id = 0;
        int allow = 0;
        int year = 0;
        int month = 0;
        int day = 0;
        int blood = 0;
        std::string city = "";
        std::string collage = "";
        int constel = 0;
        std::string country = "";
        std::string email = "";
        int face = 0;
        std::string gender = "";
        std::string homepage = "";
        std::string mobile = "";
        std::string nick = "";
        std::string occupation = "";
        std::string personal = "";
        std::string phone = "";
        std::string province = "";
        int shengxiao = 0;
        int stat = 0;
        int vip_info = 0;
    }QI;

    /**
     * 最近聊天的
     */
    typedef struct RecentItem{
        int type = 0;
        u_int64_t id = 0;
    }RI;
    class QQFriend {
    public:
        QQFriend();
        ~QQFriend();;
    private:
        std::map<int,FriendGroup> friendGroups_;
        std::map<u_int64_t,FriendInfo> friendInfos_;
        std::map<u_int64_t ,GI> groupInfo_;
        std::map<u_int64_t ,DI> discusInfo_;
        std::map<u_int64_t ,RI> recentList_;
    public:
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

#endif //QQCORE_QQ_FRIEND_H
