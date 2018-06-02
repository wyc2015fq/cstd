/*head files:*/  
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h>  
#include <assert.h>  

#include <getopt.h>   

#include <fcntl.h>   
#include <unistd.h>   
#include <errno.h>  
#include <signal.h>  


#include <sys/stat.h>   
#include <sys/types.h>   
#include <sys/time.h>   
#include <sys/mman.h>   
#include <sys/ioctl.h>  

#include <asm/types.h>   

#include <linux/videodev2.h>  

//////////////////////////////////////////declarations:  
//state reset:  
#define CLEAR(x)  memset(&(x),0,sizeof(x))  
//devices:  
#define V4L2_DEVICE     "/dev/video0"   
#define FBVID_DEVICE    "/dev/fb/3"  
#define OUTPUTFILE      "/mnt/gjf/vidrecord.uyvy"  

///////////////////////////////////////////////////////////////////////////////////////////  
#define SCREEN_BPP          16  
#define D1_WIDTH            352  
#define D1_HEIGHT           288  
#define D1_LINE_WIDTH       (D1_WIDTH * SCREEN_BPP / 8)  
#define D1_FRAME_SIZE       (D1_LINE_WIDTH * D1_HEIGHT)  

////////////////////////////////////////////////////////////////////////////////////////////  
typedef enum{  
  IO_METHOD_READ,  
    IO_METHOD_MMAP,  
    IO_METHOD_USERPTR  
}io_method;  

struct buffer{  
  void*  start;  
  size_t length;  
};  

static char* video_device=NULL;  
static char* output_device=NULL;  
static io_method io=IO_METHOD_MMAP;  
struct buffer* buffers=NULL;  
static unsigned int n_buffers=0;  
static int   videofd = -1 ;    //device discription for video device;  
static int   outputfd  = -1 ;  //device discprition for output device;   
static int   captureSize = 0 ;     //picture size  
static unsigned int imagewidth = 0 ;  //image width  
static unsigned int imageheight = 0 ; //image height  
static unsigned int timelimit = 1 ;   //capturing time  
static unsigned int totalframes = 0 ; //catpturing frames  

#define FRAME 0  
#define TIME  1  
static int flag = FRAME;  

////////////////////////////////////////////////////////  
typedef enum{YUYV=0,UYVY}imageType;  
static imageType format=YUYV;  
///////////////////////////////////////////////////////  

//command line:  
static const char short_options[]="d:pmruw:h:t:f:o:";  

static const struct option long_options[] =  
{  
  {"device",required_argument,NULL,'d'},  
  {"help",no_argument,NULL,'p'},  
  {"mmap",no_argument,NULL,'m'},  
  {"read",no_argument,NULL,'r'},  
  {"usrptr",no_argument,NULL,'u'},  
  {"width",required_argument,NULL,'w'},  
  {"height",required_argument,NULL,'h'},  
  {"time",required_argument,NULL,'t'},  
  {"frame",required_argument,NULL,'f'},  
  {"format",required_argument,NULL,'o'},  
  { 0 , 0 , 0 , 0 }  
};  

static void usage(FILE* fp,int argc,char **argv)  
{  
  fprintf (fp,"Usage: %s [options]\n\n"  
    "Options:\n"  
    "-d | --device name Video device name [/dev/video]\n"  
    "-p | --help Print this message\n"  
    "-m | --mmap Use memory mapped buffers\n"  
    "-r | --read Use read() calls\n"  
    "-u | --userp Use application allocated buffers\n"  
    "-w | --image width\n"  
    "-h | --image height\n"  
    "-t | --capture time\n"  
    "-f | --number of frames\n"  
    "-o | --image format\n"  
    "",  
    argv[0]);  
}  


