# 传统3D游戏引擎的Web化 - 逍遥剑客 - CSDN博客
2012年10月24日 15:37:14[xoyojank](https://me.csdn.net/xoyojank)阅读数：5852
最近[floh](http://flohofwoe.blogspot.com/)大神的blog又复活了, 现在n3的主要发展发向是web化
其实也是跟PC和主机平台的衰落有关, 各种移动设备正在逐渐取代PC的地位
而做为一个3D游戏引擎, 想要适应这个潮流却不是很容易
先不考虑其它, 光技术上面临的问题就很棘手:
- C++目前无法直接用于web开发, 必然要面临老代码转换/移植的问题
- 3D硬件加速的渲染在浏览器这边, 没有一个统一的标准
- 多线程, web浏览器里对于线程的使用有很多限制
- 输入设备, 比如右键的功能很多时候会跟浏览器冲突
- IO, 通过网络传输的资源在传输, 访问权限等上也有限制
- 性能问题, 脚本语言的速度虽说已经很快的, 但还是没有Native快
 就目前来说, C++的3D引擎勉强能用的解决方案大概有5种:
- Microsoft IE ActiveX
- - 平台支持: IE(Win). 本身设计有问题, 不被其它平台支持
- 优势: 相当于把现有的引擎换个窗口, 代码改动最小, 性能几乎没损失
- 缺点: 需要安装插件, 没认证过的会被大多数安全软件拦截, 不如直接做成微端
- 代表: Unity3D, 山寨(或叫自研)3D引擎开发的游戏
- Sun Java JNI
- - 平台支持: Win/Mac/Linux. 不过需要每个平台都做一个Native版本
- 优势: 浏览器兼容性好, 相当于一个通用的ActiveX
- 缺点: 操作系统跨平台需要额外工作. 需要安装Java, 并且开启浏览器支持
- 代表: Drakensang Online
- Adobe FlasCC
- - 平台支持: Win/Mac/Linux. 需要Flash11+, 把C++编译成AS能用的库
- 优势: 平台兼容性目前最好
- 缺点: 渲染器需要重写, 而且是Adobe自己的一套标准. 另外, 想要性能高? 买license!
- 代表: Epic Citadel(只是demo而已)
- Google Native Client
- - 平台支持: Chrome(Win/Mac/Linux). 浏览器限制住了...
- 优势: 与Native的性能差距不大, 而且IDE与Debugger目前也有了(VS2010)
- 缺点: 虽然看上去很完美, 但不被其它势力所支持
- 代表: OGRE, Bullet, Unity3D...Google Web Store上有不少移植好的游戏了
- Mozilla Emscripten
- - 平台支持: FireFox/Chrome(Win/Mac/Linux).  把C++编译成JS
- 优势: HTML5符合未来的发展趋势
- 缺点: WebGL不被Microsoft支持. 性能目前损失比较大, SDK也不是非常成熟
- 代表: BananaBread
值得注意的是, Adobe/Google/Mozilla这些都是基于LLVM搞的, 看来这也是一个好玩的玩意儿
暂时总结这么多, 很多观点是具有时效性的, 明年说不定就大变样了
做为游戏开发者, 还是要做好准备, 免得out了, HOHO~
注: 本文仅限于C++开发的3D游戏引擎, 不包括各种XNA/Sliverlight的3D游戏或是Flash/HTML5的2D游戏
