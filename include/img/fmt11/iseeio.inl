/********************************************************************

  iseeio.c

  ----------------------------------------------------------------
    软件许可证 － GPL
  版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
  ----------------------------------------------------------------
  这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
  可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
  据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
  目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
  GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
  ----------------------------------------------------------------
  如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

      http://isee.126.com
      http://cosoft.org.cn/projects/iseeexplorer

  或发信到：

      isee##vip.163.com
  ----------------------------------------------------------------
  本文件用途：  ISee I/O 实现文件（兼容标准：ANSI C 流I/O）
  本文件编写人：
          YZ      yzfree##sina.com
          流浪者    chenjm##netease.com
          梦晖    mssoft##sina.com
          lzp     zlan##corp.netease.com

  本文件版本：  30912
  最后修改于：  2003-09-12

  注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
    地址收集软件。
    ----------------------------------------------------------------
  修正历史：

    2003-9    模块结构改为在函数指针数组的方式下运行。
          增加插入新介质接口的函数，使模块可以在二进制方式下动态支持新介质。
    2003-7    修改内存流生长方式（由定长生长改为按指数生长，以减少重分配内存的次数）
    2003-7    修正isio_error函数ISEEIO_CONFER_LOC_FILE_SEQUENTIAL类型代码中
          的BUG（原代码将非文件类读写错误也作为文件类错误返回）
    2003-6    修正isio_seek中当lDistanceToMove为负数时出现的错误(梦晖 mssoft#sina.com)
          (错误描述:SetFilePointer的lpDistanceToMoveHigh，被设置成指向0的指针，
          当lDistanceToMove为负数时SetFilePointer认定用户指定的为64位数，
          而此时高32位都为0……，呵呵。)
    2002-11   增加一种顺序读或写文件方式，以利用操作系统提供的
          文件IO高速缓冲功能，目前只支持WIN32。(流浪者)
    2002-10   修正内存流入口参数含义，及引入内存防护系统。
    2002-8    修正联络电邮地址、及编译模式
    2002-4    修正文件缺省打开模式（定为二进制模式）
    2001-6    发布第一个测试版


********************************************************************/


#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "iseemem.h"
#include "iseeio.h"


#ifdef _DEBUG
static int __isfile_object_counts = 0;
#endif

#if defined(WIN32) && defined(_MSC_VER)
#include <fcntl.h>  /* 设定文件打开模式时需要的一些宏 */
static int old_fmode;
#endif

/* 介质接口信息结构 */
static ISIOINFAC  _isio_ifc;


#ifndef INVALID_SET_FILE_POINTER
#define INVALID_SET_FILE_POINTER 0XFFFFFFFF
#endif

/* 内存块扩展因子 */
#define MVA     2


/* 内部函数 */
void        CALLAGREEMENT _openio(void);
void        CALLAGREEMENT _closeio(void);

unsigned long   CALLAGREEMENT _analyst_for_mode(const char* mode);
int         CALLAGREEMENT _add_memory_block(MEMORY_STREAM* memstream, int add_size);
int         CALLAGREEMENT _dec_memory_block(MEMORY_STREAM* memstream);
ISFILE*       CALLAGREEMENT _alloc_ISFILE(void);
ISFILE*       CALLAGREEMENT _free_ISFILE(ISFILE* pfile);
MEMORY_STREAM*    CALLAGREEMENT _alloc_MEMORY_STREAM(void);
MEMORY_STREAM*    CALLAGREEMENT _free_MEMORY_STREAM(MEMORY_STREAM* pmem);

/* 本地文件介质读写接口函数 */
ISFILE*       CALLAGREEMENT LOC_FILE_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_FILE_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_FILE_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_FILE_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_error(ISFILE* stream);
void        CALLAGREEMENT LOC_FILE_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_FILE_length(ISFILE* stream);

/* 本地内存介质读写接口函数 */
ISFILE*       CALLAGREEMENT LOC_MEMORY_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_MEMORY_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_MEMORY_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_MEMORY_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_MEMORY_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_MEMORY_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_MEMORY_error(ISFILE* stream);
void        CALLAGREEMENT LOC_MEMORY_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_MEMORY_length(ISFILE* stream);

