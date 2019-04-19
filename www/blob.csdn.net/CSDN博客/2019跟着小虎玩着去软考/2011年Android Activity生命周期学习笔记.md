# 2011年Android Activity生命周期学习笔记 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月12日 16:38:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1429
# 2011年Android Activity生命周期学习笔记
**一．启动Activity：**会依次调用如下三个函数：
1.onCreate;
2.onStart;
3.onResume;
**二．按BACK键：**该Activity将结束，即Destroy销毁掉，会依次调用如下三个函数：
1.onPause；
2.onStop；
3.onDestroy；
**三．按HOME键：**打开其他应用程序，如音乐应用程序；
此时该Activity,会进入不可见状态，会调用如下两个函数：
1.onPause;
2.onStop;
注意：该Activity仅仅不可见，但没有别销毁，因为没有调用onDestroy函数；
**四．再次按HOME键，进入该 Activity时：**意即再次显示该Activity时；
会调用如下三个函数：
1.onRestart;
2.onStart;
3.onResume;
**五．Activity生命周期图：**
![](http://hi.csdn.net/attachment/201104/12/0_130259748741tx.gif)
1.调用onResume函数，意味着：
该activity处于激活状态，是可见的；
2.调用了onPause函数，意味着：
该activity不可见了，用户看不到了；
3.调用onRestart 函数，意味着：
该activity从后台运行变成前台运行，是可见的；
4. 调用onDestroy 函数，意味着：
该activity对象被销毁，即内存中不存在该对象。
