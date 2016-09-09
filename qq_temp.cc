//
// Created by lan on 16-9-7.
//

#include <json/json.h>
#include <sstream>
#include "qq_temp.h"

qq_core::QQTemp::QQTemp(qq_core::HttpClient &client, map<string, qq_core::Header> need) {
    this->client_ = &client;
    this->need_ = need;
}

qq_core::QQTemp::~QQTemp() {

}


bool qq_core::QQTemp::GetOnLineBuddies(list<FriendOnLine> &onelines) {
    string url = "http://d1.web2.qq.com/channel/get_online_buddies2?vfwebqq="+need_["vfwebqq"].value+
            "&clientid=53999199&psessionid="+need_["psessionid"].value+"&t=1473239796826";
    client_->setURL(url);
    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    std::cout << response <<std::endl;
    return ParseOnLineBuddies(response,onelines);
}

bool qq_core::QQTemp::ParseOnLineBuddies(string &json,list <qq_core::FriendOnLine> &onlines) {
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(json.c_str(),root)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    int count = result.size();
    for(int i = 0 ; i < count ; i++){
        FriendOnLine friendOnLine;
        friendOnLine.id = result[i]["uin"].asUInt64();
        friendOnLine.client_type = result[i]["client_type"].asInt();
        friendOnLine.status = result[i]["status"].asString();
    }
    return true;
}

u_int64_t qq_core::QQTemp::GetFriendQQNum(u_int64_t id) {
    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_uin2?tuin="
        << id << "&type=1&vfwebqq="+need_["vfwebqq"].value+"&t=1473240203103";
    client_->setURL(ssm.str());
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client_->Execute(HttpClient::GET)){
        return -1;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(response.c_str(),root)){
        //Json数据有误
        return -1;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return -1;
    }
    Json::Value result = root["result"];

    return result["account"].asUInt64();
}

string qq_core::QQTemp::GetSingleLongNick(u_int64_t uin) {
    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_uin2?"
        << uin
        << "&type=1&vfwebqq="+need_["vfwebqq"].value+"&t=1473240203103";
    client_->setURL(ssm.str());
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client_->Execute(HttpClient::GET)){
        return "";
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(response.c_str(),root)){
        //Json数据有误
        return "";
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return "";
    }
    Json::Value result = root["result"][0];

    return result["lnick"].asString();
}
bool qq_core::QQTemp::GetSelfInfo(QI &qi) {
    string url = "http://s.web2.qq.com/api/get_self_info2?t=1473237774012";
    client_->setURL(url);
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    return ParseQQInfo(response,qi);
}
bool qq_core::QQTemp::ParseQQInfo(const string &json, qq_core::QI &qi) {
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(json.c_str(),root)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    qi.id = result["uin"].asUInt64();
    qi.allow = result["allow"].asInt();
    qi.year = result["year"]["birthday"].asInt();
    qi.month = result["month"]["birthday"].asInt();
    qi.day = result["day"]["birthday"].asInt();
    qi.blood = result["blood"].asInt();
    qi.city = result["city"].asString();
    qi.collage = result["collage"].asString();
    qi.constel = result["constel"].asInt();
    qi.country = result["country"].asString();
    qi.email = result["email"].asString();
    qi.face = result["face"].asInt();
    qi.gender = result["gender"].asString();
    qi.homepage = result["homepage"].asString();
    qi.mobile = result["mobile"].asString();
    qi.nick = result["nick"].asString();
    qi.occupation = result["occupation"].asString();
    qi.personal = result["personal"].asString();
    qi.phone = result["phone"].asString();
    qi.province = result["province"].asString();
    qi.shengxiao = result["shengxiao"].asInt();
    qi.stat = result["stat"].asInt();
    qi.vip_info = result["vip_info"].asInt();
    return true;
}

bool qq_core::QQTemp::GetQQInfo(u_int64_t uin, qq_core::QI &qi) {
    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_uin2?tuin"
        << uin << "&type=1&vfwebqq="+need_["vfwebqq"].value+"&t=1473240203103";
    client_->setURL(ssm.str());
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    return ParseQQInfo(response,qi);
}