void process_command(int argc,char **argv)  
{  
  int index ;  
  int next_option ;  
  int f = -1;  
  video_device = V4L2_DEVICE ;  
  output_device = FBVID_DEVICE ;  
  imagewidth  = D1_WIDTH ;  
  imageheight = D1_HEIGHT ;  
  timelimit = 1 ;  
  totalframes = 0 ;  
  format = UYVY ;   
  
  flag = FRAME ;  
  
  do{  
    next_option = getopt_long(argc,argv,short_options,long_options,&index) ;  
    switch(next_option) {  
    case 'd' :  
      video_device=optarg ;  
      break;  
    case 'p' :  
      usage(stdout,argc,argv) ;                                           
      exit(EXIT_FAILURE) ;  
    case 'm':  
      io = IO_METHOD_MMAP ;  
      break ;  
    case 'r':  
      io = IO_METHOD_READ ;  
      break ;  
    case 'u':  
      io = IO_METHOD_USERPTR ;  
      break ;  
    case 'w':  
      imagewidth = atoi(optarg);  
      break;  
    case 'h':  
      imageheight = atoi(optarg);  
      break;  
    case 't':  
      flag = TIME;  
      timelimit = atoi( optarg ) ;  
      break;     
    case 'f':   
      flag = FRAME;  
      totalframes = atoi( optarg ) ;    
      break;       
    case 'o':  
      f = atoi(optarg);  
      format = ( f >= YUYV && f <= UYVY )? f : UYVY;  
      break;          
    case -1:  
      break ;  
    default:   
      usage(stdout,argc,argv) ;   
      exit(EXIT_FAILURE) ;              
      
    }  
    
  }while(next_option!=-1);  
}  


///////////////////videoCapture:  
void open_device()  
{  
  struct stat st;   
  
  if (-1 == stat (video_device, &st)) {  
    fprintf (stderr, "Cannot identify ¡¯%s¡¯: %d, %s\n",video_device, errno, strerror (errno));  
    exit (EXIT_FAILURE);  
  }  
  
  if (!S_ISCHR (st.st_mode)) {  
    fprintf (stderr, "%s is no device\n", video_device);  
    exit (EXIT_FAILURE);  
  }  
  
  videofd = open (video_device, O_RDWR | O_NONBLOCK, 0);  
  if (videofd == -1) {  
    fprintf ( stderr, "Cannot open ¡¯%s¡¯: %d, %s\n", video_device , errno , strerror (errno) );  
    exit (EXIT_FAILURE);  
  }  
}  


static void init_read(unsigned int bufSize)  //directly read  
{  
  buffers = calloc (1, sizeof (*buffers));  
  if (!buffers) {  
    fprintf (stderr, "Out of memory\n");  
    exit (EXIT_FAILURE);  
  }  
  buffers[0].length = bufSize ;  
  buffers[0].start = malloc (bufSize);  
  if (!buffers[0].start) {  
    fprintf (stderr, "Out of memory\n");  
    exit (EXIT_FAILURE);  
  }  
}  

static void init_mmap() //map the driver's buffer to application buffer  
{  
  struct v4l2_requestbuffers req;  
  CLEAR (req);  
  req.count = 4;  
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
  req.memory = V4L2_MEMORY_MMAP;  
  if (ioctl(videofd, VIDIOC_REQBUFS, &req) == -1) { //allocate buffer in the driver memory space  
    if (EINVAL == errno) {  
      fprintf (stderr, "%s does not support ""memory mapping\n", video_device);  
      exit (EXIT_FAILURE);  
    } else {  
      fprintf(stderr,"Failed:VIDIOC_REQBUFS");  
      exit (EXIT_FAILURE);  
    }  
  }  
  
  if (req.count < 2) {    //double buffers  
    fprintf (stderr, "Insufficient buffer memory on %s\n", video_device);  
    exit (EXIT_FAILURE);  
  }  
  
  buffers = calloc (req.count, sizeof (*buffers));  
  if (!buffers) {  
    fprintf (stderr, "Out of memory\n");  
    exit (EXIT_FAILURE);  
  }  
  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {  
    
    struct v4l2_buffer buf;  
    CLEAR (buf);  
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    buf.memory = V4L2_MEMORY_MMAP;  
    buf.index = n_buffers;  
    
    if (-1 == ioctl (videofd, VIDIOC_QUERYBUF, &buf)) {  //query the state of one of the  buffers in driver space  
      fprintf(stderr,"Failed:VIDIOC_QUERYBUF ");   
      exit(EXIT_FAILURE);  
      
    }  
    
    buffers[n_buffers].length = buf.length;  
    
    
    
    buffers[n_buffers].start = mmap (    NULL /* start anywhere */,  
      buf.length,  
      PROT_READ | PROT_WRITE /* required */,  
      MAP_SHARED /* recommended */,  
      videofd, buf.m.offset);  
    
    if (buffers[n_buffers].start == MAP_FAILED ) {  
      fprintf(stderr,"Failed:when MMAP");  
      exit(EXIT_FAILURE);  
      
    }  
  }  
  
}  

