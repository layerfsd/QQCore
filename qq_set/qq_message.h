//
// Created by lan on 16-9-8.
//

#ifndef QQCORE_QQ_MESSAGE_H
#define QQCORE_QQ_MESSAGE_H

#include "json/json.h"
#include "qq_friend.h"
#include <iostream>
#include <sstream>

namespace qq_core{

    enum ContentMessageType{STRING,FACE};

    class QQMessage {
    public:
        QQMessage();
        ~QQMessage();

    protected:
        struct FontStyle{
            int x = 0;
            int y = 0;
            int z = 0;
            Json::Value toJson(){
                Json::Value root;
                Json::Value value;
                value=x;
                root.append(value);
                value=y;
                root.append(value);
                value=z;
                root.append(value);
                return root;
            }
        };
        struct Font{
            FontStyle fontStyle;
            std::string color = "000000";
            std::string name = "宋体";
            int size = 10;
            Json::Value toJson(){
                Json::Value root;
                Json::Value value;
                root["color"] = color;
                root["name"] = name;
                root["size"] = size;
                root["style"] = fontStyle.toJson();
                return root;
            }
        };

        struct Content{
            Font font;
            std::vector<std::pair<ContentMessageType,std::string>> msg;
            //不知道为什么要转义
            std::string toString(){
                Json::Value root;
                //加入消息
                for(auto item : msg){
                    if(STRING == item.first){
                        root.append(item.second);
                    } else{
                        Json::Value faceNum;
                        faceNum.append("face");
                        faceNum.append(std::atoi(item.second.c_str()));
                        root.append(faceNum);
                    }
                }
                //加入字体类型
                Json::Value fontValue;
                fontValue.append("font");
                fontValue.append(font.toJson());
                root.append(fontValue);

                Json::FastWriter writer;
                std::string sourceMessage = writer.write(root);
                return sourceMessage.substr(0,sourceMessage.length()-1);
            }
        };
        struct StructSendMessage{
            MessageType msg_type = USER;
            u_int64_t id = 0;
            int face = 0;
            u_int64_t client_id = 53999199;
            u_int64_t msg_id = 0;
            std::string psessionid = "";
            Content content;

            std::string toJsonString(){
                Json::Value root;
                switch (msg_type){
                    case USER:
                        root["to"] = id;
                        break;
                    case GROUP:
                        root["group_uin"] = id;
                        break;
                    case DISCUS:
                        root["did"] = id;
                }
                root["content"] = content.toString();
                root["face"] =face;
                root["clientid"] = client_id;
                root["msg_id"] = msg_id;
                root["psessionid"] = psessionid;
                Json::FastWriter fastWriter;
                std::string str = fastWriter.write(root);
                return str;
            }
        };

        struct StructReceiveMessage{
            Content content;
            MessageType messageType = UNKNOW_TYPE;
            u_int64_t  from_uin = 0;//谁发的消息
            u_int64_t id = 0;//此值只有群的id和讨论组的id
            u_int64_t msg_id = 0;//消息id
            u_int64_t time = 0;//发送时间
            u_int64_t to_uin = 0;//发送给谁的消息
        };
    };
    /**
     * 发送消息类
     */
    class SendMessage:private QQMessage{
    public:
        /**
         * 构造类
         * @param to_uin uin
         * @param msg_id
         * @param psessionid
         * @param type 消息类型
         * @param msg 要发送的消息
         * @return
         */
        SendMessage(u_int64_t to_uin,u_int64_t msg_id,int face,MessageType type,std::vector<std::pair<ContentMessageType,std::string>> msg);

        ~SendMessage();

    private:
        StructSendMessage message;

    public:
        /**
         * 打包消息
         * @return
         */
        std::string PackageMessage();
        /**
         * 设置字体名称,如宋体,微软雅黑等
         * @param fontName
         * @return
         */
        SendMessage &SetFontName(std::string fontName);
        /**
         * 设置字体大小
         * @param size
         * @return
         */
        SendMessage &SetFontSize(int size);
        /**
         * 设置字体颜色
         * @param fontColor
         * @return
         */
        SendMessage &SetFontColor(std::string fontColor);
        /**
         * 设置pessionid
         * @param psessionid
         * @return
         */
        SendMessage &SetPsessionid(std::string psessionid);
        /**
         * 获取要发送的消息类型
         * @return
         */
        MessageType GetMessageType();
    };

    class ReceiveMessage:private QQMessage{
    public:
        /**
         * 传入收到消息的result节点
         * @param result
         * @return
         */
        ReceiveMessage();
        ~ReceiveMessage();
    private:
        StructReceiveMessage message;

    public:
        /**
         * 解析消息
         * @return
         */
        void ParseMessage(Json::Value result);
        /**
         * 获取消息类型
         * @return
         */
        MessageType GetMessageType();
        /**
         * 获取消息
         * @return
         */
        std::vector<std::pair<qq_core::ContentMessageType,std::string>> GetMessage();
        /**
         * 当消息为群组或者讨论组时，表示群组id
         * @return
         */
        u_int64_t GetId();
        /**
         * 获取发送者的uin
         * @return
         */
        u_int64_t GetFromUin();
        /**
         * 获取消息的id
         * @return
         */
        u_int64_t GetMessageId();
        /**
         * 获取接收到消息的时间
         * @return
         */
        u_int64_t GetReceiveTime();
        /**
         * 获取发送给谁的uin，其实就是自己的uin
         * @return
         */
        u_int64_t GetToUin();
    };
};



#endif //QQCORE_QQ_MESSAGE_H
