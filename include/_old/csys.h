
#ifdef __cplusplus
#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
EXTERN_C_BEGIN;

////////////////////////////////////////////////////////////////////////////////
// sys

#define STRCODEDEF_DEF(STRCODEDEF) \
  STRCODEDEF(GB2312, "中华人民共和国国家标准简体中文字符集") \
  STRCODEDEF(UTF7, "7比特Unicode转换格式") \
  STRCODEDEF(UTF8, "8比特Unicode转换格式") \
  STRCODEDEF(UTF16, "16比特Unicode转换格式") \
  STRCODEDEF(UNICODE, "16比特Unicode转换格式") \
  STRCODEDEF(UTF32, "16比特Unicode转换格式") \
  STRCODEDEF(GBK, "GB13000 汉字内码扩展规范") \
STRCODEDEF(BIG5, "五大码")

typedef enum {
  CE_UNKNOW,
#define STRCODEDEF(a, b)  CE_ ## a ,
  STRCODEDEF_DEF(STRCODEDEF)
#undef STRCODEDEF
}CodePage;
static int sys_wc2mb(int cp, const wchar_t* s, int nwc, void* d, int nmb);
static int sys_mb2wc(int cp, const void* pmb0, int nmb, wchar_t* pwc, int nwc);

// 映射一个unicode字符串到一个多字节字符串
// cp 指定执行转换的代码页
// pwc 指定要转换为宽字节字符串的缓冲区
// nwc 指定由参数pwc指向的缓冲区的字符个数
// pmb 指向接收被转换字符串的缓冲区
// nmb 指定由参数pmb指向的缓冲区最大值
// dc 遇到一个不能转换的宽字符，函数便会使用pDefaultChar参数指向的字符
CC_INLINE int wc2mb(CodePage cp, const wchar_t* s, int nwc, void* d, int nmb)
{
  const wchar_t* pwc = (const wchar_t*)s;
  uchar* pmb = (uchar*)d;
  int i, n = 0;
  switch (cp) {
  case CE_UTF8: {
    if (pmb) {
      for (i = 0; i < nwc; ++i) {
        unsigned short ucs2 = pwc[i];
        if (ucs2 <= 0x007f) {
          pmb[n] = (uchar)ucs2;
          n += 1;/* one char of UTF8 */
        }
        else if ((0x0080 <= ucs2) && (ucs2 <= 0x07ff)) {
          pmb[n + 0] = 0xc0 | (uchar)((ucs2 >> 6) & 0x001f);
          pmb[n + 1] = 0x80 | (uchar)(ucs2 & 0x003f);
          n += 2;/* two char of UTF8 */
        }
        else if ((0x0800 <= ucs2)) {
          pmb[n + 0] = 0xe0 | (uchar)((ucs2 >> 12) & 0x001f);
          pmb[n + 1] = 0x80 | (uchar)((ucs2 >> 6) & 0x003f);
          pmb[n + 2] = 0x80 | (uchar)(ucs2 & 0x003f);
          n += 3;/* three char of UTF8 */
        }
      }
    }
    else {
      for (i = 0; i < nwc; ++i) {
        unsigned short ucs2 = pwc[i];
        if (ucs2 <= 0x007f) {
          n += 1;/* one char of UTF8 */
        }
        else if ((0x0080 <= ucs2) && (ucs2 <= 0x07ff)) {
          n += 2;/* two char of UTF8 */
        }
        else if ((0x0800 <= ucs2)) {
          n += 3;/* three char of UTF8 */
        }
      }
    }
    return n;
  }
    break;
#if 0
  case CE_GB2312:
    {
      const static unsigned short tab_UTF16_GB2312[] = {
#include "str/tab_UTF16_GB2312.txt"
      };
      if (pwc) {
        for (i = 0; i < nwc && n + 1 < nmb; ++i) {
          if (pwc[i] < 128) {
            pmb[n] = (uchar)(pwc[n]);
          }
          else {
            *(unsigned short*)&pmb[n] = tab_UTF16_GB2312[pwc[i]];
            n += 2;
          }
        }
      }
      else {
        for (i = 0; i < nwc; ++i) {
          n += (pwc[i] < 128) ? 1 : 2;
        }
      }
      return n;
    }
    break;
#endif
  default:
    return sys_wc2mb(cp, pwc, nwc, (char*)pmb, nmb);
    break;
  }
  return n;
}
CC_INLINE int mb2wc(CodePage cp, const void* pmb0, int nmb, wchar_t* pwc, int nwc)
{
  int i, n = 0;
  const uchar* pmb = (const uchar*)pmb0;
  switch (cp) {
  case CE_UTF8:
    if (pwc) {
      for (i = 0; i < nmb; ++n) {
        ushort temp = 0;
        if (pmb[i] < 0x80) {
          temp = pmb[i];
          i += 1;/* one char of UTF8 */
        }
        else if ((0xc0 <= pmb[i]) && (pmb[i] < 0xe0)) {
          temp = pmb[i] & 0x1f;
          temp = temp << 6;
          temp = temp | (pmb[i + 1] & 0x3f);
          i += 2;/* two char of UTF8 */
        }
        else if ((0xe0 <= pmb[i]) && (pmb[i] < 0xf0)) {
          temp = pmb[i] & 0x0f;
          temp = temp << 6;
          temp = temp | (pmb[i + 1] & 0x3f);
          temp = temp << 6;
          temp = temp | (pmb[i + 2] & 0x3f);
          i += 3;/* three char of UTF8 */
        }
        else {
          i += 1;
        }
        pwc[n] = temp;
      }
    }
    else {
      for (i = 0; i < nmb; ++n) {
        if (pmb[i] < 0x80) {
          ++i;
        }
        else if ((0xc0 <= pmb[i]) && (pmb[i] < 0xe0)) {
          i += 2;/* two char of UTF8 */
        }
        else if ((0xe0 <= pmb[i]) && (pmb[i] < 0xf0)) {
          i += 3;/* three char of UTF8 */
        }
        else {
          i += 1;
        }
      }
    }
    return n;
  default:
    return sys_mb2wc(cp, (char*)pmb, nmb, (wchar_t*)pwc, nwc);
    break;
  }
  return n;
}
static char* sys_errmsg(int errcode);
//uint64 sys_db2u64(double db);
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
typedef enum {
  AS_EXIST = 0,AS_EXECUTE = 1, AS_WRITE = 2, AS_READ = 4, AS_SUBDIR = 8, AS_RDONLY=16, AS_HIDDEN, AS_WRITE_READ = AS_WRITE|AS_READ
} FILE_ACCESS_MODE;
typedef enum {
  FA_READONLY            = 0x00000001,  
    FA_HIDDEN              = 0x00000002,  
    FA_SYSTEM              = 0x00000004,  
    FA_DIRECTORY           = 0x00000010,  
    FA_ARCHIVE             = 0x00000020,  
    FA_DEVICE              = 0x00000040,  
    FA_NORMAL              = 0x00000080,  
    FA_TEMPORARY           = 0x00000100,  
    FA_SPARSE_FILE         = 0x00000200,  
    FA_REPARSE_POINT       = 0x00000400,  
    FA_COMPRESSED          = 0x00000800,  
    FA_OFFLINE             = 0x00001000,  
    FA_NOT_CONTENT_INDEXED = 0x00002000,  
FA_ENCRYPTED           = 0x00004000,  
} FILE_ATTRIBUTE;
#undef S_ISDIR
#define S_ISDIR(x)  ((x)&AS_SUBDIR)
static int sys_access(const char* file, int mode);
static int sys_mkdir(const char* file);
static int sys_chdir(const char* file);
static char* sys_get_exe_path(char* buf, int count);
static int sys_get_file_attributes(const char* fn);
static char* sys_tmpname(const char* fname);
static char* sys_temp_path(char* fname, int len);

