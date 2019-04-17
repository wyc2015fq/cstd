# python 小练习 11 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





桌子上有一堆数量不超过20的果子，每个果子的重量都是不超过20的正整数，全部记录在列表 L 里面。小明和小红决定平分它们，但是由于他们都太自私，没有人愿意对方比自己分得的总重量更多。而果子又不能切开，所以最后他们商量好的平分方案是这样的：他们可以把某些果子扔掉，再将剩下的果子平分，请你求出在这种方案下他们每人最多可以分得的糖果重量。

例如，L = [1,2,3,4,5]，则输出：7

      L = [1,3,6]，则输出：0

说明：对于样例1，他们最好的方案是把重量为 1 的果子扔掉，一人分得总重量为 7 的果子；样例2无法平分果子，因此答案是0。

**注意：**数据已于*2017/05/03*加强，原来能通过的代码不一定能够再次通过。

没有AC：还是留有疑问

```
import copy
L = [4,1,2,3,5]
L.sort()
Total_weight = sum(L)

rsl = set()
rsl.add(0)

for i in range(len(L)):         #用集合的方法取出所有可能的和
    temp = copy.deepcopy(rsl)
    for value in temp:
        rsl.add(L[i]+value)
lst = list(rsl)

for i in range(1,len(lst)):     #关键是我没有办法得到具体是pop掉了哪些数
    weight = Total_weight - lst[i]
    if weight%2 == 0:
        t = int(weight/2)
        if t in lst:
            print(t)
            break
else:
    print(0)
```













