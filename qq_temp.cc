//
// Created by lan on 16-9-7.
//

#include <json/json.h>
#include <sstream>
#include "qq_temp.h"
#include "qq_set/log_ini.h"

qq_core::QQTemp::QQTemp() {
}

qq_core::QQTemp::~QQTemp() {

}


bool qq_core::QQTemp::GetOnLineBuddies(map<string, Header> &need, list <FriendOnLine> &onlines) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    string url = "http://d1.web2.qq.com/channel/get_online_buddies2?vfwebqq=" + need["vfwebqq"].value +
                 "&clientid=53999199&psessionid=" + need["psessionid"].value + "&t=1473239796826";
    client->setURL(url);
    client->setTempHeaher(Header("Host", "d1.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取在线好友列表->"+response);
    bool isOK = ParseOnLineBuddies(response, onlines);
    client->close();
    delete client;
    return isOK;
}

bool qq_core::QQTemp::ParseOnLineBuddies(string &json, list <qq_core::FriendOnLine> &onlines) {
    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(json.c_str(), root)) {
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    Json::Value item;
    int count = result.size();
    for (int i = 0; i < count; i++) {
        item = result[i];
        FriendOnLine friendOnLine;
        friendOnLine.id = item["uin"].asUInt64();
        friendOnLine.client_type = item["clienttype"].asInt();
        friendOnLine.status = item["status"].asString();
        onlines.push_back(friendOnLine);
    }
    return true;
}

bool qq_core::QQTemp::GetFriendQQNum(map<string, Header> &need, u_int64_t uin, u_int64_t &qq_num) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_uin2?tuin="
        << uin << "&type=1&vfwebqq=" + need["vfwebqq"].value + "&t=1473240203103";
    client->setURL(ssm.str());
    client->setTempHeaher(Header("Host", "s.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();

    Log::O("获取用户的QQ号码->"+response);

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(response.c_str(), root)) {
        //Json数据有误
        return false;
    }
    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    client->close();
    delete client;
    qq_num = result["account"].asUInt64();
    return true;
}

bool qq_core::QQTemp::GetSingleLongNick(map<string, Header> &need, u_int64_t uin, std::string &long_nick) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_single_long_nick2?tuin="
        << uin
        << "&type=1&vfwebqq=" + need["vfwebqq"].value + "&t=1473240203103";
    client->setURL(ssm.str());
    client->setTempHeaher(Header("Host", "s.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取用户的签名->"+response);

    client->close();
    delete client;

    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(response.c_str(), root)) {
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"][0];
    long_nick = result["lnick"].asString();

    return true;
}

bool qq_core::QQTemp::GetSelfInfo(map<string, Header> &need, QI &qi) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    string url = "http://s.web2.qq.com/api/get_self_info2?t=1473237774012";
    client->setURL(url);
    client->setTempHeaher(Header("Host", "s.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取自己的详细信息->"+response);
    client->close();
    delete client;

    return ParseQQInfo(response, qi);
}

