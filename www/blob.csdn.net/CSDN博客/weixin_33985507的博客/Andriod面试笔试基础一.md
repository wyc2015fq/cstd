# Andriod面试笔试基础一 - weixin_33985507的博客 - CSDN博客
2016年10月30日 22:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
1.activity生命周期
第一次启动一个Activity
依次调用：onCreate-->onStart-->onResum
情形①
点击Home键返回桌面时，依次调用：onPause()-->onStop()，因为当我们点击Home键退回到桌面时并没有调用onDestory，因为此时Activity只是处于后台不可见的状态，并没有被销毁。
从后台切回到Activity时依次调用onRestart-->onStart-->onResume
情形②屏幕旋转
当屏幕旋转的时候我们发现Activity先是被销毁之后又被重建。而且我们还发现虽然Activity被重建了，但是控件里的内容还在，这是为什么呢。
  这是因为当Activity不是正常退出的时候，它不仅调用onPause，onStop，onDestory，它会在调用onStop之前调用onSaveInstanceState这个方法来保存一些数据，当Activity重建的时候，这个方法所存储的数据会以Bundle作为参数传递给onCreate，所以我们可以用参数来判断这个Activity是新建的还是重建的，如果是重建的，则可以从Bundle中取出数据来恢复界面。
  那我们可不可以不让他销毁并重新创建呢？
  答案是肯定的，你只需要在AndroidManifest文件中的对应Activity中配置
android:configChanges="keyboardHidden|orientation|screenSize"，最好这三个都配置，否则不能适配所有机型或sdk版本。
设置之后你会发现不会销毁和重新创建了，只会调用onConfigurationChanged这一个方法，所以我们可以在里面做一些特殊的操作。
  当然如果你不想让他旋转就在AndroidManifest文件中的对应Activity中配置android:screenOrientation=”landscape”(横屏，portrait是竖屏)；
![2704327-278a4cae2eefc8cf.png](https://upload-images.jianshu.io/upload_images/2704327-278a4cae2eefc8cf.png)
Paste_Image.png
参照:[http://www.jianshu.com/p/94e3ab2de108](https://www.jianshu.com/p/94e3ab2de108)
2.service生命周期，什么情况下使用service，service的二种启动方式，有什么区别
![2704327-3e41ead06d25cde0.png](https://upload-images.jianshu.io/upload_images/2704327-3e41ead06d25cde0.png)
Paste_Image.png
3.service、activity、thread生如何通信
4.对象可以不通过new么
5.构造方法，static方法，static变量的执行顺序，父类A有以上三个，子类B也有以上三个，那么执行顺序是
