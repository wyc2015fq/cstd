# linux下C编程错误捕获函数 - xqhrs232的专栏 - CSDN博客
2011年05月31日 10:41:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：801
原文地址::
[http://my.chinaunix.net/space.php?uid=20539088&do=blog&id=234016](http://my.chinaunix.net/space.php?uid=20539088&do=blog&id=234016)
本文中的错误是指在代码编译完全正确程序可运行的情况下，因为没有成功调用程序中的某些系统调用函数而产生的错误。往往这些系统调用函数通过返回值（比如1，0，－1）来说明其是否调用成功，而程序员需要知道详细的错误信息，因此自建错误捕获函数很有必要。
(1)errno和strerror()
errno它是一个整形的错误代码。当发生错误的时候，系统自动将错误代码赋给errno。使用下面的方法可以获得具体的错误描述：
- void my_err(interror)
- 
- {
- 
-     printf("error: %s with errno: %d/n",strerror(error),error);
- 
- exit(1);
- 
- }
- int main()
- {
- ..............
- 
-         my_err(errno);
- 
- ..............
- 
- }
其中char *strerror(int errnum);是通过errnum来获取错误描述，errnum即所传递的errno。该函数末尾的exit(1)使得程序发生错误时退出。但应该包含库函数stdlib.h。
下面进行测试，测试程序（源代码在本文末尾。）使用open()函数创建文件，因为要创建的文件已存在，而且使用了O_EXCL参数，因此open()会产生错误。结果如下：
- edsionte@edsionte-laptop:~/code$
./error
- 
- error: File exists with errno: 17
该方法可以详细显示错误信息以及错误代码。但不能显示错误出现的行数。
(2)perror()
其函数原型为：void perror(const char *s)。s一般是函数名。该函数会先将函数名打印出来，然后再打印出错误信息。错误信息与errno相对应。第二个参数__LINE__是一个宏，表示当前的行数。使用方法：
- void my_err2(const char* err_string,int line)
- 
- {
- 
-     fprintf(stderr,"error: line:%d ",line);
- 
-     perror(err_string);
- 
- exit(1);
- 
- }
- 
- }
- 
- int main()
- {
- .................
- 
-         my_err2("open",__LINE__);
- 
- ................
- 
- }
测试结果如下：
- edsionte@edsionte-laptop:~/code$
./error
- 
- error: line:29 open: File exists
- 
- }
该方法可以显示错误信息以及错误出现的行数。
以上方法是在《linux C编程》中常用的方法，我适当的作了小调整。现在将这两种方法结合起来：
- void my_err3(const char* err_string,int line,interror)
- {
- 
-     printf("error: line:%d %s():%s with errno:%d/n",line,err_string,strerror(error),error);
- 
- exit(1);
- 
- }
- 
- int main()
- {
- ................
- 
-         my_err3("open",__LINE__,errno);
- 
- ................
- 
- }
测试结果如下：
- [edsionte@edsionte-laptop:~/code$](mailto:edsionte@edsionte-laptop:~/code$)./error
- 
- error: line:30 open():File exists with errno:17
这样就可以显示错误代码，错误描述，错误出现的行数以及出现错误的函数。对于和我一样的新手来说，这里特别要注意的是宏__LINE__前后的那个横线是两个连续的下划线，而不是_LINE_，否则会出现错误。
源代码如下：
说明：本程序只作测试用，为了同时显示三种错误捕获函数的信息，因此屏蔽了每个函数的exit(1)。另外本文头文件函数用“”是因为显示问题，没有什么特别意义。
- #include "errno.h"
- #include "fcntl.h"
- #include "unistd.h"
- #include "stdlib.h"
- #include "stdio.h 
sys/types.h"
sys/stat.h"
string.h"
error:%s with errno:%d/n",strerror(error),error);
error: line:%d
",line); 
error: line:%d
%s():%s with errno:%d/n",line,err_string,strerror(error),error);
example_test.c",O_CREAT|O_EXCL,S_IRUSR|S_IWUSR))==-1)
open",__LINE__); 
open",__LINE__,errno); 
- #include "
- #include "
- #include "
- 
- void my_err(int error) 
- 
- { 
-     printf("
- 
- // exit(1); 
- 
- } 
- 
- void my_err2(const char* err_string,int line) 
- { 
- 
-     fprintf(stderr,"
- 
-     perror(err_string); 
- 
- // exit(1); 
- 
- } 
- 
- void my_err3(const char* err_string,int line,int error) 
- { 
- 
-     printf("
- 
- // exit(1); 
- 
- } 
- 
- int main() 
- { 
-     int fd; 
-     if((fd=open("
-     { 
-         my_err(errno); 
-         my_err2("
-         my_err3("
-     } 
-     close(fd); 
-     return 0; 
- } 
