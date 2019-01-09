
#ifndef __LINUXDEF_H__
#define __LINUXDEF_H__

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "cstd.h"

int sys_debug_puts(const char* str) {
  puts(str);
  return 1;
}
int sys_msize(void* p) {
  return p ? malloc_usable_size(p) : 0;
}
static uint64 sys_db2u64a(double db) {
  suf64_t tf;
  tf.f = db;
  return (tf.u<<32) | (tf.u>>32);
}

int sys_wc2mb(int cp, const wchar_t* s, int nwc, void* d, int nmb) {
  return 0;
}
int sys_mb2wc(int cp, const void* pmb0, int nmb, wchar_t* pwc, int nwc) {
  return 0;
}
//
#define GRADIENT_FILL_RECT_H    0x00000000
#define GRADIENT_FILL_RECT_V    0x00000001
#define GRADIENT_FILL_TRIANGLE  0x00000002
#define GRADIENT_FILL_OP_FLAG   0x000000ff

#define FILE_ACCESS_MODEDEF(a, b) if (mode&a) {mode1|=b;}
static int filemode_to_sysmode(int mode) {
  int mode1=0;
  FILE_ACCESS_MODEDEF(AS_EXIST, F_OK);
  FILE_ACCESS_MODEDEF(AS_EXECUTE, X_OK);
  FILE_ACCESS_MODEDEF(AS_WRITE, W_OK);
  FILE_ACCESS_MODEDEF(AS_READ, R_OK);
  return mode1;
}
#undef FILE_ACCESS_MODEDEF
int sys_access(const char* filename, int mode) {
  //FILE_ACCESS_MODEDEF(AS_SUBDIR, 0);
  return _access(filename, filemode_to_sysmode(mode));
}
int sys_chdir(const char* name) {
  return chdir(name);
}

//#define MAKEWORD(a, b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
//#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((uint32)((WORD)(b))) << 16))
//#define LOWORD(l) ((WORD)(l))
//#define HIWORD(l) ((WORD)(((uint32)(l) >> 16) & 0xFFFF))
//#define LOBYTE(w) ((BYTE)(w))
//#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))

//typedef wchar_t WCHAR;

static char* _strdate() {
  static char g_strdate[20] = "";
  return g_strdate;
}
static char* _strtime(char* g_strtime) {
  //static char g_strtime[20] = "";
  time_t timep = time(NULL);
  struct tm * p_tm = localtime(&timep);
  strftime(g_strtime, sizeof(g_strtime), "%Y-%m-%d %H:%M:%S", p_tm);   
  return g_strtime;
}
static double get_sys_time()
{
  struct timeval tv = {0};
  double total = 0;
  if (gettimeofday(&tv , 0) == -1) {
    //return 0;
  }
  total = (int64)tv.tv_sec * 1000000. + tv.tv_usec;
  return total;
}
//////////////////////////////////////////////////////////////////////////////
int sys_sleep(int ms) {
  usleep(ms*1000);
  return 0;
}
int sys_spin_lock(volatile long *address) {
#if 0
    __asm__ __volatile__(
      "xchgl %0, %1\n"
      : "=r"(ret), "=m"(*address)
      : "0"(1), "m"(*address)
      : "memory");
#endif
}
int sys_spin_unlock(volatile long *address) {
  *address = 0;
}

#if 0
int64 sys_utime_frequency() {
  return 1000000;
}
int64 sys_utime_counter() {
  double t;
  struct timeval tv;
  int64 total = 0;
  if (gettimeofday(&tv , 0) == -1) {
    return 0;
  }
  total = (int64)tv.tv_sec * (int64)1000000 + tv.tv_usec;
  return total;
}
#else
int64 sys_utime_frequency() {
  return 1000000000;
}
int64 sys_utime_counter() {
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (int64)tp.tv_sec * 1000000000 + tp.tv_nsec;
}
#endif
static char* get_exe_path1(char* buf, int count) {
  char link[100];
  snprintf(link, 100, "/proc/%d/exe", getpid());
  readlink(link, buf, count);
  return buf;
}
int sys_tick_count() {
  int64 tick = sys_utime_counter();
  return (int)(tick/1000000);
}
char* sys_errmsg(int dw) {
  dw = dw ? dw : errno;
  return strerror(dw);
}
////////////////////////////////////////////////////////////////////

