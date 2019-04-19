# video4linux简介 - maopig的专栏 - CSDN博客
2012年03月26日 23:32:15[maopig](https://me.csdn.net/maopig)阅读数：4519
Video4linux（简称V4L),是linux中关于视频设备的内核驱动,现在已有Video4linux2，还未加入linux内核，使用需自己下载补丁。在Linux中，视频设备是设备文件，可以像访问普通文件一样对其进行读写，摄像头在/dev/video0下。
2.Video4linux下视频编程的流程
（1）打开视频设备：
（2） 读取设备信息
（3）更改设备当前设置（没必要的话可以不做）
（4）进行视频采集，两种方法: 
        a.内存映射
        b.直接从设备读取
（5）对采集的视频进行处理
（6）关闭视频设备。
为程序定义的数据结构
typedef struct v4l_struct 
{
   int fd;
   struct video_capability capability;
   struct video_channel channel[4]; 
   struct video_picture picture; 
   struct video_window window; 
   struct video_capture capture; 
   struct video_buffer buffer; 
   struct video_mmap mmap; 
   struct video_mbuf mbuf;    
   unsigned char *map;
   int frame;
   int framestat[2]; 
}vd;
3.Video4linux支持的数据结构及其用途
（1） video_capability 包含设备的基本信息（设备名称、支持的最大最小分辨率、信号源信息等）,包含的分量：
•name[32]   //设备名称
•maxwidth ,maxheight,minwidth,minheight
•Channels //信号源个数
•type    //是否能capture，彩色还是黑白，是否能裁剪等等。值如VID_TYPE_CAPTURE等
•
（2）video_picture 设备采集的图象的各种属性
•brightness 0~65535
•hue 
•colour 
•contrast 
•whiteness 
•depth // 24
•palette //VIDEO_PALETTE_RGB24
（3）video_channel         关于各个信号源的属性
    Channel //信号源的编号
    name
    tuners
    Type     VIDEO_TYPE_TV | IDEO_TYPE_CAMERA
    Norm制式
（4）video_window //包含关于capture area的信息
    xx windows 中的坐标.
    y     x windows 中的坐标.
    width    The width of the image capture.
    height   The height of the image capture.
    chromakey A host order RGB32 value for the chroma key.
    flags      Additional capture flags.
    clips      A list of clipping rectangles. (Set only)
    clipcount    The number of clipping rectangles. (Set only)
（5）video_mbuf   //利用mmap进行映射的帧的信息
      size //每帧大小
      Frames //最多支持的帧数
      Offsets //每帧相对基址的偏移
（6）video_buffer   最底层对buffer的描述
      void *baseBase physical address of the buffer
      int heightHeight of the frame buffer
      int widthWidth of the frame buffer
      int depthDepth of the frame buffer
      int bytesperlineNumber of bytes of memory between the start of two adjacent lines
   实际显示的部分一般比它描述的部分小
