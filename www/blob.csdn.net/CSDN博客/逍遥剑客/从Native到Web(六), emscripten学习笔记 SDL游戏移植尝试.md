# 从Native到Web(六), emscripten学习笔记: SDL游戏移植尝试 - 逍遥剑客 - CSDN博客
2013年05月08日 23:26:43[xoyojank](https://me.csdn.net/xoyojank)阅读数：5487
大体浏览过emscripten的那一堆demo后, 心想试试移植个游戏试试, 顺便体验下这项技术的实用程度
首先尝试了Onescripter, 因为手头上有可以编译运行的FateStayNight. 
- 在工程配置中增加新的Platform: Emscripten, 并选择编译成可以执行的html
![](https://img-blog.csdn.net/20130508231828037)
- 编译提示缺少bzlib.h, 增加libbz2工程(开源), 注意这个就需要编译.bc库了(相当于.lib)
![](https://img-blog.csdn.net/20130508231836520)
- 再编译, 依赖库没问题了. 提示SDL_CD相关函数都找不到. 这也可以理解, web不方便访问光驱嘛. 使用#if EMSCRIPTEN宏隔离不支持的SDL特性代码
![](https://img-blog.csdn.net/20130508232015329)
- 编译通过, 可以运行了, 不过提示找不到脚本文件. 原来emscripten对于文件IO是有很大的限制的, 参考: [Filesystem Guide](https://github.com/kripken/emscripten/wiki/Filesystem-Guide)- 第一种比较简单粗暴, 把所有依赖的文件打包成一个.data文件, 载入页面时下载, 下载完再运行游戏. 不过这时候所有的数据都是在内存中的, 所以fopen什么函数其实访问的都是内存中的虚拟文件系统. 实验证明, 对于FateStayNight这种上GB资源的游戏来说, 这个方法会导致浏览器崩溃-_-
![](https://img-blog.csdn.net/20130508232116470)
- 第二种方式应该是floh在PPT中说的那种方式, 类似异步下载文件的方法. 由于只是做实验, 还没有去尝试
- 使用--preload-file链接选项把资源文件夹打包成.data先看看效果(使用了另一个比较小的游戏的资源: Tsukihime. 宅男们懂的)- 打开页面运行时没反应, Ctrl+Shift+J打开Chrome的调试窗口, 发现浏览器不允许访问file:///协议的路径
- ![](https://img-blog.csdn.net/20130508232045674)
- 上网找了个绿色的HTTP服务器小工具: HFS, 然后通过HTTP服务器访问这个页面, 终于可以读取文件了. 页面载入后会有一个进度条显示下载进度
![](https://img-blog.csdn.net/20130508232057268)
- 运行, 调试窗口显示Mix_XXXX的函数都没实现, NM, 有这么坑人的么......使用#if EMSCRIPTEN屏蔽之, 先不播放音效了
![](https://img-blog.csdn.net/20130508232107124)
- 再运行, 又提示找不到某些SDL_XXXX函数, 放弃!
![](https://img-blog.csdn.net/20130508232122533)
然后又尝试了sdlpal(大名鼎鼎的仙剑95), 也是编译没有问题, 运行时才提示找不到某个SDL函数的实现. 这个真无语, 找不到你还链接成功干嘛, 白激动了
两个SDL游戏都以失败告终. 看了下SDK里带的库, 好像也就OpenGL ES, OpenAL, SDL等. Bullet也有第三方的移植. 所以商用的话, 第三方库的选择比NaCl差很多
还是再等等看吧, 现在拿来也就是做个DEMO而已
所以说emscripten的SDL根本就不完整, 实际使用的话, 估计也就是替代一些窗口/输入消息什么的处理吧
Valve在GDC上提到移植到linux上的一些经验, 其中就说SDL是跨平台一种不错的替代器, 如果你懒得为每一种平台都实现一遍程序框架, 窗口, 键盘鼠标消息什么的, 就用SDL吧
另外, 在运行时发现, FireFox比Chrome的加载速度快好多. 果然是Mozilla自家孩子亲啊~
