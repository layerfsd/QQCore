//
// Created by lan on 16-9-7.
//

#include "qq_control.h"

qq_core::QQControl::QQControl() {
    this->client_ = new HttpClient();
    client_->setDefaultHeader(Header("Accept","*/*"));
    client_->setDefaultHeader(Header("Accept-Encoding","gzip, deflate, sdch"));
    client_->setDefaultHeader(Header("Accept-Language","en-US,en;q=0.5"));
    client_->setDefaultHeader(Header("Connection","keep-alive"));
    client_->setDefaultHeader(Header("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36"));
    this->qqLogin_ = new QQLogin(*client_);
}

qq_core::QQControl::~QQControl() {

}

