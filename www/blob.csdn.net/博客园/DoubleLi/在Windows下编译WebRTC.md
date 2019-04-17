# 在Windows下编译WebRTC - DoubleLi - 博客园






## 前言

这篇文章的目的在于为你节省生命中宝贵的10小时（甚至更多），或者浪费你10分钟。作为Google更新频繁的大型跨平台基础库，WebRTC的编译一直被人称为噩梦。如果恰巧你偏要在Windows下编译WebRTC，那么你最好提前对自己的耐心做充分的评估。

截止目前，我尝试过中文博客社区里几乎所有的文章，可以说全部失效了。我不知道这篇文章的有效期有多久，因为webrtc.org社区的家伙们更新实在太勤奋了！

## 环境 

OS：Microsoft Windows 7 Ultimate 6.1.7601 Service Pack 1 Build 7601

你只需要一台Win7 64机器即可，其他一概用WebRTC提供的就行。如果使用WebRTC自带的编译工具链的话，宿主机器已安装的Python和VS等等与WebRTC可以做到毫不相关，请不要被某些文章蒙蔽。

## 打开VPN

原因你懂的。推荐挑一个快点的TW或JP节点，保持网络通畅。

## 安装depot_tools

使用SVN checkout
- http://src.chromium.org/svn/trunk/tools/depot_tools

然后运行depot_tools\gclient.bat，这个脚本会触发检测逻辑，自动下载python276_bin和git-1.9.0.chromium.6_bin。

至此，我假设depot_tools已经安装完毕。如果没有，请关闭这个页面，然后自行Google。

## 检出WebRTC代码

建立一个文件夹，名叫WebRTC，然后新建一个bat脚本，写入：
- set PATH=[你的depot_tools路径，比如C:\depot_tools];%PATH%
- call gclient.bat config --name src http://webrtc.googlecode.com/svn/trunk
- call gclient.bat sync --force
- set GYP_GENERATORS=msvs
- call python src/webrtc/build/gyp_webrtc -G msvs_version=2013

保存，然后双击执行。接着是漫长的等待，10MB宽带的情况下我等了3个小时。期间控制台会一直显示still working on src，事实上，这时有几个G的东西等待下载。[官网](http://www.webrtc.org/reference/getting-started)的说明够迷惑人的，这个脚本应该最大程度上简化了问题。

注意，脚本中最后一句的意思是生成VS的编译文件（.sln)，这会触发gclient自动下载vs2013 express版的核心文件，所以不需要宿主机器上原先安装的VS。

脚本运行完毕后，src目录下应该出现了许多文件和目录，其中就有all.sln。

## 编译

### 下载DirectX开发包

DirectX SDK June 2010 [http://www.microsoft.com/en-us/download/details.aspx?id=6812](http://www.microsoft.com/en-us/download/details.aspx?id=6812) ，解压备用。

### 使用VS2013编译（不推荐）

直接双击all.sln打开，为每一个子模块添加include：DXSDK\Include和library：DXSDK\Lib;depot_tools\win_toolchain\vs2013_files\wdk\lib\Atl\amd64 。

由于模块非常非常多，所以不推荐。

### 使用Ninja编译

Ninja是Chromium社区开发的一套build system，不要害怕陌生的Ninja，很快你就会爱上它，并且对VS心生厌倦。

编译前依然要搞定DirectX SDK，这次直接将DXSDK\Include复制到depot_tools\win_toolchain\vs2013_files\VC\include，DXSDK\Lib复制到depot_tools\win_toolchain\vs2013_files\VC\lib。这么做简单粗暴，方便高效。

使用Ninja编译的话，我也提供一个方便的脚本：
- @EchoOff
- :: hankcs
- title Hankcs's program
- CD\ 
- %~d0
- CD %~dp0
- set PATH=[你的depot_tools路径，比如C:\depot_tools];%PATH%
- 
- del ninjia.log
- ninja -C out/Release >> ninjia.log

保存为bat，放到WebRTC\src目录下执行。

如果遇到了类似下面的错误：
- error c2220: warning treated as error -no'object' file generated
- ...
- 'utf8' codec can't decode byte 0xd1 in position 0: invalid continuation byte

请在控制面板中修改语言为下图所示：

![](http://www.hankcs.com/wp-content/uploads/2014/10/2014-10-23-22-10-51.png)

然后再次运行，你会发现error c2220神奇地消失了。

## 运行

一切顺利的话，WebRTC\src\out下会出现四个文件夹，如图所示

![](http://www.hankcs.com/wp-content/uploads/2014/10/2014-10-23-22-16-14.png)

说明编译成功。

编译完了之后我想说，他娘的编译一下就这么麻烦，做这玩意得多难啊，人与人的差距真他娘大。

## Reference

[http://www.webrtc.org/reference/getting-started](http://www.webrtc.org/reference/getting-started)

[http://egamesir.blog.163.com/blog/static/188096088201362411524585/](http://egamesir.blog.163.com/blog/static/188096088201362411524585/)

[http://blog.csdn.net/choday/article/details/19978427](http://blog.csdn.net/choday/article/details/19978427)









