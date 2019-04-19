# “Location of the Android SDK has not been set up in the preferences”问题的解决 - =朝晖= - 博客园
# [“Location of the Android SDK has not been set up in the preferences”问题的解决](https://www.cnblogs.com/dhcn/p/7115301.html)
方法来源：http://stackoverflow.com/questions/5894929/location-of-the-[Android](http://lib.csdn.net/base/android)-sdk-has-not-been-setup-in-the-preferences-in-mac-os
       虽然上面这个帖子里面很多方法鼓励重新安装ADT的最新版本，但是，通过尝试以下方法我解决了我的问题：
      Window > Preferences > General > Startup and shutdown: Check "[android](http://lib.csdn.net/base/android) development toolkit"
     Reestart Eclipse.
     解释一下：上面这个设置是Eclipse启动时需要启动的组件设置，因为我之前为了启动速度高度优化，所以把大多数都禁用了。当然包括ADT.所以重新启用这个启动组件，系统的Android SDK设置才真正开始生效。
- 顶
1

