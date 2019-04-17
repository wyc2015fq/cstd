# c语言scanf()停止接受输入及scanf("%c",&c）吃掉回车或者空格字符的问题 - 在思索中前行！ - CSDN博客





2014年10月28日 21:38:43[_Tham](https://me.csdn.net/txl16211)阅读数：8418








scanf()函数接收输入数据时，遇以下情况结束一个数据的输入：（不是结束该scanf函数，scanf函数仅在每一个数据域均有数据，并按回车后结束）。 

        ① 遇空格、“回车”、“跳格”键。 

        ② 遇宽度结束。 

        ③ 遇非法输入。

问题二：scanf()函数不能正确接受有空格的字符串？如: I love you!



#include <stdio.h> 

int main() 

{ 

    char str[80]; 


    scanf("%s",str); 

    printf("%s",str);

    return 0; 

}

输入：I live you! 

输出：I

 scanf()函数接收输入数据时，遇以下情况结束一个数据的输入：（不是结束该scanf函数，scanf函数仅在每一个数据域均有数据，并按回车后结束）。 

        ① 遇空格、“回车”、“跳格”键。 

        ② 遇宽度结束。 

        ③ 遇非法输入。

所以，上述程序并不能达到预期目的，scanf()扫描到"I"后面的空格就认为对str的赋值结束，并忽略后面的"love you!".这里要注意是"love you!"还在键盘缓冲区（关于这个问题，网上我所见的说法都是如此，但是，我经过调试发现，其实这时缓冲区字符串首尾指针已经相等了，也就是说缓冲区清空了，scanf()函数应该只是扫描stdin流，这个残存信息是在stdin中)。我们改动一下上面的程序来验证一下：

#include <stdio.h> 

int main() 

{ 

    char str[80]; 

    char str1[80]; 

    char str2[80]; 


    scanf("%s",str);/*此处输入:I love you! */ 

    printf("%s",str); 

    sleep(5);/*这里等待5秒,告诉你程序运行到什么地方*/ 

    scanf("%s",str1);/*这两句无需你再输入,是对键盘盘缓冲区再扫描   */ 

    scanf("%s",str2);/*这两句无需你再输入,是对键盘盘缓冲区再扫描    */ 

    printf("/n%s",str1); 

    printf("/n%s",str2); 

    return 0; 

}

输入：I love you! 

输出：I 

      love 

      you!

好了，原因知道了，那么scanf()函数能不能完成这个任务？回答是：能！别忘了scanf()函数还有一个 %[] 格式控制符（如果对%[]不了解的请查看本文的上篇）,请看下面的程序：

#include "stdio.h" 

int main() 

{ 

    char string[50]; 


     /*scanf("%s",string);不能接收空格符*/ 

     scanf("%[^/n]",string); 

     printf("%s/n",string); 

     return 0; 

}

问题三：键盘缓冲区残余信息问题



#include <stdio.h> 

int main() 

{ 

    int a; 

    char c;

    do 

    { 

        scanf("%d",&a); 

        scanf("%c",&c); 

        printf("a=%d     c=%c/n",a,c); 

        /*printf("c=%d/n",c);*/ 

    }while(c!='N'); 

}

scanf("%c",&c);这句不能正常接收字符,什么原因呢？我们用printf("c=%d/n",c);将C用int表示出来，启用printf("c=%d/n",c);这一句，看看scanf()函数赋给C到底是什么，结果是 c=10 ,ASCII值为10是什么？换行即/n.对了，我们每击打一下"Enter"键，向键盘缓冲区发去一个“回车”(/r),一个“换行"(/n),在这里/r被scanf()函数处理掉了（姑且这么认为吧^_^），而/n被scanf()函数“错误”地赋给了c.

解决办法：可以在两个scanf()函数之后加个fflush(stdin);，还有加getch(); getchar();也可以，但是要视具体scanf()语句加那个，这里就不分析了，读者自己去摸索吧。但是加fflush(stdin);不管什么情况都可行。

函数名: fflush 

功 能: 清除一个流 

用 法: int fflush(FILE *stream);

#include <stdio.h> 

int main() 

{ 

    int a; 

    char c;

    do 

    { 

        scanf("%d",&a); 

        fflush(stdin); 

        scanf("%c",&c); 

        fflush(stdin); 

        printf("a=%d     c=%c/n",a,c);

    }while(c!='N'); 

}     

