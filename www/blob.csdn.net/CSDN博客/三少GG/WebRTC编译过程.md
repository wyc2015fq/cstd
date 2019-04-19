# WebRTC编译过程 - 三少GG - CSDN博客
2011年09月29日 15:49:41[三少GG](https://me.csdn.net/scut1135)阅读数：7864
![](http://hi.csdn.net/attachment/201109/29/0_13173060222yfs.gif)
(一).安装 depot_tools **[http://www.chromium.org.sixxs.org/developers/how-tos/install-depot-tools](http://www.chromium.org.sixxs.org/developers/how-tos/install-depot-tools)**
**Windows **
1. Download [depot_tools.zip](http://src.chromium.org.sixxs.org/svn/trunk/tools/depot_tools.zip) and
 decompress it. 或者TortoiseSVN更新: **svn co http://src.chromium.org/svn/trunk/tools/depot_tools**
2.Add `depot_tools` to the **end** (not
 start!) of your PATH --->System Properties, Advanced properties, Environment Variables, modify the "Path" system variable.
   系统重启即可。(最好关闭win7防火墙)
**Linux and Mac**
**提前先安装好git和subversion！**
**安装git（不要自己去官网下载make，直接  ** sudo apt-get install git git-core 来安装Git程序**，或者到新立得软件包里面 输入 git然后安装)**
**[ubuntu下的SVN安装十分简单，sudo apt-get install subversion](http://www.cnblogs.com/cocowool/archive/2008/11/10/1330932.html)**
1. Grab the checkout with one of: non-committers (to depot_tools): svn co http://src.chromium.org.sixxs.org/svn/trunk/tools/depot_tools
2. Add depot_tools to your PATH:
     export PATH=$PATH:/home/grant/programfiles/depot_tools
You may want to add this to your .bash_aliases file or your shell's equivalent so that you
 don’t need to reset your $PATH manually each time you open a new shell.
(二).安装git
windows版本，去git官网下，安装时选择添加环境变量。
此外，安装
[Microsoft
 SDK 7.1](http://www.microsoft.com.sixxs.org/downloads/en/details.aspx?familyid=6b6c21d2-2006-4afa-9702-529fa782d63b&displaylang=en)
[Microsoft
 DirectX SDK](http://www.microsoft.com.sixxs.org/downloads/en/details.aspx?familyid=3021d52b-514e-41d3-ad02-438a3ba730ba)  (务必保证:
- 
Verify MSVC|Tools|Options|Projects and Solutions|VC++ Directories has Include: `$(DXSDK_DIR)include` and Lib: `$(DXSDK_DIR)lib\x86`.
- Make sure the DirectX include directories appear **first in the search path**, otherwise you may get build errors.
)
**然后如下所述**：
Create a working directory, enter it, and run:
**$ gclient config http://webrtc.googlecode.com/svn/trunk     (不能添加.sixxs.org)**
![](http://hi.csdn.net/attachment/201109/29/0_1317288239bB3G.gif)
 第一次会自动下载安装subversion和python.  此步会在新建的WebRTC目录下生成一个.gclient文件。
$ gclient sync --force
下载完毕后，即可生成各个Visual Studio2008工程文件。** 后续更新时也尽量采用gclient sync --force 而不是用subversion update.**
++++++
  参考文献：    c).创建webrtc工作目录，并在该目录下运行：
       gclient config https://webrtc.googlecode.com/svn/trunk
       gclient sync --force
       sync命令也会产生基于本地环境(Linux:make, OSX: XCode, Windows: Visual Studio)的build文件。
       也可以用命令：
       gclient runhooks --force
       对windows平台，在trunk目录下，已经有针对Visual Studio的工程文件：
       webrtc.sln
       好了，打开它，这就可以编译了。
       注：webrtc默认使用了C:/Program Files/Microsoft SDKs/Windows/v7.1/Samples/multimedia/directshow/baseclasses。我的机器上只有v7.0的sdk，于是构造了一个v7.1的目录，仅包含Samples/multimedia/directshow/baseclasses。
+++++++++++++++++++++++++++++++++++++++++++++++++++++
参考[Chrome在Visual
 Studio 2005下的编译过程](http://blog.csdn.net/xingtian713/article/details/4342003)可以了解google源代码的风格 
在下载Chrome代码前，需要安装下面几个软件：
1. 安装Visual Studio 2005.
2. 安装[Visual Studio 2005 Service Pack 1](http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=bb4a75ab-e2d4-4c96-b39d-37baf6b5b1dc).
3. 安装热补丁[Hotfix 947315](http://code.msdn.microsoft.com/KB947315/Release/ProjectReleases.aspx?ReleaseId=872).
4. 如果操作系统是Vista，还需要安装[Visual Studio 2005 Service Pack 1 Update for Windows Vista](http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=90e2942d-3ad1-4873-a2ee-4acc0aace5b6)。
5. 安装[Windows 2008 SDK](http://www.microsoft.com/downloads/details.aspx?FamilyId=E6E1C3DF-A74F-4207-8586-711EBE331CDC&displaylang=en)。按照网上说法，如果是Visual Studio 2008，就不需要安装这个了。
6. 配置Windows 2008 SDK。在开始->程序->Microsoft Windows SDK v6.1 > Visual Studio Registration > Windows SDK Configuration Tool.。选择make current按钮，幸运的话，应该能一次成功。如果不能成功，Chrome官方网站上有一个[手动配置](http://blog.csdn.net/xingtian713/article/details/4342003#TOC-Manually-registering-the-Platform-S)的帮助，大家可以参考。
## **下载 代码**
**Google为Chrome提供了一个部署工具[depot_tools](http://sites.google.com/a/chromium.org/dev/developers/how-tos/install-gclient)，包括下载代码、同步代码、上传代码等功能。这个工具采用Python编写的，其中还包含了一些Javascript脚本。Depot_tools中包含了一个gclient工具，是我们需要关注的重点。**
下载代码有几种方式：
1. Chrome官网上提供了一个[源代码包](http://build.chromium.org/buildbot/archives/chromium_tarball.html)，可以直接下载下来。不过这个包并不是最新的包。我采用的是这种方法进行下载的，相对来说比较快。
2. 采用SVN客户端工具进行下载，比如TortoiseSVN客户端工具，SVN服务器地址是http://src.chromium.org/svn/trunk/src。
3. 采用google提供的depot_tools工具。
l 下载和安装[depot_tools](http://sites.google.com/a/chromium.org/dev/developers/how-tos/install-gclient)。
l 把depot_tools的安装目录设置到系统目录（系统Path环境变量)中。
l 创建一个存放Chrome代码的目录，比如d:/chrome。目录不要包含空格。
l 在命令行下，先将当前目录切换到chrome代码的目录，例如上面的(d:/chrome)。
l 运行gclient config http://src.chromium.org/svn/trunk/src命令。Gclient将会先下载svn工具和python工具，然后调用svn进行代码同步。
注意：gclient中下载svn和python采用的是javascript实现。如果在需要设置proxy的环境中，则需要需要修改一下脚本。
++++++++
最后在线翻阅了chrome的开发group论坛，才知道Chrome确实作了修改，**原来代码中的那些****.sln、.vcproj文件全部抛弃了，google自己开发了一个脚本工具****[GYP](http://code.google.com/p/gyp/)工具，这个工具也是采用python编写的。[GYP](http://code.google.com/p/gyp/)****采用了自定义的一套规则，用于生成各种工程文件**。我们可以看一下下面这个gyp文件。
{
  'includes': [
    '../../build/common.gypi',
  ],
  'targets': [
    {
      'target_name': 'memory_watcher',
      'type': 'shared_library',
      'msvs_guid': '3BD81303-4E14-4559-AA69-B30C3BAB08DD',
      'dependencies': [
        '../../base/base.gyp:base',
      ],
      'defines': [
        'BUILD_MEMORY_WATCHER',
      ],
      'include_dirs': [
        '../..',
      ],
      'sources': [
        'call_stack.cc',
        'call_stack.h',
        'dllmain.cc',
        'hotkey.h',
        'ia32_modrm_map.cc',
        'ia32_opcode_map.cc',
        'memory_hook.cc',
        'memory_hook.h',
        'memory_watcher.cc',
        'memory_watcher.h',
        'mini_disassembler.cc',
        'preamble_patcher.cc',
        'preamble_patcher.h',
        'preamble_patcher_with_stub.cc',
      ],
    },
  ],
}
其实这个文件的内容和visual studio 2005中的.vcproj文件虽然有比较大的差别，但是描述的东西没有太大变化，更简单更简洁一些而已。无非描述了工程的文件，编译设置等内容。
下面描述一下编译步骤：
1. 运行命令行工具。
2. 切换到Chrome主目录下（我的电脑是d:/chrome目录）。
**3. 执行gclient runhooks --force。这个命令将会调用GYP工具，对Chrome.gyp进行解析，生成各个Visual Studio2005工程文件。**
4. 双击chrome/chrome.sln文件，即可打开Visual Studio 2005，一共有215个工程，非常庞大。
        右键选择解决方案，选择生成解决方案，编译开始了。这个过程在我的本本里是持续了2个小时左右
+++++
在我编译的过程中主要遇到的几个问题：
1. 公司有Visual Studio2005，可是因为Proxy的原因，depot_tools工具无法进行，为了突破proxy的限制，花了不少力气。
    2. Chrome工程组织方式发生了变化，但在官网上没有更新，网上其他的资料都是基于旧的版本介绍，所以在适应新的gyp方式方面花了不少力气。
+++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://hi.baidu.com/9856226/blog/item/fd2f3c4af0ed6afb83025c12.html](http://hi.baidu.com/9856226/blog/item/fd2f3c4af0ed6afb83025c12.html)
分析了一下gclient.bat，其实就是干两件事
    1）下载[http://src.chromium.org/svn/trunk/tools/third_party/svn_bin.zip](http://src.chromium.org/svn/trunk/tools/third_party/svn_bin.zip)
    2）下载[http://src.chromium.org/svn/trunk/tools/third_party/python/](http://src.chromium.org/svn/trunk/tools/third_party/python/) 这个目录
    原来里面有一段代码写反了，应该是先检查是否auto_updata:
    :: Shall skip automatic update?
    IF "%DEPOT_TOOLS_UPDATE%" == "0" GOTO :SKIP_UPDATE
    :: We can't sync if .\.svn\. doesn't exist.
    IF NOT EXIST "%~dp0.svn\." GOTO :SKIP_UPDATE
再去下载svn和python：
    :: Will download svn and python.
    :: If you don't want to install the depot_tools version of these tools, remove
    :: the 'force' option on the next command. The tools won't be installed only if
    :: not already in the PATH environment variable.
    call "%~dp0bootstrap\win\win_tools.bat" force
    if errorlevel 1 goto :EOF
    估计是他们的程序员大脑犯困，搞错了。换个顺序，逻辑就对了
    python那个可以自动下，就让他下吧。下好了之后貌似没出啥问题
