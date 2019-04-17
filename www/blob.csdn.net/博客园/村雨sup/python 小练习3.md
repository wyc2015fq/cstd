# python 小练习3 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




求大蜜
题：给你两个正整数a(0 < a < 100000)和n(0 <= n <=100000000000)，计算(a^n) % 20132013并输出结果
![](https://images2015.cnblogs.com/blog/1196023/201707/1196023-20170709141132384-765709288.png)

ret = 1

def PowerMod(a, n, ret):
    if n == 0:
        return ret
    if n % 2:
        ret = ret * a % 20132013
    return PowerMod(a*a%20132013, n/2, ret)
print PowerMod(a, n, ret)

#递归，比较难想到。。。

 AC2

print(pow(a,n,20132013))
pow(x, y[, z])
函数是计算x的y次方，如果z在存在，则再对结果进行取模，其结果等效于pow(x,y) %z

pow() 通过内置的方法直接调用，内置方法会把参数作为整型，而 math 模块则会把参数转换为 float。