/* 远程文件介质读写接口函数 */
ISFILE*       CALLAGREEMENT ROT_INTERNET_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT ROT_INTERNET_close(ISFILE* stream);
size_t        CALLAGREEMENT ROT_INTERNET_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT ROT_INTERNET_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_flush(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT ROT_INTERNET_rewind(ISFILE* stream);
long        CALLAGREEMENT ROT_INTERNET_tell(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_eof(ISFILE* stream);
int         CALLAGREEMENT ROT_INTERNET_error(ISFILE* stream);
void        CALLAGREEMENT ROT_INTERNET_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT ROT_INTERNET_length(ISFILE* stream);

/* 本地文件介质快速读写接口函数 */
ISFILE*       CALLAGREEMENT LOC_FILE_SEQUENTIAL_open(const char* pstream_name, const char* mode);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_close(ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_SEQUENTIAL_read(void* buffer, size_t size, size_t count, ISFILE* stream);
size_t        CALLAGREEMENT LOC_FILE_SEQUENTIAL_write(const void* buffer, size_t size, size_t count, ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_flush(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_seek(ISFILE* stream, long offset, int origin);
void        CALLAGREEMENT LOC_FILE_SEQUENTIAL_rewind(ISFILE* stream);
long        CALLAGREEMENT LOC_FILE_SEQUENTIAL_tell(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_eof(ISFILE* stream);
int         CALLAGREEMENT LOC_FILE_SEQUENTIAL_error(ISFILE* stream);
void        CALLAGREEMENT LOC_FILE_SEQUENTIAL_clearerr(ISFILE* stream);
unsigned long   CALLAGREEMENT LOC_FILE_SEQUENTIAL_length(ISFILE* stream);



/* ################################################################

  注：* 当前版本只实现了文件流和内存流的代码，网络流未实现。
    * ISIO流函数的设定原则是兼容ANSI C流函数，对于编译器厂商自己
      扩展的流函数不在考虑范围之内。

   ################################################################*/





/*
  功能：准备用于isio_open的流地址参数

  入口：
  PISADDR psct        - 待设的流结构

  ISEEIO_CONFER_TYPE type   - 流的类型。参见枚举类型ISEEIO_CONFER_TYPE。

  const char *name      - 流名称。  文件类型：指向路径串的指针
                        内存类型：指向内存文件名串（与磁盘文件名书写规
                            则相同）
                        网络类型：－
                      顺序类型：指向路径串的指针

  unsigned long param1    - 附加参数。文件类型：－
                      内存类型：指向内存块首地址的指针（该内存块必须
                            是独立的，而不能是某一内存块中间的
                            地址，并且该内存必须是由读写引擎提
                            供的内存接口分配的）
                      网络类型：－
                      顺序类型：－

  unsigned long param2    - 附加参数。文件类型：－
                      内存类型：内存块的尺寸
                      网络类型：－
                      顺序类型：－


  返回：经强制转换的psct地址。
*/
ISEEIO_API const char* CALLAGREEMENT isio_open_param(
    PISADDR psct,
    ISEEIO_CONFER_TYPE type,
    const char* name,
    unsigned long param1,
    unsigned long param2
)
{
  assert(psct);

  strcpy((char*)psct->name, (const char*)name);
  psct->type   = type;
  psct->param1 = param1;
  psct->param2 = param2;

  return (const char*)psct;
}


/*
  功能：插入第三方介质读写接口

  入口：
    新介质读写函数的地址

  出口：
    ISEEIO_CONFER_INVALID - 表示没有足够的内存放置新的介质读写接口，插入失败
    其它值          - 插入成功，返回值为新介质的ID

*/
ISEEIO_API ISEEIO_CONFER_TYPE CALLAGREEMENT isio_insert_oem_io(ISIOFNT_OPEN ioiopen,
    ISIOFNT_CLOSE ioiclose,
    ISIOFNT_READ ioiread,
    ISIOFNT_WRITE ioiwrite,
    ISIOFNT_FLUSH ioiflush,
    ISIOFNT_SEEK ioiseek,
    ISIOFNT_REWIND ioirewind,
    ISIOFNT_TELL ioitell,
    ISIOFNT_EOF ioieof,
    ISIOFNT_ERROR ioierror,
    ISIOFNT_CLEARERR ioiclearerr,
    ISIOFNT_LENGTH ioilength)
{
  int   i;

  /* 搜索未被占用的介质接口集 */
  for (i = ISEEIO_CONFER_MAX; i < (ISEEIO_CONFER_MAX + ISIO_OEM_INUM); i++) {
    if (_isio_ifc.ifc[i].id != i) {
      break;
    }
  }

  /* 如果OEM区已满，则返回无效类型 */
  if (i == (ISEEIO_CONFER_MAX + ISIO_OEM_INUM)) {
    return ISEEIO_CONFER_INVALID;
  }

  /* 所有接口函数必须全部提供 */
  assert(ioiopen && ioiclose && ioiread && ioiwrite && ioiflush && ioiseek && ioirewind && ioitell && ioilength && ioieof && ioierror && ioiclearerr);
  assert(_isio_ifc.ifc[i].id == 0);

  /* 设置接口集数据 */
  _isio_ifc.ifc[i].id = i;

  strcpy((char*)_isio_ifc.ifc[i].name, (const char*)"OEM类型");

  _isio_ifc.ifc[i].pfn[ISIOFN_MIN]       = (void*)NULL;
  _isio_ifc.ifc[i].pfn[ISIOFN_OPEN]    = (void*)ioiopen;
  _isio_ifc.ifc[i].pfn[ISIOFN_CLOSE]     = (void*)ioiclose;
  _isio_ifc.ifc[i].pfn[ISIOFN_READ]    = (void*)ioiread;
  _isio_ifc.ifc[i].pfn[ISIOFN_WRITE]     = (void*)ioiwrite;
  _isio_ifc.ifc[i].pfn[ISIOFN_FLUSH]     = (void*)ioiflush;
  _isio_ifc.ifc[i].pfn[ISIOFN_SEEK]    = (void*)ioiseek;
  _isio_ifc.ifc[i].pfn[ISIOFN_REWIND]    = (void*)ioirewind;
  _isio_ifc.ifc[i].pfn[ISIOFN_TELL]    = (void*)ioitell;
  _isio_ifc.ifc[i].pfn[ISIOFN_LENGTH]    = (void*)ioilength;
  _isio_ifc.ifc[i].pfn[ISIOFN_EOF]     = (void*)ioieof;
  _isio_ifc.ifc[i].pfn[ISIOFN_ERROR]     = (void*)ioierror;
  _isio_ifc.ifc[i].pfn[ISIOFN_CLEARERR]  = (void*)ioiclearerr;
  _isio_ifc.ifc[i].pfn[ISIOFN_MAX]     = (void*)NULL;

  _isio_ifc.used++;

  return (ISEEIO_CONFER_TYPE)i;
}




/*
  功能：打开指定的ISIO流

  入口：
  pstream_name  － 流名称结构地址：

  该参数可以是两种类型，一种是ISADDR结构地址（强制转换为char*），其内容
  含义如下：

        文件类型  内存类型  网络类型  顺序访问文件
  ---------------------------------------------------------------------
  *name   文件路径  内存文件  －      同文件类型
        串      名（与磁
              盘文件名
              同规则）

  param1    －      内存块首  －      －
              地址。
              写属性时
              可不设置
  param2    －      内存块总  －      －
              长度值。
              写属性时
              可不设置
  ---------------------------------------------------------------------
                          注："－"表示未被使用

  mode      － 打开流的方式：

  有效的组合如下：r,r+,rb,r+b,w,w+,wb,w+b,a,a+,ab,a+b，它们的含义可参见
  ANSI C中fopen的注释。
  注：对于顺序访问类型来说，只支持r，a，w三种模式。

  注：当用isio_open打开一个内存流后，入口参数pstream_name所指向的ISADDR
    结构不允许删除，因为当流结束时，系统将向其中填写结果数据。详细说明
    参见isio_close函数。

  返回：  0 － 打开失败
    非0 － 打开成功，返回值可用于其它的ISIO函数
*/
ISEEIO_API ISFILE* CALLAGREEMENT isio_open(const char* pstream_name, const char* mode)
{
  assert((pstream_name) && (mode));

  return (*(ISIOFNT_OPEN)_isio_ifc.ifc[((PISADDR)pstream_name)->type].pfn[ISIOFN_OPEN])(pstream_name, mode);
}




/*
  功能：关闭指定的ISIO流

  入口：
    stream  － 由isio_open()函数打开的ISIO流句柄

  备注：

  isee I/O 流被关闭后，isio_close函数将向由isio_open函数传入的PISADDR结构
  填写必要的数据，具体数据如下：

        文件类型  内存类型  网络类型  顺序类型
  ---------------------------------------------------------------------
  *name   －      内存文件  －      －
              名
  param1    －      最后的内  －      －
              存块首地址
  param2    －      内存块内容  －      －
              长度值
  ---------------------------------------------------------------------
                          注："－"表示未被改变

  内存类型备注：当打开一个内存流后，原来的内存块（如果指定了的话）将不能
    再使用，这个内存块很可能会被ISIO函数改变所在位置（尤其是向流中写数
    据的情况），只有关闭这个内存流后，在它的流名结构中返回的地址（name
    ）才可以被安全的使用。


  返回：  0 － 关闭成功
    EOF － 关闭失败
*/
ISEEIO_API int CALLAGREEMENT isio_close(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_CLOSE)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_CLOSE])(stream);
}




/*
  功能：从ISIO流读数据

  入口：
    buffer  － 用于存放读入数据的缓冲区首地址
    size  － 数据项的尺寸
    count － 数据项的个数
    stream  － ISIO流句柄

  返回：成功读入的数据项个数
*/
ISEEIO_API size_t CALLAGREEMENT isio_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  assert(buffer && size && count);
  assert(stream);

  return (*(ISIOFNT_READ)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_READ])(buffer, size, count, stream);
}




/*
  功能：向ISIO流写数据

  入口：
    buffer  － 待写的数据缓冲区首地址
    size  － 数据项的尺寸
    count － 数据项的个数
    stream  － ISIO流句柄

  返回：成功写入的数据项个数
*/
ISEEIO_API size_t CALLAGREEMENT isio_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  assert(buffer && size && count);
  assert(stream);

  return (*(ISIOFNT_WRITE)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_WRITE])(buffer, size, count, stream);
}



