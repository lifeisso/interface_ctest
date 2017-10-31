
####################################################################################### 
#                                   Variable Define                                   #           
####################################################################################### 
#export CROSS_COMPILE=/usr/local/arm/arm-2007q3/bin/arm-none-linux-gnueabi-
include ../cross_config.mk
#export CROSS_COMPILE=

TOPDIR= `pwd`

LDFLAGS+=-lpthread
CFLAGS=-Wall -g2  -I./  -I../src 

TARGET ?= imx6_server

#SQLITE_INCLUDE=/home/wzq/9263/lib/sqlite-autoconf-3080500_9263/
SQLITE_LIB=../src

SRC_DIR=$(TOPDIR)/../src/
#SRC_INC=$(TOPDIR)/../include/
#SRC_LIB=$(SRC_DIR)/*.o
SRC_LIB=$(shell ls $(SRC_DIR)/*.cpp)
OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

############################################################################


all: $(TARGET)

$(TARGET):  $(OBJS) 
	$(CROSS_COMPILE)g++ -o $@ $^ $(SRC_LIB) $(CFLAGS) $(LDFLAGS) -lrt -L$(SQLITE_LIB) 
	cp $@ /nfs

%.o: %.cpp
	$(CROSS_COMPILE)g++ -c -o $@ $^ $(CFLAGS)


.PHONY:clean 
clean:  
	-@$(RM) *.o *~ $(TARGET) 




