# 学习笔记--cache映像 - 左直拳的马桶_日用桶 - CSDN博客
2019年01月12日 06:46:09[左直拳](https://me.csdn.net/leftfist)阅读数：234
个人分类：[计算机基础																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/2622721)
在计算机的主存（内存）和CPU之间，还有cache，常用数据先从内存复制到cache，以提高性能。
这样就有一个如何复制的问题，也就是数据从内存复制到cache后，存放在哪里的问题。二者有一个地址对应关系。
有3种对应模式：
> 
1、直接相联
2、全相联
3、组相联
不管哪种对应模式，为了适应将数据复制到cache，内存区大小 == cache大小；内存的块（也称为页）大小，会和cache保持一致。
```
内存区大小 == cache大小
内存块大小 == cache块大小
```
**1、直接相联**
`内存块号==cache块号`。冲突机会最大。
cache标记位记录内存区号。块号不必记录，因为`内存块号==cache块号`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111230449201.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**2、全相联**
内存块可以复制到cache任一块。冲突机会最小。
cache标记记录内存页号。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111230215436.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**3、组相联**
内存和cache各自分组，组之间直接相联，组号保持一致；而组内的块则全相联，自由搭配。冲突机会折中。
教程里说，组相联情况下，`内存组内的块数 == cache组数`，好像没道理，是不是说错了？
cache标记记录内存区号 + 块号。内存组号不必记录，因为`内存组号 == cache组号`。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190111231449883.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**4、习题一**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190112061040255.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
答：
1)
所谓Cache的地址变换表，就是Cache的标记表。已知组相联方式下，`cache标记 = 内存区号 + 块号`
Cache大小 = 2 * 4 * 512 = 2 ^12，2的12次方
则有：
内存区数量 = (2^20） / (2 ^12) = 2 ^ 8
内存每组块数 == Cache每组块数 = 4 = 2^2
所以Cache每行标记为 8 + 2 = 10bit
由于Cache共有2 * 4 = 8块，因此标记表容量为  8 * 10bit
2)组相联的特点是组间直接相联，不需要比较；但组内块全相联，需要一一比较，因为每组有4块，所以每次都要比较4次，参与比较的存储单元有 4 个。所谓存储单元，就是Cache的一块。
