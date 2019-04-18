# C语言中的函数声明 - 小灰笔记 - CSDN博客





2017年03月30日 23:44:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：546







C语言中的函数声明有三种方法：第一，使用extern关键字进行全局可用的声明；第二，使用不带extern关键字的默认声明方式，这种方式功能与第一种相同；第三，使用static关键字将函数声明为私有函数。
方法1，使用extern关键字进行全局函数的声明。创建包含exp_13.c、exp_13.h、main.c三个文件的工程。工程代码如下：

exp_13.c

#include "exp_13.h"



void MyPrint(void)

{
printf("my print function\n");

}



exp_13.h

#include "stdio.h"



extern void MyPrint(void);



main.c

#include "exp_13.h"



int main(void)

{
MyPrint();


return 0;

}

E:\WorkSpace\01_编程语言\01_C语言\exp_13>gmake

gcc    -c -o exp_13.o exp_13.c

gcc    -c -o main.o main.c

gcc -o app exp_13.o main.o



E:\WorkSpace\01_编程语言\01_C语言\exp_13>app.exe

my print function
由上面的结果可以看出，函数能够在全局的范围内被调用。
方法2：使用默认的声明方式，修改exp_13.h代码如下：

#include "stdio.h"



void MyPrint(void);
代码工程编译执行如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_13>gmake

gcc    -c -o exp_13.o exp_13.c

gcc    -c -o main.o main.c

gcc -o app exp_13.o main.o



E:\WorkSpace\01_编程语言\01_C语言\exp_13>app.exe

my print function
看得出，程序的执行正常。
方法3：把函数声明为私有函数，修改代码细节如下：

Exp_13.c

#include "exp_13.h"



static void MyPrint(void);



void MyPrint(void)

{
printf("my print function\n");

}



Exp_13.h

#include "stdio.h"



/*

void MyPrint(void);

*/

Main.c

#include "exp_13.h"



extern void MyPrint(void);



int main(void)

{
MyPrint();


return 0;

}
如果是把函数定义为static，函数变具有了私有属性，对本文件可见。即使是在外部使用了extern关键字声明也不能够正确编译。编译提示如下：

E:\WorkSpace\01_编程语言\01_C语言\exp_13>gmake

gcc    -c -o exp_13.o exp_13.c

gcc    -c -o main.o main.c

gcc -o app exp_13.o main.o

main.o:main.c:(.text+0xc): undefined reference to `MyPrint'

collect2.exe: error: ld returned 1 exit status

gmake: *** [app] Error 1
在《C专家编程》翻译本中提到，大多数时候全局函数的声明一般都不会使用extern关键字。不过，从我自己的个人编程实践经验来看，一直以来似乎少见不使用extern的时候。说起来，获取还真是谨慎为上。
如果函数使用static在头文件中进行声明，最终的作用域还是会在相应的C文件中。这是跟编译实现的机理有关的，编译器的预处理器会在编译的时候把所需的头文件信息插入到C文件中。工程修改如下：

exp_13.c

#include "exp_13.h"



void MyPrint(void)

{
printf("my print function\n");

}



exp_13.h

#include "stdio.h"



static void MyPrint(void);



#include "exp_13.h"



extern void MyPrint(void);



main.c

int main(void)

{
MyPrint();


return 0;

}
编译编译提示：

E:\WorkSpace\01_编程语言\01_C语言\exp_13>gmake

gcc    -c -o exp_13.o exp_13.c

gcc    -c -o main.o main.c

main.c:3:13: warning: 'MyPrint' used but never defined

 extern void MyPrint(void);

             ^

gcc -o app exp_13.o main.o

main.o:main.c:(.text+0xc): undefined reference to `MyPrint'

collect2.exe: error: ld returned 1 exit status

gmake: *** [app] Error 1
上面的信息，跟直接在内部声明的效果完全一样。
            


