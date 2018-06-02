
#ifndef _CAPV4L_INL_
#define _CAPV4L_INL_
#define MAX_V4L_BUFFERS  16
#define MAX_DEVICE_DRIVER_NAME 80
#define MAX_CAMERAS 16

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
#include "img/color.inl"


typedef struct capv4l {
  int w, h;
  int fd;
  ColorSpace colortype;
  buf_t buffers[MAX_V4L_BUFFERS];
} capv4l;

int capv4l_close(capdev* s0) {
  capv4l* s = (capv4l*)s0->x;
  int i;
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == ioctl (s->fd, VIDIOC_STREAMOFF, &type)) //停止捕捉图像数据
    printf ("VIDIOC_STREAMOFF failed\n");
  else
    printf ("VIDIOC_STREAMOFF ok\n");
  
  if (-1 == ioctl (s->fd, VIDIOC_STREAMOFF, &type)) //停止捕捉图像数据
    printf ("VIDIOC_STREAMOFF failed\n");
  else
    printf ("VIDIOC_STREAMOFF ok\n");
  
  for (i = 0; i < MAX_V4L_BUFFERS; ++i) {
    if (-1 == munmap(s->buffers[i].data, s->buffers[i].len)) {
      printf ("munmap error");
    }
  }
  close (s->fd);
  pfree(s);
  memset(s0, 0, sizeof(*s0));
  return 0;
}
int capv4l_getframe(capdev* s0, img_t* im, int flag) {
  capv4l* s = (capv4l*)s0->x;
  if (s) {
    struct v4l2_buffer buf = {0};
    unsigned int i;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    ioctl(s->fd, VIDIOC_DQBUF, &buf); //出列采集的帧缓冲
    //printf("process image %d sec %d usec\n",buf.timestamp.tv_sec,buf.timestamp.tv_usec);
    ASSERT(buf.index < MAX_V4L_BUFFERS);
    //	printf ("buf.index dq is %d,\n",buf.index);
    //printf ("buf.m.offset = 0x%x\n",buf.m.offset);
    //printf ("buffers[ %d ].len = %d\n", buf.index, s->buffers[ buf.index ].len);
    //disp_set_addr(320,240,&buf.m.offset);
    imsetsize(im, s->h, s->w, 3, 1);
    colorcvt(s->h, s->w, s->buffers[ buf.index ].data, 0, 0, im->tt.data, im->s, 3, s->colortype, T_BGR);
    //memset(buffers[ buf.index ].start, 255, s->buffers[ buf.index ].len);
    //disp_set_addr(disp_size.width, disp_size.height,&buf.m.offset);
    
    //printf ("press ENTER to continue!\n");
    //getchar();
    //fwrite(s->buffers[buf.index].start, s->buffers[buf.index].len, 1, file_fd); //将其写入文件中
    ioctl(s->fd, VIDIOC_QBUF, &buf); //再将其入列
  }
  return 1;
}