enum SYS_MODE {
AS_IFMT  = 0170000,  /* file type mask */
AS_IFDIR = 0040000,  /* directory */
AS_IFCHR = 0020000,  /* character special */
AS_IFIFO = 0010000,  /* pipe */
AS_IFREG = 0100000,  /* regular */
AS_IREAD = 0000400,  /* read permission, owner */
AS_IWRITE= 0000200,  /* write permission, owner */
AS_IEXEC = 0000100,  /* execute/search permission, owner */

AS_ISUID = 04000, //文件的 (set user-id on execution)位
AS_ISGID = 02000, //文件的 (set group-id on execution)位
AS_ISVTX = 01000, //文件的sticky 位
AS_IRUSR = 00400, //文件所有者具可读取权限
AS_IWUSR = 00200, //文件所有者具可写入权限
AS_IXUSR = 00100, //文件所有者具可执行权限
AS_IRGRP = 00040, //用户组具可读取权限
AS_IWGRP = 00020, //用户组具可写入权限
AS_IXGRP = 00010, //用户组具可执行权限
AS_IROTH = 00004, //其他用户具可读取权限
AS_IWOTH = 00002, //其他用户具可写入权限
AS_IXOTH = 00001, //其他用户具可执行权限
};

typedef struct {
  uint dev;      //文件的设备编号
  uint ino; //节点
  uint mode; //文件的类型和存取的权限
  int nlink;  //连到该文件的硬连接数目，刚建立的文件值为1
  int uid;//用户ID
  int gid;//组ID
  uint rdev; //(设备类型)若此文件为设备文件，则为其设备编号
  uint64 size;//文件字节数(文件大小)
  uint blksize;   //块大小(文件系统的I/O 缓冲区大小)
  uint blocks;    //块数
  time_t atime; //最后一次访问时间
  time_t mtime;//最后一次修改时间
  time_t ctime;//最后一次改变时间(指属性)
  unsigned attrib; // _A_SUBDIR
} sys_stat;

