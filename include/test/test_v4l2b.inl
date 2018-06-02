#ifndef __G2D_H__
#define __G2D_H__


#ifdef __cplusplus
extern "C"  {
#endif
  
  
  enum g2d_format
  {
    //rgb formats
    G2D_RGB565               = 0,
      G2D_RGBA8888             = 1,
      G2D_RGBX8888             = 2,
      G2D_BGRA8888             = 3,
      G2D_BGRX8888             = 4,
      G2D_BGR565               = 5,
      
      
      //yuv formats
      G2D_NV12                 = 20,
      G2D_I420                 = 21,
      G2D_YV12                 = 22,
      G2D_NV21                 = 23,
      G2D_YUYV                 = 24,
      G2D_YVYU                 = 25,
      G2D_UYVY                 = 26,
      G2D_VYUY                 = 27,
      G2D_NV16                 = 28,
      G2D_NV61                 = 29,
  };
  
  
  enum g2d_blend_func
  {
    G2D_ZERO                  = 0,
      G2D_ONE                   = 1,
      G2D_SRC_ALPHA             = 2,
      G2D_ONE_MINUS_SRC_ALPHA   = 3,
      G2D_DST_ALPHA             = 4,
      G2D_ONE_MINUS_DST_ALPHA   = 5,
  };
  
  
  enum g2d_cap_mode
  {
    G2D_BLEND                 = 0,
      G2D_DITHER                = 1,
      G2D_GLOBAL_ALPHA          = 2,//only support source global alpha
  };
  
  
  enum g2d_rotation
  {
    G2D_ROTATION_0            = 0,
      G2D_ROTATION_90           = 1,
      G2D_ROTATION_180          = 2,
      G2D_ROTATION_270          = 3,
      G2D_FLIP_H                = 4,
      G2D_FLIP_V                = 5,
  };
  
  
  enum g2d_cache_mode
  {
    G2D_CACHE_CLEAN           = 0,
      G2D_CACHE_FLUSH           = 1,
      G2D_CACHE_INVALIDATE      = 2,
  };
  
  
  enum g2d_hardware_type
  {
    G2D_HARDWARE_2D           = 0,//default type
      G2D_HARDWARE_VG           = 1,
  };
  
  
  struct g2d_surface
  {
    enum g2d_format format;
    
    
    int planes[3];//surface buffer addresses are set in physical planes separately
    //RGB:  planes[0] - RGB565/RGBA8888/RGBX8888/BGRA8888/BRGX8888
    //NV12: planes[0] - Y, planes[1] - packed UV
    //I420: planes[0] - Y, planes[1] - U, planes[2] - V
    //YV12: planes[0] - Y, planes[1] - V, planes[2] - U
    //NV21: planes[0] - Y, planes[1] - packed VU
    //YUYV: planes[0] - packed YUYV
    //YVYU: planes[0] - packed YVYU
    //UYVY: planes[0] - packed UYVY
    //VYUY: planes[0] - packed VYUY
    //NV16: planes[0] - Y, planes[1] - packed UV
    //NV61: planes[0] - Y, planes[1] - packed VU
    
    
    //blit rectangle in surface
    int left;
    int top;
    int right;
    int bottom;
    
    
    int stride;//surface buffer stride
    
    
    int width;//surface width
    int height;//surface height
    
    
    //alpha blending parameters
    enum g2d_blend_func blendfunc;
    
    
    //the global alpha value is 0 ~ 255
    int global_alpha;
    
    
    //clrcolor format is RGBA8888
    int clrcolor;
    
    
    //rotation degree
    enum g2d_rotation rot;
  };
  
  
  struct g2d_buf
  {
    void *buf_handle;
    void *buf_vaddr;
    int  buf_paddr;
    int  buf_size;
  };
  
  
  int g2d_open(void **handle);
  int g2d_close(void *handle);
  
  
  int g2d_make_current(void *handle, enum g2d_hardware_type type);
  
  
  int g2d_clear(void *handle, struct g2d_surface *area);
  int g2d_blit(void *handle, struct g2d_surface *src, struct g2d_surface *dst);
  int g2d_copy(void *handle, struct g2d_buf *d, struct g2d_buf* s, int size);
  
  
  int g2d_query_cap(void *handle, enum g2d_cap_mode cap, int *enable);
  int g2d_enable(void *handle, enum g2d_cap_mode cap);
  int g2d_disable(void *handle, enum g2d_cap_mode cap);
  
  
  int g2d_cache_op(struct g2d_buf *buf, enum g2d_cache_mode op);
  struct g2d_buf *g2d_alloc(int size, int cacheable);
  int g2d_free(struct g2d_buf *buf);
  
  
  int g2d_flush(void *handle);
  int g2d_finish(void *handle);
  
  
#ifdef __cplusplus
}
#endif


