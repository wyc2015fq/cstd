# python 小练习2 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




给你一个整数列表L,判断L中是否存在相同的数字，
若存在，输出YES，否则输出NO。
解1
l=[]
for i in L:
    if L.count(i) != 1:
        print('YES')
        break
    else:
        l.append(i)   %用一个新的列表存储“正确”的数字，然后和原列表比较，或者用len判断有没有”错误“的数字
if l ==L:
    print('NO')


解2
**print ****'YES' ****if **sum([L.count(x) **for **x **in **L]) > len(L) **else ****'NO'  %count会重复取重复的数字所以大于len(L)**











