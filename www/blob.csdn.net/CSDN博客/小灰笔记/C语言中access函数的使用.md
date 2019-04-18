# C语言中access函数的使用 - 小灰笔记 - CSDN博客





2017年08月08日 22:08:35[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：18381








       GNU的C语言库其实功能还是很强大的，不过这方面我了解学习的确实是不多。最近又接触到了一个文件访问操作的函数，之前觉得只有高级语言才能够实现，没想到借用glibc用C语言实现起来也蛮简单的。这个函数的声明在io.h中，但是网络上的很多文章都写错了。不过，他们说的头文件恰好包含了io.h文件，因此代码上倒是没有什么大的问题。

       关于文件的访问模式，在io.h文件中有如下宏定义：

  /* Some defines for _access nAccessMode (MSdoesn't define them, but

  * it doesn't seem to hurt to add them). */

#define  F_OK      0     /* Check for file existence */

#define  X_OK      1     /* Check for execute permission. */

#define  W_OK     2     /* Check for write permission */

#define  R_OK      4     /* Check for read permission *

       从代码的注释中可以看到，在Windows系统中这个似乎是没有用的。看来，这个功能是专门为unix-linke的系统设计的。不过，相应的参数应该还是可以使用，只是结果或许不是我们想要的。

       函数的原型如下：

  _CRTIMP int __cdecl _access(const char*_Filename,int _AccessMode)

       写一段测试代码如下：

#include"stdio.h"

#include"io.h"



int main(void)

{

       if(access("./access_demo.c",F_OK)==0)

       {

              printf("fileexists!\n");

       }



       if(access("./access_demo.c",X_OK)==0)

       {

              printf("file can beexecuted!\n");

       }



       if(access("./access_demo.c",W_OK)==0)

       {

              printf("file can bewritten!\n");

       }



       if(access("./access_demo.c",R_OK)==0)

       {

              printf("file can beread!\n");

       }



       return 0;

}



       代码编译后程序执行结果如下：

E:\01_workSpace\02_programme_language\01_clang\2017\08\08>gccaccess_demo.c



E:\01_workSpace\02_programme_language\01_clang\2017\08\08>a

file exists!

file can beexecuted!

file can bewritten!

file can be read!

       从上面的结果可以看出，在Windows下面其实这个函数只是提供了这样的一个接口，其实并没有相应的功能。



