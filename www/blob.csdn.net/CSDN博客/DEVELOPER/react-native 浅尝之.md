# react-native 浅尝之 - DEVELOPER - CSDN博客





2018年05月21日 17:16:54[学术袁](https://me.csdn.net/u012827205)阅读数：104标签：[Hybrid](https://so.csdn.net/so/search/s.do?q=Hybrid&t=blog)
个人分类：[ReactNative](https://blog.csdn.net/u012827205/article/category/7683279)

所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









![这里写图片描述](https://img-blog.csdn.net/20180521162310209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此图确实描述了我学习过程的抓狂！ 

经过反复的尝试和对错误log的解读、搜索和解决处理，最终成功编译出了第一个Hybrid-App。效果这样的： 
![这里写图片描述](https://img-blog.csdn.net/20180521162624946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
当然这里我不是在讲解或者去描述它的编译过程。因为这些在官网或者网络上是很多的，不妨看看这些

**该博文内容基于Windows系统，针对Android系统下的APP-参考：**
[官网的Reat-Native环境搭建和测试安装](https://reactnative.cn/docs/0.41/getting-started.html)
[腾讯的一个关于Reat-Native的教学视频](https://ke.qq.com/webcourse/index.html#course_id=197101&term_id=100340928&taid=2091687728054765&vid=d1417tgg1ez)

通过对她们的仔细学习，应该是没什么大问题！但是也不是绝对的。。对于新手来说，有可能每个问题都是致命的。但是都是能通过学习来解决问题的！
现在最新版的react-native-cli命令从创建一个项目到编译命令只有 
`react-native init XXProject`
`cd XXProject`
`react-native run-android`
**现在简单展示一下，通过react-native-cli命令创建并编译执行一个app的过程效果**
*而对于一个正常的操作和反馈的过程是这样的*

使用命令`react-native init XXProject` 效果展示 
![这里写图片描述](https://img-blog.csdn.net/20180521164845592?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此过程不会出现什么错误，只有**零星的几个警告** 效果展示 
![这里写图片描述](https://img-blog.csdn.net/20180521165020428?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用命令`react-native run-android` 效果展示 
![这里写图片描述](https://img-blog.csdn.net/20180521165121806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

当前过程，当你成功创建的Project所需要的gradle目录中没有其压缩包，则会自动去下载`gradle-2.14.1-all.zip` ，之前自动下载的它，有问题，然后我就到gradle官网下载了一个一模一样的并放到它原要求的目录下，但是当我执行`react-native run-android` 的时候还是会自动去下载（或许是我个人特例）。直到最后下载成功之后，便自动安装到手机上，效果展示 
![这里写图片描述](https://img-blog.csdn.net/20180521165537805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**这篇博客的写作目的是来分享一下我学习过程中遇到的react-native问题！**

IMPORTANT：执行init 或者 run 命令的时候，尽量多执行几次，避免由于网络原因造成不必要的执行失败，并让你认为是自己的问题；并且，这两个执行过程是很慢的，要有心里准备哦！ 

比如，我第一次执行init，意外的出错！！但是我再次执行，又正确了！看下错误的效果 
![这里写图片描述](https://img-blog.csdn.net/20180521174413511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
执行命令`react-native init` 出现了好多你在我上面没有见到过的WARN警告，或者最后没有出现 
`to run your app on IOS`  和 `ro run your app on Android` 这两句话。 

说明你的init有问题，请查看您的环境配置。 

比如，我把SDK的环境故意给配置错误(注：我的正确SDK环境配置是在用户变量中进行配置的)然后出现了预知的错误 
![这里写图片描述](https://img-blog.csdn.net/20180521174251898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后会出现错误的误导，让你执行命令`npm install`（之前的老版本是要执行它，但是现在从官网上看是不用的），误导效果 
![这里写图片描述](https://img-blog.csdn.net/2018052117303843?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
执行命令`react-native run-android` 出现了下面的错误 
![这里写图片描述](https://img-blog.csdn.net/20180521163436266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里说明了，对于你已经下载过的 `gradle-2.14.1-all.zip` 文件是损坏的。你只需要找到它所对应的目录，重新下载。
执行命令`react-native run-android` 出现了下面的错误 
![这里写图片描述](https://img-blog.csdn.net/20180521171236944?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里说明了，对于你已经下载的 `gradle-2.14.1-all.zip` 文件需要重新下载，不对！因为我已测试，删除损坏的并使用重新下载的完整且正确`gradle-2.14.1-all.zip`之后，再次执行上面命令，能正常执行并安装到手机上！！
执行命令`react-native run-android` 出现了下面的错误 
![这里写图片描述](https://img-blog.csdn.net/20180521170740583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

走到这里说明你的各种配置已经没有问题了，只是你手机上存在一个使用`react-native`装载了的app，卸载之前的app并摇动手机`reload`就好了！
**总结起来**，如果按照上面做基本的配置，比如下载安装、npm的配置等，自己觉得都弄好了，那么在`react-native init`  时候出现了好多警告问题并且`react-native run-android`的时候老不成功、出错。这时，你需要仔细排查一下你的`SDK环境`和`gradle`了。












