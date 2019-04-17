# 牛客网——E进阶吧阶乘 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/75/E](https://www.nowcoder.net/acm/contest/75/E)
来源：牛客网




时间限制：C/C++ 3秒，其他语言6秒

空间限制：C/C++ 32768K，其他语言65536K

 64bit IO Format: %lld



## 题目描述


给定一个整数N（0≤N≤10000），求取N的阶乘

## 输入描述:
多个测试数据，每个测试数据输入一个数N
## 输出描述:
每组用一行输出N的阶乘

示例1



## 输入

1
2
3



## 输出

1
2
6
```
while True:
    try:
        n = int(input())
        ans = 1
        for i in range(1,n+1):
            ans = ans * i
        print(ans)
    except:
        break
```

好气啊，用py，比赛时没想到。
















