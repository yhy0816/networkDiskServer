#ifndef PROTOCOL_H
#define PROTOCOL_H
using uint = unsigned int;

enum class EnMsgType  {
    REGIST_MSG = 1, // 注册消息
    REGIST_RESPOND, // 注册消息响应
    LOGIN_MSG, // 登录消息
    LOGIN_RESPOND, // 登录消息响应
    FIND_FRIEND_MSG, // 查找用户消息
    FIND_FRIEND_RESPONE, // 查找用户响应
    GET_ONLINE_USERS_MSG, // 获取在线用户消息
    GET_ONLINE_USERS_RESPONE, // 获取在线用户响应
    ADD_FRIEND_MSG,   // 添加好友请求消息
    ADD_FRIEND_RESPONE, // 添加好友响应消息
    AGREE_FRIEND_MSG, // 同意添加好友消息
    AGREE_FRIEND_RESPONE, // 同意添加好友响应
    GET_FRIENDS_MSG,  // 请求好友列表
    GET_FRIENDS_RESPONE, // 请求好友列表响应
    REMOVE_FRIENDS_MSG,  // 删除好友列表
    REMOVE_FRIENDS_RESPONE, // 删除好友列表响应
    CHAT_MSG, // 聊天消息
};

struct PDU { // 协议数据单元， 使用柔性数组存储数据
    uint totalLen;  // 总长度
    EnMsgType msgType; // 消息类型
    uint msgLen;     // 实际消息长度
    char data[64];   // 参数
    char msg[];  // 实际消息

};

PDU* makePDU(uint msgLen);

#endif // PROTOCOL_H
