
# V4L2用户空间和kernel层driver的交互过程 - 嵌入式Linux - CSDN博客

2015年05月15日 15:16:23[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：813


这篇文章详细分析了V4L2用户空间和kernel层driver的交互过程，目的只有一个：
更清晰的理解V4L2视频驱动程序的系统结构，驱动编程方法，为以后开发视频驱动打好基础
![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/30.gif)
既然从用户层出发探究驱动层，这里先贴出应用层code：
\#include<stdio.h>
\#include<stdlib.h>
\#include<string.h>
\#include<assert.h>
\#include<getopt.h>
\#include<fcntl.h>
\#include<unistd.h>
\#include<errno.h>
\#include<sys/stat.h>
\#include<sys/types.h>
\#include<sys/time.h>
\#include<sys/mman.h>
\#include<sys/ioctl.h>
\#include<asm/types.h>
\#include<linux/videodev2.h>
\#include<linux/fb.h>
\#define CLEAR(x)memset(&(x),0,sizeof(x))

struct buffer{
void*start;
size_t length;
};

static char*dev_name=NULL;
staticintfd=-1;
struct buffer*buffers=NULL;
static unsignedintn_buffers=0;
staticinttime_in_sec_capture=5;
staticintfbfd=-1;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static char*fbp=NULL;
static long screensize=0;

static void errno_exit(constchar*s)
{
fprintf(stderr,"%s error %d, %s/n",s,errno,strerror(errno));
exit(EXIT_FAILURE);
}

staticintxioctl(intfd,intrequest,void*arg)
{
intr;
/*Here use this methodtomake sure cmd success*/
dor=ioctl(fd,request,arg);
while(-1==r&&EINTR==errno);
return r;
}

inlineintclip(intvalue,intmin,intmax){
return(value>max?max:value<min?min:value);
}

static void process_image(constvoid*p){
//ConvertYUVToRGB321;
unsigned char*in=(char*)p;
intwidth=640;
intheight=480;
intistride=1280;
intx,y,j;
inty0,u,y1,v,r,g,b;
longlocation=0;

for(y=100;y<height+100;++y){
for(j=0,x=100;j<width*2;j+=4,x+=2){
location=(x+vinfo.xoffset)*(vinfo.bits_per_pixel/8)+
(y+vinfo.yoffset)*finfo.line_length;

y0=in[j];
u=in[j+1]-128;
y1=in[j+2];
v=in[j+3]-128;

r=(298*y0+409*v+128)>>8;
g=(298*y0-100*u-208*v+128)>>8;
b=(298*y0+516*u+128)>>8;

fbp[location+0]=clip(b,0,255);
fbp[location+1]=clip(g,0,255);
fbp[location+2]=clip(r,0,255);
fbp[location+3]=255;

r=(298*y1+409*v+128)>>8;
g=(298*y1-100*u-208*v+128)>>8;
b=(298*y1+516*u+128)>>8;

fbp[location+4]=clip(b,0,255);
fbp[location+5]=clip(g,0,255);
fbp[location+6]=clip(r,0,255);
fbp[location+7]=255;
}
in+=istride;
}
}

staticintread_frame(void)
{
struct v4l2_buffer buf;
unsignedinti;

CLEAR(buf);
buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory=V4L2_MEMORY_MMAP;
/*11.VIDIOC_DQBUF把数据放回缓存队列*/
if(-1==xioctl(fd,VIDIOC_DQBUF,&buf)){
switch(errno){
caseEAGAIN:
return 0;
caseEIO:
default:
errno_exit("VIDIOC_DQBUF");
}
}

assert(buf.index<n_buffers);
printf("v4l2_pix_format->field(%d)/n",buf.field);
//assert(buf.field==V4L2_FIELD_NONE);
process_image(buffers[buf.index].start);

/*12.VIDIOC_QBUF把数据从缓存中读取出来*/
if(-1==xioctl(fd,VIDIOC_QBUF,&buf))
errno_exit("VIDIOC_QBUF");

return 1;
}

static void run(void)
{
unsignedintcount;
intframes;
frames=30*time_in_sec_capture;

while(frames-->0){
for(;;){
fd_set fds;
struct timeval tv;
intr;
FD_ZERO(&fds);
FD_SET(fd,&fds);


tv.tv_sec=2;
tv.tv_usec=0;
/*10.poll
 method*/
r=select(fd+1,&fds,NULL,NULL,&tv);

if(-1==r){
if(EINTR==errno)
continue;
errno_exit("select");
}

if(0==r){
fprintf(stderr,"select timeout/n");
exit(EXIT_FAILURE);
}

if(read_frame())
break;

}
}
}

static void stop_capturing(void)
{
enum v4l2_buf_type type;

type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
/*13.VIDIOC_STREAMOFF结束视频显示函数*/
if(-1==xioctl(fd,VIDIOC_STREAMOFF,&type))
errno_exit("VIDIOC_STREAMOFF");
}

static void start_capturing(void)
{
unsignedinti;
enum v4l2_buf_type type;

for(i=0;i<n_buffers;++i){
struct v4l2_buffer buf;
CLEAR(buf);

buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory=V4L2_MEMORY_MMAP;
buf.index=i;
/*8.VIDIOC_QBUF把数据从缓存中读取出来*/
if(-1==xioctl(fd,VIDIOC_QBUF,&buf))
errno_exit("VIDIOC_QBUF");
}

type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
/*9.VIDIOC_STREAMON开始视频显示函数*/
if(-1==xioctl(fd,VIDIOC_STREAMON,&type))
errno_exit("VIDIOC_STREAMON");

}

static void uninit_device(void)
{
unsignedinti;

for(i=0;i<n_buffers;++i)
if(-1==munmap(buffers[i].start,buffers[i].length))
errno_exit("munmap");

if(-1==munmap(fbp,screensize)){
printf(" Error: framebuffer device munmap() failed./n");
exit(EXIT_FAILURE);
}
free(buffers);
}


static void init_mmap(void)
{
struct v4l2_requestbuffers req;

//mmap framebuffer
fbp=(char*)mmap(NULL,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0);
if((int)fbp==-1){
printf("Error: failed to map framebuffer device to memory./n");
exit(EXIT_FAILURE);
}
memset(fbp,0,screensize);
CLEAR(req);

req.count=4;
req.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
req.memory=V4L2_MEMORY_MMAP;
/*6.VIDIOC_REQBUFS分配内存*/
if(-1==xioctl(fd,VIDIOC_REQBUFS,&req)){
if(EINVAL==errno){
fprintf(stderr,"%s does not support memory mapping/n",dev_name);
exit(EXIT_FAILURE);
}else{
errno_exit("VIDIOC_REQBUFS");
}
}

if(req.count<4){
fprintf(stderr,"Insufficient buffer memory on %s/n",dev_name);
exit(EXIT_FAILURE);
}

buffers=calloc(req.count,sizeof(*buffers));

if(!buffers){
fprintf(stderr,"Out of memory/n");
exit(EXIT_FAILURE);
}

for(n_buffers=0;n_buffers<req.count;++n_buffers){
struct v4l2_buffer buf;

CLEAR(buf);

buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
buf.memory=V4L2_MEMORY_MMAP;
buf.index=n_buffers;
/*7.VIDIOC_QUERYBUF把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址*/
if(-1==xioctl(fd,VIDIOC_QUERYBUF,&buf))
errno_exit("VIDIOC_QUERYBUF");

buffers[n_buffers].length=buf.length;
buffers[n_buffers].start=mmap(NULL,buf.length,PROT_READ|PROT_WRITE,MAP_SHARED,fd,buf.m.offset);

if(MAP_FAILED==buffers[n_buffers].start)
errno_exit("mmap");
}

}



static void init_device(void)
{
struct v4l2_capability cap;
struct v4l2_cropcap cropcap;
struct v4l2_crop crop;
struct v4l2_format fmt;
unsignedintmin;


//Getfixed screen information
if(-1==xioctl(fbfd,FBIOGET_FSCREENINFO,&finfo)){
printf("Error reading fixed information./n");
exit(EXIT_FAILURE);
}

//Getvariable screen information
if(-1==xioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo)){
printf("Error reading variable information./n");
exit(EXIT_FAILURE);
}
screensize=vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;

/*2.VIDIOC_QUERYCAP查询驱动功能*/
if(-1==xioctl(fd,VIDIOC_QUERYCAP,&cap)){
if(EINVAL==errno){
fprintf(stderr,"%s is no V4L2 device/n",dev_name);
exit(EXIT_FAILURE);
}else{
errno_exit("VIDIOC_QUERYCAP");
}
}

/*Checkifitisa video capture device*/
if(!(cap.capabilities&V4L2_CAP_VIDEO_CAPTURE)){
fprintf(stderr,"%s is no video capture device/n",dev_name);
exit(EXIT_FAILURE);
}

/*Checkifsupport streaming I/O ioctls*/
if(!(cap.capabilities&V4L2_CAP_STREAMING)){
fprintf(stderr,"%s does not support streaming i/o/n",dev_name);
exit(EXIT_FAILURE);
}

CLEAR(cropcap);
/*Settype*/
cropcap.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
/*3.VIDIOC_CROPCAP查询驱动的修剪能力*/
/*这里在vivi驱动中我们没有实现此方法，即不支持此操作*/
if(0==xioctl(fd,VIDIOC_CROPCAP,&cropcap)){
crop.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
crop.c=cropcap.defrect;
/*4.VIDIOC_S_CROP设置视频信号的边框*/
/*同样不支持这个操作*/
if(-1==xioctl(fd,VIDIOC_S_CROP,&crop)){
switch(errno){
caseEINVAL:
break;
default:
break;
}
}
}else{}

CLEAR(fmt);

fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
fmt.fmt.pix.width=640;
fmt.fmt.pix.height=480;
fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUYV;
fmt.fmt.pix.field=V4L2_FIELD_INTERLACED;
/*5.VIDIOC_S_FMT设置当前驱动的频捕获格式*/
if(-1==xioctl(fd,VIDIOC_S_FMT,&fmt))
errno_exit("VIDIOC_S_FMT");

init_mmap();

}

