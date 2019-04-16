# HDU 5984 Pocky 数学期望 - 紫芝的博客 - CSDN博客





2018年10月03日 16:35:52[紫芝](https://me.csdn.net/qq_40507857)阅读数：27
个人分类：[数学期望](https://blog.csdn.net/qq_40507857/article/category/8099550)









# Pocky

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/65536 K (Java/Others)Total Submission(s): 2004    Accepted Submission(s): 1065****

**Problem Description**

Let’s talking about something of eating a pocky. Here is a Decorer Pocky, with colorful decorative stripes in the coating, of length L.

While the length of remaining pocky is longer than d, we perform the following procedure. We break the pocky at any point on it in an equal possibility and this will divide the remaining pocky into two parts. Take the left part and eat it. When it is not longer than d, we do not repeat this procedure.

Now we want to know the expected number of times we should repeat the procedure above. Round it to 6 decimal places behind the decimal point.

**Input**

The first line of input contains an integer N which is the number of test cases. Each of the N lines contains two float-numbers L and d respectively with at most 5 decimal places behind the decimal point where 1 ≤ d, L ≤ 150.

**Output**

For each test case, output the expected number of times rounded to 6 decimal places behind the decimal point in a line.

**Sample Input**

```
6 
1.0 1.0 
2.0 1.0 
4.0 1.0 
8.0 1.0 
16.0 1.0 
7.00 3.00
```

**Sample Output**

```
0.000000 
1.693147 
2.386294 
3.079442 
3.772589 
1.847298
```

**Source**

[2016ACM/ICPC亚洲区青岛站-重现赛（感谢中国石油大学）](http://acm.hdu.edu.cn/search.php?field=problem&key=2016ACM%2FICPC%D1%C7%D6%DE%C7%F8%C7%E0%B5%BA%D5%BE-%D6%D8%CF%D6%C8%FC%A3%A8%B8%D0%D0%BB%D6%D0%B9%FA%CA%AF%D3%CD%B4%F3%D1%A7%A3%A9&source=1&searchmode=source)

# 题解

题意相对比较容易理解

对于一根长度为 `L` 的木棍,每次等概率的取一个点将其分成两半,然后吃掉左边一半,直到剩下的长度小于 `d`,计算需要吃(分割)次数的数学期望

显然,如果 `L/d` 相同,则结果必定相同

根据 `ln2 = 0.693147` 可以推测出结果应该是 `ln(L/d) + 1`

再单独考虑不需要分割的情况( `d>=L` )，结果是0次

## 证明

定义 `f(x)` 为长度为 `x` 时,的数学期望

如图,对于 `f(x)` 若 `x<=d` 这时已经满足条件,有 `f(x)=0`

而对于 `x>d` 其结果应该是从上面任选一点后求其右半部份的数学期望再加上本次分割的 `1`

用 φ 表示从长度为 `x` 的线段上取到一个点的概率,有    ![](https://img-blog.csdn.net/20181003162747206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也即   ![](https://img-blog.csdn.net/20181003162816352?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也即
![](https://img-blog.csdn.net/20181003162844207?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中,特别要注意的是

![](https://img-blog.csdn.net/20181003162915471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此可以得知 `f(x)` 不是一个连续函数

但是我们迭代运算的部分都是 `x > d` 部分

![](https://img-blog.csdn.net/20181003163001413?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018100316303310?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

[原文地址](http://blog.oyohyee.com/post/HDU/5984.html?tdsourcetag=s_pctim_aiomsg)

```cpp
#include<stdio.h>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#define LL long long
#define PI atan(1.0)*4
#define DD double
#define mod 100
#define dian 1.000000011
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
const int maxn=6e3;

int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        double x,d;
        scanf("%lf%lf",&x,&d);
        if(x-d<=eps)
            printf("0.000000\n");
        else
            printf("%.6lf\n",log(x/d)+1);
    }
    return 0;
}
```





