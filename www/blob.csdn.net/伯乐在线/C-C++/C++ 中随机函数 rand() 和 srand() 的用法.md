# C++ 中随机函数 rand() 和 srand() 的用法 - 文章 - 伯乐在线
原文出处： [candyliuxj](http://blog.csdn.net/candyliuxj/article/details/4396666)
**一、rand()**
函数名:   rand
功   能:   随机数发生器
用   法:   int rand(void);
所在头文件: stdlib.h
函数说明 :
- rand()的内部实现是用线性同余法做的，它不是真的随机数，因其周期特别长，故在一定的范围里可看成是随机的。
-  rand()返回一随机数值的范围在0至RAND_MAX 间。RAND_MAX的范围最少是在32767之间(int)。用 unsigned int 双字节是65535，四字节是4294967295的整数范围。0~RAND_MAX每个数字被选中的机率是相同的。
- 用户未设定随机数种子时，系统默认的随机数种子为1。
- rand()产生的是伪随机数字，每次执行时是相同的;若要不同,用函数srand()初始化它。
程序示例:


```
#include <iostream>  
using namespace std;  
#include   <stdlib.h>      
#include   <time.h>   
#define MIN 1    //随机数产生的范围      
#define MAX 10  
  
int main()     
{   
    int i;   
    srand((unsigned)time(0));  
    cout<<"Ten random numbers from "<<MIN<<  
          " to "<<MAX<<" :/n"<<endl;     
    for(i=0; i<10; i++)          //产生随机数  
    {  
        cout<<MIN + (int)MAX * rand() / (RAND_MAX + 1)<<"/t";   
    }  
    cout<<endl;  
    return   0;     
}
```
**二、srand()**
函数名:   srand
功   能:   初始化随机数发生器
用   法:   void srand(unsigned  int  seed);
所在头文件： stdlib.h
函数说明：
- srand()用来设置rand()产生随机数时的随机数种子。
- 参数seed必须是个整数，通常可以利用time(0)的返回值或NULL来当做seed。
- 如果每次seed都设相同值，rand()所产生的随机数值每次就会一样。
程序示例:


```
#include <iostream>  
using namespace std;  
#include   <stdlib.h>      
#include   <time.h>   
#define MIN 0    //随机数产生的范围      
#define MAX 99  
  
int main()     
{   
    int i;   
    srand((unsigned)time(NULL));  
    cout<<"Ten random numbers from "<<MIN<<  
          " to "<<MAX<<" :/n"<<endl;     
    for(i=0; i<10; i++)          //产生随机数  
    {  
        cout<<MIN + rand() % (MAX + MIN - 1)<<"/t";   
    }  
    cout<<endl;  
    return   0;     
}
```
**三、rand()和srand()的关系**
rand()和srand()要一起使用，其中srand()用来初始化随机数种子,rand()用来产生随机数。
因为默认情况下随机数种子为1，而相同的随机数种子产生的随机数是一样的,失去了随机性的意义，所以为使每次得到的随机数不一样，用函数srand()初始化随机数种子。srand()的参数，用time函数值（即当前时间），因为两次调用rand()函数的时间通常是不同的，这样就可以保证随机性了。
**四、产生一定范围随机数的通用表示公式**
要取得[a,b)的随机整数，使用(rand() % (b-a))+ a （结果值含a不含b）。
要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a （结果值含a和b）。
要取得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1 （结果值不含a含b）。
（总的来说，通用公式：a + rand() % n ；其中的a是起始值，n是整数的范围）
要取得a到b之间的随机整数，另一种表示：a + (int)b * rand() / (RAND_MAX + 1)。
要取得0～1之间的浮点数，可以使用rand() / double(RAND_MAX)。
**五、产生相同的随机数的原因**
计算机的随机数都是由伪随机数，即是由小M多项式序列生成的，其中产生每个小序列都有一个初始值，即随机种子。（注意： 小M多项式序列的周期是65535，即每次利用一个随机种子生成的随机数的周期是65535，当你取得65535个随机数后它们又重复出现了。）
我们知道rand()函数可以用来产生随机数，但是这不是真真意义上的随机数，是一个伪随机数，是根据一个数（我们可以称它为种子）为基准以某个递推公式推算出来的一系列数，当这系列数很大的时候，就符合正态公布，从而相当于产生了随机数，但这不是真正的随机数，当计算机正常开机后，这个种子的值是定了的，除非你破坏了系统。
程序示例：


```
#include <iostream>  
using namespace std;  
#include <stdlib.h>  
#include <time.h>  
  
int main()  
{  
    int i;  
    for (i=0; i<10; i++)      //产生10个随机数  
    {  
        cout<<rand()<<"/t";  
    }  
    cout<<endl;  
    return 0;  
}
```
每次运行得到相同的随机序列：


```
41 18467 6334 26500 19169 15724 11478  
41 18467 6334 26500 19169 15724 11478 29358 26962 24464
```
为得到不同的随机数序列，则需改变这个种子的值。方法：在开始产生随机数前，调用一次srand(time(NULL))（注意：srand()一定要放在循环外面或者是循环调用的外面，否则的话得到的是相同的随机数）。
程序示例：


```
#include <iostream>  
using namespace std;  
#include <stdlib.h>  
#include <time.h>  
  
int main()  
{  
    int i;  
    srand((unsigned)time(NULL)); //初始化随机数种子  
    for (i=0; i<10; i++)         //产生10个随机数  
    {  
        cout<<rand()<<"/t";  
    }  
    cout<<endl;  
    return 0;  
}
```
每次运行得到不同的随机序列：


```
1294 18562 14141 18165 11910 29784 11070 13225 131 24405
1774 25714 18734 16528 20825 17189 9848 8899 2503 5375
```
