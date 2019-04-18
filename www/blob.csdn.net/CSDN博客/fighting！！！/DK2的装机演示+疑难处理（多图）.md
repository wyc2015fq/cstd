# DK2的装机演示+疑难处理（多图） - fighting！！！ - CSDN博客
2017年08月05日 19:37:31[dujiahei](https://me.csdn.net/dujiahei)阅读数：350
                
【迟到的抽筋教程】DK2的装机演示+疑难处理（多图）_oculusrift吧_百度贴吧
http://tieba.baidu.com/p/4378676276
大家可能遇到的程序问题，第一个应该是系统版本和补丁：
首先（应该没有人还在用XP了吧![](http://tb2.bdstatic.com/tb/editor/images/face/i_f05.png?t=20140803))，如果你在用win7，一定要打上sp1的补丁，然后再安装KB2670838补丁。PS;最好不要用ghost系统，不然安装这些补丁后很容易蓝屏~![](http://tb2.bdstatic.com/tb/editor/images/face/i_f33.png?t=20140803)
接下来，咱们需要安装运行环境包，我用的是三大妈的，百度网盘链接：[http://pan.baidu.com/s/1hrwQDSC](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cIHyXBR4Zi8xQSWjf19UXXJfMy43DyOLVxGKEq1V/n2XTwPaOqOs5DNK5IHmLrjNuDPKbicycc6TfFGT0JZiCS3SFCUy7OYy3WbkMntjBuG1jA8Zu4mdgY0) 密码：ak1d；
然后是O记的runtime驱动程序，0.6.0.1版本就能玩大部分游戏了，0.8能玩一些最新的Demo。![](http://tb2.bdstatic.com/tb/editor/images/face/i_f05.png?t=20140803)win10系统只支持0.7以上的版本，这里给大家贴上0.4到0.8版本的所有驱动吧~
链接：[http://pan.baidu.com/s/1i4v2JWx](http://jump.bdimg.com/safecheck/index?url=x+Z5mMbGPAvVIlwZePSt0B3tEqEFWbC4tOatFxkC6cKDKpwh1y1ltkR9weFIp+djfMy43DyOLVxGKEq1V/n2XTwPaOqOs5DNK5IHmLrjNuDPKbicycc6TfFGT0JZiCS3SFCUy7OYy3WbkMntjBuG1jA8Zu4mdgY0) 密码：gmpx
好啦，上面这些应该都是小儿科，下面才是问题重灾区：（如果里面没有你的问题，私聊楼主呗，但不能保证即时回复诶）
问题重灾区一：配置
亲测，玩DK2的最低配置其实是：（千万不要看走眼！！）i3+GTX660![](http://tb2.bdstatic.com/tb/editor/images/face/i_f16.png?t=20140803)
在这个配置上面，一些精细Demo和游戏玩起来会有明显的拖影，比如【子弹时间】【回到恐龙岛】
想大部分游戏玩得流畅，配置在：i5 4590+GTX970 是一个档次，然后贵效果就越舒服。
![](http://tb2.bdstatic.com/tb/editor/images/face/i_f25.png?t=20140803)
PS. 本码农的机器是E3+970，卡吧基佬标配~![](http://tb2.bdstatic.com/tb/editor/images/face/i_f25.png?t=20140803)
想流畅地DK2乃至我已经订了而且还没被砍单的CV1（再次强调）![](http://tb2.bdstatic.com/tb/editor/images/face/i_f25.png?t=20140803)，这个配置是分水岭哟~
再PS. 笔记本免谈，再牛X的神级笔记本也不行，i7+970独显的笔记本上，直接无视那张显卡，原因可能是功耗跟不上+发热的潜在问题~
重灾区二：安装Oculus
 Runtime时崩溃
![](http://tb2.bdstatic.com/tb/editor/images/face/i_f05.png?t=20140803)首先~得检查有没有把游戏运行环境包装全，win7的话右键点击【我的电脑】属性先看是不是sp1的，然后看更新包KB2670838有没有装；
win10的话没法使用0.7以下的O记runtime~装上了也总是会报错；
![](http://imgsrc.baidu.com/forum/w%3D580/sign=281343e4ce177f3e1034fc0540ce3bb9/dcb3a6efce1b9d1604214b33f4deb48f8d546470.jpg)
然后，我们还需要去控制面板的【程序安装/卸载】那里检查.NET组件装全没有![](http://tb2.bdstatic.com/tb/editor/images/face/i_f17.png?t=20140803)~
![](http://imgsrc.baidu.com/forum/w%3D580/sign=884441a07a1ed21b79c92eed9d6fddae/7721574e9258d109bc0e79e0d658ccbf6d814d6c.jpg)
还有微软 VC++运行库，各版本的32位和64位缺一不可![](http://tb2.bdstatic.com/tb/editor/images/face/i_f33.png?t=20140803)
![](http://imgsrc.baidu.com/forum/w%3D580/sign=563579e0d658ccbf1bbcb53229d9bcd4/09b97dd98d1001e9dceac45dbf0e7bec55e79700.jpg)
没完~还有VF+VJ
![](http://imgsrc.baidu.com/forum/w%3D580/sign=4c941154a8c379317d688621dbc5b784/1367c9fcc3cec3fdc8677427d188d43f869427f8.jpg)
最后了！XNA ！
![](http://imgsrc.baidu.com/forum/w%3D580/sign=ceee94e94010b912bfc1f6f6f3fcfcb5/8780e1fe9925bc315f9fd2ca59df8db1ca137013.jpg)
运行环境组件就这么多了，上面的网盘里有打包，大约700多M。
重灾区三：安装时弹这样的错误：
![](http://imgsrc.baidu.com/forum/w%3D580/sign=5a99a8a0d262853592e0d229a0ee76f2/f1c0c8ea15ce36d314f051623df33a87e850b166.jpg)
经过测试，只要尼玛安装过旧版本O记Runtime驱动，卸了再安装新版本的，有很有机会出现这个问题，可能是因为某些程序溢出或者冲突之类。（ PS. 很多国外硬件的驱动都会这个问题）
解决办法：找到安装目录，彻底粉碎性删除，重启，就OK~![](http://tb2.bdstatic.com/tb/editor/images/face/i_f33.png?t=20140803)
重灾区四（重灾区一没处理好的结果）：在安装进度条进行到90%的时候，提示无法安装Display
 Driver，点【重试】没用，点【跳过】，提示无法安装Tracker Driver![](http://tb2.bdstatic.com/tb/editor/images/face/i_f17.png?t=20140803)
问题原因：游戏运行环境组建漏了某N个；
解决办法：现有下载帖子2楼的游戏运行环境包，装个痛快；然后把安装了90%的O记Runtime删个干净，然后继续安装；还是不行的话就重装系统，记住不要用ghost哟~![](http://tb2.bdstatic.com/tb/editor/images/face/i_f25.png?t=20140803)
最恶心人的重灾区五：游戏环境装好了，Runtime驱动装好了，头显里面却无法显示内容，选显示器扩展模式也不行。
原因1：Runtime驱动的版本问题，下载最新的吧，帖子2楼有；
原因2：游戏是虚幻引擎制作的话，就按Alt+Enter键切换VR模式和普通模式；
