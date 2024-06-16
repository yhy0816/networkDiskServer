#include "protocol.h"
#include <cstdlib>
#include <cstring>
#include "logger.h"

PDU* makePDU(uint msgLen)
{
    uint totalLen = msgLen + sizeof(PDU) + 1; // 多存储一个 '\0'
    PDU* pdu = (PDU*)malloc(totalLen);
    if(pdu == nullptr) {
        exit(1);
    }
    memset(pdu, 0, totalLen);
    pdu->totalLen = totalLen;
    pdu->msgLen = msgLen;
    return pdu;
}
