// Precompiled headers stuff

#ifndef __STDAFX_h_
#define __STDAFX_h_

#define safe_strlen(s)   ((s) ? strlen(s) : 0)
// C++ stuff
#include <stdio.h>

#define _USE_MATH_DEFINES
//#include <cmath>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define myparallel_for(_b, _e, _i)  for (int _i=_b; _i<_e; ++_i )



// dlib stuff
// Used for face detection
//#define _GET_POSE
#define _USE_CVFACE
#define _USE_MYFACE
#define _USE_OPENCV
#include "cstd.h"
//#define _USE_DLIB
#ifdef _USE_DLIB
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/opencv.h>
#endif

using namespace std;

// #define _USE_BOOST
#ifdef _USE_BOOST
#else

#include <assert.h>
//using namespace std;
//typedef std::char* path;


#if 0
static char* tostring(const std::char* s)
{
  return s;
}

static char* operator /(const char* a, const char* b)
{
  return a + "/" + b;
}
#endif

static int get_filename_pos(const char* fn)
{
  char* pos = strrchr(fn, '/');

  if (!pos) {
    pos = strrchr(fn, '\\');
  }

  if (!pos) {
    return -1;
  }

  return pos - fn;
}
static char* substr(const char* fn, int beg, int len, char* buf, int maxlen) {
  len = MIN(maxlen-1, len);
  memcpy(buf, fn + beg, len);
  buf[len] = 0;
  return buf;
}
static char* parent_path(const char* fn, char* buf, int maxlen)
{
  int pos = get_filename_pos(fn);
  return substr(fn, 0, pos >= 0 ? pos : 0, buf, maxlen);
}

static char* filename(const char* fn)
{
  int pos = get_filename_pos(fn);
  return (char*)fn + (pos >= 0 ? (pos + 1) : strlen(fn));
}

static char* extension(const char* fn)
{
  char* pos = strrchr(fn, '.');
  return (pos ? (pos) : 0);
}

static char* replace_extension(const char* fn, const char* new_ext, char* buf, int maxlen)
{
  char* pos = strrchr(fn, '.');
  
  if (pos) {
    substr(fn, 0, pos-fn, buf, maxlen);
  }
  else {
    strncpy(buf, fn, maxlen);
  }
  strcat(buf, ".");
  strcat(buf, new_ext);
  return buf;
}

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

static bool mycreate_directories(const char* file)
{
#ifdef _WIN32
  return 0 == _mkdir(file);
#else
  return 0 == mkdir(file, 0755);
#endif
}

static bool path_exists(const char* path)
{
  // file_exist
  struct stat info;
  int ret = stat(path, &info);
  return ret >= 0;
}

