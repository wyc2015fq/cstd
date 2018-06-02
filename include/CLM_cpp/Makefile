ifeq "" "$(ARCH)"
        ARCH=arm64
endif

ifeq ($(ARCH),linux)
	CC=
	CFLAGES = -Wall -g -DRYW_LINUX
	LIBS=-lpthread -lm  
else ifeq ($(ARCH),arm)	
	CC=arm-linux-androideabi-
	CFLAGES = -Wall -g -DRYW_ANDROID -pie -fPIE
	LIBS=-lm -llog 
else ifeq ($(ARCH),arm64)
	CC=aarch64-linux-android-
	CFLAGES = -w -g -DRYW_ANDROID -pie -fPIE -std=c++11
	LIBS=-lm -llog -lc++ -ldlib -lopencv_core -lopencv_objdetect -lopencv_video -lopencv_calib3d -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_flann
else
$(warning ARCH=$(ARCH),[ARCH=linux/arm/arm64])
endif

INCS= -I./ 
TARGET = rywclm

DIRS = .

C_SRC = $(foreach dir, $(DIRS),$(wildcard $(dir)/*.c))
CPP_SRC = $(foreach dir, $(DIRS),$(wildcard $(dir)/*.cpp))
OBJS = $(C_SRC:.c=.o) $(CPP_SRC:.cpp=.o)

$(TARGET):$(OBJS)
	$(CC)g++ -o $(TARGET) $(OBJS) $(CFLAGES) $(LIBS)

%.o:%.c         
	$(CC)gcc $(INCS) -c  $< $(CFLAGES) -o $@ 

%.o:%.cpp         
	$(CC)g++ $(INCS) -c  $< $(CFLAGES) -o $@ 

clean:
	-rm -f $(TARGET) $(OBJS)