static int sys_filestat(const char* file, sys_stat* s);
#ifndef MAX_PATH
#define MAX_PATH 260
#endif // MAX_PATH
typedef struct fileinfo_t {
  unsigned attrib; // _A_SUBDIR
  time_t ctime;//_create;    /* -1 for FAT file systems */
  time_t atime;//access;    /* -1 for FAT file systems */
  time_t wtime;//write;
  uint64 size;
  char name[MAX_PATH];
} fileinfo_t;
typedef struct findinfo_t {
  void* dir;
  int count;
} findinfo_t;
#define FF_FULLNAME  (1<<0)
#define FF_SUBDIR    (1<<1)
#define FF_NODIR     (1<<3)
#define FF_FILTERS   (1<<4)
static int sys_find_next_file(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag);
static int find_next_file_utf8(findinfo_t* s, const char* path, const char* filters, fileinfo_t* f, int flag);

static int sys_gettime(sys_time_t* st);
static int sys_setfiletime(const char* name, const sys_time_t* ctime, const sys_time_t* atime, const sys_time_t* mtime);
static int sys_getfiletime(const char* name, sys_time_t* ctime, sys_time_t* atime, sys_time_t* mtime);
//void timet2systime(const lutime64_t t, sys_time_t* st);
static void timet2systime(const lutime64_t t, sys_time_t* pst)
{
  time_t t2 = (time_t)t;
  struct tm* ptm = localtime(&t2);
  pst->year = ptm->tm_year + 1900;
  pst->month=ptm->tm_mon+1;
  pst->day=ptm->tm_mday;
  pst->hour=ptm->tm_hour;
  pst->minute=ptm->tm_min;
  pst->second=ptm->tm_sec;
  return ;
}

#define SEEK_BEG SEEK_SET
#if 0
{utime_start(_start_time);
t=utime_elapsed(_start_time);}
{char buf[256];utime_start(_start_time);
t=utime_elapsed(_start_time);_snprintf(buf, 256, "%.3f", t);}
{utime_start(_start_time);
printf("%f\n", utime_elapsed(_start_time));}
#endif

