#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include        <netinet/in_systm.h>
#include        <netinet/ip.h>
#include        <netinet/ip_icmp.h>

#include "ping.h"

uint16_t in_cksum(uint16_t *addr, int len)
{
	int                             nleft = len;
	uint32_t                sum = 0;
	uint16_t                *w = addr;
	uint16_t                answer = 0;

	/*
	 *          * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	 *                   * sequential 16 bit words to it, and at the end, fold back all the
	 *                            * carry bits from the top 16 bits into the lower 16 bits.
	 *                                     */
	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	/* 4mop up an odd byte, if necessary */
	if (nleft == 1) {
		*(unsigned char *)(&answer) = *(unsigned char *)w ;
		sum += answer;
	}

	/* 4add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff);     /* add hi 16 to low 16 */
	sum += (sum >> 16);                     /* add carry */
	answer = ~sum;                          /* truncate to 16 bits */
	return(answer);
}

struct ping_st * ping_v4_init(struct sockaddr sa)
{
    struct ping_st *pst;

    pst = (struct ping_st *)malloc(sizeof(struct ping_st));
    if(pst == NULL){
        printf("malloc fail\n");
        return NULL;
    }
	pst->pr.sasend = sa;
	pst->pr.salen = sizeof(sa);
	pst->pr.icmpproto = IPPROTO_ICMP;
	pst->sockfd = socket(pst->pr.sasend.sa_family, SOCK_RAW, pst->pr.icmpproto);
	if(pst->sockfd < 0){
		printf("socket fail\n");
		return NULL;
	}

	int size = 60 * 1024;               /* OK if setsockopt fails */  
	setsockopt(pst->sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
    pst->datalen = 56;

	return pst;
}

void send_v4(struct ping_st *pst)
{
	int  len;
	struct icmp     *icmp;

	icmp = (struct icmp *) pst->sendbuf;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = pst->pid;
	icmp->icmp_seq = pst->nsent++;
	memset(icmp->icmp_data, 0xa5, pst->datalen); /* fill with pattern */

	gettimeofday((struct timeval *) icmp->icmp_data, NULL);
	memcpy(&pst->tvrecv, icmp->icmp_data, sizeof(struct timeval));
	len = 8 + pst->datalen;              /* checksum ICMP header and data */
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

	struct sockaddr_in *sa = (struct sockaddr_in *)&pst->pr.sasend;
	printf("addr: %d, %d, %d\n", sa->sin_addr, sa->sin_port, pst->nsent);
	sendto(pst->sockfd, pst->sendbuf, len, 0, &pst->pr.sasend, pst->pr.salen);
}

void recv_v4(struct ping_st *pst)
{
	struct msghdr   msg;
	struct iovec    iov;
	char                    controlbuf[BUFSIZE];    
	int n, errno;

	iov.iov_base = pst->recvbuf;                                                    
	iov.iov_len = sizeof(pst->recvbuf);                                             
	msg.msg_name = &pst->pr.sarecv;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;                                                        
	msg.msg_control = controlbuf;  
	msg.msg_namelen = pst->pr.salen;
	msg.msg_controllen = sizeof(controlbuf);                           
	n = recvmsg(pst->sockfd, &msg, 0);
	if (n < 0) {
		if (errno == EINTR){
			//continue;
		}else{
			printf("recvmsg error\n");
		}
	}
	pst->recvlen = n;
    //struct sockaddr_in *sa = (struct sockaddr_in *)&pst->pr.sasend;
    //printf("addr: %d, %d, %d\n", sa->sin_addr, sa->sin_port, pst->nsent);
    
#if 0
	int i;
	for (i = 0; i < n; i++) {
		printf("%x ", pst->recvbuf[i] & 0xFF);
	}
	printf("recv: %d\n", n);
#endif
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if ( (out->tv_usec -= in->tv_usec) < 0) {       /* out -= in */
		--out->tv_sec;
		out->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}


int proc_v4(struct ping_st *pst)
{
	struct ip               *ip;
	struct icmp             *icmp;
	struct timeval  *tvsend;
	int  hlen1, icmplen;
	double rtt;

	//gettimeofday(&tvrecv, NULL);

    struct sockaddr_in *sa = (struct sockaddr_in *)&pst->pr.sasend;
        //printf("addr: %d, %d, %d\n", sa->sin_addr, sa->sin_port, pst->nsent);

	ip = (struct ip *) pst->recvbuf;         /* start of IP header */
	hlen1 = ip->ip_hl << 2;         /* length of IP header */
	if (ip->ip_p != IPPROTO_ICMP)   
		return -1;                         /* not ICMP */

	icmp = (struct icmp *) (pst->recvbuf + hlen1);   /* start of ICMP header */
	if ( (icmplen = pst->recvlen - hlen1) < 8)
		return -1;                         /* malformed packet */

	if (icmp->icmp_type == ICMP_ECHOREPLY) {
		if (icmp->icmp_id != pst->pid)
			return -1;                 /* not a response to our ECHO_REQUEST */
		if (icmplen < 16)
			return -1;                 /* not enough data to use */

		tvsend = (struct timeval *) icmp->icmp_data;
		printf("%d recv: %d.%d\n", sa->sin_addr, pst->tvrecv.tv_sec, pst->tvrecv.tv_usec);
		//printf("recv: %d.%d\n", tvsend->tv_sec, tvsend->tv_usec);
		tv_sub(&pst->tvrecv, tvsend);
		//printf("recv: %d.%d\n", pst->tvrecv.tv_sec, pst->tvrecv.tv_usec);
		rtt = pst->tvrecv.tv_sec * 1000.0 + pst->tvrecv.tv_usec / 1000.0;
		printf("%d %d bytes from : seq=%u,%d ttl=%d, rtt=%.3f ms\n", sa->sin_addr,
				icmplen, icmp->icmp_seq, pst->nsent, ip->ip_ttl, rtt);
	}else{
		printf("type error\n");
		return -1;
	}
	return 0;
}

int ping_v4(struct ping_st *pst)
{
	send_v4(pst);
	recv_v4(pst);
	return proc_v4(pst);
}
