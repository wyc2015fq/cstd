
# 交互系统的构建之（二）Linux下鼠标和键盘的模拟控制 - zouxy09的专栏 - CSDN博客


2012年08月29日 14:53:03[zouxy09](https://me.csdn.net/zouxy09)阅读数：13480


**交互系统的构建之（二）****Linux****下鼠标和键盘的模拟控制**
zouxy09@qq.com
http://blog.csdn.net/zouxy09
[ ](mailto:zouxy09@qq.com)
交互系统的构建之（一）http://blog.csdn.net/zouxy09/article/details/7919618中提到我的整个交互系统包含以下部分：TLD系统、TTS语音合成、语音识别、手势和语音控制鼠标和键盘、运行前加入手掌的检测（这样就不用鼠标画目标box了）、拳头的检测等等。
**目前已完成：**
1、TLD系统的介绍与编译：
http://blog.csdn.net/zouxy09/article/details/7893022
2、TLD系统工作过程分析：
http://blog.csdn.net/zouxy09/article/details/7893026
3、重写Makefile编译TLD系统:
http://blog.csdn.net/zouxy09/article/details/7919618
**本文将完成：**
Linux下鼠标和键盘的模拟控制，也就是为手势和语音控制鼠标和键盘部分服务的。
有关于本系统构建的文章结构都会由三个部分来组织，一是该功能模块的介绍和在Linux下简单应用程序的实现；二是将该功能模块整合到交互系统（先以TLD为地基）中去；三是分析目前存在的问题与未来的解决构思。
**一、****input****子系统和模拟程序编写：**
Linux输入子系统是Linux内核用于管理各种输入设备（键盘，鼠标，遥控杆，书写板等等）的。输入子系统分为三块：input core，drivers和event handlers。正常的路径是从底层硬件到驱动，从驱动到input core，从input core到event handler，从event handler到user space。
这么说吧：如果是没有这个input子系统的话，假如我们用键盘按下了一个键A，键盘会有一个linux的设备驱动文件，假设是/dev/keyboard，我们的用户空间的应用程序就会打开并访问这个设备文件/dev/keyboard，应用程序会轮询这个文件，一旦你按下了一个键A了，它就会返回给用户程序说你按下了键A。那么如果有了input子系统的话，我们的用户空间的应用程序就不是直接打开和访问键盘的设备驱动文件了，而是访问由键盘驱动在input子系统中注册的event事件文件，例如/dev/input/event3，而对于键盘驱动来说，它也是实现由input子系统提供的接口就可以了。按下键了，就发送给input子系统。不再直接与用户空间的应用程序直接面对面了。这样，就很方便的对驱动和应用程序都统一了接口，而且同一种接口还适合管理多种硬件。好像很简单的问题给我啰嗦化了，不知道有没有说错，呵呵。
这个input子系统可以很容易地让我们在用户空间模拟鼠标和键盘事件。例如，你可以写一个应用程序，往input子系统的/dev/input/event3设备文件（假设这个是键盘设备文件）写入A，这样就相当于你通过键盘按下了A，而这个A对系统任意的一个当前活动窗口有效（捕捉）。
要实现这个功能，我们需要回答一下几个问题：
**1****、往什么设备文件写？**
也就是如何查看哪些设备文件是键盘的，哪些是鼠标的，找到这些设备文件，我们才可以通过应用程序来打开和写入键值（或者鼠标的控制信息）。
通过\#cat /proc/bus/input/devices可以查看到当前input子系统下面的所有event设备，我们找到鼠标和键盘的即可。
例如：
I: Bus=0003 Vendor=046d Product=c018 Version=0111
N: Name=" USB OpticalMouse"
P: Phys=usb-0000:00:1d.1-2/input0
S: Sysfs=/class/input/input24
U: Uniq=
H: Handlers=mouse1event2
B: EV=7
B: KEY=70000 0 0 0 0 0 0 0 0
B: REL=103
上面Name处可以看到这个鼠标设备，然后对应的句柄Handlers是event2；
**2****、怎么写入：**
大家都知道，Linux下万物皆文件，所以对于文件操作我们只需要：open()和write()就可以了。
**3****、要写入什么东西（键值的编码）：**
在/usr/include/linux/input.h中有定义，这个文件定义了event事件的结构体，API和标准按键的编码等；我们需要将要写入的按键编码填充到结构体中，然后写入键盘或者鼠标的事件设备驱动文件中。
**输入事件的结构体：**
struct input_event {
struct timeval time;  //按键时间
__u16 type;  //事件的类型
__u16 code;  //要模拟成什么按键
__s32 value;  //是按下1还是释放0
};
**标准按键的编码：**（只列举部分）
**type:**
事件的类型：
EV_KEY,按键事件，如键盘的按键（按下哪个键），鼠标的左键右键（是非击下）等；
EV_REL,相对坐标，主要是指鼠标的移动事件（相对位移）；
EV_ABS,绝对坐标，主要指触摸屏的移动事件，但好像这个不能用在鼠标上面，也就是说无法通过这个来获取鼠标的绝对坐标（鼠标是一个相对位移的设备）。
**code****：**
事件的代码：
如果事件的类型代码是EV_KEY，该代码code为设备键盘代码。代码植0~127为键盘上的按键代码，0x110~0x116为鼠标上按键代码，其中0x110(BTN_ LEFT)为鼠标左键，0x111(BTN_RIGHT)为鼠标右键，0x112(BTN_ MIDDLE)为鼠标中键。其它代码含义请参看include/linux /input.h文件。该文件中会定义相应的宏来代表不同的按键。
如果事件的类型代码是EV_REL，code值表示轨迹的类型。如指示鼠标的X轴方向REL_X（代码为0x00），指示鼠标的Y轴方向REL_Y，指示鼠标中轮子方向REL_WHEEL。
**value****：**
事件的值：
如果事件的类型代码是EV_KEY，当按键按下时值为1，松开时值为0；
如果事件的类型代码是EV_ REL，value的正数值和负数值分别代表两个不同方向的值。例如：如果code是REL_X，value是10的话，就表示鼠标相对于上一次的坐标，往x轴向右移动10个像素点。
**在****Linux****下写的简单的模拟鼠标和键盘事件的程序：**
\#include <stdio.h>
\#include <linux/input.h>
\#include <fcntl.h>
\#include <sys/time.h>
\#include <unistd.h>
//按键模拟，按键包含按下和松开两个环节
void simulate_key(int fd, int kval)
{
struct input_event event;
gettimeofday(&event.time, 0);
//按下kval键
event.type = EV_KEY;
event.value = 1;
event.code = kval;
write(fd, &event, sizeof(event));
//同步，也就是把它报告给系统
event.type = EV_SYN;
event.value = 0;
event.code = SYN_REPORT;
write(fd, &event, sizeof(event));
memset(&event, 0, sizeof(event));
gettimeofday(&event.time, 0);
//松开kval键
event.type = EV_KEY;
event.value = 0;
event.code = kval;
write(fd, &event, sizeof(event));
//同步，也就是把它报告给系统
event.type = EV_SYN;
event.value = 0;
event.code = SYN_REPORT;
write(fd, &event, sizeof(event));
}
//鼠标移动模拟
void simulate_mouse(int fd, int rel_x, int rel_y)
{
struct input_event event;
gettimeofday(&event.time, 0);
//x轴坐标的相对位移
event.type = EV_REL;
event.value = rel_x;
event.code = REL_X;
write(fd, &event, sizeof(event));
//y轴坐标的相对位移
event.type = EV_REL;
event.value = rel_y;
event.code = REL_Y;
write(fd, &event, sizeof(event));
//同步
event.type = EV_SYN;
event.value = 0;
event.code = SYN_REPORT;
write(fd, &event, sizeof(event));
}
int main(int argc, char **argv)
{
int fd_mouse = -1;
int fd_kbd = -1;
int i = 0;
fd_kbd = open("/dev/input/event3", O_RDWR);
if(fd_kbd <= 0)
{
printf("Can not open keyboard input file\n");
return -1;
}
fd_mouse = open("/dev/input/event2", O_RDWR);
if(fd_mouse <= 0)
{
printf("Can not open mouse input file\n");
return -1;
}
for (i = 0; i < 50; i++)
{
simulate_key(fd_mouse, BTN_LEFT);  //模拟按下鼠标左键
//if (i % 3 == 0)
//      simulate_key(fd_kbd, KEY_A);  //模拟按下键盘A键
//模拟鼠标相对上次x和y轴相应移动10个像素
//simulate_mouse(fd_mouse, 10, 10);
sleep(3);
}
close(fd_kbd);
close(fd_mouse);
}
那么如何模拟组合键呢？其实和大家平时按键盘的过程是一样的，我们用程序按照这个过程来模拟就可以了。以CTRL + SPACE为例：
//先发送一个CTRL按下去的事件
//再发送一个SPACE按下去的事件
//然后发送一个释放SPACE的事件
//再发送一个释放CTRL的事件
得注意每步的发送都需要同步一次。
**二、整合到交互系统（先以****TLD****为地基）中去**
这个因为不用涉及额外链接一些库，所以整合就变得很简单了。只需要修改run_tld.cpp:
1、添加run_tld.cpp中没有的，但模拟按键却需要的头文件：
\#include <linux/input.h>
2、把simulate_key()和simulate_mouse()两个函数的实现复制到run_tld.cpp的main函数的前面。然后在tld.init(last_gray, box, bb_file);后面添加：
//xiaoyi added here
int fd_kbd = -1;
int fd_mouse = -1;
int open_success = 1;
fd_kbd = open("/dev/input/event3", O_RDWR);
if(fd_kbd <= 0)
{
printf("Can not open keyboard input file\n");
open_success = 0;
}
fd_mouse = open("/dev/input/event2", O_RDWR);
if(fd_mouse <= 0)
{
printf("Can not open mouse input file\n");
open_success = 0;
}
3、在TLD跟踪到box后，获取本帧跟踪的box和上一帧box的位移，如果位移大于2个像素（避免抖动），鼠标就移动8倍像素距离。
pbox是当前帧跟踪到的目标box，tbox是我自己定义的，用来存放上一帧跟踪到的目标box的。
tld.processFrame(last_gray,current_gray,pts1,pts2,pbox,status,tl,bb_file);
//Draw Points
if (status) {
drawPoints(frame,pts1);
drawPoints(frame,pts2,Scalar(0,255,0));
drawBox(frame,pbox);
detections++;
//xiaoyi added here
x_pixel_move =(int)( (tbox.x + tbox.width)/2 - (pbox.x + pbox.width)/2);
y_pixel_move =(int)( (pbox.y + pbox.height)/2 - (tbox.y + tbox.height)/2);
if (norm(x_pixel_move) > 2 || norm(y_pixel_move) > 2 )
simulate_mouse(fd_mouse, 8 * x_pixel_move, 8 * y_pixel_move);
tbox = pbox;
//下面这部分是用来测试当手快速左右上下移动时，向系统发送左右上下的模拟按键事件
/*
if (open_success && (x_pixel_move > 8 || x_pixel_move < -8 || y_pixel_move > 8 || y_pixel_move < -8))
{
if (x_pixel_move < -8)
simulate_key(fd_kbd, KEY_RIGHT);
else if (x_pixel_move > 8)
simulate_key(fd_kbd, KEY_LEFT);
else if (y_pixel_move < -8)
simulate_key(fd_kbd, KEY_UP);
else if (y_pixel_move > 8)
simulate_key(fd_kbd, KEY_DOWN);
tbox = pbox;
x_pixel_move = 0;
y_pixel_move = 0;
}
*/
}
**三、存在的问题和解决思路：**
1、鼠标绝对坐标的获取：据我的了解，好像Linux或者c并不提供直接获取鼠标绝对坐标的API，而需要通过第三方的API来获取。另外，也许也不需要获取绝对坐标，所以暂时搁置；
2、手掌控制鼠标不稳定（漂移）与范围控制没处理好：可能通过卡尔曼滤波和速度映射等方法来做改进，后面再处理；
3、代码结构乱：一旦自己后面加入了很多模块，这样代码就会比较混乱，所以需要后期进行各模块代码的整理，已达到内聚性强点，而且代码容易管理。

