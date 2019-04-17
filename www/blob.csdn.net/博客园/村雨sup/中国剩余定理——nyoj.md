# 中国剩余定理——nyoj - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





## 中国剩余定理------解法如下：假设存在一个数M M%A=a , M%B=b , M%C=c并且A，B，C必须俩俩互质。满足这一条件下：存在一个R1使得 ， K1=A*B*R1 ,K1%C==1.存在一个R2使得 ， K2=C*B*R2,K2%A==1.存在一个R3使得 ， K3=C*A*R3,K3%B==1.则必定满足 M=(K1*c+K2*a+k3*b)%(A*B*C);但是此题有条件10<=M<=100;所以可以在此范围里找，有则输出，没有则输出"No answer";

1.因为这个数能被5和7整除而不能被3整除，所以肯定是5和7的倍数，也就是35k，但是我们需要保证被3除的结果是a，所以我们令k=2，这样，70k%3=1，而k=1时，35k%3=2，所以，能被5和7整除不能被3整除且余数为a的数为70a。


2.被3和7整除，不能被5整除，则为21k，k=1时，正好余1，所以这个数为21b。


3.被3和5整除，不能被7整除，则为15k，k=1时，正好余1，所以这个数为15c。


所以，这个数为70a+21b+15c，又因为3、5、7的最小公倍数为105，所以这个数肯定在0到105以内，所以对结果取余一下即可。


推广到其他情况也是同样道理。


比如，求除以5、7、11以后所得余数为a,b,c.则这个数是：231a+330b+210c，然后对5×7×11=385取余即可。

代码如下：

```
#include <bits/stdc++.h>
using namespace std;
typedef long long  ll;
const int INF = 0x3f3f3f3f;
const int maxn = 2000;

int main()
{
    int a,b,c;
    while(~scanf("%d%d%d",&a,&b,&c)){
        int ans = (70*a+21*b+15*c)%105;
        if(ans <= 100&&ans >= 10)
            printf("%d\n",ans);
        else
            printf("No answer\n");
    }

    return 0;
}
```

——











