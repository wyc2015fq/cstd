
# Java 数组存储机制 - lin_tuer的博客 - CSDN博客


2018年10月08日 14:32:12[lin_tuer](https://me.csdn.net/lin_tuer)阅读数：268


[参考资料](https://www.cnblogs.com/chenyaqiang/p/5419493.html)
数组是一种引用类型。
数组用来存储类型相同的一组数据，一旦初始化完成，其所占的空间也确定下来了，即使清除某个元素，其所占用的空间仍然存在，即，数组的长度不能被改变，且数组只有在分配空间后才能使用。
数组的初始化方式有两种，a, 动态初始化，只指定长度，后面再进行初始值的指定 int[] dynamic_initial = new int[len];  b. 静态初始化，不指定长度，直接给出初始值，数组长度就是初始值的个数 int[] static_iniyial = new int[] {1,2,3};
数组的引用变量存放在栈中，其元素则存放在堆内存中。
![数组内存示意图](https://img-blog.csdn.net/20181008142928141?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpbl90dWVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
另外，以 int 型数组为例，也可以发现，基本数据类型在堆中也会出现。
int[] array=new int[]{1,2};
由于new了一个对象，所以new int[]{1,2}这个对象时存储在堆中的，也就是说1,2这两个基本数据类型是存储在堆中