static void close_device(void)
{
if(-1==close(fd))
errno_exit("close");
fd=-1;
/*14.close
 method*/
close(fbfd);
}

static void open_device(void)
{
struct stat st;

if(-1==stat(dev_name,&st)){
fprintf(stderr,"Cannot identify '%s': %d, %s/n",dev_name,errno,strerror(errno));
exit(EXIT_FAILURE);
}

if(!S_ISCHR(st.st_mode)){
fprintf(stderr,"%s is no device/n",dev_name);
exit(EXIT_FAILURE);
}

fbfd=open("/dev/fb0",O_RDWR);
if(fbfd==-1){
printf("Error: cannot open framebuffer device./n");
exit(EXIT_FAILURE);
}

/*1.open
 the char device*/
fd=open(dev_name,O_RDWR|O_NONBLOCK,0);
if(-1==fd){
fprintf(stderr,"Cannot open '%s': %d, %s/n",dev_name,errno,strerror(errno));
exit(EXIT_FAILURE);
}
}

static void usage(FILE*fp,intargc,char**argv)
{
fprintf(fp,
"Usage: %s [options]/n/n"
"Options:/n"
"-d | --device name Video device name [/dev/video]/n"
"-h | --help Print this message/n"
"-t | --how long will display in seconds/n"
"",
argv[0]);
}

