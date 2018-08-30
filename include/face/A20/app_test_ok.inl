
//#加了点注释

//#Rockie Cheng

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>            

#include <fcntl.h>             
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <asm/types.h>         
#include <linux/videodev2.h>
#include <time.h>
#include <linux/fb.h>
//#include "../../../../../../linux-v2.6.36.4/include/linux/drv_display.h"//modify this
#include "drv_display_sun4i.h"//modify this

//#define READ_NUM 5000
//#define DISPLAY
#define LCD_WIDTH  800
#define LCD_HEIGHT 480

#define CLEAR(x) memset (&(x), 0, sizeof (x))

int count;

struct buffer {
  void *                  start;
  size_t                  length;
};

struct size{
  int width;
  int height;
};

static char *           dev_name        = "/dev/video0";//摄像头设备名
static int              fd              = -1;
struct buffer *         buffers         = NULL;
static unsigned int     n_buffers       = 0;

FILE *file_fd;
static unsigned long file_length;
static unsigned char *file_name;

int disphd;
unsigned int hlay;
int sel = 0;//which screen 0/1
__disp_layer_info_t layer_para;
__disp_video_fb_t video_fb;
__u32 arg[4];

//struct timeval time_test;   
//struct timezone tz; 

struct size input_size;
struct size disp_size;
//int  csi_format;
__disp_pixel_fmt_t  disp_format;
__disp_pixel_mod_t  disp_mode;
__disp_pixel_seq_t disp_seq;
int  read_num=100;
int  test_num=10;
int  req_frame_num;
int  fps=30;
int  fps_test=0;
int  invalid_ops=0;
int  invalid_fmt_test=0;
int  control_test=0;
int  ioctl_test=0;
int  lost_frame_test=0;
struct test_case{
  int          input_width;
  int         input_height;
  int          disp_width;
  int         disp_height;
  int         csi_format;
  __disp_pixel_fmt_t  disp_format;
  __disp_pixel_mod_t disp_mode;
  __disp_pixel_seq_t disp_seq;
}; 

#include "test_case_set.inl"
#include "disp.inl"

#include "face/face_recog.inl"
#include "linux_tty.inl"
#include "arg.inl"

