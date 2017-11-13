
####################################################################################### 
#                                   Variable Define                                   #           
####################################################################################### 
include ../cross_config.mk

TOPDIR= `pwd`

LDFLAGS+=-lpthread -lrt
CFLAGS=-Wall -g2  -I./  -I../src 

TARGET ?= imx6_client

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

############################################################################

all: $(TARGET)

$(TARGET): $(OBJS) 
	$(CROSS_COMPILE)g++ -o $@ $^ $(CFLAGS) $(LDFLAGS) 
	cp $@ /nfs
	rm *.o

%.o: %.cpp
	$(CROSS_COMPILE)g++ -c -o $@ $^ $(CFLAGS)

.PHONY:clean 
clean:  
	-@$(RM) *.o *~ $(TARGET) 

