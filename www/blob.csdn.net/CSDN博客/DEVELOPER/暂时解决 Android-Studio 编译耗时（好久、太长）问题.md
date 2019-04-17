# 暂时解决 Android-Studio 编译耗时（好久、太长）问题 - DEVELOPER - CSDN博客





2017年10月25日 11:02:34[学术袁](https://me.csdn.net/u012827205)阅读数：3431








*简单描述一下我遇到的问题*

2017年10月24日下午6点之前我的 android studio 编译器还是瞬间编译，但是之后我遇到的问题就让“怒发冲冠”，从6点到晚上10点从gradle的初始化编译到apk编译，我一共执行了3次，分别是2小时、48分钟、20分钟，一晚上好像什么都没干。虽然是在加班，但是这样不太好！

这么耗时，肿么办！然后在网上针对我的情况了解了一下，找到了一个适合我的，但是也不是长久的 解决问题，作为临时的解决方法。针对我的问题首先表明一下问题的所在：
- Project 下的 gradle.properties

![](https://img-blog.csdn.net/20171025110959387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





在你的电脑所能承受的范围内，尽量的把 org.gradle.jvmargs 她的值设置更大，这样运行编译的时候我们的 android studio 就会拥有足够的编译内存空间。

- Offline work

然后就是开启离线的代码编译模式。我们都知道，我们自己写的项目会依赖很多的第三方类库，有的是你每次进行项目的编译，她都会进行检测更新最新版本。这样导致你的编译器无论是在初次打开项目环境的初始化，或是运行项目编译，都会非常的慢。但是，请注意，如果是在我们已经完全初始化过了项目所需要的依赖内容。这时候，我们就可以开启“离线”代码编译模式。如此，android studio 就不会再到互联网上检测我们的依赖环境-或更新了。可以节约很多时间，具体操作设置请看





![](https://img-blog.csdn.net/20171025112057982?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171025112105242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




- 但是有一点要注意:如果有新的项目代码依赖，要关闭离线模式，进行在线代码环境初始化，之后再次开启就-ok






