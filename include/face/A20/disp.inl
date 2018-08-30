
int disp_int(int w,int h)
{
  /*display start*/ 
  //unsigned int h,w;
  __u32 id = 0;
  
  //h= 480;
  //w= 640;
  
  if((disphd = open("/dev/disp",O_RDWR)) == -1)
  {
    printf("open file /dev/disp fail. \n");
    return 0;
  }
  
  arg[0] = 0;
  ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);
  
  //layer0
  arg[0] = 0;
  arg[1] = DISP_LAYER_WORK_MODE_SCALER;
  hlay = ioctl(disphd, DISP_CMD_LAYER_REQUEST, (void*)arg);
  if(hlay == 0)
  {
    printf("request layer0 fail\n");
    return 0;
  }
  printf("video layer hdl:%d\n", hlay);
  
  layer_para.mode = DISP_LAYER_WORK_MODE_SCALER; 
  layer_para.pipe = 0; 
  layer_para.fb.addr[0]       = 0;//your Y address,modify this 
  layer_para.fb.addr[1]       = 0; //your C address,modify this 
  layer_para.fb.addr[2]       = 0; 
  layer_para.fb.size.width    = w;
  layer_para.fb.size.height   = h;
  layer_para.fb.mode          = disp_mode;///DISP_MOD_INTERLEAVED;//DISP_MOD_NON_MB_PLANAR;//DISP_MOD_NON_MB_PLANAR;//DISP_MOD_NON_MB_UV_COMBINED;
  layer_para.fb.format        = disp_format;//DISP_FORMAT_YUV420;//DISP_FORMAT_YUV422;//DISP_FORMAT_YUV420;
  layer_para.fb.br_swap       = 0;
  layer_para.fb.seq           = disp_seq;//DISP_SEQ_UVUV;//DISP_SEQ_YUYV;//DISP_SEQ_YVYU;//DISP_SEQ_UYVY;//DISP_SEQ_VYUY//DISP_SEQ_UVUV
  layer_para.ck_enable        = 0;
  layer_para.alpha_en         = 1; 
  layer_para.alpha_val        = 0xff;
  layer_para.src_win.x        = 0;
  layer_para.src_win.y        = 0;
  layer_para.src_win.width    = w;
  layer_para.src_win.height   = h;
  layer_para.scn_win.x        = 0;
  layer_para.scn_win.y        = 0;
  layer_para.scn_win.width    = LCD_WIDTH;//800;
  layer_para.scn_win.height   = LCD_HEIGHT;//480;
  arg[0] = sel;
  arg[1] = hlay;
  arg[2] = (__u32)&layer_para;
  ioctl(disphd,DISP_CMD_LAYER_SET_PARA,(void*)arg);
#if 0
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd,DISP_CMD_LAYER_TOP,(void*)arg);
#endif
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd,DISP_CMD_LAYER_OPEN,(void*)arg);
  
#if 1
  int fb_fd;
  unsigned long fb_layer;
  void *addr = NULL;
  fb_fd = open("/dev/fb0", O_RDWR);
  if (ioctl(fb_fd, FBIOGET_LAYER_HDL_0, &fb_layer) == -1) {
    printf("get fb layer handel\n"); 
  }
  
  addr = malloc(LCD_WIDTH*LCD_HEIGHT*3);
  memset(addr, 0xff, LCD_WIDTH*LCD_HEIGHT*3);
  write(fb_fd, addr, LCD_WIDTH*LCD_HEIGHT*3);
  //memset(addr, 0x12, 800*480*3);
  
  printf("fb_layer hdl: %ld\n", fb_layer);
  close(fb_fd);
  
  arg[0] = 0;
  arg[1] = fb_layer;
  ioctl(disphd, DISP_CMD_LAYER_BOTTOM, (void *)arg);
#endif
}

void disp_start(void)
{
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd, DISP_CMD_VIDEO_START,  (void*)arg);
}

void disp_stop(void)
{
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd, DISP_CMD_VIDEO_STOP,  (void*)arg);
}

int disp_on()
{
  arg[0] = 0;
  ioctl(disphd, DISP_CMD_LCD_ON, (void*)arg);
}

int disp_set_addr(int w,int h,int *addr, int csi_format)
{
#if 0 
  layer_para.fb.addr[0]       = *addr;//your Y address,modify this 
  layer_para.fb.addr[1]       = *addr+w*h; //your C address,modify this 
  layer_para.fb.addr[2]       = *addr+w*h*3/2; 
  
  arg[0] = sel;
  arg[1] = hlay;
  arg[2] = (__u32)&layer_para;
  ioctl(disphd,DISP_CMD_LAYER_SET_PARA,(void*)arg);
#endif
  __disp_video_fb_t  fb_addr; 
  memset(&fb_addr, 0, sizeof(__disp_video_fb_t));
  
  fb_addr.interlace       = 0;
  fb_addr.top_field_first = 0;
  fb_addr.frame_rate      = 25;
  fb_addr.addr[0] = *addr;
  // fb_addr.addr[1] = *addr + w * h;
  // fb_addr.addr[2] = *addr + w*h*3/2;
  
  
  switch(csi_format){
  case V4L2_PIX_FMT_YUV422P:
  case V4L2_PIX_FMT_YUYV:
  case V4L2_PIX_FMT_YVYU:
  case V4L2_PIX_FMT_UYVY:
  case V4L2_PIX_FMT_VYUY:  
    fb_addr.addr[1]       = *addr+w*h; //your C address,modify this 
    fb_addr.addr[2]       = *addr+w*h*3/2; 
    break;
  case V4L2_PIX_FMT_YUV420:
    fb_addr.addr[1]       = *addr+w*h; //your C address,modify this 
    fb_addr.addr[2]       = *addr+w*h*5/4;
    break;
  case V4L2_PIX_FMT_NV16:
  case V4L2_PIX_FMT_NV12: 
  case V4L2_PIX_FMT_HM12: 
    fb_addr.addr[1]       = *addr+w*h; //your C address,modify this 
    fb_addr.addr[2]       = layer_para.fb.addr[1];
    break;
    
  default:
    printf("csi_format is not found!\n");
    break;
    
  }
  
  fb_addr.id = 0;  //TODO
  
  arg[0] = sel;
  arg[1] = hlay;
  arg[2] = (__u32)&fb_addr;
  ioctl(disphd, DISP_CMD_VIDEO_SET_FB, (void*)arg);
}

int disp_quit()
{
  __u32 arg[4];
  arg[0] = 0;
  ioctl(disphd, DISP_CMD_LCD_OFF, (void*)arg);
  
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd, DISP_CMD_LAYER_CLOSE,  (void*)arg);
  
  arg[0] = sel;
  arg[1] = hlay;
  ioctl(disphd, DISP_CMD_LAYER_RELEASE,  (void*)arg);
  close (disphd);
}
