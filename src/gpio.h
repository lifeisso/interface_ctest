#ifndef GPIO_H_
#define GPIO_H_

#ifdef IMX6
#define CMD_GPIO_A _IO('k', 1)
#define CMD_GPIO_B _IO('k', 2)
#define CMD_GPIO_C _IO('k', 3)
#define CMD_GPIO_D _IO('k', 4)
#define CMD_RF_A   _IO('k', 5)
#define CMD_RF_B   _IO('k', 6)
#define CMD_PWN_RFA _IO('k', 7)
#define CMD_PWN_RFB _IO('k', 8)
#define CMD_PWN_422 _IO('k', 9)
#define CMD_PWN_LCD _IO('k', 10)
#endif
//控制gpio输出高低
#define GPIO_ON     1
#define GPIO_OFF    0

//gpio设备名字
#define GPIO_DEV "/dev/GPIO"

/*gpio口*/
enum CMD_GPIO{
    //内部zigbee1复位脚
    CMD_RF1_RST,
    //内部zigbee2复位脚
    CMD_RF2_RST,
    //内部zigbee1电源脚
    CMD_RF1_PWR,
    //内部zigbee2电源脚
    CMD_RF2_PWR,
    //422电源脚
    CMD_422_PWR,
    CMD_PWR_DOWN,
    //IO->I2C
    CMD_GPIO_A,
    CMD_GPIO_B,
    CMD_GPIO_C,
    CMD_GPIO_D,
};
#define  PWR_OFF 0
#define  PWR_ON 1

#define	GPIO_DIR_IN	0
#define   GPIO_DIR_OUT 1

#define  DEV_PATH  "/sys/class/gpio/gpio%d/value"  // 输入输出电平值设备
#define  EXPORT_PATH "/sys/class/gpio/export"  // GPIO 设备导出设备
#define  DIRECT_PATH "/sys/class/gpio/gpio%d/direction" // GPIO 输入输出控制设备
#define	 BACKLIGHT_PATH "/sys/class/backlight/backlight/brightness" //背光控制设备



class IMX6_GPIO{
public:
    IMX6_GPIO();
    ~IMX6_GPIO();

    //控制422电源
    int cmd_422(int k);
    //控制屏幕亮灭
    int cmd_lcd(int k);

    int cmd_u9_pwr(int k);

    int cmd_u9_rst(int k);

    int cmd_u10_pwr(int k);

    int cmd_u10_rst(int k);
    
    int cmd_beep();
    int gpio_fd;
private:
    int SetGpioStatus(int iGpio, int iStatus);
    //int SetGpioDir(int cmd, int dir);
    int GetGpioNum(int cmd);
    int ul_lcd(int chLightLevel);
};

#endif
