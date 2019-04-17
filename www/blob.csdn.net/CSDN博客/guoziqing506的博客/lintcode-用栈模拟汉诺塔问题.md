# lintcode - 用栈模拟汉诺塔问题 - guoziqing506的博客 - CSDN博客





2016年09月09日 11:21:20[guoziqing506](https://me.csdn.net/guoziqing506)阅读数：1377
所属专栏：[LintCode-Python题解](https://blog.csdn.net/column/details/guoziqing-blog.html)









题目描述：在经典的汉诺塔问题中，有 3 个塔和 N 个可用来堆砌成塔的不同大小的盘子。要求盘子必须按照从小到大的顺序从上往下堆 （如，任意一个盘子，其必须堆在比它大的盘子上面）。同时，你必须满足以下限制条件：



(1) 每次只能移动一个盘子。
(2) 每个盘子从堆的顶部被移动后，只能置放于下一个堆中。

(3) 每个盘子只能放在比它大的盘子上面。




请写一段程序，实现将第一个堆的盘子移动到最后一个堆中。




汉诺塔问题本身题目中已经讲得非常清楚了，我主要说一下解题的思路：递归。

汉诺塔问题是非常经典的递归问题，可以这样来思考，若是现在只有一个盘子，那么把它从一个塔移动到另一个塔很容易，直接移动就好。若是有两个盘子（一大一小），则先将小盘子放到辅助的塔中，再将大盘子放到目的地，最后将小盘子移动到目的地。。。依次类推，其实可以得到这样一种递归关系：

1. 先将放置在上面的n - 1个盘子放到辅助的塔里面（记为buffer），这个过程以目的地为辅助；

2. 再将最底下的盘子（也就是最大的）放到目的地；

3. 最后将buffer中的盘子移动到目的地，这个过程以原先的塔为辅助




最后根据lintcode中已有的代码框架，将代码补充完整：



```python
class Tower(object):
    # create three towers (i from 0 to 2)
    def __init__(self, i):
        self.disks = []
    
    # Add a disk into this tower
    def add(self, d):
        if len(self.disks) > 0 and self.disks[-1] <= d:
            print("Error placing disk %s" % d)
        else:
            self.disks.append(d)
    
    # @param {Tower} t a tower
    # Move the top disk of this tower to the top of t.
    def move_top_to(self, t):
        if len(self.disks) > 0:
            t.disks.append(self.disks.pop(-1))
        # Write your code here
    
    # @param {int} n an integer
    # @param {Tower} destination a tower
    # @param {Tower} buffer a tower
    # Move n Disks from this tower to destination by buffer tower
    def move_disks(self, n, destination, buffer):
        if n == 0:
            return
        if n == 1:
            self.move_top_to(destination)
        else:
            self.move_disks(n - 1, buffer, destination)
            self.move_top_to(destination)
            buffer.move_disks(n - 1, destination, self)
        # Write your code here

    def get_disks(self):
        return self.disks

"""
Your Tower object will be instantiated and called as such:
towers = [Tower(0), Tower(1), Tower(2)]
for i in xrange(n - 1, -1, -1): towers[0].add(i)
towers[0].move_disks(n, towers[2], towers[1])
print towers[0], towers[1], towers[2]
"""
```