（7）video_mmap //用于mmap
4.关键步骤介绍
（1）打开视频：
Open（”/dev/video0”，vdàfd);
关闭视频设备用close（”/dev/video0”，vdàfd);
（2）读video_capability 中信息
ioctl(vd->fd, VIDIOCGCAP, &(vd->capability))
成功后可读取vd->capability各分量 eg.
（3）读video_picture中信息
ioctl(vd->fd, VIDIOCGPICT, &(vd->picture))；
（4）改变video_picture中分量的值 （可以不做的）
先为分量赋新值，再调用VIDIOCSPICT
Eg. 
•vd->picture.colour = 65535;
•if(ioctl(vd->fd, VIDIOCSPICT, &(vd->picture)) < 0) 
•{
•perror("VIDIOCSPICT");
•return -1;
•}
（5）初始化channel （可以不做的）
•必须先做得到vd->capability中的信息
•for (i = 0; i < vd->capability.channels; i++) 
•   {
•      vd->channel[i].channel = i;
•      if (ioctl(vd->fd, VIDIOCGCHAN, &(vd->channel[i])) < 0)
• {
•         perror("v4l_get_channel:");
•         return -1;
•         }
•   }
重点：截取图象的两种方法
1,用mmap（内存映射）方式截取视频
•mmap( )系统调用使得进程之间通过映射同一个普通文件实现共享内存。普通文件被映射到进程地址空间后，进程可以向访问普通内存一样对文件进行访问，不必再调用read()，write（）等操作。
•两个不同进程A、B共享内存的意思是，同一块物理内存被映射到进程A、B各自的进程地址空间。进程A可以即时看到进程B对共享内存中数据的更新，反之亦然
•采用共享内存通信的一个显而易见的好处是效率高，因为进程可以直接读写内存，而不需要任何数据的拷贝
（1）设置picture的属性
（2） 初始化video_mbuf，以得到所映射的buffer的信息
ioctl(vd->fd, VIDIOCGMBUF, &(vd->mbuf)) 
（3）可以修改video_mmap和帧状态的当前设置
•    Eg.     vd->mmap.format = VIDEO_PALETTE_RGB24
•              vd->framestat[0] = vd->framestat[1] = 0; vd->frame = 0;
（4）将mmap与video_mbuf绑定
•void* mmap ( void * addr , size_t len , int prot , int flags , int fd , off_t offset )
•len //映射到调用进程地址空间的字节数，它从被映射文件开头offset个字节开始算起
•Prot //指定共享内存的访问权限 PROT_READ（可读）, PROT_WRITE （可写）, PROT_EXEC （可执行）
•flags // MAP_SHARED   MAP_PRIVATE中必选一个 // MAP_ FIXED不推荐使用addr //共内存享的起始地址，一般设0，表示由系统分配
•Mmap( ) 返回值是系统实际分配的起始地址
•if((vd->map = (unsigned char*)mmap(0, vd->mbuf.size, PROT_READ|PROT_WRITE, MAP_SHARED, vd->fd, 0)) < 0)
•{
•perror("v4l_mmap mmap:");
•return -1;
•}
（5）Mmap方式下真正做视频截取的 VIDIOCMCAPTURE
ioctl(vd->fd, VIDIOCMCAPTURE, &(vd->mmap)) ;
•若调用成功，开始一帧的截取，是非阻塞的，
•是否截取完毕留给VIDIOCSYNC来判断
（6）调用VIDIOCSYNC等待一帧截取结束
•if(ioctl(vd->fd, VIDIOCSYNC, &frame) < 0)
•{
•perror("v4l_sync:VIDIOCSYNC");
•return -1;
•}
若成功，表明一帧截取已完成。可以开始做下一次 VIDIOCMCAPTURE
•frame是当前截取的帧的序号。
****关于双缓冲：
•video_bmuf bmuf.frames = 2;
•一帧被处理时可以采集另一帧
•int frame; //当前采集的是哪一帧
•int framestat[2]; //帧的状态 没开始采集|等待采集结束
•帧的地址由vd->map + vd->mbuf.offsets[vd->frame]得到
•采集工作结束后调用munmap取消绑定
•munmap(vd->map, vd->mbuf.size)
2,视频截取的第二种方法：直接读设备
关于缓冲大小，图象等的属性须由使用者事先设置
•调用read（）；
•int read (要访问的文件描述符；指向要读写的信息的指针；应该读写的字符数);
•返回值为实际读写的字符数
•int len ;
•unsigned char *vd->map= (unsigned char *) malloc(vdàcapability.maxwidth*vdàcapability.maxheight );
•len = read(vdàfd,vdà vd->map,
•                   vdàcapability.maxwidth*vdàcapability.maxheight*3 );
一.什么是video4linux
Video4linux2（简称V4L2),是linux中关于视频设备的内核驱动。在Linux中，视频设备是设备文件，可以像访问普通文件一样对其进行读写，摄像头在/dev/video0下。
二、一般操作流程（视频设备）：
1. 打开设备文件。 int fd=open(”/dev/video0″,O_RDWR);
2. 取得设备的capability，看看设备具有什么功能，比如是否具有视频输入,或者音频输入输出等。VIDIOC_QUERYCAP,struct v4l2_capability
3. 选择视频输入，一个视频设备可以有多个视频输入。VIDIOC_S_INPUT,struct v4l2_input
4. 设置视频的制式和帧格式，制式包括PAL，NTSC，帧的格式个包括宽度和高度等。
VIDIOC_S_STD,VIDIOC_S_FMT,struct v4l2_std_id,struct v4l2_format
5. 向驱动申请帧缓冲，一般不超过5个。struct v4l2_requestbuffers
6. 将申请到的帧缓冲映射到用户空间，这样就可以直接操作采集到的帧了，而不必去复制。mmap
7. 将申请到的帧缓冲全部入队列，以便存放采集到的数据.VIDIOC_QBUF,struct v4l2_buffer
8. 开始视频的采集。VIDIOC_STREAMON
9. 出队列以取得已采集数据的帧缓冲，取得原始采集数据。VIDIOC_DQBUF
10. 将缓冲重新入队列尾,这样可以循环采集。VIDIOC_QBUF
11. 停止视频的采集。VIDIOC_STREAMOFF
12. 关闭视频设备。close(fd);
三、常用的结构体(参见/usr/include/linux/videodev2.h)：
struct v4l2_requestbuffers reqbufs;//向驱动申请帧缓冲的请求，里面包含申请的个数
struct v4l2_capability cap;//这个设备的功能，比如是否是视频输入设备
struct v4l2_input input; //视频输入
struct v4l2_standard std;//视频的制式，比如PAL，NTSC
struct v4l2_format fmt;//帧的格式，比如宽度，高度等
struct v4l2_buffer buf;//代表驱动中的一帧
v4l2_std_id stdid;//视频制式，例如：V4L2_STD_PAL_B
struct v4l2_queryctrl query;//查询的控制
struct v4l2_control control;//具体控制的值
下面具体说明开发流程（网上找的啦，也在学习么）
**打开视频设备**
在V4L2中，视频设备被看做一个文件。使用open函数打开这个设备：
//用非阻塞模式打开摄像头设备
intcameraFd;
cameraFd=
open(“/dev/video0″,O_RDWR
| O_NONBLOCK, 0);
//如果用阻塞模式打开摄像头设备，上述代码变为：
//cameraFd = open(”/dev/video0″, O_RDWR, 0);
**关于阻塞模式和非阻塞模式**
应用程序能够使用阻塞模式或非阻塞模式打开视频设备，如果使用非阻塞模式调用视频设备，即使尚未捕获到信息，驱动依旧会把缓存（DQBUFF）里的东西返回给应用程序。
**设定属性及采集方式**
打开视频设备后，可以设置该视频设备的属性，例如裁剪、缩放等。这一步是可选的。在Linux编程中，一般使用ioctl函数来对设备的I/O通道进行管理：
extern intioctl
(int __fd,unsigned long int
__request, …)__THROW;
__fd：设备的ID，例如刚才用open函数打开视频通道后返回的cameraFd；
__request：具体的命令标志符。
在进行V4L2开发中，一般会用到以下的命令标志符：
- VIDIOC_REQBUFS：分配内存
- VIDIOC_QUERYBUF：把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址
- VIDIOC_QUERYCAP：查询驱动功能
- VIDIOC_ENUM_FMT：获取当前驱动支持的视频格式
- VIDIOC_S_FMT：设置当前驱动的频捕获格式
- VIDIOC_G_FMT：读取当前驱动的频捕获格式
- VIDIOC_TRY_FMT：验证当前驱动的显示格式
- VIDIOC_CROPCAP：查询驱动的修剪能力
- VIDIOC_S_CROP：设置视频信号的边框
- VIDIOC_G_CROP：读取视频信号的边框
- VIDIOC_QBUF：把数据从缓存中读取出来
- VIDIOC_DQBUF：把数据放回缓存队列
- VIDIOC_STREAMON：开始视频显示函数
- VIDIOC_STREAMOFF：结束视频显示函数
- VIDIOC_QUERYSTD：检查当前视频设备支持的标准，例如PAL或NTSC。
这些IO调用，有些是必须的，有些是可选择的。
**检查当前视频设备支持的标准**
在亚洲，一般使用PAL（720X576）制式的摄像头，而欧洲一般使用NTSC（720X480），使用VIDIOC_QUERYSTD来检测：
v4l2_std_id std;
do{
ret=
ioctl(fd,VIDIOC_QUERYSTD, &std);
} 
while (ret == -1 && 
errno == EAGAIN);
switch(std) {
caseV4L2_STD_NTSC:
//……
caseV4L2_STD_PAL:
//……
}
**设置视频捕获格式**
当检测完视频设备支持的标准后，还需要设定视频捕获格式：
structv4l2_format    fmt;
memset( &fmt, 0,
sizeof(fmt) );
fmt.type=
V4L2_BUF_TYPE_VIDEO_CAPTURE;
fmt.fmt.pix.width=
720;
fmt.fmt.pix.height=
576;
fmt.fmt.pix.pixelformat=
V4L2_PIX_FMT_YUYV;
fmt.fmt.pix.field=
V4L2_FIELD_INTERLACED;
if(ioctl(fd,VIDIOC_S_FMT, &fmt)
 == -1) {
return-1;
}
v4l2_format结构体定义如下：
structv4l2_format
{
enumv4l2_buf_type type;   
// 数据流类型，必须永远是//V4L2_BUF_TYPE_VIDEO_CAPTURE
union
{
structv4l2_pix_format    pix;
structv4l2_window        win;
structv4l2_vbi_format    vbi;
__u8    raw_data[200];
} 
fmt;
};
structv4l2_pix_format
{
__u32                   width;        //
宽，必须是16的倍数
__u32                    height;       //
高，必须是16的倍数
__u32                   pixelformat;  //
视频数据存储类型，例如是//YUV4：2：2还是RGB
enumv4l2_field         field;
__u32                   bytesperline;
__u32                   sizeimage;
enumv4l2_colorspace    colorspace;
__u32                   priv;
};
**前言：**目前正在忙于ARM平台的Linux应用程序的开发（其实是刚刚起步学习啦）。底层的东西不用考虑了，开发板子提供了NAND Bootloader，和Linux 2.6的源码，而且都编译好了。自己编译的bootloader可以用，但是Linux编译后，文件很大，暂且就用人家编译的系统，先专心写应用程序
 吧。。