staticconstchar short_options[]="d:ht:";
staticconststructoptionlong_options[]={
{"device",required_argument,NULL,'d'},
{"help",no_argument,NULL,'h'},
{"time",no_argument,NULL,'t'},
{0,0,0,0}
};

intmain(intargc,char**argv)
{
dev_name="/dev/video0";
for(;;)
{
intindex;
intc;

c=getopt_long(argc,argv,short_options,long_options,&index);
if(-1==c)
break;

switch(c){
case0:
break;

case'd':
dev_name=optarg;
break;

case'h':
usage(stdout,argc,argv);
exit(EXIT_SUCCESS);
case't':
time_in_sec_capture=atoi(optarg);
break;

default:
usage(stderr,argc,argv);
exit(EXIT_FAILURE);
}
}

open_device();
init_device();
start_capturing();
run();
stop_capturing();
uninit_device();
close_device();
exit(EXIT_SUCCESS);
return 0;
}
上面code中我已经标注出程序顺序指向的步骤1--14步，下面将一一说明应用从做这14步时驱动层是怎样响应，变化过程，驱动加载初始化部分上一篇文章已经说过了
正式开始取经之路哇。。。。![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/13.gif)![](http://blog.chinaunix.net/kindeditor/plugins/emoticons/images/30.gif)。。。
STEP 1：
fd = open (dev_name, O_RDWR| O_NONBLOCK, 0);
打开字符设备，这个字符设备是video_device_register时创建的，code在v4l2_dev.c中，具体：
staticintv4l2_open(struct inode*inode,struct
 file*filp)
{
struct video_device*vdev;
intret=0;

/*Checkifthe video deviceisavailable*/
mutex_lock(&videodev_lock);
vdev=video_devdata(filp);
/*return ENODEVifthe video device has already been removed.*/
if(vdev==NULL||!video_is_registered(vdev)){
mutex_unlock(&videodev_lock);
return-ENODEV;
}
/*andincrease the device refcount*/
video_get(vdev);
mutex_unlock(&videodev_lock);

/*
*Here using the API yougetthe method yougetthe open()method write
*The other methodsinfops use the same methodtouse you own code
*/
if(vdev->fops->open){
if(vdev->lock&&mutex_lock_interruptible(vdev->lock)){
ret=-ERESTARTSYS;
gotoerr;
}
if(video_is_registered(vdev))
ret=vdev->fops->open(filp);
else
ret=-ENODEV;
if(vdev->lock)
mutex_unlock(vdev->lock);
}

err:
/*decrease the refcountincaseof anerror*/
if(ret)
video_put(vdev);
return ret;
}
重点在标注部分，通过这个V4L2的API调用我们自己驱动程序中定义的open方法，我们自己的open方法所属的fops是在vivi.c驱动程序的vivi_create_instance方法中video_device_register之前关联进来的
intv4l2_fh_open(struct file*filp)
{
struct video_device*vdev=video_devdata(filp);
struct v4l2_fh*fh=kzalloc(sizeof(*fh),GFP_KERNEL);

/*
*INthe open method,doonly one job
*setv4l2_fh into filp->private_dataforlater use,andinitial v4l2_fh
*/
filp->private_data=fh;
if(fh==NULL)
return-ENOMEM;
v4l2_fh_init(fh,vdev);
v4l2_fh_add(fh);
return 0;
}
EXPORT_SYMBOL_GPL(v4l2_fh_open);
这个open方法只是初始化了一个v4l2_fh，并关联到filp->private中，方便以后使用
这里设置V4L2_FL_USES_V4L2_FH这个标志位，设置优先级为UNSET，如果我们的自己驱动程序实现了，支持
VIDIOC_SUBSCRIBE_EVENT,那么v4l2_event_init，在events初始化中初始化链表，并设置sequence为-1，如果不支持，则设置fh->events为NULL
最后add list
STEP 2：
if (-1 == xioctl (fd,
 VIDIOC_QUERYCAP, &cap))
这么调用完成下面过程，不行的从驱动层获取cap。直到成功拿到我们想要的数据
staticintxioctl(intfd,intrequest,void*arg)
{
intr;
/*Here use this methodtomake sure cmd success*/
dor=ioctl(fd,request,arg);
while(-1==r&&EINTR==errno);
return r;
}
也就是调用驱动层的ioctl方法，从v4l2 api中的ictol 调用我们自己定义的ioctl ，这中间的过程不在多做说明，我们自己的驱动的控制过程由v4l2_ioctl.c这个文件中的方法实现，一个很庞大的switch
值得一提的是，慢慢后面你会明白的，这里v4l2_ioctl.c这个文件中的方法实现其实只是会中转站，它接着就回调了我们自己驱动程序中定义的控制接口，后面再说吧
long video_ioctl2(struct file*file,
unsignedintcmd,unsigned long arg)
{
returnvideo_usercopy(file,cmd,arg,__video_do_ioctl);
}
这里这个__video_do_ioctl方法其实完全做了我们所有的控制过程，又为什么又要经过video_usercopy这个方法呢，不妨看一看这个方法
long
video_usercopy(struct file*file,unsignedintcmd,unsigned long arg,
v4l2_kioctl func)
{
char    sbuf[128];
void*mbuf=NULL;
void*parg=(void*)arg;
longerr=-EINVAL;
bool    has_array_args;
size_t array_size=0;
void __user*user_ptr=NULL;
void**kernel_ptr=NULL;

/*Copy arguments into temp kernel buffer*/
if(_IOC_DIR(cmd)!=_IOC_NONE){
........这里检查128个字节的大小是否够存放用户端发送来的数据，不够则需要重新申请一个新的内存用来存放，指向parg这个地址
if(_IOC_SIZE(cmd)<=sizeof(sbuf)){
parg=sbuf;
}else{
/*too bigtoallocate from stack*/
mbuf=kmalloc(_IOC_SIZE(cmd),GFP_KERNEL);
if(NULL==mbuf)
return-ENOMEM;
parg=mbuf;
}

err=-EFAULT;
if(_IOC_DIR(cmd)&_IOC_WRITE){
unsigned long n=cmd_input_size(cmd);

if(copy_from_user(parg,(void __user*)arg,n))
goto out;

/*zero out anything we don't copy from userspace*/
if(n<_IOC_SIZE(cmd))
memset((u8*)parg+n,0,_IOC_SIZE(cmd)-n);
}else{
/*read-only ioctl*/
memset(parg,0,_IOC_SIZE(cmd));
}
}
....check
err=check_array_args(cmd,parg,&array_size,&user_ptr,&kernel_ptr);
if(err<0)
goto out;
has_array_args=err;
....这里这块如果用户端有数据写到kernel，这里负责数据拷贝
if(has_array_args){
/*
*When adding new types ofarrayargs,make sure that the
*parent argumenttoioctl(which contains the pointertothe
*array)fits into sbuf(so that mbuf will still remain
*unused uptohere).
*/
mbuf=kmalloc(array_size,GFP_KERNEL);
err=-ENOMEM;
if(NULL==mbuf)
goto out_array_args;
err=-EFAULT;
if(copy_from_user(mbuf,user_ptr,array_size))
goto out_array_args;
*kernel_ptr=mbuf;
}

/*Handles IOCTL*/
err=func(file,cmd,parg);
if(err==-ENOIOCTLCMD)
err=-EINVAL;
if(has_array_args){
*kernel_ptr=user_ptr;
if(copy_to_user(user_ptr,mbuf,array_size))
err=-EFAULT;
goto out_array_args;
}
if(err<0)
goto out;

out_array_args:
/*Copy
 results into user buffer*/
switch(_IOC_DIR(cmd)){
case_IOC_READ:
case(_IOC_WRITE|_IOC_READ):
if(copy_to_user((void __user*)arg,parg,_IOC_SIZE(cmd)))
err=-EFAULT;
break;
}

out:
kfree(mbuf);
returnerr;
}
EXPORT_SYMBOL(video_usercopy);
自我感觉这个方法还是有很多精妙之处的，主要的控制过程是在我标注的地方调用完成的，这个调用之前做check动作，检查用户端发来的命令是否合法，
最重要的是把用户端的数据copy到kernel 端；而这个调用之后，则是我们处理完我们的动作之后，我们在这里吧用户端请求的数据从kernel 端copy到用户端
这样做的好处是显而易见的，任务明确，控制只做控制，用户空间和kernel空间数据的copy在所有控制之前，控制之后进行
以上动作做完之后，进入庞大的控制中枢，这来开始至贴出具体到某一个控制的代码，否则code过大，不易分析：
caseVIDIOC_QUERYCAP://查询视频设备的功能
{
struct v4l2_capability*cap=(struct v4l2_capability*)arg;

if(!ops->vidioc_querycap)
break;

ret=ops->vidioc_querycap(file,fh,cap);
if(!ret)/*i don't think here needtocheck*/
dbgarg(cmd,"driver=%s, card=%s, bus=%s, "
"version=0x%08x, "
"capabilities=0x%08x\n",
cap->driver,cap->card,cap->bus_info,
cap->version,
cap->capabilities);
break;
}
这来调用了我们自己驱动中填充的v4l2_ioctl_ops结构体，从这里开始，我上面说到的话得到了验证，这就是linux 中API 的强大之处
作为中间层的这个控制中枢又回调驱动自己定义编写的控制
/*------------------------------------------------------------------
IOCTL vidioc handling
------------------------------------------------------------------*/
staticintvidioc_querycap(struct file*file,void*priv,
struct v4l2_capability*cap)
{
struct vivi_dev*dev=video_drvdata(file);

strcpy(cap->driver,"vivi");
strcpy(cap->card,"vivi");
strlcpy(cap->bus_info,dev->v4l2_dev.name,sizeof(cap->bus_info));
cap->version=VIVI_VERSION;
cap->capabilities=V4L2_CAP_VIDEO_CAPTURE|V4L2_CAP_STREAMING|\
V4L2_CAP_READWRITE;
return 0;
}
这来做的事情很简单，只是将配置信息保存到cap这个变量中，之后上传给用户空间
STEP 3：
/* 3. VIDIOC_CROPCAP查询驱动的修剪能力*/
/* 这里在vivi 驱动中我们没有实现此方法，即不支持此操作*/
if (0 == xioctl (fd, VIDIOC_CROPCAP, &cropcap))
这个判断在中间层控制中枢中进行的，check到我们自己的驱动中没有这个控制功能的支持
所以这里的STEP
 4同样不会进行
STEP 5：
/* 5. VIDIOC_S_FMT设置当前驱动的频捕获格式*/
if (-1 == xioctl (fd, VIDIOC_S_FMT, &fmt))
对应到控制中心是这样的
caseVIDIOC_S_FMT:
{
struct v4l2_format*f=(struct v4l2_format*)arg;

/*FIXME:Should be one dump per type*/
dbgarg(cmd,"type=%s\n",prt_names(f->type,v4l2_type_names));

switch(f->type){
caseV4L2_BUF_TYPE_VIDEO_CAPTURE:
CLEAR_AFTER_FIELD(f,fmt.pix);
v4l_print_pix_fmt(vfd,&f->fmt.pix);
if(ops->vidioc_s_fmt_vid_cap){
ret=ops->vidioc_s_fmt_vid_cap(file,fh,f);
}elseif(ops->vidioc_s_fmt_vid_cap_mplane){
if(fmt_sp_to_mp(f,&f_copy))
break;
ret=ops->vidioc_s_fmt_vid_cap_mplane(file,fh,
&f_copy);
if(ret)
break;

if(f_copy.fmt.pix_mp.num_planes>1){
/*Drivers shouldn't adjust from 1-plane
*tomore than 1-plane formats*/
ret=-EBUSY;
WARN_ON(1);
break;
}

ret=fmt_mp_to_sp(&f_copy,f);
}
break;
caseV4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE:
CLEAR_AFTER_FIELD(f,fmt.pix_mp);
v4l_print_pix_fmt_mplane(vfd,&f->fmt.pix_mp);
if(ops->vidioc_s_fmt_vid_cap_mplane){
ret=ops->vidioc_s_fmt_vid_cap_mplane(file,
fh,f);
}elseif(ops->vidioc_s_fmt_vid_cap&&
f->fmt.pix_mp.num_planes==1){
if(fmt_mp_to_sp(f,&f_copy))
break;
ret=ops->vidioc_s_fmt_vid_cap(file,
fh,&f_copy);
if(ret)
break;

ret=fmt_sp_to_mp(&f_copy,f);
}
break;
caseV4L2_BUF_TYPE_VIDEO_OVERLAY:
CLEAR_AFTER_FIELD(f,fmt.win);
if(ops->vidioc_s_fmt_vid_overlay)
ret=ops->vidioc_s_fmt_vid_overlay(file,
fh,f);
break;
caseV4L2_BUF_TYPE_VIDEO_OUTPUT:
CLEAR_AFTER_FIELD(f,fmt.pix);
v4l_print_pix_fmt(vfd,&f->fmt.pix);
if(ops->vidioc_s_fmt_vid_out){
ret=ops->vidioc_s_fmt_vid_out(file,fh,f);
}elseif(ops->vidioc_s_fmt_vid_out_mplane){
if(fmt_sp_to_mp(f,&f_copy))
break;
ret=ops->vidioc_s_fmt_vid_out_mplane(file,fh,
&f_copy);
if(ret)
break;

if(f_copy.fmt.pix_mp.num_planes>1){
/*Drivers shouldn't adjust from 1-plane
*tomore than 1-plane formats*/
ret=-EBUSY;
WARN_ON(1);
break;
}

ret=fmt_mp_to_sp(&f_copy,f);
}
break;
caseV4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE:
CLEAR_AFTER_FIELD(f,fmt.pix_mp);
v4l_print_pix_fmt_mplane(vfd,&f->fmt.pix_mp);
if(ops->vidioc_s_fmt_vid_out_mplane){
ret=ops->vidioc_s_fmt_vid_out_mplane(file,
fh,f);
}elseif(ops->vidioc_s_fmt_vid_out&&
f->fmt.pix_mp.num_planes==1){
if(fmt_mp_to_sp(f,&f_copy))
break;
ret=ops->vidioc_s_fmt_vid_out(file,
fh,&f_copy);
if(ret)
break;

ret=fmt_mp_to_sp(&f_copy,f);
}
break;
caseV4L2_BUF_TYPE_VIDEO_OUTPUT_OVERLAY:
CLEAR_AFTER_FIELD(f,fmt.win);
if(ops->vidioc_s_fmt_vid_out_overlay)
ret=ops->vidioc_s_fmt_vid_out_overlay(file,
fh,f);
break;
caseV4L2_BUF_TYPE_VBI_CAPTURE:
CLEAR_AFTER_FIELD(f,fmt.vbi);
if(ops->vidioc_s_fmt_vbi_cap)
ret=ops->vidioc_s_fmt_vbi_cap(file,fh,f);
break;
caseV4L2_BUF_TYPE_VBI_OUTPUT:
CLEAR_AFTER_FIELD(f,fmt.vbi);
if(ops->vidioc_s_fmt_vbi_out)
ret=ops->vidioc_s_fmt_vbi_out(file,fh,f);
break;
caseV4L2_BUF_TYPE_SLICED_VBI_CAPTURE:
CLEAR_AFTER_FIELD(f,fmt.sliced);
if(ops->vidioc_s_fmt_sliced_vbi_cap)
ret=ops->vidioc_s_fmt_sliced_vbi_cap(file,
fh,f);
break;
caseV4L2_BUF_TYPE_SLICED_VBI_OUTPUT:
CLEAR_AFTER_FIELD(f,fmt.sliced);
if(ops->vidioc_s_fmt_sliced_vbi_out)
ret=ops->vidioc_s_fmt_sliced_vbi_out(file,
fh,f);
break;
caseV4L2_BUF_TYPE_PRIVATE:
/*CLEAR_AFTER_FIELD(f,fmt.raw_data);<-doesnothing*/
if(ops->vidioc_s_fmt_type_private)
ret=ops->vidioc_s_fmt_type_private(file,
fh,f);
break;
}
break;
}
以后根据不同的type 决定了我们自己驱动程序中不同的控制实现，这个type是根据用户空间的设置而定的，还包括其他几个参数，如下：
fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
fmt.fmt.pix.width=640;
fmt.fmt.pix.height=480;
fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUYV;
fmt.fmt.pix.field=V4L2_FIELD_INTERLACED;
这里根据设定的type，所以驱动程序的处理过程如下：
staticintvidioc_s_fmt_vid_cap(struct file*file,void*priv,
struct v4l2_format*f)
{
struct vivi_dev*dev=video_drvdata(file);
struct vb2_queue*q=&dev->vb_vidq;
....在下面这个函数中，做了一些试探性的动作，如果试探失败则下面不会赋值，试探通过则后续正常设置即可，在这个试探函数中同时做了一些设置动作
intret=vidioc_try_fmt_vid_cap(file,priv,f);
if(ret<0)
return ret;

if(vb2_is_streaming(q)){
dprintk(dev,1,"%s
 device busy\n",__func__);
return-EBUSY;
}
....按用户空间需求设置
dev->fmt=get_format(f);
dev->width=f->fmt.pix.width;
dev->height=f->fmt.pix.height;
dev->field=f->fmt.pix.field;

return 0;
}

STEP6
 ：
/* 6. VIDIOC_REQBUFS分配内存*/
if (-1 == xioctl (fd, VIDIOC_REQBUFS, &req))
中间层控制中枢：
caseVIDIOC_REQBUFS:
{
struct v4l2_requestbuffers*p=arg;

if(!ops->vidioc_reqbufs)
break;
........这个方法check 驱动必须实现了fmt方法，看具体看代码
ret=check_fmt(ops,p->type);
if(ret)
break;

if(p->type<V4L2_BUF_TYPE_PRIVATE)
CLEAR_AFTER_FIELD(p,memory);

ret=ops->vidioc_reqbufs(file,fh,p);
dbgarg(cmd,"count=%d, type=%s, memory=%s\n",
p->count,
prt_names(p->type,v4l2_type_names),
prt_names(p->memory,v4l2_memory_names));
break;
}
驱动中实现：
staticintvidioc_reqbufs(struct file*file,void*priv,
struct v4l2_requestbuffers*p)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_reqbufs(&dev->vb_vidq,p);
}
到了这里来到了这个全新的话题，实现
vb2_reqbufs(&dev->vb_vidq,p);
这里暂且不讨论这个方法，相对较复杂，待日后研究，先把注释部分放到这里，包括其他内存操作，之后深入研究补充，专门作为一篇整理
/**
* Should be called from vidioc_reqbufs ioctl handler of a driver.
* This function:
* 1) verifies streaming parameters passed from the userspace,
* 2) sets up the queue,
* 3) negotiates number of buffers and planes per buffer with the driver to be used during streaming,
* 4) allocates internal buffer structures (struct vb2_buffer), according to the agreed parameters,
* 5) for MMAP memory type, allocates actual video memory, using the memory handling/allocation routines provided during queue initialization
* If req->count is 0, all the memory will be freed instead.
* If the queue has been allocated previously (by a previous vb2_reqbufs) call
* and the queue is not busy, memory will be reallocated.
* The return values from this function are intended to be directly returned from vidioc_reqbufs handler in driver.
*/
STEP 7：
/* 7. VIDIOC_QUERYBUF把VIDIOC_REQBUFS中分配的数据缓存转换成物理地址*/
if (-1 == xioctl (fd, VIDIOC_QUERYBUF, &buf))
中间层控制中枢：
caseVIDIOC_QUERYBUF:
{
struct v4l2_buffer*p=arg;

if(!ops->vidioc_querybuf)
break;
ret=check_fmt(ops,p->type);
if(ret)
break;

ret=ops->vidioc_querybuf(file,fh,p);
if(!ret)
dbgbuf(cmd,vfd,p);
break;
}
驱动中控制实现：
staticintvidioc_querybuf(struct file*file,void*priv,struct
 v4l2_buffer*p)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_querybuf(&dev->vb_vidq,p);
}
/**
* Should be called from vidioc_querybuf ioctl handler in driver.
* This function will verify the passed v4l2_buffer structure and fill the
* relevant information for the userspace.
* The return values from this function are intended to be directly returned from vidioc_querybuf handler in driver.
*/
STEP 8：
/* 8. VIDIOC_QBUF把数据从缓存中读取出来*/
if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
中间层控制中枢：
caseVIDIOC_QBUF:
{
struct v4l2_buffer*p=arg;

if(!ops->vidioc_qbuf)
break;
ret=check_fmt(ops,p->type);
if(ret)
break;

ret=ops->vidioc_qbuf(file,fh,p);
if(!ret)
dbgbuf(cmd,vfd,p);
break;
}
驱动中控制实现：
staticintvidioc_qbuf(struct file*file,void*priv,struct
 v4l2_buffer*p)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_qbuf(&dev->vb_vidq,p);
}
/**
* Should be called from vidioc_qbuf ioctl handler of a driver.
* This function:
* 1) verifies the passed buffer,
* 2) calls buf_prepare callback in the driver (if provided), in which driver-specific buffer initialization can be performed,
* 3) if streaming is on, queues the buffer in driver by the means of buf_queue callback for processing.
* The return values from this function are intended to be directly returned from vidioc_qbuf handler in driver.
*/
STEP 9：
/* 9. VIDIOC_STREAMON开始视频显示函数*/
if (-1 == xioctl (fd, VIDIOC_STREAMON, &type))
中间层控制中枢：
caseVIDIOC_STREAMON:
{
enum v4l2_buf_type i=*(int*)arg;

if(!ops->vidioc_streamon)
break;
dbgarg(cmd,"type=%s\n",prt_names(i,v4l2_type_names));
ret=ops->vidioc_streamon(file,fh,i);
break;
}
驱动控制实现;
staticintvidioc_streamon(struct file*file,void*priv,enum
 v4l2_buf_type i)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_streamon(&dev->vb_vidq,i);
}
/**
* Should be called from vidioc_streamon handler of a driver.
* This function:
* 1) verifies current state
* 2) starts streaming and passes any previously queued buffers to the driver
* The return values from this function are intended to be directly returned from vidioc_streamon handler in the driver.
*/
STEP 10：
/* 10. poll method*/
select (fd + 1, &fds, NULL, NULL, &tv);
从V4L2驱动API开始：
static unsignedintv4l2_poll(struct file*filp,struct
 poll_table_struct*poll)
{
struct video_device*vdev=video_devdata(filp);
intret=POLLERR|POLLHUP;

if(!vdev->fops->poll)
return DEFAULT_POLLMASK;
if(vdev->lock)
mutex_lock(vdev->lock);
if(video_is_registered(vdev))
ret=vdev->fops->poll(filp,poll);
if(vdev->lock)
mutex_unlock(vdev->lock);
return ret;
}
驱动实现：
static unsignedint
vivi_poll(struct file*file,struct poll_table_struct*wait)
{
struct vivi_dev*dev=video_drvdata(file);
struct vb2_queue*q=&dev->vb_vidq;

dprintk(dev,1,"%s\n",__func__);
return vb2_poll(q,file,wait);
}
/**
* This function implements poll file operation handler for a driver.
* For CAPTURE queues, if a buffer is ready to be dequeued, the userspace will be informed that the file descriptor of a video device is available for reading.
* For OUTPUT queues, if a buffer is ready to be dequeued, the file descriptor will be reported as available for writing.
* The return values from this function are intended to be directly returned from poll handler in driver.
*/
STEP 11：
/* 11. VIDIOC_DQBUF把数据放回缓存队列*/
if (-1 == xioctl (fd, VIDIOC_DQBUF, &buf))
中间层控制中枢：
caseVIDIOC_DQBUF:
{
struct v4l2_buffer*p=arg;

if(!ops->vidioc_dqbuf)
break;
ret=check_fmt(ops,p->type);
if(ret)
break;

ret=ops->vidioc_dqbuf(file,fh,p);
if(!ret)
dbgbuf(cmd,vfd,p);
break;
}
驱动控制实现：
staticintvidioc_dqbuf(struct file*file,void*priv,struct
 v4l2_buffer*p)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_dqbuf(&dev->vb_vidq,p,file->f_flags&O_NONBLOCK);
}
/**
* Should be called from vidioc_dqbuf ioctl handler of a driver.
* This function:
* 1) verifies the passed buffer,
* 2) calls buf_finish callback in the driver (if provided), in which driver can perform any additional operations that may be required before returning the buffer
 to userspace, such as cache sync,
* 3) the buffer struct members are filled with relevant information for the userspace.
* The return values from this function are intended to be directly returned from vidioc_dqbuf handler in driver.
*/
STEP 12：
/*12. VIDIOC_QBUF把数据从缓存中读取出来*/
if (-1 == xioctl (fd, VIDIOC_QBUF, &buf))
中间层控制中枢：
caseVIDIOC_QBUF:
{
struct v4l2_buffer*p=arg;

if(!ops->vidioc_qbuf)
break;
ret=check_fmt(ops,p->type);
if(ret)
break;

ret=ops->vidioc_qbuf(file,fh,p);
if(!ret)
dbgbuf(cmd,vfd,p);
break;
}
驱动控制实现：
staticintvidioc_qbuf(struct file*file,void*priv,struct
 v4l2_buffer*p)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_qbuf(&dev->vb_vidq,p);
}