#include <pthread.h>
typedef struct critical_section_t {
  pthread_mutex_t   m_sect;
} critical_section_t;
int sys_lock_init(lock_t* c) {
  critical_section_t* cs = (critical_section_t*)sys_malloc(sizeof(critical_section_t));
  pthread_mutexattr_t m_attr;
  c->x = cs;
  pthread_mutexattr_init(&m_attr);
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE_NP);
  pthread_mutex_init(&cs->m_sect, &m_attr);
  pthread_mutexattr_destroy(&m_attr);
  return 0;
}
int sys_lock_destroy(lock_t* c) {
  critical_section_t* cs = (critical_section_t*)c->x;
  pthread_mutex_destroy(&cs->m_sect);
  sys_free(cs);
  c->x = NULL;
  return 0;
}
int sys_lock_lock(lock_t* c) {
  critical_section_t* cs = (critical_section_t*)c->x;
  return pthread_mutex_lock(&cs->m_sect);
}
int sys_lock_unlock(lock_t* c) {
  critical_section_t* cs = (critical_section_t*)c->x;
  return pthread_mutex_unlock(&cs->m_sect);
}
#if 0
typedef struct cond_t {
  pthread_cond_t    m_hEvent;
  pthread_mutex_t   m_sect;
  pthread_mutexattr_t m_attr;
} cond_t;
int sys_event_reset(event_t* c) {
  cond_t* cs = (cond_t*)c->x;
  pthread_cond_init(&cs->m_hEvent, 0);
  pthread_mutexattr_init(&cs->m_attr);
  #ifdef PTHREAD_MUTEX_TIMED_NP
  pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_TIMED_NP);
  #endif // PTHREAD_MUTEX_TIMED_NP
  pthread_mutex_init(&cs->m_sect, &cs->m_attr);
  pthread_mutexattr_destroy(&cs->m_attr);
  return 0;
}
int sys_event_init(event_t* c, const char* name) {
  cond_t* cs = (cond_t*)sys_malloc(sizeof(cond_t));
  memset(cs, 0, sizeof(*cs));
  c->x = cs;
  sys_event_reset(c);
  return 0;
}
int sys_event_destroy(event_t* c) {
  cond_t* cs = (cond_t*)c->x;
  pthread_cond_destroy(&cs->m_hEvent);
  pthread_mutexattr_destroy(&cs->m_attr);
  pthread_mutex_destroy(&cs->m_sect);
  sys_free(cs);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int dwMillises) {
  cond_t* cs = (cond_t*)c->x;
  pthread_mutex_lock(&cs->m_sect);
  if (dwMillises<0) {
    pthread_cond_wait(&cs->m_hEvent, &cs->m_sect);
  }
  else {
    struct timespec timeout;
    timeout.tv_sec = 0;
    timeout. tv_nsec = dwMillises * 10000000;
    pthread_cond_timedwait(&cs->m_hEvent, &cs->m_sect, &timeout);
  }
  pthread_mutex_unlock(&cs->m_sect);
  return 0;
}
int sys_event_signal(event_t* c) {
  cond_t* cs = (cond_t*)c->x;
  pthread_mutex_lock(&cs->m_sect);
  pthread_cond_signal(&cs->m_hEvent);
  pthread_mutex_unlock(&cs->m_sect);
  return 0;
}
#else
typedef struct {  
    bool state;  
    bool manual_reset;  
    pthread_mutex_t mutex;  
    pthread_cond_t cond;  
}cond_t;

