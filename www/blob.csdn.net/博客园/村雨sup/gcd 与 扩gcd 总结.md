# gcd 与 扩gcd 总结 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





gcd 定理的证明：

![](https://images2017.cnblogs.com/blog/1196023/201802/1196023-20180203213810609-31977360.png)

模板：

```
ll gcd(ll a,ll b)
{
    if(b == 0) return a;
    else return gcd(b,a%b);
}
```



 扩gcd证明：

![](https://images2017.cnblogs.com/blog/1196023/201802/1196023-20180203213920625-1871046257.png)

模板：

```
ll extgcd(ll a,ll b,ll &x,ll &y)
{
    ll d = a;
    if(b == 0)
    {
        x = 1;
        y = 0;
    } 
    else
    {
        d = extgcd(b,a%b,y,x);
        y -= (a/b)*x;
    }
    return d;
}
```



解题规律：

首先化为 ax+by = c 的形式，一般采用增加常量的方式，然后把a,b,x,y 代入extgcd 模板，返回的是d = gcd(a,b)

得到的x 是 ax+by = gcd(a,b)=d 的一个解，这时候两边同时乘以c/d就能得到解x’ = x * c/d











