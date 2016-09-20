//
// Created by lan on 16-9-5.
//

#include "qq_contact.h"
#include "qq_set/log_ini.h"

qq_core::QQContact::QQContact() {
}
qq_core::QQContact::~QQContact() {
}

bool qq_core::QQContact::GetUserFriends(map<string,Header> &need,FriendBaseInfo &friendBaseInfo) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if(NULL == client){
        return false;
    }
    client->setURL("http://s.web2.qq.com/api/get_user_friends2");
    client->setTempHeaher(Header("Host","s.web2.qq.com"));
    client->setTempHeaher(Header("Origin","http://s.web2.qq.com"));
    client->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    Cookie uin = need["uin"];
    long qq_id = atoi(uin.value.c_str());

    string r = "{\"vfwebqq\":\""+need["vfwebqq"].value+"\",\"hash\":\""+GetHash(qq_id,need["ptwebqq"].value)+"\"}";

    client->setPostField(Field("r",client->URLEncoded(r)));

    if(!client->Execute(HttpClient::POST)){
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取好友基本信息->"+response);
    bool isOK = PaserUserFriendsJson(response,friendBaseInfo.friendGroups,friendBaseInfo.friendInfos);
    client->close();
    delete client;
    return isOK;
}
bool qq_core::QQContact::PaserUserFriendsJson(const string &json,std::map<int,FriendGroup> &friendGroups,std::map<u_int64_t,FriendInfo> &friendInfos) {
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
    //获取所有分组的信息
    Json::Value categories = result["categories"];
    int count = categories.size();
    Json::Value item;
    for(int i = 0 ; i < count ; ++i){
        item = categories[i];
        FriendGroup friendGroup;
        friendGroup.index = item["index"].asInt();
        friendGroup.sort = item["sort"].asInt();
        friendGroup.name = item["name"].asString();
        friendGroups.insert(pair<int,FriendGroup>(friendGroup.index,friendGroup));
    }
    //获取好友所在分组
    Json::Value friends = result["friends"];
    count = friends.size();
    for(int i =0 ; i < count ; ++i){
        item = friends[i];
        FriendInfo friendInfo;
        friendInfo.id = item["uin"].asUInt64();
        friendInfo.group_index = item["categories"].asInt();
        friendInfos.insert(pair<u_int64_t,FI>(friendInfo.id,friendInfo));
    }
    //获取好友的昵称,头像信息
    Json::Value infos = result["info"];
    count = infos.size();
    for(int i =0 ; i < count ; ++i){
        item = infos[i];
        u_int64_t  id_t = item["uin"].asUInt64();
        friendInfos[id_t].face = item["face"].asInt();
        friendInfos[id_t].nick_name = item["nick"].asString();
    }
    //获取所有好友备注信息
    Json::Value mark_names = result["marknames"];
    count = mark_names.size();
    for(int i =0 ; i < count ; ++i){
        item = mark_names[i];
        u_int64_t  id_t = item["uin"].asUInt64();
        friendInfos[id_t].mark_name = item["markname"].asString();
    }
    //获取所有好友vip信息
    Json::Value vips = result["vipinfo"];
    count = mark_names.size();
    for(int i =0 ; i < count ; ++i){
        item = mark_names[i];
        u_int64_t  id_t = item["u"].asUInt64();
        friendInfos[id_t].vip_level = item["vip_level"].asInt();
        int vipCode = item["is_vip"].asInt();
        friendInfos[id_t].is_vip = (0 == vipCode?false:true);
    }
    return true;
}
const string qq_core::QQContact::GetHash(const long &uin, const string &ptwebqq) {
    int a[4] = {0};
    for (int i = 0; i < ptwebqq.length(); ++i) {
        a[i % 4] ^= ptwebqq[i];
    }
    char w[4] = {'E', 'C', 'O', 'K'};
    long d[4]={0};
    long lb = uin;
    d[0] = lb >> 24 & 255 ^ w[0];
    d[1] = lb >> 16 & 255 ^ w[1];
    d[2] = lb >> 8 & 255 ^ w[2];
    d[3] = lb & 255 ^ w[3];

    char a1[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char dl[16]={0};
    for (int i = 0; i < 8; ++i) {
        long t = (i % 2 == 0 ? a[i >> 1] : d[i >> 1]);
        dl[i * 2] = a1[(int) (t >> 4 & 15)];
        dl[i * 2 + 1] = a1[(int) (t & 15)];
    }

    return string(dl, 0, 16);
}

bool qq_core::QQContact::GetGroupNameList(map<string,Header> &need,std::map<u_int64_t ,GI> &groupInfos) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    client->setURL("http://s.web2.qq.com/api/get_group_name_list_mask2");
    client->setTempHeaher(Header("Host","s.web2.qq.com"));
    client->setTempHeaher(Header("Origin","http://s.web2.qq.com"));
    client->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    Cookie uin = need["uin"];
    long qq_id = atoi(uin.value.c_str());

    string r = "{\"vfwebqq\":\""+need["vfwebqq"].value+"\",\"hash\":\""+GetHash(qq_id,need["ptwebqq"].value)+"\"}";

    client->setPostField(Field("r",client->URLEncoded(r)));

    if(!client->Execute(HttpClient::POST)){
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取群列表->"+response);

    bool isOK = PaserGroupNameListJson(response,groupInfos);
    client->close();
    delete client;
    return isOK;
}

bool qq_core::QQContact::PaserGroupNameListJson(const string &json,std::map<u_int64_t ,GI> &groupInfos) {
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
    Json::Value namelist = root["result"]["gnamelist"];
    int count = namelist.size();
    Json::Value item;
    for(int i = 0 ;i < count ; ++i){
        item = namelist[i];
        GroupInfo groupInfo;
        groupInfo.id = item["gid"].asUInt64();
        groupInfo.code = item["code"].asUInt64();
        groupInfo.name = item["name"].asString();
        groupInfos.insert(pair<u_int64_t,GI>(groupInfo.id,groupInfo));
    }
    return true;
}

bool qq_core::QQContact::GetDicusList(map<string,Header> &need,std::map<u_int64_t ,DI> &discusList) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if(NULL == client){
        return false;
    }

    string url = "http://s.web2.qq.com/api/get_discus_list?clientid=53999199&psessionid="
                 +need["psessionid"].value
                 +"&vfwebqq="
                 +need["vfwebqq"].value
                 +"&t=1473237774012";
    client->setURL(url);
    client->setTempHeaher(Header("Host","s.web2.qq.com"));
    client->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!client->Execute(HttpClient::GET)){
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取讨论组列表->"+response);
    bool isOK = PaserDiscusListJson(response,discusList);
    client->close();
    delete client;
    return isOK;
}