#endif





//2.mxc_v4l2_tvin.c


/*
* Copyright 2007-2013 Freescale Semiconductor, Inc. All rights reserved.
*/


/*
* The code contained herein is licensed under the GNU General Public
* License. You may obtain a copy of the GNU General Public License
* Version 2 or later at the following locations:
*
* http://www.opensource.org/licenses/gpl-license.html
* http://www.gnu.org/copyleft/gpl.html
*/


/*
* @file mxc_v4l2_tvin.c
*
* @brief Mxc TVIN For Linux 2 driver test application
*
*/


#ifdef __cplusplus
extern "C"{
#endif
  
/*
* Copyright 2007-2013 Freescale Semiconductor, Inc. All rights reserved.
  */
  
  
  /*
  * The code contained herein is licensed under the GNU General Public
  * License. You may obtain a copy of the GNU General Public License
  * Version 2 or later at the following locations:
  *
  * http://www.opensource.org/licenses/gpl-license.html
  * http://www.gnu.org/copyleft/gpl.html
  */
  
  
  /*
  * @file mxc_v4l2_tvin.c
  *
  * @brief Mxc TVIN For Linux 2 driver test application
  *
  */
  
  
#ifdef __cplusplus
  extern "C"{
#endif
    
    
  /*=======================================================================
  INCLUDE FILES
    =======================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <math.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>
    
    
#include <linux/mxcfb.h>
#include <linux/mxc_v4l2.h>
#include <linux/ipu.h>
    
    
    //#include "g2d.h"
    
    
#define TFAIL -1
#define TPASS 0
    
    
#define NUMBER_BUFFERS    4
#define G2D_CACHEABLE    0
    char v4l_capture_dev[100] = "/dev/video0";
    int fd_capture_v4l = 0;
    char fb_dev[100] = "/dev/fb0";
    int fd_fb = 0;
    int fd_ipu = 0;
    int g_fb_phys;
    unsigned short * g_fb;
    int g_fb_size;
    struct fb_var_screeninfo g_screen_info;
    int g_num_buffers = NUMBER_BUFFERS;
    int g_fmt = V4L2_PIX_FMT_UYVY;
    int g_rotate = 0;
    int g_vdi_enable = 0;
    int g_vdi_motion = 0;
    int g_tb = 0;
    int g_in_width = 720;
    int g_in_height = 480;
    int g_display_width0 = 720;
    int g_display_height0 = 480;
    int g_display_top0 = 0;
    int g_display_left0 = 0;
    int g_display_width1 = 720;
    int g_display_height1 = 480;
    int g_display_top1 = 0;
    int g_display_left1 = 0;
    int g_display_width2 = 720;
    int g_display_height2 = 480;
    int g_display_top2 = 0;
    int g_display_left2 = 0;
    int g_display_width3 = 720;
    int g_display_height3 = 480;
    int g_display_top3 = 0;
    int g_display_left3 = 0;
    int g_display_width4 = 720;
    int g_display_height4 = 480;
    int g_display_top4 = 0;
    int g_display_left4 = 0;
    
    
    int g_frame_size;
    int g_g2d_fmt;
    int g_mem_type = V4L2_MEMORY_MMAP;
    v4l2_std_id g_current_std = V4L2_STD_NTSC;
    
    
    typedef struct
    {
      char s[64];
    }EP_str;
    int channel = 0;
    
    
    struct testbuffer
    {
      unsigned char *start;
      size_t offset;
      unsigned int length;
    };
    
    
    struct testbuffer capture_buffers[NUMBER_BUFFERS];
    struct g2d_buf *g2d_buffers[NUMBER_BUFFERS];
    
    
    
    
    static int parse_str(unsigned char *s, EP_str *d)
    {
      int num = 0;
      int i = 0, t = 0;
      while (s[t] != 0)
      {
        i = 0;
        while (s[t] != 0)
        {
          if (s[t] == ',')
          {
            d[num].s[i] = 0;
            num++;
            t++;
            break;
          }
          d[num].s[i++] = s[t++];
          d[num].s[i]   = 0;
          
        }
      }
      return num;
    }
    
    
    static int parse_num(EP_str *items, int num)
    {
      if (num < 20)
        return -1;
      g_display_top0 = atol(items[0].s);
      g_display_left0 = atol(items[1].s);
      g_display_width0 = atol(items[2].s);
      g_display_height0 = atol(items[3].s);
      
      
      g_display_top1 = atol(items[4].s);
      g_display_left1 = atol(items[5].s);
      g_display_width1 = atol(items[6].s);
      g_display_height1 = atol(items[7].s);
      
      
      g_display_top2 = atol(items[8].s);
      g_display_left2 = atol(items[9].s);
      g_display_width2 = atol(items[10].s);
      g_display_height2 = atol(items[11].s);
      
      
      g_display_top3 = atol(items[12].s);
      g_display_left3 = atol(items[13].s);
      g_display_width3 = atol(items[14].s);
      g_display_height3 = atol(items[15].s);
      
      
      g_display_top4 = atol(items[16].s);
      g_display_left4 = atol(items[17].s);
      g_display_width4 = atol(items[18].s);
      g_display_height4 = atol(items[19].s);
      
      
      
      
      
      
      return 1;
    }
    
    
    
    
    
    
    static void draw_image_to_framebuffer(struct g2d_buf *buf, int img_width, int img_height, int img_format, 
      struct fb_var_screeninfo *screen_info, int left, int top, int to_width, int to_height, int set_alpha, int rotation)
    {
      int i;
      struct g2d_surface src,dst;
      void *g2dHandle;
      
      
      if ( ( (left+to_width) > (int)screen_info->xres ) || ( (top+to_height) > (int)screen_info->yres ) )  {
        printf("Bad display image dimensions!\n");
        return;
      }
      
      
#if CACHEABLE
      g2d_cache_op(buf, G2D_CACHE_FLUSH);
#endif
      
      
      if(g2d_open(&g2dHandle) == -1 || g2dHandle == NULL) {
        printf("Fail to open g2d device!\n");
        g2d_free(buf);
        return;
      }
      
      
      /*
      NOTE: in this example, all the test image data meet with the alignment requirement.
      Thus, in your code, you need to pay attention on that.
      
        
          Pixel buffer address alignment requirement,
          RGB/BGR:  pixel data in planes [0] with 16bytes alignment,
          NV12/NV16:  Y in planes [0], UV in planes [1], with 64bytes alignment,
          I420:    Y in planes [0], U in planes [1], V in planes [2], with 64 bytes alignment,
          YV12:  Y in planes [0], V in planes [1], U in planes [2], with 64 bytes alignment,
          NV21/NV61:  Y in planes [0], VU in planes [1], with 64bytes alignment,
          YUYV/YVYU/UYVY/VYUY:  in planes[0], buffer address is with 16bytes alignment.
      */
      
      
      src.format = img_format;
      switch (src.format) {
      case G2D_RGB565:
      case G2D_RGBA8888:
      case G2D_RGBX8888:
      case G2D_BGRA8888:
      case G2D_BGRX8888:
      case G2D_BGR565:
      case G2D_YUYV:
      case G2D_UYVY:
        src.planes[0] = buf->buf_paddr;
        break;
      case G2D_NV12:
        src.planes[0] = buf->buf_paddr;
        src.planes[1] = buf->buf_paddr + img_width * img_height;
        break;
      case G2D_I420:
        src.planes[0] = buf->buf_paddr;
        src.planes[1] = buf->buf_paddr + img_width * img_height;
        src.planes[2] = src.planes[1]  + img_width * img_height / 4;
        break;
      case G2D_NV16:
        src.planes[0] = buf->buf_paddr;
        src.planes[1] = buf->buf_paddr + img_width * img_height;
        break;
      default:
        printf("Unsupport image format in the example code\n");
        return;
      }
      
      
      src.left = 0;
      src.top = 0;
      src.right = img_width;
      src.bottom = img_height;
      src.stride = img_width;
      src.width  = img_width;
      src.height = img_height;
      src.rot  = G2D_ROTATION_0;
      
      
      dst.planes[0] = g_fb_phys;
      dst.left = left;
      dst.top = top;
      dst.right = left + to_width;
      dst.bottom = top + to_height;
      dst.stride = screen_info->xres;
      dst.width  = screen_info->xres;
      dst.height = screen_info->yres;
      dst.rot    = rotation;
      dst.format = screen_info->bits_per_pixel == 16 ? G2D_RGB565 : (screen_info->red.offset == 0 ? G2D_RGBA8888 : G2D_BGRA8888);
      
      
      if (set_alpha)
      {
        src.blendfunc = G2D_ONE;
        dst.blendfunc = G2D_ONE_MINUS_SRC_ALPHA;
        
        src.global_alpha = 0x80;
        dst.global_alpha = 0xff;
        
        g2d_enable(g2dHandle, G2D_BLEND);
        g2d_enable(g2dHandle, G2D_GLOBAL_ALPHA);
      }
      
      
      g2d_blit(g2dHandle, &src, &dst);
      g2d_finish(g2dHandle);
      
      
      if (set_alpha)
      {
        g2d_disable(g2dHandle, G2D_GLOBAL_ALPHA);
        g2d_disable(g2dHandle, G2D_BLEND);
      }
      
      
      g2d_close(g2dHandle);
}


int start_capturing(void)
{
  unsigned int i;
  struct v4l2_buffer buf;
  enum v4l2_buf_type type;
  
  
  for (i = 0; i < g_num_buffers; i++)
  {
    memset(&buf, 0, sizeof (buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = g_mem_type;
    buf.index = i;
    if (g_mem_type == V4L2_MEMORY_USERPTR) {
      buf.length = capture_buffers[i].length;
      // buf.m.userptr = (unsigned long) capture_buffers[i].offset;
      buf.m.userptr = (unsigned long) capture_buffers[i].start;
    }
    if (ioctl(fd_capture_v4l, VIDIOC_QUERYBUF, &buf) < 0) //step 8 获取缓冲帧的地址，长度
    {
      printf("VIDIOC_QUERYBUF error\n");
      return TFAIL;
    }
    
    
    if (g_mem_type == V4L2_MEMORY_MMAP) {
      capture_buffers[i].length = buf.length;
      capture_buffers[i].offset = (size_t) buf.m.offset;
      capture_buffers[i].start = mmap (NULL, capture_buffers[i].length,
        PROT_READ | PROT_WRITE, MAP_SHARED,
        fd_capture_v4l, capture_buffers[i].offset);
      memset(capture_buffers[i].start, 0xFF, capture_buffers[i].length);
    }
    //printf("capture_buffers[%d].offset = 0x%x.\r\n", i, capture_buffers[i].offset);
    //printf("capture_buffers[%d].start = 0x%x.\r\n", i, capture_buffers[i].start);
  }
  
  
  for (i = 0; i < g_num_buffers; i++)
  {
    memset(&buf, 0, sizeof (buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = g_mem_type;
    buf.index = i;
    if (g_mem_type == V4L2_MEMORY_USERPTR)
      buf.m.offset = (unsigned int)capture_buffers[i].start;
    else
      buf.m.offset = capture_buffers[i].offset;
    buf.length = capture_buffers[i].length;
    if (ioctl (fd_capture_v4l, VIDIOC_QBUF, &buf) < 0) { //step 9   把帧放入队列
      printf("VIDIOC_QBUF error\n");
      return TFAIL;
    }
  }
  
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl (fd_capture_v4l, VIDIOC_STREAMON, &type) < 0) {  //step 10 启动数据流
    printf("VIDIOC_STREAMON error\n");
    return TFAIL;
  }
  return 0;
}


int prepare_g2d(void)
{
  int i;
  
  
  for (i = 0; i < g_num_buffers; i++) {
#if CACHEABLE
    g2d_buffers[i] = g2d_alloc(g_frame_size, 1);//alloc physical contiguous memory for source image data with cacheable attribute
#else
    g2d_buffers[i] = g2d_alloc(g_frame_size, 0);//alloc physical contiguous memory for source image data
#endif
    if(!g2d_buffers[i]) {
      printf("Fail to allocate physical memory for image buffer!\n");
      return TFAIL;
    }
    //printf("g2d_buffers[%d].buf_paddr = 0x%x.\r\n", i, g2d_buffers[i]->buf_paddr);
  }
  
  
  return 0;
}


void memfree(int buf_size, int buf_cnt)
{
  int i;
  unsigned int page_size;
  
  
  page_size = getpagesize();
  buf_size = (buf_size + page_size - 1) & ~(page_size - 1);
  
  
  for (i = 0; i < buf_cnt; i++) {
    if (capture_buffers[i].start)
      munmap(capture_buffers[i].start, buf_size);
    if (capture_buffers[i].offset)
      ioctl(fd_ipu, IPU_FREE, &capture_buffers[i].offset);
  }
}


int memalloc(int buf_size, int buf_cnt)
{
  int i, ret = TPASS;
  unsigned int page_size;
  
  
  for (i = 0; i < buf_cnt; i++) {
    page_size = getpagesize();
    buf_size = (buf_size + page_size - 1) & ~(page_size - 1);
    capture_buffers[i].length = capture_buffers[i].offset = buf_size;
    ret = ioctl(fd_ipu, IPU_ALLOC, &capture_buffers[i].offset);
    if (ret < 0) {
      printf("ioctl IPU_ALLOC fail\n");
      ret = TFAIL;
      goto err;
    }
    capture_buffers[i].start = mmap(0, buf_size, PROT_READ | PROT_WRITE,
      MAP_SHARED, fd_ipu, capture_buffers[i].offset);
    if (!capture_buffers[i].start) {
      printf("mmap fail\n");
      ret = TFAIL;
      goto err;
    }
    //printf("USRP: alloc bufs offset 0x%x, start 0x%x, size %d\r\n", capture_buffers[i].offset, capture_buffers[i].start, buf_size);
  }
  
  
  return ret;
err:
  memfree(buf_size, buf_cnt);
  return ret;
}


int v4l_capture_setup(void)
{
  struct v4l2_input input;
  struct v4l2_fmtdesc fmtdesc;
  struct v4l2_capability cap;
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  v4l2_std_id id;
  unsigned int min, i;
  
  
  if (ioctl (fd_capture_v4l, VIDIOC_QUERYCAP, &cap) < 0) {// step 2 查询设备属性
    if (EINVAL == errno) {
      fprintf (stderr, "%s is no V4L2 device\n",
        v4l_capture_dev);
      return TFAIL;
    } else {
      fprintf (stderr, "%s isn not V4L device,unknow error\n",
        v4l_capture_dev);
      return TFAIL;
    }
  }
  
  
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf (stderr, "%s is no video capture device\n",
      v4l_capture_dev);
    return TFAIL;
  }
  
  
  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    fprintf (stderr, "%s does not support streaming i/o\n",
      v4l_capture_dev);
    return TFAIL;
  }
  
  
  input.index = 0;
  while (ioctl(fd_capture_v4l, VIDIOC_ENUMINPUT, &input) >= 0)  //step 3 列举video input信息 总共有多少路
  {
    input.index += 1;
  }
  
  
  if (ioctl(fd_capture_v4l, VIDIOC_G_STD, &id) < 0) //step 4  获得了当前输入使用的standard 是NTSC格式还是PAL格式
  {
    printf("VIDIOC_G_STD failed\n");
    close(fd_capture_v4l);
    return TFAIL;
  }
  g_current_std = id;
  
  
  fmtdesc.index = 0;
  while (ioctl(fd_capture_v4l, VIDIOC_ENUM_FMT, &fmtdesc) >= 0) //step 5  查询并显示所有支持的格式
  {
    fmtdesc.index += 1;
  }
  
  
  /* Select video input, video standard and tune here. */
  memset(&fmt, 0, sizeof(fmt));
  
  
  fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = g_in_width;
  fmt.fmt.pix.height      = g_in_height;
  fmt.fmt.pix.pixelformat = g_fmt;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
  
  
  if (ioctl (fd_capture_v4l, VIDIOC_S_FMT, &fmt) < 0){ //step 6 设置当前视频格式
    fprintf (stderr, "%s iformat not supported \n",
      v4l_capture_dev);
    return TFAIL;
  }
  
  
  /* Note VIDIOC_S_FMT may change width and height. */
  
  
  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 2;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  
  
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;
  
  
  if (ioctl(fd_capture_v4l, VIDIOC_G_FMT, &fmt) < 0)  //step 6 查询当前视频格式
  {
    printf("VIDIOC_G_FMT failed\n");
    close(fd_capture_v4l);
    return TFAIL;
  }
  
  
  g_in_width = fmt.fmt.pix.width;
  g_in_height = fmt.fmt.pix.height;
  
  
  printf("VIDIOC_G_FMT failed\n");
  
  
  memset(&req, 0, sizeof (req));
  
  
  req.count = g_num_buffers;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = g_mem_type;
  
  
  if (ioctl (fd_capture_v4l, VIDIOC_REQBUFS, &req) < 0) {  //step 7 向设备申请缓冲区
    if (EINVAL == errno) {
      fprintf (stderr, "%s does not support "
        "memory mapping\n", v4l_capture_dev);
      return TFAIL;
    } else {
      fprintf (stderr, "%s does not support "
        "memory mapping, unknow error\n", v4l_capture_dev);
      return TFAIL;
    }
  }
  
  
  if (req.count < 2) {
    fprintf (stderr, "Insufficient buffer memory on %s\n",
      v4l_capture_dev);
    return TFAIL;
  }
  
  
  return 0;
}


int mxc_v4l_tvin_test(void)
{
  struct v4l2_buffer capture_buf;
  v4l2_std_id id;
  int i, j;
  enum v4l2_buf_type type;
  struct ipu_task task;
  int total_time;
  struct timeval tv_start, tv_current;
  char str[4096];
  char file_name[128] = "videocontrol.para";
  EP_str items[256];
  int num  = 0;
  FILE *fp = NULL;
  
  if (prepare_g2d() < 0) {
    printf("prepare_output failed\n");
    return TFAIL;
  }
  
  
  if (start_capturing() < 0) {
    printf("start_capturing failed\n");
    return TFAIL;
  }
  
  
  if (g_vdi_enable) {
    memset(&task, 0, sizeof(struct ipu_task));
    task.output.width = g_in_width;
    task.output.height = g_in_height;
    task.output.crop.w = g_in_width;
    task.output.crop.h = g_in_height;
    task.output.format = g_fmt;
    
    
    task.input.width  = g_in_width;
    task.input.height = g_in_height;
    task.input.crop.w = g_in_width;
    task.input.crop.h = g_in_height;
    task.input.format = g_fmt;
    task.input.deinterlace.enable = g_vdi_enable;
    task.input.deinterlace.motion = g_vdi_motion;
    
    
    task.input.paddr = capture_buffers[0].offset;
    task.output.paddr = g2d_buffers[0]->buf_paddr;
    
    
    if (ioctl(fd_ipu, IPU_CHECK_TASK, &task) != IPU_CHECK_OK) {
      printf("IPU_CHECK_TASK failed.\r\n");
      return TFAIL;
    }
  }
  
  
  gettimeofday(&tv_start, 0);
  printf("start time = %d s, %d us\n", (unsigned int) tv_start.tv_sec,
    (unsigned int) tv_start.tv_usec);
  
  
  for (i = 0; ; i++) 
    // for (i = 0; i<2000; i++) {
  {
    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
      
      
    }
    else
    {
      if (fgets(str, 4096, fp) == NULL)
      {
        
        
      }
      else
      {
        num = parse_str(str, items);
        parse_num(items,num);
      }
    }
    fclose(fp);
    fp = NULL; 
    
    
    
    
    memset(&capture_buf, 0, sizeof(capture_buf));
    capture_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    capture_buf.memory = g_mem_type;
    if (ioctl(fd_capture_v4l, VIDIOC_DQBUF, &capture_buf) < 0) { //step 11 从队列中取出帧
      printf("VIDIOC_DQBUF failed.\n");
      return TFAIL;
    }
    
    
    if (g_vdi_enable) {
      task.input.paddr = capture_buffers[capture_buf.index].offset;
      task.output.paddr = g2d_buffers[capture_buf.index]->buf_paddr;
      if ((task.input.paddr != 0) && (task.output.paddr != 0)) {
        if (ioctl(fd_ipu, IPU_QUEUE_TASK, &task) < 0) {
          printf("IPU_QUEUE_TASK failed\n");
          return TFAIL;
        }
      }
    } else
      memcpy(g2d_buffers[capture_buf.index]->buf_vaddr, capture_buffers[capture_buf.index].start, g_frame_size);
    
    
    if (ioctl(fd_capture_v4l, VIDIOC_QBUF, &capture_buf) < 0) { //step 12 把数据帧放入队列
      printf("VIDIOC_QBUF failed\n");
      return TFAIL;
    }
    switch(channel)
    {
    case 0:
      if(g_display_width0)
        draw_image_to_framebuffer(g2d_buffers[capture_buf.index], g_in_width, g_in_height, g_g2d_fmt, &g_screen_info, g_display_left0, g_display_top0, g_display_width0, g_display_height0, 0, G2D_ROTATION_0);
      break;
    case 1:
      if(g_display_width1)
        draw_image_to_framebuffer(g2d_buffers[capture_buf.index], g_in_width, g_in_height, g_g2d_fmt, &g_screen_info, g_display_left1, g_display_top1, g_display_width1, g_display_height1, 0, G2D_ROTATION_0);
      break;
    case 2:
      if(g_display_width2)
        draw_image_to_framebuffer(g2d_buffers[capture_buf.index], g_in_width, g_in_height, g_g2d_fmt, &g_screen_info, g_display_left2, g_display_top2, g_display_width2, g_display_height2, 0, G2D_ROTATION_0);
      break;
    case 3:
      if(g_display_width3)
        draw_image_to_framebuffer(g2d_buffers[capture_buf.index], g_in_width, g_in_height, g_g2d_fmt, &g_screen_info, g_display_left3, g_display_top3, g_display_width3, g_display_height3, 0, G2D_ROTATION_0);
      break;
    case 4:
      if(g_display_width4)
        draw_image_to_framebuffer(g2d_buffers[capture_buf.index], g_in_width, g_in_height, g_g2d_fmt, &g_screen_info, g_display_left4, g_display_top4, g_display_width4, g_display_height4, 0, G2D_ROTATION_0);
      break;
    }
  }
  gettimeofday(&tv_current, 0);
  total_time = (tv_current.tv_sec - tv_start.tv_sec) * 1000000L;
  total_time += tv_current.tv_usec - tv_start.tv_usec;
  printf("total time for %u frames = %u us =  %lld fps\n", i, total_time, (i * 1000000ULL) / total_time);
  
  
  return 0;
}


int process_cmdline(int argc, char **argv)
{
  int i, val;
  char node[8];
  
  
  for (i = 1; i < argc; i++) {
  /*if (strcmp(argv[i], "-ow") == 0) {
  g_display_width = atoi(argv[++i]);
  }
  else if (strcmp(argv[i], "-oh") == 0) {
  g_display_height = atoi(argv[++i]);
  }
  else if (strcmp(argv[i], "-ot") == 0) {
  g_display_top = atoi(argv[++i]);
  }
  else if (strcmp(argv[i], "-ol") == 0) {
  g_display_left = atoi(argv[++i]);
  }
    else */ if (strcmp(argv[i], "-iw") == 0) {
    g_in_width = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-ih") == 0) {
      g_in_height = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-r") == 0) {
      g_rotate = atoi(argv[++i]);
    }
    else if (strcmp(argv[i], "-x") == 0) {
      channel = atoi(argv[++i]);
      sprintf(node, "%d", channel);
      strcpy(v4l_capture_dev, "/dev/video");
      strcat(v4l_capture_dev, node);
    }
    else if (strcmp(argv[i], "-o") == 0) {
      val = atoi(argv[++i]);
      sprintf(node, "%d", val);
      strcpy(fb_dev, "/dev/fb");
      strcat(fb_dev, node);
    }
    else if (strcmp(argv[i], "-f") == 0) {
      i++;
      g_fmt = v4l2_fourcc(argv[i][0], argv[i][1],argv[i][2],argv[i][3]);
      if ((g_fmt != V4L2_PIX_FMT_NV12) &&
        (g_fmt != V4L2_PIX_FMT_UYVY) &&
        (g_fmt != V4L2_PIX_FMT_YUYV) &&
        (g_fmt != V4L2_PIX_FMT_YUV420))
      {
        g_fmt = V4L2_PIX_FMT_UYVY;
        printf("Default format is used: UYVY\n");
      }
    }
    else if (strcmp(argv[i], "-m") == 0) {
      g_vdi_enable = 1;
      g_vdi_motion = atoi(argv[++i]);
      g_mem_type = V4L2_MEMORY_USERPTR;
    }
    else if (strcmp(argv[i], "-tb") == 0) {
      g_tb = 1;
    }
    else if (strcmp(argv[i], "-help") == 0) {
      printf("MXC Video4Linux TVin Test\n\n" \
        "Syntax: mxc_v4l2_tvin.out\n" \
        " -iw <capture in width>\n" \
        " -ih <capture in height>\n" \
        " -ow <display width>\n" \
        " -oh <display height>\n" \
        " -ot <display top>\n" \
        " -ol <display left>\n" \
        " -x <capture device> 0 = /dev/video0; 1 = /dev/video1 ...>\n" \
        " -o <framebuffer device> 0 = /dev/fb0; 1 = /dev/fb1 ...>\n" \
        " -r <rotation> -c <capture counter> \n"
        " -m <motion> 0:medium 1:low 2:high, 0-default\n"
        " -tb top field first, bottom field first-default\n"
        " -f <format, only YU12, YUYV, UYVY and NV12 are supported> \n");
      return TFAIL;
    }
  }
  /*
  if ((g_display_width0 == 0) || (g_display_height0 == 0)) {
  printf("Zero display width or height\n");
  return TFAIL;
  }
  */
  switch (g_fmt) {
  case V4L2_PIX_FMT_UYVY:
    g_frame_size = g_in_width * g_in_height * 2;
    g_g2d_fmt = G2D_UYVY;
    break;
    
    
  case V4L2_PIX_FMT_YUYV:
    g_frame_size = g_in_width * g_in_height * 2;
    g_g2d_fmt = G2D_YUYV;
    break;
    
    
  case V4L2_PIX_FMT_YUV420:
    g_frame_size = g_in_width * g_in_height * 3 / 2;
    g_g2d_fmt = G2D_I420;
    break;
    
    
  case V4L2_PIX_FMT_NV12:
    g_frame_size = g_in_width * g_in_height * 3 / 2;
    g_g2d_fmt = G2D_NV12;
    break;
    
    
  default:
    printf("Unsupported format.\n");
    return TFAIL;
  }
  
  
  return 0;
}


int main(int argc, char **argv)
{
  int i, ret = TFAIL;
  enum v4l2_buf_type type;
  struct fb_fix_screeninfo fb_info;
  
  
  if (process_cmdline(argc, argv) < 0) {
    goto EXIT_0;
  }
  
  
  if ((fd_capture_v4l = open(v4l_capture_dev, O_RDWR, 0)) < 0) {
    //step 1 open video fd
    printf("Unable to open %s\n", v4l_capture_dev);
    goto EXIT_0;
  }
  if ((fd_ipu = open("/dev/mxc_ipu", O_RDWR, 0)) < 0) {
    printf("open ipu dev fail\n");
    goto EXIT_1;
  }
  
  
  if ((fd_fb = open(fb_dev, O_RDWR, 0)) < 0) {
    printf("Unable to open %s\n", fb_dev);
    goto EXIT_2;
  }
  
  
  /* Get fix screen info. */
  if ((ioctl(fd_fb, FBIOGET_FSCREENINFO, &fb_info)) < 0) {
    printf("%s FBIOGET_FSCREENINFO failed.\n", fb_dev);
    goto EXIT_3;
  }
  g_fb_phys = fb_info.smem_start;
  
  
  /* Get variable screen info. */
  if ((ioctl(fd_fb, FBIOGET_VSCREENINFO, &g_screen_info)) < 0) {
    printf("%s FBIOGET_VSCREENINFO failed.\n", fb_dev);
    goto EXIT_3;
  }
  
  
  g_fb_phys += (g_screen_info.xres_virtual * g_screen_info.yoffset * g_screen_info.bits_per_pixel / 8);
  
  
  g_fb_size = g_screen_info.xres_virtual * g_screen_info.yres_virtual * g_screen_info.bits_per_pixel / 8;
  
  
  /* Map the device to memory*/
  g_fb = (unsigned short *) mmap(0, g_fb_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
  if ((int)g_fb <= 0) {
    printf("\nError: failed to map framebuffer device to memory.\n");
    goto EXIT_3;
  }
  
  
  if (g_mem_type == V4L2_MEMORY_USERPTR)
    if (memalloc(g_frame_size, g_num_buffers) < 0)
      goto EXIT_4;
    
    
    if (v4l_capture_setup() < 0) {
      printf("Setup v4l capture failed.\n");
      goto EXIT_4;
    }
    
    
    ret = mxc_v4l_tvin_test();
    
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(fd_capture_v4l, VIDIOC_STREAMOFF, &type); //step 12 关闭数据流
    
    
    for (i = 0; i < g_num_buffers; i++)
    {
      g2d_free(g2d_buffers[i]);
    }
    
    
    if (g_mem_type == V4L2_MEMORY_USERPTR) {
      memfree(g_frame_size, g_num_buffers);
    } else {
      for (i = 0; i < g_num_buffers; i++)
      {
        munmap(capture_buffers[i].start, capture_buffers[i].length);
      }
    }
    
    
EXIT_4:
    munmap(g_fb, g_fb_size);
    
    
EXIT_3:
    close(fd_fb);
EXIT_2:
    close(fd_ipu);
EXIT_1:
    close(fd_capture_v4l);
EXIT_0:
    
    
    return ret;
}