#define TIC {utime_start(_start_time)
#define TOC printf("%f\n", utime_elapsed(_start_time));}
#define utime_elapsed2(_start_time, _start_time1)   ((_start_time1 - _start_time) * 1.0 / sys_utime_frequency())
#define utime_elapsed(_start_time)   utime_elapsed2(_start_time, sys_utime_counter())
#define utime_restart(_start_time)   _start_time = sys_utime_counter()
#define utime_start(_start_time)     int64 _start_time = sys_utime_counter()
static int sys_tick_count();
static int64 sys_utime_counter();
static int64 sys_utime_frequency();
static int sys_trace_dbg(const char* fmt, ...);
#ifdef _MSC_VER
#define _trace_dbg sys_trace_dbg
#else
#define _trace_dbg printf
#endif
static int sys_set_clipboard_text(const char* str, int nStrLen);
static int sys_get_clipboard_text(char* str, int nMaxStrLen);
static char* sys_get_clipboard_text_static();
static int sys_set_cursor(int w, int h, int hotx, int hoty, void* pix);
static int sys_set_cursor_image(int h, int w, void* data, int step, int cn, int x_hotspot, int y_hotspot);
typedef enum {FOF_MOVE,FOF_COPY,FOF_DELETE,FOF_RENAME} FileOptFlag;
static int sys_fileopt(FileOptFlag opt, const char* strFrom, const char* strTo);
static int sys_cpfile(const char* lpExistingFileName, const char* lpNewFileName, int bFailIfExists);
static int sys_rmfile(const char* lpFileName);
static int sys_rmdir(const char* lpFileName);
static int sys_mvfile(const char* lpExistingFileName, const char* lpNewFileName);
static int sys_msgbox(const char* fmt, ...);
////////////////////////////////////////////////////////////////////////////////
typedef struct thread_t thread_t;
typedef int (*thread_callback)(void* arg);
typedef int (*parallel_callback)(void* arg, int beg, int end);
#define CC_INFINITE 0xffffffff
#define PUSHARG(args, j, T, NAME)     args[j++] = &NAME;
#define POPARG(args, j, T, NAME)      T& NAME = *(T*)args[j++];
static int parallel_for(void* arg, int beg, int end, parallel_callback fun);

struct thread_t {
  thread_callback run;
  void* arg;
  void* security;
  unsigned stack_size;
  unsigned initflag;
  unsigned threadID;
  void* x;
  void* data[4];
  struct thread_t* prev;
  struct thread_t* next;
};
typedef struct thread_t job_t;
typedef struct {
  void* x;
} mutex_t;
typedef struct {
  void* x;
} event_t;
typedef struct {
  void* x;
} lock_t;
static int sys_sleep(int ms);
static int sys_spin_lock(volatile long *address);
static int sys_spin_unlock(volatile long *address);
static int sys_lock_init(lock_t* c);
static int sys_lock_destroy(lock_t* c);
static int sys_lock_lock(lock_t* c);
static int sys_lock_unlock(lock_t* c);
static int sys_event_init(event_t* c, bool manual_reset, bool init_state);
static int sys_event_destroy(event_t* c);
static int sys_event_wait(event_t* c, int dwMillises);
static int sys_event_signal(event_t* c);
static int sys_event_reset(event_t* c);
static int sys_mutex_init(mutex_t* mut, const char* name);
static int sys_mutex_destroy(mutex_t* mut);
static int sys_mutex_lock(mutex_t* mut);
static int sys_mutex_unlock(mutex_t* mut);
static int sys_thread_create(thread_t* th);
static int sys_thread_join(thread_t* th);
static int sys_thread_id();
static int sys_getpid();


#define TT_LINE       0
#define TT_QSPLINE    1
#define TT_CSPLINE    2

typedef struct {
  char x;
  char y;
} pointi8;

typedef struct {
  uchar type;
  uchar  npt;
  pointi8 pt[1];
} ttpoly;

typedef struct {
  ushort cb;
  pointi8 start;
} ttpolygon;


typedef struct {
  uchar   gmBlackBoxX;
  uchar   gmBlackBoxY;
  pointi8 gmptGlyphOrigin;
  short   gmCellIncX;
  short   gmCellIncY;
} glyphmetrics;

