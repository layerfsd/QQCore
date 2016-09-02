//
// Created by lan on 16-9-2.
//

#include <cstring>
#include "http_client.h"
qq_core::HttpClient::HttpClient() {
    requestData = (DS*)malloc(sizeof(DS));
    requestData->data = (char*)malloc(sizeof(char)*kDefualtStoreSize);
    requestData->size = kDefualtStoreSize;
    requestData->used = 0;
    response_code = 0;
}

qq_core::HttpClient::~HttpClient() {
    if(requestData){
        if(requestData->data){
            free(requestData->data);
        }
        free(requestData);
    }
    if(handle_){
        curl_easy_cleanup(handle_);
    }
}
bool qq_core::HttpClient::Init() {
    handle_ = curl_easy_init();
    if(!handle_){
        return false;
    }
    curl_easy_setopt(handle_,CURLOPT_SSL_VERIFYPEER, false);//绕过证书
    curl_easy_setopt(handle_,CURLOPT_SSL_VERIFYHOST,true);
    curl_easy_setopt(handle_,CURLOPT_TIMEOUT,5L);//设置请求超时
    curl_easy_setopt(handle_,CURLOPT_WRITEFUNCTION,write_callback);
    curl_easy_setopt(handle_,CURLOPT_WRITEDATA,requestData);
    curl_easy_setopt(handle_,CURLOPT_FOLLOWLOCATION,0L);//禁止重定向
    curl_easy_setopt(handle_,CURLOPT_COOKIEFILE,"");//开启Cookie引擎
    return true;
}

void qq_core::HttpClient::setURL(const string url) {
    this->url_ = url;
}

void qq_core::HttpClient::setDefaultHeader(qq_core::Header header) {
    this->default_headers_.push_back(header);
}

void qq_core::HttpClient::setTempHeaher(qq_core::Header header) {
    this->temp_headers_.push_back(header);
}

void qq_core::HttpClient::setSendCookies(Cookie cookie) {
    this->send_cookies_.push_back(cookie);
}

bool qq_core::HttpClient::Execute(qq_core::HttpClient::RequestMethod method) {
    if(!handle_){
        return false;
    }
    //复位客户端
    ResetClient();
    curl_easy_setopt(handle_,CURLOPT_URL,url_.c_str());//设置请求地址
    //设置请求方式
    if(RequestMethod::Get == method){
        curl_easy_setopt(handle_,CURLOPT_HTTPGET,1L);
    } else{
        curl_easy_setopt(handle_,CURLOPT_POST,true);
    }
    //设置请求头
    curl_slist *headers = NULL;
    for(auto item : default_headers_){
        headers = curl_slist_append(headers,item.toHeaderString().c_str());
    }
    for(auto item : temp_headers_){
        headers = curl_slist_append(headers,item.toHeaderString().c_str());
    }
    curl_easy_setopt(handle_, CURLOPT_HTTPHEADER, headers);
    //设置请求Cookies
    string cookies;
    for(auto item : send_cookies_){
        cookies += item.toCookieString() +";";
    }
    if(!cookies.empty()){
        cookies[cookies.length()-1] = '\0';
        curl_easy_setopt(handle_,CURLOPT_COOKIE,cookies.c_str());
    }
    //执行请求
    CURLcode res = curl_easy_perform(handle_);
    if(CURLE_OK == res){
        GetResponseData();
    }
    curl_slist_free_all(headers);
    return CURLE_OK == res;
}

int qq_core::HttpClient::getResponseCode() {
    return response_code;
}

list <qq_core::Header> qq_core::HttpClient::GetCookie() {
    return receive_cookies_;
}

const char *qq_core::HttpClient::GetData(int &size) {
    size = requestData->used;
    return requestData->data;
}

string qq_core::HttpClient::GetDataByString() {
    requestData->data[requestData->used] = '\0';
    string res(requestData->data);
    return res;
}

void qq_core::HttpClient::close() {
    curl_easy_cleanup(handle_);
    handle_ = NULL;
}

size_t qq_core::HttpClient::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;

    DS * mem = (DS*)userdata;
    //如果默认内存不够用
    if((mem->size - mem->used) < realsize){
        size_t newSize = mem->size +realsize +1;//新的内存大小
        mem->data = (char*)realloc(mem->data, newSize);
        mem->size = newSize;
    }
    if(mem->data == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->data[mem->used]), ptr, realsize);
    mem->used += realsize;
    mem->data[mem->used] = '\0';

    return realsize;
}

void qq_core::HttpClient::GetResponseData() {
    curl_easy_getinfo(handle_,CURLINFO_RESPONSE_CODE,&response_code);
    curl_slist *cookies = NULL;
    if(CURLE_OK == curl_easy_getinfo(handle_,CURLINFO_COOKIELIST,&cookies)){
        curl_slist *temp = cookies;
        while (temp){
            printf("%s\n",temp->data);
            temp = temp->next;
        }
    }
    curl_slist_free_all(cookies);
}

void qq_core::HttpClient::ResetClient() {
    temp_headers_.clear();
    send_cookies_.clear();
    receive_cookies_.clear();
    requestData->used = 0;
    response_code = 0;
}





