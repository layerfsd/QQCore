//
// Created by lan on 16-9-2.
//

#ifndef QQCORE_HTTP_CLIENT_H
#define QQCORE_HTTP_CLIENT_H

#include <string>
#include <string.h>
#include <list>
#include <curl/curl.h>

using namespace std;
namespace qq_core{
    typedef struct Header{
        string name;
        string value;
        Header(string name,string value){this->name = name;this->value = value;}
        Header(){};
        string toHeaderString(){
            return this->name+":"+this->value;
        }
        string toCookieString(){
            return this->name+"="+this->value;
        }
        string toFieldString(){
            return toCookieString();
        }
    }Cookie,Field;
    class HttpClient {
    public:
        HttpClient();
        ~HttpClient();
        HttpClient *GetDefaultClient();
    private:
        /**
         * 数据存储结构体
         */
        typedef struct DataStore{
            char * data;//数据指针
            int size;//空间总大小
            int used;//被使用的大小
        }MemoryStore,MS,DS;
    private:
        CURL *handle_;
        string url_;
        list<Header> default_headers_;
        list<Header> temp_headers_;
        list<Cookie> send_cookies_;
        list<Cookie> receive_cookies_;
        list<Field> post_fields_;
        DataStore *requestData;
        static const int kDefualtStoreSize = 1024 * 10;
        int response_code;

        static std::string cookie_file_path_ = "";
    public:
        enum RequestMethod{GET,POST};
        /**
         * 设置保存cookie文件的位置
         * @param cookie_file_path
         */
        static void setCookieFilePath(std::string cookie_file_path){
            HttpClient::cookie_file_path_ = cookie_file_path;
        }
    public:
        /**
         * 初始化客户端
         * @return 成功：true;失败:false
         */
        bool Init();
        /**
         * 设置请求地址
         * @param url 请求地址
         */
        void setURL(const string url);
        /**
         * 设置默认请求头,整个生命周期不会被清空
         * @param header
         */
        void setDefaultHeader(Header header);
        /**
         * 设置临时请求头,请求一次会失效
         * @param header
         */
        void setTempHeaher(Header header);
        /**
         * 设置请求的Cookie
         * @param cookie
         */
        void setSendCookies(Cookie cookie);
        /**
         * 设置请求域
         * @param field
         */
        void setPostField(Field field);
        /**
         * 执行请求
         * @param method 请求方式 Get和Posts
         * @return 成功：true;失败:false
         */
        bool Execute(RequestMethod method);
        /**
         * 获取请求响应码
         * @return
         */
        int getResponseCode();
        /**
         * 获取返回的Cookie值
         * @return Cookie列表
         */
        list<Cookie> GetCookie();
        /**
         * 获取请求到的数据
         * @param size 数据大小
         * @return 大小
         */
        const char *GetData(int &size);
        /**
         * 以字符串的方式获取请求到的数据
         * @return
         */
        string GetDataByString();
        /**
         * 对指定字符实行编码
         * @param str
         * @return
         */
        string URLEncoded(string str);
        /**
         * 对数据进行反编码
         * @param data
         * @param size
         * @return
         */
        string URLUnEncoded(const char * data,int size);
        /**
         * 关闭客户端
         */
        void close();

    private:
        /**
         * 复位客户端
         */
        void ResetClient();
        /**
         * 访问返回的回调函数
         * @param ptr 数据内存指针
         * @param size 数据字符大小
         * @param nmemb 数据字符个数
         * @param userdata 用户的数据结构体
         * @return
         */
        static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
        /**
         * 解析请求数据
         */
        void GetResponseData();
        /**
         * 解析Cookie值
         * @param str_cookie解析一个Cookie值，保存到RecieveCookie中
         */
        void ParseOneCookie(const char * str_cookie);
    };
}



#endif //QQCORE_HTTP_CLIENT_H