/*
  功能：刷新ISIO流（注：内存流没有刷新特性）

  入口：
    stream  － ISIO流句柄

  返回： 0  － 刷新成功
    EOF － 失败
*/
ISEEIO_API int CALLAGREEMENT isio_flush(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_FLUSH)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_FLUSH])(stream);
}




/*
  功能：ISIO流指针定位（注：任意位置）

  入口：
    stream  － ISIO流句柄
    offset  － 相对于origin的偏移量（以字节为单位）
    origin  － 定位用的参考点（SEEK_SET、SEEK_CUR、SEEK_END）

  返回： 0 － 定位成功
    -1 － 失败
*/
ISEEIO_API int CALLAGREEMENT isio_seek(ISFILE* stream, long offset, int origin)
{
  assert(stream);

  return (*(ISIOFNT_SEEK)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_SEEK])(stream, offset, origin);
}




/*
  功能：ISIO流指针定位（注：定位在流头部）

  入口：
    stream  － ISIO流句柄

  返回：－
*/

ISEEIO_API void CALLAGREEMENT isio_rewind(ISFILE* stream)
{
  assert(stream);

  (*(ISIOFNT_REWIND)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_REWIND])(stream);
}




/*
  功能：获取ISIO流当前指针位置

  入口：
    stream  － ISIO流句柄

  返回：  -1 － 函数执行失败
    非-1 － 位置值
*/
ISEEIO_API long CALLAGREEMENT isio_tell(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_TELL)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_TELL])(stream);
}




/*
  功能：判断ISIO流当前指针是否在流的尾部

  入口：
    stream  － ISIO流句柄

  返回：  0 － 不在尾部
    非0 － 在尾部
*/
ISEEIO_API int CALLAGREEMENT isio_eof(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_EOF)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_EOF])(stream);
}




/*
  功能：判断ISIO流是否发生了错误

  入口：
    stream  － ISIO流句柄

  返回：  0 － 没有错误
    非0 － 有错误
*/
ISEEIO_API int CALLAGREEMENT isio_error(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_ERROR)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_ERROR])(stream);
}




/*
  功能：清除ISIO流的错误标志及EOF标志

  入口：
    stream  － ISIO流句柄

  返回：－
*/
ISEEIO_API void CALLAGREEMENT isio_clearerr(ISFILE* stream)
{
  assert(stream);

  (*(ISIOFNT_CLEARERR)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_CLEARERR])(stream);
}




/*
  功能：取得ISIO流的长度

  入口：
    stream  － ISIO流句柄

  返回：流长度（以字节为单位）
*/
ISEEIO_API unsigned long CALLAGREEMENT isio_length(ISFILE* stream)
{
  assert(stream);

  return (*(ISIOFNT_LENGTH)_isio_ifc.ifc[stream->stream_type].pfn[ISIOFN_LENGTH])(stream);
}





/***********************************************************************************/
/* 系统内部函数 */


/* 解析ISIO流Open模式 */
unsigned long CALLAGREEMENT _analyst_for_mode(const char* mode)
{
  unsigned long result = 0UL;
  int count = 0;

  assert(mode);

  switch (*mode) {
  case 'r':
    result = ISIO_MODE_READ;
    break;

  case 'w':
    result = ISIO_MODE_CREATE | ISIO_MODE_WRITE | ISIO_MODE_READ;
    break;

  case 'a':
    result = ISIO_MODE_CREATE | ISIO_MODE_APPEND | ISIO_MODE_READ;
    break;

  default:
    assert(0);            /* 非法的模式标识符 ANSI (r/w/a)*/
    return 0UL;
  }

  result |= ISIO_MODE_BIN;      /* ANSI 流模式为二进制 */

  while (*++mode) {
    switch (*mode) {
    case '+':
      if (result & (ISIO_MODE_WRITE | ISIO_MODE_APPEND))
        ;
      else {
        result |= ISIO_MODE_WRITE;
      }

      break;

    case 'b':
      break;

    default:
      assert(0);          /* 非法的模式标识符 */
      count = 2;
      break;
    }

    /* 模式串最大长度为3个字符，如："r+b"等等 */
    if (++count >= 2) {
      break;
    }
  }

  return result;
}


