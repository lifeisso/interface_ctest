#ifndef MSG_H_
#define MSG_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_SIZE        1024
struct MsgBuf {
    long mtype;     /* 消息类型，必须 > 0 */
    char mtext[MSG_SIZE];  /* 消息文本 */
};

class CMsg {
public:
	CMsg();
	CMsg(key_t key);
	virtual ~CMsg();
	virtual void SetMsgID(key_t key);
	virtual int GetMsgID();
	virtual bool SendMsg(long lType,void *pText,size_t nLength);
	virtual bool RecvMsg(long& lType,void *pText,size_t nLength);
private:
	int m_nMsgID;
};

#endif /* MSG_H_ */
