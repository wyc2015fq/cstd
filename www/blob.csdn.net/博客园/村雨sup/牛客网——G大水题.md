# 牛客网——G大水题 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





链接：[https://www.nowcoder.net/acm/contest/75/G](https://www.nowcoder.net/acm/contest/75/G)
来源：牛客网




时间限制：C/C++ 1秒，其他语言2秒

空间限制：C/C++ 32768K，其他语言65536K

 64bit IO Format: %lld



## 题目描述


给出一个数n，求1到n中，有多少个数不是2 5 11 13的倍数。

## 输入描述:
本题有多组输入
每行一个数n，1<=n<=10^18.
## 输出描述:
每行输出输出不是2 5 11 13的倍数的数共有多少。

示例1



## 输入

15



## 输出

4



## 说明

1 3 7 9
```
try:
    while True:
        n = input()
        n = int(n)

        ans = n // 2 + n // 5 + n // 11 + n // 13
        ans -= n // 10 + n // 22 + n // 26 + n // 55 + n // 65 + n // 143
        ans += n // 110 + n // 130 + n // 286 + n // 715
        ans -= n // 1430

        ans = n - ans

        print(ans)

except EOFError:
    pass
```

容斥，一开始被1e18吓到了，早该想到用python的。
















