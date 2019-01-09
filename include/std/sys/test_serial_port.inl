
#include "cstd.h"
//#include "math/rand.inl"
int my_recv_message_1(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  if (len<100) {
    buf[len] = 0;
    printf("recv(%d)%s", len, buf);
    //for (i=0; i<len; ++i) {    printf(" %d ", buf[i]);  }
    if (0) {
      char buf1[256];
      _snprintf(buf1, countof(buf1), "RE:%s\n", buf);
      int len1 = strlen(buf1);
      sys_serial_port_send(ss, buf1, len1);
    }
  } else {
    int i, err=0;
    printf(":-(%d) %s", len, buf+len-10);
    for (i=0; i<len-1; ++i) {
      char t = (i%10) + '0';
      if (buf[i]!=t) {
        ++err;
      }
    }
    printf("err=%d\n", err);
  }
  return 0;
}
#include "cfile.h"

static int parse_name_featstr(char* buf, const char* key, char** pname, char** pfeatstr) {
  char* p;
  char* p1;
  char* p2;
  if (p = strchr(buf, '=')) {
    *p++ = 0;
    if (0==strcmp(buf, key)) {
      if (p1 = strchr(p, '<')) {
        *p1++ = 0;
        if (p2 = strchr(p1, '>')) {
          *p2 = 0;
          *pname = p;
          *pfeatstr = p1;
          return 1;
        }
      }
    }
  }
  return 0;
}

int my_recv_message_2(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  if (len>1) {
    buf[len] = 0;
    {
      char shortstr[80];
      int len2 = cstr_shortstr(buf, len, shortstr, 60);
      shortstr[len2] = 0;
      printf("recv(%d):%s", len, shortstr);
    }
    char* p=0;
    char* p1=0;
    if (parse_name_featstr(buf, "RE_UP_CHAR", &p, &p1)) {
      char name[256];
      _snprintf(name, 256, "%s.txt", p);
      savefile(name, p1, strlen(p1));
    }
  }
  return 0;
}
int my_recv_message(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  int i;
  if ((ss->pos+len)>=ss->bf->len) {
    int newlen = next_power(ss->pos+len);
    bfsetsize(ss->bf, newlen);
  }
  //printf("%d\n", bf->pos);
  for (i=0; i<len; ++i) {
    uchar t = buf[i];
    ss->bf->data[ss->pos++] = t;
    if ('\n'==t) {
      ss->bf->data[ss->pos] = 0;
      my_recv_message_2(ss, (char*)ss->bf->data, ss->pos, Msg);
      ss->pos = 0;
    }
  }
  return 0;
}

char* facerecogapp_loadfeattxt(const char* name1) {
  char* buf2 = NULL;
  int len2 = 0;
  char name[256];
  str_t str[1] = {0};
  GetFileNameCopy(name1, name, 256);
  str_load(name1, str);
  len2 = str->l+strlen(name)+20;
  MYREALLOC(buf2, len2);
  printf("%s", name1);
  _snprintf(buf2, len2, "DOWN_CHAR=%s<%s>\n", name, str->s);
  str_free(str);
  return buf2;
}
int test_serial_port_up_char(int n, char** argc) {
  return 0;
}

int test_serial_port(int n, char** argc) {
  int asdf = next_power(5);
  if (0) {
    char* buf = "adfasdfasdfafaer asdfasdf asdfasdfasdfasd fasdfas fasdfa dfasdfa sdf asd";
      int len = strlen(buf);
    char shortstr[80];
    int len2 = cstr_shortstr(buf, len, shortstr, 20);
    shortstr[len2] = 0;
    printf("recv(%d):%s", len, shortstr);
  }

  sys_serial_port_t ss[1] = {0};
  char buf[256];
  int port = 7, len, maxlen = 100;
#ifdef _WIN32
  sys_chdir("D:/pub/bin/face");
  port = 3;
#endif
  if (n>1) {
    port = atoi(argc[1]);
  }
  sys_serial_port_open(ss, port, 115200, 'N', 8, 1);
  ss->recv_message = my_recv_message;

  for(; 1; ) {
    const char* s = gets(buf);
    //if ('a'<=*s && *s<='z')
    {
      len = strlen(buf);
      if (buf[0]=='_') {
        maxlen = atoi(buf+1);
      } else if (len<3 && buf[0]=='2') {
        char buf2[256];
        _snprintf(buf2, 256, "UP_CHAR_ALL=%d\n", 1);
        int len2 = strlen(buf2);
        sys_serial_port_send(ss, buf2, len2);
      } else if (len<3 && buf[0]=='3') {
        char buf2[256];
        _snprintf(buf2, 256, "GET_ID_LIST=%d\n", 1);
        int len2 = strlen(buf2);
        sys_serial_port_send(ss, buf2, len2);
      } else if (len<3 && buf[0]=='4') {
        dirlist_t dl[1] = {0};
        int i;
        dirlist(dl, ".\\down", "*.txt", FF_FULLNAME);
        for(i=0; i<dl->n; ++i) {
          char* buf2 = facerecogapp_loadfeattxt(dl->v[i].name);
          int len2 = strlen(buf2);
          sys_serial_port_send(ss, buf2, len2);
          FREE(buf2);
        }
        dirlist_free(dl);
      } else if (len<3 && buf[0]=='1') {
        int j;
        char* buf2 = 0;
        int len2 = maxlen;
        MYREALLOC(buf2, len2+10);
        //randstr(NULL, buf2, len2, "0");
        for (j=0; j<len2; ++j) {
          buf2[j] = (j%10)+'0';
        }
        buf2[j++] = '\n';
        buf2[j] = 0;
        len2 = j;
        sys_serial_port_send(ss, buf2, len2);
        FREE(buf2);
      } else {
        buf[len++] = '\n';
        buf[len] = 0;
        sys_serial_port_send(ss, buf, len);
      }
    }
  }
  return 0;
}

#ifdef _WIN32
#include "sys/sys_win32_serial_port.inl"
#else
#include "sys/sys_linux.inl"
#include "sys/sys_linux_serial_port.inl"
#endif

