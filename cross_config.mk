tool = imx6ul

ifeq ($(tool), 9263)
	CROSS_COMPILE=/usr/local/arm/arm-2007q3/bin/arm-none-linux-gnueabi-
else ifeq ($(tool), 2440)
	CROSS_COMPILE=/usr/local/arm/4.3.2/bin/arm-linux-
else ifeq ($(tool), imx6d)
	CROSS_COMPILE=/usr/local/arm/gcc-4.6.2-glibc-2.13-linaro-multilib-2011.12/fsl-linaro-toolchain/bin/arm-linux- 
else ifeq ($(tool), imx6ul)
	CROSS_COMPILE=/usr/local/arm/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/bin/arm-linux-gnueabihf-
endif
GPP =$(CROSS)g++

TARGET_DIR=/nfs

export tool
export CROSS
export CROSS_COMPILE GPP
export TARGET_DIR

