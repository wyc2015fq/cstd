# 随机数——Linux C编程(ZZ) - xqhrs232的专栏 - CSDN博客
2016年05月12日 18:03:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：380
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.sina.com.cn/s/blog_6e4728e50100zbcl.html](http://blog.sina.com.cn/s/blog_6e4728e50100zbcl.html)
相关文章
1、[linux产生随机数](http://blog.csdn.net/imxiangzi/article/details/7710982)----[http://blog.csdn.net/imxiangzi/article/details/7710982](http://blog.csdn.net/imxiangzi/article/details/7710982)
2、[linux shell实现随机数多种方法（date,random,uuid)](http://www.cnblogs.com/chengmo/archive/2010/10/23/1858879.html)----[http://www.cnblogs.com/chengmo/archive/2010/10/23/1858879.html](http://www.cnblogs.com/chengmo/archive/2010/10/23/1858879.html)
ZZ: http://aijiekj.blog.163.com/blog/static/1298667892011215113049124/
随机数
当我们听到这个词时，第一个听到应该是C中的rand()函数吧！我也是这样的，下样就一个关于这产生随机数的小例子：
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
int
 i;
for(i=0;i<
 10;i++)
{
printf("
 %d ", rand());
}
printf("\n");
return
 0;
}
当我编译运行时，他的运行结果都是一样的，例如下面的运行结果：
FreeOS@FreeOS:~/source/test$ ./rand
846930886  1714636915  424238335  1649760492  1189641421  1350490027  1102520059 1967513926  1540383426  1303455736
FreeOS@FreeOS:~/source/test$ ./rand
846930886  1714636915  424238335  1649760492  1189641421  1350490027  1102520059 1967513926  1540383426  1303455736
我们来看看为什么会出现这样的结果吧！！
先看看他的定义：
表头文件
#include<stdlib.h>
定义函数
int
 rand(void)
函数说明
rand()会返回一随机数值，范围在0至RAND_MAX
 间。在调用此函数产生随机数前，必须先利用srand()设好随机数种子，如果未设随机数种子，rand()在调用时会自动设随机数种子为1。关于随机数种子请参考srand()。
返回值
返回0至RAND_MAX之间的随机数值，RAND_MAX定义在stdlib.h，其值为2147483647。
通过看他的函数说明我们可以了解到要想每次运行得到的随机数不同，我们还要设置随机数种子。在这里我们还要注意一下rand()返回值的范围后面可能会用到这个知识点。
既然要设置随机数种子，那我们就看看如何设置吧！那我们先看看上面提到srand()这个函数，下面就是关于srand()函数的简要描述：
表头文件
#include<stdlib.h>
定义函数
void
 srand (unsigned int seed);
函数说明
srand()用来设置rand()产生随机数时的随机数种子。参数seed必须是个整数，通常可以利用geypid()或time(0)的返回值来当做seed。如果每次seed都设相同值，rand()所产生的随机数值每次就会一样。
大体知道怎么用了吧！我们还是先看一个例子：
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[])
{
int
 i;
srand((int)
 time(0));
for(i=0;i<
 10;i++)
{
printf("
 %d ", rand());
}
printf("\n");
return
 0;
}
我们再进行编译运行看看运行结果吧！
FreeOS@FreeOS:~/source/test$ gcc rand.c -o rand
FreeOS@FreeOS:~/source/test$ ./rand
1592998629  62065346  1618188255  474778836  1137226535  1855330403  1479439673 728663645  1630754993  1335724443
FreeOS@FreeOS:~/source/test$ ./rand
68161350  37176306  1830473139  775793046  482242175  303569985  918670071  1691631506 1020365576  1949831114
运行结果是不是不同了，但是我们我们怎么利用这些这些随机数呢？？这么大的数对我们来说也没有多大意义呀！是这样的，我们可对他进行范围化分，就可以产生我们想要范围内的数。那我们就开始吧！
提前声明下面的这段文字来自己网络，摘自：http://blog.csdn.net/Chris_Magic/archive/2010/10/21/5957350.aspx
产生一定范围随机数的通用表示公式
要取得[a,b)的随机整数，使用(rand()
 % (b-a))+ a;
要取得[a,b]的随机整数，使用(rand()
 % (b-a+1))+ a;
要取得(a,b]的随机整数，使用(rand()
 % (b-a))+ a + 1;
通用公式:a
 + rand() % n；其中的a是起始值，n是整数的范围。
要取得a到b之间的随机整数，另一种表示：a
 + (int)b * rand() / (RAND_MAX + 1)。
要取得0～1之间的浮点数，可以使用rand()
 / double(RAND_MAX)。
我们来年这样一个例子：
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(int argc, char *argv[])
{
int
 i, nu;
srand((int)
 time(0));
for(i=0;i<
 10;i++)
{
nu
 = 0 + (int)( 26.0 *rand()/(RAND_MAX + 1.0));
printf("
 %d ", nu);
}
printf("\n");
return
 0;
}
再看看运行结果：
FreeOS@FreeOS:~/source/test$ ./rand
2  22  3  17  19  20  6  25  18  11
FreeOS@FreeOS:~/source/test$ ./rand
0  25  9  24  25  3  17  17  23  7
FreeOS@FreeOS:~/source/test$ ./rand
15  6  7  10  7  21  7  1  21  13
这个小例子主要是生成［0，26］之间的数，一会儿我会用这个小例子生成随机字符串。但是这里还要先说说这条语句“nu = 0 + (int)( 26.0 *rand()/(RAND_MAX + 1.0));”，有心的人会说，怎么和上面的通用公式有点出入呀？？？
如果我们使用通用公式的话，在编译程序时加上“—Wall”的话，就会看到下面的情况：
FreeOS@FreeOS:~/source/test$ gcc rand.c -o rand -Wall
rand.c: In function ‘main’:
rand.c:12: warning: integer overflow in expression（整数溢出在表达）
这就是为什么上面我你们注意rand()函数的返回值的范围了。
好了，随机数的问题就说完了，那我看看如果用随机数生成随机字符串吧！
下面就是一个自己小的小程序，代码如下：
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main(int argc, char *argv[])
{
int
 leng =128 ;
int
 i, nu ;
char
 buffer[256+1] ;
printf("Please
 Input length for the String, Default is 128, The Maxest legth is 256:");
fgets(buffer,
 257, stdin);
buffer[strlen(buffer)-1]
 = '\0' ;
if(buffer[0]
 == '\0')
