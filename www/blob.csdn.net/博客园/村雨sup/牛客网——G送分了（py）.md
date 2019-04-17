# 牛客网——G送分了（py） - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/74/G](https://www.nowcoder.net/acm/contest/74/G)
来源：牛客网



## 题目描述



杭州人称傻乎乎的人为62，而嘟嘟家这里没有这样的习俗。

相比62，他那里的人更加讨厌数字38，当然啦，还有4这个

数字！所以啊，嘟嘟不点都不想见到包含38或者4的数字。

每次给出一个区间[n，m],你能找到所有令人讨厌的数字吗？


## 输入描述:
多组输入输出；
输入的都是整数对n、m（0<n≤m<1000000），
如果遇到都是0的整数对，则输入结束。
## 输出描述:
对于每次的输入
输出全部令人讨厌的数的个数

示例1



## 输入

1 100
0 0



## 输出

20
```
lst = [0]
n = 0
for i in range(1,1000001):
    if '4' in str(i) or '38' in str(i):
        n = n + 1
    lst.append(n)

while True:
    try:
        a,b = input().split()
        if a == '0' and b == '0'://此处注意是'0'不是0
            break
        if '4' in str(a) or '38' in str(a):
            print(lst[int(b)] - lst[int(a)] + 1)
        else:
            print(lst[int(b)] - lst[int(a)])

    except:
        break
```

py 大法好，不用数位dp，暴力打表秒过呜呜呜。
