STEP 13：
/*13. VIDIOC_STREAMOFF结束视频显示函数*/
if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
中间层控制中枢：
caseVIDIOC_STREAMOFF:
{
enum v4l2_buf_type i=*(int*)arg;

if(!ops->vidioc_streamoff)
break;
dbgarg(cmd,"type=%s\n",prt_names(i,v4l2_type_names));
ret=ops->vidioc_streamoff(file,fh,i);
break;
}
驱动控制实现：
staticintvidioc_streamoff(struct file*file,void*priv,enum
 v4l2_buf_type i)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_streamoff(&dev->vb_vidq,i);
}

STEP 13：
/*13. VIDIOC_STREAMOFF结束视频显示函数*/
if (-1 == xioctl (fd, VIDIOC_STREAMOFF, &type))
中间层控制中枢：
caseVIDIOC_STREAMOFF:
{
enum v4l2_buf_type i=*(int*)arg;

if(!ops->vidioc_streamoff)
break;
dbgarg(cmd,"type=%s\n",prt_names(i,v4l2_type_names));
ret=ops->vidioc_streamoff(file,fh,i);
break;
}
驱动控制实现：
staticintvidioc_streamoff(struct file*file,void*priv,enum
 v4l2_buf_type i)
{
struct vivi_dev*dev=video_drvdata(file);
return vb2_streamoff(&dev->vb_vidq,i);
}

STEP 14：
/*14. close method*/
close(fbfd);
staticintv4l2_release(struct inode*inode,struct
 file*filp)
{
struct video_device*vdev=video_devdata(filp);
intret=0;

if(vdev->fops->release){
if(vdev->lock)
mutex_lock(vdev->lock);
vdev->fops->release(filp);
if(vdev->lock)
mutex_unlock(vdev->lock);
}
/*decrease the refcount unconditionally since the release()
return valueisignored.*/
video_put(vdev);
return ret;
}

staticintvivi_close(struct file*file)
{
struct video_device*vdev=video_devdata(file);
struct vivi_dev*dev=video_drvdata(file);

dprintk(dev,1,"close
 called (dev=%s), file %p\n",
video_device_node_name(vdev),file);

if(v4l2_fh_is_singular_file(file))
vb2_queue_release(&dev->vb_vidq);
return v4l2_fh_release(file);
}
到此为止，整个过程算是基本完结了，不过其中videobuf2_core.c 在我看来自己必须专门钻研一下了
videobuf2_core.c 是视频数据传输的核心
也可以说是视频驱动的重中之重
待续。。。。。。

