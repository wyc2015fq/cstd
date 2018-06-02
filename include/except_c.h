#ifndef __EXCEPT_C_H
#define __EXCEPT_C_H
#include <setjmp.h>
/* 定义全部异常类型，可按需要定义任何异常类型，供ON_EXCEPT宏使用 */
#define EXCEPT_ALL        0
//
// 定义C语言使用的异常类型、函数和宏：
//
// 1、Raise(type, msg)：抛出type异常，msg为异常信息
// 2、RaiseMessage(msg)：抛出异常，相当于Raise(EXCEPT_ALL, msg)
// 3、ReRaise()：重新抛出以前的异常
//
// 4、异常响应。对可能出现的异常进行处理(无异常时，处理代码不执行)：
//
//    TRY
//        正常代码
//    ON_EXCEPT(type)
//        可选项。处理type异常的代码，可在EXCEPT前连续使用
//    EXCEPT
//       可选项。所有异常处理代码，相当于ON_EXCEPT(EXCEPT_ALL)
//    END_TRY
//
// 5、异常保护。无论是否出现异常，均执行的保护性质代码，如资源释放：
//
//    TRY
//        正常代码
//    FINALLY
//        保护性质代码
//    END_TRY
//
// 6、套异常可嵌套使用，但不能混用，如：
//
//    TRY
//        代码块1
//        TRY
//           代码块2
//        FINALLY
//            保护性质代码
//        END_TRY
//    EXCEPT
//        异常处理代码
//    END_TRY
//
#define TRY                except_Set(); if (!except_SetNum(setjmp(*except_Buf()))) {
#define Raise(type, msg)   except_Raise(type, msg, __FILE__, __LINE__)
#define RaiseMessage(msg)  Raise(EXCEPT_ALL, msg)
#define ReRaise()          except_ReRaise()
#define ON_EXCEPT(type)    } else if (except_On(type)) {
#define EXCEPT             ON_EXCEPT(EXCEPT_ALL)
#define FINALLY            } {
#define END_TRY            } except_end();
/* 异常结构 */
typedef struct __Exception {
  int type;     /* 异常类型 */
  char* message; /* 消息 */
  char* soufile; /* 源文件 */
  int lineNum;  /* 产生异常的行号 */
}
Exception;
// 获取当前异常消息
char* except_Message(void);
// 获取当前异常结构
Exception* except_Exception(void);
// 以下函数为内部使用
void except_Set(void);
void except_Raise(int type, const char* message, char* file, int line);
void except_ReRaise(void);
int except_On(int type);
void except_end(void);
jmp_buf* except_Buf(void);
int except_SetNum(int Num);
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "except_c.h"
#define    MESSAGE_FORMAT    "%s(%d): "
enum {evEnter, evRaise, evExcept = 2};
typedef struct __Exception_Event {
  struct __Exception_Event* prev;
  jmp_buf eBuf;
  int evNum;
  Exception exception;
}
Exception_Event;
static Exception_Event* except_ptr = NULL;
static char* except_msg;
static char except_msg_size = 0;
static void except_Set(void)
{
  Exception_Event* ev;
  ev = (Exception_Event*) malloc(sizeof(Exception_Event));
  ev->prev = except_ptr;
  except_ptr = ev;
}
static void except_Clear(void)
{
  Exception_Event* ev;
  if (except_ptr) {
    ev = except_ptr;
    except_ptr = except_ptr->prev;
    free(ev);
  }
}
static void except_Raise(int type, const char* message, char* file, int line)
{
  int len = 0, size;
#ifndef    NDEBUG
  char buf[ 100 ];
  sprintf(buf, MESSAGE_FORMAT, file, line);
  len = strlen(buf);
#endif
  size = strlen(message) + len + 1;
  if (except_msg_size < size) {
    if (except_msg_size > 0) {
      free(except_msg);
    }
    except_msg_size = size;
    except_msg = (char*) malloc(except_msg_size);
  }
#ifndef    NDEBUG
  strcpy(except_msg, buf);
  strcat(except_msg, message);
#else
  strcpy(except_msg, message);
#endif
  if (except_ptr) {
    except_ptr->exception.type = type;
    except_ptr->exception.message = &except_msg[ len ];
    except_ptr->exception.soufile = file;
    except_ptr->exception.lineNum = line;
    longjmp(except_ptr->eBuf, evRaise);
  }
  else {
    fprintf(stderr, except_msg);
    abort();
  }
}
static void except_ReRaise(void)
{
  Exception e;
  if (except_ptr) {
    e = except_ptr->exception;
    if (except_ptr->prev) {
      except_Clear();
      except_ptr->exception = e;
      longjmp(except_ptr->eBuf, evRaise);
    }
    else {
      fprintf(stderr, except_msg);
      abort();
    }
  }
}
static int except_On(int type)
{
  if (except_ptr->evNum == evRaise &&
      (type == EXCEPT_ALL || type == except_ptr->exception.type)) {
    except_ptr->evNum = evExcept;
    return 1;
  }
  return 0;
}
static void except_end(void)
{
  if (except_ptr->evNum == evRaise) {
    except_ReRaise();
  }
  except_Clear();
}
static jmp_buf* except_Buf(void)
{
  return & except_ptr->eBuf;
}
static char* except_Message(void)
{
  return except_msg;
}
static Exception* except_Exception(void)
{
  return & except_ptr->exception;
}
static int except_SetNum(int Num)
{
  except_ptr->evNum = Num;
  return except_ptr->evNum;
}
#endif  /* __EXCEPT_C_H */

