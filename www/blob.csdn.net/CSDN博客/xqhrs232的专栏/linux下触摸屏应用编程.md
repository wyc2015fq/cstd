# linux下触摸屏应用编程 - xqhrs232的专栏 - CSDN博客
2016年11月07日 17:14:13[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1987
原文地址::[http://blog.163.com/jk_new/blog/static/9954856220081112547867/](http://blog.163.com/jk_new/blog/static/9954856220081112547867/)
相关文章
1、linux-2.6.32在mini2440开发板上移植----添加触摸屏驱动程序----[http://www.linuxidc.com/Linux/2013-04/82383p9.htm](http://www.linuxidc.com/Linux/2013-04/82383p9.htm)
2、[Linux驱动学习笔记之触摸屏驱动](http://blog.chinaunix.net/uid-25737580-id-3143259.html) ----[http://blog.chinaunix.net/uid-25737580-id-3143259.html](http://blog.chinaunix.net/uid-25737580-id-3143259.html)
3、linux-2.6.32在mini2440开发板上移植----触摸屏工作原理以及驱动程序详细分析----[http://www.linuxidc.com/Linux/2013-04/82383p10.htm](http://www.linuxidc.com/Linux/2013-04/82383p10.htm)
触摸屏相关的文件定义在drivers/input/touchscreen/下，输入设备相关的文件主要有drivers/input/input.c input.h
 另外一个参考博客:http://blog.chinaunix.net/u2/66024/showart_674042.html
以下是转载部分linux下输入设备知识:
查看/dev/input/eventX是什么类型的事件， cat /proc/bus/input/devices
设备有着自己特殊的按键键码，我需要将一些标准的按键，比如0－9，X－Z等模拟成标准按键，比如KEY_0,KEY-Z等，所以需要用到按键模拟，具体方法就是操作/dev/input/event1文件，向它写入个input_event结构体就可以模拟按键的输入了。
linux/input.h中有定义，这个文件还定义了标准按键的编码等
struct input_event {
struct timeval time; //按键时间
__u16 type; //类型，在下面有定义
__u16 code; //要模拟成什么按键
__s32 value;//是按下还是释放
};
code：
事件的代码.如果事件的类型代码是EV_KEY,该代码code为设备键盘代码.代码植0~127为键盘上的按键代码,0x110~0x116 为鼠标上按键代码,其中0x110(BTN_ LEFT)为鼠标左键,0x111(BTN_RIGHT)为鼠标右键,0x112(BTN_ MIDDLE)为鼠标中键.其它代码含义请参看include/linux/input.h文件. 如果事件的类型代码是EV_REL,code值表示轨迹的类型.如指示鼠标的X轴方向REL_X(代码为0x00),指示鼠标的Y轴方向REL_Y(代码为0x01),指示鼠标中轮子方向REL_WHEEL(代码为0x08).
type: 
EV_KEY,键盘
EV_REL,鼠标
EV_ABS,绝对坐标
value：
事件的值.如果事件的类型代码是EV_KEY,当按键按下时值为1,松开时值为0;如果事件的类型代码是EV_ REL,value的正数值和负数值分别代表两个不同方向的值.
/*
* Event types
*/
#define EV_SYN 0x00
#define EV_KEY 0x01 //按键
#define EV_REL 0x02 //相对坐标(轨迹球)鼠标
#define EV_ABS 0x03 //绝对坐标
#define EV_MSC 0x04 //其他
#define EV_SW 0x05
#define EV_LED 0x11 //LED
#define EV_SND 0x12//声音
#define EV_REP 0x14//repeat
#define EV_FF 0x15 
#define EV_PWR 0x16
#define EV_FF_STATUS 0x17
#define EV_MAX 0x1f
#define EV_CNT (EV_MAX+1)
1。模拟按键输入
//其中0表示释放，1按键按下，2表示一直按下
//0 for EV_KEY for release, 1 for keypress and 2 for autorepeat.
void simulate_key(int fd,int value)
{
struct input_event event;
event.type = EV_KEY;
//event.code = KEY_0;//要模拟成什么按键
event.value = value;//是按下还是释放按键或者重复
gettimeofday(&event.time,0);
if(write(fd,&event,sizeof(event)) < 0){
dprintk("simulate key error~~~\n");
return ;
}
}
2。模拟鼠标输入（轨迹球）
void simulate_mouse(int fd,char buf[4])
{
int rel_x,rel_y;
static struct input_event event,ev;
//buf[0],buf[2],小于0则为左移，大于0则为右移
//buf[1],buf[3],小于0则为下移，大于0则为上移
dprintk("MOUSE TOUCH: x1=%d,y1=%d,x2=%d,y2=%d\n",buf[0],buf[1],buf[2],buf[3]);
rel_x = (buf[0] + buf[2]) /2;
rel_y = -(buf[1] + buf[3]) /2; //和我们的鼠标是相反的方向，所以取反
event.type = EV_REL;
event.code = REL_X;
event.value = rel_x;
gettimeofday(&event.time,0);
if( write(fd,&event,sizeof(event))!=sizeof(event))
dprintk("rel_x error~~~:%s\n",strerror(errno));
event.code = REL_Y;
event.value = rel_y;
gettimeofday(&event.time,0);
if( write(fd,&event,sizeof(event))!=sizeof(event))
dprintk("rel_y error~~~:%s\n",strerror(errno));
//一定要刷新空的
write(fd,&ev,sizeof(ev));
}
鼠标和键盘文件打开方法：
int fd_kbd; // /dev/input/event1
int fd_mouse; //dev/input/mouse2
fd_kbd = open("/dev/input/event1",O_RDWR);
if(fd_kbd<=0){
printf("error open keyboard:%s\n",strerror(errno));
return -1;
}
fd_mouse = open("/dev/input/event3",O_RDWR); //如果不行的话，那试试/dev/input/mice
if(fd_mouse<=0){
printf("error open mouse:%s\n",strerror(errno));
return -2;
}
}
/dev/input/mice是鼠标的抽象，代表的是鼠标，也许是/dev/input/mouse,/dev/input/mouse1，或者空，
这个文件一直会存在。
这里你也许会问，我怎么知道/dev/input/eventX这些事件到底是什么事件阿，是鼠标还是键盘或者别的，
eventX代表的是所有输入设备(input核心)的事件，比如按键按下，或者鼠标移动，或者游戏遥控器等等，
在系统查看的方法是 cat /proc/bus/input/devices 就可以看到每个eventX是什么设备的事件了。
PS: 在GTK中用的话，可以参考下gtk_main_do_event这个函数
static void simulate_key(GtkWidget *window,int keyval,int press)
{
GdkEvent *event;
GdkEventType type;
if(press) 
type = GDK_KEY_PRESS;
else 
type = GDK_KEY_RELEASE;
event = gdk_event_new(type);
//event->key.send_event = TRUE;
event->key.window = window->window; //一定要设置为主窗口
event->key.keyval = keyval;
//FIXME:一定要加上这个,要不然容易出错
g_object_ref(event->key.window);
gdk_threads_enter();
//FIXME: 记得用这个来发送事件
gtk_main_do_event(event);
gdk_threads_leave();
gdk_event_free(event);
}