typedef struct {
  float size;
  float* IndexXAdvance;
  int IndexXAdvance_Size;
  float FallbackXAdvance;
} FontInfo;

typedef struct ttffont_t_ ttffont_t;
struct ttffont_t_ {
  void* x;
  char name[32];
  FontInfo info[1];
  int (*get_glyph)(const ttffont_t* f, uint code, char* obuf, int olen);
  int (*del)(ttffont_t* f);
};
#define ttffont_get_glyph(f, code, obuf, olen)  (f)->get_glyph(f, code, obuf, olen)
#define ttffont_del(f)  (f)->del(f)
static int sys_font_set(ttffont_t* f, const char* fontname, int style);
static int sys_debug_puts(const char* str);
///////////////////////////////////////////////////////////////////////////
static int NamedWindow(const char* name, int flags);

#define waitkey(delay)  WaitMsg(delay, false)
static int WaitMsg(int delay, bool bWaitMouseMsg);
static int ShowImagePal(const char* name, int height, int width, const unsigned char* arr, int step, int cn, PixFmt fmt, const void* inpal);
#define ShowImage(n, h, w, a, al, ai) ShowImagePal(n, h, w, a, al, ai, PixFmtMax, 0)
#define imwritedat2(_FN, im)  imwritedat( _FN, im->h, im->w, im->tt.data, im->s, im->c )
#define imwriterect( _FN, img, al, ai, x, y, cx, cy ) imwrite(_FN, cy, cx, img+y*al+x*ai, al, ai)
#define imwriterect2( _FN, img, al, ai, _RC ) imwriterect(_FN, img, al, ai, (_RC).x, (_RC).y, (_RC).cx, (_RC).cy)
#define imwrite2txt(_FN, _IM) FPRINT2D(_FN, _T("%d,\r\n"), _T(""), (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
#define img2txt( in, out ) { img_t * im = imread2( in, 0 ); imwrite2txt( out, im ); imfree( im ); }
#define imshowpal(_NAME, _IM, pal) ShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, pal)
#define imshow_(_IM) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, PixFmtMax, 0)
#define imshow_f(_IM, f) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data+f*(_IM)->h*(_IM)->s, (_IM)->s, (_IM)->c, PixFmtMax, 0)
#define imshowfmt(_IM, fmt) ShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, fmt, 0)
#define imshowrc(_IM, rc) ShowImagePal(#_IM, RCH(&rc), RCW(&rc), (_IM)->tt.data + (_IM)->s*(rc).t + (_IM)->c*(rc).l, (_IM)->s, (_IM)->c, 0)
#define imshow2(_NAME, _IM) ShowImage(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
#define imshow_cn(_NAME, _IM, icn) ShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data+icn, (_IM)->s, (_IM)->c, PF_8bppGrayUchar, NULL)
#define imwrite3(_IM) imwrite(#_IM".bmp", (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
//#define imshow imshow_

static int cvShowMat(const char* name, const char* fmt, int h, int w, const void* arr, int al, int ai, int nColumnWidth /*= -1*/);
#define imshowmat_u1(mm) cvShowMat( #mm , "%I8u", mm->h, mm->w*mm->c, mm->tt.data, mm->s, 1, -1)
#define imshowmat_u4(mm) cvShowMat( #mm , "%I32u", mm->h, mm->w*mm->c/4, mm->tt.data, mm->s, 4, -1)
#define imshowmat_x4(mm) cvShowMat( #mm , "%08x", mm->h, mm->w*mm->c/4, mm->tt.data, mm->s, 4, -1)
#define imshowmat_u1_cn(mm, icn) cvShowMat( #mm , "%I8u", mm->h, mm->w, mm->tt.data+icn, mm->s, mm->c, -1)
#define imshowmat_f4(mm) cvShowMat( #mm , "%I32f", mm->h, mm->w, mm->tt.f8, mm->s, mm->c, -1)
#define imshowmat_f8(mm) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.f8, mm->s, mm->c, -1)
#define imshowmat_f8_cn(mm, icn) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.f8+icn, mm->s, mm->c, -1)
#define imshowmat_f8_f(mm, f) cvShowMat( #mm , "%I64f", mm->h, mm->w, mm->tt.data+(f)*mm->w*mm->s, mm->s, mm->c, -1)
#define cvShowMat_f4(mm, h, w) cvShowMat( #mm , "%I32f", h, w, mm, w*sizeof(float), sizeof(float), -1)

