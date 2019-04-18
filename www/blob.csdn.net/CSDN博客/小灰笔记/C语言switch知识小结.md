# C语言switch知识小结 - 小灰笔记 - CSDN博客





2017年03月31日 00:04:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：419







总的说来，我使用switch的频率较少。因为，我使用的条件转移分支通常都很少，使用if处理已经基本够用。再者，分支少的时候switch在效率上也没有什么大的优势。加之switch写起来在复杂度上也比if麻烦不少，我使用的也就少了些。
不过，使用switch一般需要注意几个地方：
1，通常，每个分支后面都会有break，如果该有没有则会造成程序的错误；
2，default可以加在任何地方而不是必须放在最后；
3，条件分支的标签必须是常量；
4，标签使用可以随意。
正常的使用不再做什么赘述，做两个“诡异”一点的测试：

测试1：

#include "stdio.h"



int main()

{
int num = 0;
const int case_num = 1;


switch(num)
{
case case_num:
printf("xxxx");
break;
case 3:
printf("aaaa");
break;
default:
break;
}


return 0;

}
编译提示：

E:\WorkSpace\01_编程语言\01_C语言\exp_14>gcc exp_14.c

exp_14.c: In function 'main':

exp_14.c:10:3: error: case label does not reduce to an integer constant

   case case_num:
这个地方能够看得出，const其实并不是常量，而是仅仅限定一个量为只读。

测试2：

#include "stdio.h"



int main()

{
int num = 0;
const int case_num = 1;


switch(num)
{
case 1:
printf("xxxx");
break;
case 3:
printf("aaaa");
break;
defaulttttt:
break;
}


return 0;

}
上面的这段代码，倒是能够顺利编译通过。然而，上面的最后一个case倒不能够代替default的分支，只是能够编译通过而已。能够比较随意地使用分支标签，这个确实是有点不寻常理了。不过，如此引入bug几乎是必然的了。
            


