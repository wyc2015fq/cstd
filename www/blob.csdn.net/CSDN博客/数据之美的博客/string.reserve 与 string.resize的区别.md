# string.reserve 与 string.resize的区别 - 数据之美的博客 - CSDN博客
2017年07月24日 01:19:43[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2626
string中有两个函数：reserve()和resize()，最近细细的看了下这两个函数的使用，记录下来。
    reserve为容器预留足够的空间，避免不必要的重复分配，分配空间大于等于函数的参数，影响capacity。
    resize调整容器中有效数据区域的尺寸，如果尺寸变小，原来数据多余的截掉。若尺寸变大，不够的数据用该函数第二个参数填充,影响size。
(1)reserve代码如下：
**[cpp]**[view
 plain](http://blog.csdn.net/freecloud_insky/article/details/47058597#)[copy](http://blog.csdn.net/freecloud_insky/article/details/47058597#)
- string str3 = "smiada";  
- str3.reserve(20);  
为str3初始分配空间，15。
![](https://img-blog.csdn.net/20150725181311238)
调用reserve()函数后，空间变为31，即保留空间变大了(string内存分配按照:(n*16-1)分配)。
![](https://img-blog.csdn.net/20150725181345324)
(2)resize代码如下：
a、尺寸变小
**[cpp]**[view
 plain](http://blog.csdn.net/freecloud_insky/article/details/47058597#)[copy](http://blog.csdn.net/freecloud_insky/article/details/47058597#)
- string str3 = "smiada";  
- str3.resize(2);  
尺寸由6改变为2，多余字符被截掉了，结果如下：
![](https://img-blog.csdn.net/20150725181923591)
b、尺寸变大，可以给一个填充参数。尺寸由6变大到10，4个空位填充b(默认参数为0，即NULL或者'\0')。；
**[cpp]**[view
 plain](http://blog.csdn.net/freecloud_insky/article/details/47058597#)[copy](http://blog.csdn.net/freecloud_insky/article/details/47058597#)
- string str3 = "smiada";  
- str3.resize(10,'b');  
![](https://img-blog.csdn.net/20150725182346783)
哈哈，当有疑问时，敲两行代码最实用了。
