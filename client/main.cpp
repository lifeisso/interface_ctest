#include <stdio.h>

#include "gpio.h"
#include "com.h"
#include "LinuxSocket.h"
#include "LinuxMsg.h"
#include "device.h"

#define BIT232      0
#define BIT485      1
#define BIT422      2
#define BITNET1     3
#define BITNET2     4
#define BITU9       5
#define BITU10      6
#define BITIO       7
#define BITSCREEN   8
#define BITBEEP     9

#define SET_BIT(val, bit)     (val |= 0x1 << bit)
#define CLEAR_BIT(val, bit)     (val &= ~(0x1 << bit))
#define TEST_BIT(val, bit)      (0x1 & (val >> bit))
#define GET_BIT(val, bit)       (((val >> bit) & 0x1) << bit)

//bit, 0:232 1:485 2:422 3:net1 4:net2 5:u9 6:u10 7:io 8:屏
int cmd = 0;
int result = 0, result_old = 0;;
device_des d;

//0：不接收命令， 1：接收命令
int cmd_flag = 0;

void *com_thread(void *arg)
{
    char buf[] = {3, 2, 4};
    int port = (int)arg;
    COM COMPort;
    IMX6_GPIO igpio;
    printf("com pthread %d\n", port);

    if(TEST_BIT(port, BIT232)){
        printf("232 open\n");
        COMPort.COMOpen((char *)d.name_232, 115200);
    }else if(TEST_BIT(port, BIT485)){
        printf("485 open\n");
        COMPort.COMOpen((char *)d.name_485, 115200);
    }else if(TEST_BIT(port, BITU9)){
        COMPort.COMOpen((char *)d.zigbee_in1, 115200);
    }else if(TEST_BIT(port, BITU10)){
        COMPort.COMOpen((char *)d.zigbee_in2, 115200);
    }

    int len = COMPort.COMWrite(buf, sizeof(buf));
    printf("%d write len %d\n", port, len);

    sleep(1);
    char rbuf[100];
    len = COMPort.COMRead(rbuf, 1);
    if(rbuf[0] == 3){                                                                                                                       
            len = COMPort.COMRead(rbuf+1, 1);
            if(rbuf[1] == 2){
                    len = COMPort.COMRead(rbuf+2, 1);
                    if(rbuf[2] == 4){
                            printf("%d read len = %d %x %x %x\n", port, len, rbuf[0], rbuf[1], rbuf[2]);
                            if(len != 0 && ((memcmp(rbuf, buf, len) == 0))){
                                    if(TEST_BIT(port, BIT232)){
                                            result |= 0x1;
                                    }else if(TEST_BIT(port, BIT485)){
                                            result |= 0x2;
                                    }else if(TEST_BIT(port, BITU9)){
                                            for(int i = 0; i < 3; i++){
                                                    igpio.cmd_u9_pwr(GPIO_ON);
                                                    igpio.cmd_u9_rst(GPIO_ON);
                                                    sleep(1);
                                                    igpio.cmd_u9_pwr(GPIO_OFF);
                                                    igpio.cmd_u9_rst(GPIO_OFF);
                                                    sleep(1);
                                            }
                                            SET_BIT(result, BITU9);
                                    }else if(TEST_BIT(port, BITU10)){
                                            for(int i = 0; i < 3; i++){
                                                    igpio.cmd_u10_pwr(GPIO_ON);
                                                    igpio.cmd_u10_rst(GPIO_ON);
                                                    sleep(1);
                                                    igpio.cmd_u10_pwr(GPIO_OFF);
                                                    igpio.cmd_u10_rst(GPIO_OFF);
                                                    sleep(1);
                                            }

                                            SET_BIT(result, BITU10);
                                    }
                            }
                    }
            }
    }
    printf("com exit %d\n", port);
    pthread_exit(NULL);
}

