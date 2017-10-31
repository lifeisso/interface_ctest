#ifndef PING_H
#define PING_H

struct proto {
	void   (*fproc)(char *, ssize_t, struct msghdr *, struct timeval *);
	void   (*fsend)(void);
	void   (*finit)(void);
	struct sockaddr  sasend;     /* sockaddr{} for send, from getaddrinfo */
	struct sockaddr  sarecv;     /* sockaddr{} for receiving */
	socklen_t         salen;              /* length of sockaddr{}s */
	int               icmpproto;  /* IPPROTO_xxx value for ICMP */
} ;
#define BUFSIZE         1500
struct ping_st{
    int              datalen;
    int              nsent;
    pid_t    pid;
    char     sendbuf[BUFSIZE];
    char    recvbuf[BUFSIZE];                                  
    int recvlen;
    int sockfd;
    struct timeval  tvrecv;
    struct proto pr;
};


struct ping_st * ping_v4_init(struct sockaddr sa);
int ping_v4(struct ping_st *pst);

#endif
