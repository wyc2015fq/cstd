#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
// 00 = NONE, 01 = Odd, 02 = Even, 03 = Mark, 04 = Space
enum {NONE = 0, ODD = 1, EVEN = 2, Mark = 3, Space = 4};

typedef char* PCHAR;
typedef void* PVOID;
typedef uchar* PBYTE;
typedef struct 
{
  int m_SerialFD;
  int m_nPortNr;
  BOOL m_bIsOpen;
  void* m_pOwner;
  recv_message_f recv_message;
} sys_linux_serial_port_t;

PVOID ComProc(PVOID param)
{
  int i = 0;
  fd_set rd;
  char chr[4];
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)param;
  FD_ZERO(&rd);
  FD_SET(s->m_SerialFD, &rd);
  while (s->m_bIsOpen) {
    while (FD_ISSET(s->m_SerialFD, &rd)) {
      if (select(s->m_SerialFD + 1, &rd, NULL, NULL, NULL) < 0) {
        printf("error!\n");
      }
      else {
        while (read(s->m_SerialFD, &chr, 1) > 0) {
          sys_serial_port_SendMessage(s->m_pOwner, WM_COMM_RXCHAR, chr[0], s->m_nPortNr);
        }
      }
    }
  }
  pthread_exit(0);
}
int sys_linux_serial_port_send(sys_serial_port_t* ss, void* pBuf, int dwLength)   /**/
{
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  return write(s->m_SerialFD, pBuf, dwLength);
}
int sys_linux_serial_port_close(sys_serial_port_t* ss)
{
  sys_linux_serial_port_t* s = (sys_linux_serial_port_t*)ss->x;
  if (isatty(s->m_SerialFD)) {
    close(s->m_SerialFD);
    s->m_SerialFD = -1;
    s->m_bIsOpen = false;
    pfree(s);
    memset(ss, 0, sizeof(*ss));
    return 1;
  }
  return 0;
}

bool sys_linux_serial_port_SetRaw(int nSerialFD, bool isRaw)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  if (isRaw) {
    option.c_iflag &= ~(ICRNL | IGNCR | INLCR | IGNBRK | BRKINT);
    option.c_iflag &= ~(IXON | IXOFF | IXANY);
    option.c_oflag &= ~(OCRNL | OLCUC | ONLCR | OPOST);
    option.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  }
  else {
    option.c_oflag |= OPOST;
    option.c_lflag |= (ICANON | ECHO | ECHOE | ISIG);
  }
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetMin(int nSerialFD, UINT min)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  option.c_cc[VMIN] = min;
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetTime(int nSerialFD, UINT delay)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  option.c_cc[VTIME] = delay;
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetSpeed(int nSerialFD, int speed)
{
  int i = 0;
  if (!isatty(nSerialFD)) {
    return false;
  }
  static const int speed_table[] = { 0, 50, 75, 110, 134, 150, 200, 300, 600, 1200,
      1800, 2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800,
      500000, 576000, 921600
                      };
  static const int speeds[] = { B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200,
      B1800, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800,
      B500000, B576000, B921600
  };
  int table_size = 23;
  for (i = 0; i < table_size; i++) {
    if (speed_table[i] >= speed) {
      //printf("speed = %d\n", speed);
      break;
    }
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  if (cfsetispeed(&option, speeds[i]) != 0) {
    return false;
  }
  if (cfsetospeed(&option, speeds[i]) != 0) {
    return false;
  }
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetByteSize(int nSerialFD, BYTE bytesize)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  option.c_cflag &= ~CSIZE;
  switch (bytesize) {
  case 5:
    option.c_cflag |= CS5;
    break;
  case 6:
    option.c_cflag |= CS6;
    break;
  case 7:
    option.c_cflag |= CS7;
    break;
  case 8:
    option.c_cflag |= CS8;
    break;
  default:
    option.c_cflag |= CS8;
    break;
  }
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetStopBits(int nSerialFD, BYTE stopbits)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  switch (stopbits) {
  case 1:
    option.c_cflag &= ~CSTOPB;
    break;
  case 2:
    option.c_cflag |= CSTOPB;
    break;
  default:
    option.c_cflag &= ~CSTOPB;
    break;
  }
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}
bool sys_linux_serial_port_SetParity(int nSerialFD, BYTE parity)
{
  if (!isatty(nSerialFD)) {
    return false;
  }
  struct termios option;
  if (tcgetattr(nSerialFD, &option) != 0) {
    return false;
  }
  switch (parity) {
  case NONE:
    option.c_cflag &= ~PARENB;
    option.c_iflag &= ~INPCK;
    break;
  case ODD:
    option.c_cflag |= PARENB;
    option.c_cflag |= PARODD;
    option.c_iflag |= INPCK;
    break;
  case EVEN:
    option.c_cflag |= PARENB;
    option.c_cflag &= ~PARODD;
    option.c_iflag |= INPCK;
  default:
    option.c_cflag &= ~PARENB;
    option.c_iflag &= ~INPCK;
    break;
  }
  if (tcsetattr(nSerialFD, TCSANOW, &option) != 0) {
    return false;
  }
  else {
    return true;
  }
}

int sys_serial_port_set(int serial_fd, UINT port, UINT baud, char parity, UINT databits, UINT stopbits) {
  if (!sys_linux_serial_port_SetRaw(serial_fd, true)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetMin(serial_fd, 0)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetTime(serial_fd, 0)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetSpeed(serial_fd, baud)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetByteSize(serial_fd, databits)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetStopBits(serial_fd, stopbits)) {
    return FALSE;
  }
  if (!sys_linux_serial_port_SetParity(serial_fd, parity)) {
    return FALSE;
  }
  return true;
}

int sys_serial_port_open(sys_serial_port_t* ss, UINT port, UINT baud, char parity, UINT databits, UINT stopbits, void* pPortOwner, recv_message_f recv_message )
{
  enum {max_port = 16};
  sys_linux_serial_port_t* s = NULL;
  pthread_t id;
  int ret;
  int serial_fd = 0;
  char comname[50] = {0};
  for (; port<max_port; ++port) {
    snprintf(comname, countof(comname), "/dev/ttyS%d", port);
    serial_fd = open(comname, O_RDWR | O_NOCTTY | O_NDELAY);
    printf("comname %s\n", comname);
    if (isatty(serial_fd)) {
      break;
    } else {
      close(serial_fd);
    }
  }
  if (port==max_port) {
    return 0;
  }
  if (!sys_serial_port_set(serial_fd, port, baud, parity, databits, stopbits)) {
    close(serial_fd);
    return 0;
  }
  s = (sys_linux_serial_port_t*)pmalloc(sizeof(*s));
  memset(s, 0, sizeof(*s));
  ss->x = s;
  s->m_SerialFD = serial_fd;
  s->m_bIsOpen = true;
  s->m_nPortNr = port;
  ss->close = sys_linux_serial_port_close;
  ss->send = sys_linux_serial_port_send;
  ret = pthread_create(&id, NULL, ComProc, s);
  if (ret < 0) {
    FREE(s);
    return false;
  }
  return port+1;
}
