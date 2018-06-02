#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <termios.h> 
#include <errno.h> 
#include <string.h>

#include <sched.h> 
#include <sys/ioctl.h> 
#include <linux/serial.h> 
#include <unistd.h> 
#include <termios.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <strings.h> 

typedef struct 
{
  int m_SerialFD;
  pthread_t id_read; // thread id
  pthread_t id_send; // thread id
  int m_nPort;
  BOOL m_bIsOpen;
  void* m_pOwner;
  recv_message_f recv_message;
  pthread_cond_t cond;
  pthread_mutex_t lock;
} sys_linux_serial_port_t;


int UART_Set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity)
{
  int i; 
  
  // int status; 
  static const int name_arr[] = { 0, 50, 75, 110, 134, 150, 200, 300, 600, 1200,
    1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800,
    500000, 576000, 921600
  };
  static const int speed_arr[] = { B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200,
    B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800,
    B500000, B576000, B921600
  };
  
  int table_size = countof(speed_arr);
  struct termios options; 
  if(tcgetattr( fd,&options) != 0){ 
    perror("SetupSerial 1"); 
    return(FALSE); 
  }
  speed = MIN(speed, 921600);
  for(i= 0;i < table_size;i++) { 
    if (speed <= name_arr[i]) {
      cfsetispeed(&options, speed_arr[i]);
      cfsetospeed(&options, speed_arr[i]);
      break;
    }
  }
  options.c_cflag |= CLOCAL;
  options.c_cflag |= CREAD;
  switch(flow_ctrl){
  case 0 :
    options.c_cflag &= ~CRTSCTS;
    break; 
  case 1 :
    options.c_cflag |= CRTSCTS;
    break;
  case 2 :
    options.c_cflag |= IXON | IXOFF | IXANY;
    break;
  }
  
  options.c_cflag &= ~CSIZE; 
  switch (databits){ 
  case 5 :
    options.c_cflag |= CS5;
    break;
  case 6 :
    options.c_cflag |= CS6;
    break;
  case 7 : 
    options.c_cflag |= CS7; 
    break;
  default: 
  case 8: 
    options.c_cflag |= CS8;
    break;
  }
  switch (parity) { 
  default:
  case 'n':
  case 'N': 
    options.c_cflag &= ~PARENB; 
    options.c_iflag &= ~INPCK; 
    break; 
  case 'o': 
  case 'O': 
    options.c_cflag |= (PARODD | PARENB); 
    options.c_iflag |= INPCK; 
    break; 
  case 'e': 
  case 'E': 
    options.c_cflag |= PARENB; 
    options.c_cflag &= ~PARODD; 
    options.c_iflag |= INPCK; 
    break;
  case 's': 
  case 'S': 
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    break; 
  } 
  switch (stopbits) {
  default:
  case 1:
    options.c_cflag &= ~CSTOPB; 
    break;
  case 2:
    options.c_cflag |= CSTOPB; 
    break;
  }
  options.c_oflag &= ~OPOST;
  // 默认文件是行缓冲的. 要设置原始输入模式
  options.c_lflag &= ~(ICANON | ECHO | ECHOE);
  options.c_cc[VTIME] = 50; // 15 seconds
  options.c_cc[VMIN] = 0;
  tcflush(fd,TCIFLUSH);

  if(tcsetattr(fd,TCSANOW,&options) != 0){ 
    perror("com set error!\n"); 
    return (FALSE); 
  }

  return 1; 
}
int write1(int fd, const void * buf, unsigned int len) {
  int i, times = 0;
  const char* p = (const char*)buf;
  for (i=0; i<len && times<100; ) {
    int ret = write(fd,p+i,1);
    if (ret>0) {
      ++i;
    } else {
      ++times;
    }
    usleep(100);
  }
  if (times>0) {
    printf("times %d\n", times);
  }
  return len;
}

int write2(int fd, const void * buf, unsigned int len) {
  fd_set rd;
  char chr[4];
  struct timeval time;
  FD_ZERO(&rd);
  const char* p = (const char*)buf;
  FD_SET(fd, &rd);
  int i;
  fcntl(fd,F_SETFL,0);
  for (i=0; i<len; ++i) {
    if (select(fd + 1, NULL, &rd, NULL, NULL) < 0) {
      printf("error!\n");
    }
    else {
      int ret = write(fd,p+i,1);
    }
  }
  return len;
}
int sys_linux_serial_port_send_buf(int fd, const void* send_buf, int data_len)
{
  int ret = 0;
  ret = write1(fd,send_buf,data_len);
  if (data_len == ret ) {
    printf("send %d %d\n", data_len, ret);
    return ret;
  } else {
    printf("write err ret=%d datalen=%d", ret, data_len);
    tcflush(fd,TCOFLUSH); 
    return 0;
  }
  return data_len;
}
int sys_linux_serial_port_send(sys_serial_port_t* ss, const void* send_buf, int data_len)
{
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  int ret = data_len;
    ret = sys_linux_serial_port_send_buf(s->m_SerialFD, send_buf, data_len);
#if 0
  if (data_len<1024) {
  } else {
    pthread_mutex_lock(&s->lock);
    sendbuf_addbuf(&ss->m_szWriteBufferList, send_buf, data_len);
    pthread_cond_signal(&s->cond);
    pthread_mutex_unlock(&s->lock);
  }
#endif
  return ret;
}
int sys_linux_serial_port_close(sys_serial_port_t* ss)
{
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  s->m_bIsOpen = false;
  if (isatty(s->m_SerialFD)) {
    pthread_join(s->id_send, NULL);
    pthread_join(s->id_send, NULL);
    close(s->m_SerialFD);
    pthread_mutex_destroy(&s->lock);  
    pthread_cond_destroy(&s->cond);  
    pfree(s);
    memset(ss, 0, sizeof(*ss));
    return 1;
  }
  return 0;
}