int sys_event_reset(event_t* c) {
  cond_t* hevent = (cond_t*)c->x;
  if (pthread_mutex_lock(&hevent->mutex) != 0) {  
    return -1;
  }
  hevent->state = false;
  if (pthread_mutex_unlock(&hevent->mutex) != 0) {        
    return -1;  
  }
  return 0;
}
//
int sys_event_init(event_t* c, bool manual_reset, bool init_state) {
  cond_t *hevent = (cond_t*)sys_malloc(sizeof(cond_t));
  hevent->state = init_state;  
  hevent->manual_reset = manual_reset;  
  if (pthread_mutex_init(&hevent->mutex, NULL)) {
    sys_free(hevent);
    return 0;  
  }
  if (pthread_cond_init(&hevent->cond, NULL)) {  
    pthread_mutex_destroy(&hevent->mutex);
    sys_free(hevent);
    return 0;
  }
  c->x = hevent;
  return 1;
}
int sys_event_destroy(event_t* c) {
  cond_t* hevent = (cond_t*)c->x;
  pthread_cond_destroy(&hevent->cond);  
    pthread_mutex_destroy(&hevent->mutex);  
  sys_free(hevent);
  c->x = NULL;
  return 0;
}
int sys_event_wait(event_t* c, int milliseconds) {
  cond_t* hevent = (cond_t*)c->x;
  
  if (milliseconds<0) {
    if (pthread_mutex_lock(&hevent->mutex)) { 
      return -1;
    } 
    while (!hevent->state) { 
      if (pthread_cond_wait(&hevent->cond, &hevent->mutex)) { 
        pthread_mutex_unlock(&hevent->mutex); 
        return -1; 
      }
    }
    if (!hevent->manual_reset) { 
      hevent->state = false; 
    }
    if (pthread_mutex_unlock(&hevent->mutex)) { 
      return -1; 
    } 
    return 0; 
  } else {
    int rc = 0;
    struct timespec abstime; 
    struct timeval tv; 
    gettimeofday(&tv, NULL); 
    abstime.tv_sec = tv.tv_sec + milliseconds / 1000; 
    abstime.tv_nsec = tv.tv_usec*1000 + (milliseconds % 1000)*1000000; 
    if (abstime.tv_nsec >= 1000000000) { 
      abstime.tv_nsec -= 1000000000; 
      abstime.tv_sec++; 
    } 
    
    if (pthread_mutex_lock(&hevent->mutex) != 0) { 
      return -1;
    } 
    while (!hevent->state) {
      if (rc = pthread_cond_timedwait(&hevent->cond, &hevent->mutex, &abstime)) { 
        if (rc == ETIMEDOUT) break; 
        pthread_mutex_unlock(&hevent->mutex); 
        return -1; 
      } 
    } 
    if (rc == 0 && !hevent->manual_reset) { 
      hevent->state = false; 
    } 
    if (pthread_mutex_unlock(&hevent->mutex) != 0) { 
      return -1; 
    } 
    if (rc == ETIMEDOUT) {
      //timeout return 1
      return 1; 
    }
  }
  //wait event success return 0 
  return 0;
}
int sys_event_signal(event_t* c) {
  cond_t* hevent = (cond_t*)c->x;
  if (pthread_mutex_lock(&hevent->mutex) != 0) {
    return -1;
  }
  
  hevent->state = true;
  if (hevent->manual_reset) {
    if(pthread_cond_broadcast(&hevent->cond)) {
      return -1;  
    }  
  } else {
    if(pthread_cond_signal(&hevent->cond)) {  
      return -1;
    }
  }
  
  if (pthread_mutex_unlock(&hevent->mutex) != 0) {  
    return -1;  
  }

  return 0;
}
#endif
int sys_mutex_init(mutex_t* mut, const char* name) {
  //pthread_mutex_init(&mutex,NULL);  
  return 0;
}
int sys_mutex_destroy(mutex_t* mut) {
  return 0;
}
int sys_mutex_lock(mutex_t* mut) {
  return 0;
}
int sys_mutex_unlock(mutex_t* mut) {
  return 0;
}
static void* ThreadProxy(void* args) {
  thread_t* th = (thread_t*)(args);
  int ret = 0;
  UNUSED(ret);
  if (th && th->run) {
    ret = th->run(th->arg);
  }
  return NULL;
}

int sys_thread_create(thread_t* th) {
  pthread_t pth=0;
  int ret = pthread_create(&pth, NULL, ThreadProxy, th);
  th->x = (void*)pth;
  return ret;
}
int sys_thread_join(thread_t* th)
{
  unsigned long m_nRet = 0;
  int ret = 0;
  ret = (0 == pthread_join((pthread_t)(th->x), (void**)&m_nRet));
  return ret;
}
int sys_thread_id() {
  return pthread_self();
}
int sys_getpid() {
  int pid = getpid();
  return pid;
}

////////////////////////////////////////////////////////////////////
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

