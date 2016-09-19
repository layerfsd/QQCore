//
// Created by lan on 16-9-7.
//

#include "qq_control.h"

qq_core::QQControl::QQControl() {
    this->client_ = new HttpClient();
    if(!client_->Init()){
        throw ("can't init client");
    }
    client_->setDefaultHeader(Header("Accept","*/*"));
    client_->setDefaultHeader(Header("Accept-Encoding","gzip, deflate, sdch"));
    client_->setDefaultHeader(Header("Accept-Language","en-US,en;q=0.5"));
    client_->setDefaultHeader(Header("Connection","keep-alive"));
    client_->setDefaultHeader(Header("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36"));
    this->qqLogin_ = new QQLogin(*client_);
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
    this->client_->close();
    delete client_;
    client_ = NULL;

    need_ = qqLogin_->getUseful();

    qqContact_ = new QQContact();
    qqTemp_ = new QQTemp();
    return true;
}

bool qq_core::QQControl::GetUserFriends(qq_core::FriendBaseInfo &friendBaseInfo) {
    if(NULL == qqContact_){
        return false;
    }

    std::thread thread(&qq_core::QQContact::GetUserFriends,qqContact_,std::ref(need_),std::ref(friendBaseInfo));

    thread.join();
    return true;
}

bool qq_core::QQControl::GetGroupNameList(map<u_int64_t, qq_core::GI> &groupList) {
    if(NULL == qqContact_){
        return false;
    }

    std::thread thread(&qq_core::QQContact::GetGroupNameList,qqContact_,std::ref(need_),std::ref(groupList));

    thread.join();
    return true;
}

bool qq_core::QQControl::GetDicusList(map<u_int64_t, qq_core::DI> &discusList) {
    if(NULL == qqContact_){
        return false;
    }

    std::thread thread(&qq_core::QQContact::GetDicusList,qqContact_,std::ref(need_),std::ref(discusList));

    thread.join();
    return true;
}

bool qq_core::QQControl::GetRecentList(map<u_int64_t, qq_core::RI> &recentList) {
    if(NULL == qqContact_){
        return false;
    }

    std::thread thread(&qq_core::QQContact::GetRecentList,qqContact_,std::ref(need_),std::ref(recentList));

    thread.join();
    return true;
}

bool qq_core::QQControl::GetOnLineBuddies(list <qq_core::FriendOnLine> &onlines) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::GetOnLineBuddies,qqTemp_,std::ref(need_),std::ref(onlines));
    thread.join();

    return true;
}

u_int64_t qq_core::QQControl::GetFriendQQNum(u_int64_t uin) {
    if(NULL == qqTemp_){
        return false;
    }

    u_int64_t qq_num = 0;

    std::thread thread(&qq_core::QQTemp::GetFriendQQNum,qqTemp_,std::ref(need_),uin,std::ref(qq_num));
    thread.join();

    return qq_num;
}

string qq_core::QQControl::GetSingleLongNick(u_int64_t uin) {
    if(NULL == qqTemp_){
        return string();
    }
    string long_nick = "";

    std::thread thread(&qq_core::QQTemp::GetSingleLongNick,qqTemp_,std::ref(need_),uin,std::ref(long_nick));
    thread.join();

    return long_nick;
}

bool qq_core::QQControl::GetSelfInfo(qq_core::QI &qi) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::GetSelfInfo,qqTemp_,std::ref(need_),std::ref(qi));
    thread.join();

    return true;
}

bool qq_core::QQControl::GetQQInfo(u_int64_t uin, qq_core::QI &qi) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::GetQQInfo,qqTemp_,std::ref(need_),uin,std::ref(qi));
    thread.join();

    return true;
}

bool qq_core::QQControl::GetGroupDetailInfo(u_int64_t gcode, qq_core::GroupDetailInfo &groupDetailInfo) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::GetGroupDetailInfo,qqTemp_,std::ref(need_),gcode,std::ref(groupDetailInfo));
    thread.join();

    return true;
}

bool qq_core::QQControl::GetDiscusDetailInfo(u_int64_t did, qq_core::DDI &ddi) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::GetDiscusDetailInfo,qqTemp_,std::ref(need_),did,std::ref(ddi));
    thread.join();

    return true;
}

const char *qq_core::QQControl::GetUserFace(u_int64_t uin, int &size) {
    if(NULL == qqTemp_){
        return NULL;
    }
    char * imgData = new char[1024*5];
    std::thread thread(&qq_core::QQTemp::GetUserFace,qqTemp_,std::ref(need_),uin,imgData,std::ref(size));
    thread.join();
    if(0 == size){
        delete  imgData;
        imgData = NULL;
    }
    return imgData;
}

bool qq_core::QQControl::ChangeStatus(qq_core::QQStatus status) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::ChangeStatus,qqTemp_,std::ref(need_),status);
    thread.join();

    return true;
}

bool qq_core::QQControl::Poll(bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage)) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::Poll,qqTemp_,std::ref(need_),receiveMessageListener);
    thread.detach();
    return true;
}

bool qq_core::QQControl::SendOneMessage(qq_core::SendMessage &sendMessage) {
    if(NULL == qqTemp_){
        return false;
    }
    std::thread thread(&qq_core::QQTemp::SendOneMessage,qqTemp_,std::ref(need_),sendMessage.SetPsessionid(need_["psessionid"].value));
    thread.detach();
    return true;
}