int main_test (void)
{
  struct v4l2_capability cap;
  struct v4l2_format fmt;
  unsigned int i;
  enum v4l2_buf_type type;
  struct v4l2_cropcap cropcap;
  struct v4l2_input inp;
  struct v4l2_streamparm parms;
  int fdtty = OpenDev_2("/dev/ttyS7");
  
  //fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备
  //fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备
  //close (fd);
  fd = open (dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备
  
  if(invalid_ops)
  {
    if(-1 == ioctl (fd, 0xff, &cropcap))
      printf("invalid_ops return error\n");
  }
  
  inp.index = 0;
  
  if (inp.type == V4L2_INPUT_TYPE_CAMERA)
    printf("enuminput type is V4L2_INPUT_TYPE_CAMERA!\n");
  
  if (-1 == ioctl (fd, VIDIOC_S_INPUT, &inp)) //设置输入index
    printf("VIDIOC_S_INPUT error!\n");
  
  if(ioctl_test==1)
  {
    //Test VIDIOC_QUERYCAP
    if (-1 == ioctl (fd, VIDIOC_QUERYCAP, &cap))//获取摄像头参数
      printf("VIDIOC_QUERYCAP error!\n");
    
    printf("cap.driver=%s\n",cap.driver);
    printf("cap.card=%s\n",cap.card);
    printf("cap.bus_info=%s\n",cap.bus_info);
    printf("cap.version=%d\n",cap.version);
    printf("cap.capabilities=%d\n",cap.capabilities);
    
    //Test VIDIOC_ENUMINPUT,VIDIOC_S_INPUT,VIDIOC_G_INPUT
    struct v4l2_input inp;
    
    for(i=0;i<2;i++)
    {
      inp.index = i;
      if (-1 == ioctl (fd, VIDIOC_ENUMINPUT, &inp))//获取输入参数
        printf("VIDIOC_ENUMINPUT error!\n");
      if (inp.type == V4L2_INPUT_TYPE_CAMERA)
        printf("enuminput type is V4L2_INPUT_TYPE_CAMERA!\n");
      
      if (-1 == ioctl (fd, VIDIOC_S_INPUT, &inp)) //设置输入index
        printf("VIDIOC_S_INPUT error!\n");
      
      if (-1 == ioctl (fd, VIDIOC_G_INPUT, &inp)) //获取输入index
        printf("VIDIOC_G_INPUT error!\n");
      printf("input index is %d\n",inp.index);
    } 
    //Test VIDIOC_ENUM_FMT
    struct v4l2_fmtdesc fmtdesc;
    
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    for(i=0;i<12;i++)
    {
      fmtdesc.index = i;
      if (-1 == ioctl (fd, VIDIOC_ENUM_FMT, &fmtdesc))//获取格式参数
      {
        printf("VIDIOC_ENUM_FMT error!\n");
        continue;
      }
      printf("**************************************************************\n");
      printf("format index = %d, name = %s, v4l2 pixel format = %x\n",i,fmtdesc.description,fmtdesc.pixelformat); 
    }  
  }
  //V4L2_PIX_FMT_YUYV;//csi_format;//V4L2_PIX_FMT_YUV422P;//V4L2_PIX_FMT_NV12;//V4L2_PIX_FMT_YUYV;
  int csi_format = V4L2_PIX_FMT_YUYV;
  //  printf("%s %d\n",__FILE__,__LINE__);
  //Test VIDIOC_S_FMT
  CLEAR (fmt);
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = input_size.width; //320; 
  fmt.fmt.pix.height      = input_size.height; //240;
  fmt.fmt.pix.pixelformat = csi_format;
  fmt.fmt.pix.field       = V4L2_FIELD_NONE;
  int ret = -1;
  
  
  if(invalid_fmt_test==1)
  {
    printf("Try V4L2_PIX_FMT_YUV410\n");
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUV410;
    if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt)) //设置错误图像格式
    {
      printf("VIDIOC_S_FMT error!\n");
    }
    
    printf("Try V4L2_PIX_FMT_YVU420\n");
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YVU420;
    if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt)) //设置错误图像格式
    {
      printf("VIDIOC_S_FMT error!\n");
    }
    
    printf("Try V4L2_PIX_FMT_NV16\n");
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_NV16;
  }
  
  if (-1 == ioctl (fd, VIDIOC_S_FMT, &fmt)) //设置图像格式
  {
    printf("VIDIOC_S_FMT error!\n");
    ret = -1;
    return ret;
    //goto close;
  }
  
  int pic_width = fmt.fmt.pix.width;
  int pic_height = fmt.fmt.pix.height;
  disp_size.width = fmt.fmt.pix.width;
  disp_size.height = fmt.fmt.pix.height;
  

  //  printf("%s %d\n",__FILE__,__LINE__);
  if(ioctl_test==1)
  { 
    printf("**************************************************************\n");
    printf("fmt.type = %d\n",fmt.type);
    printf("fmt.fmt.pix.width = %d\n",fmt.fmt.pix.width);
    printf("fmt.fmt.pix.height = %d\n",fmt.fmt.pix.height);
    printf("fmt.fmt.pix.pixelformat = %x\n",fmt.fmt.pix.pixelformat);
    printf("fmt.fmt.pix.field = %d\n",fmt.fmt.pix.field);
  }  
  //Test VIDIOC_G_FMT

  if (-1 == ioctl (fd, VIDIOC_G_FMT, &fmt)) //获取图像格式
  {
    printf("VIDIOC_G_FMT error!\n");
  }
  else
  {
    printf("**************************************************************\n");
    printf("resolution got from sensor = %d*%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
    printf("**************************************************************\n");
  }

  if(ioctl_test==1)
  { 
    printf("**************************************************************\n");
    printf("fmt.fmt.pix.width = %d\n",fmt.fmt.pix.width);
    printf("fmt.fmt.pix.height = %d\n",fmt.fmt.pix.height);
    printf("fmt.fmt.pix.pixelformat = %x\n",fmt.fmt.pix.pixelformat);
    printf("fmt.fmt.pix.field = %d\n",fmt.fmt.pix.field);
    printf("fmt.fmt.pix.bytesperline = %d\n",fmt.fmt.pix.bytesperline);
    printf("fmt.fmt.pix.sizeimage = %d\n",fmt.fmt.pix.sizeimage);
  }
  
  parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  parms.parm.capture.timeperframe.denominator = 1;
  parms.parm.capture.timeperframe.denominator = fps;
  
  if (-1 == ioctl (fd, VIDIOC_S_PARM, &parms)) //获取帧率
    printf ("VIDIOC_S_PARM error\n");
  
  if(fps_test==1)
  {
    //Test VIDIOC_G_PARM
    struct v4l2_streamparm parms;
    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    if (-1 == ioctl (fd, VIDIOC_G_PARM, &parms)) //获取帧率
      printf ("VIDIOC_G_PARM error\n");
    
    printf("numerator = %d\n",parms.parm.capture.timeperframe.numerator);
    printf("denominator = %d\n",parms.parm.capture.timeperframe.denominator);
  }
  
  //count=read_num;
  
  struct v4l2_requestbuffers req;
  CLEAR (req);
  req.count               = req_frame_num;
  req.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory              = V4L2_MEMORY_MMAP;
  
  ioctl (fd, VIDIOC_REQBUFS, &req); //申请缓冲，count是申请的数量
  buffers = calloc (req.count, sizeof (*buffers));//内存中建立对应空间
  
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
  {
    struct v4l2_buffer buf;   //驱动中的一帧
    CLEAR (buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;
    
    if (-1 == ioctl (fd, VIDIOC_QUERYBUF, &buf)) //映射用户空间
      printf ("VIDIOC_QUERYBUF error\n");
    
    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start  = mmap (NULL /* start anywhere */,    //通过mmap建立映射关系
      buf.length,
      PROT_READ | PROT_WRITE /* required */,
      MAP_SHARED /* recommended */,
      fd, buf.m.offset);
    
    if (MAP_FAILED == buffers[n_buffers].start) {
      printf ("mmap failed\n");
    }
  }
  
  for (i = 0; i < n_buffers; ++i) 
  {
    struct v4l2_buffer buf;
    CLEAR (buf);
    
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    
    if (-1 == ioctl (fd, VIDIOC_QBUF, &buf))//申请到的缓冲进入列队
      printf ("VIDIOC_QBUF failed\n");
  }
  
#ifdef DISPLAY
  disp_int(disp_size.width,disp_size.height);
  disp_start();
#endif 
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  
  if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
    printf ("VIDIOC_STREAMON failed\n");
  else 
    printf ("VIDIOC_STREAMON ok\n");
  
  if (-1 == ioctl (fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
    printf ("VIDIOC_STREAMON failed\n");
  else 
    printf ("VIDIOC_STREAMON ok\n");
  
  count = read_num;
  
#ifdef DISPLAY
  disp_on();
#endif
  while(count-->0)    // while(1)
  {
    //gettimeofday(&time_test,&tz);
    //这一段涉及到异步IO
    for(;;) {
      fd_set fds;
      struct timeval tv;
      int r;
      
      FD_ZERO(&fds);//将指定的文件描述符集清空
      //在文件描述符集合中增加一个新的文件描述符
      FD_SET(fdtty, &fds);
      FD_SET(fd, &fds);
      
      /* Timeout. */
      tv.tv_sec = 2;
      tv.tv_usec = 0;
      
      r = select(fd + 1, &fds, NULL, NULL, &tv);//判断是否可读（即摄像头是否准备好），tv是定时
      
      if (-1 == r) { //(EINTR == errno)
        continue;
        printf ("select err\n");
      }
      if (0 == r) {
        fprintf (stderr, "select timeout\n");
        exit (EXIT_FAILURE);
      }
      
      char buff[256];//循环找在最大范围内的fd1
      
      //查找是否fd在tmpfds里面
      if (FD_ISSET(fdtty,&fds)) {
        int nread = read(fdtty, buff, 256);
        if (nread>0) {
          buff[nread] = 0;
          if (':'==buff[0]) {
            parse_arg(buff+1);
          }
          printf("%s", buff);
        }
      }
      if(FD_ISSET(fd,&fds)) {
        //获取一帧数据
        struct v4l2_buffer vbuf;
        unsigned int i;
        CLEAR (vbuf);
        vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        vbuf.memory = V4L2_MEMORY_MMAP;
        
        ioctl (fd, VIDIOC_DQBUF, &vbuf); //出列采集的帧缓冲
        
        if((fps_test==1)||(lost_frame_test==1)) {
          printf("process image %d sec %d usec\n", vbuf.timestamp.tv_sec, vbuf.timestamp.tv_usec);
        }
        
        assert(vbuf.index < n_buffers);
        //printf("buffers[ %d ].length = %d (%d %d)\n", vbuf.index, buffers[ vbuf.index ].length, input_size.width, input_size.height);
        // printf ("vbuf.index dq is %d,\n",vbuf.index);
        // printf ("vbuf.m.offset = 0x%x\n",vbuf.m.offset);  
        // disp_set_addr(320,240,&vbuf.m.offset);
        
        argA20_t* arg = argA20_run(pic_height, pic_width, buffers[vbuf.index].start, pic_width*2, 2, "YUYV");
#ifdef DISPLAY
        disp_set_addr(disp_size.width, disp_size.height,&vbuf.m.offset, csi_format);
#endif
        ioctl (fd, VIDIOC_QBUF, &vbuf); //再将其入列
        snprintf(buff, 256, ":frame=%d picW=%d picH=%d x=%d y=%d w=%d h=%d id=%d dis=%d time=%.4lf\n", count, pic_width, pic_height,
          arg->xywh[0], arg->xywh[1], arg->xywh[2], arg->xywh[3], arg->id, arg->dis, arg->time);
        int num = strlen(buff);
        int nwrite = write(fdtty, buff ,num);
        break;
      }
    } 
  }
  
  if(control_test==1) 
  {
    struct v4l2_queryctrl qc;
    struct v4l2_control ctrl;
    
    for(i=0;i<sizeof(qc_ctrl);i++)
    {
      CLEAR(qc);
      qc.id = qc_ctrl[i];
      if (-1 == ioctl (fd, VIDIOC_QUERYCTRL, &qc))
      {
        printf("VIDIOC_QUERYCTRL %s failed!\n",qc.name);
        continue;
      }
      else
      {
        printf("**************************************************************\n");
        printf("Name:%s\n",qc.name);
        printf("qc.min = %d\n",qc.minimum);
        printf("qc.max = %d\n",qc.maximum);
        printf("qc.step = %d\n",qc.step);
        printf("qc.default_value = %d\n",qc.default_value);
      }
      
      ctrl.id = qc.id;
      ctrl.value = qc.maximum;
      if (-1 == ioctl (fd, VIDIOC_S_CTRL, &ctrl))
        printf("VIDIOC_S_CTRL %s failed!\n",qc.name);
      else 
      {
        printf("%s set to max\n",qc.name);
        
      }
      
      ctrl.value = 0;
      if (-1 == ioctl (fd, VIDIOC_G_CTRL, &ctrl))
        printf("VIDIOC_G_CTRL %s failed!\n",qc.name);
      else 
      {
        printf("Name:%s\n",qc.name);
        printf("read ctrl.value = %d\n",ctrl.value);
      }
      
      printf("press ENTER to continue!\n");
      getchar();
      
      ctrl.value = qc.minimum;
      if (-1 == ioctl (fd, VIDIOC_S_CTRL, &ctrl))
        printf("VIDIOC_S_CTRL %s failed!\n",qc.name);
      else 
      {
        printf("%s set to min\n",qc.name);
      }
      
      ctrl.value = 0;
      if (-1 == ioctl (fd, VIDIOC_G_CTRL, &ctrl))
        printf("VIDIOC_G_CTRL %s failed!\n",qc.name);
      else 
      {
        printf("Name:%s\n",qc.name);
        printf("read ctrl.value = %d\n",ctrl.value);
      }
      
      printf("press ENTER to continue!\n");
      getchar();
      
      ctrl.value = qc.default_value;
      if (-1 == ioctl (fd, VIDIOC_S_CTRL, &ctrl))
        printf("VIDIOC_S_CTRL %s failed!\n",qc.name);
      else 
      {
        printf("%s set to default_value\n",qc.name);
      }
      
    }
  }
  
close: 
  if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type)) //停止捕捉图像数据
    printf ("VIDIOC_STREAMOFF failed\n");
  else
    printf ("VIDIOC_STREAMOFF ok\n");
  
  if (-1 == ioctl (fd, VIDIOC_STREAMOFF, &type)) //停止捕捉图像数据
    printf ("VIDIOC_STREAMOFF failed\n");
  else
    printf ("VIDIOC_STREAMOFF ok\n");
  
  if(read_num==1)
  {
    printf("press ENTER key to continue!\n");
    getchar();
  }
  
unmap:
  for (i = 0; i < n_buffers; ++i) {
    if (-1 == munmap (buffers[i].start, buffers[i].length)) {
      printf ("munmap error");
    }
  }
  disp_stop();
  disp_quit();
  
  close (fd);
  return 0;
}

int main(void)
{
  test_num=1;
  read_num=200;
  
  req_frame_num = 5;
  input_size.width = 640;//1600;//640;
  input_size.height = 480;//1200;//480;
  // disp_size.width = 1280;//1600;//640;
  // disp_size.height = 1024;//1200;//480;
  //csi_format=V4L2_PIX_FMT_NV12;
  disp_format=DISP_FORMAT_YUV420;
  disp_mode=DISP_MOD_NON_MB_UV_COMBINED;
  disp_seq=DISP_SEQ_UVUV;
  
  printf("********************************************************************Read stream test start,capture 1000 frames,press to continue\n");

#ifdef DISPLAY
    getchar();
#endif
  
  read_num = 1000;
  main_test();
  
  printf("********************************************************************test done,press to end\n");
  getchar();
  exit(EXIT_SUCCESS);
  return 0;
}

#include "sys/sys_linux.inl"


