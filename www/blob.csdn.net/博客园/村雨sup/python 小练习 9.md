# python 小练习 9 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




还记得中学时候学过的杨辉三角吗？具体的定义这里不再描述，你可以参考以下的图形：
1
1 1
1 2 1
1 3 3 1
1 4 6 4 1
1 5 10 10 5 1
..............
先在给你一个正整数n，请你输出杨辉三角的前n层
注意：层数从1开始计数,每层数字之间用一个空格隔开，行尾不要有空格。
如n=2,则输出：
1
1 1

AC代码如下：
```
a = [[0 for i in range(n)]for j in range(n)]

a[0][0] = 1
a[1][0] = 1
a[1][1] = 1
for i in range(n):
    a[i][0] = a[i][i] = 1
    for j in range(1,i//2+1): #取膜除法
        a[i][j] = a[i][i-j] = a[i-1][j-1] +a[i-1][j]

for i in range(n):
    print(' '.join(str(x)for x in a[i] if x!=0) )#这是一种输出方式
```

```
for i in range(n):
    for j in range(i+1):
        print(a[i][j],end=' ')
    print('\n',end='')#这是第二种输出方式
```

两行代码的解法：

```
for i in xrange(n):
    print " ".join(map(str, reduce(lambda x, y: map(sum, zip([0]+x, x+[0])), xrange(i), [1])))
```













