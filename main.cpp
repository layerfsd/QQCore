#include <iostream>

#include "qq_login.h"
#include "json/json.h"

using namespace qq_core;
string url = "";

void listener(QQLogin::QRC_Code code, string msg){
    cout << msg <<endl;
    if(QQLogin::SUCCESS == code){
        url = msg;
    }
}


void JsonF(){
    Json::Reader reader;
    Json::Value root;
    string j = "{\"retcode\":0,\"result\":{\"vfwebqq\":\"c1da4f55fd69ddbf53e752c9b973c2f727ff808713248d226f85bab40fc0de61b936df31bf6868fd\"}}";
    if(reader.parse(j.c_str(),root)){
        int retcode = root["retcode"].asInt();
        Json::Value result = root["result"];
        string vfwebqq = result["vfwebqq"].asString();
        int a = 0;
    }
}
int main() {
    HttpClient client;
    client.setDefaultHeader(Header("Accept","*/*"));
    client.setDefaultHeader(Header("Accept-Encoding","gzip, deflate, sdch"));
    client.setDefaultHeader(Header("Accept-Language","en-US,en;q=0.5"));
    client.setDefaultHeader(Header("Connection","keep-alive"));
    client.setDefaultHeader(Header("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36"));

    if(!client.Init()){
        printf("error:can't init client\n");
        return false;
    }
    QQLogin login(client);
    login.GetLoginSig();
    int imgSize = 0;
    const char*img = login.GetQRC(imgSize);
    if(!login.SaveImg("/home/lan/a.png",img,imgSize)){
        cout << "error :SaveImg"<<endl;
        return 0;
    }
    if(!login.CheckQRC(listener))
    {
        cout << "error :CheckQRC"<<endl;
        return 0;
    }
    if(!login.CheckSig(url)){
        cout << "error :CheckSig"<<endl;
        return 0;
    }
    login.GetVFWebqq();
    login.Login();
    client.close();
    return 0;
}