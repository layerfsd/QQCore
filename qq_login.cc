//
// Created by lan on 16-9-2.
//


#include "qq_login.h"

qq_core::QQLogin::QQLogin(qq_core::HttpClient &client) {
    this->client_ = &client;
}
qq_core::QQLogin::~QQLogin() {}

bool qq_core::QQLogin::GetLoginSig() {
    client_->setURL("https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001");
    client_->setTempHeaher(Header("Host","ui.ptlogin2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://w.qq.com/"));
    client_->setTempHeaher(Header("Upgrade-Insecure-Requests","1"));
    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    auto cookies = client_->GetCookie();
    for(auto item:cookies){
        if(item.name == "pt_login_sig"){
            useful_.insert(pair<string, Header>(item.name,item));
        }
    }
    return true;
}

const char *qq_core::QQLogin::GetQRC(int &size) {
    client_->setURL("https://ssl.ptlogin2.qq.com/ptqrshow?appid=501004106&e=0&l=M&s=5&d=72&v=4&t=0.6176311824459054");
    client_->setTempHeaher(Header("Host","ssl.ptlogin2.qq.com"));
    client_->setTempHeaher(Header("Referer","https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106"
            "&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login"
            "=1&login_state=10&t=20131024001"));
    if(!client_->Execute(HttpClient::GET)){
        size = 0;
        return NULL;
    }
    return client_->GetData(size);
}

bool qq_core::QQLogin::SaveImg(string filePath, const char *data, int size) {
    if(0 == size){
        return false;
    }
    ofstream of(filePath,ios::binary|ios::out);
    if(!of.is_open()){
        return false;
    }
    of.write(data,size);
    of.flush();
    of.close();
    return true;
}

bool qq_core::QQLogin::CheckQRC(void (*listener)(QRC_Code, string)) {
    while (true){
        client_->setURL("https://ssl.ptlogin2.qq.com/ptqrlogin?webqq_type=10&remember_uin=1&login2qq=1&aid=501004106&u1=http%3A%2F%2Fw.qq.com%2Fproxy.html%3Flogin2qq%3D1%26webqq_type%3D10&ptredirect=0&ptlang=2052&daid=164&from_ui=1&pttype=1&dumy=&fp=loginerroralert&action=0-0-6097&mibao_css=m_webqq&t=1&g=1&js_type=0&js_ver=10173&login_sig=&pt_randsalt=2");
        client_->setTempHeaher(Header("Host","ssl.ptlogin2.qq.com"));
        client_->setTempHeaher(Header("Referer","https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106"
                "&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login"
                "=1&login_state=10&t=20131024001"));
        if(!client_->Execute(HttpClient::GET)){
            cout<< "Check QRC not response"<<endl;
            return false;
        }
        string returnMsg;
        string msg = client_->GetDataByString();
        QRC_Code code = ParseCheckQRC(msg,returnMsg);
        listener(code,returnMsg);

        if(SUCCESS == code){
            auto cookies = client_->GetCookie();
            for(auto item:cookies){
                if(item.name == "ptwebqq"){
                    useful_.insert(pair<string, Header>(item.name,item));
                }
            }
            return true;
        }
        if(UNKNOW_ERROR == code || INVALID == code){
            return false;
        }
        //休息一秒
        sleep(1);
    }
}

qq_core::QQLogin::QRC_Code qq_core::QQLogin::ParseCheckQRC(string &msg, string &returnMsg) {
    if(msg.empty()){
        return UNKNOW_ERROR;
    }
    //查看状态码
    int f_first = msg.find('\'');
    int f_second = msg.find('\'',f_first+1);
    string str_code = msg.substr(f_first+1,f_second-f_first-1);
    int retcode =  atoi(str_code.c_str());
    switch (retcode){
        case SUCCESS:
            f_first = msg.find('\'',15);
            f_second = msg.find('\'',f_first+1);
            returnMsg = msg.substr(f_first+1,f_second - f_first -1);
            return SUCCESS;
        case INVALID:
            returnMsg = "二维码已经失效,请重新加载!";
            break;
        case VALID:
            returnMsg = "二维码未失效,请扫描!";
            return VALID;
        case SCAN:
            returnMsg = "二维码认证中!";
            return SCAN;
        default:
            returnMsg = "二维码认证中!";
            return UNKNOW_ERROR;
    }
}

bool qq_core::QQLogin::CheckSig(string url) {
    if("" == url){
        return false;
    }
    client_->setURL(url);
    client_->setTempHeaher(Header("Host","ptlogin4.web2.qq.com"));
    client_->setTempHeaher(Header("Upgrade-Insecure-Requests","1"));
    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    auto cookies = client_->GetCookie();
    for(auto item:cookies){
        if(item.name == "p_skey"){
            useful_.insert(pair<string, Header>(item.name,item));
        }
    }
    return true;
}

bool qq_core::QQLogin::GetVFWebqq() {
    string url = "http://s.web2.qq.com/api/getvfwebqq?ptwebqq="
    +useful_["ptwebqq"].value
    +"&clientid=53999199&psessionid=&t=1472870102086";
    cout << url << endl;
    client_->setURL(url);
    client_->setTempHeaher(Header("Host","s.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));
    if(!client_->Execute(HttpClient::GET)){
        return false;
    }
    string response = client_->GetDataByString();
    std::cout << client_->GetDataByString() <<endl;

    Json::Reader reader;
    Json::Value root;
    if(!reader.parse(response.c_str(),root)){
        return false;
    }
    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        return false;
    }
    string vfwebqq = root["result"]["vfwebqq"].asString();
    useful_.insert(pair<string,Header>("vfwebqq",Header("vfwebqq",vfwebqq)));
    return true;
}

bool qq_core::QQLogin::Login() {

    client_->setURL("http://d1.web2.qq.com/channel/login2");
    client_->setTempHeaher(Header("Host","d1.web2.qq.com"));
    client_->setTempHeaher(Header("Origin","http://d1.web2.qq.com"));
    client_->setTempHeaher(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    string r = "{\"ptwebqq\":\""+useful_["ptwebqq"].value+"\",\"clientid\":53999199,\"psessionid\":\"\",\"status\":\"online\"}";

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
    useful_.insert(pair<string,Header>(string("cip"),Header(string("cip"),result["cip"].asString())));
    useful_.insert(pair<string,Header>(string("index"),Header(string("index"),result["index"].asString())));
    useful_.insert(pair<string,Header>(string("port"),Header(string("port"),result["port"].asString())));
    useful_.insert(pair<string,Header>(string("psessionid"),Header(string("psessionid"),result["psessionid"].asString())));
    useful_.insert(pair<string,Header>(string("uin"),Header(string("uin"),result["uin"].asString())));
    useful_.insert(pair<string,Header>(string("login_vfwebqq"),Header(string("vfwebqq"),result["vfwebqq"].asString())));
    return false;
}

map<string, qq_core::Header> qq_core::QQLogin::getUseful() {
    return useful_;
}