int capv4l_capnum() {
  int deviceHandle = 0;
  int CameraNumber = 0, numCameras = 0;
  char dev_name[ MAX_DEVICE_DRIVER_NAME ];
  while (CameraNumber < MAX_CAMERAS) {
    /* Print the CameraNumber at the end of the string with a width of one character */
    sprintf(dev_name, "/dev/video%1d", CameraNumber);
    /* Test using an open to see if this new device name really does exists. */
    deviceHandle = open(dev_name, O_RDONLY);
    
    if (deviceHandle != -1) {
      /* This device does indeed exist - add it to the total so far */
      // add indexList
      // indexList |= (1 << CameraNumber);
      numCameras++;
    }
    
    if (deviceHandle != -1) {
      close(deviceHandle);
    }
    /* Set up to test the next /dev/video source in line */
    CameraNumber++;
  } /* End while */
  return numCameras;
}
#if 0
int enum_frame_intervals(int dev, __u32 pixfmt, __u32 width, __u32 height)
{
  int ret;
  struct v4l2_frmivalenum fival;
  
  memset(&fival, 0, sizeof(fival));
  fival.index = 0;
  fival.pixel_format = pixfmt;
  fival.width = width;
  fival.height = height;
  printf("Time interval between frame: ");
  while ((ret = ioctl(dev, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) {
    if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) {
      printf("%u/%u, ", fival.discrete.numerator, fival.discrete.denominator); //输出分数
    } else if (fival.type == V4L2_FRMIVAL_TYPE_CONTINUOUS) {
      printf("{min { %u/%u } .. max { %u/%u } }, ",
        fival.stepwise.min.numerator, fival.stepwise.min.numerator,
        fival.stepwise.max.denominator, fival.stepwise.max.denominator);
      break;
    } else if (fival.type == V4L2_FRMIVAL_TYPE_STEPWISE) {
      printf("{min { %u/%u } .. max { %u/%u } / "
        "stepsize { %u/%u } }, ",
        fival.stepwise.min.numerator, fival.stepwise.min.denominator,
        fival.stepwise.max.numerator, fival.stepwise.max.denominator,
        fival.stepwise.step.numerator, fival.stepwise.step.denominator);
      break;
    }
    fival.index++;
  }
  printf("\n");
  if (ret != 0 && errno != EINVAL) {
    printf("ERROR enumerating frame intervals: %d\n", errno);
    return errno;
  }
  
  return 0;
}
#endif

int capv4l_open(capdev* s0, int wIndex, int width, int height) {
  char dev_name[MAX_DEVICE_DRIVER_NAME] = "/dev/video0";
  int fps = 30;
  struct v4l2_capability cap = {{0}};
  enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  struct v4l2_cropcap cropcap = {0};
  struct v4l2_streamparm parms = {0};
  int fd=-1;

  if (NULL==s0) {
    return capv4l_capnum();
  }
  for (; wIndex<16; ++wIndex) {
    _snprintf(dev_name, MAX_DEVICE_DRIVER_NAME, "/dev/video%d", wIndex);
    fd = open(dev_name, O_RDWR /* required */ | O_NONBLOCK, 0);//打开设备
    if (fd>=0) {
      break;
    }
  }
  if(fd<0) {
    perror("camera_open：");
    return 0;
  }
  printf("dev_name=%s fd=%d\n", dev_name, fd);
  if (1) {
    struct v4l2_fmtdesc fmtdesc;
    //struct v4l2_frmsizeenum frmsize;
    bzero(&fmtdesc,sizeof(fmtdesc));  
    fmtdesc.index = 0;//从0 开始枚举  
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;//视频捕获设备  
    while (0 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc)) {
      printf("fmtdesc: %s\n",fmtdesc.description);
      //enum_frame_intervals(fd, fmtdesc.pixelformat, 0, 0);
      fmtdesc.index++;
    }
  }
  if (0) {
    struct v4l2_input inp = {0};
    inp.index = 0;
    if (inp.type == V4L2_INPUT_TYPE_CAMERA)
      printf("enuminput type is V4L2_INPUT_TYPE_CAMERA!\n");
    
    if (-1 == ioctl(fd, VIDIOC_S_INPUT, &inp))	{//设置输入index
      printf("VIDIOC_S_INPUT error!\n");
    }
  }
  //		printf("%s %d\n",__FILE__,__LINE__);
  capv4l* s = NULL;
  if (1) {
    struct v4l2_format fmt = {0};
    memset(&fmt, 0, sizeof(fmt));
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = width; //320;
    fmt.fmt.pix.height      = height; //240;
    //V4L2_PIX_FMT_YUV422P;//V4L2_PIX_FMT_NV12;//V4L2_PIX_FMT_YUYV; V4L2_PIX_FMT_NV16
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV; // 1.016321/100 frame
    fmt.fmt.pix.field       = V4L2_FIELD_NONE;
    //printf("%d %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    //设置图像格式
    if (-1 == ioctl(fd, VIDIOC_S_FMT, &fmt)) {
      printf("VIDIOC_S_FMT error!\n");
      close(fd);
      return 0;
      //goto close;
    }
    
    //		printf("%s %d\n",__FILE__,__LINE__);
#if 0
    printf("**************************************************************\n");
    printf("fmt.type = %d\n",fmt.type);
    printf("fmt.fmt.pix.width = %d\n",fmt.fmt.pix.width);
    printf("fmt.fmt.pix.height = %d\n",fmt.fmt.pix.height);
    printf("fmt.fmt.pix.pixelformat = %x\n",fmt.fmt.pix.pixelformat);
    printf("fmt.fmt.pix.field = %d\n",fmt.fmt.pix.field);
#endif
    //Test VIDIOC_G_FMT
    if (-1 == ioctl(fd, VIDIOC_G_FMT, &fmt)) //获取图像格式
    {
      printf("VIDIOC_G_FMT error!\n");
      close(fd);
      return 0;
    }
    else
    {
      printf("resolution got from sensor = %d*%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);
    }

    s = (capv4l*)pmalloc(sizeof(*s));
    memset(s, 0, sizeof(*s));
    s->colortype = T_YUYV;
    //printf("%d %d\n", fmt.fmt.pix.width, fmt.fmt.pix.height);
    s->w = fmt.fmt.pix.width;
    s->h = fmt.fmt.pix.height;
    
#if 1
    {
      char buf[8] = {0};
      *(int*)buf = fmt.fmt.pix.pixelformat;
      printf("**************************************************************\n");
      printf("fmt.fmt.pix.width = %d\n",fmt.fmt.pix.width);
      printf("fmt.fmt.pix.height = %d\n",fmt.fmt.pix.height);
      printf("fmt.fmt.pix.pixelformat = %x %s\n",fmt.fmt.pix.pixelformat, buf);
      printf("fmt.fmt.pix.field = %d\n",fmt.fmt.pix.field);
      printf("fmt.fmt.pix.bytesperline = %d\n",fmt.fmt.pix.bytesperline);
      printf("fmt.fmt.pix.sizeimage = %d\n",fmt.fmt.pix.sizeimage);
    }
#endif
  }
  if (1) {
    parms.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    parms.parm.capture.timeperframe.denominator = 1;
    parms.parm.capture.timeperframe.denominator = fps;
    
    if (-1 == ioctl(fd, VIDIOC_S_PARM, &parms)) {//获取帧率
      printf ("VIDIOC_S_PARM error\n");
      close(fd);
      return 0;
    }
  }
  //count=read_num;
  
  struct v4l2_requestbuffers req;
  memset(&req, 0, sizeof(req));
  req.count  = MAX_V4L_BUFFERS;
  req.type   = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  ioctl(fd, VIDIOC_REQBUFS, &req); //申请缓冲，count是申请的数量
  int n_buffers;
  for (n_buffers = 0; n_buffers < (int)req.count; ++n_buffers)
  {
    struct v4l2_buffer buf;   //驱动中的一帧
    BZERO1(&buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;
    
    if (-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf)) {//映射用户空间
      printf ("VIDIOC_QUERYBUF error\n");
    }
    
    s->buffers[n_buffers].len = buf.length;
    //通过mmap建立映射关系
    s->buffers[n_buffers].data = (uchar*)mmap(NULL /* start anywhere */,
      buf.length,
      PROT_READ | PROT_WRITE /* required */,
      MAP_SHARED /* recommended */,
      fd, buf.m.offset);
    
    if (MAP_FAILED == s->buffers[n_buffers].data) {
      printf ("mmap failed\n");
    }
  }
  int i;
  for (i = 0; i < n_buffers; ++i) 
  {
    struct v4l2_buffer buf;
    BZERO1(&buf);
    
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = i;
    
    if (-1 == ioctl(fd, VIDIOC_QBUF, &buf)) {//申请到的缓冲进入列队
      printf ("VIDIOC_QBUF failed\n");
      close(fd);
      return 0;
    }
  }

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  
  if (-1 == ioctl(fd, VIDIOC_STREAMON, &type)) //开始捕捉图像数据
    printf ("VIDIOC_STREAMON failed\n");
  else 
    printf ("VIDIOC_STREAMON ok\n");
  
  s0->x = s;
  s0->getframe = capv4l_getframe;
  s0->close = capv4l_close;
  s->fd=fd;
  return wIndex+1;
}

#endif // _CAPV4L_INL_
