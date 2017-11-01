#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "device.h"
#include "RegMatch.h"

#if 1
#define DEVICE_DEBUG(...)
#else
    //printf("[%s:%s] ---> ", __FILE__,  __FUNCTION__);    
#define DEVICE_DEBUG(...) do{   \
        printf("[%s] ---> ",  __FUNCTION__);    \
        printf(__VA_ARGS__);          \
}while(0)
#endif

device_des::device_des()
{
    device_type = get_board_type();
    if(device_type == TYPE_9263){
        DEVICE_DEBUG("Board: zhongqian 9263\n");
    }else if(device_type == TYPE_IMX6D){
        DEVICE_DEBUG("Board: pinnike imx6d\n");
    }else if(device_type == TYPE_IMX6UL){
        DEVICE_DEBUG("Board: zhiyuan imx6ul\n");
    }else{
        DEVICE_DEBUG("Board: not find\n");
    }
    get_device_name();
    DEVICE_DEBUG("422: %s\n 232: %s\n485: %s\ninside1: %s\ninside2: %s\n", name_422, name_232, name_485, zigbee_in1, zigbee_in2);
}

device_des::~device_des()
{

}

void device_des::get_device_name()
{
    RegMatch re("([a-zA-Z0-9]+):([a-zA-Z0-9/]+)");
    char buf[1024], *p;
    char tmp_buf[100], tmp_buf1[100];
    int len;
    int find_flag = 0;
    FILE *fp = fopen("./device_des.conf", "r");
    if(fp == NULL){
        DEVICE_DEBUG("fopen fail\n");
        return;
    }

    while(1){
        if((p = fgets(buf, sizeof(buf), fp)) == NULL){
            break;
        }
        if(buf[0] == '\n'){
            continue;
        }
        printf("%s", buf);
        if(find_flag == 0){
            if(device_type == TYPE_9263){
                if(strstr(buf, "9263") != NULL){
                    find_flag = 1;
                }
            }else if(device_type == TYPE_IMX6D){
                if(strstr(buf, "imx6d") != NULL){
                    find_flag = 1;
                }
            }else if(device_type == TYPE_IMX6UL){
                if(strstr(buf, "imx6ul") != NULL){
                    find_flag = 1;
                }
            }
        }else if(find_flag == 1){
            if(re.RegMatch_match(buf) < 0)continue;
            for(int i = 1; re.pmatch[i].rm_eo != -1;){
                memset(tmp_buf, 0, sizeof(tmp_buf));
                len = re.RegMatch_next_data(i, tmp_buf);
                if(len == 0)continue;
                i++;
                len = re.RegMatch_next_data(i, tmp_buf1);
                if(len == 0)continue;
                DEVICE_DEBUG("#%s %s\n", tmp_buf, tmp_buf1);
                if(strcmp(tmp_buf, "outside") == 0){
                    strcpy(name_422, tmp_buf1);
                }else if(strcmp(tmp_buf, "232") == 0){
                    strcpy(name_232, tmp_buf1);
                }else if(strcmp(tmp_buf, "485") == 0){
                    strcpy(name_485, tmp_buf1);
                }else if(strcmp(tmp_buf, "inside1") == 0){
                    strcpy(zigbee_in1, tmp_buf1);
                }else if(strcmp(tmp_buf, "inside2") == 0){
                    strcpy(zigbee_in2, tmp_buf1);
                }
            }
        }
    }
    fclose(fp);
}

int device_des::get_board_type()
{
    int type = -1;
    char buf[1024], *p;
    FILE *fp = fopen("/proc/cpuinfo", "r");
    if(fp == NULL){
        DEVICE_DEBUG("open fail");
        return type;
    }

    while(1){
        if((p = fgets(buf, sizeof(buf), fp)) == NULL){
            break;
        }

        if(strstr(buf, "Hardware") != NULL){
            if(strstr(buf, "i.MX6 Ultralite") != NULL){
                type = TYPE_IMX6UL;
            }else if(strstr(buf, "6Quad/DualLite/Solo") != NULL){
                type = TYPE_IMX6D;
            }
            break;
        }
    }
    fclose(fp);

    return type;
}