int net_thread_lock = 0;
void *net_thread(void *arg)
{
    net_thread_lock = 1;

    LSocket eth;
    int port = (int)arg;
    char local_ip[20], remote_ip[20];

    sleep(5);
    if(port & 0x8){
        eth.set_eth("eth0");
        strcpy(remote_ip, "192.168.1.40");
    }else if(port & 0x10){
        eth.set_eth("eth1");
        strcpy(remote_ip, "192.168.2.40");
    }

    memset(local_ip, 0, sizeof(local_ip));
    eth.get_ip(local_ip);
    printf("net: %s %s\n", local_ip, remote_ip);
    if(eth.get_netlink_status() == 1){
        if(eth.check_net(remote_ip) == 0){
            if(port & 0x8){
                result |= 0x8;
            }else if(port & 0x10){
                result |= 0x10;
            }
        }
    }
    printf("%s exit\n", remote_ip);
    net_thread_lock = 0;
    pthread_exit(NULL);
}

int GetID(char *send_buf) {
    send_buf[0] = 0x7E;
    send_buf[1] = 0;
    send_buf[2] = 0x04;
    send_buf[3] = 0x08;
    send_buf[4] = 0x52;
    send_buf[5] = 0x49;
    send_buf[6] = 0x44;
    send_buf[7] = 0x18;

    return 8;
}

#if 0
void *zigbee_thread(void *arg)
{

    pthread_exit(NULL);
}
#endif

struct out_to_scats {                                                                                                                          
        char byVD_OUTPUT_ORDER;
        char cBE_VEHICLE;
};

#define I2C "/dev/sim-iic-scats"
#define BE_VEHICLE 1
#define NO_VEHICLE 0
void *io_thread(void *arg)
{
        out_to_scats sOutput;
        int scats_fd = open(I2C, O_RDWR);
        if (scats_fd < 0) {
                printf("open i2c error\n");
                return NULL;
        }

        int i;
        for(int j = 0; j < 3; j++){
            for(i = 0; i < 32; i++){
                sOutput.byVD_OUTPUT_ORDER = i+1;
                sOutput.cBE_VEHICLE = BE_VEHICLE;
                ioctl(scats_fd, 1,&sOutput);
            }
            sleep(1);
            printf("send no \n");
            for(i = 0; i < 32; i++){
                sOutput.byVD_OUTPUT_ORDER = i+1;
                sOutput.cBE_VEHICLE = NO_VEHICLE;
                ioctl(scats_fd, 1,&sOutput);
            }
            sleep(1);
        }
    pthread_exit(NULL);
}

void *zigbee422_thread(void *arg)
{
    COM COMPort;
    IMX6_GPIO igpio;
    char sendbuf[10];
    char rbuf[200];
    int datalen;
    printf("422 thread\n");

#if 1
    for(int i = 0; i < 3; i++){
        //断电
        igpio.cmd_422(GPIO_OFF);
        sleep(1);
        //供电
        igpio.cmd_422(GPIO_ON);
        sleep(1);
    }
#endif
    sleep(3);
    COMPort.COMOpen(d.name_422, 115200);
    //读协调器ID命令
    int len = GetID(sendbuf);
#if 0
    for(int i = 0; i < len; i++){
        printf("%x ", sendbuf[i]);
    }
#endif
    COMPort.COMWrite(sendbuf, len);

    sleep(3);
    len = COMPort.COMRead(rbuf, 3);
    printf("len = %d\n", len);
    if(len == 3){
        if(rbuf[0] == 0x7E){
                //printf("%x %x %x\n", rbuf[0], rbuf[1], rbuf[2]);
            datalen = rbuf[1]*256 + rbuf[2] + 1;
            len = COMPort.COMRead(rbuf, datalen);
            char nCRC = 0xFF;
            for (int i = 0; i < (len-1); i++){
                    nCRC -= rbuf[i];
            }
            printf("\n%d %d\n", nCRC, rbuf[len-1]);
            if(nCRC == rbuf[len-1]){
                SET_BIT(result, BIT422);
            }
        }
    }
    pthread_exit(NULL);
}

void *beep_thread(void *arg)
{
    IMX6_GPIO igpio;

    igpio.cmd_beep();
    pthread_exit(NULL);
}

void *screen_thread(void *arg)
{
    IMX6_GPIO igpio;

    for(int i = 0; i < 5; i++){
        igpio.cmd_lcd(GPIO_OFF);
        sleep(1);
        igpio.cmd_lcd(GPIO_ON);
        sleep(1);
    }
    SET_BIT(result, BITSCREEN);

    pthread_exit(NULL);
}

