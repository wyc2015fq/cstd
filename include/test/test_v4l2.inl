
#include <stdio.h>
#include <stdlib.h> //stdio.h and stdlib.h are needed by perror function
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> //O_RDWR
#include <unistd.h>
#include <sys/mman.h> //unistd.h and sys/mman.h are needed by mmap function
#include <stdbool.h>//false and true
#include <sys/ioctl.h>
#include <linux/videodev2.h>//v4l2 API

typedef struct _v4l2_struct
{
  int fd;
  struct v4l2_capability capability;
}v4l2_device;

int v4l2_close(v4l2_device *vd)
{
  printf("v4l2_close\n");
  close(vd->fd);
  return 0;
}

int v4l2_get_capability(v4l2_device *vd)
{
  if(ioctl(vd->fd,VIDIOC_QUERYCAP,&vd->capability)<0) {
    perror("v4l2_get_capability fail");
    return -1;
  }
  printf("v4l2_get_capability ok\n");
  return 0; 
}

#define DEFAULT_DEVICE "/dev/video0"
int v4l2_open(char *dev,v4l2_device *vd)
{
  if(!dev)
    dev=DEFAULT_DEVICE;
  if((vd->fd=open(dev,O_RDWR))<0)
  {
    perror("v4l2_open fail");
    return -1;
  }
  if(v4l2_get_capability(vd))
    return -1;
  printf("v4l2_open ok\n");
  //printf("video capture device name:%s\n",vd->capability.driver);
  struct v4l2_capability* cap = &vd->capability;
  printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",
    cap->driver,cap->card,cap->bus_info,(cap->version>>16)&0XFF, (cap->version>>8)&0XFF,cap->version&0XFF);
  return 0;
}

void test_v4l2()
{
  v4l2_device vd[1] = {0};
  printf("1111 1\n");
  v4l2_open(DEFAULT_DEVICE,vd);
  if (0) {
    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format:\n");
    
    while(ioctl(vd->fd, VIDIOC_ENUM_FMT, &fmtdesc) != -1)
    {
      printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
      fmtdesc.index++;
    }
  }
  {
    struct v4l2_format fmt;
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(vd->fd, VIDIOC_G_FMT, &fmt);
    
    printf("Current data format information:\n\twidth:%d\n\theight:%d\n",
      fmt.fmt.pix.width,fmt.fmt.pix.height);

    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    
    while(ioctl(vd->fd, VIDIOC_ENUM_FMT,&fmtdesc)!=-1) {
      if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat) {
        printf("\tformat:%s\n",fmtdesc.description);
        break;
      }
      fmtdesc.index++;
    }
  }
  if (0) {
    unsigned int i;
    enum v4l2_buf_type type;
    for (i = 0; i < 4; ++i) // 将缓冲帧放入队列
    {
      struct v4l2_buffer buf;
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.memory = V4L2_MEMORY_MMAP;
      buf.index = i;
      ioctl (vd->fd, VIDIOC_QBUF, &buf);
    }
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl (vd->fd, VIDIOC_STREAMON, &type);
  }
  if (1) {
    struct v4l2_buffer buf = {0};
    //CLEAR (buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == ioctl (vd->fd, VIDIOC_DQBUF, &buf)) { // 从缓冲区取出一个缓冲帧
      perror ("VIDIOC_DQBUF FAILURE) %d", errno);
    }
    //process_image (buffers[buf.index.]start);
    printf("buf %d\n", buf.length);
    if (-1 == ioctl (vd->fd, VIDIOC_QBUF, &buf)) {
      perror ("VIDIOC_DQBUF FAILURE)");
    }
    printf("buf %d\n", buf.length);
  }
  if (0) {
    struct v4l2_input input;
    struct v4l2_standard standard;
    memset (&input, 0, sizeof (input));
    //首先获得当前输入的 index,注意只是 index，要获得具体的信息，就的调用列举操作
    if (-1 == ioctl (vd->fd, VIDIOC_G_INPUT, &input.index)) {
      perror ("VIDIOC_G_INPUT FAILURE)");
      exit (EXIT_FAILURE);
    }
    
    //调用列举操作，获得 input.index 对应的输入的具体信息
    if (-1 == ioctl (vd->fd, VIDIOC_ENUMINPUT, &input)) {
      perror ("VIDIOC_ENUM_INPUT");
      exit (EXIT_FAILURE);
    }
    
    printf ("Current input %s supports:\n", input.name); memset (&standard, 0, sizeof (standard)); standard.index = 0;
    
    //列举所有的所支持的 standard，如果 standard.id 与当前 input 的 input.std 有共同的
    //bit flag，意味着当前的输入支持这个 standard,这样将所有驱动所支持的 standard 列举一个
    //遍，就可以找到该输入所支持的所有 standard 了。
    while (0 == ioctl (vd->fd, VIDIOC_ENUMSTD, &standard)) {
      if (standard.id & input.std) {
        printf ("%s\n", standard.name);
      }
      standard.index++;
    }
    
    /* EINVAL indicates the end of the enumeration, which cannot be empty unless this device falls under the USB exception. */
    
    if (standard.index == 0) {
      perror ("VIDIOC_ENUMSTD");
      exit (EXIT_FAILURE);
    }
  }
  //v4l2_close(vd);
  printf("3333 1\n");
}
