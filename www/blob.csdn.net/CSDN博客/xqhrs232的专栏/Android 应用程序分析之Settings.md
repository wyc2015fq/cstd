# Android 应用程序分析之Settings - xqhrs232的专栏 - CSDN博客
2011年11月08日 10:19:29[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1313
原文地址::[http://blog.csdn.net/caszhao/article/details/5986900](http://blog.csdn.net/caszhao/article/details/5986900)
相关网帖::[替换原生系统apk](http://blog.csdn.net/tlight/article/details/6724208)----[http://blog.csdn.net/tlight/article/details/6724208](http://blog.csdn.net/tlight/article/details/6724208)
由于需要修改Setting默认设定的需要，需要分析下Setting的源码，以及数据存储与读取。
Android自带的应用程序存储在packages/apps目录下
主要目录和文件如下：
![clip_image002](http://hi.csdn.net/attachment/201011/4/0_1288842927dJ0c.gif)
其中resouce是资源文件，src是源代码，tests中含有测试代码。而Android.mk和AndroidManifest.xml分别是编译以及应用程序全局数据。
源码的布局当然也是遵循Java的package管理方式。我们以了解Settings – Sound & Display – Orientation 为线索，了解整个Settings应用的工作原理。因为我目前开发的目标板没有gsenser，我的目标是把该项的默认为unchecked。
res/xml/setting.xml中指定Settings包含的选项，从中找到Sound and Display相关的一项
![clip_image004](http://hi.csdn.net/attachment/201011/4/0_1288842929yEot.gif)
根据targetClass的定义，我们便可以从源码中找到对应的SoundAndDisplaySettings类。
![clip_image007](http://hi.csdn.net/attachment/201011/4/0_1288842930QygM.gif)
Sound & Display选项中仍有很多子选项，Orientation只是Sound & Display选项中的一项。同样道理，在res/xml/sound_and_display_settings.xml中，可以找到Orientation选项的定义。
![clip_image009](http://hi.csdn.net/attachment/201011/4/0_1288842931Zl5T.gif)
如何知道accelerometer就是Orientation呢？一方面，从字面意思猜个8、9不离10，另一方面，从packages/apps/Settings/res/values/strings.xml（如下图）也可以看出accelerometer就是对应着Orientation，（默认语言的字符串描述，如果选择了中文语言，那么则在packages/apps/Settings/res/values-zh-rCN/strings.xml找到中文的描述）
![clip_image011](http://hi.csdn.net/attachment/201011/4/0_1288842932CSOi.gif)
进到源码中，常量字符串KEY_ACCELEROMETER定义了该选项的唯一标识。
![clip_image013](http://hi.csdn.net/attachment/201011/4/0_1288842933sSFh.gif)
在OnCreate()初始化中，系统使用了findPreference找到了acclerometer在xml中的描述，把获取的Preference类型强制转换为CheckBoxPreference类。
![clip_image015](http://hi.csdn.net/attachment/201011/4/0_12888429353f2k.gif)
当该Checkbox被点击后，来到响应按钮的代码，若checkbox为checked则写1否则写0
![clip_image017](http://hi.csdn.net/attachment/201011/4/0_1288842936Cc9j.gif)
Setting.System.putInt做的工作，就是把这个选项修改的值写到数据库中去，PutString函数：
![clip_image020](http://hi.csdn.net/attachment/201011/4/0_1288842937kLnf.gif)
![clip_image018](http://hi.csdn.net/attachment/201011/4/0_12888429380oJE.gif)
![clip_image022](http://hi.csdn.net/attachment/201011/4/0_12888429395wGg.gif)
数据库存储于com.android.providers.settings/databases/settings.db 。ANDROID的数据库是采用SQLite存储的，把settings.db复制出来，然后使用sqlite管理工具打开，可以看到刚写进去的值：
![clip_image024](http://hi.csdn.net/attachment/201011/4/0_12888429415nZ3.gif)
accelerometer_rotation的值为0，表示orientation这个Checkbox为not checked.
至此，我们了解了，几乎所有的Settings默认值都会写在settings.db中，若要修改的默认选项值较多，可以使用sqlite数据库工具，手动修改settings.db中的值。已达到批量修改默认值的目的，系统在下次启动时，会自动加载这些写在Settings.db中的值。在批量生产前，只需要把修改后的settings.db 压制到data.img再烧录到系统中就好了。当然，也可以从源码入手，修改该默认值。
还需要了解的一个问题是：系统又是如何在开机设定这个我们设定的Orientation的状态呢？思路无非是从settings.db中读取数据库的值，然后设定相应的状态。
首先，要确定编译时平台的Policy，在build/target/product/core.mk中，有这样的描述：
![clip_image026](http://hi.csdn.net/attachment/201011/4/0_1288842942TzFt.gif)
编译选项中，PRODUCT_POLICY为android.policy_phone。那么系统启动就会采用phone这个Policy。启动读取数据库的值，相关的一个类是PhoneWindowManager（同理，mid对应着MidWindowManager）
从frameworks/policies/base/phone/com/android/internal/policy/impl/PhoneWindowManager.java找到这个类，
这里只关注Orientation这个Checkbox的相关的功能，虽然PhoneWindowManager.java两千多行代码，这里只看Update方法中相关的几行：
![clip_image028](http://hi.csdn.net/attachment/201011/4/0_1288842943mTUM.gif)
这几行就是从settings.db数据库中取得Orientation项的值，如果这个值默认值0不等（即值为1）的话，那么就updateOrientationListenerLp();
整个的分析路线是
![clip_image029](http://hi.csdn.net/attachment/201011/4/0_1288842944cd1u.gif)
全文完
