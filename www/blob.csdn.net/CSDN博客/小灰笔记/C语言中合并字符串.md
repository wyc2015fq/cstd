# C语言中合并字符串 - 小灰笔记 - CSDN博客





2017年08月08日 23:09:41[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：3747








       学会Perl以及Python之后，处理字符串也只是我很喜欢做的一件事情。进行字符串的拼接在这些高级脚本语言中是一件轻松的事情。

       C语言是我的编程入门语言，但是我一直对这门语言了解并不是很深。确切说，我是对庞大的标准库以及GNU等函数库知之甚少。

       使用C语言进行字符串的拼接应该怎么做呢？在先前，我肯定是要定义一个新的“字符串”，然后进行内存的拷贝。其实，有一个printf函数的孪生兄弟可以实现这个功能，那就是sprintf。

       其实，在接触操作系统的时候我对这个函数多少有一点认识。那就是这个函数在实现屏显的时候比printf更为底层，而且可以直接操作内存。那么，使用这个函数进行字符串的拼接应该如何做呢？

       示范代码如下：

#include"stdio.h"

#include"stdlib.h"

#include"string.h"



int main(void)

{

       char str1[] = "my string 1";

       char str2[] = "string 2";

       char *strCat = (char*)malloc(strlen(str1) + strlen(str2));



       sprintf(strCat,"%s%s",str1,str2);



       printf("%s\n",strCat);



       return 0;

}

       代码的编译执行如下：

E:\01_workSpace\02_programme_language\01_clang\2017\08\08>gccstrCat.c



E:\01_workSpace\02_programme_language\01_clang\2017\08\08>a

my string 1string2

       从上面的结果可以看出，通过sprintf实现了字符串的拼接功能。