bool qq_core::QQContact::PaserDiscusListJson(const string &json,std::map<u_int64_t ,DI> &discusInfos) {
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
    Json::Value namelist = root["result"]["dnamelist"];
    int count = namelist.size();
    Json::Value item;
    for(int i = 0 ;i < count ; ++i){
        item = namelist[i];
        DI di;
        di.id = item["did"].asUInt64();
        di.name = item["name"].asString();
        discusInfos.insert(pair<u_int64_t,DI>(di.id,di));
    }
    return true;
}

bool qq_core::QQContact::GetRecentList(map<string,Header> &need,std::map<u_int64_t ,RI> &recentList) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if(NULL == client){
        return false;
    }

    client->setURL("http://d1.web2.qq.com/channel/get_recent_list2");
    client->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client->setTempHeaher(Header("Origin","http://d1.web2.qq.com"));
    client->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    string r ="{\"vfwebqq\":\""+need["vfwebqq"].value+"\",\"clientid\":53999199,\"psessionid\":\""+need["psessionid"].value+"\"}";
    cout << r <<endl;
    client->setPostField(Field("r",client->URLEncoded(r)));

    if(!client->Execute(HttpClient::POST)){
        return false;
    }
    string response = client->GetDataByString();

    Log::O("获取最近联系人列表->"+response);

    bool isOK = PaserRecentListJson(response,recentList);

    client->close();
    delete client;
    return isOK;
}

bool qq_core::QQContact::PaserRecentListJson(const string &json,std::map<u_int64_t ,RI> &recentList) {
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
    Json::Value item;
    for(int i = 0; i < count ; ++i){
        RI ri;
        item = result[i];
        ri.id = item["uin"].asUInt64();
        ri.type = item["type"].asInt();
        ri.status = item["status"].asString();
        recentList.insert(pair<u_int64_t,RI>(ri.id,ri));
    }
    return true;
}



