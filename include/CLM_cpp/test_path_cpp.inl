

#include <assert.h>
using namespace std;

int get_filename_pos(const string& fn)
{
  int pos = fn.find_last_of('/');

  if (pos < 0) {
    pos = fn.find_last_of('\\');
  }

  if (pos < 0) {
    return -1;
  }

  return pos;
}
string parent_path(const string& fn)
{
  int pos = get_filename_pos(fn);
  return fn.substr(0, pos >= 0 ? pos : 0);
}

string filename(const string& fn)
{
  int pos = get_filename_pos(fn);
  return fn.substr(pos >= 0 ? (pos + 1) : 0);
}

string extension(const string& fn)
{
  int pos = fn.find_last_of('.');
  return fn.substr(pos >= 0 ? (pos) : 0);
}

string replace_extension(const string& fn, const string& new_ext)
{
  int pos = fn.find_last_of('.');

  if (pos >= 0) {
    return fn.substr(0, pos) + "." + new_ext;
  }
  else {
    return fn + "." + new_ext;
  }
}

//#include "cfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <io.h>
#include <direct.h>
#include <sys/stat.h>

bool exists(const string& fn)
{
  // file_exist
  int ret = _access(fn.c_str(), _A_NORMAL);
  return ret >= 0;
}

bool is_directory(const string& path)
{
  struct stat info;
  int ret = stat(path.c_str(), &info);
  int IsDirPath = 0;

  if (ret >= 0) {
    ret = (_A_SUBDIR & info.st_mode);
    IsDirPath = (_S_IFDIR == (_S_IFDIR & info.st_mode));
  }

  return IsDirPath;
}

int directory_list(const string& path, vector<string>& vs)
{
  int vcstr_free(char** vstrs);
  int dirlist_c(char** * plist, const char * path, const char * filters, int flag);
  char** list = NULL;
  int i = 0;
  dirlist_c(&list, path.c_str(), NULL, 0);

  for (; list[i]; ++i) {
    vs.push_back(list[i]);
  }

  vcstr_free(list);
#if 0
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  int flag = FF_FULLNAME;
  const char* path1 = path.c_str();

  for (; sys_find_next_file(finfo, path1, NULL, info, flag);) {
    vs.push_back(info->name);
  }

#endif
  return 0;
}

int test_path_cpp()
{
  std::string aa = "D:\\code\\CLM/CLM.inl";
  std::string aa1 = "D:\\code\\CLM/CLM.inl";
  string bb = parent_path(aa);
  string bb1 = parent_path(aa1);
  assert(bb == "D:\\code\\CLM");
  assert(bb == bb1);
  string cc = filename(aa);
  string cc1 = filename(aa1);
  assert(cc == "CLM.inl");
  assert(cc == cc1);
  string dd = replace_extension(aa, "txt");
  string dd1 = replace_extension(aa, "h");
  assert(dd == "D:\\code\\CLM/CLM.txt");
  assert(dd1 == "D:\\code\\CLM/CLM.h");
  int exists_ret0 = exists(dd);
  int exists_ret1 = exists(dd1);
  assert(0 == exists_ret0);
  assert(1 == exists_ret1);
  int dir_ret0 = is_directory(dd);
  int dir_ret1 = is_directory(bb);
  assert(0 == dir_ret0);
  assert(1 == dir_ret1);
  vector<string> vs;
  directory_list(bb, vs);
  return 0;
}
extern "C" {
  int sys_mkdir(const char* file);
}

//#include "D:/code/CLM/sys.cpp"
