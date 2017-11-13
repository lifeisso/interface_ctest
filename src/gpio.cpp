#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "gpio.h"

IMX6_GPIO::IMX6_GPIO()
{
#ifdef IMX6
	gpio_fd = open(GPIO_DEV,O_RDWR);
	if(gpio_fd < 0 ){
		printf("open GPIO dev error \n");
		return;
	}
#endif
}

IMX6_GPIO::~IMX6_GPIO()
{
#ifdef IMX6
    if(gpio_fd > 0){
        close(gpio_fd);
    }
#endif
}

int IMX6_GPIO::cmd_422(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_422, 1);  
#else
        SetGpioStatus(CMD_422_PWR, 1);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_422, 0);  
#else
        SetGpioStatus(CMD_422_PWR, 0);
#endif
    }
    return 0;
}

int IMX6_GPIO::ul_lcd(int chLightLevel)
{
        int fd, iRet;
        char chTmp[5];

        if(chLightLevel > 7) return -1;
        fd = open(BACKLIGHT_PATH, O_RDWR);
        if(fd < 0) {
            perror("open backlight:");
            return -1;
        }
        sprintf(chTmp, "%d", chLightLevel);
        iRet = write(fd, chTmp, 1);
        close(fd);
        if(iRet < 0) {
            perror("write backlight:");
            return -1;
        }
        return 0;
}

int IMX6_GPIO::cmd_lcd(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
        ioctl(gpio_fd, CMD_PWN_LCD, 1);  
#else
        ul_lcd(7);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_LCD, 0);  
#else
        ul_lcd(0);
#endif
    }
    return 0;
}

int IMX6_GPIO::cmd_u9_pwr(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_RFA, 1);  
#else
        SetGpioStatus(CMD_RF1_PWR, 1);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_RFA, 0);  
#else
        SetGpioStatus(CMD_RF1_PWR, 0);
#endif
    }
    return 0;

}

int IMX6_GPIO::cmd_u9_rst(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
		ioctl(gpio_fd, CMD_RF_A, 1);  
#else
        SetGpioStatus(CMD_RF1_RST, 1);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_RF_A, 0);  
#else
        SetGpioStatus(CMD_RF1_RST, 0);
#endif
    }
    return 0;
}

int IMX6_GPIO::cmd_u10_pwr(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_RFB, 1);  
#else
        SetGpioStatus(CMD_RF2_PWR, 1);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_PWN_RFB, 0);  
#else
        SetGpioStatus(CMD_RF2_PWR, 0);
#endif
    }
    return 0;

}

int IMX6_GPIO::cmd_u10_rst(int k)
{
    if(k == GPIO_ON){
#ifdef IMX6
		ioctl(gpio_fd, CMD_RF_B, 1);  
#else
        SetGpioStatus(CMD_RF2_RST, 1);
#endif
    }else{
#ifdef IMX6
		ioctl(gpio_fd, CMD_RF_B, 0);  
#else
        SetGpioStatus(CMD_RF2_RST, 0);
#endif
    }
    return 0;
}

#define BEEP_ON  _IO('k', 0)
#define BEEP_OFF _IO('k', 1)
#define SET_CNT  _IO('k', 2)
#define SET_PRE  _IO('k', 3)

int IMX6_GPIO::cmd_beep()
{
#ifdef IMX6
    int dev_fd;
    dev_fd = open("/dev/PPWM",O_RDWR | O_NONBLOCK);
    if ( dev_fd == -1 ) {
            perror("open");
            exit(1);
    }

    for(int i = 0; i < 10; i++){
            ioctl(dev_fd,BEEP_ON,0);
            ioctl(dev_fd,SET_CNT,50);
            usleep(60000);
            ioctl(dev_fd,BEEP_OFF,0);
            sleep(1);
    }
    close(dev_fd);
#else
    char on[]  = "100";
    char off = '0';
    int fd, ret;

    fd = open("/sys/class/leds/beeper-pwm/brightness", O_RDWR);
    if (fd < 0) {
        perror("open /sys/class/leds/beeper-pwm/brightness\n");
        return -1;
    }

    for(int i = 0; i < 10; i++){
        ret = write(fd, on, sizeof(on));
        sleep(1);
        ret = write(fd, &off, 1);
        sleep(1);
    }
    close(fd);
#endif
    return 0;
}

int IMX6_GPIO::GetGpioNum(int cmd)
{
	int iRet=0;
		switch(cmd){
		case CMD_RF1_RST:
			iRet=9;
			break;
		case CMD_RF2_RST:
			iRet=137;
			break;
		case CMD_RF1_PWR:
			iRet = 11;
			break;
		case CMD_RF2_PWR:
			iRet = 134;
			break;
		case CMD_422_PWR:
			iRet = 91;
			break;
		case CMD_PWR_DOWN:
			iRet = 23;
			break;
		}
		return iRet;
}


