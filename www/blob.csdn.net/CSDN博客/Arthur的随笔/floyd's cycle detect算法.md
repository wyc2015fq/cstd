# floyd's cycle detect算法 - Arthur的随笔 - CSDN博客
2011年11月29日 23:15:14[largetalk](https://me.csdn.net/largetalk)阅读数：1537
今天在网上看到一个python脚本，用来求一串数字中重复得那个数字，具体看这里：[http://www.keithschwarz.com/interesting/code/?dir=find-duplicate](http://www.keithschwarz.com/interesting/code/?dir=find-duplicate)
他得程序是
```python
def findArrayDuplicate(array):
    assert len(array) > 0
    # The "tortoise and hare" step.  We start at the end of the array and try
    # to find an intersection point in the cycle.
    slow = len(array) - 1
    fast = len(array) - 1
    # Keep advancing 'slow' by one step and 'fast' by two steps until they
    # meet inside the loop.
    while True:
        slow = array[slow]
        fast = array[array[fast]]
#        print 'step: ', 's :',slow, ' f: ', fast
        if slow == fast:
#            print 'slow: ',slow
            break
    # Start up another pointer from the end of the array and march it forward
    # until it hits the pointer inside the array.
    finder = len(array) - 1
    while True:
        slow   = array[slow]
        finder = array[finder]
        # If the two hit, the intersection index is the duplicate element.
        if slow == finder:
            return slow
```
思路就是根据f(i)=A(i)这个函数，将问题抽象成一个闭环求交点得问题。不过我有点疑问，第一个while还好理解，第二while在我看来好像有死循环得可能。另外，只要是有限个数得数组，根据其抽象都会形成一个环，比如[0,1,3,2], ...f(3)=2, f(2)=3...., so, 这个程序对错误数据毫不容错。
ps。上面是我想错了，第二个while那么做是有理论依据的， 如下：
关于找到找到环的入口点的问题，当fast若与slow相遇时，slow肯定没有走遍历完链表，而fast已经在环内循环了n圈(1<=n)。假设slow走了s步，则fast走了2s步（fast步数还等于s 加上在环上多转的n圈），设环长为r，则：
2s = s + nr
则: s = nr
设整个链表长L，入口环与相遇点距离为x，起点到环入口点的距离为a。
a + x = nr
a + x = (n-1) r + r = (n-1)r + L - a
a = (n-1)r + (L - a  - x) 
(L – a – x)为相遇点到环入口点的距离，由此可知，从链表头到环入口点等于(n-1)循环内环+相遇点到环入口点，于是我们从链表头、与相遇点分别设一个指针，每次各走一步，两个指针必定相遇，且相遇第一点为环入口点。