static void init_usrptr(unsigned int bufSize)  //  
{  
  struct v4l2_requestbuffers req;  
  CLEAR (req);  
  req.count = 4;  
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
  req.memory = V4L2_MEMORY_USERPTR;  
  if (-1 == ioctl (videofd, VIDIOC_REQBUFS, &req)) {  
    if (EINVAL == errno) {  
      fprintf (stderr, "%s does not support ""user pointer i/o\n", video_device);  
      exit (EXIT_FAILURE);  
    }   
    else {  
      fprintf(stderr,"Failed:VIDIOC_REQBUFS\n");  
    }  
  }  
  buffers = calloc (4, sizeof (*buffers));  
  if (!buffers) {  
    fprintf (stderr, "Out of memory\n");  
    exit (EXIT_FAILURE);  
  }  
  for (n_buffers = 0; n_buffers < 4; ++n_buffers) {  
    buffers[n_buffers].length = bufSize;  
    buffers[n_buffers].start = malloc (bufSize);  //allocate buffers in user space  
    if (!buffers[n_buffers].start) {  
      fprintf (stderr, "Out of memory\n");  
      exit (EXIT_FAILURE);  
    }  
  }  
  
}  

static int xioctl( int fd,int request,void * arg)  
{  
  int r;  
  do{  
    r = ioctl (fd, request, arg);  
  }  
  while (-1 == r && EINTR == errno);  
  return r;  
}  

#define VPFE_STD_AUTO ((v4l2_std_id)(0x1000000000000000ULL))  


void init_device()  
{  
  struct v4l2_capability      cap;   
  struct v4l2_cropcap         cropCap;   
  struct v4l2_crop            crop;   
  struct v4l2_format          fmt;  
  unsigned int min;   
  //get  the capture device's capabilities  
  if (ioctl(videofd, VIDIOC_QUERYCAP, &cap) == -1) {   
    if (errno == EINVAL) {   
      fprintf ( stderr, "%s is no V4L2 device\n",video_device);   
      exit(EXIT_FAILURE);   
    }   
    fprintf(stderr,"Failed VIDIOC_QUERYCAP on %s (%s)\n", video_device,strerror(errno));   
    exit(EXIT_FAILURE);   
  }  
  
  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {   
    fprintf(stderr,"%s is no video capture device\n", video_device);   
    exit(EXIT_FAILURE);   
  }   
  
  switch (io) {  
  case IO_METHOD_READ:  
    if (!(cap.capabilities & V4L2_CAP_READWRITE)) {  
      fprintf (stderr, "%s does not support read i/o\n",video_device);   
      exit (EXIT_FAILURE);  
    }  
    break;  
  case IO_METHOD_MMAP:  
  case IO_METHOD_USERPTR:  
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {  
      fprintf (stderr, "%s does not support streaming i/o\n",video_device);  
      exit (EXIT_FAILURE);  
    }  
  }  
  
  //set croping and scaling attribure  
  CLEAR(cropCap);  
  cropCap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
  if (0 == ioctl (videofd, VIDIOC_CROPCAP, &cropCap)) {  
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    crop.c = cropCap.defrect; /* reset to default */  
    if (-1 == ioctl (videofd, VIDIOC_S_CROP, &crop)) {  
      switch (errno) {  
      case EINVAL:  
        fprintf(stderr,"Cropping not supported\n");  
        //exit (EXIT_FAILURE);  
      default:  
        fprintf(stderr, "Failed:VIDIOC_S_CROP");  
        // exit (EXIT_FAILURE);  
      }  
    }  
  }   
  else {  
    fprintf(stderr,"Failed:VIDIOC_CROPCAP");  
    //exit(EXIT_FAILURE);  
  }        
  
  
  
  //set picture format  
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
  fmt.fmt.pix.width  = D1_WIDTH;    //720  
  fmt.fmt.pix.height = D1_HEIGHT;   //480  
  
  switch(format) {  
  case YUYV :  
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;  
    break;  
  case UYVY :  
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;  
    break;  
  default :  
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_UYVY;  
  }  
  
  //fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;  
  fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;  
  
  if (-1 == ioctl (videofd, VIDIOC_S_FMT, &fmt)){  
    fprintf(stderr,"Failed: VIDIOC_S_FMT");  
    exit(EXIT_FAILURE);  
  }      
  
  captureSize = fmt.fmt.pix.sizeimage;  
  /* 
  min = fmt.fmt.pix.width * 2; 
  if (fmt.fmt.pix.bytesperline < min) 
  fmt.fmt.pix.bytesperline = min; 
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height; 
  if (fmt.fmt.pix.sizeimage < min) 
  fmt.fmt.pix.sizeimage=min;  
  fprintf(stderr,"fmt size : %u \n",fmt.fmt.pix.sizeimage); 
  
  */  
  
  //   printf("%width:%u,height:%u,sizeimage:%u\n",fmt.fmt.pix.width,fmt.fmt.pix.height,fmt.fmt.pix.sizeimage);  
  //allocate  buffer:  
  switch(io){  
  case IO_METHOD_READ:  
    init_read(fmt.fmt.pix.sizeimage);  
    break;  
  case IO_METHOD_MMAP:  
    init_mmap();  
    break;  
  case IO_METHOD_USERPTR:  
    init_usrptr(fmt.fmt.pix.sizeimage);  
    break;  
  default:  
    fprintf(stderr,"IO_METHOD : No such method\n");  
    break;  
    
  }  
  
}  


