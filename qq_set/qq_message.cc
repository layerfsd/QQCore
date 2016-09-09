//
// Created by lan on 16-9-8.
//

#include "qq_message.h"

qq_core::SendMessage::SendMessage(u_int64_t uin,
                                  u_int64_t msg_id,
                                  int face,
                                  std::string psessionid,
                                  qq_core::MessageType type,
                                  std::string msg) {
    message.id = uin;
    message.msg_id = msg_id;
    message.face = face;
    message.psessionid = psessionid;
    message.msg_type = type;
    message.content.msg = msg;

}

qq_core::SendMessage::~SendMessage() {

}

std::string qq_core::SendMessage::PackageMessage() {
    return message.toJsonString();;
}

qq_core::SendMessage &qq_core::SendMessage::SetFontName(std::string fontName) {
    message.content.font.name = fontName;
    return *this;
}

qq_core::SendMessage &qq_core::SendMessage::SetFontSize(int size) {
    message.content.font.size = size;
    return *this;
}

qq_core::SendMessage &qq_core::SendMessage::SetFontColor(std::string fontColor) {
    message.content.font.color = fontColor;
    return *this;
}

qq_core::MessageType qq_core::SendMessage::GetMessageType() {
    return message.msg_type;
}

qq_core::ReceiveMessage::ReceiveMessage(Json::Value result) {
    this->result = result;
    ParseMessage();
}

qq_core::ReceiveMessage::~ReceiveMessage() {

}

void qq_core::ReceiveMessage::ParseMessage() {
    result = result[0];
    std::string poll_type = result["poll_type"].asString();
    Json::Value value = result["value"];
    if("message" == poll_type){
        message.messageType = USER;
        message.from_uin = value["from_uin"].asUInt64();
    } else if("group_message" == poll_type){
        message.messageType = GROUP;
        message.from_uin = value["send_uin"].asUInt64();
        message.id = value["from_uin"].asUInt64();
    } else if("discu_message" == poll_type){
        message.messageType = DISCUS;
        message.from_uin = value["send_uin"].asUInt64();
        message.id = value["from_uin"].asUInt64();
    } else{
        message.messageType = UNKNOW_TYPE;
    }
    message.msg_id = value["msg_id"].asUInt64();
    message.time = value["time"].asUInt64();
    message.to_uin = value["to_uin"].asUInt64();
    Json::Value msgContent = value["content"];
    message.content.msg = msgContent[1].asString();
    Json::Value fontValue = msgContent[0];
    fontValue = fontValue[1];
    message.content.font.name = fontValue["name"].asString();
    message.content.font.color = fontValue["color"].asString();
    message.content.font.size = fontValue["size"].asInt();
    Json::Value fonstyle = fontValue["style"];
    message.content.font.fontStyle.x = fonstyle[0].asInt();
    message.content.font.fontStyle.y = fonstyle[1].asInt();
    message.content.font.fontStyle.z = fonstyle[2].asInt();
    return ;
}
qq_core::MessageType qq_core::ReceiveMessage::GetMessageType() {
    return message.messageType;
}
std::string qq_core::ReceiveMessage::GetMessage() {
    return message.content.msg;
}

u_int64_t qq_core::ReceiveMessage::GetId() {
    return message.id;
}

u_int64_t qq_core::ReceiveMessage::GetFromUin() {
    return message.from_uin;
}

u_int64_t qq_core::ReceiveMessage::GetMessageId() {
    return message.msg_id;
}

u_int64_t qq_core::ReceiveMessage::GetReceiveTime() {
    return message.time;
}

u_int64_t qq_core::ReceiveMessage::GetToUin() {
    return message.to_uin;
}


qq_core::QQMessage::QQMessage() {

}

qq_core::QQMessage::~QQMessage() {

}
