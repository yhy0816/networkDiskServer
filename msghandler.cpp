#include "msghandler.h"
#include "database.h"
#include "logger.h"
#include "tcpserver.h"
#include "server.h"
#include <QDir>

MsgHandler::MsgHandler()
{

}



PDU* MsgHandler::handleMsg(PDU* msgPDU)
{
    PDU* resPDU = nullptr;
    switch(msgPDU->msgType) {
        case EnMsgType::REGIST_MSG : {

           resPDU = registMsg(msgPDU);
           break;
        }

        case EnMsgType::LOGIN_MSG : {
            resPDU = loginMsg(msgPDU);
            break;

        }
        case EnMsgType::FIND_FRIEND_MSG : {

            resPDU = findFriendMsg(msgPDU);
            break;
        }

        case EnMsgType::GET_ONLINE_USERS_MSG : {

            resPDU = onlineUsersMsg();
            break;
        }
        case EnMsgType::ADD_FRIEND_MSG : {

            resPDU = addFriendMsg(msgPDU);
            break;
        }
        case EnMsgType::AGREE_FRIEND_MSG : {

            resPDU = agreeFriendMsg(msgPDU);
            break;
        }
        case EnMsgType::GET_FRIENDS_MSG : {

            resPDU = getFriendsMsg(msgPDU);
            break;
        }
        case EnMsgType::REMOVE_FRIENDS_MSG : {

            resPDU = removeFriendsMsg(msgPDU);
            break;
        }
        case EnMsgType::CHAT_MSG : {

            ChatMsg(msgPDU);
            break;
        }
        case EnMsgType::MKDIR_MSG : {

            resPDU = makeDir(msgPDU);
            break;
        }
        case EnMsgType::GET_FILES_MSG : {

            resPDU = getFiles(msgPDU);
            break;
        }
        case EnMsgType::DEL_DIR_MSG : {

            resPDU = delDir(msgPDU);
            break;
        }
        case EnMsgType::DEL_FILE_MSG : {

            resPDU = delFile(msgPDU);
            break;
        }
        case EnMsgType::RENAME_FILE_MSG : {

            resPDU = renameFile(msgPDU);
            break;
        }
        case EnMsgType::MOVE_FILE_MSG : {

            resPDU = moveFile(msgPDU);
            break;
        }
        default :{
            INFO << "未知消息 " << (int)msgPDU->msgType;
        }
    }
    return resPDU;
}


