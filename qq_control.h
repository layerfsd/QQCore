//
// Created by lan on 16-9-7.
//

#ifndef QQCORE_QQ_CONTROL_H
#define QQCORE_QQ_CONTROL_H

#include "qq_login.h"
#include "qq_contact.h"
#include "http_client.h"
namespace qq_core{
    class QQControl :private QQLogin, private QQContact{

    };
};
#endif //QQCORE_QQ_CONTROL_H