# linux移植错误大全 - 三少GG - CSDN博客
2014年08月06日 07:22:58[三少GG](https://me.csdn.net/scut1135)阅读数：3054
uint32_t has not been declared
#include <stdint.h>
no matching function for call to find(
You forgot to #include <algorithm>
stderr was not declared in this scope
#include <stdio.h>
NULL was not declared in this scope
#include <stdlib.h>
exit was not declared in this scope
#include <stdlib.h>
memset was not declared in this scope
#include <string.h>
bzero was not declared in this scope
#include <string.h>
"invalid conversion from 'const char*' to 'char' [-fpermissive]" 
invalid conversion from const char* to char* [-fpermissive]
int* modifier = const_cast<int*>(const_p);
/usr/bin/ld: cannot find -lz
I had the exact same error, and like you, installing zlib1g-dev did not fix it. Installing lib32z1-dev got me past it. I have a 64 bit system and it seems like it wanted the 32 bit library.
/usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-linux-gnu/4.6/libstdc++.so when searching for -lstdc++
/usr/bin/ld: skipping incompatible /usr/lib/gcc/x86_64-linux-gnu/4.6/libstdc++.a when searching for -lstdc++
/usr/bin/ld: cannot find -lstdc++
Posting for future reference, a solution I found was to install g++-multilib.
typedef was ignored in this declaration [-Werror]
去掉 -Werror
 error: format not a string literal and no format arguments [-Werror=format-security]
You can also put this in your Application.mk to disable treating those warnings as errors: APP_CFLAGS += -Wno-error=format-security
-Wno-error=unused-result
archive.h: No such file or directory
You need to install libarchive-dev.
************************************s*p*p*******************
error: chmod was not declared in this scope
 #include <sys/stat.h>
strncpy was not declared in this scope
#include <string.h>
INT_MAX was not declared in this scope
#include <limits.h>
time_t does not name a type
include <time.h>
error: lseek was not declared in this scope
error: ::close has not been declared
#include <unistd.h>
snprintf was not declared in this scope
#include <stdio.h>
undefined reference to `dlclose'
在makefile的 ldflags 后面把选项 -ldl添加上即可(注意链接顺序）
no matching function for call to stat::stat(const char [15], stat*)
`#include <sys/stat.h>`
