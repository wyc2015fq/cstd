# C++递归法求解汉诺塔问题 - fanyun的博客 - CSDN博客
2018年10月30日 07:20:44[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：112
1.汉诺塔问题分析
![](https://img-blog.csdnimg.cn/20181029152140620.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_27,color_FFFFFF,t_70)
1个盘子：直接移动， “N==1”是递归终结条件。 
N个盘子：吧移动N个盘子的问题转化为移动N-1盘子的问题。 
(1)把A上面的N-1个盘子移动B（借助C）； 
(2)把第N个盘子一道C； 
(3)把B上的N-1个盘子移到C（借助A）
2.实现代码如下
```cpp
#include <iostream>
using namespace std;
void hanoi(int N ,char source , char relay ,char destination)
{
    if(N == 1)
        cout << source << "-->" << destination << endl ;
    else
    {
        hanoi(N-1 , source , destination , relay) ;
        cout << source << "-->" << destination << endl ;
        hanoi(N-1 , relay , source , destination) ;
    }
}
int main()
{
    cout << "移动盘子：" << endl ;
    hanoi(3, 'A' , 'B' , 'C') ;
    return 0;
}
```
执行结果如下：
![](https://img-blog.csdnimg.cn/20181029152558695.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zhbnl1bl8wMQ==,size_27,color_FFFFFF,t_70)
3.递归法与递推法的比较
(1).递归法比递推法速度慢；
(2).递归法比递推法适用范围广；
