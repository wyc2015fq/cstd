# LINUX系统中动态链接库的创建与使用{补充} - DoubleLi - 博客园






 大家都知道，在WINDOWS系统中有很多的动态链接库(以.DLL为后缀的文件，DLL即Dynamic Link Library)。这种动态链接库，和静态函数库不同，它里面的函数并不是执行程序本身的一部分，而是根据执行程序需要按需装入，同时其执行代码可在多个执行程序间共享，节省了空间，提高了效率，具备很高的灵活性，得到越来越多程序员和用户的青睐。那么，在LINUX系统中有无这样的函数库呢？



答案是肯定的，LINUX的动态链接库不仅有，而且为数不少。在/lib目录下，就有许多以.so作后缀的文件，这就是LINUX系统应用的动态链接库，只不过与WINDOWS叫法不同，它叫so，即Shared Object，共享对象。(在LINUX下，静态函数库是以.a作后缀的) X-WINDOW作为LINUX下的标准图形窗口界面，它本身就采用了很多的动态链接库(在/usr/X11R6/lib目录下)，以方便程序间的共享，节省占用空间。著名的APACHE网页服务器，也采用了动态链接库，以便扩充程序功能。你只需将PHP动态链接库拷到其共享目录，修改一下配置，APACHE就可以支持PHP网页了。如果你愿意，可以自己编写动态链接库，让APACHE支持你自己定义的网页格式。这就是动态链接的好处。


1、LINUX下动态链接库的创建
       在LINUX系统下，创建动态链接库是件再简单不过的事情。只要在编译函数库源程序时加上-shared选项即可，这样所生成的执行程序即为动态链接库。从某种意义上来说，动态链接库也是一种执行程序。按一般规则，程序名应带.so后缀。下面举个例子说说。我准备编写两个函数，一个用于查询当前日期getdate，一个用于查询当前时间gettime，并将这两个函数存于动态链接库my.so中。为此，需要做以下几项工作。


1.1 编写用户接口文件datetime.h，内容如下(每行前面的数字为行号)：



**[cpp]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- /* datetime.h :*/  
- 
- #ifndef __DATETIME_H                                                    
- 
- #define __DATETIME_H                                                    
- 
- /* 日期结构 */  
- typedef struct  
- {  
- int year;  
- int mon;  
- int day;  
- }DATETYPE;  
- 
- /* 时间结构 */  
- typedef struct  
- {  
- char hour;  
- char min;  
- char sec;  
- } TIMETYPE;  
- 
- /* 函数原型说明 */  
- 
- #ifdef SHARED                                                          
- int (*getdate)(DATETYPE *d);  
- #else                                                                  
- int getdate(DATETYPE *d);  
- #endif                                                                 
- 
- #ifdef SHARED                                                          
- int (*gettime)(TIMETYPE *t);  
- #else                                                                  
- int gettime(TIMETYPE *t);  
- #endif                                                                 
- 
- #endif                                                                 


这个用户接口文件中，先定义了日期与时间结构，接着定义一下函数的原型。动态函数与静态函数的原型说明不同的是，动态函数应使用(*函数名)的形式，以便引用其指针。若要引用文件中的动态函数说明，用户应该定义一下SHARED宏，这样才能使用。


1.2 编写getdate.c，源程序如下：



**[cpp]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- /* getdate.c : */  
- 
- ＃include "time.h"  
- ＃include "datetime.h"  
- 
- int getdate(DATETYPE *d)  
- {  
- long ti;  
- struct tm *tm;  
- 
-     time(&ti);  
- tm=localtime(&ti);  
-     d->year=tm->tm_year+1900;  
-     d->mon=tm->tm_mon+1;  
-     d->day=tm->tm_mday;  
- }  


在getdate函数中，先调用time取得以秒计的系统时间，再用localtime函数转换一下时间结构，最后调整得到正确的日期。


1.3 编写gettime.c，源程序如下：



**[cpp]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- /* gettime.c :  */  
- 
- ＃include "time.h"  
- ＃include "datetime.h"  
- 
- int gettime(TIMETYPE *t)  
- {  
- long ti;  
- struct tm *tm;  
- 
-     time(&ti);  
- tm=localtime(&ti);  
-     t->hour=tm->tm_hour;  
-     t->min=tm->tm_min;  
-     t->sec=tm->tm_sec;  
- }  


gettime函数与getdate函数相仿，先用time函数取得以秒计的系统时间，再用localtime函数转换一下时间结构，最后返回当前的时间(不需调整)。

