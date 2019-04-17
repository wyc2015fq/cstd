# C语言中字符串常量到底存在哪了？ - DoubleLi - 博客园






常量存储总结
局部变量、静态局部变量、全局变量、全局静态变量、字符串常量以及动态申请的内存区

1、局部变量存储在栈中
2、全局变量、静态变量（全局和局部静态变量）存储在静态存储区
3、new申请的内存是在堆中
4、字符串常量也是存储在静态存储区

补充说明：
1、栈中的变量内存会随着定义所在区间的结束自动释放；而对于堆，需要手动free，否则它就一直存在，直到程序结束；
2、对于静态存储区，其中的变量常量在程序运行期间会一直存在，不会释放，且变量常量在其中只有一份拷贝，不会出现相同的变量和常量的不同拷贝。

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
关于字符串常量，下面有篇文章解释的很清晰：

char *c="zhouming";
书上说： "zhouming"这个字符串被当作常量而且被放置在此程序的内存静态区。
c为一个字符型指针，若为局部变量，则存储在栈内，该指针变量里面存了个地址，

该地址为字符串中第一个字母Z的地址。



当使用printf（）输出时，格式化时选择%s，会输出zhouming，这是printf遇到结尾符号‘\0’即停止显示打印。



字符串“zhouming”是个常量，存储在一片连续的内存中，末尾有结尾符表示字符串的结束。



那一般的int i=1;


所有的字符窜常量都被放在静态内存区
因为字符串常量很少需要修改，放在静态内存区会提高效率

例：

char str1[] = "abcd";
char str2[] = "abcd";

const char str3[] = "abcd";
const char str4[] = "abcd";

const char *str5 = "abcd";
const char *str6 = "abcd";

char *str7 = "abcd";
char *str8 = "abcd";


cout << ( str1 == str2 ) << endl;
cout << ( str3 == str4 ) << endl;
cout << ( str5 == str6 ) << endl;
cout << ( str7 == str8 ) << endl;

结果是：0 0 1 1
str1,str2,str3,str4是数组变量，它们有各自的内存空间；字符数组作为局部变量被存储在栈区；
而str5,str6,str7,str8是指针，它们指向相同的常量区域。，"abcd"被存储在静态数据区，而且是全局的，



问题的引入：
看看下面的程序的输出：

#include <stdio.h>
char *returnStr()
{
char *p="hello world!";
return p;
}
int main()
{
char *str=NULL;//一定要初始化，好习惯
str=returnStr();
printf("%s\n", str);

return 0;
}


这个没有任何问题，因为"hello world!"是一个字符串常量，存放在静态数据区，
把该字符串常量存放的静态数据区的首地址赋值给了指针，
所以returnStr函数退出时，该该字符串常量所在内存不会被回收，故能够通过指针顺利无误的访问。

但是，下面的就有问题：
#include <stdio.h>
char *returnStr()
{
char p[]="hello world!";
return p;
}
int main()
{
char *str=NULL;//一定要初始化，好习惯
str=returnStr();
printf("%s\n", str);

return 0;
}


"hello world!"是一个字符串常量，存放在静态数据区，没错，
但是把一个字符串常量赋值给了一个局部变量(char []型数组)，该局部变量存放在栈中，
这样就有两块内容一样的内存，也就是说“char p[]="hello world!";”这条语句让“hello world!”这个字符串在内存中有两份拷贝，一份在动态分配的栈中，另一份在静态存储区。这是与前者最本质的区别，
当returnStr函数退出时，栈要清空，局部变量的内存也被清空了，
所以这时的函数返回的是一个已被释放的内存地址，所以打印出来的是乱码。

如果函数的返回值非要是一个局部变量的地址，那么该局部变量一定要申明为static类型。如下：

static 主要是为了限定范围用的。

#include <stdio.h>
char *returnStr()
{
static char p[]="hello world!";
return p;
}
int main()
{
char *str=NULL;
str=returnStr();
printf("%s\n", str);

return 0;
}


这个问题可以通过下面的一个例子来更好的说明：

#include <stdio.h>
//返回的是局部变量的地址，该地址位于动态数据区，栈里

char *s1()
{
char* p1 = "qqq";//为了测试‘char p[]="Hello world!"’中的字符串在静态存储区是否也有一份拷贝
char p[]="Hello world!";
char* p2 = "w";//为了测试‘char p[]="Hello world!"’中的字符串在静态存储区是否也有一份拷贝
printf("in s1 p=%p\n", p);
printf("in s1 p1=%p\n", p1);
printf("in s1: string's address: %p\n", &("Hello world!"));
printf("in s1 p2=%p\n", p2);
return p;
}

//返回的是字符串常量的地址，该地址位于静态数据区

char *s2()
{
char *q="Hello world!";
printf("in s2 q=%p\n", q);
printf("in s2: string's address: %p\n", &("Hello world!"));
return q;
}

//返回的是静态局部变量的地址，该地址位于静态数据区

char *s3()
{
static char r[]="Hello world!";
printf("in s3 r=%p\n", r);
printf("in s3: string's address: %p\n", &("Hello world!"));
return r;
}

int main()
{
char *t1, *t2, *t3;
t1=s1();
t2=s2();
t3=s3();

printf("in main:");
printf("p=%p, q=%p, r=%p\n", t1, t2, t3);

printf("%s\n", t1);
printf("%s\n", t2);
printf("%s\n", t3);

return 0;
}


运行输出结果：

in s1 p=0013FF0C
in s1 p1=00431084
in s1: string's address: 00431074
in s1 p2=00431070
in s2 q=00431074
in s2: string's address: 00431074
in s3 r=00434DC0
in s3: string's address: 00431074
in main:p=0013FF0C, q=00431074, r=00434DC0
$
Hello world!
Hello world!


这个结果正好应证了上面解释，同时，还可是得出一个结论：
字符串常量，之所以称之为常量，因为它可一看作是一个没有命名的字符串且为常量，存放在静态数据区。
这里说的静态数据区，是相对于堆、栈等动态数据区而言的。
静态数据区存放的是全局变量和静态变量，从这一点上来说，字符串常量又可以称之为一个无名的静态变量，
因为"Hello world!"这个字符串在函数 s1和s2 中都引用了，但在内存中却只有一份拷贝，这与静态变量性质相当神似。



另外还有个实验：





[cpp]
#include <stdio.h>  
#include <stdlib.h>  
#include <math.h>  
int main() 
{ 

    char *b; 

    char *c; 

    char a[]="chenhuijie"; 
    b="chenhuijie"; 
    c="chenhuijie"; 
    printf("%d,%d,%d,%d\n",b,a,c,&("chenhuijie")); 





} 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main()
{

 char *b;

 char *c;

 char a[]="chenhuijie";
 b="chenhuijie";
 c="chenhuijie";
    printf("%d,%d,%d,%d\n",b,a,c,&("chenhuijie"));





}



实验结果为：

4282272,1244988,4282272,4282272
请按任意键继续. . .



对了，字符常量‘a’

sizeof（‘a’）

为4，及一个字长。









