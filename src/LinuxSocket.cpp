
#include "LinuxSocket.h"
#include "ping.h"

LSocket::LSocket()
{
}

LSocket::~LSocket()
{
}

void LSocket::set_eth(char *port)
{
    strcpy(port_name, port);
}

int LSocket::get_netlink_status()
{
	int skfd;
	struct ifreq ifr;
	struct ethtool_value edata;
	edata.cmd = ETHTOOL_GLINK;
	edata.data = 0;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, port_name, sizeof(ifr.ifr_name) - 1);
	ifr.ifr_data = (char *) &edata;
	if (( skfd = socket( AF_INET, SOCK_DGRAM, 0 )) == 0)
		return -1;
	if(ioctl( skfd, SIOCETHTOOL, &ifr ) == -1)
	{
		close(skfd);
		return -1;
	}
	close(skfd);
	return edata.data;
}

void LSocket::get_gateway(char *ip)
{
}

int LSocket::check_net(char *str_ip)
{
	int i;
	struct sockaddr_in addr;
    struct sockaddr addr1;
    struct ping_st *pst;

	addr.sin_family = AF_INET;
	addr.sin_port = 0;
	inet_pton(AF_INET, str_ip, &addr.sin_addr);
    memcpy(&addr1, &addr, sizeof(addr1)); 
	pst = ping_v4_init(addr1);

	for (i = 0; i < 10; i++) {
		if(ping_v4(pst) == 0){
			return 0;
		}	
		sleep(2);
	}
	return 1;
}

struct addrinfo *Host_serv(const char *host, const char *serv, int family, int socktype)
{
	int                             n;
	struct addrinfo hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;  /* always return canonical name */
	hints.ai_family = family;               /* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;   /* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		printf("host_serv error for %s, %s: %s",
				(host == NULL) ? "(no hostname)" : host,
				(serv == NULL) ? "(no service name)" : serv,
				gai_strerror(n));

	return(res);    /* return pointer to first on linked list */
}


//#define HOST	"www.baidu.com"
void LSocket::get_ip(char *host, char *ip)
{
	struct addrinfo *res;
	struct sockaddr_in *addr;

	memset(ip, 0, 20);
	res = Host_serv(host, NULL, 0, 0);
	if(res == NULL){
		printf("not find ip\n");
		return ;
	}
	addr = (struct sockaddr_in *)res->ai_addr;
	if(addr->sin_family == AF_INET){
		if(inet_ntop(AF_INET, &addr->sin_addr, ip, 20) != NULL){
			printf("ip: %s\n", ip);
		}
	}
}

void LSocket::get_ip(char *ip)
{
        int sock_get_ip;  

        struct   sockaddr_in *sin;  
        struct   ifreq ifr_ip;     

        if ((sock_get_ip=socket(AF_INET, SOCK_STREAM, 0)) == -1){
                printf("socket create failse...GetLocalIp!/n");  
                return ;  
        }  

        memset(&ifr_ip, 0, sizeof(ifr_ip));     
        strncpy(ifr_ip.ifr_name, port_name, sizeof(ifr_ip.ifr_name) - 1); 
        if( ioctl( sock_get_ip, SIOCGIFADDR, &ifr_ip) < 0 ){
            close( sock_get_ip );  
            return ;     
        }       
        sin = (struct sockaddr_in *)&ifr_ip.ifr_addr;     
        strcpy(ip,inet_ntoa(sin->sin_addr));         

        close( sock_get_ip );  
}

void LSocket::set_gateway(void)
{
	char Buf[400];
	char gateway_ip[20];

	get_gateway(gateway_ip);
	memset(Buf, 0, sizeof(Buf));
	strcat(Buf, "route add default gw ");
	strcat(Buf, gateway_ip);
	strcat(Buf, " dev eth0");
	printf("cmd: %s\n", Buf);
	system(Buf);
}