1.4 编写维护文件makefile-lib，内容如下：



**[plain]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- # makefile-lib : .  
- 
- all : my.so  
- 
- SRC = getdate.c gettime.c  
- 
- TGT = $(SRC:.c=.o)  
- 
- $(SRC) : datetime.h  
- @touch $@  
- 
- %.o : %.c  
- cc -c $?  
- 
- # 动态函数库(my.so)生成  
- my.so : $(TGT)  
- cc -shared -o $@ $(TGT)  



编写维护文件的目的，在于方便程序员维护程序，尤其是维护比较大的工程项目。一个素质良好的程序员应该学会熟练地编写维护文件makefile。定义了文件间的依赖关系后，一旦源文件发生变化，仅需make一下，其目标文件维护代码会自动执行，从而自动更新目标文件，减少了许多工作量。

注意: 每行维护代码必须以TAB(跳格键)开始，不是的话make时将出错。本维护文件第1行是注释行，以#号开头；

文件第3行定义所有需要维护的函数库；

第5行定义相关源程序文件；第7行定义目标文件；

第9-10行说明所有源程序依赖于datetime.h头文件，并有相应维护代码，即touch一下，更新一下源文件的时间；

第12-13行定义.o文件依赖于相应的.c文件，并指定了维护代码，即用cc编译一下；

第16-17行定义共享库my.so依赖的目标文件，维护代码中用-shared编译选项，以生成动态链接库my.so。


1.5 运行make -f makefile-lib 命令
make运行后，动态链接库my.so就产生了，我们就可以在程序中调用了。如果想让系统所有用户都可以使用，则应以root用户登录系统，将这个库拷贝到/lib目录下(命令：cp my.so /lib)，或者在/lib目录下建个符号连接即可(命令：ln -s `pwd`/my.so /lib)。


2、LINUX下动态链接库的使用
2.1 重要的dlfcn.h头文件
LINUX下使用动态链接库，源程序需要包含dlfcn.h头文件，此文件定义了调用动态链接库的函数的原型。下面详细说明一下这些函数。


2.1.1 dlerror
原型为: const char *dlerror(void);

当动态链接库操作函数执行失败时，dlerror可以返回出错信息，返回值为


NULL时表示操作函数执行成功。


2.1.2 dlopen
原型为: void *dlopen (const char *filename, int flag);
dlopen用于打开指定名字(filename)的动态链接库，并返回操作句柄。
filename: 如果名字不以/开头，则非绝对路径名，将按下列先后顺序查找该


文件。


(1) 用户环境变量中的LD_LIBRARY值；
(2) 动态链接缓冲文件/etc/ld.so.cache
(3) 目录/lib，/usr/lib
flag表示在什么时候解决未定义的符号(调用)。取值有两个:
1) RTLD_LAZY : 表明在动态链接库的函数代码执行时解决。
2) RTLD_NOW : 表明在dlopen返回前就解决所有未定义的符号，一旦未解决，dlopen将返回错误。


dlopen调用失败时，将返回NULL值，否则返回的是操作句柄。
2.1.3 dlsym : 取函数执行地址
原型为: void *dlsym(void *handle, char *symbol);
dlsym根据动态链接库操作句柄(handle)与符号(symbol)，返回符号对应的函


数的执行代码地址。由此地址，可以带参数执行相应的函数。


如程序代码: void (*add)(int x,int y); /* 说明一下要调用的动态函数add */
add=dlsym("xxx.so","add"); /* 打开xxx.so共享库,取add函数地址 */
add(89,369); /* 带两个参数89和369调用add函数 */
2.1.4 dlclose : 关闭动态链接库
原型为: int dlclose (void *handle);
dlclose用于关闭指定句柄的动态链接库，只有当此动态链接库的使用计数为0时,才会真正被系统卸载。


2.2 在程序中使用动态链接库函数
2.2.1 程序范例
下面的程序装载了动态链接库my.so，并用getdate,gettime取得当前日期与


时间后输出。



