# gclient多源码管理工具 DEPS文件 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年10月26日 16:58:44[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：112
所属专栏：[webRTC](https://blog.csdn.net/column/details/30214.html)
gclient来管理源码的checkout, update等。 gclient是google专门为这种多源项目编写的脚本，它可以将多个源码管理系统中的代码放在一起管理。甚至包括将[Git](http://lib.csdn.net/base/git)和svn代码放在一起。
重点说明下和gclient密切相关的两类文件.gclient和DEPS。
.gclient文件是gclient的控制文件，该文件放在工作目录的最上层。是主文件， ".gclient"文件是一个[Python](http://lib.csdn.net/base/python)的脚本
它指定以下变量
solutions：一组字典，用于指定要获取的项目
此文件在src目录的同级目录。
- target_os：一个可选的（目标）操作系统数组，用于获取特定于操作系统的依赖项。
solutions数组中描述的每个项目都可以包含一个可选的DEPS文件。.gclient文件是gclient config <url>手动生成的。
solutions下面又有下面这些值：
- name: the path of the checkout.
- url: the remote repository to fetch/clone.
- url : 源码所在的目录，gclient希望checkout出的源码中包括一个DEPS的文件，这个文件包含了必须checkout到工作目录的源码的信息；
- deps_file 这是一个文件名（不包括路径），指在工程目录中包含依赖列表的文件，该项为可选，默认值为"DEPS"
- custom_deps 这是一个可选的字典对象，会覆盖工程的"DEPS"文件定义的条目。一般它用作本地目录中，那些不用checkout的代码，如
- "custom_deps": {  
-   "src/content/test/data/layout_tests/LayoutTests": None,  
-   "src/chrome/tools/test/reference_build/chrome_win": None,  
-   "src/chrome_frame/tools/test/reference_build/chrome_win": None,  
-   "src/chrome/tools/test/reference_build/chrome_linux": None,  
-   "src/chrome/tools/test/reference_build/chrome_mac": None,  
-   "src/third_party/hunspell_dictionaries": None,  
- },  
         或者让本地目录从不同位置checkout一个新的代码出来,或者checkout不同的分支、版本等。也可以用于增加在DEPS中不存在的新的项 目
在每个checkout出的工程中，gclient期望发现一个DEPS文件，它定义了工程不同部分都是如何checkout出来。
“DEPS”也是一个python脚本，最简单的，如下：
DEPS文件指定项目的依赖项，它指定以下变量：
deps: a dictionary of child dependencies to fetch要获取的子依赖项
- deps = {  
-   "src/outside" : "http://outside-server/trunk@1234",  
-   "src/component" : "svn://svnserver/component/trunk/src@77829",  
-   "src/relative" : "/trunk/src@77829",  
- }  
deps的每个条目都包含一个key-value对，key是被checkout的本地目录，而value就是对应的远程URL。
如果路径是以'/'开头的，那么它是一个相对URL，相对与.gclient中URL地址。
hooks: hooks to run after a sync.
- Var(): replace this string with a variable defined in vars or overridden.
将此字符串替换为在vars中定义或覆盖的变量
[http://blog.csdn.net/doon/article/details/9287693](http://blog.csdn.net/doon/article/details/9287693)
