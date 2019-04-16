# HDU1407 测试你是否和LTC水平一样高 暴力、二分、hash - 紫芝的博客 - CSDN博客





2018年06月05日 20:03:12[紫芝](https://me.csdn.net/qq_40507857)阅读数：59








# 测试你是否和LTC水平一样高
**Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 20831    Accepted Submission(s): 7015**


Problem Description

大家提到LTC都佩服的不行，不过，如果竞赛只有这一个题目，我敢保证你和他绝对在一个水平线上！
你的任务是：
计算方程x^2+y^2+z^2= num的一个正整数解。





Input

输入数据包含多个测试实例，每个实例占一行，仅仅包含一个小于等于10000的正整数num。





Output

对于每组测试数据，请按照x,y,z递增的顺序输出它的一个最小正整数解，每个实例的输出占一行，题目保证所有测试数据都有解。





Sample Input


3





Sample Output


1 1 1





Author

lcy




Source

[杭电ACM集训队训练赛（IV）](http://acm.hdu.edu.cn/search.php?field=problem&key=%BA%BC%B5%E7ACM%BC%AF%D1%B5%B6%D3%D1%B5%C1%B7%C8%FC%A3%A8IV%A3%A9&source=1&searchmode=source)




Recommend

Ignatius.L   |   We have carefully selected several similar problems for you:  [1406](http://acm.hdu.edu.cn/showproblem.php?pid=1406)[1408](http://acm.hdu.edu.cn/showproblem.php?pid=1408)[1412](http://acm.hdu.edu.cn/showproblem.php?pid=1412)[1425](http://acm.hdu.edu.cn/showproblem.php?pid=1425)[1877](http://acm.hdu.edu.cn/showproblem.php?pid=1877)




方法一：

暴力枚举


注意结束循环，否则超时

202MS


```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int num;
    while(scanf("%d",&num)!=EOF){
        for(int x=1;x<100;x++){
            for(int y=x;y<100;y++){
                for(int z=y;z<100;z++){
                if(x*x+y*y+z*z==num){
                    printf("%d %d %d\n",x,y,z);
                    x=200;//结束循环
                }
                }//z
            }//y
        }//x
    }//while
    return 0;
}
```

用goto从内重循环直接跳出，简洁而优雅。


249MS

```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int num;
    while(scanf("%d",&num)!=EOF){
        for(int x=1;x<100;x++){
            for(int y=x;y<100;y++){
                for(int z=y;z<100;z++){
                if(x*x+y*y+z*z==num){
                    printf("%d %d %d\n",x,y,z);
                    //x=200;//结束循环
                    goto here;
                }//if
                }//z
            }//y
        }//x
        here:;
    }//while
    return 0;
}
```

方法二：

枚举x和y，对z进行二分查找。

62MS



```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
    int num;
    while(scanf("%d",&num)!=EOF){
        for(int x=1;x<100;x++){
            for(int y=x;y<100;y++){
                int left=y,right=100;
            while(left<right)
            {
                int mid=(left+right)/2;
                if(x*x+y*y+mid*mid<num)
                    left=mid+1;
                else if(x*x+y*y+mid*mid>num)
                    right=mid;
                else{
                    printf("%d %d %d\n",x,y,mid);
                    //x=200;//结束循环
                    goto here;
                }
            }
            }//y
        }//x
        here:;
    }//while
    return 0;
}
```

方法三：


枚举x和y，对z用hash判断是否存在

 15MS

```cpp
#include<cstdio>
#include<cmath>
const int MAXN=10001;
int res[101];
struct HASH
{
    bool exist;
    int index;
}hash[MAXN];
int main()
{
    int n;
    for(int i=1;i<=100;i++)
    {
        res[i]=i*i;
        hash[ res[i] ].exist=true;
        hash[ res[i] ].index=i;
    }

    while(~scanf("%d",&n))
    {    
        for(int x=1;x<=100;x++)
        {
            for(int y=1;res[x]+res[y]<n;y++)
            {
                int id=n-res[x]-res[y];
                if(hash[id].exist)    
                {    
                    printf("%d %d %d\n",x,y,hash[id].index);
                goto end;

                }
            }
        }

end:;
    }
    return 0;
}
```


方法二和方法三都是枚举x和y，但是二分查找为O(log(n))而hash为O(1）











