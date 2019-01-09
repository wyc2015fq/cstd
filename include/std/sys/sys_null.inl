
#include <stdio.h>
#ifdef __linux__
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#endif


int sys_filestat(const char* file, sys_stat* s) {
  return 0;
}

int64 sys_utime_frequency() {
  return 1000;
}
int64 sys_utime_counter() {
  return clock();
}
int sys_debug_puts(const char* str) {
  puts(str);
  return 0;
}
int sys_mb2wc(int cp, const void* pmb0, int nmb, wchar_t* pwc, int nwc) {
  int n = 0;
  switch (cp) {
  case CE_GB2312:
    return n;
  default:
    break;
  }
  return 0;
}
int sys_wc2mb(int cp, const wchar_t* s, int nwc, void* d, int nmb) {
  return 0;
}

int sys_access(const char* filename, int mode) {
  return 0;
}
int cvWaitMsg(int delay, bool bWaitMouseMsg) {
  return 0;
}
int cvShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, PixFmt fmt, const void* inpal0) {
  return 0;
}
int sys_mkdir(const char* file) {
  return 0;
}
int sys_gettime(sys_time_t* st) {
  return 0;
}

int sys_find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag) {
#ifdef __linux__
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
    f->attrib = (file_stat.st_mode);
    return 1;
  }
  closedir((DIR*)s->dir);
#endif
  return FALSE;
}

int sys_tick_count() {
  return clock();
}
