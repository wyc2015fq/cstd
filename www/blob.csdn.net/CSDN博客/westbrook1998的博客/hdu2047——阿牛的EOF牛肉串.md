# hdu2047——阿牛的EOF牛肉串 - westbrook1998的博客 - CSDN博客





2018年05月21日 15:16:00[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：69








> 
今年的ACM暑期集训队一共有18人，分为6支队伍。其中有一个叫做EOF的队伍，由04级的阿牛、XC以及05级的COY组成。在共同的集训生活中，大家建立了深厚的友谊，阿牛准备做点什么来纪念这段激情燃烧的岁月，想了一想，阿牛从家里拿来了一块上等的牛肉干，准备在上面刻下一个长度为n的只由”E” “O” “F”三种字符组成的字符串（可以只有其中一种或两种字符，但绝对不能有其他字符）,阿牛同时禁止在串中出现O相邻的情况，他认为，”OO”看起来就像发怒的眼睛，效果不好。  

  你，NEW ACMer,EOF的崇拜者，能帮阿牛算一下一共有多少种满足要求的不同的字符串吗？  

  PS: 阿牛还有一个小秘密，就是准备把这个刻有 EOF的牛肉干，作为神秘礼物献给杭电五十周年校庆，可以想象，当校长接过这块牛肉干的时候该有多高兴！这里，请允许我代表杭电的ACMer向阿牛表示感谢！  

  再次感谢！ 

  Input 

  输入数据包含多个测试实例,每个测试实例占一行,由一个整数n组成，`(0<n<40)`。  

  Output 

  对于每个测试实例，请输出全部的满足要求的涂法，每个实例的输出占一行。  

  Sample Input 

  1 

  2 

  Sample Output 

  3 

  8
因为O不能连续所以分为两种情况，一、最后一个是O，那么倒数第二个就只能是E或F，就是2*1种可能，再考虑前面n-2个有f(n-2)种可能，所以f(n)=2*f(n-2)   二、最后一个不是O，那么就只能是E或F，再考虑前面n-1个有f(n-1)种可能，所以f(n)=2*f(n-1) 综合起来就是`f(n)=2*(f(n-1)+f(n-2))`

代码：

```
##includeinclude  <cstdio><cstdio>
 ##includeinclude  <algorithm><algorithm>
#include <cstring>
using namespace std;
const int MAXN=50;
long long q[MAXN];
int main(void){
    q[1]=3;
    q[2]=8;
    for(int i=3;i<=40;i++){
        q[i]=2*(q[i-1]+q[i-2]);
    }
    int n;
    while(~scanf("%d",&n)){
        printf("%lld\n",q[n]);
    }
    return 0;
}
```