这里再给一个用“空格符”来处理缓冲区残余信息的示例：

运行出错的程序：

#include <stdio.h> 

int main() 

{ 

    int i; 

    char j; 

    for(i = 0;i < 10;i++) 

    { 

        scanf("%c",&j);/*这里%前没有空格*/ 

    } 

}

使用了空格控制符后：

#include <stdio.h> 

int main() 

{ 

    int i; 

    char j; 

    for(i = 0;i < 10;i++) 

    { 

        scanf(" %c",&j);/*注意这里%前有个空格*/ 

    } 

}

    可以运行看看两个程序有什么不同。

问题四   如何处理scanf()函数误输入造成程序死锁或出错？

#include <stdio.h> 

int main() 

{ 

int a,b,c; /*计算a+b*/

scanf("%d,%d",&a,&b); 

c=a+b; 

printf("%d+%d=%d",a,b,c); 

}

如上程序，如果正确输入a,b的值，那么没什么问题，但是，你不能保证使用者每一次都能正确输入，一旦输入了错误的类型，你的程序不是死锁，就是得到一个错误的结果,呵呵，这可能所有人都遇到过的问题吧？

解决方法：scanf()函数执行成功时的返回值是成功读取的变量数,也就是说，你这个scanf()函数有几个变量，如果scanf()函数全部正常读取，它就返回几。但这里还要注意另一个问题，如果输入了非法数据，键盘缓冲区就可能还个有残余信息问题。

正确的例程：

#include <stdio.h> 

int main() 

{ 

int a,b,c; /*计算a+b*/


while(scanf("%d,%d",&a,&b)!=2)fflush(stdin);

c=a+b;

printf("%d+%d=%d",a,b,c);

}



```cpp
#include<stdio.h>
void main(){
 int a;
 char b;
 printf("input a integer\n");
 scanf("%d",&a);
 //这里需要吸收 回车符  下面有种方法，第二种我没看懂 谁能改下代码 告诉我第二种怎么用
 printf("input a char\n");
 scanf("%c",&b);
 printf("%d,%c\n",a,b);
 
}
/*
(1)用getchar()清除
(2)用"%1s"来限制
(3)用格式串中空格或"%*c"来"吃掉"
(4)用函数fflush(stdin)清除全部剩余内容
*/
```

```cpp
#include<stdio.h>
int main(){ 
 
 int a;
 char b;

 printf("input a integer\n");
 scanf("%d",&a);
 //这里需要吸收 回车符  下面有种方法，第二种我没看懂 谁能改下代码 告诉我第二种怎么用
 printf("input a char\n");
 scanf("%1s",&b);

 printf("%d %c\n",a,b);
 system("pause");
 return 0;
 
}
```
环境（dev c++）



测试结果

![](https://img-my.csdn.net/uploads/201210/17/1350470781_8645.PNG)![](https://img-my.csdn.net/uploads/201210/17/1350470784_6265.PNG)


可以看出a的值异常了，为什么呢？


这应该和编译器有关系，因为%1s的输入方式的问题。


解决方法有两种：


第一：





**[cpp]**[view
 plain](http://blog.csdn.net/frank0712105003/article/details/8082600#)[copy](http://blog.csdn.net/frank0712105003/article/details/8082600#)



- char b;  
- int a;  


将这两个变量的声明换一下，哈哈，以前老师说过。但我感觉着不是解决方法，如果还有其他的变量，这种方式是不是还会危害其他呢。



第二：





**[cpp]**[view
 plain](http://blog.csdn.net/frank0712105003/article/details/8082600#)[copy](http://blog.csdn.net/frank0712105003/article/details/8082600#)



- #include<stdio.h>
- int main(){   
- 
- int a;  
- char b[2];  
- 
- 
-  printf("input a integer\n");  
-  scanf("%d",&a);  
- //这里需要吸收 回车符  下面有种方法，第二种我没看懂 谁能改下代码 告诉我第二种怎么用
-  printf("input a char\n");  
-  scanf("%1s",&b);  
- 
- 
- 
-  printf("%d %c\n",a,b[0]);  
-  system("pause");  
- return 0;  
- 
- }  


b声明成字符创的形式，哈哈，这个好。。。。



结果为：

![](https://img-my.csdn.net/uploads/201210/17/1350471031_1746.PNG)