/* 为内存流增加空间（增加的尺寸add_size）*/
int CALLAGREEMENT _add_memory_block(MEMORY_STREAM* memstream, int add_size)
{
  char* ptmp;
  int new_size;

  assert(memstream);
  assert(memstream->mode & ISIO_MODE_CREATE);

  /* 如果add_size被指定为0，则函数增加系统缺省的内存尺寸：MEMORY_STREAM_UP_SIZE*MEMORY_STREAM_UP_STEP */
  new_size = memstream->total_size + ((add_size == 0) ? (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP) : add_size);

  ptmp = (char*)im_realloc((void*)(memstream->p_start_pos), new_size);

  if (!ptmp) {
    return -1;  /* 内存不足 */
  }

  memstream->p_start_pos = ptmp;
  memstream->total_size  = new_size;

  return 0;       /* 成功 */
}


/* 释放内存块尾部的空闲内存 */
int CALLAGREEMENT _dec_memory_block(MEMORY_STREAM* memstream)
{
  char* ptmp;

  assert(memstream);
  assert(memstream->total_size >= memstream->len);

  if ((memstream->p_start_pos) && (memstream->total_size > memstream->len)) {
    ptmp = (char*)im_realloc((void*)(memstream->p_start_pos), memstream->len);

    if (!ptmp) {
      return -1;  /* realloc函数失败（似乎不可能:） */
    }

    memstream->p_start_pos = ptmp;
    memstream->total_size  = memstream->len;
    return 0;     /* 成功 */
  }

  return 1;       /* 没有空闲的内存区域 */
}


/* 申请一个新的内存流结构 */
MEMORY_STREAM* CALLAGREEMENT _alloc_MEMORY_STREAM()
{
  MEMORY_STREAM* pmem = (MEMORY_STREAM*)im_malloc(sizeof(MEMORY_STREAM));

  if (!pmem) {
    return (MEMORY_STREAM*)0;
  }

  pmem->p_start_pos = (char*)0;

  pmem->len = pmem->cur_pos = pmem->total_size = 0UL;

  pmem->mode    = 0UL;
  pmem->error   = 0UL;
  pmem->pisaddr = 0;

  return pmem;
}


/* 释放一个内存流结构（该函数并不释放流对应当内存块） */
MEMORY_STREAM* CALLAGREEMENT _free_MEMORY_STREAM(MEMORY_STREAM* pmem)
{
  assert(pmem);       /* 更严格的入口参数检查 */

  im_free(pmem);

  return (MEMORY_STREAM*)0;
}


/* 申请一个新的ISFILE结构 */
ISFILE* CALLAGREEMENT _alloc_ISFILE()
{
  ISFILE* pfile = (ISFILE*)im_malloc(sizeof(ISFILE));

  if (!pfile) {
    return (ISFILE*)0;
  }

#ifdef _DEBUG
  strcpy((char*)pfile->dbg_iseeio_stream_id, ISFILE_DBG_ID);
#endif

  pfile->org_name = (char*)0;
  pfile->stream   = (void*)0;
  pfile->stream_type = ISEEIO_CONFER_INVALID;

#ifdef _DEBUG
  __isfile_object_counts++;
#endif

  return pfile;
}


/* 释放一个ISFILE结构 */
ISFILE* CALLAGREEMENT _free_ISFILE(ISFILE* pfile)
{
  assert(pfile);        /* 更严格的入口参数检查 */

  if (!pfile) {
    return (ISFILE*)0;
  }

  assert(!strcmp((const char*)pfile->dbg_iseeio_stream_id, ISFILE_DBG_ID));

  im_free(pfile);

#ifdef _DEBUG
  __isfile_object_counts--;
#endif

  return (ISFILE*)0;
}


/* 调试专用：获取ISFILE实体的个数 */
#ifdef _DEBUG
ISEEIO_API int CALLAGREEMENT __get_object_counts(void)
{
  return __isfile_object_counts;
}
#endif /*_DEBUG*/


