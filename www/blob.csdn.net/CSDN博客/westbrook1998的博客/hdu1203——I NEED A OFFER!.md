# hdu1203——I NEED A OFFER! - westbrook1998的博客 - CSDN博客





2018年08月14日 18:59:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：21标签：[dp																[01背包](https://so.csdn.net/so/search/s.do?q=01背包&t=blog)](https://so.csdn.net/so/search/s.do?q=dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Speakless很早就想出国，现在他已经考完了所有需要的考试，准备了所有要准备的材料，于是，便需要去申请学校了。要申请国外的任何大学，你都要交纳一定的申请费用，这可是很惊人的。Speakless没有多少钱，总共只攒了n万美元。他将在m个学校中选择若干的（当然要在他的经济承受范围内）。每个学校都有不同的申请费用a（万美元），并且Speakless估计了他得到这个学校offer的可能性b。不同学校之间是否得到offer不会互相影响。“I NEED A OFFER”，他大叫一声。帮帮这个可怜的人吧，帮助他计算一下，他可以收到至少一份offer的最大概率。（如果Speakless选择了多个学校，得到任意一个学校的offer都可以）。  

  Input 

  输入有若干组数据，每组数据的第一行有两个正整数n,m(0<=n<=10000,0<=m<=10000)  

  后面的m行，每行都有两个数据ai(整型),bi(实型)分别表示第i个学校的申请费用和可能拿到offer的概率。  

  输入的最后有两个0。  

  Output 

  每组数据都对应一个输出，表示Speakless可能得到至少一份offer的最大概率。用百分数表示，精确到小数点后一位。  

  Sample Input 

  10 3 

  4 0.1 

  4 0.2 

  5 0.3 

  0 0 

  Sample Output 

  44.0% 

  Hint 

  You should use printf(“%%”) to print a ‘%’.
复习一下背包问题 

这里因为如果用获得offer的几率 选或不选无法递推，比如选第一个获得的概率是0.1 第二个获得的概率是0.2 两个都选至少获得一个并不是0.1*0.2 但是如果反过来看，选第一个没得到offer的概率是0.9，第二个是0.8，选两个都没得到的概率就可以0.9*0.8 

然后还要注意用一维数组，背包容量需要从n—>c[i] 也就是反向
代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=10050;
int n,m;
int c[N];
double v[N];
double dp[N];
int main(void){
    while(~scanf("%d%d",&n,&m) && (n || m)){
        _clr(c,0);
        _clr(v,0.0);
        for(int i=0;i<=n;i++){
            dp[i]=1.0;
        }
        for(int i=0;i<m;i++){
            scanf("%d%lf",&c[i],&v[i]);
        }
        //一维数组的背包一定要从后面遍历
        for(int i=0;i<m;i++){
            for(int j=n;j>=c[i];j--){
                dp[j]=min(dp[j],dp[j-c[i]]*(1-v[i]));
            }
        }
        printf("%.1lf%%\n",(1-dp[n])*100);
    }
    return 0;
}
```







