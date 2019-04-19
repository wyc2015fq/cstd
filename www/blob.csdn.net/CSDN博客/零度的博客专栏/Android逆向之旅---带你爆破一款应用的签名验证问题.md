# Android逆向之旅---带你爆破一款应用的签名验证问题 - 零度的博客专栏 - CSDN博客
2017年02月28日 13:48:24[零度anngle](https://me.csdn.net/zmx729618)阅读数：292标签：[android																[应用																[破解																[安全](https://so.csdn.net/so/search/s.do?q=安全&t=blog)](https://so.csdn.net/so/search/s.do?q=破解&t=blog)](https://so.csdn.net/so/search/s.do?q=应用&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/zmx729618/article/category/6238076)
# 一、前言
在之前的文章中说过[Android](http://lib.csdn.net/base/15)中的安全和破解是相辅相成的，为了防止被破解，很多应用做了一些防护策略，但是防护策略也是分等级，一般简单的策略就是混淆代码和签名校验，而对于签名校验很多应用都是会做的，一般现在就两种方式：
**第一种：签名校验不通过直接退出程序，也就是你会发现回编译二次打包运行失败的现象**
**第二种：签名校验不通过不触发程序指定逻辑，导致没有错误信息，也进不了应用的现象**
关于Android中应用防护策略的文章可以看这里：[Android中应用的攻防之战](http://www.wjdiankong.cn/android%E9%80%86%E5%90%91%E4%B9%8B%E6%97%85-android%E5%BA%94%E7%94%A8%E7%9A%84%E5%AE%89%E5%85%A8%E7%9A%84%E6%94%BB%E9%98%B2%E4%B9%8B%E6%88%98/) 今天我们就来看一下签名校验的应用案例，在回编译二次签名之后运行进不去游戏的问题，其实在之前已经分析过了一个签名验证的问题，那里的问题是回编译二次打包运行失败的问题。不了解的同学可以去这里看一下：[Android中使用静态方式破解apk应用](http://www.wjdiankong.cn/android%E9%80%86%E5%90%91%E4%B9%8B%E6%97%85-%E9%9D%99%E6%80%81%E5%88%86%E6%9E%90%E6%8A%80%E6%9C%AF%E6%9D%A5%E7%A0%B4%E8%A7%A3apk/)；这种运行失败的应用比较好解决，因为这种签名校验的一般都在应用的入口处，所以直接在入口处加一些日志，通过打印日志就可以看到，**入口一般是Application的onCreate方法和attachBaseContext方法**，而通过这个案例之后我们也学习到一个快速定位签名校验的方法的技巧：全局搜索signatures字符串，因为我们知道如果要做签名验证，必须调用系统的一个api方法：**getPackageManager().getPackageInfo(getPackageName(), 64).signatures**，所以在用Jadx工具反编译apk之后，全局搜signatures字符串可以立马定位到签名校验的方法。
# 二、案例分析
上面分析完了之前一个解决签名校验的案例，下面就用这个小游戏作为简单分析，这个游戏在市场都可以下载到：
![](https://img-blog.csdn.net/20170120153555554?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
本文研究的是V3.3版本的。我们下载游戏之后，可以利用apktool进行反编译，然后回编译重签名，安装运行：
![](https://img-blog.csdn.net/20170220090646538?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
游戏可以运行成功，但是卡在这里，进不了游戏了，这时候我们就猜想他可能有签名验证逻辑，可能放在本地，也有可能放在服务端，我们利用上面说到的那个技巧：在Jadx中全局搜"**signatures"**字符串内容：
![](https://img-blog.csdn.net/20170120130612769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这时候会发现有很多地方都在使用，其实可以推断出，这个游戏为了防止二次打包在很多地方都加了签名验证的逻辑，这样对于破解也是增加一定的难度，因为这里签名校验的地方太多了，所以没法一个一个修改，所以咋们用另外一种方式高效率的解决问题：**在运行游戏的时候发现游戏卡在进度条了，所以猜想和网络请求有关，所以咋们就通过Fiddler抓包来分析他的请求信息:**
![](https://img-blog.csdn.net/20170220090743101?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
游戏打开就这些请求，但是作为一个游戏不可能就这点请求的，这个也是卡住的原因，我们可以这么做，用一个正常的游戏在抓包看看情况：
![](https://img-blog.csdn.net/20170220090653865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个是没有二次打包签名的请求信息，和上面二次打包签名之后的请求相比较，多了很多请求，可以发现有一个get_user_info这个接口比较特殊，所以我们可以去Jadx全局搜这个请求接口：
![](https://img-blog.csdn.net/20170120153634788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里看到定义了url的地方，然后在全局Find Usage的地方：
![](https://img-blog.csdn.net/20170120153638508?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点进去看方法调用：
![](https://img-blog.csdn.net/20170120154023731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
继续查看这个方法的调用地方：
![](https://img-blog.csdn.net/20170120154123462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里有四个地方调用这个方法，但是可以依次排除，最终定位到是com.wepie.snake.module.home.b.d.e()方法，那么下面来看一下如何进行排除的，可以从下往上排除，看最下面一个方法：
![](https://img-blog.csdn.net/20170120154403730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后进入l.a方法中查看逻辑：
![](https://img-blog.csdn.net/20170120154407346?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里其实是个请求，请求接口是d.r的值：
![](https://img-blog.csdn.net/20170120154410534?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看到是这个接口信息，但是我们在用Fiddler抓包的时候并没有看到这个请求：
![](https://img-blog.csdn.net/20170220090653865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
所以这个地方可以过滤了，其他三个地方的分析结果类似。所以通过这种方式定位到了方法调用的地方：
![](https://img-blog.csdn.net/20170120154714372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们在查找这个e方法调用的地方：
![](https://img-blog.csdn.net/20170120154743961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在这里被调用了，通过调用结果发现，有第一个判断：
![](https://img-blog.csdn.net/20170120154916776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里有一个字符串比较相等的逻辑，很有可能是比较签名信息的，可以查看h.a这个方法：
![](https://img-blog.csdn.net/20170120154922108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里貌似有一个字符串的构造算法，我们为了看到最终的字符串内容，可以新建一个简单的Java项目，然后把这个方法打印一下看看结果：
![](https://img-blog.csdn.net/20170120155141936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看到这个最终生成的字符串内容是：**678a930b9829b54a44f92a840916f7d1**，然后再看一下equals的o.a的方法：
![](https://img-blog.csdn.net/20170120155310875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个方法是获取应用签名信息的，然后用MD5计算结果。所以到这里我们就知道校验签名逻辑是，获取应用的签名信息，然后MD5一下，和"**678a930b9829b54a44f92a840916f7d1**"字符串进行比较。那么我们二次签名之后，这个判断肯定就是false了，所以后续的逻辑就不走了，没有后面的请求，发现卡在开始界面了。
那么问题找到了，改起来就比较简单了，直接反编译游戏，然后找到这个 com.wepie.snake.helper.f.o.a()方法对应的smali代码：
![](https://img-blog.csdn.net/20170120155803352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
把这个方法直接返回"678a930b9829b54a44f92a840916f7d1"字符串即可。**有的同学说这个怎么改？是手动编写samli代码吗？肯定不是的，咋们可以先写一个简单的返回这个字符串的方法，然后反编译得到这段smali代码就可以了，可千万别自己手动的去编写**，除非你有这个耐心和毅力，反正我没有。替换完成之后，咋们就回编译，二次签名运行安装游戏，可惜的是还是卡住了，所以还得回去看代码：
![](https://img-blog.csdn.net/20170120160258686?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
咋们修改了一次签名校验方法，进入了第一层签名判断，继续往下看代码：
![](https://img-blog.csdn.net/20170120160303957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里又有一个判断，点进去查看逻辑：
![](https://img-blog.csdn.net/20170120160307233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
果然，这里又有了一次签名校验方法，所以还得手动的修改，修改方法和上面类似，把这个p方法改一下即可：
![](https://img-blog.csdn.net/20170120160647975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改成功之后，再次回编译重签名安装运行，可惜的是还是卡住了，进不了游戏，这时候我们在次抓包看看：
![](https://img-blog.csdn.net/20170220090658959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这次比之前多了一个config_v3_android请求，但是还是没有get_user_info的请求！所以还得去看代码逻辑，不过从请求结果来看，我们之前的两次签名校验修改已经有效果了，继续看下面的代码：
![](https://img-blog.csdn.net/20170120161514499?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这里可以看到，有一个设置进度条的逻辑，而且有一个**tag=999**的日志，貌似是取配置信息的进度条，那么我们可以查看这个日志信息：
![](https://img-blog.csdn.net/20170120161349044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看到了这个日志信息之后，发现有开始取config到获取成功config了，但是后面就没有日志了，所以这里猜想应该是在本地解析这个配置信息的时候还有判断，咋们全局搜一个字符串信息"**getConfigAndroid**"：
![](https://img-blog.csdn.net/20170120161854590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
第一条信息就是我们想要的，点击进入：
![](https://img-blog.csdn.net/20170120161857411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
果然这里还有一个判断，进入查看：
![](https://img-blog.csdn.net/20170120161900833?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
又是一次签名校验的逻辑，好吧，还得再一次改这个com.wepie.snake.helper.b.a.i()方法了：
![](https://img-blog.csdn.net/20170120162114044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改成功之后，再次回编译二次签名安装运行即可，这次终于运行成功，进入游戏界面了。
# 三、签名校验破解方案
好了，到这里我们来总结一下关于如何解决应用二次签名校验的问题：
**第一步：**先在Jadx中打开应用之后，**全局搜字符串内容："signatures"**，这个就可以定位到获取应用签名信息的地方了，然后可以依次跟踪校验的地方了。
**第二步：**在第一步搜索结果比较多的情况下，我们可以通过应用运行效果采用这两种方式：
- **第一种方式：**如果应用运行失败，那么可能是在应用入口处作了签名校验逻辑，如果校验失败就退出程序。一般入口处是在Application的onCreate方法和attachBaseContext方法。
- **第二种方式：**如果应用可以运行，但是卡在某一个地方，我们可以采用抓包技术来分析应用运行到哪一步卡住了，因为现在的应用都会有请求的，如果是卡住了，那么请求就会和原始包的请求不同，可以根据不同的请求结果来跟踪代码逻辑，就比如本文的案例。
上面就是本人总结的签名校验的大致解决步骤和方法，但是肯定还有其他场景，比如有的应用会把签名放到native层，但是这些校验逻辑都是可以做处理的，也有的应用把签名信息放到服务端进行校验，这个也是可以处理的。放在native层的话，最终也是通过java层做连接访问的，只要到了java层，那么就可以找到校验方法的地方，放在服务端校验，还是可以通过抓包查看请求来解决的。
# 四、签名校验破解后续
从上面看其实在给应用做防护的时候，签名校验这种方式不是百分百的安全，只能防止一些破解小白。其实本文的案例中可以发现，**这个应用其实想通过多个地方的签名校验来做到全局校验，本文中可以看到我们做了三次的签名校验方法代码修改才成功的，而且后续版本也不排除他们还会继续增加，以及在其他地方逻辑处也做了签名校验判断**。
从本文中也可以看到如果一个应用在很多地方都做了签名校验，那么我们手动修改会显得很麻烦，其实这里有这个思路：**获取应用的签名方法是固定的：getPackageManager().getPackageInfo(getPackageName(), 64).signatures，我们可以直接修改应用的Application信息，通过反射机制，把校验对象的字符串内容设置到signatures中，说白了就是用反射来修改应用的签名信息，这样在应用中所有获取签名的地方都是我们设置的指定签名值，也就是比对的那个常量字符串内容，所有的签名校验方法都是返回true了**。这个思路本文不再尝试了，感兴趣的同学可以尝试。就是一个反射调用即可。有的同学可能在想怎么不用Xposed进行hook应用的签名获取方法那样不就更简单了。这里一定要注意呀，我们为什么要解决签名校验，就是因为我们想二次签名打包，而Xposed进行hook的时候是不需要二次打包的，主要hook点找到，不用进行二次打包安装就可以实现破解了。这两种方式思路是不同的，要注意哈！
# 五、给应用安全防护的意见
通过本文案例，其实对于现在应用防护有这些建议：首先关于应用签名校验这块逻辑，可以做的更安全点，就是在native层用反射调用系统获取签名的方法，然后直接在native层进行比较，如果发现签名不正确，就退出程序，全部放在native层做，这样安全系数会高点！
但是这种也不是最安全的，安全点也就是考虑加固方案了，现在加固平台很多，选择企业版加固，对于破解难度那是肯定会加大的。但是加固有一个弊端，就是崩溃率的问题，因为了解加固原理的同学都知道，现在加固平台都会涉及到so逻辑，而Android因为机型和系统版本复杂，导致兼容性不好，会有一定的加固崩溃率的，而对于一些用户量比较多的应用和游戏，他们无法忍受这种崩溃率，那么就放弃加固了。但是加固还是一个相对安全的策略！
**严重声明：本文的目的只有一个，通过一个案例来分析现在应用的签名校验逻辑，如何破解这种校验方式，如果有人利用本文内容进行任何商业目的和非法牟利，带来的任何法律责任将由操作者本人承担，和本文作者没有任何关系，所以还是由衷的希望大家秉着技术学习的目的阅读此文，非常感谢！**
# **六、总结**
本文通过一个案例分析了应用签名信息校验的防护策略破解，最后也总结了现在这种签名校验防护策略的弊端和改进的地方，如何做到安全系数比较高的防护。但是也不是最安全的，因为没有绝对的安全！安全和逆向是相互进步！共同促进社会进步和技术革新！最后要是阅读完本文内容觉得有收获就点个赞多多分享，要是有打赏那就最好了啦！
