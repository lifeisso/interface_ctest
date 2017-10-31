#include <stdio.h>

#include "gpio.h"
#include "com.h"

//COM COM232;// COM485, COM422;

#define DEVICE_232      "/dev/ttymxc1"
#define DEVICE_485      "/dev/ttyS0"
#define DEVICE_422      "/dev/ttymxc2"
#define DEVICE_U9      "/dev/ttymxc3"
#define DEVICE_U10      "/dev/ttymxc4"

//bit, 0:232 1:485 2:422 3:net1 4:net2
int cmd = 0;
int result = 0;

void *com_thread(void *arg)
{
    int port = (int)arg;
    COM COMPort;
    printf("com pthread %d\n", port);


    if(port & 0x1){
         while(1){
            if(COMPort.COMOpen((char *)DEVICE_232, 115200) < 0){
                sleep(1);
            }else break;
         }
         printf("232 open \n");

    }else if(port & 0x2){
         while(1){
            if(COMPort.COMOpen((char *)DEVICE_485, 115200) < 0){
                sleep(1);
            }else break;
         }
    }

    char rbuf[100];
    int len = 3, retLength, i;
    while(1){
            memset(rbuf, 0 , sizeof(rbuf));
            len = COMPort.COMRead(rbuf, 1);
            if(rbuf[0] == 3){
                len = COMPort.COMRead(rbuf+1, 1);
                if(rbuf[1] == 2){
                    len = COMPort.COMRead(rbuf+2, 1);
                    if(rbuf[2] == 4){
                        printf("%d read len = %d %x %x %x\n", port, len, rbuf[0], rbuf[1], rbuf[2]);
                        COMPort.COMWrite(rbuf, 3);
                    }
                }
            }
    }
}

void *net_thread(void *arg)
{

}

int main(int argc, char *argv[])
{
    pthread_t pid; 

    //232
    cmd = 0x1;
    pthread_create(&pid, NULL, com_thread, (void *)cmd);
    //485
    cmd = 0x2;
    pthread_create(&pid, NULL, com_thread, (void *)cmd);


    pause();
    return 0;
}

