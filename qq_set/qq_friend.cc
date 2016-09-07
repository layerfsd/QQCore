//
// Created by lan on 16-9-6.
//

#include "qq_friend.h"
qq_core::QQFriend::QQFriend() {}

qq_core::QQFriend::~QQFriend() {}

void qq_core::QQFriend::AddFriendGroup(const qq_core::FriendGroup friendGroup) {
    this->friendGroups_.insert(std::pair<int,FriendGroup>(friendGroup.index,friendGroup));
}

void qq_core::QQFriend::AddFriendInfo(const qq_core::FriendInfo friendInfo) {
    this->friendInfos_.insert(std::pair<long,FriendInfo>(friendInfo.id,friendInfo));
}

qq_core::FriendGroup qq_core::QQFriend::GetFriendGroupByIndex(int index) {
    if(friendGroups_.find(index) == friendGroups_.end()){
        return FriendGroup();
    } else{
        return friendGroups_[index];
    }
}

qq_core::FriendInfo qq_core::QQFriend::GetFriendInfoById(u_int64_t id) {
    if(friendInfos_.find(id) == friendInfos_.end()){
        return FriendInfo();
    } else{
        return friendInfos_[id];
    }
}

void qq_core::QQFriend::AddAllFriendsInfo(const std::map<u_int64_t, qq_core::FriendInfo> &friendInfos) {
    this->friendInfos_ = friendInfos;
}

void qq_core::QQFriend::AddGroupInfo(const qq_core::GroupInfo groupInfo) {
    this->groupInfo_.insert(std::pair<u_int64_t,GI>(groupInfo.id,groupInfo));
}

void qq_core::QQFriend::AddDiscusnfo(const qq_core::DiscusInfo discusInfo) {
    this->discusInfo_.insert(std::pair<u_int64_t,DI>(discusInfo.id,discusInfo));
}

void qq_core::QQFriend::AddRecent(const qq_core::RI ri) {
    this->recentList_.insert(std::pair<u_int64_t,RI>(ri.id,ri));
}