**正文：**要做的任务是，把一块板子上的摄像头采集的图像和声卡采集的声音（貌似很啰嗦哈）通过TCP/IP协议传输到另一块板子上。第一步，先把视频获取并且在本地LCD上显示。看了板子提供的文档，视频传输需要用V4L2的API。
**一.什么是****video4linux**
Video4linux2（简称V4L2),是linux中关于视频设备的内核驱动。在Linux中，视频设备是设备文件，可以像访问普通文件一样对其进行读写，摄像头在/dev/video0下。
**二、一般操作流程（视频设备）：**
**1. **打开设备文件。 int fd=open(”/dev/video0″,O_RDWR);
**2. **取得设备的capability，看看设备具有什么功能，比如是否具有视频输入,或者音频输入输出等。VIDIOC_QUERYCAP,struct v4l2_capability
**3. **选择视频输入，一个视频设备可以有多个视频输入。VIDIOC_S_INPUT,struct v4l2_input
**4. **设置视频的制式和帧格式，制式包括PAL，NTSC，帧的格式个包括宽度和高度等。
VIDIOC_S_STD,VIDIOC_S_FMT,struct v4l2_std_id,struct v4l2_format
**5. **向驱动申请帧缓冲，一般不超过5个。struct v4l2_requestbuffers
**6. **将申请到的帧缓冲映射到用户空间，这样就可以直接操作采集到的帧了，而不必去复制。mmap
**7. **将申请到的帧缓冲全部入队列，以便存放采集到的数据.VIDIOC_QBUF,struct v4l2_buffer
**8. **开始视频的采集。VIDIOC_STREAMON
**9. **出队列以取得已采集数据的帧缓冲，取得原始采集数据。VIDIOC_DQBUF
**10.**将缓冲重新入队列尾,这样可以循环采集。VIDIOC_QBUF
**11. **停止视频的采集。VIDIOC_STREAMOFF
**12. **关闭视频设备。close(fd);
三、常用的结构体(参见/usr/include/linux/videodev2.h)：
struct **v4l2_requestbuffers** reqbufs;  //**向驱动申请帧缓冲的请求，里面包含申请的个数**
struct v4l2_capability cap;                 //这个设备的功能，比如是否是视频输入设备
struct v4l2_input input;                      //视频输入
struct v4l2_standard std;                   //视频的制式，比如PAL，NTSC
struct v4l2_format fmt;                      //帧的格式，比如宽度，高度等
struct v4l2_buffer buf;                       //代表驱动中的一帧
v4l2_std_id stdid;                               //视频制式，例如：V4L2_STD_PAL_B
struct v4l2_queryctrl query;            //查询的控制
struct v4l2_control control;             //具体控制的值
下面具体说明开发流程（网上找的啦，也在学习么）
**打开视频设备**
在V4L2中，视频设备被看做一个文件。使用open函数打开这个设备：
//
用非阻塞模式打开摄像头设备
int
cameraFd;
cameraFd
= open(“/dev/video0″,
O_RDWR | O_NONBLOCK, 0);
//
如果用阻塞模式打开摄像头设备，上述代码变为：
//cameraFd = open(”/dev/video0″, O_RDWR, 0);
**关于阻塞模式和非阻塞模式**
应用程序能够使用阻塞模式或非阻塞模式打开视频设备，如果使用非阻塞模式调用视频设备，即使尚未捕获到信息，驱动依旧会把缓存（DQBUFF）里的东西返回给应用程序。
**设定属性及采集方式**
打开视频设备后，可以设置该视频设备的属性，例如裁剪、缩放等。这一步是可选的。在Linux编程中，一般使用ioctl函数来对设备的I/O通道进行管理：
extern int
ioctl (int __fd,
unsigned long int __request, …)
__THROW;
__fd：设备的ID，例如刚才用open函数打开视频通道后返回的cameraFd；
__request：具体的命令标志符。
在进行V4L2开发中，一般会用到以下的命令标志符：
- **VIDIOC_REQBUFS**：**分配内存**
- **VIDIOC_QUERYBUF**：**把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址**
- **VIDIOC_QUERYCAP**：**查询驱动功能**
- **VIDIOC_ENUM_FMT**：**获取当前驱动支持的视频格式**
- **VIDIOC_S_FMT**：**设置当前驱动的频捕获格式**
- **VIDIOC_G_FMT**：**读取当前驱动的频捕获格式**
- **VIDIOC_TRY_FMT**：**验证当前驱动的显示格式**
- **VIDIOC_CROPCAP**：**查询驱动的修剪能力**
- **VIDIOC_S_CROP**：**设置视频信号的边框**
- **VIDIOC_G_CROP**：**读取视频信号的边框**
- **VIDIOC_QBUF**：**把数据从缓存中读取出来**
- **VIDIOC_DQBUF**：**把数据放回缓存队列**
- **VIDIOC_STREAMON**：**开始视频显示函数**
- **VIDIOC_STREAMOFF**：**结束视频显示函数**
- **VIDIOC_QUERYSTD**：**检查当前视频设备支持的标准，例如PAL或NTSC**。
这些IO调用，有些是必须的，有些是可选择的。
**检查当前视频设备支持的标准**
**在亚洲，一般使用PAL（720X576）制式的摄像头，而欧洲一般使用NTSC（720X480）**，**使用VIDIOC_QUERYSTD来检测**：
v4l2_std_id std;
do
{
ret
= ioctl(fd,
VIDIOC_QUERYSTD, &std);
} 
while (ret == -1 && 
errno == EAGAIN);
switch
(std) {
case
V4L2_STD_NTSC:
//……
case
V4L2_STD_PAL:
//……
}
**设置视频捕获格式**
当检测完视频设备支持的标准后，还需要设定视频捕获格式：
struct
**v4l2_format  **  fmt;
memset
( &fmt, 0, 
sizeof(fmt) );
fmt.type
= V4L2_BUF_TYPE_VIDEO_CAPTURE;
fmt.fmt.pix.width
= 720;
fmt.fmt.pix.height
= 576;
fmt.fmt.pix.pixelformat
= **V4L2_PIX_FMT_YUYV;**
fmt.fmt.pix.field
= V4L2_FIELD_INTERLACED;
if
(**ioctl(fd,VIDIOC_S_FMT, &fmt)** == -1) {
return
-1;
}
v4l2_format结构体定义如下：
struct
**v4l2_format**
{
enum
v4l2_buf_type type;    
// 数据流类型，必须永远是//V4L2_BUF_TYPE_VIDEO_CAPTURE 
union
{
struct
**v4l2_pix_format**    pix;
struct
v4l2_window        win; 
struct
v4l2_vbi_format    vbi; 
__u8    raw_data[200];
} 
fmt;
};
struct
**v4l2_pix_format**
{
__u32                   width;        
// 宽，必须是16的倍数
__u32                    height;       
// 高，必须是16的倍数
__u32                   pixelformat;  
// 视频数据存储类型，例如是//**YUV4****：2：2还是RGB**
enum
v4l2_field         field;
__u32                   bytesperline;
__u32                   sizeimage;
enum
v4l2_colorspace    colorspace;
__u32                   priv;
};
**分配内存**
接下来可以为视频捕获分配内存：
struct **v4l2_requestbuffers** req;
if (**ioctl(fd, VIDIOC_REQBUFS, &req)** == -1) {
return -1;
}
v4l2_requestbuffers定义如下：
struct **v4l2_requestbuffers**
{
__u32               count; // **缓存数量，也就是说在缓存队列里保持多少张照片**
enum v4l2_buf_type type;   // 数据流类型，必须永远是V4L2_BUF_TYPE_VIDEO_CAPTURE
enum v4l2_memory    memory; // V4L2_MEMORY_MMAP 或 V4L2_MEMORY_USERPTR
__u32               reserved[2];
};
**获取并记录缓存的物理空间**
**使用VIDIOC_REQBUFS，我们获取了req.count个缓存，下一步通过调用VIDIOC_QUERYBUF命令来获取这些缓存的地址**，然后使用mmap函数转换成应用程序中的绝对地址，最后把这段缓存放入缓存队列：
typedef struct VideoBuffer {
void *start;
size_t length;
} VideoBuffer;
VideoBuffer*          buffers = calloc( req.count, sizeof(*buffers) );
//**这里的 buffers可以理解为一个结构体数组，用来存放视频帧，一共****req.count个**
struct **v4l2_buffer **** buf**;
//**这里的buf，只有一个，它相当于内核与用户空间传递数据的一个中介，会被循环利用：取出、放入、取出、放入……**
**for **(numBufs = 0; **numBufs** <
**req.count**; numBufs++) {
memset( &buf, 0, sizeof(buf) );
buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory = V4L2_MEMORY_MMAP;
buf.index = **numBufs**;  //**这里要标记索引**
// 读取缓存，**buf取出（循环利用）**
if (**ioctl(fd, VIDIOC_QUERYBUF, &buf)** == -1) {
return -1;
}
buffers[numBufs].length =
buf.length;
// 转换成相对地址
**buffers[numBufs].start**= **mmap**(NULL,** buf.length**,
PROT_READ | PROT_WRITE,
MAP_SHARED,
**fd,**
**buf.m.offset**);
if (buffers[numBufs].start == MAP_FAILED) {
return -1;
}
// 放入缓存队列，**buf放入****（循环利用）**
if **(ioctl(fd, VIDIOC_QBUF, &buf) **
== -1) {
return -1;
}
}
关于视频采集方式
操作系统一般把系统使用的内存划分成用户空间和内核空间，分别由应用程序管理和操作系统管理。应用程序可以直接访问内存的地址，而内核空间存放的是 供内核访问的代码和数据，用户不能直接访问。v4l2捕获的数据，最初是存放在内核空间的，这意味着用户不能直接访问该段内存，必须通过某些手段来转换地 址。
一共有三种视频采集方式：使用read、write方式；内存映射方式和用户指针模式。
read、write方式:在用户空间和内核空间不断拷贝数据，占用了大量用户内存空间，效率不高。
内存映射方式：把设备里的内存映射到应用程序中的内存控件，直接处理设备内存，这是一种有效的方式。上面的mmap函数就是使用这种方式。
用户指针模式：内存片段由应用程序自己分配。这点需要在v4l2_requestbuffers里将memory字段设置成V4L2_MEMORY_USERPTR。
处理采集数据
**V4L2有一个数据缓存，存放req.count数量的缓存数据**。**数据缓存采用FIFO的方式，当应用程序调用缓存数据时，缓存队列将最先采集到的 视频数据缓存送出，并重新采集一张视频数据。这个过程需要用到两个ioctl命令,VIDIOC_DQBUF和VIDIOC_QBUF：**
struct v4l2_buffer buf;
memset(&buf,0,sizeof(buf));
buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory=V4L2_MEMORY_MMAP;
buf.index=0;
//读取缓存
if (ioctl(cameraFd, VIDIOC_DQBUF, &buf) == -1)
{
return -1;
}
//…………视频处理算法
//重新放入缓存队列
if (ioctl(cameraFd, VIDIOC_QBUF, &buf) == -1) {
return -1;
}
关闭视频设备
使用close函数关闭一个视频设备
close(cameraFd)
还需要使用munmap方法。
附录：标准的V4l2的API
[http://v4l.videotechnology.com/dwg/v4l2.pdf](http://v4l.videotechnology.com/dwg/v4l2.pdf)
