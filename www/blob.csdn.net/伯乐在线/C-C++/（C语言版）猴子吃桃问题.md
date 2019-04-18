# （C语言版）猴子吃桃问题 - 文章 - 伯乐在线
原文出处： [snow_5288](http://blog.csdn.net/snow_5288/article/details/52561882)
问题描述：
/*有一群猴子，去摘了一堆桃子*/
/*商量之后决定每天吃剩余桃子的一半*/
/*当每天大家吃完桃子之后，有个贪心的小猴都会偷偷再吃一个桃子*/
/*按照这样的方式猴子们每天都快乐的吃着桃子*/
/*直到第十天，当大家再想吃桃子时，发现只剩下一个桃子了*/
问：猴子们一共摘了多少桃子
问题分析：
按照题意：猴子们每天吃桃子的一半，小猴子再偷吃一个，等于每天都吃前一天剩余桃子的一半加一个，知道第十天只剩一个桃子。
如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/12/66e444ca07dc00041b2c4ceaff2375db.png)
迭代法实现：
1.采用for循环实现


```
#include<stdio.h>  
#include <stdlib.h>  
int main()  
{  
    int peach = 0;//桃子总数  
    int rest = 1;//第10 天只剩一个桃子  
    int day = 0;  
    for(day = 9; day > 0; day--)  
    {  
        peach = (rest + 1) * 2;//每天的桃子总数是后一天剩余桃子加1乘2  
        rest = peach;  
    }  
    printf("猴子第一天一共摘了%d个桃子\n",peach);  
    system("pause");  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/fb4eddd108b848deba0d0c7a23e620c3.png)
2.while循环实现：


```
#include<stdio.h>  
#include <stdlib.h>  
int main()  
{  
    int peach = 0;//桃子总数  
    int rest = 1;//第10 天只剩一个桃子  
    int day = 9;  
    while(day--)  
    {  
        peach = (rest + 1) * 2;//每天的桃子总数是后一天剩余桃子加1乘2  
        rest = peach;  
    }  
    printf("猴子第一天一共摘了%d个桃子\n",peach);  
    system("pause");  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/7fb3717ad1ba56e4d5ddfd0319706e52.png)
递归实现：


```
#include<stdio.h>  
#include <stdlib.h>  
  
int total_peach(int rest)  
{  
    return (rest+1)*2;  
}  
int main()  
{  
    int peach = 0;//桃子总数  
    int rest = 1;//第10 天只剩一个桃子  
    int day = 9;  
    while(day--)  
    {  
        peach = total_peach(rest);//递归实现  
        rest = peach;  
    }  
    printf("猴子第一天一共摘了%d个桃子\n",peach);  
    system("pause");  
    return 0;  
}
```
运行结果：
![](http://jbcdn2.b0.upaiyun.com/2016/12/0c76083b2aa1ca5748c1343c7e64e188.png)
