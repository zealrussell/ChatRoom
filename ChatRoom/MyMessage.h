#ifndef MYMESSAGE_H
#define MYMESSAGE_H
#include <QString>
#include <QDataStream>

// 自定义的消息结构体
struct MyMessage {
    // 消息类型：1.聊天消息 2.在线用户人数 3.在线用户列表
    int type;
    // 用户名
    QString username;
    // 内容
    QString message;
};

#endif // MYMESSAGE_H
