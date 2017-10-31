#include "LinuxMsg.h"
#include "string.h"

CMsg::CMsg() {

}

CMsg::CMsg(key_t key) {
	//创建消息队列或得到已有的消息队列
	m_nMsgID=msgget(key,IPC_CREAT|0666);
}

CMsg::~CMsg() {

}

void CMsg::SetMsgID(key_t key){
	m_nMsgID = msgget(key,IPC_CREAT|0666);
}

int CMsg::GetMsgID()
{
	return m_nMsgID;
}

bool CMsg::SendMsg(long lType,void *pText,size_t nLength)
{
	if(nLength>MSG_SIZE) return false;
	MsgBuf mb;
	mb.mtype=lType;
	memcpy(mb.mtext,pText,nLength);
	if(msgsnd(m_nMsgID,&mb,nLength,IPC_NOWAIT)<0) return false;
	else return true;
}

bool CMsg::RecvMsg(long& lType,void *pText,size_t nLength)
{
	if(nLength>MSG_SIZE) return false;
	MsgBuf mb;
//	if(msgrcv(m_nMsgID,&mb,nLength,lType,0)<0) return FALSE;
	if(msgrcv(m_nMsgID,&mb,nLength,lType,IPC_NOWAIT)<0) return false;
	memcpy(pText,mb.mtext,nLength);
	lType=mb.mtype;
	return true;
}