void startCapturing(void)  
{  
  unsigned int i;  
  enum v4l2_buf_type type;  
  switch (io) {  
  case IO_METHOD_READ:  
    /* Nothing to do. */  
    break;  
  case IO_METHOD_MMAP:  
    for (i = 0; i < n_buffers; ++i) {  
      
      struct v4l2_buffer buf;  
      
      CLEAR (buf);  
      
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
      buf.memory = V4L2_MEMORY_MMAP;  
      buf.index = i;  
      if (-1 == xioctl (videofd, VIDIOC_QBUF, &buf))  {  //enqueue an empty capturing buffer allocated in   
        //driver space  
        fprintf(stderr,"Failed(MMAP):VIDIOC_QBUF\n");  
        exit(EXIT_FAILURE);  
      }  
    }  
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if (-1 == ioctl (videofd, VIDIOC_STREAMON, &type)){ //start capturing  
      fprintf(stderr,"Failed(MMAP):VIDIOC_STREAMON\n");  
      exit(EXIT_FAILURE);  
    }  
    break;  
  case IO_METHOD_USERPTR:  
    for (i = 0; i < n_buffers; ++i) {  
      struct v4l2_buffer buf;  
      CLEAR (buf);  
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
      buf.memory = V4L2_MEMORY_USERPTR;  
      buf.m.userptr = (unsigned long) buffers[i].start;  
      buf.length = buffers[i].length;  
      
      if (-1 == ioctl (videofd, VIDIOC_QBUF, &buf)) {  //encode an empty buffer allocated in usr space  
        // in the capturing queue  
        fprintf(stderr,"Failed(USERPTR):VIDIOC_QBUF\n");  
        exit(EXIT_FAILURE);  
      }  
    }  
    
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if (-1 == ioctl (videofd, VIDIOC_STREAMON, &type)) {  //start capturing  
      fprintf(stderr,"Failed(USERPTR):VIDIOC_STREAMON\n");  
      exit(EXIT_FAILURE);  
    }  
  }  
  
}  

static int process_image(const void *p)  
{  
  
  
  
  
  return 0;  
}  

