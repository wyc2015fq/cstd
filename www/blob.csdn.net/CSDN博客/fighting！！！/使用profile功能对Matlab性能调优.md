# 使用profile功能对Matlab性能调优 - fighting！！！ - CSDN博客
2018年05月23日 20:45:22[dujiahei](https://me.csdn.net/dujiahei)阅读数：173
转自：使用profile功能对Matlab性能调优 - CSDN博客 https://blog.csdn.net/orangehdc/article/details/38110321
做PCANet的时候，Matlab抽风，加不了断点，无奈之下只好把所有的function都写到了一个m文件里。结果发现所有function写在同一个m文件下时，程序运行速度快很多。
后来师兄告诉我，用Matlab的profile功能可以查看程序各个函数的运行时间，这样就能找到原因所在。我尝试了一下，profile真是炫酷得不行。使用pfofile的代码如下：
[plain][view plain](https://blog.csdn.net/orangehdc/article/details/38110321#)[copy](https://blog.csdn.net/orangehdc/article/details/38110321#)
- profile on;  
- profile clear;  
- my_binary_net(1);%我的运行函数  
- profile viewer;  
然后就会一个窗口出来，这个是我把所有function放在一个m文件时候的运行时间：
![](https://img-blog.csdn.net/20140725104013101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb3JhbmdlaGRj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
下面这幅图是我程序的function分开在不同m文件时候的运行时间：
![](https://img-blog.csdn.net/20140725104205484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb3JhbmdlaGRj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过这两个图可以发现造成时间大幅增加是因为addpath函数。这是因为，当我运行的程序的function在不同m文件时，我调用的很多function都包含addpath语句，于是matlab足足调用了22613次addpath。而我运行程序的function在同一个m文件时，我把多余的addpath语句删了，只在程序开头添加了addpath。
既然已经真相大白了，我把其他m文件的多余addpath语句删掉，再运行一次，结果如下：
![](https://img-blog.csdn.net/20140725104337953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb3JhbmdlaGRj/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这次就比上次用时少了500多秒。但还是比所有function写在一个m文件要多出200多秒，可以看出调用m文件也是要耗费很多时间的，但把不同函数写在不同m文件有助于代码复用，是良好的编程规范，没必要为了节省这点时间而把代码全部写在一个文件里。
最后再次发表一下感慨，profile真是个无比强大的功能！以后再也不用tic toc了！