#define _stat stat
static int file_subdir(const char* name)
{
  struct stat st;
  if (lstat(name, &st) < 0) {
    return 0;
  }
  return S_ISDIR(st.st_mode);
}
int sys_mkdir(const char* fname) {
  return mkdir(fname, 0755);
}

int sys_filestat(const char* file, sys_stat* s) {
  struct stat st;
  int ret = stat(file, &st);
#define COPYMEMBER(_X)  s->_X = st.st_##_X
  COPYMEMBER(dev);
  COPYMEMBER(ino);
  COPYMEMBER(mode);
  COPYMEMBER(nlink);
  COPYMEMBER(uid);
  COPYMEMBER(gid);
  COPYMEMBER(rdev);
  COPYMEMBER(size);
  COPYMEMBER(blksize);
  COPYMEMBER(blocks);
  COPYMEMBER(atime);
  COPYMEMBER(mtime);
  COPYMEMBER(ctime);
#undef COPYMEMBER
  return ret;
}
#define UNEIXST_FILE(_FNAME) find_not_eixst_file(_FNAME, 0, 0)

static int linux_attrib_cvt(int attrib) {
  int attrib2=0;
#define LINUX_ATTRIB_CVT(a, b) if (a(attrib))  attrib2|=(b);
  LINUX_ATTRIB_CVT(S_ISDIR, AS_SUBDIR);
  return attrib2;
}

int sys_find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
  struct dirent* d_ent = NULL;
  filters = filters;
  if (NULL==s->dir) {
    if (!(s->dir = opendir(path))) {
      return 0;
    }
  }
  if ((d_ent = readdir((DIR*)s->dir)) != NULL) {
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
    f->attrib = linux_attrib_cvt(file_stat.st_mode);
    return 1;
  }
  closedir((DIR*)s->dir);
  return 0;
}
int sys_rmfile(const char *pathname) {
  return unlink(pathname);
}
static int sys_rmdir1(const char* lpFileName) {
  return rmdir( lpFileName );
}
//recursively delete all the file in the directory.
int sys_rmdir(const char* dir_full_path)
{
  DIR* dirp = opendir(dir_full_path);    
  if(!dirp) {
    return -1;
  }
  struct dirent *dir;
  struct stat st;
  while((dir = readdir(dirp)) != NULL)
  {
    if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) {
      continue;
    }    
    char sub_path[256];
    _snprintf(sub_path, countof(sub_path), "%s/%s", dir_full_path, dir->d_name);
    if(lstat(sub_path, &st) == -1) {
      printf("sys_rmdir:lstat %s error", sub_path);
      continue;
    }    
    if(S_ISDIR(st.st_mode)) { // 如果是目录文件，递归删除
      if(sys_rmdir(sub_path) == -1) {
        closedir(dirp);
        return -1;
      }
      rmdir(sub_path);
    }
    else if(S_ISREG(st.st_mode)) {
      unlink(sub_path);     // 如果是普通文件，则unlink
    } else {
      printf("sys_rmdir:st_mode %s error", sub_path);
      continue;
    }
  }
  if(rmdir(dir_full_path) == -1)//delete dir itself.
  {
    closedir(dirp);
    return -1;
  }
  closedir(dirp);
  return 0;
}

//实现rm()函数，判断文件类型，如果是目录文件则sys_rmdir，普通文件则unlink.
int rm(const char* file_name)
{
  const char* file_path = file_name;
  struct stat st;    
  if(lstat(file_path,&st) == -1) {
    return -1;
  }
  if(S_ISREG(st.st_mode)) {
    if(unlink(file_path) == -1) {
      return -1;
    }    
  } else if(S_ISDIR(st.st_mode)) {
    if(0 == strcmp(file_name, ".") || 0 == strcmp(file_name, "..")) {
      return -1;
    }
    if(sys_rmdir(file_path) == -1)//delete all the files in dir.
    {
      return -1;
    }
  }
  return 0;
}

int WaitMsg(int delay, bool bWaitMouseMsg) {
  if (delay>0) {
    usleep(delay);
  }
  return 0;
}
int ShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, PixFmt fmt, const void* inpal0) {
  return 0;
}


//#include "sys_linux_serial_port.inl"
//#include "sys_linux_net.inl"

#endif // __LINUXDEF_H__