bool qq_core::QQTemp::GetGroupDetailInfo(u_int64_t uin, qq_core::GroupDetailInfo &groupDetailInfo) {

    stringstream ssm;
    ssm<< "http://s.web2.qq.com/api/get_group_info_ext2?gcode="
       <<uin<<"&type=1&vfwebqq="+need_["vfwebqq"].value+"&t=1473240203103";
    client_->setURL(ssm.str());
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    return ParseGroupDetailInfo(response,groupDetailInfo);
}

bool qq_core::QQTemp::ParseGroupDetailInfo(string &json, qq_core::GroupDetailInfo &groupDetailInfo) {
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(json.c_str(),root)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    Json::Value cards = result["cards"];
    Json::Value ginfo = result["ginfo"];
    Json::Value minfo = result["minfo"];
    Json::Value stats = result["stats"];
    Json::Value vipinfo = result["vipinfo"];
    Json::Value members = ginfo["members"];

    int count = members.size();
    for(int i = 0 ; i < count ; ++i){
        GroupMem groupMem;
        groupMem.uin = members[i]["muin"].asUInt64();
        groupMem.flag = members[i]["mflag"].asUInt();
        groupDetailInfo.members.insert(pair<u_int64_t,GroupMem>(groupMem.uin,groupMem));
    }
    count = cards.size();
    for(int i = 0 ; i < count ; ++i){
        u_int64_t id = cards["muin"].maxUInt64;
        groupDetailInfo.members[id].card = cards["card"].asString();
    }
    count = minfo.size();
    for(int i = 0 ; i < count ; ++i){
        u_int64_t id = minfo["uin"].maxUInt64;
        groupDetailInfo.members[id].nick = minfo["nick"].asString();
        groupDetailInfo.members[id].province = minfo["province"].asString();
        groupDetailInfo.members[id].gender = minfo["gender"].asString();
        groupDetailInfo.members[id].contry = minfo["contry"].asString();
        groupDetailInfo.members[id].city = minfo["city"].asString();
    }
    count = stats.size();
    for(int i = 0 ; i < count ; ++i){
        u_int64_t id = stats["uin"].maxUInt64;
        groupDetailInfo.members[id].client_type = stats["client_type"].asInt();
        groupDetailInfo.members[id].stat = stats["stat"].asInt();
    }
    count = vipinfo.size();
    for(int i = 0 ; i < count ; ++i){
        u_int64_t id = vipinfo["uin"].maxUInt64;
        groupDetailInfo.members[id].vip_level = vipinfo["vip_level"].asInt();
        groupDetailInfo.members[id].is_vip = vipinfo["is_vip"].asInt();
    }

    groupDetailInfo.class_ = ginfo["class"].asInt();
    groupDetailInfo.code = ginfo["code"].asUInt64();
    groupDetailInfo.create_time = ginfo["createtime"].asUInt64();
    groupDetailInfo.face = ginfo["face"].asInt();
    groupDetailInfo.fingermemo = ginfo["fingermemo"].asString();
    groupDetailInfo.flag = ginfo["flag"].asUInt64();
    groupDetailInfo.id = ginfo["id"].asUInt64();
    groupDetailInfo.level = ginfo["level"].asInt();
    groupDetailInfo.memo = ginfo["memo"].asString();
    groupDetailInfo.name = ginfo["name"].asString();
    groupDetailInfo.option = ginfo["option"].asInt();
    groupDetailInfo.owner = ginfo["owner"].asUInt64();
    return true;
}

bool qq_core::QQTemp::GetDiscusDetailInfo(u_int64_t did, qq_core::DDI &ddi) {
    stringstream ssm;
    ssm<< "http://d1.web2.qq.com/channel/get_discu_info?did="
       <<did<<"&vfwebqq="+need_["vfwebqq"].value
       +"&clientid=53999199&psessionid="+need_["psessionid"].value+"&t=1473239796826";
    client_->setURL(ssm.str());

    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    std::cout << response <<std::endl;
    return ParseDiscusDetailInfo(response,ddi);
}