#if 0
int IMX6_GPIO::SetGpioDir(int cmd, int dir)
{
    char path[100], tmp_data[100];
    int fd_export, fd_dir, fd_dev;
    int num, ret;

    num = GetGpioNum(cmd);
    printf("num = %d\n", num);
    sprintf(path, EXPORT_PATH, num);
    printf("path %s\n", path);
    fd_export = open(path, O_WRONLY); // 打开 GPIO 设备导出设备
    if(fd_export < 0) {
        perror("open export:");
        return -1;
    }

    sprintf(tmp_data, "%d", num);
    write(fd_export, tmp_data, strlen(tmp_data));


    sprintf(path, DEV_PATH, num);
    printf("path %s\n", path);
    fd_dev = open(path, O_RDWR);  // 打开输入输出电平值设备
    if(fd_dev < 0) {
        perror("open gpio:");
        return -1;
    }

    sprintf(path, DIRECT_PATH, num);
    printf("path %s\n", path);
    fd_dir = open(path, O_RDWR); // 打开 GPIO 输入输出控制设备
    if(fd_dir < 0) {
        perror("open direction:");
        return -1;
    }
    if(dir == GPIO_DIR_IN){
        strcpy(tmp_data, "in");
    }else if(dir == GPIO_DIR_OUT){
        strcpy(tmp_data, "out");
    }
    printf("dir %s\n", tmp_data);
    ret = write(fd_dir, tmp_data, strlen(tmp_data));
    if(ret < 0) {
        perror("write direction:");
        close(fd_export);
        close(fd_dir);
        close(fd_dev);
        return -1;
    }
    //printf("current direction:%sinput level:%s", direction, buf);
    close(fd_export);
    close(fd_dir);
    close(fd_dev);
    return 0;
}
#endif

int IMX6_GPIO::SetGpioStatus(int iGpio, int iStatus){
	char path[100], tmp_data[100];
    int fd_export, fd_dir, fd_dev;
    int num, ret;

    num = GetGpioNum(iGpio);
    sprintf(path, EXPORT_PATH, num);
//    printf("export file:%s\n", path);
    fd_export = open(path, O_WRONLY); // 打开 GPIO 设备导出设备
    if(fd_export < 0) {
        perror("open export:");
        return -1;
    }

    sprintf(tmp_data, "%d", num);
    write(fd_export, tmp_data, strlen(tmp_data));

    sprintf(path, DEV_PATH, num);
//    printf("value file:%s\n", path);
    fd_dev = open(path, O_RDWR);  // 打开输入输出电平值设备
    if(fd_dev < 0) {
        perror("open gpio:");
        return -1;
    }

    sprintf(path, DIRECT_PATH, num);
//    printf("direct file:%s\n", path);
    fd_dir = open(path, O_RDWR); // 打开 GPIO 输入输出控制设备
    if(fd_dir < 0) {
        perror("open direction:");
        return -1;
    }
//    if(dir == GPIO_DIR_IN){
//        strcpy(tmp_data, "in");
//    }else if(dir == GPIO_DIR_OUT){
//        strcpy(tmp_data, "out");
//    }
    strcpy(tmp_data, "out");
    ret = write(fd_dir, tmp_data, strlen(tmp_data));
    if(ret < 0) {
        perror("write direction:");
        close(fd_export);
        close(fd_dir);
        close(fd_dev);
        return -1;
    }
    //printf("current direction:%sinput level:%s", direction, buf);
    memset(tmp_data, 0, 100);
    sprintf(tmp_data, "%d", iStatus);
    ret = write(fd_dev, tmp_data, sizeof(tmp_data));
    if(ret < 0){
    	perror("set io status:");
        close(fd_export);
        close(fd_dir);
        close(fd_dev);
        return -1;
    }
    close(fd_export);
    close(fd_dir);
    close(fd_dev);
    return 0;
}
#if 0
int main()
{
	int fd = -1;
	int data;
	char buf[1024];

	

		sleep(DELAYS);
		return 0;
	while(1){
		ioctl(fd, CMD_GPIO_A, 0);  
		ioctl(fd,CMD_GPIO_B,0); 
		ioctl(fd,CMD_GPIO_C,0);  
		ioctl(fd,CMD_GPIO_D,0);  
		ioctl(fd,CMD_PWN_RFA,1);  
		ioctl(fd,CMD_RF_A,0);  
		ioctl(fd,CMD_PWN_RFB,1);  
		ioctl(fd,CMD_RF_B,0);  
		ioctl(fd,CMD_PWN_422,0);  
		ioctl(fd,CMD_PWN_LCD,0);  
		sleep(DELAYS);


		ioctl(fd, CMD_GPIO_A, 1);  
		ioctl(fd,CMD_GPIO_B,1); 
		ioctl(fd,CMD_GPIO_C,1);  
		ioctl(fd,CMD_GPIO_D,1);  
		ioctl(fd,CMD_PWN_RFA,0);  
		ioctl(fd,CMD_RF_A,1);  
		ioctl(fd,CMD_PWN_RFB,0);  
		ioctl(fd,CMD_RF_B,1);  
		ioctl(fd,CMD_PWN_422,1);  
		ioctl(fd,CMD_PWN_LCD,1);  
		sleep(DELAYS);
	}
}
#endif
#if 0
		ioctl(fd,CMD_GPIO_A,0);  
		ioctl(fd,CMD_GPIO_B,0); 
		ioctl(fd,CMD_GPIO_C,0);  
		ioctl(fd,CMD_GPIO_D,0);  
		ioctl(fd,CMD_RF_A,0);  
		ioctl(fd,CMD_RF_B,0);  
		ioctl(fd,CMD_PWN_RFA,0);  
		ioctl(fd,CMD_PWN_RFB,0);  
		ioctl(fd,CMD_PWN_422,0);  
		ioctl(fd,CMD_PWN_LCD,0);  
#endif


