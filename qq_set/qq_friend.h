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
        u_int64_t id = 0;//uin,非qq号码
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
        u_int64_t code = 0;
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
        std::string status = "";
    }RI;
    /**
     * QQ所有状态
     */
    enum QQStatus{ONLINE,AWAY,CALLME,BUSY,SLIENT,HIDDEN,OFFLINE};
    /**
     * 在线好友列表
     */
    typedef struct FriendOnLine{
        int client_type = 0;
        std::string status = "";
        u_int64_t id = 0;
    }FL;
    /**
     * 群成员详细信息
     */
    typedef struct GroupMem{
        u_int64_t uin = 0;
        std::string card = "";
        int flag = 0;
        std::string nick = "";
        std::string province = "";
        std::string gender = "";
        std::string contry = "";
        std::string city = "";
        int client_type = 0;
        int stat = 0;
        int is_vip = 0;
        int vip_level = 0;
    }GM;
    /**
     * 群的详细信息
     */
    typedef struct GroupDetailInfo{
        std::map<u_int64_t ,GroupMem> members;
        int class_ = 0;
        u_int64_t code = 0;
        u_int64_t create_time = 0;
        int face = 0;
        std::string fingermemo = "";
        u_int64_t  flag = 0;
        u_int64_t id = 0;
        int level = 0;
        std::string memo = "";
        std::string name = 0;
        int option = 0;
        u_int64_t owner = 0;
    }GDI;
    /**
     * 讨论组成员信息
     */
    typedef struct DiscusMem{
        u_int64_t uin = 0;
        u_int64_t ruin = 0;
        std::string nick = "";
    }DM;
    /**
     * 讨论组的详细信息
     */
    typedef struct DiscusDetailInfo{
        u_int64_t did = 0;
        std::string name = "";
        std::map<u_int64_t ,DM> mems;
    }DDI;
    /**
     * 消息类型
     */
    enum MessageType{USER,GROUP,DISCUS,UNKNOW_TYPE};
};

#endif //QQCORE_QQ_FRIEND_H
