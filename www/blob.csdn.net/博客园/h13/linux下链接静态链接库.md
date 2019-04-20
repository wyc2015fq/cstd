# linux下链接静态链接库 - h13 - 博客园
生成静态链接库/动态链接库的makefile如下：
```
#CC:=arm-linux-gcc
CFLAGS := -I..
LDFLAGS := -lpthread -lrt
SRCS := $(wildcard *.c)
ifdef ARCH
libclient := libclient-$(ARCH).a
else
libclient := libclient.a
endif
all: $(libclient) libclient.so
$(libclient): client.o net.o
 $(AR) -rcv $@ $^
libclient.so: client.o net.o
 $(CC) -shared -o $@ $^
.depend: Makefile $(SRCS)
 @$(CC) $(CFLAGS) -MM $(SRCS) >$@
sinclude .depend
clean:
 rm -rf *.o .depend $(test_client) apitest $(libclient) libclient.so
```
链接动态链接库的makefile如下：
```
ARCH=arm-linux
ifdef ARCH
CC:=$(ARCH)-gcc
AR:=$(ARCH)-ar
export ARCH
export CC
export AR
endif
CFLAGS:=-I..
LDFLAGS:=-L. -lrt
SRCS := $(wildcard *.c)
ifdef ARCH
camera_track := camera_track-$(ARCH)
LIB:=-lclient-$(ARCH)
else
camera_track := camera_track
LIB:=-lclient
endif
all: $(camera_track)
$(camera_track): camera_track.o camera_ctrl.o
 $(CC) $(LDFLAGS) -o $@ $^ $(LIB)
.depend: Makefile $(SRCS)
 @$(CC) $(CFLAGS) -MM $(SRCS) >$@
sinclude .depend
clean:
 rm -rf *.o .depend $(camera_track)
```
当需要直接用命令行进行编译时，可以用如下命令：
arm-linux-gcc -L. -lrt -o camera_track camera_track.c camera_ctrl.c -lclient
