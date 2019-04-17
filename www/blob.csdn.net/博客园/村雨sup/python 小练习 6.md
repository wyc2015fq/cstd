# python 小练习 6 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





几种不同的方法写fibonacci

刚学Python不久的的C程序员：



```
01 def fib(n):#{

02     if n<=2 :

03         return 1;

04     else:

05         return fib(n-1)+fib(n-2);

06 #}
```

效率比较低



 懒散的Python程序员：



```
01 def fib(n):

02     return 1 and n<=2 or fib(n-1)+fib(n-2）
```

更懒的Python程序员：

```
01 fib=lambda n:1 if n<=2 else fib(n-1)+fib(n-2)
```

```
01 def fib(n):

02     x,y=0,1

03     while(n):

04         x,y,n=y,x+y,n-1

05     return x
```

这个比较好

刚学完数据结构的Python程序员：