bool qq_core::QQTemp::ParseDiscusDetailInfo(string &json,qq_core::DDI &ddi) {
    Json::Reader reader;
    Json::Value root;

    if(!reader.parse(json.c_str(),root)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];

    Json::Value info = result["info"];
    Json::Value memlist = info["mem_list"];
    Json::Value meminfo = result["meminfo"];
    int count = memlist.size();
    for(int i = 0; i < count ;++i){
        DM dm;
        dm.uin = memlist[i]["mem_uin"].asUInt64();
        dm.ruin = memlist[i]["ruin"].asUInt64();
        ddi.mems.insert(pair<u_int64_t,DM>(dm.uin,dm));
    }
    count = meminfo.size();
    for(int i = 0; i < count ;++i){
        u_int64_t  id = meminfo[i]["uin"].asUInt64();
        ddi.mems[id].nick = meminfo[i]["nick"].asString();
    }
    ddi.name = info["discu_name"].asString();
    ddi.did = info["did"].asUInt64();
    return true;
}

const char *qq_core::QQTemp::GetUserFace(u_int64_t uin, int &size) {
    stringstream ssm;
    ssm << "http://face1.web.qq.com/cgi/svr/face/getface?cache=1&type=1&f=40&uin=";
    ssm << uin << "&t=1473242771&vfwebqq="+need_["vfwebqq"].value;
    client_->setURL(ssm.str());
    client_->setTempHeaher(Header("Host","face1.web.qq.com"));
    client_->setTempHeaher(Header("Referer","http://w.qq.com/"));

    if(!client_->Execute(HttpClient::GET)){
        return nullptr;
    }
    return client_->GetData(size);
}

bool qq_core::QQTemp::ChangeStatus(qq_core::QQStatus status) {
    string str = "";
    switch (status){
        case ONLINE:
            str = "online";
            break;
        case AWAY:
            str = "away";
            break;
        case CALLME:
            str = "callme";
            break;
        case BUSY:
            str = "busy";
            break;
        case SLIENT:
            str = "slient";
            break;
        case HIDDEN:
            str = "hidden";
            break;
        case OFFLINE:
            str = "offline";
            break;
    }
    string url = "http://d1.web2.qq.com/channel/change_status2?newstatus="+str+
                 "&clientid=53999199&psessionid="+need_["psessionid"].value+"&t=1473239796826";
    client_->setURL(url);
    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    Json::Reader reader;
    Json::Value root;
    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    std::cout << response <<std::endl;

    if(!reader.parse(response.c_str(),root)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    return 0 == retcode;
}

bool qq_core::QQTemp::Poll(ReceiveMessage &receiveMessage) {
    client_->setURL("https://d1.web2.qq.com/channel/poll2");
    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Origin","http://d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    string r = "{\"ptwebqq\":\""+need_["ptwebqq"].value+"\",\"clientid\":53999199,\"psessionid\":\"\",\"key\":\"\"}";

    client_->setPostField(Field("r",client_->URLEncoded(r)));

    if(!client_->Execute(HttpClient::POST)){
        return false;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(response.c_str(),root)){
        return false;
    }
    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        return false;
    }
    Json::Value result = root["result"];
    Json::Value errMsg = root["errmsg"];
    if(!errMsg.isNull()){
        receiveMessage.ParseMessage(result);
    }
    return true;
}

bool qq_core::QQTemp::SendOneMessage(qq_core::SendMessage &sendMessage) {
    switch(sendMessage.GetMessageType()) {
        case USER:
            client_->setURL("https://d1.web2.qq.com/channel/send_buddy_msg2");
            break;
        case GROUP:
            client_->setURL("https://d1.web2.qq.com/channel/send_qun_msg2");
            break;
        case DISCUS:
            client_->setURL("https://d1.web2.qq.com/channel/send_discu_msg2");
            break;
        case UNKNOW_TYPE:
            return false;
    }
    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Origin","http://d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    client_->setPostField(Field("r",client_->URLEncoded(sendMessage.PackageMessage())));

    if(!client_->Execute(HttpClient::POST)){
        return false;
    }
    string response = client_->GetDataByString();
    cout << response <<endl;
    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(response.c_str(),root)){
        return false;
    }
    int errCode = root["errCode"].asInt();
    return 0 == errCode;
}
