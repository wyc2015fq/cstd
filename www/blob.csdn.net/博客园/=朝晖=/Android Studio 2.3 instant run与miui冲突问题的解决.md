# Android Studio 2.3 instant run与miui冲突问题的解决 - =朝晖= - 博客园
# [Android Studio 2.3 instant run与miui冲突问题的解决](https://www.cnblogs.com/dhcn/p/7130757.html)
Android Studio最近发布的2.3版本，由于这个版本改进后的Instant Run功能和很多国内ROM存在兼容问题，所以导致不得不做一些妥协策略，具体在小米Rom上，就是把小米rom的调试定制优化功能关闭掉：      
In case you are running a device with MIUI, [Go](http://lib.csdn.net/base/go) to Developer Settings > Turn on MIUI optimization and turn it off.
You will then need to restart your device and recompile the code.
Instant Run should then work again
http://stackoverflow.com/questions/42571175/[Android](http://lib.csdn.net/base/android)-studio-2-3-instant-run-not-working