typedef int (* TrackbarCB)(int pos);
typedef int (* MouseCB)(int event, int x, int y, int flags, void* param);
typedef int (* TrackbarCB2)(int pos, void* userdata);

static int CreateTrackbar(const char* trackbar_name, const char* window_name, int* val, int count, TrackbarCB on_notify);

///////////////////////////////////////////////////////////////////////////
static int sys_reg_save_data(const char* keyname, const char* dataname, const void* pdata, int len);
static int sys_reg_load_data(const char* keyname, const char* dataname, void* pdata, int len);
///////////////////////////////////////////////////////////////////////////
//  串口枚举 (注意被占用问题)
typedef struct {
  char name[32];
} dev_desc;
static int sys_enum_comport(dev_desc* m_listComPort, int maxport);

enum {
  FO_RDONLY = 1, // 只读打开
    FO_WRONLY = 2,// 只写打开
    FO_RDWR = FO_RDONLY|FO_WRONLY,// 可读可写打开
    FO_APPEND = 1<<2,// 追加
    FO_CREAT = 1<<3,// 不存在则创建
    FO_EXCL = 1<<4,// 如果同时指定了O_CREAT，并且文件已存在，则出错返回
    FO_TRUNC = 1<<5,// 如果文件已存在，并且以只写或可读可写方式打开，则将其长度截断（Truncate）为0字节
    FO_NOCTTY = 1<<6,// 
    FO_NDELAY = 1<<7,// 
    FO_NONBLOCK = 1<<8,// 
};

typedef struct sys_file_t_ sys_file_t;
struct sys_file_t_ {
  void* x;
  int (*close)(sys_file_t* f);
};
#define sys_close(f)  f->close(f)

typedef struct sys_serial_port_t_ sys_serial_port_t;
typedef int (*recv_message_f)(sys_serial_port_t* s, char* buf, int len, UINT Msg);
#define sys_serial_port_SendMessage(ss, buf, len, Msg)  if (ss->recv_message) {ss->recv_message(ss, buf, len, Msg);}

struct sys_serial_port_t_ {
  buf_t bf[1];
  int pos;
  void* x;
  // owner window
  void* user;
  recv_message_f recv_message;
  sendbuf_t* m_szWriteBufferList;// 写缓冲区
  int (*close)(sys_serial_port_t* s);
  int (*send)(sys_serial_port_t* s, const void* data, int n);
  int (*recv)(sys_serial_port_t* s, void* data, int n);
};
#define sys_serial_port_recv(s, data, n)  s->recv(s, data, n)
#define sys_serial_port_send(s, data, n)  s->send(s, data, n)
#define sys_serial_port_close(s)  s->close(s)
static int sys_serial_port_open(sys_serial_port_t* ss, UINT portnumber, UINT baud,
                          char parity, UINT databits, UINT stopbits);

///////////////////////////////////////////////////////////////////////////

static char* sys_open_filename(const char* szFilter, char* szFile, int len);
static char* sys_browse_folder(const char* title, char* buf, int len);

///////////////////////////////////////////////////////////////////////////

// mode
enum {
  CAP_GRAY,   //
    CAP_RGB,    //RGB
    CAP_DEPTH,  //
};

typedef struct cap_t {
  void* x;
  int (*getframe)(struct cap_t* s, img_t* im, int flag);
  int (*close)(struct cap_t* s);
} cap_t;
typedef cap_t capdev;
//#define cap_num(mode)  cap_open(NULL, 0, mode)
#define cap_getframe(s, im, flag)  (s)->getframe(s, im, flag)
#define cap_close(s)  (s)->close(s)


///////////////////////////////////////////////////////////////////////////

EXTERN_C_END;

