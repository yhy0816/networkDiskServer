#ifndef PROTOCOL_H
#define PROTOCOL_H
using uint = unsigned int;

enum class EnMsgType  {
    REGIST_MSG = 1, // 注册消息
    REGIST_RESPOND, // 注册消息响应

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