/* 打开ISeeIO系统（初始化函数）*/
void CALLAGREEMENT _openio()
{
  memset((void*)&_isio_ifc, 0, sizeof(ISIOINFAC));

  _isio_ifc.magic = _ISIOINFAC_MAGIC;
  _isio_ifc.rev   = 0;
  _isio_ifc.total = ISIO_MAX_INFACNUM;
  _isio_ifc.used  = ISEEIO_CONFER_XXX - 1;


  /* 设置无效类型的接口数据（用于捕获错误）*/
  _isio_ifc.ifc[ISEEIO_CONFER_INVALID].id = ISEEIO_CONFER_INVALID;
  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_INVALID].name, "无效类型");
  memset((void*)_isio_ifc.ifc[ISEEIO_CONFER_INVALID].pfn, 0, sizeof(void*)*ISIOFN_MAX);


  /* 设置本地文件读写接口数据 */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].id = ISEEIO_CONFER_LOC_FILE;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].name, "本地文件");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_OPEN]     = (void*)LOC_FILE_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_CLOSE]    = (void*)LOC_FILE_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_READ]     = (void*)LOC_FILE_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_WRITE]    = (void*)LOC_FILE_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_FLUSH]    = (void*)LOC_FILE_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_SEEK]     = (void*)LOC_FILE_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_REWIND]   = (void*)LOC_FILE_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_TELL]     = (void*)LOC_FILE_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_LENGTH]   = (void*)LOC_FILE_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_EOF]      = (void*)LOC_FILE_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_ERROR]    = (void*)LOC_FILE_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE].pfn[ISIOFN_CLEARERR] = (void*)LOC_FILE_clearerr;


  /* 设置本地内存读写接口数据 */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].id = ISEEIO_CONFER_LOC_MEMORY;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].name, "本地内存");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_OPEN]     = (void*)LOC_MEMORY_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_CLOSE]    = (void*)LOC_MEMORY_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_READ]     = (void*)LOC_MEMORY_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_WRITE]    = (void*)LOC_MEMORY_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_FLUSH]    = (void*)LOC_MEMORY_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_SEEK]     = (void*)LOC_MEMORY_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_REWIND]   = (void*)LOC_MEMORY_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_TELL]     = (void*)LOC_MEMORY_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_LENGTH]   = (void*)LOC_MEMORY_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_EOF]      = (void*)LOC_MEMORY_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_ERROR]    = (void*)LOC_MEMORY_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_MEMORY].pfn[ISIOFN_CLEARERR] = (void*)LOC_MEMORY_clearerr;


  /* 设置本地文件快速读写接口数据 */
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].id = ISEEIO_CONFER_LOC_FILE_SEQUENTIAL;

  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].name, "本地内存");

  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_MIN]      = NULL;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_OPEN]     = (void*)LOC_FILE_SEQUENTIAL_open;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_CLOSE]    = (void*)LOC_FILE_SEQUENTIAL_close;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_READ]     = (void*)LOC_FILE_SEQUENTIAL_read;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_WRITE]    = (void*)LOC_FILE_SEQUENTIAL_write;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_FLUSH]    = (void*)LOC_FILE_SEQUENTIAL_flush;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_SEEK]     = (void*)LOC_FILE_SEQUENTIAL_seek;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_REWIND]   = (void*)LOC_FILE_SEQUENTIAL_rewind;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_TELL]     = (void*)LOC_FILE_SEQUENTIAL_tell;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_LENGTH]   = (void*)LOC_FILE_SEQUENTIAL_length;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_EOF]      = (void*)LOC_FILE_SEQUENTIAL_eof;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_ERROR]    = (void*)LOC_FILE_SEQUENTIAL_error;
  _isio_ifc.ifc[ISEEIO_CONFER_LOC_FILE_SEQUENTIAL].pfn[ISIOFN_CLEARERR] = (void*)LOC_FILE_SEQUENTIAL_clearerr;


  /* 设置ISEEIO_CONFER_ROT_INTERNET类型的接口数据（暂未实现）*/
  _isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].id = ISEEIO_CONFER_ROT_INTERNET;
  strcpy((char*)_isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].name, "远程文件类型");
  memset((void*)_isio_ifc.ifc[ISEEIO_CONFER_ROT_INTERNET].pfn, 0, sizeof(void*)*ISIOFN_MAX);

}


/* 关闭ISeeIO系统（清理资源）*/
void CALLAGREEMENT _closeio()
{
  return;
}



/**************************************************************************************
 * ISEEIO_CONFER_LOC_FILE - 本地文件介质读写接口
 *************************************************************************************/

ISFILE* CALLAGREEMENT LOC_FILE_open(const char* pstream_name, const char* mode)
{
  PISADDR       pname = (PISADDR)pstream_name;
  ISFILE*        pfile = (ISFILE*)0;
  void*        pstream = (void*)0;

  /* 申请ISFILE结构块 */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;              /* 内存不足 */
  }

  /* 打开文件 */
  if ((pstream = (void*)fopen((const char*)pname->name, mode)) == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream      = pstream;
  pfile->stream_type = ISEEIO_CONFER_LOC_FILE;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_FILE_close(ISFILE* stream)
{
  int result;

  /* 关闭文件，并释放相应的结构 */
  result = fclose((FILE*)(stream->stream));

  _free_ISFILE(stream);

  return result;
}


size_t CALLAGREEMENT LOC_FILE_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return fread(buffer, size, count, (FILE*)(stream->stream));
}


size_t CALLAGREEMENT LOC_FILE_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return fwrite(buffer, size, count, (FILE*)stream->stream);
}


int CALLAGREEMENT LOC_FILE_flush(ISFILE* stream)
{
  return fflush((FILE*)(stream->stream));
}


int CALLAGREEMENT LOC_FILE_seek(ISFILE* stream, long offset, int origin)
{
  return fseek((FILE*)stream->stream, offset, origin);
}


void CALLAGREEMENT LOC_FILE_rewind(ISFILE* stream)
{
  rewind((FILE*)(stream->stream));
}


long CALLAGREEMENT LOC_FILE_tell(ISFILE* stream)
{
  return ftell((FILE*)(stream->stream));
}


int CALLAGREEMENT LOC_FILE_eof(ISFILE* stream)
{
  return feof((FILE*)stream->stream);
}


int CALLAGREEMENT LOC_FILE_error(ISFILE* stream)
{
  return ferror((FILE*)stream->stream);
}


void CALLAGREEMENT LOC_FILE_clearerr(ISFILE* stream)
{
  clearerr((FILE*)(stream->stream));
}


unsigned long CALLAGREEMENT LOC_FILE_length(ISFILE* stream)
{
  long      pos;
  unsigned long len;

  pos = ftell((FILE*)(stream->stream));
  fseek((FILE*)stream->stream, 0, SEEK_END);
  len = (unsigned long)ftell((FILE*)(stream->stream));
  fseek((FILE*)stream->stream, pos, SEEK_SET);

  return len;
}










/**************************************************************************************
 * ISEEIO_CONFER_LOC_MEMORY - 本地内存介质读写接口
 *************************************************************************************/
