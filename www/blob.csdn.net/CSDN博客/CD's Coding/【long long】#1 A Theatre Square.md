# 【long long】#1 A.Theatre Square - CD's Coding - CSDN博客





2014年03月31日 20:29:21[糖果天王](https://me.csdn.net/okcd00)阅读数：587标签：[c++																[python																[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=python&t=blog)











Theatre Square in the capital city of Berland has a rectangular shape with the size *n* × *m* meters. On the occasion of the city's anniversary, a decision
 was taken to pave the Square with square granite flagstones. Each flagstone is of the size *a* × *a*.


What is the least number of flagstones needed to pave the Square? It's allowed to cover the surface larger than the Theatre Square, but the Square has to be covered. It's not allowed to break the flagstones. The sides of flagstones should be parallel to the
 sides of the Square.




Input


The input contains three positive integer numbers in the first line: *n*,  *m* and *a* (1 ≤  *n*, *m*, *a* ≤ 109).




Output


Write the needed number of flagstones.




Sample test(s)




input
6 6 4




output
4





```python
my_list = raw_input().split()  
  
n = int(my_list[0])  
m = int(my_list[1])  
a = int(my_list[2])  
  
print (n/a+(n%a>0))*(m/a+(m%a>0))
```


先十分激动的写一个Python2.7的代码~~~

当然正题是C++

这道题……我从左上开始向右向下排，刚超过为止不就行了么——





```cpp
#include<stdio.h>  
int main()  
{  
    long m, n, a;  
    long i = 1, j = 1;         
    scanf("%ld %ld %ld",&m, &n, &a);  
    while(i * a <  m)  
    {  
        i++;  
    }  
    while(j * a < n)  
    {  
        j++;  
    }  
    printf("%I64d\n", (long long)i * j);  
    return 0;  
}
```](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




