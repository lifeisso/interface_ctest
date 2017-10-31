#ifndef COM_H
#define COM_H
#include <error.h>
#include <fcntl.h>		// open
#include <string.h>		// bzero
#include <stdlib.h>		// exit
#include <sys/times.h>		// times
#include <sys/types.h>		// pid_t
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>		// ioctl
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/msg.h>
#include <sys/ipc.h>


#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


class COM{
public:
    COM();
    //COM(char *port, int baud);
    ~COM();

    int COMOpen(char *port, int baud);
    int COMRead(char *pBuf, int nLength);
    int COMWrite(char *pBuf, int nLength);
private:
    int PortSet(int baud, int bits, int par, int stopbit);
    int convbaud(unsigned long int baudrate);
    int PortOpen(char *port);
    void PortClose();

    fd_set m_rdset;
    int fdcom;
};

#endif

