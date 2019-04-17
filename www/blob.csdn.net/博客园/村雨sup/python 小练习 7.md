# python 小练习 7 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




有一楼梯共n级，刚开始时你在第一级，若每次只能跨上一级或二级，要走上第n级，共有多少种走法？ 
这其实是fibonacci数列，记走法为f(n),在n-1和n-2时你都可以直接跨上去。
因此
         f(n) = f(n-1)+f(n-2)
跑到n级的方法等于跑到n-1和n-2之和因此可以用fib做

```
def fib(n):
    x,y = 0,1
    while(n):
        x,y,n = y,x+y,n-1
    return x
```

