static int read_frame (void)  
{  
  struct v4l2_buffer buf;  
  unsigned int i;  
  switch (io) {  
  case IO_METHOD_READ:  
    if (-1 == read (videofd, buffers[0].start, buffers[0].length)) {  //read data into the usr buffer,  
      //note there is only one buffer  
      switch (errno) {  
      case EAGAIN:  
        return 0;  
      case EIO:  
        /* Could ignore EIO, see spec. */  
        /* fall through */  
      default:  
        {  
          fprintf(stderr,"Failed : read");  
          exit(EXIT_FAILURE);                   
        }  
      }  
    }   
    
    
    // if (write(outputfd, buffers[buf.index].start , captureSize ) == -1)    //write to the output file   
    //       fprintf(stderr,"Error writing the data to output file\n");   
    
    process_image (buffers[0].start);              //process this buffer  
    break;  
  case IO_METHOD_MMAP:  
    CLEAR (buf);  
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    buf.memory = V4L2_MEMORY_MMAP;  
    if (-1 == xioctl (videofd, VIDIOC_DQBUF, &buf)) {    //dequeue a buffer  
      switch (errno) {  
      case EAGAIN:  
        return 0;  
      case EIO:  
        /* Could ignore EIO, see spec. */  
        /* fall through */  
      default:  
        {  
          fprintf(stderr,"Failed : VIDIOC_DQBUF");  
          exit(EXIT_FAILURE);  
        }  
      }  
    }  
    assert (buf.index < n_buffers);  
    
    
    
    //    if (write(outputfd, buffers[buf.index].start , captureSize ) == -1);   
    
    process_image (buffers[buf.index].start);      //process this buffer  
    if (-1 == xioctl (videofd, VIDIOC_QBUF, &buf)) {    //enqueue this buffer in order to reuse it  
      fprintf(stderr,"Failed : VIDIOC_QBUF\n");  
      exit(EXIT_FAILURE);  
    }  
    break;  
  case IO_METHOD_USERPTR:  
    CLEAR (buf);  
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;         
    buf.memory = V4L2_MEMORY_USERPTR;  
    if (-1 == xioctl (videofd, VIDIOC_DQBUF, &buf)) {    
      switch (errno) {  
      case EAGAIN:  
        return 0;  
      case EIO:  
        /* Could ignore EIO, see spec. */  
        /* fall through */  
      default:  
        {   
          fprintf(stderr,"Failed : VIDIOC_DQBUF");  
          exit(EXIT_FAILURE);  
        }  
      }  
    }  
    for (i = 0; i < n_buffers; ++i)            //find the dequeued buffer in usr space  
      if (buf.m.userptr == (unsigned long) buffers[i].start && buf.length == buffers[i].length)  
        break;                         
      assert (i < n_buffers);  
      
      //        if (write(outputfd, buffers[i].start , captureSize ) == -1)    //write to the output file   
      //              fprintf(stderr,"Error writing the data to output file\n");   
      
      process_image ((void *) buf.m.userptr);  
      if (-1 == xioctl (videofd, VIDIOC_QBUF, &buf)) { //reuse  
        fprintf(stderr,"Failed : VIDIOC_QBUF");  
        exit(EXIT_FAILURE);  
      }   
      break;  
  }  
  return 0;  
}  



///////////////////////////////////////////////////////////////////////////////////////////////////////  
void timehandle(int signo)   
{   
  fprintf(stderr,"EXIT : time over !--->%u\n",timelimit);  
  exit(0);   
}   

void init_sigaction(void)   
{   
  struct sigaction act;   
  act.sa_handler=timehandle;   
  act.sa_flags=0;   
  sigemptyset(&act.sa_mask);   
  sigaction(SIGPROF,&act,NULL);   
}   

void init_time()   
{   
  struct itimerval value;   
  value.it_value.tv_sec= 1;   
  value.it_value.tv_usec=0;   
  value.it_interval=value.it_value;   
  setitimer(ITIMER_PROF,&value,NULL);   
}  


///////////////////////////////////////////////////////////////////////////////////////////////////////  

