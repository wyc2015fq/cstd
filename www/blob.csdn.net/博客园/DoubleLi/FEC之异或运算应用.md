# FEC之异或运算应用 - DoubleLi - 博客园






话说为啥FEC需要异或( ^/⊕ )操作呢?

**异或**：xor

**异或运算规则：**

0 xor 0 = 0

0 xor 1 = 1

1 xor 0 = 1

1 xor 1 = 0

**异或运算特性：**

1). a xor a = 0

2). a xor 0 = a

3). (a xor b) xor c = a xor (b xor c)
**4). IF a xor b = c THEN a xor c = b   **


异或的运算特性有两点很好的应用

**查找**

case 1：

一个数组，除了其中一个元素，其他元素都为空，如何找到这个元素？

对数组所有元素做异或的结果即为这个元素

利用了异或运算特性的1)、2)、3)


case 2:
给一个很大的数组，里面有两个数只出现过一次，其他数都出现过两次，把这两个数找出来?
这个case复杂一些，有兴趣的同学可以参考：[一个百度的面试题目](http://blog.csdn.net/pathuang68/article/details/7567027)


**校验&恢复**

校验&恢复主要利用的了异或的第四条特性：IF a xor b = c THEN a xor c = b 
一个很好的应用实例是**RAID5**
简单描述下：
使用3块磁盘（A、B、C）组成RAID5阵列，当用户写数据时，将数据分成两部分，分别写到磁盘A和磁盘B
A xor B的结果写到磁盘C
当读取A的数据时，通过B xor C可以对A的数据做校验
当A盘出错时，通过B xor C也可以恢复A盘的数据

RAID5的实现比上述的描述复杂多了，有兴趣的同学看下[RAID5](http://baike.baidu.com/view/645826.htm?fromId=969385)









