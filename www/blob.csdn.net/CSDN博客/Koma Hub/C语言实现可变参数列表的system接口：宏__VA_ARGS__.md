# C语言实现可变参数列表的system接口：宏__VA_ARGS__ - Koma Hub - CSDN博客
2018年11月18日 17:39:39[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：39
个人分类：[C/C++																[Shell](https://blog.csdn.net/Rong_Toa/article/category/7346222)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**目录**
[t_shell.h](#t_shell.h)
[t_shell.c](#t_shell.c)
[效果](#%E6%95%88%E6%9E%9C)
### t_shell.h
```cpp
/*	file name: 		t_shell.h
 *	author:			Rong Tao
 *	create time:	2018.11.14
 * 	
 */
#ifndef _T_SYS_SHELL_CMD_H_
#define _T_SYS_SHELL_CMD_H_
#ifndef _T_SHELL_STRING_MAXLEN_
#define _T_SHELL_STRING_MAXLEN_		1024
#endif 
#define t_sh_cmd(fmt, ...) _t_execfmt2string_(NULL, fmt, __VA_ARGS__)
/**
 *	Execute a format string by shell command
 *	Author: rongtao
 *	Time:	2018.11.15
 *
 */
int _t_execfmt2string_(char *string, const char *fmt, ...);
#endif /*<_T_SYS_SHELL_CMD_H_>*/
```
### t_shell.c
```cpp
/*	file name: 		t_shell.c
 *	author:			Rong Tao
 *	create time:	2018.11.15
 *	create time:	2018.11.17
 * 	
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h> 
#include "t_shell.h"
#include "t_user.h"
#define t_shell_getlogin() getlogin()
#define t_shell_getcwd(buf,size) getcwd(buf,size)
#ifndef _T_SHELL_COLOR_RED__
#define _T_SHELL_COLOR_RED__ "\033[31m"
#endif 
#ifndef _T_SHELL_COLOR_GREEN__
#define _T_SHELL_COLOR_GREEN__ "\033[32m"
#endif
#ifndef _T_SHELL_COLOR_END__
#define _T_SHELL_COLOR_END__ "\033[0m"
#endif
FILE *t_fp_sys_shell;
/**
 *	Execute a format string by shell command
 *	Author: rongtao
 *	Time:	2018.11.15
 *
 */
int _t_execfmt2string_(char *string, const char *fmt, ...)
{
	if((string = (char*)malloc(_T_SHELL_STRING_MAXLEN_)) == NULL)
	{
		return -1;
	}
	va_list arg;
	va_start(arg, fmt);
	vsprintf(string, fmt, arg);
	va_end(arg);
	char folder[_T_SHELL_STRING_MAXLEN_];
	t_shell_getcwd(folder, _T_SHELL_STRING_MAXLEN_);
	char *user = t_getcurrentusername(NULL);
	char *grp = t_getcurrentgrpname(NULL);
	char *shell = t_getcurrentusershell(NULL);
	fprintf(stdout, "%s[%s@%s-%s %s]%c %s", 
		strcmp(user, "root")==0?_T_SHELL_COLOR_RED__:_T_SHELL_COLOR_GREEN__,
		user,grp,
		basename(shell), 
		basename((char*)folder), strcmp(user, "root")==0?'#':'$',
		_T_SHELL_COLOR_END__);
	
	fprintf(stdout, "%s\n", string);
	
	system(string);
	
	free(string);
	free(user);
	free(grp);
	free(shell);
	
	return 0;
}
#if 1
int main()
{
	t_sh_cmd("ls %s; mkdir %s; rm -rf %s", "/home/rongtao", "1a.out", "1a.out");
}
#endif
```
### 效果
```
$ gcc t_shell.c t_user.c
$ ./a.out 
$ ls /home/rongtao; mkdir 1a.out; rm -rf 1a.out
clean.sh  文档
```
实际上的效果
![](https://img-blog.csdnimg.cn/2018111817350832.PNG)
今天发现个好玩的linux注释：
![](https://img-blog.csdnimg.cn/20181118173846132.PNG)