void mainloop (void)  
{  
  static  int framecount=0;  
  
  
  outputfd = open(OUTPUTFILE , O_WRONLY | O_CREAT | O_TRUNC, 00644);  
  if (outputfd == -1) {   
    fprintf(stderr,"Failed to open %s for writing\n", OUTPUTFILE);  
    exit(EXIT_FAILURE);   
  }  
  
  if(flag == TIME)  
  {  
    //init_sigaction( ) ;  
    //init_time( ) ;  
    while( 1 ){  
      fd_set fds;  
      struct timeval tv;  
      int r;  
      FD_ZERO (&fds);  
      FD_SET (videofd, &fds);  
      /* Timeout. */  
      tv.tv_sec = 2;  
      tv.tv_usec = 0;  
      r = select (videofd + 1, &fds, NULL, NULL, &tv);  //wait for a new frame  
      if (-1 == r) {  
        if (EINTR == errno)  
          continue;  
        fprintf(stderr,"select");  
        exit(EXIT_FAILURE);  
      }  
      if (0 == r) {  
        fprintf (stderr, "select timeout\n");  
        exit (EXIT_FAILURE);  
      }  
      
      if (read_frame ())   
        break;  
      printf("capturing frame %d \n",framecount);  
      framecount++;  
      
      /* EAGAIN - continue select loop. */  
    }  
  }  
  else   
    while( framecount < totalframes ){  
      fd_set fds;  
      struct timeval tv;  
      int r;  
      FD_ZERO (&fds);  
      FD_SET (videofd, &fds);  
      /* Timeout. */  
      tv.tv_sec = 2;  
      tv.tv_usec = 0;  
      // fprintf(stderr,"wait for a new frame\n");  
      r = select (videofd + 1, &fds, NULL, NULL, &tv);  //wait for a new frame  
      if (-1 == r) {  
        if (EINTR == errno)  
          continue;  
        fprintf(stderr,"select");  
        exit(EXIT_FAILURE);  
      }  
      if (0 == r) {  
        fprintf (stderr, "select timeout\n");  
        exit (EXIT_FAILURE);  
      }  
      
      //fprintf(stderr,"Read next frame :\n");  
      if (read_frame ())   
        break;  
      printf("capturing frame %d \n",framecount);  
      framecount++;  
      
      /* EAGAIN - continue select loop. */  
    }  
    if( outputfd != -1 )  
      close(outputfd);  
}  


//---------------------------------------------  
void stop_capturing (void)     //stop catpureing pictures  
{  
  enum v4l2_buf_type type;  
  switch (io) {  
  case IO_METHOD_READ:  
    /* Nothing to do. */  
    break;  
  case IO_METHOD_MMAP:  
  case IO_METHOD_USERPTR:  
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;  
    if (-1 == xioctl (videofd, VIDIOC_STREAMOFF, &type)){  
      fprintf (stderr,"VIDIOC_STREAMOFF");  
      exit(EXIT_FAILURE);           
    }   
    break;   
  }  
  
  void uninit(void)  
  {  
    unsigned int i;  
    switch (io) {  
    case IO_METHOD_READ:  
      free (buffers[0].start);  
      break;  
    case IO_METHOD_MMAP:  
      for (i = 0; i < n_buffers; ++i)  
        if (-1 == munmap (buffers[i].start, buffers[i].length)){  
          fprintf(stderr,"munmap");  
          exit(EXIT_FAILURE);  
        }  
        break;  
    case IO_METHOD_USERPTR:  
      for (i = 0; i < n_buffers; ++i)  
        free (buffers[i].start);  
      break;  
    }  
    free (buffers);  
  }  
}  

void close_device (void)  
{  
  if (-1 == close (videofd))  
  {  
    fprintf(stderr,"close");  
    exit(EXIT_FAILURE);  
  }  
  videofd = -1;  
}  

///////////////////////////////////////////////////////////////////////////entry:  

int test_v4l2a(int argc,char **argv)  
{  
  process_command(argc,argv);  
  open_device();  
  fprintf(stderr,"SUCCESS : open_device \n");   
  init_device();  
  fprintf(stderr,"SUCCESS : Init_device \n");  
  startCapturing();  
  fprintf(stderr,"SUCCESS : startCapturing\n");  
  
  mainloop();  
  fprintf(stderr,"SUCCESS : stopCapturing\n");  
  stop_capturing() ;   
  fprintf(stderr,"SUCCUSS : closeDevice\n");  
  close_device();  
  
  return 0;  
}  