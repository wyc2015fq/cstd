# python 小练习4 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




给你一个整数list L, 如 L=[2,-3,3,50], 求L的一个连续子序列，使其和最大，输出最大子序列的和。
例如，对于L=[2,-3,3,50]， 输出53（分析：很明显，该列表最大连续子序列为[3,50]).

我个人想法比较简单啦，只是按顺序遍历列表
AC1l=[]
maxL = -1000
L = [2,-3,3,50]
for i in range(len(L)):
    toto = 0
for j in range(i,len(L),1):
        toto = toto+L[j]
        if maxL<toto:
            maxL = toto
    l.append(maxL)
print(max(l))

AC2
网上抄了一段别人的代码，其思想是把各个点孤立起来，如果前面数之和为0就把前面数全舍去。
now = 0
sum = 0
for x in L:
now +=x
if now>sum:
      sum = now
if now<0:
now = 0
print(sum)