**[cpp]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- #include "stdio.h" /* 包含标准输入输出文件 */  
- #include "dlfcn.h" /* 包含动态链接功能接口文件 */  
- #define SOFILE "./my.so" /* 指定动态链接库名称 */  
- #define SHARED /* 定义宏,确认共享,以便引用动态函数 */  
- #include "datetime.h" /* 包含用户接口文件 */  
- 
- main()  
- {  
-     DATETYPE d;  
-     TIMETYPE t;  
- void *dp;  
- char *error;  
- 
-     puts("动态链接库应用示范");  
- 
-     dp=dlopen(SOFILE,RTLD_LAZY); /* 打开动态链接库 */  
- 
- if (dp==NULL) /* 若打开失败则退出 */  
-     {  
-         fputs(dlerror(),stderr);  
-         exit(1);  
-     }  
- 
-     getdate=dlsym(dp,"getdate"); /* 定位取日期函数 */  
- 
-     error=dlerror(); /* 检测错误 */  
- if (error) /* 若出错则退出 */  
-     {  
-         fputs(error,stderr);  
-         exit(1);  
-     }  
- 
-     getdate(&d); /* 调用此共享函数 */  
-     printf("当前日期: %04d-%02d-%02d\n",d.year,d.mon,d.day);  
- 
-     gettime=dlsym(dp,"gettime"); /* 定位取时间函数 */  
- 
-     error=dlerror(); /* 检测错误 */  
- if (error) /* 若出错则退出 */  
-     {  
-         fputs(error,stderr);  
-         exit(1);  
-     }  
- 
-     gettime(&t); /* 调用此共享函数 */  
-     printf("当前时间: %02d:%02d:%02d\n",t.hour,t.min,t.sec);  
- 
-     dlclose(dp); /* 关闭共享库 */  
- 
-     exit(0); /* 成功返回 */  
- 
- }  





程序说明:
第8行: 包含标准输入输出头文件,因为程序中使用了printf,puts,fputs等标准输入输出函数,需要让编译器根据头文件中函数的原型,检查一下语法;
第10-11行: 包含动态链接库功能头文件,并定义动态链接库名称;
第13-14行: 定义宏SHARED以便引用14行的头文件datetime.h中的动态函数说明;
第25行: 用dlopen打开SOFILE共享库,返回句柄dp;
第27-31行: 检测dp是否为空,为空则显示错误后退出;
第33行: 用dlsym取得getdate函数动态地址;
第35-40行: 如果dlerror返回值不为空,则dlsym执行出错,程序显示错误后退出;
第42-43行: 执行getdate调用,输出当前日期;
第45行: 用dlsym取得gettime函数动态地址;
第47-52行: 如果dlerror返回值不为空,则dlsym执行出错,程序显示错误后退出;
第54-55行: 执行gettime调用,输出当前时间;
第57行: 用dlclose关闭dp所指示的动态链接库;
第59行: 程序退出,返回0值。
2.2.2 编写维护文件
维护文件makefile内容如下:



**[plain]**[view plain](http://blog.csdn.net/dragon101788/article/details/32330919)[copy](http://blog.csdn.net/dragon101788/article/details/32330919)



- # makefile : 纵横软件制作中心雨亦奇编写, 2001-06-28.  
- 
- all : dy  
- 
- DYSRC = dy.c  
- 
- DYTGT = $(DYSRC:.c=.o)  
- 
- %.o : %.c  
-     cc -c $?  
- 
- # 动态库应用示范程序  
- dy : $(DYTGT)  
-     cc -rdynamic -s -o $@ $(DYTGT) -ldl  





维护文件说明:
第3行: 定义所有需要维护的模块;
第5行: 定义源程序;
第7行: 定义目标文件;
第9-10行: 定义.o文件依赖于.c文件,维护代码为“cc -c 变动的源文件名”;
第13-14行: 定义dy依赖于变量DYTGT指示的值,维护代码中采用-rdynamic选项以指定输出文件为动态链接的方式，选项-s指定删除目标文件中的符号表,最后的选项-ldl则指示装配程序ld需要装载dl函数库。


2.2.3 运行make命令
运行make后将产生执行文件dy，运行后将产生如下类似信息：

动态链接库应用示范
当前日期: 2001-06-28
当前时间: 10:06:21
当删除my.so文件时,将出现以下信息:
动态链接库应用示范
my.so: cannot open shared object file: 文件或目录不存在
3、小结
LINUX创建与使用动态链接库并不是一件难事。编译函数源程序时选用-shared选项即可创建动态链接库，注意应以.so后缀命名，最好放到公用库目录(如/lib,/usr/lib等)下面，并要写好用户接口文件，以便其它用户共享。使用动态链接库，源程序中要包含dlfcn.h头文件，写程序时注意dlopen等函数的正确调用，编译时要采用-rdynamic选项与-ldl选项，以产生可调用动态链接库的执行代码。