void auto_test_thread(void)
{   
#if 0
    SET_BIT(cmd, BITSCREEN);
    sleep(1);
    SET_BIT(cmd, BITIO);
    sleep(1);
    SET_BIT(cmd, BIT422);//no
    sleep(1);
    SET_BIT(cmd, BIT485);
    sleep(1);
    SET_BIT(cmd, BITNET1);
    sleep(1);
    SET_BIT(cmd, BITNET2);
    sleep(1);
    SET_BIT(cmd, BITU9);
    sleep(1);
    SET_BIT(cmd, BITU10);
    sleep(1);
    SET_BIT(cmd, BITIO);
    sleep(1);
    SET_BIT(cmd, BITSCREEN);
    sleep(1);
    SET_BIT(cmd, BITBEEP);
    sleep(1);
#endif
    cmd_flag = 1;
}

int main(int argc, char *argv[])
{
    pthread_t pid; 
    CMsg msg(0x5000);
    struct MsgBuf mb;

    cmd_flag = 0;
    auto_test_thread();

    while(1){
        //接收qt命令
        if(cmd_flag == 1){
            mb.mtype = 2;
            if(msg.RecvMsg(mb.mtype, &cmd, sizeof(cmd))){
                //cmd = atoi(buf);
                printf("\033[31mrecv cmd = %x, result = %x\n\033[0m", cmd, result);
            }
        }

        if(TEST_BIT(cmd, BIT232)){
            //232
            pthread_create(&pid, NULL, com_thread, (void *)GET_BIT(cmd, BIT232));
            CLEAR_BIT(cmd, BIT232);
        }
        if(TEST_BIT(cmd, BIT485)){
            //485
            pthread_create(&pid, NULL, com_thread, (void *)GET_BIT(cmd, BIT485));
            CLEAR_BIT(cmd, BIT485);
        }
        if(TEST_BIT(cmd, BIT422)){
            //422
            pthread_create(&pid, NULL, zigbee422_thread, (void *)GET_BIT(cmd, BIT422));
            CLEAR_BIT(cmd, BIT422);
        }
        if(net_thread_lock == 0 && TEST_BIT(cmd, BITNET1)){
            //net1
            pthread_create(&pid, NULL, net_thread, (void *)GET_BIT(cmd, BITNET1));
            CLEAR_BIT(cmd, BITNET1);
        }else if(net_thread_lock == 0 && TEST_BIT(cmd, BITNET2)){
            //net2
            pthread_create(&pid, NULL, net_thread, (void *)GET_BIT(cmd, BITNET2));
            CLEAR_BIT(cmd, BITNET2);
        }
        if(TEST_BIT(cmd, BITU9)){
            //u9
            pthread_create(&pid, NULL, com_thread, (void *)GET_BIT(cmd, BITU9));
            CLEAR_BIT(cmd, BITU9);
        }
        if(TEST_BIT(cmd, BITU10)){
            //u10
            pthread_create(&pid, NULL, com_thread, (void *)GET_BIT(cmd, BITU10));
            CLEAR_BIT(cmd, BITU10);
        }
        if(TEST_BIT(cmd, BITIO)){
            //IO
            pthread_create(&pid, NULL, io_thread, (void *)GET_BIT(cmd, BITIO));
            CLEAR_BIT(cmd, BITIO);
        }
        if(TEST_BIT(cmd, BITSCREEN)){
            //screen
            pthread_create(&pid, NULL, screen_thread, (void *)GET_BIT(cmd, BITSCREEN));
            CLEAR_BIT(cmd, BITSCREEN);
        }

        if(TEST_BIT(cmd, BITBEEP)){
            //BEEP
            pthread_create(&pid, NULL, beep_thread, (void *)GET_BIT(cmd, BITBEEP));
            CLEAR_BIT(cmd, BITBEEP);
        }
        sleep(1);
        if(result != result_old){
                printf("\033[32mcmd = %x, send result = %x\n\033[0m", cmd, result);
                //发送结果给qt
                mb.mtype = 1;
                msg.SendMsg(mb.mtype, &result, sizeof(result));
                result_old = result;
        }
    }

    return 0;
}

