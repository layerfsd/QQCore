//
// Created by lan on 16-9-7.
//

#include "qq_control.h"

qq_core::QQControl::QQControl(HttpClient &client) {
    this->client_ = &client;
    client_->setDefaultHeader(Header("Accept","*/*"));
    client_->setDefaultHeader(Header("Accept-Encoding","gzip, deflate, sdch"));
    client_->setDefaultHeader(Header("Accept-Language","en-US,en;q=0.5"));
    client_->setDefaultHeader(Header("Connection","keep-alive"));
    client_->setDefaultHeader(Header("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36"));
    this->qqLogin_ = new QQLogin(client);
}

qq_core::QQControl::~QQControl() {
    if(this->qqLogin_){
        delete  qqLogin_;
    }
    if(qqContact_){
        delete qqContact_;
    }
    if(qqTemp_){
        delete qqTemp_;
    }
}

const char *qq_core::QQControl::GetQRCImg(int &size) {
    if(!qqLogin_){
        return nullptr;
    }
    if(!qqLogin_->GetLoginSig()){
        return nullptr;
    }
    return qqLogin_->GetQRC(size);;
}

bool qq_core::QQControl::SaveImgToFile(string filePath, const char *data, int size) {
    if(!qqLogin_){
        return false;
    }
    return qqLogin_->SaveImg(filePath,data,size);
}

bool qq_core::QQControl::LoginQQ(void (*Listener)(QQLogin::QRC_Code code, string)) {
    if(!qqLogin_){
        return false;
    }
    return qqLogin_->CheckQRC(Listener);
}
bool qq_core::QQControl::GetControlNeed(string url) {
    if(!qqLogin_->CheckSig(url)){
        return false;
    }
    if(!qqLogin_->GetVFWebqq()){
        return false;
    }
    if(!qqLogin_->Login()){
        return false;
    }
    qqContact_ = new QQContact(*client_,qqLogin_->getUseful());
    qqTemp_ = new QQTemp(*client_,qqLogin_->getUseful());
    return true;
}
bool qq_core::QQControl::GetUserFriends(map<int, qq_core::FriendGroup> &friendGroups,
                                        map<u_int64_t, qq_core::FriendInfo> &friendInfos) {
    if(!qqContact_){
        return false;
    }
    return qqContact_->GetUserFriends(friendGroups,friendInfos);
}

bool qq_core::QQControl::GetGroupNameList(map<u_int64_t, qq_core::GI> &groupList) {
    if(!qqContact_){
        return false;
    }
    return qqContact_->GetGroupNameList(groupList);
}

bool qq_core::QQControl::GetDicusList(map<u_int64_t, qq_core::DI> &discusList) {
    if(!qqContact_){
        return false;
    }
    return qqContact_->GetDicusList(discusList);
}

bool qq_core::QQControl::GetRecentList(map<u_int64_t, qq_core::RI> &recentList) {
    if(!qqContact_){
        return false;
    }
    return qqContact_->GetRecentList(recentList);
}

bool qq_core::QQControl::GetOnLineBuddies(list<qq_core::FriendOnLine> &onlines) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetOnLineBuddies(onlines);
}

u_int64_t qq_core::QQControl::GetFriendQQNum(u_int64_t id) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetFriendQQNum(id);
}

string qq_core::QQControl::GetSingleLongNick(u_int64_t uin) {
    if(!qqTemp_){
        return string();
    }
    return qqTemp_->GetSingleLongNick(uin);
}

bool qq_core::QQControl::GetSelfInfo(qq_core::QI &qi) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetSelfInfo(qi);
}

bool qq_core::QQControl::GetQQInfo(u_int64_t uin, qq_core::QI &qi) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetQQInfo(uin,qi);
}

bool qq_core::QQControl::GetGroupDetailInfo(u_int64_t uin, qq_core::GroupDetailInfo &groupDetailInfo) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetGroupDetailInfo(uin,groupDetailInfo);
}

bool qq_core::QQControl::GetDiscusDetailInfo(u_int64_t did, qq_core::DDI &ddi) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->GetDiscusDetailInfo(did,ddi);
}

const char *qq_core::QQControl::GetUserFace(u_int64_t uin, int &size) {
    if(!qqTemp_){
        return nullptr;
    }
    return qqTemp_->GetUserFace(uin,size);
}

bool qq_core::QQControl::ChangeStatus(qq_core::QQStatus status) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->ChangeStatus(status);
}

bool qq_core::QQControl::Poll(ReceiveMessage &receiveMessage) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->Poll(receiveMessage);
}

bool qq_core::QQControl::SendOneMessage(qq_core::SendMessage &sendMessage) {
    if(!qqTemp_){
        return false;
    }
    return qqTemp_->SendOneMessage(sendMessage);
}





