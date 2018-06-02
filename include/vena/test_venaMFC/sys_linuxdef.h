
#ifndef __LINUXDEF_H__
#define __LINUXDEF_H__


// 计时 (单位为秒, 精确到毫秒)
/*
{char buf[256];utime_start(_start_time);
t=utime_elapsed(_start_time);_snprintf(buf, 256, "%.3f", t);}
{utime_start(_start_time);
printf("%f\n", utime_elapsed(_start_time));}
*/

#include <time.h>
#include <sys/time.h>


static char* _strdate() {
  static char g_strdate[20] = "";
  return g_strdate;
}
static char* _strtime() {
  static char g_strtime[20] = "";
  time_t timep = time(NULL);
  struct tm * p_tm = localtime(&timep);
  strftime(g_strtime, sizeof(g_strtime), "%Y-%m-%d %H:%M:%S", p_tm);   
  return g_strtime;
}
//#include "utime.h"
// CCS中如何看dsp时钟频率
// 一般的TI DSP都有一个CLKOUT管脚，她的变化通常是系统时钟的分频关系，
// 修改它控制寄存器，并输出使能，使用示波器量 再乘以分频比即可。也可以查看PLL的控制寄存器，
// 用相应的计算公式和使用的晶振得出系统时钟。
static double utime_linux_count()
{
  struct timeval tv;
  time_t s;
  double total = 0;
  if (gettimeofday(&tv , 0) == -1) {
    return 0;
  }
  total = tv.tv_sec * 1000000 + tv.tv_usec;
  return total;
}
#define utime_start(_start_time)     double _start_time = utime_linux_count()
#define utime_restart(_start_time)   (_start_time = utime_linux_count())
#define utime_count(_start_time)     (utime_linux_count() - _start_time)
#define utime_elapsed(_start_time)   (utime_count(_start_time) / 1000000.)
//#define utime_start(_start_count)    int _start_count = CLK_gethtime()
//#define utime_elapsed(_start_count)  (double)((CLK_gethtime() - _start_count)*1./CLK_countspms())

#include <unistd.h>

static char* get_exe_path1(char* buf, int count) {
  char link[100];
  snprintf(link, 100, "/proc/%d/exe", getpid());
  readlink(link, buf, count);
  return buf;
}

static char* get_exe_path( char* buf, int count)
{
  int i;
  int rslt = readlink("/proc/self/exe", buf, count - 1);// 注意这里使用的是self
  if (rslt < 0 || (rslt >= count - 1)) {
    return NULL;
  }
  buf[rslt] = '\0';
  for (i = rslt; i >= 0; i--) {
    printf("buf[%d] %c\n", i, buf[i]);
    if (buf[i] == '/')
    {
      buf[i + 1] = '\0';
      break;
    }
  }
  return buf;
}
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define _stat stat
#define MAX_PATH 260
int file_subdir(const char* name)
{
  struct stat st;
  if (lstat(name, &st) < 0) {
    return 0;
  }
  return S_ISDIR(st.st_mode);
}
#define ACCESS access
#define MKDIR(a) mkdir((a),0755)
#define UNEIXST_FILE(_FNAME) find_not_eixst_file(_FNAME, 0, 0)
#define FF_FULLNAME  (1<<0)
#define FF_SUBDIR    (1<<1)
#define FF_NODIR     (1<<3)
typedef struct fileinfo_t {
  char name[MAX_PATH];
  unsigned attrib; // _A_SUBDIR
  time_t ctime;//_create;    /* -1 for FAT file systems */
  time_t atime;//access;    /* -1 for FAT file systems */
  time_t wtime;//write;
  unsigned long long size;
} fileinfo_t;
typedef struct findinfo_t {
  DIR* dir;
  int count;
} findinfo_t;

int find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  struct dirent* d_ent = NULL;
  if (NULL==s->dir) {
    if (!(s->dir = opendir(path))) {
      return 0;
    }
  }
  if ((d_ent = readdir(s->dir)) != NULL) {
    struct stat file_stat;
    if (flag & (FF_FULLNAME|FF_SUBDIR)) {
      snprintf(f->name, MAX_PATH, "%s/%s", path, d_ent->d_name);
    } else {
      strncpy(f->name, d_ent->d_name, MAX_PATH);
    }
    if (lstat(f->name, &file_stat) < 0) {
      //assert(FALSE);
      return 0;
    }
    f->size = file_stat.st_size;
    f->ctime = file_stat.st_ctime;
    f->atime = file_stat.st_atime;
    f->wtime = file_stat.st_mtime;
    f->attrib = file_stat.st_mode;
    return 1;
  }
  closedir(s->dir);
  return 0;
}
#endif // __LINUXDEF_H__
