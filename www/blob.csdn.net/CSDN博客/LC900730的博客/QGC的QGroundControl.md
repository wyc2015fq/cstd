# QGC的QGroundControl - LC900730的博客 - CSDN博客
2017年09月19日 17:36:54[lc900730](https://me.csdn.net/LC900730)阅读数：906
## 注册的单例QGroundControl
在QGCApplication.cc中注册的，类是QGroundControlQmlGlobal 
在QGroundControlQmlGlobal头文件中 
定义了QML可以调用的很多属性和方法
```
Q_PROPERTY(QString appName READ appName CONSTANT)
Q_PROPERTY(LinkManager* linkManager READ linkManager CONSTANT)
...
...
Q_INVOKABLE void startPX4MockLink(bool sendStatusText)
```
在源文件中，初始化了一些属性。
//程序显示的名字 
QGRoundControl Development master:11371d9
```
exists($$PWD/.git){
GIT_DESCRIBE=$$system(git --git-dir );
...
...
}
GIT_VERSION="Development $${GIT_BRANCH}::$${GIT_HASH} $${GIT_TIME}"
```