int sys_linux_serial_port_recv(sys_serial_port_t* ss, void* data, int nSize)
{
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  //接收数据
  int mylen = read(s->m_SerialFD, (char*)data, nSize);
  return mylen;
}

void* com_proc_recv(void* pParam)
{
  sys_serial_port_t* ss = (sys_serial_port_t*)pParam;
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  int i = 0, fd;
  fd_set rd;
  char chr[4];
  struct timeval time;
  fd = s->m_SerialFD;
  FD_ZERO(&rd);
  FD_SET(fd, &rd);
  time.tv_sec = 1;
  time.tv_usec = 0;
  char* buf = NULL;
  int buflen = 1*1024*1024;
  struct serial_struct serial;
  MYREALLOC(buf, buflen);

  ioctl(fd, TIOCGSERIAL, &serial);
  serial.xmit_fifo_size = buflen;
  ioctl(fd, TIOCSSERIAL, &serial);
  while (s->m_bIsOpen) {
    while (FD_ISSET(s->m_SerialFD, &rd)) {
      if (select(fd + 1, &rd, NULL, NULL, NULL) < 0) {
        printf("error!\n");
      }
      else {
        int mylen = read(s->m_SerialFD, buf, buflen-1);
        //printf("%d\n", buflen);
        if (mylen>0) {
          buf[mylen] = 0;
          sys_serial_port_SendMessage(ss, buf, mylen, 0);
        }
      }
    }
  }
  pthread_exit(0);
  FREE(buf);
}

void* com_proc_send(void* pParam) {
  sys_serial_port_t* ss = (sys_serial_port_t*)pParam;
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  while (s->m_bIsOpen) {
    pthread_mutex_lock(&s->lock);
    pthread_cond_wait(&s->cond, &s->lock);
    while (ss->m_szWriteBufferList) {
      // send event 发送数据
      // Write character event from s
      sendbuf_t* sb = ss->m_szWriteBufferList;
      ss->m_szWriteBufferList = ss->m_szWriteBufferList->next;
      sys_linux_serial_port_send_buf(s->m_SerialFD, sb->data, sb->len);
      pfree(sb);
    }
    pthread_mutex_unlock(&s->lock);
  }
  pthread_exit(0);
}

int sys_serial_port_open(sys_serial_port_t* ss, UINT port, UINT baud, char parity, UINT databits, UINT stopbits) {
  enum {max_port = 16};
  char comname[50] = {0};
  int fd = 0;
  for (; port<max_port; ++port) {
    snprintf(comname, countof(comname), "/dev/ttyS%d", port);
    //fd = open(comname, O_RDWR|O_NOCTTY|O_NDELAY);
    fd = open(comname, O_RDWR|O_NOCTTY);
    if (-1 != fd) {
      break;
    }
  }
  if (port==max_port) {
    printf("sys_serial_port_open open fail\n");
    return 0;
  }
  if(fcntl(fd, F_SETFL, 0) < 0){
    printf("sys_serial_port_open fcntl failed!\n");
    close(fd);
    return 0;
  }
#if 0
  if(0 == isatty(STDIN_FILENO)){
    printf("standard input is not a terminal device\n");
    close(fd);
    return 0;
  }
#endif
  if (!UART_Set(fd, baud, 0, databits, stopbits, parity)) {
    printf("UART_Set fail\n");
    close(fd);
    return 0;
  } else {
    int ret;
    sys_linux_serial_port_t* s;
    s = (sys_linux_serial_port_t*)pmalloc(sizeof(*s));
    ss->x = s;
    s->m_SerialFD = fd;
    s->m_bIsOpen = true;
    s->m_nPort = port;
    //s->cond = PTHREAD_COND_INITIALIZER;
    //s->lock  = PTHREAD_MUTEX_INITIALIZER;
    //pthread_cond_init(&s->cond, 0);
    //pthread_mutex_init(&s->lock, NULL);
    ss->close = sys_linux_serial_port_close;
    ss->send = sys_linux_serial_port_send;
    ss->recv = sys_linux_serial_port_recv;
    ret = pthread_create(&s->id_read, NULL, com_proc_recv, ss);
    if (ret < 0) {
      printf("pthread_create fail\n");
      FREE(s);
      close(fd);
      return 0;
    }
#if 1
    ret = pthread_create(&s->id_send, NULL, com_proc_send, ss);
    if (ret < 0) {
      FREE(s);
      close(fd);
      return 0;
    }
#endif
  }
  return port+1;
}
