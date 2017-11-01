#ifndef DEVICE_H
#define DEVICE_H

/*获得板子设备名字*/
class device_des{
public:
    device_des();
    ~device_des();

    char name_232[100];
    char name_485[100];
    char name_422[100];
    char zigbee_in1[100];
    char zigbee_in2[100];

private:
    void get_device_name();

enum{
    TYPE_9263,
    TYPE_IMX6D,
    TYPE_IMX6UL,
};
    /*
     *获得是哪个板子 
     * 存储在device_type中
     * */
    int get_board_type();
    int device_type;
};

#endif
