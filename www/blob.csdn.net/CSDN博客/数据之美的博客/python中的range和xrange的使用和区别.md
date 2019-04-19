# python中的range和xrange的使用和区别 - 数据之美的博客 - CSDN博客
2017年05月06日 20:25:19[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：211
个人分类：[python](https://blog.csdn.net/oppo62258801/article/category/6738404)
**1.range（）函数的几个用法**：
![](http://images.cnitblog.com/blog/408927/201301/05113441-b7d3c0ac37af4ecb8cb5a261908e73ef.png)
函数原型：range（start， end， scan):
参数含义：start:计数从start开始。默认是从0开始。例如range（5）等价于range（0， 5）;
              end:技术到end结束，但不包括end.例如：range（0， 5） 是[0, 1, 2, 3, 4]没有5
              scan：每次跳跃的间距，默认为1。例如：range（0， 5） 等价于 range(0, 5, 1)
**2.xrange**
    函数说明：用法与range完全相同，所不同的是生成的不是一个数组，而是一个生成器。
xrange示例: 
``
- >>> xrange(5)
- xrange(5)
- >>> list(xrange(5))
- [0,1,2,3,4]
- >>> xrange(1,5)
- xrange(1,5)
- >>> list(xrange(1,5))
- [1,2,3,4]
- >>> xrange(0,6,2)
- xrange(0,6,2)
- >>> list(xrange(0,6,2))
- [0,2,4]
    由上面的示例可以知道：要生成很大的数字序列的时候，用xrange会比range性能优很多，因为不需要一上来就开辟一块很大的内存空间，这两个基本上都是在循环的时候用：
``- for i in range(0,100):
- print i 
- for i in xrange(0,100):
- print i 
    这两个输出的结果都是一样的，实际上有很多不同，range会直接生成一个list对象：
``- a = range(0,100)
- print type(a)
- print a 
- print a[0], a[1]
    输出结果：
``- <type'list'>
- [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99]
- 0 1
    而xrange则不会直接生成一个list，而是每次调用返回其中的一个值：
``- a = xrange(0,100)
- print type(a)
- print a 
- print a[0], a[1]
    输出结果：
``- <type'xrange'>
- xrange(100)
- 0 1
