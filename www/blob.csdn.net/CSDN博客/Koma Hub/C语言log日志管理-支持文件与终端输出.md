# C语言log日志管理-支持文件与终端输出 - Koma Hub - CSDN博客
2018年11月17日 16:22:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：48
个人分类：[C/C++																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**目录**
[文件结构](#%E6%96%87%E4%BB%B6%E7%BB%93%E6%9E%84)
[t_log.c](#t_log.c)
[t_log.h](#t_log.h%E5%86%85%E5%AE%B9%EF%BC%9A)
[t_color.c](#t_color.c)
[t_color.h](#t_color.h)
[编译运行](#%E7%BC%96%E8%AF%91%E8%BF%90%E8%A1%8C)
### 文件结构
```
├── t_color.c
├── t_color.h
├── t_log.c
└── t_log.h
```
### t_log.c
```cpp
/*	file name: 		t_log.c
 *	author:			Rong Tao
 *	create time:	2018.11.10
 *	modify time:	2018.11.11
 *	modify time:	2018.11.12 pthread security
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>  
#include <sys/stat.h> 
#include <string.h>
#include <libintl.h>
#include <sys/file.h>
#include <pthread.h>
#include "t_log.h"
#include "t_color.h"
FILE  *log_fp = NULL;
pthread_mutex_t t_log_g_mutexlock = PTHREAD_MUTEX_INITIALIZER;
/* T_LOG_FMT(32bit) = 0x 00     00          00           00 
 						 00 T_LOG_FONT T_LOG_FCOLOR T_LOG_BCOLOR */
uint32_t T_LOG_FMT = 0x0;
#define _2string(val)	#val
/**
 *	Log function: make each %s, %d, %c, %x...
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_1_test__(FILE *fp, char *fmt, ...)
{
	_T_LOG_G_PTHREAD_MUTEX_LOCK_
	if(fmt == NULL || fp == NULL)
	{
		return -1;
	}
	
	if(flock(fileno(log_fp),LOCK_EX) != 0)
	{
		perror("log_fp lock error.");
	}
	if(log_fp == NULL)
	{
		log_fp = stdout;
	}
	
	va_list arg;
	va_start(arg, fmt);
	int _i;
	double _f;
	char *_s;
	char *p = fmt;
	while(*p != '\0')
	{
		switch(*p)
		{
			case '%':
				p++;
				switch(*p)
				{
					case '%':
						fprintf(log_fp, "%");
						break;
					case 'd':
						_i = va_arg(arg, int);
						fprintf(log_fp, "%d", _i);
						break;
					case 'c':
						_i = va_arg(arg, int);
						fprintf(log_fp, "%c", _i);
						break;
					case 's':
						_s = va_arg(arg, char*);
						fprintf(log_fp, "%s", _s);
						break;
					case 'x':
						_i = va_arg(arg, int);
						fprintf(log_fp, "0x%x", _i);
						break;
					case 'f':
						_f = va_arg(arg, double);
						fprintf(log_fp, "%f", _f);
						break;
				}
				p++;
				break;
			default:
				fprintf(log_fp, "%c", *p);
				p++;
				break;
		}
	}
	
	if(flock(fileno(log_fp),LOCK_UN) != 0)
	{
		perror("log_fp unlock error.");
	}
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_
	va_end(arg);
}
/**
 *	Log function: t_log initialization
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_init(LOG_TYPE log_type, const char *filename)
{
	_T_LOG_G_PTHREAD_MUTEX_LOCK_
	switch(log_type)
	{
		case LOG_TEXT:
			if(!filename)
			{
				debug();
				fprintf(stdout, "null pointer error\n");
				return -1;
			}
			/* file exist */
			if(0 == access(filename, F_OK))
			{
				chmod(filename, S_IRUSR|S_IWUSR);
				log_fp = fopen(filename, "a+");
			}
			else /* file not exist */
			{
				log_fp = fopen(filename, "w");
			}
			break;
			
		case LOG_SCREEN:
			log_fp = stdout;
			break;
		
		default:
			debug();
			fprintf(stdout, "no such log type error: %s\n", _2string(log_type));
			log_fp = stderr;
			return -1;
			break;
	}
	
	fprintf(log_fp, "\n\n------------------------------------------------------------------\n"\
					" %s %s running...\n", __DATE__, __TIME__);
	
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_
	return 0;
}
/**
 *	Log function: t_log clean
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_clean()
{
	_T_LOG_G_PTHREAD_MUTEX_LOCK_
		
	if(log_fp == NULL)
	{
		fprintf(stdout, "\n------------------------------------------------------------------\n\n");
	}
	else if(log_fp == stdout || log_fp == stderr)
	{
		fprintf(log_fp, "\n------------------------------------------------------------------\n\n");
	}
	else if(log_fp != stdout && log_fp != stderr && log_fp != NULL)
	{
		fprintf(log_fp, "\n------------------------------------------------------------------\n\n");
		fclose(log_fp);
		log_fp = NULL;
		return 0;
	}
	log_fp = NULL;
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_
	return -1;
}
/**
 *	Q&A
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
t_boolean t_log_QuestionAnswer(char *question)
{
	char *Q = NULL;
	if(question == NULL)
	{
		Q = (char *)malloc(sizeof(256));
		sprintf(Q, "Are you sure? ");
	}
	else
	{
		Q = question;
	}
	void *sret = NULL;
	while(sret == strcat(Q, "[yes/no]"))
	{
		sret = realloc(Q, strlen(Q) + 256);
	}
	/* Use a safe default. */
	t_boolean answer = false;
	
	fputs (gettext (Q), stdout);
	fflush (stdout);
	
	/* Prepare the getline call. */
	char *line = NULL;
	size_t len = 0;
	while (getline (&line, &len, stdin) >= 0)
	{
		/* Check the response. */
		int res = rpmatch (line);
		if (res >= 0)
		{
			/* We got a definitive answer. */
			if (res > 0)
			{
				answer = true;
			}
			break;
		}
	}
	/* Free what getline allocated. */
	free (line);
	
	return answer;
}
/**
 *	Get t_log_fmt start string, for example: "\033[1;31;40m"
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
void* getTLogFmtString(const uint32_t t_log_fmt, char *t_log_fmt_start)
{
	T_LOG_FMT = t_log_fmt;
	unsigned int i;
	uint8_t font8, fcolor8, bcolor8;
	uint32_t font32, fcolor32, bcolor32;
	f123_color_t font_t=0, fcolor_t=0, bcolor_t=0;
	t_boolean flag = false;
	
	font8 = (t_log_fmt&0x000000ff)>>0;
	fcolor8 = (t_log_fmt&0x0000ff00)>>8;
	bcolor8 = (t_log_fmt&0x00ff0000)>>16;
	font32 = t_log_fmt&0x000000ff;
	fcolor32 = t_log_fmt&0x0000ff00;
	bcolor32 = t_log_fmt&0x00ff0000;
	COLOR_Fmt color_fmt;
	flag = false;
	for(i=0; i<F1_COLOR_NUM; i++)
	{
		switch(font32&(1<<(i+0)))
		{
			case T_LOG_DEFAULT: 	font_t = F1_COLOR_DEFAULT; flag = true; break;
			case T_LOG_BOLD: 		font_t = F1_COLOR_BOLD; flag = true; break;
			case T_LOG_GRAY: 		font_t = F1_COLOR_GRAY; flag = true; break;
			case T_LOG_ITALIC: 		font_t = F1_COLOR_ITALIC; flag = true; break;
			case T_LOG_UNDERLINE: 	font_t = F1_COLOR_UNDERLINE; flag = true; break;
			case T_LOG_INVERT: 		font_t = F1_COLOR_INVERT; flag = true; break;
			case T_LOG_DELETELINE: 	font_t = F1_COLOR_DELETELINE; flag = true; break;
			default: break;
		}
		if(flag == true)
		{
			break;
		}	
	}
	flag = false;
	for(i=0; i<F2_COLOR_NUM; i++)
	{
		switch(fcolor32&(1<<(i+8)))
		{
			case T_LOG_FBLACK: 		fcolor_t = F2_COLOR_BLACK; flag = true; break;
			case T_LOG_FRED:		fcolor_t = F2_COLOR_RED; flag = true; break;
			case T_LOG_FGREEN:		fcolor_t = F2_COLOR_GREEN; flag = true; break;
			case T_LOG_FYELLOW:		fcolor_t = F2_COLOR_YELLOW; flag = true; break;
			case T_LOG_FDARKPURPLE:	fcolor_t = F2_COLOR_DARKPURPLE; flag = true; break;
			case T_LOG_FPURPLE:		fcolor_t = F2_COLOR_PURPLE; flag = true; break;
			case T_LOG_DARKBLUE:	fcolor_t = F2_COLOR_DARKBLUE; flag = true; break;
			case T_LOG_WHITE:		fcolor_t = F2_COLOR_WHITE; flag = true; break;
			default: break;
		}
		if(flag == true)
		{
			break;
		}	
	}
	flag = false;
	for(i=0; i<F3_COLOR_NUM; i++)
	{
		switch(bcolor32&(1<<(i+16)))
		{
			case T_LOG_BBLACK: 		bcolor_t = F3_COLOR_BLACK; flag = true; break;
			case T_LOG_BRED:		bcolor_t = F3_COLOR_RED; flag = true; break;
			case T_LOG_BGREEN:		bcolor_t = F3_COLOR_GREEN; flag = true; break;
			case T_LOG_BYELLOW:		bcolor_t = F3_COLOR_YELLOW; flag = true; break;
			case T_LOG_BBLUE:		bcolor_t = F3_COLOR_BLUE; flag = true; break;
			case T_LOG_BPURPLE:		bcolor_t = F3_COLOR_PURPLE; flag = true; break;
			case T_LOG_BBABYBLUE:	bcolor_t = F3_COLOR_BABYBLUE; flag = true; break;
			case T_LOG_BWHITE:		bcolor_t = F3_COLOR_WHITE; flag = true; break;
			default: break;
		}
		if(flag == true)
		{
			break;
		}	
	}
	if(f1_color_t_outofrange(font_t) != 0)
	{
		font_t = F1_COLOR_DEFAULT;
	}
	if(f2_color_t_outofrange(fcolor_t) != 0)
	{
		fcolor_t = F2_COLOR_BLACK;
	}
	if(f3_color_t_outofrange(bcolor_t) != 0)
	{
		bcolor_t = F3_COLOR_WHITE;
	}
	
	getColorFmt(font_t, fcolor_t, bcolor_t, &color_fmt);
	void *pret = NULL;
	while(pret == NULL)
	{
		t_log_fmt_start = malloc(sizeof(color_fmt.fmt_1));
		pret = t_log_fmt_start;
	};
	pret = NULL;
	while(pret == NULL)
	{
		pret = memcpy(t_log_fmt_start, &(color_fmt.fmt_1), sizeof(color_fmt.fmt_1));
		pret = t_log_fmt_start;
	};
	t_log_print("%x:%d, %x:%d, %x:%d\n", font8,font_t, fcolor8,fcolor_t, bcolor8,bcolor_t);
	return pret;
}
/**
 *	Macro: log function macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int _t_log_4(uint32_t t_log_fmt_start_bit_t, char *fmt, ...)
{
	if(fmt == NULL)
	{
		return -1;
	}
	va_list arg;
	va_start(arg, fmt);
	char *_t_log_fmt_start = NULL;
	_t_log_fmt_start = getTLogFmtString(t_log_fmt_start_bit_t, _t_log_fmt_start);
	char t_log_fmt[strlen(fmt)+strlen(_t_log_fmt_start)+strlen(COLOR_FMT_END)];
	sprintf(t_log_fmt, "%s%s%s\n", _t_log_fmt_start, fmt, COLOR_FMT_END);	
	_T_LOG_G_PTHREAD_MUTEX_LOCK_
	if(log_fp == NULL || log_fp == stdout)
	{
		vfprintf(stdout, t_log_fmt, arg);
	}
	else if(log_fp == stderr)
	{
		vfprintf(log_fp, t_log_fmt, arg);
	}
	else
	{
		vfprintf(log_fp, fmt, arg);
	}
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_
}
/**
 *	Log test
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
#if 1
int main()
{
	//t_log_init(LOG_TEXT, "log.txt");
	//t_log_init(LOG_SCREEN, NULL);
	
	t_log_error("rongtao123%lf\n", 1212);
	t_log_info("rongtao123%lf\n", 1212);
	t_log_success("rongtao123%lf\n", 1212);
	t_log_warning("rongtao123%lf\n", 1212);
	t_log_debug("rongtao123%lf\n", 1212);
	t_log_summary("helllow%s", "rongadfadsfadf\n");
	t_log_time();
	t_log_print("ronghellow_______ %s", "asdfadf\n");
	t_log(T1_NONE|T3_BLACK|T2_RED, "adsfadsf1\n32123123123adfadf%d\n", 123);
	t_log_clean();
	//t_log_info("%s\n", t_log_QuestionAnswer(NULL)==true?"true":"false");
	
	return 0;
}
#endif
```
### t_log.h
```
/*	file name: 		t_log.h
 *	author:			Rong Tao
 *	create time:	2018.11.10
 *	modify time:	2018.11.11
 *	modify time:	2018.11.12 pthread security
 */
#ifndef _T_LOG_H_
#define _T_LOG_H_
#include <stdint.h>
#include <pthread.h>
#include <syslog.h>
#include "t_color.h"
#ifndef false
#define false -1
#endif
#ifndef true
#define true 0
#endif
typedef int t_boolean;
typedef enum{
	LOG_TEXT,
	LOG_SCREEN,
}LOG_TYPE;
extern FILE  *log_fp;
extern uint32_t T_LOG_FMT;
/**
 *	To make pthread security
 *	Author: Rong Tao
 *	Time:	2018.11.12
 */
extern pthread_mutex_t t_log_g_mutexlock;
#define _T_LOG_G_PTHREAD_MUTEX_LOCK_ 	do{pthread_mutex_lock(&t_log_g_mutexlock);}while(0);
#define _T_LOG_G_PTHREAD_MUTEX_UNLOCK_ 	do{pthread_mutex_unlock(&t_log_g_mutexlock);}while(0);
	
/**
 *	T_LOG_FMT(32bit) = 0x 00 	00			00			 00 
 *						  00 T_LOG_FONT T_LOG_FCOLOR T_LOG_BCOLOR
 */
#ifdef COLORFUL_FMT
#define	T_LOG_DEFAULT		(1<<0)
#define	T_LOG_BOLD			(1<<1)
#define	T_LOG_GRAY			(1<<2)
#define	T_LOG_ITALIC		(1<<3) /*xieti*/
#define	T_LOG_UNDERLINE		(1<<4) /*xiahauxian*/
#define	T_LOG_INVERT		(1<<5) /*fanxian*/
#define	T_LOG_DELETELINE	(1<<6)
	
#define	T_LOG_FBLACK		(1<<(0+8))
#define	T_LOG_FRED			(1<<(1+8))
#define	T_LOG_FGREEN		(1<<(2+8))
#define	T_LOG_FYELLOW		(1<<(3+8))
#define	T_LOG_FDARKPURPLE	(1<<(4+8))
#define	T_LOG_FPURPLE		(1<<(5+8))
#define	T_LOG_DARKBLUE		(1<<(6+8))
#define	T_LOG_WHITE			(1<<(7+8))
#define	T_LOG_BBLACK		(1<<(0+16))
#define	T_LOG_BRED			(1<<(1+16))
#define	T_LOG_BGREEN		(1<<(2+16))
#define	T_LOG_BYELLOW		(1<<(3+16))
#define	T_LOG_BBLUE			(1<<(4+16))
#define	T_LOG_BPURPLE		(1<<(5+16))
#define	T_LOG_BBABYBLUE		(1<<(6+16))
#define	T_LOG_BWHITE		(1<<(7+16))
#else
	
#endif
#define T1_NONE				T_LOG_DEFAULT
#define T1_BOLD				T_LOG_BOLD
#define T1_GRAY				T_LOG_GRAY
#define T1_ITAL				T_LOG_ITALIC
#define T1_ULIN				T_LOG_UNDERLINE
#define T1_INVE				T_LOG_INVERT
#define T1_DLIN				T_LOG_DELETELINE
#define T2_BLACK			T_LOG_FBLACK
#define T2_RED				T_LOG_FRED
#define T2_GREEN			T_LOG_FGREEN
#define T2_YELLOW			T_LOG_FYELLOW
#define T2_DPURPLE			T_LOG_FDARKPURPLE
#define T2_PURPLE			T_LOG_FPURPLE
#define T2_DBULE			T_LOG_DARKBLUE
#define T2_WHITE			T_LOG_WHITE
#define T3_BLACK			T_LOG_BBLACK
#define T3_RED				T_LOG_BRED
#define T3_GREEN			T_LOG_BGREEN
#define T3_YELLOW			T_LOG_BYELLOW
#define T3_BLUE				T_LOG_BBLUE
#define T3_PURPLE			T_LOG_BPURPLE
#define T3_BBLUE			T_LOG_BBABYBLUE
#define T3_WHITE			T_LOG_BWHITE
/**
 *	Output in terminal
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
#define debug()	\
	do{\
	printf("%s:%d %s %s %s\n", \
	__FILE__, __LINE__, __func__, __DATE__, __TIME__); \
	}while(0)
/**
 *	System time string
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
#define _T_LOG_TIME_ \
	__DATE__":"__TIME__
/**
 *	Macro: log function macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
/* _t_log_1(string1, string2, __VA_ARGS__) */
#define _t_log_1(COLOR_FMT_START, STRING, ...) \
	do{\
	_T_LOG_G_PTHREAD_MUTEX_LOCK_ \
	if(log_fp == stdout || log_fp == stderr){ 	\
		fprintf(log_fp, COLOR_FMT_START STRING COLOR_FMT_END " "__VA_ARGS__);	\
	}else if(log_fp == NULL){	\
		fprintf(stdout, COLOR_FMT_START STRING COLOR_FMT_END " "__VA_ARGS__);	\
	}else{	\
		fprintf(log_fp, STRING " "__VA_ARGS__);} \
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_ \
	}while(0)
	
/**
 *	Macro: log function macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
/* _t_log_2(string1, string2, string3) */
#define _t_log_2(COLOR_FMT_START, STRING1, STRING2) \
	do{\
	_T_LOG_G_PTHREAD_MUTEX_LOCK_ \
	if(log_fp == stdout || log_fp == stderr){ 	\
		fprintf(log_fp, COLOR_FMT_START STRING1 COLOR_FMT_END " "STRING2);	\
	}else if(log_fp == NULL){	\
		fprintf(stdout, COLOR_FMT_START STRING1 COLOR_FMT_END " "STRING2);	\
	}else{	\
		fprintf(log_fp, STRING1 " "STRING2);} \
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_ \
	}while(0)
	
/**
 *	Macro: log function macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */	
/* _t_log_3(__VA_ARGS__) */
#define _t_log_3(...) \
	do{\
	_T_LOG_G_PTHREAD_MUTEX_LOCK_ \
	if(log_fp == stdout || log_fp == stderr){ 	\
		fprintf(log_fp,  __VA_ARGS__);	\
	}else if(log_fp == NULL){	\
		fprintf(stdout, __VA_ARGS__);	\
	}else{	\
		fprintf(log_fp, __VA_ARGS__);} \
	_T_LOG_G_PTHREAD_MUTEX_UNLOCK_\
	}while(0)
/**
 *	Macro: log function macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
/* _t_log_4(COLOR_FMT_START, __VA_ARGS__) */
int _t_log_4(uint32_t, char*, ...);
/**
 *	Macro: log functions macro
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
#define t_log_error(...)	_t_log_1(COLOR_FMT_ERROR,		"[ error ]", __VA_ARGS__)
#define t_log_info(...)		_t_log_1(COLOR_FMT_INFO,		"[ info  ]", __VA_ARGS__)
#define t_log_success(...)	_t_log_1(COLOR_FMT_SUCCESS,		"[success]", __VA_ARGS__)
#define t_log_warning(...)	_t_log_1(COLOR_FMT_WARNING,		"[warning]", __VA_ARGS__)
#define t_log_debug(...)	_t_log_1(COLOR_FMT_DEBUG,		"[ debug ]", __VA_ARGS__)
#define t_log_summary(...)	_t_log_1(COLOR_FMT_SUMMARY,		"[summary]", __VA_ARGS__)
#define t_log_time()		_t_log_2(COLOR_FMT_TIME,		"[ time  ]", _T_LOG_TIME_"\n")
#define t_log_print(...)	_t_log_3(__VA_ARGS__)
#define t_log(t_log_fmt_start, ...) _t_log_4(t_log_fmt_start, __VA_ARGS__)
/**
 *	Log function: make each %s, %d, %c, %x...
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_1_test__(FILE *, char *, ...);
/**
 *	Log function: t_log initialization
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_init(LOG_TYPE, const char *);
/**
 *	Log function: t_log clean
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
int t_log_clean();
/**
 *	Q&A
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
t_boolean t_log_QuestionAnswer(char *question);
/**
 *	Get t_log_fmt start string, for example: "\033[1;31;40m"
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
void* getTLogFmtString(const uint32_t fmt, char* t_log_fmt_start);
/**
 *	Syslog to t_log Macros
 *	Author:	rongtao
 *	Time:	2018.11.12
 */
#define t_sys_log_open(ident, option, facility) openlog(ident, option, facility)
#define t_sys_log(facility_priority, ...) syslog(facility_priority, __VA_ARGS__)
#define t_sys_vlog(facility_priority, vlist) syslog(facility_priority, vlist)
#define t_sys_log_close() closelog()
#define t_sys_log_setmask(mask) setlogmask(mask)
#endif /*<_T_LOG_H_>*/
```
### t_color.c
```cpp
/*	file name: 		t_color.c
 *	author:			Rong Tao
 *	create time:	2018.11.10
 *	create time:	2018.11.13
 */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "t_color.h"
/**
 *	Get color fmt: font, front color, background color to COLOT_Fmt struct
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
void* getColorFmt(f123_color_t font, f123_color_t fcolor, f123_color_t bcolor, COLOR_Fmt *fmt)
{
	if(fmt == NULL)
	{
		COLOR_Fmt *tmp = (COLOR_Fmt *)malloc(sizeof(COLOR_Fmt));
		if(tmp == NULL)
		{
			return NULL;
		}
		tmp->font = font;
		tmp->bcolor = bcolor;
		tmp->fcolor = fcolor;
		memset(tmp->fmt_1, 0, sizeof(tmp->fmt_1));
		memset(tmp->fmt_2, 0, sizeof(tmp->fmt_2));
		sprintf(tmp->fmt_1, "\033[%d;%d;%dm", font, fcolor, bcolor);
		sprintf(tmp->fmt_2, "\033[0m");
		return (void *)tmp;
	}
	else
	{
		fmt->font = font;
		fmt->bcolor = bcolor;
		fmt->fcolor = fcolor;
		memset(fmt->fmt_1, 0, sizeof(fmt->fmt_1));
		memset(fmt->fmt_2, 0, sizeof(fmt->fmt_2));
		sprintf(fmt->fmt_1, "\033[%d;%d;%dm", font, fcolor, bcolor);
		sprintf(fmt->fmt_2, "\033[0m");
		void *p = fmt;
		return (void *)p;
	}
}
/**
 *	Check f1 format is valiable
 *	Author: Rong Tao
 *	Time:	2018.11.13
 */
int f1_color_t_outofrange(f123_color_t font_t)
{
	switch(font_t)
	{
		case F1_COLOR_DEFAULT: case F1_COLOR_BOLD: case F1_COLOR_GRAY:
		case F1_COLOR_ITALIC: case F1_COLOR_UNDERLINE: case F1_COLOR_INVERT:
		case F1_COLOR_DELETELINE:
			return 0;
		default: return -1;
	}
	return -1;
}
/**
 *	Check f2 format is valiable
 *	Author: Rong Tao
 *	Time:	2018.11.13
 */
int f2_color_t_outofrange(f123_color_t fcolor_t)
{
	switch(fcolor_t)
	{
		case F2_COLOR_BLACK: case F2_COLOR_RED: case F2_COLOR_GREEN:
		case F2_COLOR_YELLOW: case F2_COLOR_DARKPURPLE: case F2_COLOR_PURPLE:
		case F2_COLOR_DARKBLUE: case F2_COLOR_WHITE:
			return 0;
		default: return -1;
	}
	return -1;
}
/**
 *	Check f3 format is valiable
 *	Author: Rong Tao
 *	Time:	2018.11.13
 */
int f3_color_t_outofrange(f123_color_t bcolor_t)
{
	switch(bcolor_t)
	{
		case F3_COLOR_BLACK: case F3_COLOR_RED: case F3_COLOR_GREEN:
		case F3_COLOR_YELLOW: case F3_COLOR_BLUE: case F3_COLOR_PURPLE:
		case F3_COLOR_BABYBLUE: case F3_COLOR_WHITE:
			return 0;
		default: return -1;
	}
	return -1;
}
#if 0
int main()
{
	debug();
	COLOR_Fmt *fmt = getColorFmt(3, 31, 42, NULL);
	printf("%sHello%s\n", ((COLOR_Fmt*)getColorFmt(2, 31, 44, NULL))->fmt_1, fmt->fmt_2);
	free(fmt);
	
	return 0;
}
#endif
```
### t_color.h
```cpp
/*	file name: 		t_color.h
 *	author:			Rong Tao
 *	create time:	2018.11.10
 *	modify time:	2018.11.11
 *	modify time:	2018.11.13
 */
#ifndef _T_COLOR_H_
#define _T_COLOR_H_
#include <stdio.h>
#if defined(__linux__) || defined(x86_64) 
#define COLORFUL_FMT
#else
#define COLORNONE
#endif
/* \033[way;front color;background color m Hello World! \033[m 
 * way: 
 * 	0-default, 1-bold, 2-gray, 3-xieti, 4-xiahuaxian, 7-fanxian, 9-delete line 
 * 
 * front color
 * 	30-black, 31-red, 32-green,	33-yellow, 34-dark purple, 35-purple, 36-dark blue, 37-white 
 * 
 * background color 
 * 	40-black, 41-red, 42-green, 43-yellow, 44-blue, 45purple,46-low blue, 47-white 
 */
#define F1_COLOR_NUM	7
#define F2_COLOR_NUM	8
#define F3_COLOR_NUM	8
#define	F1_COLOR_DEFAULT	0
#define F1_COLOR_BOLD		1
#define F1_COLOR_GRAY		2
#define F1_COLOR_ITALIC		3	/*xieti*/
#define F1_COLOR_UNDERLINE	4	/*xiahauxian*/
#define F1_COLOR_INVERT		7	/*fanxian*/
#define F1_COLOR_DELETELINE	9	
#define F2_COLOR_BLACK		30
#define F2_COLOR_RED		31
#define F2_COLOR_GREEN		32
#define F2_COLOR_YELLOW		33
#define F2_COLOR_DARKPURPLE	34
#define F2_COLOR_PURPLE		35
#define F2_COLOR_DARKBLUE	36
#define F2_COLOR_WHITE		37
#define F3_COLOR_BLACK		40
#define F3_COLOR_RED		41
#define F3_COLOR_GREEN		42
#define F3_COLOR_YELLOW		43
#define F3_COLOR_BLUE		44
#define F3_COLOR_PURPLE		45
#define F3_COLOR_BABYBLUE	46
#define F3_COLOR_WHITE		47
#ifdef COLORFUL_FMT
#define COLOR_FMT_ERROR			"\033[31m"
#define COLOR_FMT_WARNING		"\033[35m"
#define COLOR_FMT_INFO			"\033[36m"
#define COLOR_FMT_SUCCESS		"\033[32m"
#define COLOR_FMT_SUMMARY		"\033[3;32m"
#define COLOR_FMT_TIME			"\033[4;33m"
#define COLOR_FMT_DEBUG			"\033[1m"
#define COLOR_FMT_END			"\033[0m"
#else
#define COLOR_FMT_ERROR			""
#define COLOR_FMT_WARNING		""
#define COLOR_FMT_INFO			""
#define COLOR_FMT_SUCCESS		""
#define COLOR_FMT_SUMMARY		""
#define COLOR_FMT_TIME			""
#define COLOR_FMT_DEBUG			""
#define COLOR_FMT_END			""
#endif
typedef unsigned int f123_color_t;
typedef struct{
	unsigned int font;
	unsigned int fcolor;
	unsigned int bcolor;
	char fmt_1[64];
	char fmt_2[64];
}COLOR_Fmt;
/**
 *	Get color fmt: font, front color, background color to COLOT_Fmt struct
 *	Author: Rong Tao
 *	Time:	2018.11.10
 */
void* getColorFmt(f123_color_t, f123_color_t, f123_color_t, COLOR_Fmt*);
/**
 *	Check f123 format is valiable
 *	Author: Rong Tao
 *	Time:	2018.11.13
 */
int f1_color_t_outofrange(f123_color_t);
int f2_color_t_outofrange(f123_color_t);
int f3_color_t_outofrange(f123_color_t);
#endif /*<_T_COLOR_H_>*/
```
### 编译运行
```
$ gcc t_log.c t_color.c
$ ./a.out 
[ error ] rongtao1230.000000
[ info  ] rongtao1230.000000
[success] rongtao1230.000000
[warning] rongtao1230.000000
[ debug ] rongtao1230.000000
[summary] helllowrongadfadsfadf
[ time  ] Nov 17 2018:16:20:02
ronghellow_______ asdfadf
1:0, 2:31, 1:40
adsfadsf1
32123123123adfadf123
------------------------------------------------------------------
```
而实际上是有颜色的
![](https://img-blog.csdnimg.cn/20181117162119979.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1JvbmdfVG9h,size_16,color_FFFFFF,t_70)
也可以选择输出到文件txt，在程序中的初始化函数中决定
```
//t_log_init(LOG_TEXT, "log.txt");
//t_log_init(LOG_SCREEN, NULL);
```
