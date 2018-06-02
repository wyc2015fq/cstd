
#include "cstd.h"
#include "str_format_len.inl"
#include "cfile.h"

int test_find_next_file_utf8() {
  findinfo_t finfo[1] = {0};
  fileinfo_t info[1] = {0};
  const char* path = "G:/Youku";
  path = "G:/Youku";
  _chdir(path);
  for (; find_next_file_utf8(finfo, path, "*.mp4", info, 0);) {
    char* name = GetFileNameExt(info->name);
    utf8_printf("%s\n", info->name);
    if (0) {
      WCHAR wbuf[256] = {0};
      int len = MultiByteToWideChar( CP_UTF8, 0, info->name, -1, wbuf, 256 );
      HANDLE hFile;//文件句柄
      FILE* pf = _wfopen(wbuf, L"rb");
      hFile=CreateFileW(
        wbuf,//创建或打开的文件或设备的名称(这里是txt文件)。
        GENERIC_READ,// 文件访问权限,写
        0,//共享模式,这里设置0防止其他进程打开文件或设备
        NULL,//SECURITY_ATTRIBUTES结构，安全描述，这里NULL代表默认安全级别
        OPEN_EXISTING,//对于存在或不存在的设置执行的操作，这里是始终创建
        FILE_ATTRIBUTE_NORMAL,//设置文件的属性，里面有高速缓存的选项
        NULL);
      printf("%s\n", GetErrorMsg(0));
    }
  }
  return 0;
}
int test_codeformat() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  _chdir("E:/book");
  test_find_next_file_utf8();
  dirlist(dir, "E:/book", "*.pdf", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}
