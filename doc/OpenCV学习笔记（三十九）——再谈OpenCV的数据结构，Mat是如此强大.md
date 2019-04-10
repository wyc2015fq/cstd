# OpenCV学习笔记（三十九）——再谈OpenCV的数据结构，Mat是如此强大

2011年12月27日 21:09:36

yang_xian521

阅读数：38084

更多

 								所属专栏： 																[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html) 																 							

 									

还是一个实时性要求的程序最近把我弄得有些上火。为了提高代码运行的速度，我也是又新啃东西学了。由于我代码里使用了vector，网上搜来搜去有人说vector慢，又有人说STL里的sort排序速度是我等常人不能企及的，有人说要少resize，这个重新分配内存非常耗时，又有人说要用swap来彻底删除不要的内存空间。带着这么多疑问，我实在不知道我程序慢的原因。于是乎阅读了《Effective  STL》，这书挺深的，粗浅读读，先把自己急着弄清楚的以上各问题弄懂。再看程序，做了些优化，速度还是提高不多，头开始大了

我的解决办法是用我上一讲提到的getTickCount、getTickFrequency函数分析我代码的运行时间，遇到的问题是下面这句非常耗时



```cpp
vector<vector<int>>test = vector<vector<int>>(10000, vector<int>(10, 0));
```

 10000*10只是我要开辟的空间，不知道怎么用reserve函数开辟，

这句运行了大概100ms

。今天再看这段代码，发现我太傻了，何必要用vector<vector<int>>呢，完全可以用OpenCV自带的Mat来解决啊，于是把上面这句改写如下 



```cpp
Mat test1 = Mat_<int>::zeros(10000, 10);
```

结果只需要0.2ms！！！

同志们，STL在Mat面前都显得如此无力啊，有木有啊！我决定花费几天的时间再好好读读reference的core的部分，来吃透OpenCV的数据结构。而且感觉Mat跟STL的兼容性很好，也有push_back，pop_back这样的操作，所以啊，同志们，千万别把Mat只当做是显示图片用的，它是很强大的数据结构，用了它，可以事半功倍，谁用谁知道！~~             