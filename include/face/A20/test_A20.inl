
#include "cstd.h"
#include "face_A20.h"
#include "cfile.h"
//#define printf dbg_printf
//#include "test_comportui.inl"

#include "net/test_tcp.inl"

//#include "lock_free_slist.h"

int sys_serial_port_recv_safe(sys_serial_port_t* ss, void* data, int n) {
  int len = 0, time = n/10;
  char* cdata = (char*)data;
  for (;len<n;) {
    int recvlen = sys_serial_port_recv(ss, cdata+len, n - len);
    if (recvlen<0) {
      break;
    }
    printf("recvlen %d\r", len);
    len += recvlen;
  }
  return len;
}
#define TESTLEN  (1<<8)

int test_on_recv_message(sys_serial_port_t* ss, UINT Msg)
{
  int ishex = *(int*)ss->user;
  char buf[256] = {0};
  int len = 0;
  static int skip = 0;
  if (ishex) {
    len = sys_serial_port_recv(ss, buf, 1);
    if (len && buf[0]==0) {
      
    } else {
      skip++;
      //printf("err %02x\n", (uchar)buf[0]);
      return 0;
    }
    len = sys_serial_port_recv_safe(ss, buf+1, TESTLEN-1);
    if (len>0) {
      int i, err = 0;
      ++len;
      for (i=0; i<len; ++i) {
        err += ((uchar)buf[i])!=((uchar)i);
      }
      for (i=0; i<16; ++i) {
        printf("%02x ", (uchar)buf[i]);
      }
      printf("%d %d %d\n", len, err, skip);
      skip = 0;
    }
  } else {
    len = sys_serial_port_recv(ss, buf, 256);
    if (len>0) {
      buf[len] = 0;
      printf("%s", buf);
    }
  }
  return 0;
}
#include <io.h>

int test_comport() {
  //dev_desc m_listComPort[8] = {0};
  //int n = sys_enum_comport(m_listComPort, countof(m_listComPort));
  sys_serial_port_t com[1] = {0};
  char buf[TESTLEN];
  //int fn = _open("\\\\.\\COM4", 0);
  int i, len, port = 4, ishex = 0;
#ifndef _WIN32
  port = 7;
#endif
  com->user = &ishex;
  com->recv_message = test_on_recv_message;
  port = sys_serial_port_open(com, port, 115200, 'N', 8, 0);
  if (port>0) {
    printf("port = %d\n", port-1);
    for (i=0; i<1000; ++i) {
      if (!ishex) {
        if (0) {
          if (len = mygetline(buf, countof(buf))) {
            sys_serial_port_send(com, buf, len);
          }
        } else {
          _snprintf(buf, TESTLEN, "%4d %4d 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n", port-1, i);
          len = strlen(buf);
        }
        sys_serial_port_send(com, buf, len);
        //printf("send %s\n", buf);
        sys_sleep(100);
      } else {
        len = strlen(buf);
        for (len=0; len<TESTLEN; ++len) {
          buf[len] = len;
        }
        sys_serial_port_send(com, buf, len);
        //printf("send %s\n", buf);
        sys_sleep(100);
      }
    }
    sys_serial_port_close(com);
  }
  return 0;
}

int test_on_recv_message_file(sys_serial_port_t* ss, UINT Msg)
{
  int ishex = 0;
  int recvlen = 0, lenfn=0, lenfile=0;
  str_t str[1] = {0};
  char fn[256] = {0};
  char magic[] = {0x11, 0x22, 0x33, 0x44};
  char magic2[4] = {0};
  recvlen = sys_serial_port_recv_safe(ss, magic2, 4);
  printf("%d %02x %02x %02x %02x\n", recvlen, magic2[0], magic2[1], magic2[2], magic2[3]);
  if (0 && 4==recvlen && 0==memcmp(magic, magic2, 4)) {
    recvlen = sys_serial_port_recv_safe(ss, &lenfn, sizeof(int));
    recvlen = sys_serial_port_recv_safe(ss, &lenfile, sizeof(int));
    recvlen = sys_serial_port_recv_safe(ss, fn, lenfn);
    str_setsize(str, lenfile);
    recvlen = sys_serial_port_recv_safe(ss, str->s, lenfile);
    //str_save(fn, str);
    printf("%s %d %d\n", fn, lenfile, recvlen);
    str_free(str);
  }
  return 0;
}

int test_serial_port_file() {
  int i;
  //dev_desc m_listComPort[8] = {0};
  //int n = sys_enum_comport(m_listComPort, countof(m_listComPort));
  sys_serial_port_t com[1] = {0};
  int port = 4;
#ifndef _WIN32
  port = 7;
#endif
  com->recv_message = test_on_recv_message_file;
  port = sys_serial_port_open(com, port, 115200, 'N', 8, 0);
  
  if (port>0) {
    printf("port = %d\n", port-1);
    for (i=0; i<1000; ++i) {
      char fn[256];
      if (mygetline(fn, countof(fn))) {
        buf_t bf[1] = {0};
        if (buf_load(fn, bf)) {
          int lenfn = strlen(fn);
          int lenfile = bf->len;
          char magic[] = {0x11, 0x22, 0x33, 0x44};
          sys_serial_port_send(com, magic, 4);
          if (0) {
            sys_serial_port_send(com, &lenfn, sizeof(int));
            sys_serial_port_send(com, &lenfile, sizeof(int));
            sys_serial_port_send(com, fn, lenfn);
            sys_serial_port_send(com, bf->data, lenfile);
          }
          bffree(bf);
        }
      }
      sys_sleep(100);
    }
    sys_serial_port_close(com);
  }
  return 0;
}

int test_A20_cap() {
  int high = 0;
  capdev cap[1] = {0};
  if ((capdshow_open(cap, 0, 640, 320) > 0)) {
    int fram = 0;
    char ch = 0;
    img_t im[1] = {0};
    
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      printf("%d w=%d h=%d\n", fram, im->w, im->h);
      imshow(im);
      ch = cvWaitKey(10);
    }
    
    cap_close(cap);
    imfree(im);
  }
  return 0;
}

//#include "lib/x264/libx264.inl"
//#include "net/sock.inl"
int test_A20() {
  int high = 0;
  int ishex = 0;
  capdev cap[1] = {0};
  sys_serial_port_t com[1] = {0};
  char buf[256];
  int ret = 1;
  com->recv_message = test_on_recv_message;
  ret = sys_serial_port_open(com, 4, 115200, 'N', 8, 0);
  if (ret && (capdshow_open(cap, 0, 640, 320) > 0)) {
    int fram = 0, len;
    char ch = 0;
    img_t im[1] = {0};
    
    for (; 'q' != ch && 'Q' != ch; ++fram) {
      cap_getframe(cap, im, CAP_RGB);
      printf("%d w=%d h=%d\n", fram, im->w, im->h);
      _snprintf(buf, 256, "%d w=%d h=%d\n", fram, im->w, im->h);
      imshow(im);
      len = strlen(buf);
      //sys_serial_port_send(com, buf, len);
      ch = cvWaitKey(10);
    }
    
    cap_close(cap);
    imfree(im);
  }

  return 0;
}

