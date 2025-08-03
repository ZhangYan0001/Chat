#ifndef GLOBAL_H
#define GLOBAL_H

#include <QWidget>
#include "QStyle"
#include <functional>

enum ReqId {
    ID_GET_VARIFY_CODE = 1001,
    ID_REG_USER = 1002,
};

enum ErrorCodes {
    SUCCESS = 0,
    ERROR_JSON = 1,
    ERROR_NETWORK = 2,
};

enum Modules {
    REGISTERMOD = 0,
};

extern std::function<void(QWidget *)> repolish;
#endif // GLOBAL_H
