# Cocos2d-x底层的驱动引擎 - walkabc - 博客园
# [Cocos2d-x底层的驱动引擎](https://www.cnblogs.com/hitfire/articles/3064010.html)
Cocos2d-x的帧频系统默认是60帧每秒。
```
View Code 
1 /**这里返回的单位是按毫秒计算，这个时间的WallClock时间，即1970_1_1到现在的时间差*/
2 static long getCurrentMillSecond() {
3     long lLastTime;
4     struct timeval stCurrentTime;
5 
6     gettimeofday(&stCurrentTime,NULL);
7     lLastTime = stCurrentTime.tv_sec*1000+stCurrentTime.tv_usec*0.001; //millseconds
8     return lLastTime;
9 }
```
上面的方法就是计算当前时间的，而且是以毫秒为单位返回的。至于这个时间有什么用处，接着看下面的代码。
```
View Code 
 1 /**这个方法是个死循环，而且经常会睡一会，但是每次醒来都是干一件大事，驱动场景的重绘*/
 2 int CCApplication::run()
 3 {
 4     // Initialize instance and cocos2d.
 5     if (! applicationDidFinishLaunching())
 6     {
 7         return 0;
 8     }
 9 
10 
11     for (;;) {
12         long iLastTime = getCurrentMillSecond();
13         CCDirector::sharedDirector()->mainLoop();
14         long iCurTime = getCurrentMillSecond();
15         if (iCurTime-iLastTime<m_nAnimationInterval){
16             usleep((m_nAnimationInterval - iCurTime+iLastTime)*1000);
17         }
18 
19     }
20     return -1;
21 }
```
每次App都会睡一会，睡醒之后就干活，当然它也有可能不睡，当if判断为False时，它就没有时间睡觉了，真可怜！
关于Cocos2d-x的底层定时驱动就是这样了。