ISFILE* CALLAGREEMENT LOC_MEMORY_open(const char* pstream_name, const char* mode)
{
  PISADDR       pname = (PISADDR)pstream_name;
  ISFILE*        pfile = (ISFILE*)0;
  MEMORY_STREAM*   pmemory;

  /* 申请ISFILE结构块 */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;              /* 内存不足 */
  }

  /* 申请内存流结构 */
  if ((pmemory = _alloc_MEMORY_STREAM()) == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream = (void*)pmemory;

  pmemory->mode = _analyst_for_mode(mode);

  if (pname->param1) { /* 已经存在的内存流 */
    pmemory->p_start_pos = (char*)(pname->param1);
    /* 如果以"w"或"w+"方式打开的内存流，则长度被截为0 */
    pmemory->len         = ((pmemory->mode & (ISIO_MODE_WRITE | ISIO_MODE_CREATE)) == (ISIO_MODE_WRITE | ISIO_MODE_CREATE)) ? 0 : pname->param2;
    pmemory->total_size  = pname->param2;
    pmemory->cur_pos     = 0;
    pmemory->error       = (pmemory->len == pmemory->cur_pos) ? ISIO_ERROR_EOF : 0;
  }
  else {      /* 不存在的内存流 */
    if (!(pmemory->mode & ISIO_MODE_CREATE)) {
      _free_MEMORY_STREAM((MEMORY_STREAM*)(pfile->stream));
      return _free_ISFILE(pfile);   /* 没有创建属性 */
    }

    /* 创建内存流（一个分配单位的容量）*/
    if (_add_memory_block(pmemory, 0) == -1) {
      _free_MEMORY_STREAM((MEMORY_STREAM*)(pfile->stream));
      return _free_ISFILE(pfile);   /* 内存不足 */
    }

    pmemory->len     = 0UL;
    pmemory->cur_pos = 0UL;
    pmemory->error   = ISIO_ERROR_EOF;
  }

  pmemory->pisaddr   = pname;       /* 回写结构地址 */
  pfile->stream_type = ISEEIO_CONFER_LOC_MEMORY;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_MEMORY_close(ISFILE* stream)
{
  /* 压缩内存尾部的空余内存以节省空间 */
  _dec_memory_block((MEMORY_STREAM*)stream->stream);

  assert(((MEMORY_STREAM*)(stream->stream))->p_start_pos);

  /* 回写内存流信息

     注意：* 在流被关闭以前，用于Open流的地址结构不能被删除。
       * 内存流的内容块不会被Close（本函数）释放，它被传回调用者
  */
  ((MEMORY_STREAM*)(stream->stream))->pisaddr->param1 = (unsigned long)(((MEMORY_STREAM*)(stream->stream))->p_start_pos);   /* 内存块首地址 */
  ((MEMORY_STREAM*)(stream->stream))->pisaddr->param2 = (unsigned long)(((MEMORY_STREAM*)(stream->stream))->len);       /* 内存块内容的尺寸 */

  /* 释放内存流结构 */
  _free_MEMORY_STREAM((MEMORY_STREAM*)(stream->stream));
  /* 释放流结构 */
  _free_ISFILE(stream);

  return 0;
}


size_t CALLAGREEMENT LOC_MEMORY_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  MEMORY_STREAM* pmemory;
  int       i, step;

  pmemory = (MEMORY_STREAM*)stream->stream;

  if (pmemory->error & ISIO_ERROR_EOF) {
    assert(pmemory->len == pmemory->cur_pos);
    return 0;             /* 已到达文件尾部 */
  }

  assert(pmemory->cur_pos < pmemory->len);

  /* 拷贝数据到指定的缓冲区 */
  for (i = 0; i < (int)count; i++) {
    if ((pmemory->cur_pos + size) > pmemory->len) {
      step = (int)(pmemory->len - pmemory->cur_pos);
    }
    else {
      step = size;
    }

    memcpy((void*)buffer, (const void*)(pmemory->p_start_pos + pmemory->cur_pos), step);

    /* 刷新流当前指针 */
    pmemory->cur_pos += (unsigned long)step;
    ((char*)buffer) += step;

    if (pmemory->cur_pos == pmemory->len) {
      pmemory->error |= ISIO_ERROR_EOF;

      if (step == (int)size) {
        i++;
      }

      break;
    }
  }

  return i;
}


size_t CALLAGREEMENT LOC_MEMORY_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  MEMORY_STREAM* pmemory;
  int       i, addsize;

  pmemory = (MEMORY_STREAM*)stream->stream;

  /* 判断指定内存块是否有写（追加）权限 */
  if (!(pmemory->mode & (ISIO_MODE_WRITE | ISIO_MODE_APPEND))) {
    pmemory->error |= ISIO_ERROR_ERR;
    return 0;             /* 没有写权限 */
  }

  /* 追加型流的所有写操作的数据都追加到文件尾部 */
  if (pmemory->mode & ISIO_MODE_APPEND) {
    pmemory->cur_pos = pmemory->len;
    pmemory->error |= ISIO_ERROR_EOF;
  }

  /* 判断是否需要扩大内存块 */
  if ((pmemory->cur_pos + (unsigned long)(size * count)) > pmemory->total_size) {
    /* 计算实际需要的增长量 */
    addsize = (pmemory->cur_pos + size * count) - pmemory->total_size;
    /* 内存块尺寸按指数增长 */
    addsize = max(addsize, (int)pmemory->total_size);
    /* 边界对齐 */
    addsize = ((addsize + (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP - 1)) / (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP)) * (MEMORY_STREAM_UP_SIZE * MEMORY_STREAM_UP_STEP);

    /* 增加内存块尺寸 */
    if (_add_memory_block(pmemory, addsize) == -1) {
      pmemory->error |= ISIO_ERROR_ERR;
      return 0;     /* 内存不足 */
    }

    assert((pmemory->cur_pos + (unsigned long)(size * count)) <= pmemory->total_size);
  }

  /* 拷贝数据到指定的缓冲区 */
  for (i = 0; i < (int)count; i++) {
    memcpy((void*)(pmemory->p_start_pos + pmemory->cur_pos), (const void*)buffer, size);

    pmemory->cur_pos += (unsigned long)size;
    ((char*)buffer) += (int)size;

    if (pmemory->cur_pos >= pmemory->len) {
      pmemory->len = pmemory->cur_pos;
      pmemory->error |= ISIO_ERROR_EOF;
    }

    assert(pmemory->cur_pos <= pmemory->len);
    assert(pmemory->len <= pmemory->total_size);
  }

  return i;
}


