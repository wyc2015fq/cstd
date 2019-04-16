# HDU 6362 oval-and-rectangle 定积分求期望（精度T_T） - 紫芝的博客 - CSDN博客





2018年08月30日 21:43:21[紫芝](https://me.csdn.net/qq_40507857)阅读数：96
个人分类：[计算几何](https://blog.csdn.net/qq_40507857/article/category/7631777)









# oval-and-rectangle

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 1234    Accepted Submission(s): 533****

**Problem Description**

Patrick Star find an oval.


The half of longer axes is on the x-axis with length a.


The half of shorter axes is on the y-axis with length b.


Patrick Star plan to choose a real number c randomly from [0,b], after that, Patrick Star will get a rectangle :


1. The four vertexes of it are on the outline of the oval.


2. The two sides of it parallel to coordinate axis.


3. One of its side is y=c.


Patrick Star want to know the expectations of the rectangle's perimeter.

**Input**

The first line contain a integer T (no morn than 10), the following is T test case, for each test case :


Each line contains contains two integer a, b (0<b<a<105). Separated by an white space.

**Output**

For each test case output one line denotes the expectations of the rectangle's perimeter .

You should keep exactly 6 decimal digits and ignore the remain decimal digits. 


It is guaranted that the 7-th decimal digit of answer wont be 0 or 9.

**Sample Input**

1 

2   1

**Sample Output**

8.283185

**Source**

[2018 Multi-University Training Contest 6](http://acm.hdu.edu.cn/search.php?field=problem&key=2018+Multi-University+Training+Contest+6&source=1&searchmode=source)

**Recommend**

chendu   |   We have carefully selected several similar problems for you:  [6447](http://acm.hdu.edu.cn/showproblem.php?pid=6447)[6446](http://acm.hdu.edu.cn/showproblem.php?pid=6446)[6445](http://acm.hdu.edu.cn/showproblem.php?pid=6445)[6444](http://acm.hdu.edu.cn/showproblem.php?pid=6444)[6443](http://acm.hdu.edu.cn/showproblem.php?pid=6443)

### 题意：

求椭圆内接矩形周长的期望

![è¿éåå¾çæè¿°](https://img-blog.csdn.net/20180808204628506?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvZGVzd2Fycmlvcg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

分析：

其实就是求所有矩形周长的平均值，把所有可能周长加起来，除以变化范围即可。

首先，求出周长公式，椭圆的公式为

![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D154/sign=1ca95d23b20e7bec27da07e41b2fb9fa/d058ccbf6c81800a3c70c022bb3533fa828b4700.jpg)

这样我们知道y的变化范围是[0,b][0,-b]，并且我们可以求出

![](https://img-blog.csdn.net/20180809095252963?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l1MTIxMzgw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此可以得到周长公式

![](https://img-blog.csdn.net/20180809095305417?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l1MTIxMzgw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因为我们要求所有可能周长之和，因此实际上就是求周长的黎曼和，即求周长公式的积分

因此我们实际上要求

![](https://img-blog.csdn.net/2018080909532769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l1MTIxMzgw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们当时做的时候就求到这里的，然后直接用辛普森求积分的近似公式直接做了，样例过提交死活不对，我们知道肯定是精度问题，然后又手动求导，得到式子，还是不行，就死活用三角函数代换，哎没办法基础知识不牢固，高数都忘了，反正还是太菜了

![](https://img-blog.csdn.net/20180809095350857?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l1MTIxMzgw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

因此周长和为![](https://img-blog.csdn.net/20180809095412540?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3l1MTIxMzgw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)因为变化范围是0-b ，所以再除以b便是期望

所以最终只需求一个式子 ：2b+![\pi](https://private.codecogs.com/gif.latex?%5Cpi)a

因为题目要求保留6位小数，并且后面的全部舍去，因此为了防止%.6f造成四舍五入，因此给答案-0.0000005

```cpp
#include<bits/stdc++.h>
using namespace std;
const double pi=acos(-1);
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        double a,b;
        scanf("%lf%lf",&a,&b);
        double ans=2*b+pi*a;
        printf("%.6f\n",ans-0.0000005);
    }
    return 0;
}
```

还有一种做法是先给ans乘以1000000，然后将ans转化为整型，去掉小数点6位以后的数字，最后再乘以1000000.

```cpp
#include<bits/stdc++.h>
using namespace std;
const double pi=acos(-1);
int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        double a,b;
        scanf("%lf%lf",&a,&b);
        double ans=2*b+pi*a;
        ans*=1000000;
        ans=(long long)ans;
        printf("%.6f\n",ans/1000000);
    }
    return 0;
}
```





