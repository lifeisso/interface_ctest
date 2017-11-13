include ../cross_config.mk

IMX6UL_CROSS_COMPILE=/usr/local/arm/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-linux-
IMX6_CROSS_COMPILE=/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/arm-linux-gnueabihf-

SRC_DIR = `pwd`
SRC_CPP = $(wildcard *.cpp)
SRC_OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

IMX6UL_LIB = libimx6ul.so
IMX6_LIB = libimx6.so

###########################################################
all: $(IMX6_LIB) $(IMX6UL_LIB)

$(IMX6_LIB): 
	$(IMX6_CROSS_COMPILE)g++ -fPIC -shared -o $@ $(SRC_CPP)
	mv $@ ../lib

$(IMX6UL_LIB): 
	$(IMX6UL_CROSS_COMPILE)g++ -fPIC -shared -o $@ $(SRC_CPP)
	mv $@ ../lib

.PHONY:clean
clean:
	rm *.o