int CALLAGREEMENT LOC_MEMORY_flush(ISFILE* stream)
{
  return 0; /* 无缓冲操作，不用刷新 */
}


int CALLAGREEMENT LOC_MEMORY_seek(ISFILE* stream, long offset, int origin)
{
  MEMORY_STREAM* pmemory;
  unsigned long new_off = 0UL;

  pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  switch (origin) {
  case  SEEK_SET:
    new_off = (unsigned long)offset;
    break;

  case  SEEK_CUR:
    new_off = (unsigned long)((long)(pmemory->cur_pos) + offset);
    break;

  case  SEEK_END:
    new_off = (unsigned long)((long)(pmemory->len) + offset);
    break;

  default:
    assert(0);              /* 软件设计错误 */
    break;
  }

  if (new_off > pmemory->len) {     /* 非法位置，当前位置不改变 */
    return -1;
  }

  if (new_off == pmemory->len) {
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~(ISIO_ERROR_EOF);
  }

  pmemory->cur_pos = new_off;

  return 0;
}


void CALLAGREEMENT LOC_MEMORY_rewind(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  pmemory->cur_pos = 0UL;

  if (pmemory->cur_pos == pmemory->len) {
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~(ISIO_ERROR_EOF);
  }

  return;
}


long CALLAGREEMENT LOC_MEMORY_tell(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)stream->stream;

  assert(pmemory->p_start_pos);

  return (long)pmemory->cur_pos;
}


int CALLAGREEMENT LOC_MEMORY_eof(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* 内存块必须存在 */

  return (pmemory->error & ISIO_ERROR_EOF) ? ISIO_ERROR_EOF : 0;
}


int CALLAGREEMENT LOC_MEMORY_error(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* 内存块必须存在 */

  return (pmemory->error & ISIO_ERROR_ERR) ? ISIO_ERROR_ERR : 0;
}


void CALLAGREEMENT LOC_MEMORY_clearerr(ISFILE* stream)
{
  MEMORY_STREAM* pmemory = (MEMORY_STREAM*)(stream->stream);

  assert(pmemory->p_start_pos);     /* 内存块必须存在 */

  pmemory->error &= ~(ISIO_ERROR_ERR);  /* 清除错误指示符 */

  if (pmemory->cur_pos == pmemory->len) { /* 重置错误标志 */
    pmemory->error |= ISIO_ERROR_EOF;
  }
  else {
    pmemory->error &= ~ISIO_ERROR_EOF;
  }

  return;
}


unsigned long CALLAGREEMENT LOC_MEMORY_length(ISFILE* stream)
{
  return ((MEMORY_STREAM*)(stream->stream))->len;
}









/**************************************************************************************
 * ISEEIO_CONFER_ROT_INTERNET - 介质读写接口
 *************************************************************************************/
ISFILE* CALLAGREEMENT ROT_INTERNET_open(const char* pstream_name, const char* mode)
{
  return (ISFILE*)0;                  /* 暂不支持 */
}


int CALLAGREEMENT ROT_INTERNET_close(ISFILE* stream)
{
  return EOF;
}


size_t CALLAGREEMENT ROT_INTERNET_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return 0;
}


size_t CALLAGREEMENT ROT_INTERNET_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  return 0;
}


int CALLAGREEMENT ROT_INTERNET_flush(ISFILE* stream)
{
  return EOF;
}


int CALLAGREEMENT ROT_INTERNET_seek(ISFILE* stream, long offset, int origin)
{
  return -1;
}


void CALLAGREEMENT ROT_INTERNET_rewind(ISFILE* stream)
{
  return;
}


long CALLAGREEMENT ROT_INTERNET_tell(ISFILE* stream)
{
  return -1L;
}


int CALLAGREEMENT ROT_INTERNET_eof(ISFILE* stream)
{
  return 0;
}


int CALLAGREEMENT ROT_INTERNET_error(ISFILE* stream)
{
  return 0;
}


void CALLAGREEMENT ROT_INTERNET_clearerr(ISFILE* stream)
{
  return;
}


unsigned long CALLAGREEMENT ROT_INTERNET_length(ISFILE* stream)
{
  return 0UL;
}










/**************************************************************************************
 * ISEEIO_CONFER_LOC_FILE_SEQUENTIAL - 本地文件介质快速读写接口
 *************************************************************************************/