PDU * MsgHandler::registMsg(PDU *pdu)
{
    bool ret = Database::getInstance().registHandle(pdu->data, pdu->data + 32);
    if(ret) {
        QDir dir;
        INFO << "mkdir " << pdu->data;
        dir.mkdir(QString("%1/%2").arg(Server::getInstance().getRootpath()).arg(pdu->data));
    }
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::REGIST_RESPOND;
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::loginMsg(PDU *pdu)
{
    bool ret = Database::getInstance().loginHandle(pdu->data, pdu->data + 32);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::LOGIN_RESPOND;
//    if(ret) sock->set_name(pdu->data);
    memcpy(respone_pdu->data, &ret, sizeof(ret));
    return respone_pdu;
}

PDU *MsgHandler::findFriendMsg(PDU *pdu)
{
    int ret = Database::getInstance().findUserHandle(pdu->data);
    PDU* respone_pdu = makePDU(0);
    respone_pdu->msgType = EnMsgType::FIND_FRIEND_RESPONE;

    memcpy(respone_pdu->data, &ret, sizeof(ret));
    memcpy(respone_pdu->data + 32, pdu->data, 32);
    return respone_pdu;
}

PDU *MsgHandler::onlineUsersMsg()
{
    QStringList ret = Database::getInstance().findOnlineUserHandle();
    PDU* respone_pdu = makePDU(ret.size() * 32);
    respone_pdu->msgType = EnMsgType::GET_ONLINE_USERS_RESPONE;
    for(int i = 0; i < ret.size(); i++) {
        std::string cur_s = ret.at(i).toStdString();
        memcpy(respone_pdu->msg + i * 32, cur_s.c_str(), cur_s.size());
    }
    return respone_pdu;
}

PDU *MsgHandler::addFriendMsg(PDU *pdu)
{
    int ret = Database::getInstance().friendhandle(pdu->data, pdu->data + 32);
    if(ret == 0) {
        TcpServer::getInstance().sendTo(pdu->data + 32, pdu);
    }
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::ADD_FRIEND_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

PDU *MsgHandler::agreeFriendMsg(PDU *pdu)
{
    Database::getInstance().agreeFriendHandle(pdu->data, pdu->data + 32);
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::AGREE_FRIEND_RESPONE;
    memcpy(resPDU->data, pdu->data, 64);
    TcpServer::getInstance().sendTo(resPDU->data, resPDU);
    return resPDU;
}

PDU *MsgHandler::getFriendsMsg(PDU *pdu)
{
    QStringList list = Database::getInstance().getFriendsHandle(pdu->data);

    PDU* resPDU = makePDU(list.size() * 32);
    resPDU->msgType = EnMsgType::GET_FRIENDS_RESPONE;
    for(int i = 0; i < list.size(); i++) {
        std::string name = list.at(i).toStdString();
        memcpy(resPDU->msg + i * 32, name.c_str(), name.size());
    }
    return resPDU;
}

PDU *MsgHandler::removeFriendsMsg(PDU *pdu)
{
    int ret = Database::getInstance().removeFriendsHandle(pdu->data, pdu->data + 32);
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::REMOVE_FRIENDS_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

void MsgHandler::ChatMsg(PDU *pdu)
{
    TcpServer::getInstance().sendTo(pdu->data + 32, pdu);
}

PDU *MsgHandler::makeDir(PDU *pdu)
{
//    QString newPath = QString("%1/%2/%3/%4")
//            .arg(Server::getInstance().getRootpath())
//            .arg(pdu->data)
//            .arg(pdu->msg)
//            .arg(pdu->data + 32);
//    INFO << "newPath " << newPath;
    QString newPath = mergePath(pdu) + "/" + QString(pdu->data + 32);
    QDir dir;
    bool res = true;
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::MKDIR_RESPONE;
    INFO << "1 ";
    if(dir.exists(newPath) || !dir.mkdir(newPath) ) {
        res = false;
        INFO << "2 ";
    }
    INFO << "3 ";
    memcpy(resPDU->data, &res, sizeof(res));
    INFO << "4 ";
    return resPDU;

}

PDU *MsgHandler::getFiles(PDU *pdu)
{
    QString path = mergePath(pdu);
    QDir dir(path);
    QFileInfoList fileList = dir.entryInfoList();
    if(fileList.size() == 0) {

        PDU* respdu = makePDU(0);
        respdu->msgType = EnMsgType::GET_FILES_RESPONE;
        return respdu;
    }

    PDU* respdu = makePDU((fileList.size() - 2) * sizeof(FileInfo));
    INFO << "文件数量 " << fileList.size();
    respdu->msgType = EnMsgType::GET_FILES_RESPONE;

    for(int i = 2; i < fileList.size(); i++) {
        FileInfo* info = reinterpret_cast<FileInfo*>(respdu->msg) + i - 2;
        info->fileType = fileList[i].isDir() ? 0 : 1;
        std::string fname = fileList[i].fileName().toStdString();
        memcpy(info->fileName, fname.c_str(), fname.size());
    }
    return respdu;
}

PDU *MsgHandler::delDir(PDU *pdu)
{
    QString path = mergePath(pdu);
    QDir dir(path);
    bool ret = dir.removeRecursively();
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::DEL_DIR_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

PDU *MsgHandler::delFile(PDU *pdu)
{
    QString path = mergePath(pdu);
    QDir dir;
    bool ret = dir.remove(path);
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::DEL_DIR_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

PDU *MsgHandler::renameFile(PDU *pdu)
{
    QString path = mergePath(pdu);
    QDir dir;
    QStringList list = path.split("/");
    list.back() = pdu->data + 32;
    QString newNamePath =list.join("/");

    bool ret = dir.rename(path, newNamePath);
    INFO << "old: " << path << " new: " << newNamePath;
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::RENAME_FILE_RESPONE;
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

PDU *MsgHandler::moveFile(PDU *pdu)
{
    QString oldPath = mergePath(pdu);
    QDir dir;
    int oldLen = 0;
    memcpy(&oldLen, pdu->data + 32, sizeof(oldLen));
    QString newPath  = QString("%1/%2/%3")
            .arg(Server::getInstance().getRootpath())
            .arg(pdu->data)
            .arg(pdu->msg + oldLen + 1);

    INFO << "old: " << oldPath << " new: " << newPath;
    PDU* resPDU = makePDU(0);
    resPDU->msgType = EnMsgType::RENAME_FILE_RESPONE;
    bool ret = dir.rename(oldPath, newPath);
    memcpy(resPDU->data, &ret, sizeof(ret));
    return resPDU;
}

void MsgHandler::printPDU(PDU *pdu)
{
    INFO << "总长度: " << pdu->totalLen ;
    INFO << "消息类型: " << (int)pdu->msgType;
    INFO << "数据1" << pdu->data;
    INFO << "数据2" << pdu->data + 32;
    INFO << "消息长度" << pdu->msgLen;
    INFO << "消息内容" << pdu->msg;
}

QString MsgHandler::mergePath(PDU *pdu)
{
    QString newPath = QString("%1/%2/%3")
            .arg(Server::getInstance().getRootpath())
            .arg(pdu->data)
            .arg(pdu->msg);
    INFO << "newPath " << newPath;
    return newPath;
}


