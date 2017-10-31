#ifndef LINUX_SOCKET_H
#define LINUX_SOCKET_H
#include <stdio.h>
#include <string.h> 
#include <linux/sockios.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <netdb.h>
#include <linux/ethtool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

class LSocket{
public:
    LSocket();
    ~LSocket();

    //设置网卡名字
    void set_eth(char *port);
    //检测网线是否插上
    int get_netlink_status();
    void get_gateway(char *ip);
    void set_gateway(void);
    //检测网络是否通
    int check_net(char *str_ip);
    //获取网卡的IP
    void get_ip(char *host, char *ip);
    void get_ip(char *ip);

private:
    char port_name[30];
};

#endif
