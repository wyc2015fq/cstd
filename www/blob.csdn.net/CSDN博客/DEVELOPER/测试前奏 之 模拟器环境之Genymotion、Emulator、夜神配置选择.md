# 测试前奏 之 模拟器环境之Genymotion、Emulator、夜神配置选择 - DEVELOPER - CSDN博客





2018年09月09日 16:14:57[学术袁](https://me.csdn.net/u012827205)阅读数：500








  周末是一个学习与总结的好时机。利用工作之余、闲暇之中做些喜欢的事情，一时间会感到很满足、很充实。笔者做安卓移动开发的，为了提高在所开发应用app的性能，得到一款流畅、讨人喜爱的应用，之前在开发路线上也做了很多的功课。晓得怎么快速定位自己亲手开发的应用哪里存在bug问题，哪里存在性能问题，通过什么方式能够更好的优化处理，是很重要的。那么学习一下测试我觉得会是一个很好的技术方案与方向。

  测试中，设备的测试是免不了的操作。起初使用真机，而权限问题却是硬伤。无奈之下忍屈憋痛找了模拟器使用下下，但在配置环境使用**Genymotion**时问题出现了，坑多多吖 ~

为了让更多学习中的伙伴能够快速定位并解决问题，并做一个静静的美男子。在此把自己的配置经验分享一下；
[所需文件下载链接](https://pan.baidu.com/s/169hD5bxqkJ4zn7zgP3KRZg)
**密码：qghq**
![这里写图片描述](https://img-blog.csdn.net/20180909163730571?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
  简单解释一下图的内容，并根据下图目的进行配置。
![这里写图片描述](https://img-blog.csdn.net/20180909164729113?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|apache-ant|adt-bundle-windows|VirtualBox|genymotion|.ova 结尾文件|
|----|----|----|----|----|
|开发环境配置其一|集成了adt、sdk和eclipse的开发包|虚拟机|手机模拟器设备|双击装入VirtualBox的虚拟手机设备|

# 配置自动化测试环境

### 阐明下我的正确配置组合
|操作系统|genymotion版本|VirtualBox版本|
|----|----|----|
|win10-64位|2.6.0|5.2.18|

  接下来，先走一遍我错误配置的过程，然后详尽描述又是如何纠正和走回正道的。

### 错误的配置组合
|操作系统|genymotion版本|VirtualBox版本|
|----|----|----|
|win10-64位|2.6.0|4.3.x|

![这里写图片描述](https://img-blog.csdn.net/20180909162918130?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 配置的整个流程
- 配置jdk
- 配置ant环境(系统变量中配置)
![这里写图片描述](https://img-blog.csdn.net/20180909165709770?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180909165717736?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- 配置**adb**和**andriod**命令环境(同样是在系统变量中进行配置)

adb的命令在路径在`**/sdk/platform-tools` (**'adb’不是内部或外部命令，也不是可运行的程序或批处理文件**)

andriod的命令在路径在`**/sdk/tools` ( **‘android’ 不是内部或外部命令，也不是可运行的程序或批处理文件。**)- 安装VirtualBox
- 安装Genymotion

  我这里的VirtualBox版本使用的是v4.3.4，一个会导致错误的版本，详情请继续翻看
![这里写图片描述](https://img-blog.csdn.net/20180909171747906?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  上图就是出现的问题，而且对于一个初学者而言更是一个很难以解决的问题。但在这个问题上少许花费点时间还是可以解决的。

解决流程：- 设置 VirtualBox 和 Genymotion 兼容性、使用管理员身份运行，如下。其中两者的兼容性设置要一致。
![这里写图片描述](https://img-blog.csdn.net/20180909172303901?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  当然这个设置在我的电脑上还是不能完全解决让我做正常操作的问题。尽管我有些不服气多次尝试了几个版本，如**4.3.10**版本，还是不能！好像**4.3.x版本**本身在我的电脑上是存在问题的。- 于是卸载4.3.x的版本，并重新安装此时最新的**5.2.18**版本（[这个版本是我实在无奈到官网重新下载](https://www.virtualbox.org/wiki/Downloads)）

在最新版本之后就没有问题了，**Genymotion**可以正常打开。但还是无法满足我的需求，在操作过程问题依然存在。

下面的则是`5.2.18版本+Genymotion+兼容性配置+管理员运行配置` 正常启动后的样子
![这里写图片描述](https://img-blog.csdn.net/20180909173624355?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

里面的手机虚拟设备是通过上图中的**.ova文件**双击加入到**VirtualBox虚拟机**中的。- 新的问题出现就是下图中的——委屈，我先在模拟器设备中打开了调试
![这里写图片描述](https://img-blog.csdn.net/20180909174239880?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后自我感觉良好的在dos命令窗口中秀一下，结果
![这里写图片描述](https://img-blog.csdn.net/20180909174402210?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**又出错！！！**尴尬，又有意料之外的问题出现，怎么办？解决它！经过各种方式，得出结论
![这里写图片描述](https://img-blog.csdn.net/20180909174557323?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```
adb server version (32) doesn't match this client (39); killing...
error: could not install *smartsocket* listener: cannot bind to 127.0.0.1:5037: 通常每个套接字地址(协议/网络地址/端口)只允许使用一次。 (10048)
```

这个问题主要是出现在Genymotion身上，当Genymotion启动之后，原始配置上是使用自己默认的。导致了各种的不匹配，但是使用上图中开发包**adt-bundle-windows**中的**sdk**也是不行，会出现同样的问题。

4.于是再次进行尝试，使用我Andrdoid Studio 中SDK，并把sdk地址添加了进去。最后再次操作执行，发现成功了，像下面。最后的总结是，**VirtualBox版本**和**SDK的配置**影响好深远。
![这里写图片描述](https://img-blog.csdn.net/20180909175234966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### Genymotion模拟器安装apk失败

  紧接着我已经迫不及待的要使用adb命令对手机模拟器设备进行apk的安装测试了。然后输入adb安装apk命令，结果~~又炸了。。
`Failure [INSTALL_FAILED_NO_MATCHING_ABIS]`
![这里写图片描述](https://img-blog.csdn.net/20180909205959976?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  Genymotion从版本2.0开始提供的虚拟设备移除了Google Apps,以及ARM library support支持，依靠这些组件运行的app都会报这个错误。而解决方案就是[下载让Genymotion支持app安装运行的ARM架构的工具包](https://download.csdn.net/download/u012827205/10655678)，并装到Genymotion模拟器中。
  网络上有说，下载完了之后直接拖到模拟器中就能提示并配置安装了。我拖我拽了半天，电脑碰巧也是不能够使用这种方式。于是我只能使用命令安装了，生活真美好(**life is shit**)。步骤，
- 下载相应的工具包
- 启开Genymotion虚拟机设备
- 打开dos字符窗口，将本地Genymotion-ARM-Translation.zip工具包拷贝到模拟器的 `/sdcard/Download/`目录
- 然后再进行安装执行

本地拷贝：`adb -s 模拟器名 push 工具包名 /sdcard/Download/工具包的新名字`

安装执行：`adb shell flash-archive.sh /sdcard/Download/工具包的新名字`
![这里写图片描述](https://img-blog.csdn.net/20180909210256449?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018090921070716?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
看到 Done successfully ! 说明工具包安装成功了。然后我就直接执行安装apk的命令，
![这里写图片描述](https://img-blog.csdn.net/20180909210818841?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

发现还是不能安装apk软件到虚拟机中。为什么？？因为需要重新启动虚拟机才行！

4. 重新启动虚拟即，并执行adb的安装命令，终发现，成功了。
![这里写图片描述](https://img-blog.csdn.net/20180909210943568?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 最后的问题 Genymotion 点开安装的app，崩溃

你以为，这样就完美成功了吗？你错了，因为当你安装apk到模拟器中之后，点击运行会发现——瞬间崩溃！而这个问题，我目前试过N中方式，暂时无解。。。。。
![这里写图片描述](https://img-blog.csdn.net/20180910132944123?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后我决定**放弃使用Genymotion**，使用Android Studio的模拟器。喵喵钟成功展示
![这里写图片描述](https://img-blog.csdn.net/20180910152203121?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
你以为这样就算成功了吗？非也，使用我自己Android Studio中自带的安卓模拟器安装上apk，依然出现和使用Genymotion模拟器出现的所有状况一样，app起开崩溃！！突然觉得这世界太复杂了，我就想做一个测试用的手机模拟器而已，犯得着跟我这么较劲儿吗？！

### 跳出惯用方案，改换模拟器——夜神手机模拟器

     简单截图，以作分享
![在这里插入图片描述](https://img-blog.csdn.net/20180919135941942?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

  设置方便、流畅、快捷，既能满足测试功能，又能满足你对美的追求。关键的一点是不用考虑什么 x86、arm架构兼容问题。着实方便，开心到雀跃！[](http://www.yeshen.com/)下载地址

  下载安装之后，要进行手机可调式的模式设置，
``1)手机模式设置，请看设置图例；
![在这里插入图片描述](https://img-blog.csdn.net/2018091914060444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

``2)打开USB调试，设置成为手机的可调式模式；
![在这里插入图片描述](https://img-blog.csdn.net/20180919141443910?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

之后，就能进入到开发者功能选项中进行设置。打开USB调试模式即可！

但是这样使用adb命令``adb devices
![在这里插入图片描述](https://img-blog.csdn.net/20180919141709542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

找不到？！经查询解决方案，找到解决方法。使用夜神手机模拟器的话要使用他自己的adb命令 ——nox_adb命令

但是使用之前，你要配置你的path的系统环境。把你安装的目录中nox_adb的路径配置进去。
![在这里插入图片描述](https://img-blog.csdn.net/20180919142034455?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如此一来你才能进行正常的nox_adb命令。

而事实并非这样，本以为已经万事俱备的时候，突然又出了幺蛾子
![在这里插入图片描述](https://img-blog.csdn.net/20180919142349695?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

又再次走向追寻问题解决方法、良药的坦途！

使用命令 ``nox_adb connect 127.0.0.1:62001 开启连接电脑IP+新的端口。然后就能真正的正常使用了！！！
![在这里插入图片描述](https://img-blog.csdn.net/20180919142654616?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后再安装一下抖音了、慕课网了的apk，安装成功、运行成功且流畅！到此，几天的折腾圆满结束~未完，继续

今天是9月30号，距离开这篇博客已近月。中间辗转回旋，阻碍连连。今天在使用过程中又出现了新的问题——使用Eclipse进行Robotium测试时候，Eclipse无法连接夜神模拟器！！

按照正常的操作逻辑，当我们打开了Eclipse和夜神模拟器（已打开USB调试）。执行测试操作可以看到这样
![在这里插入图片描述](https://img-blog.csdn.net/20180930140457126?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

而结果Eclipse偏偏不能连接已经调试好的夜神模拟器。
无法正常连接原因一，可能夜神模拟器和SDK中的adb版本不同导致的；可以通过下图中命令查看
![在这里插入图片描述](https://img-blog.csdn.net/20180930141316483?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果两者的adb版本不同，复制sdk中的adb，覆盖夜神的adb。
无法正常连接原因二，未进行手动adb连接夜神模拟器，连接方式看下图
![在这里插入图片描述](https://img-blog.csdn.net/20180930141717834?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

连接成功之后执行Eclipse操作（或者重启Eclipse）就能看到识别的夜神模拟器了啦~

然后，我自己写了一个简单的安卓应用作为被测工程，又写了一个测试工程。使用夜神模拟器进行测试，完美通过！！！！！！

















