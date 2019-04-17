# hdu1850——Being a Good Boy in Spring Festival - westbrook1998的博客 - CSDN博客





2018年08月23日 23:23:15[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：32标签：[Nim博弈																[博弈论](https://so.csdn.net/so/search/s.do?q=博弈论&t=blog)](https://so.csdn.net/so/search/s.do?q=Nim博弈&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
一年在外 父母时刻牵挂  

  春节回家 你能做几天好孩子吗  

  寒假里尝试做做下面的事情吧  

  陪妈妈逛一次菜场  

  悄悄给爸爸买个小礼物  

  主动地 强烈地 要求洗一次碗  

  某一天早起 给爸妈用心地做回早餐  

  如果愿意 你还可以和爸妈说  

  咱们玩个小游戏吧 ACM课上学的呢～  

  下面是一个二人小游戏：桌子上有M堆扑克牌；每堆牌的数量分别为Ni`(i=1…M)`；两人轮流进行；每走一步可以任意选择一堆并取走其中的任意张牌；桌子上的扑克全部取光，则游戏结束；最后一次取牌的人为胜者。  

  现在我们不想研究到底先手为胜还是为负，我只想问大家：  

  ——“先手的人如果想赢，第一步有几种选择呢？”  

  Input 

  输入数据包含多个测试用例，每个测试用例占2行，首先一行包含一个整数M`(1<M<=100)`，表示扑克牌的堆数，紧接着一行包含M个整数N`i(1<=Ni<=1000000，i=1…M)`，分别表示M堆扑克的数量。M为0则表示输入数据的结束。  

  Output 

  如果先手的人能赢，请输出他第一步可行的方案数，否则请输出0，每个实例的输出占一行。  

  Sample Input 

  3 

  5 7 9 

  0 

  Sample Output 

  1
Nim博弈 说实话没咋看懂 异或来异或去的 

k是要从a[i]中取出后剩下的 

这样k=ans^a[i]=a[1]^a[2]^…^a[i-1]^a[i+1]^… 

/所以k^a[1]^a[2]^…^a[i-1]^a[i+1]^…=k^k=0 奇异局势
代码：

```
#include <cstdio>
 #include <algorithm>
 #include <cstring>
 using namespace std;
 int main(void){
     int n;
     int a[105];
     int ans;
     int cnt;
     while(~scanf("%d",&n) && n){
         ans=cnt=0;
         memset(a,0,sizeof(a));
         for(int i=0;i<n;i++){
             scanf("%d",&a[i]);
             ans^=a[i];
         }
         if(ans==0){
             printf("0\n");
         }
         else{
             for(int i=0;i<n;i++){
             //k是要从a[i]中取出后剩下的
                 //这样k=ans^a[i]=a[1]^a[2]^...^a[i-1]^a[i+1]^...
                 //所以k^a[1]^a[2]^...^a[i-1]^a[i+1]^...=k^k=0 奇异局势
                 int k=ans^a[i];
                 if(k<a[i]){
                     cnt++;
                 }
             }
             printf("%d\n",cnt);
         }
     }
     return 0;
 }
```