bool qq_core::QQTemp::ParseQQInfo(const string &json, qq_core::QI &qi) {
    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(json.c_str(), root)) {
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
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

bool qq_core::QQTemp::GetQQInfo(map<string, Header> &need, u_int64_t uin, QI &qi) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_info2?tuin="
        << uin << "&vfwebqq=" + need["vfwebqq"].value +"&clientid=53999199&psessionid="+need["psessionid"].value+ "&t=1473240203103";
    client->setURL(ssm.str());
    client->setTempHeaher(Header("Host", "s.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取用户的详细信息->"+response);
    client->close();
    delete client;
    return ParseQQInfo(response, qi);
}

bool qq_core::QQTemp::GetGroupDetailInfo(map<string, Header> &need, u_int64_t gcode, GroupDetailInfo &groupDetailInfo) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_group_info_ext2?gcode="
        << gcode << "&type=1&vfwebqq=" + need["vfwebqq"].value + "&t=1473240203103";
    client->setURL(ssm.str());
    client->setTempHeaher(Header("Host", "s.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取一个群的详细信息->"+response);
    client->close();
    delete client;
    return ParseGroupDetailInfo(response, groupDetailInfo);
}

bool qq_core::QQTemp::ParseGroupDetailInfo(string &json, qq_core::GroupDetailInfo &groupDetailInfo) {
    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(json.c_str(), root)) {
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
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
    for (int i = 0; i < count; ++i) {
        GroupMem groupMem;
        groupMem.uin = members[i]["muin"].asUInt64();
        groupMem.flag = members[i]["mflag"].asUInt();
        groupDetailInfo.members.insert(pair<u_int64_t, GroupMem>(groupMem.uin, groupMem));
    }
    count = cards.size();
    for (int i = 0; i < count; ++i) {
        u_int64_t id = cards[i]["muin"].asUInt64();
        groupDetailInfo.members[id].card = cards[i]["card"].asString();
    }
    count = minfo.size();
    for (int i = 0; i < count; ++i) {
        u_int64_t id = minfo[i]["uin"].asUInt64();
        groupDetailInfo.members[id].nick = minfo[i]["nick"].asString();
        groupDetailInfo.members[id].province = minfo[i]["province"].asString();
        groupDetailInfo.members[id].gender = minfo[i]["gender"].asString();
        groupDetailInfo.members[id].contry = minfo[i]["contry"].asString();
        groupDetailInfo.members[id].city = minfo[i]["city"].asString();
    }
    count = stats.size();
    for (int i = 0; i < count; ++i) {
        u_int64_t id = stats[i]["uin"].asUInt64();
        groupDetailInfo.members[id].client_type = stats[i]["clienttype"].asInt();
        groupDetailInfo.members[id].stat = stats[i]["stat"].asInt();
    }
    count = vipinfo.size();
    for (int i = 0; i < count; ++i) {
        u_int64_t id = vipinfo[i]["uin"].asUInt64();
        groupDetailInfo.members[id].vip_level = vipinfo[i]["vip_level"].asInt();
        groupDetailInfo.members[id].is_vip = vipinfo[i]["is_vip"].asInt();
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

bool qq_core::QQTemp::GetDiscusDetailInfo(map<string, Header> &need, u_int64_t did, DDI &ddi) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }
    stringstream ssm;
    ssm << "http://d1.web2.qq.com/channel/get_discu_info?did="
        << did << "&vfwebqq=" + need["vfwebqq"].value
                  + "&clientid=53999199&psessionid=" + need["psessionid"].value + "&t=1473239796826";
    client->setURL(ssm.str());

    client->setTempHeaher(Header("Host", "d1.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    Log::O("获取一个讨论组的详细信息->"+response);
    client->close();
    delete client;
    return ParseDiscusDetailInfo(response, ddi);
}

bool qq_core::QQTemp::ParseDiscusDetailInfo(string &json, qq_core::DDI &ddi) {
    Json::Reader reader;
    Json::Value root;

    if (!reader.parse(json.c_str(), root)) {
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];

    Json::Value info = result["info"];
    Json::Value memlist = info["mem_list"];
    Json::Value meminfo = result["meminfo"];
    int count = memlist.size();
    for (int i = 0; i < count; ++i) {
        DM dm;
        dm.uin = memlist[i]["mem_uin"].asUInt64();
        dm.ruin = memlist[i]["ruin"].asUInt64();
        ddi.mems.insert(pair<u_int64_t, DM>(dm.uin, dm));
    }
    count = meminfo.size();
    for (int i = 0; i < count; ++i) {
        u_int64_t id = meminfo[i]["uin"].asUInt64();
        ddi.mems[id].nick = meminfo[i]["nick"].asString();
    }
    ddi.name = info["discu_name"].asString();
    ddi.did = info["did"].asUInt64();
    return true;
}

bool qq_core::QQTemp::GetUserFace(map<string, Header> &need, u_int64_t uin, char *data, int &size) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }
    stringstream ssm;
    ssm << "http://face1.web.qq.com/cgi/svr/face/getface?cache=1&type=1&f=40&uin=";
    ssm << uin << "&t=1473242771&vfwebqq=" + need["vfwebqq"].value;
    client->setURL(ssm.str());
    client->setTempHeaher(Header("Host", "face1.web.qq.com"));
    client->setTempHeaher(Header("Referer", "http://w.qq.com/"));

    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    std::stringstream log;
    log << "获取用户" << uin << "头像成功";
    Log::O(log.str());

    const char *imgData = client->GetData(size);
    if (size > 0) {
        memcpy(data, imgData, size);
    }
    return 0 != size;
}

bool qq_core::QQTemp::ChangeStatus(map<string, Header> &need, QQStatus status) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    string str = "";
    switch (status) {
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
    string url = "http://d1.web2.qq.com/channel/change_status2?newstatus=" + str +
                 "&clientid=53999199&psessionid=" + need["psessionid"].value + "&t=1473239796826";
    client->setURL(url);
    client->setTempHeaher(Header("Host", "d1.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    Json::Reader reader;
    Json::Value root;
    if (!client->Execute(HttpClient::GET)) {
        return false;
    }
    string response = client->GetDataByString();
    std::cout << response << std::endl;
    client->close();
    delete client;
    if (!reader.parse(response.c_str(), root)) {
        //Json数据有误
        return false;
    }

    Log::O("改变状态["+str+"]->"+response);

    int retcode = root["retcode"].asInt();
    return 0 == retcode;
}

bool qq_core::QQTemp::Poll(map<string, Header> &need,bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage)) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }
    int poll_count = 0;
    while (true){
        client->setURL("https://d1.web2.qq.com/channel/poll2");
        client->setTempHeaher(Header("Host", "d1.web2.qq.com"));
        client->setTempHeaher(Header("Origin", "http://d1.web2.qq.com"));
        client->setTempHeaher(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

        string r = "{\"ptwebqq\":\"" + need["ptwebqq"].value + "\",\"clientid\":53999199,\"psessionid\":\"" +
                   need["psessionid"].value + "\",\"key\":\"\"}";

        client->setPostField(Field("r", client->URLEncoded(r)));

        if (!client->Execute(HttpClient::POST)) {
            break;
        }
        string response = client->GetDataByString();

        Log::O("Poll->"+response);
        Json::Reader reader;
        Json::Value root;

        if (!reader.parse(response.c_str(), root)) {
            break;
        }
        int retcode = root["retcode"].asInt();
        if (0 != retcode) {
            break;
        }
        Json::Value result = root["result"];
        Json::Value errMsg = root["errmsg"];

        ReceiveMessage receiveMessage;

        if (errMsg.isNull()) {
            receiveMessage.ParseMessage(result);
            receiveMessageListener(true,receiveMessage);
        } else{
            receiveMessageListener(false,receiveMessage);
        }
    }
    client->close();
    delete client;
    return false;
}

bool qq_core::QQTemp::SendOneMessage(map<string, Header> &need, SendMessage sendMessage) {
    HttpClient *client = new HttpClient();
    client = client->GetDefaultClient();
    if (NULL == client) {
        return false;
    }

    switch (sendMessage.GetMessageType()) {
        case USER:
            client->setURL("https://d1.web2.qq.com/channel/send_buddy_msg2");
            break;
        case GROUP:
            client->setURL("https://d1.web2.qq.com/channel/send_qun_msg2");
            break;
        case DISCUS:
            client->setURL("https://d1.web2.qq.com/channel/send_discu_msg2");
            break;
        case UNKNOW_TYPE:
            return false;
    }
    client->setTempHeaher(Header("Host", "d1.web2.qq.com"));
    client->setTempHeaher(Header("Origin", "http://d1.web2.qq.com"));
    client->setTempHeaher(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    client->setPostField(Field("r", client->URLEncoded(sendMessage.PackageMessage())));

    if (!client->Execute(HttpClient::POST)) {
        return false;
    }
    string response = client->GetDataByString();
    client->close();
    delete client;

    Log::O("发送消息成功->"+sendMessage.PackageMessage());

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(response.c_str(), root)) {
        return false;
    }
    int errCode = root["errCode"].asInt();

    return 0 == errCode;
}