ISFILE* CALLAGREEMENT LOC_FILE_SEQUENTIAL_open(const char* pstream_name, const char* mode)
{
  PISADDR         pname = (PISADDR)pstream_name;
  ISFILE*          pfile = (ISFILE*)0;
  void*          pstream = (void*)0;
  LPCTSTR         lpFileName;
  DWORD         dwDesiredAccess;
  DWORD         dwShareMode;
  LPSECURITY_ATTRIBUTES lpSecurityAttributes;
  DWORD         dwCreationDisposition;
  DWORD         dwFlagsAndAttributes;
  HANDLE          hTemplateFile;

  /* 申请ISFILE结构块 */
  if ((pfile = _alloc_ISFILE()) == (ISFILE*)0) {
    return (ISFILE*)0;                /* 内存不足 */
  }

  lpFileName           = (const char*)pname->name;
  lpSecurityAttributes = NULL;
  dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN; /*高速缓冲优化*/
  hTemplateFile        = NULL;

  switch (mode[0]) {
  case 'a':
    dwDesiredAccess       = GENERIC_WRITE;
    dwShareMode           = FILE_SHARE_READ;
    dwCreationDisposition = OPEN_ALWAYS;
    break;

  case 'w':
    dwDesiredAccess       = GENERIC_WRITE;
    dwShareMode           = 0;
    dwCreationDisposition = CREATE_ALWAYS;
    break;

  case 'r':
    dwDesiredAccess       = GENERIC_READ;
    dwShareMode           = FILE_SHARE_READ;
    dwCreationDisposition = OPEN_EXISTING;
    break;

  default:
    assert(!"不支持的模式");
    return _free_ISFILE(pfile);
    break;
  }

  /* 以顺序访问方式打开文件 */
  pstream = (void*)CreateFile(
      lpFileName,
      dwDesiredAccess,
      dwShareMode,
      lpSecurityAttributes,
      dwCreationDisposition,
      dwFlagsAndAttributes,
      hTemplateFile);

  if (pstream == (void*)0) {
    return _free_ISFILE(pfile);
  }

  pfile->stream      = pstream;
  pfile->stream_type = ISEEIO_CONFER_LOC_FILE_SEQUENTIAL;
  pfile->org_name    = (char*)pname->name;

  return pfile;
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_close(ISFILE* stream)
{
  int result;

  assert(stream->stream);
  result = !CloseHandle((HANDLE)stream->stream);
  _free_ISFILE(stream);

  return result;
}


size_t CALLAGREEMENT LOC_FILE_SEQUENTIAL_read(void* buffer, size_t size, size_t count, ISFILE* stream)
{
  unsigned long nNumberOfBytesRead;

  if (!ReadFile((HANDLE)stream->stream, buffer, size * count, &nNumberOfBytesRead, NULL)) {
    nNumberOfBytesRead = 0;
  }
  else {
    nNumberOfBytesRead /= size;
  }

  return nNumberOfBytesRead;
}


size_t CALLAGREEMENT LOC_FILE_SEQUENTIAL_write(const void* buffer, size_t size, size_t count, ISFILE* stream)
{
  unsigned long nNumberOfBytesWritten;

  if (!WriteFile((HANDLE)stream->stream, buffer, size * count, &nNumberOfBytesWritten, NULL)) {
    nNumberOfBytesWritten = 0;
  }
  else {
    nNumberOfBytesWritten /= size;
  }

  return nNumberOfBytesWritten;
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_flush(ISFILE* stream)
{
  if (FlushFileBuffers((HANDLE)stream->stream)) {
    return 0;
  }
  else {
    return EOF;
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_seek(ISFILE* stream, long offset, int origin)
{
  long      lDistanceToMove;
  unsigned long dwMoveMethod;

  switch (origin) {
  case SEEK_SET:
    dwMoveMethod = FILE_BEGIN;
    break;

  case SEEK_CUR:
    dwMoveMethod = FILE_CURRENT;
    break;

  case SEEK_END:
    dwMoveMethod = FILE_END;
    break;

  default:
    assert(!"不支持的定位原点");
    return -1;
  }

  lDistanceToMove = offset;

  if (INVALID_SET_FILE_POINTER == SetFilePointer((HANDLE)stream->stream, lDistanceToMove, NULL, dwMoveMethod)) {
    return -1L;
  }
  else {
    return 0;
  }
}


void CALLAGREEMENT LOC_FILE_SEQUENTIAL_rewind(ISFILE* stream)
{
  SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_BEGIN);
}


long CALLAGREEMENT LOC_FILE_SEQUENTIAL_tell(ISFILE* stream)
{
  long  lDistanceToMove;

  lDistanceToMove = SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_CURRENT);

  if (INVALID_SET_FILE_POINTER ==  lDistanceToMove) {
    return -1L;
  }
  else {
    return lDistanceToMove;
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_eof(ISFILE* stream)
{
  long  lDistanceToMove;

  lDistanceToMove = SetFilePointer((HANDLE)stream->stream, 0, NULL, FILE_CURRENT);

  if (INVALID_SET_FILE_POINTER ==  lDistanceToMove) {
    return -1;
  }
  else {
    return (GetFileSize((HANDLE)stream->stream, NULL) == (unsigned long)lDistanceToMove);
  }
}


int CALLAGREEMENT LOC_FILE_SEQUENTIAL_error(ISFILE* stream)
{
  long  err = GetLastError();

  switch (err) {
  case  ERROR_FILE_NOT_FOUND:           /* 只识别与文件操作有关的错误 */
  case  ERROR_PATH_NOT_FOUND:
  case  ERROR_TOO_MANY_OPEN_FILES:
  case  ERROR_WRITE_PROTECT:
  case  ERROR_NOT_READY:
  case  ERROR_SEEK:
  case  ERROR_NOT_DOS_DISK:
  case  ERROR_WRITE_FAULT:
  case  ERROR_READ_FAULT:
  case  ERROR_SHARING_VIOLATION:
  case  ERROR_HANDLE_EOF:
  case  ERROR_HANDLE_DISK_FULL:
  case  ERROR_OPEN_FAILED:
  case  ERROR_BUFFER_OVERFLOW:
  case  ERROR_DISK_FULL:
  case  ERROR_INVALID_NAME:
  case  ERROR_SEEK_ON_DEVICE:
  case  ERROR_FILENAME_EXCED_RANGE:
    break;

  default:
    err = 0;                    /* 忽略非文件读写方面的错误 */
    break;
  }

  return err;
}


void CALLAGREEMENT LOC_FILE_SEQUENTIAL_clearerr(ISFILE* stream)
{
  SetLastError(0);
}


unsigned long CALLAGREEMENT LOC_FILE_SEQUENTIAL_length(ISFILE* stream)
{
  return GetFileSize((HANDLE)stream->stream, NULL);
}



/**************************************************************************************
 * ISEEIO_CONFER_XXXXXX - 介质读写接口
 *************************************************************************************/
/*
ISFILE * CALLAGREEMENT XXX_open(const char *pstream_name, const char *mode)
{
}


int CALLAGREEMENT XXX_close(ISFILE *stream)
{
}


size_t CALLAGREEMENT XXX_read(void *buffer, size_t size, size_t count, ISFILE *stream)
{
}


size_t CALLAGREEMENT XXX_write(const void *buffer, size_t size, size_t count, ISFILE *stream)
{
}


int CALLAGREEMENT XXX_flush(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_seek(ISFILE *stream, long offset, int origin)
{
}


void CALLAGREEMENT XXX_rewind(ISFILE *stream)
{
}


long CALLAGREEMENT XXX_tell(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_eof(ISFILE *stream)
{
}


int CALLAGREEMENT XXX_error(ISFILE *stream)
{
}


void CALLAGREEMENT XXX_clearerr(ISFILE *stream)
{
}


unsigned long CALLAGREEMENT XXX_length(ISFILE *stream)
{
}

*/



