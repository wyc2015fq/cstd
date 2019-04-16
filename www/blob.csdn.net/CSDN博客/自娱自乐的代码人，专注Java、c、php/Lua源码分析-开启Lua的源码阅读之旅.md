# Lua源码分析 - 开启Lua的源码阅读之旅 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2018年11月26日 17:43:41[initphp](https://me.csdn.net/initphp)阅读数：395
个人分类：[Lua源码分析](https://blog.csdn.net/initphp/article/category/8446441)









很久很久没有写博客了，一直忙于工作和项目，最近依然想静下来阅读一些好的源码。自从读完了Nginx和Memcache的源码后，对服务器端的基础开源软件的实现原理有了一定的认识，接下来想看看Lua这门精巧的语言是如何实现的。

开启Lua源码阅读之旅。

Lua是一门用C语言编写的脚本语言，一共1w多行代码，非常的轻巧，适合做web脚本、游戏脚本、物联网等场景下使用。

Lua的下载地址：[http://www.lua.org/](http://www.lua.org/)

源码包下载后，我们可以看一下lua-5.3.5/src目录下的代码结构。代码结构基本会分3部分

1. 虚拟机核心功能部分
|文件|作用|
|----|----|
|lua.c|lua的可执行入口 main函数|
|lapi.c|C语言接口|
|ldebug.c|Debug 接口|
|ldo.c|函数调用以及栈管理|
|lfunc.c|函数原型及闭包管理|
|lgc.c|垃圾回收机制|
|lmem.c|内存管理接口|
|lobject.c|对象操作函数|
|lopcodes.c|虚拟机字节码定义|
|lstate.c|全局状态机 管理全局信息|
|lstring.c|字符串池|
|ltable.c|表类型的相关操作|
|ltm.c|元方法|
|lvm.c|虚拟机|
|lzio.c|输入流接口|

2. 源代码解析和预编译
|文件|作用|
|----|----|
|lcode.c|代码生成器|
|ldump.c|序列化预编译的Lua 字节码|
|llex.c|词法分析器|
|lparser.c|解析器|
|lundump.c|还原预编译的字节码|

3. 内嵌库
|文件|作用|
|----|----|
|lauxlib.c|库编写用到的辅助函数库|
|lbaselib.c|基础库|
|ldblib.c|Debug 库|
|linit.c|内嵌库的初始化|
|liolib.c|IO 库|
|lmathlib.c|数学库|
|loadlib.c|动态扩展库管理|
|loslib.c|OS 库|
|lstrlib.c|字符串库|
|ltablib.c|表处理库|

每次阅读源码，其实最难的是开始，通过网上各种资料，先把lua的整个目录结构弄明白，幸好lua真的比较小，很容易就能弄明白每个文件是干什么的。接下去就是开始一点一点的啃整个源码的过程了。

啃整个lua语言链路解析过程之前，我会优先把lua周边的库以及虚拟机字节码这块搞明白，然后再开始进行整个解析流程的阅读。