static bool path_is_directory(const char* path)
{
  int IsDirPath = 0;
#ifdef _WIN32
  struct stat info;
  int ret = stat(path, &info);

  if (ret >= 0) {
    IsDirPath = (_S_IFDIR == (_S_IFDIR & info.st_mode));
  }

#else
  struct stat info;
  int ret = stat(path, &info);

  if (ret >= 0) {
    IsDirPath = (S_IFDIR == (S_IFDIR & info.st_mode));
  }

#endif
  return IsDirPath!=0;
}
static char* pstrresize(char** pstr, int n) {
  *pstr = (char*)realloc(*pstr, n+1);
  (*pstr)[n] = 0;
  return (*pstr);
}
static char* pstrcat(char** pstr, const char* s1) {
  int l = safe_strlen(*pstr), l1 = safe_strlen(s1);
  pstrresize(pstr, l + l1);
  memcpy((*pstr) + l, s1, l1);
  return *pstr;
}
static char* pstrcat2(char** pstr, const char* s1, const char* s2) {
  int l = safe_strlen(*pstr), l1 = safe_strlen(s1), l2 = safe_strlen(s2);
  pstrresize(pstr, l + l1 + l2);
  memcpy((*pstr) + l, s1, l1);
  memcpy((*pstr) + l+l1, s2, l2);
  return *pstr;
}
static char* pstrcat3(char** pstr, const char* s1, const char* s2, const char* s3) {
  int l = safe_strlen(*pstr), l1 = safe_strlen(s1), l2 = safe_strlen(s2), l3 = safe_strlen(s3);
  pstrresize(pstr, l + l1 + l2 + l3);
  memcpy((*pstr) + l, s1, l1);
  memcpy((*pstr) + l+l1, s2, l2);
  memcpy((*pstr) + l+l1+l2, s3, l3);
  return *pstr;
}
static int ppstrlen(char** const ppstr) {
  int i=0;
  if (ppstr) {
    for (;(ppstr)[i];++i);
  }
  return i;
}
char** ppstr_reset(char*** ppstr, int n) {
  int l = ppstrlen(*ppstr);
  for (;l>n;) {
    --l;
    free((*ppstr)[l]);
    (*ppstr)[l] = NULL;
  }
  if (n>0) {
    *ppstr = (char**)realloc(*ppstr, (n+1)*sizeof(char*));
    for (; l<=n; ++l) {
      (*ppstr)[l] = NULL;
    }
  } else {
    free(*ppstr);
    *ppstr = NULL;
  }
  return (*ppstr);
}
char** ppstr_free(char*** ppstr) {
  ppstr_reset(ppstr, 0);
  return *ppstr;
}
static char** ppstrnew(char*** ppstr) {
  int l = ppstrlen(*ppstr);
  ppstr_reset(ppstr, l+1);
  return (*ppstr)+l;
}
int strendof(const char* s, const char* ext) {
  int l = strlen(s), l2 = strlen(ext);
  return l>l2 && 0==memicmp(s+l-l2, ext, l2);
}
static int dirlist(const char* dir1, char*** file_in_directory, const char* ext)
{
#ifdef _WIN32
  _finddata_t fileinfo;
  memset(&fileinfo, 0x0, sizeof(fileinfo));
  char* buf = NULL;
  char* dir = pstrcat2(&buf, dir1, "/*");
  intptr_t iFind = _findfirst(dir, &fileinfo);

  if (iFind == -1) {
    printf("error: function _findfirst failed.\n");
    exit(-1);
  }

  *buf = 0;
  char* filePath = pstrcat3(&buf, dir1, "/", fileinfo.name);
  if (strendof(filePath, ext)) {
    pstrcat(ppstrnew(file_in_directory), filePath);
  }
  
  // cout << "name: " << filePath << endl;
  while (_findnext(iFind, &fileinfo) == 0) {
    *buf = 0;
    char* filePath = pstrcat3(&buf, dir1, "/", fileinfo.name);
    //cout << "name: " << fileinfo.name << endl;
    if (strendof(filePath, ext)) {
      pstrcat(ppstrnew(file_in_directory), filePath);
    }
  }

  _findclose(iFind);
  free(buf);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	const char* dir = dir1;
	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "cannot open directory : %s\n", dir);       //判断dir是否是个文件夹
		return 0;
	}
	while ((entry = readdir(dp)) != NULL) {              //一直读取dir里面的文件直到最后一个文件。
		lstat(entry->d_name, &statbuf);
		printf("%s\n", entry->d_name);
		if (0 && S_IFDIR == (S_IFDIR & statbuf.st_mode)) {
			/* Found a directory, but ignore . and .. */
			if (strcmp(".", entry->d_name) == 0 ||
				strcmp("..", entry->d_name) == 0)
				continue;
			//printf("%*s%s / \n", depth, "", entry->d_name);
			/* Recurse at a new indent level */
			//printdir(entry->d_name, depth + 4);     //递归调用，文件夹下面的文件再查找
		}
		else {
			//printf("%*s%s\n", depth, "", entry->d_name);
			char* filePath = dir1 + "/" + entry->d_name;
			//cout << "name: " << fileinfo.name << endl;
			file_in_directory.push_back(filePath);
		}
	}
	closedir(dp);
#endif
  return 0;
}

#endif





//int asdf = -0;
#endif
