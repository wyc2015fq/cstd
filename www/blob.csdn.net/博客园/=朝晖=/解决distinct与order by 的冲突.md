# 解决distinct与order by 的冲突 - =朝晖= - 博客园
# [解决distinct与order by 的冲突](https://www.cnblogs.com/dhcn/p/7115211.html)
```
sql="select distinct id from test order by otherfield desc"
```
需要找到不同的id,同时又想让记录按fbsj排序。但是这样一定会出现错误,出现distinct与order by冲突问题。如何来得到完美的解决呢,看下面的方法,依然用一条sql语句
```
sql="select id from test group by id order by max(<span style="font-family: Arial, Helvetica, sans-serif;">otherfield</span>) desc"
```
用group by 代替disitnct实现找不同id的功能。
以上已经过[测试](http://lib.csdn.net/base/softwaretest),可以完美解决distinct与order by的冲突问题
原载: 大连网站制作 http://www.noonenet.cn/
----------------------
补充两句在Django中解决这个问题是，直接annotate max即可，annotate本身自动添加group by
这个问题的解决体现了对SQL集合理论的理解，第一种方式其逻辑在集合理论中出现了跳跃，所以无法完成。

