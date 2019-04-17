# hdu1420——Prepared for New Acmer - westbrook1998的博客 - CSDN博客





2018年07月06日 14:12:05[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：58








> 
集训进行了将近2个礼拜，这段时间以恢复性训练为主，我一直在密切关注大家的训练情况，目前为止，对大家的表现相当满意，首先是绝大部分队员的训练积极性很高，其次，都很遵守集训纪律，最后，老队员也起到了很好的带头作用，这里特别感谢为这次DP专题练习赛提供题目和测试数据的集训队队长xhd同学.  

  特别高兴的是，跟随集训队训练的一批新队员表现非常好，进步也比较显著，特别是训练态度大大超出我的预期，我敢说，如果各位能如此坚持下去，绝对前途无量！  

  考虑到新队员还没有经过系统训练，我这里特别添加一道简单题：  

  给定三个正整数A，B和C（A,B,C<=1000000），求A^B mod C的结果.  

  希望各位都能体会到比赛中AC的快乐，绝对的量身定制，很高的待遇哟，呵呵…  

  Input 

  输入数据首先包含一个正整数N,表示测试实例的个数，然后是N行数据，每行包括三个正整数A,B,C。  

  Output 

  对每个测试实例请输出计算后的结果，每个实例的输出占一行。  

  Sample Input 

  3 

  2 3 4 

  3 3 5 

  4 4 6 

  Sample Output 

  0 

  2 

  4
快速幂取模

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long pow_mod(long long a,long long b,long long m){
    long long res=1;
    a%=m;
    while(b){
        if(b%2){
            res=(res*a)%m;
        }
        b/=2;
        a=(a*a)%m;
    }
    return res;
}
int main(void){
    int t;
    long long a,b,m;
    scanf("%d",&t);
    while(t--){
        scanf("%lld%lld%lld",&a,&b,&m);
        printf("%lld\n",pow_mod(a,b,m));
    }
    return 0;
}
```





