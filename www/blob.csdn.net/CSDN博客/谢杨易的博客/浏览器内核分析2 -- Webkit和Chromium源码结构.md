
# 浏览器内核分析2 -- Webkit和Chromium源码结构 - 谢杨易的博客 - CSDN博客

2017年02月15日 16:33:50[谢杨易](https://me.csdn.net/u013510838)阅读数：8143标签：[Blink																](https://so.csdn.net/so/search/s.do?q=Blink&t=blog)[renderer																](https://so.csdn.net/so/search/s.do?q=renderer&t=blog)[browser																](https://so.csdn.net/so/search/s.do?q=browser&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=renderer&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=Blink&t=blog)个人分类：[前端																](https://blog.csdn.net/u013510838/article/category/6726259)
[
																								](https://so.csdn.net/so/search/s.do?q=Blink&t=blog)



# 1 Webkit架构
Webkit和Blink代码量都是几百万行级别，阅读难度很大，故先了解下它的大体架构是十分有意义的。下面是《Webkit技术内幕》一书中给出的架构图
![这里写图片描述](http://s9.51cto.com/wyfs02/M02/30/88/wKiom1OoFoXiajoAAACtCpO9nQU069.jpg)
由图中可见，大体分为以下几个层次
1. 操作系统：WebKit可以在不同OS上运行，如mac OS，Windows，Linux等。
2. 三方库：WebKit依赖了很多三方库，如音频，视频等
3. WebKit：大概分为两个部分
`1）WebCore：与平台无关，各个浏览器移植所共享。包含HTML解析，CSS解析，渲染，调试等部分
2）JavaScript引擎：WebKit中默认使用JavaScriptCore引擎，Blink中使用V8引擎，二者的区别后面文章会详细介绍
2）WebKit ports：平台相关，需要浏览器根据硬件平台做更改。包含网络栈，音视频，GPU等部分`4. WebKit接口层：提供接口给浏览器应用调用
# 2 Webkit源码结构
Webkit源码结构还是十分清晰的，基本上每个模块对应一个子目录。主要代码在Source/目录下，如下
WebCore/: 平台无关，各个不同移植所共享
css/: CSS解析器
dom/: DOM解析器
html/: HTML解析器
inspector/: 调试相关
loader/: 资源加载模块
page/: 与页面相关的全局对象，如window，navigator，DOM事件，动画等
storage/: cookie处理和HTML5中的LocalStorage处理
JavaScriptCore/：Webkit中默认的JS引擎，Blink中改为V8
WebKit/ WebKit2/：嵌入式接口层
WTF/：基础类库，如字符串操作，线程，算法等
3 Chromium浏览器架构
首先明确一个概念，Chromium和Chrome一样，是一个浏览器，他们都是基于Blink内核的。二者区别如下
Chromium可以看做是Chrome的试验版本，加入了很多比较激进的特性。Chrome是发行版本，待Chromium稳定后再发布
Chromium发版周期很快，而Chrome比较稳定
Chrome中多了Google的一些服务，并有自动更新功能
Blink中采用了多进程架构，它的优点如下
避免单个page crash影响整个浏览器
避免第三方插件crash影响整个浏览器
多进程充分利用多核优势
方便使用沙盒模型隔离插件等进程，提高浏览器稳定性
Blink中包含的主要进程如下
Browser进程：主进程，只有一个。它的作用有
负责浏览器界面显示，与用户交互。如前进，后退等
负责各个页面的管理，创建和销毁其他进程
将Renderer进程得到的内存中的Bitmap，绘制到用户界面上
网络资源的管理，下载等
Renderer进程：默认每个页面一个，互不影响。主要作用为解析HTML，CSS，构建DOM树和RenderObject树，布局和绘制等。
第三方插件进程：每种类型的插件对应一个进程，仅当使用该插件时才创建
GPU进程：最多一个，用于3D绘制等
Browser和Renderer通信过程
![Markdown](http://p1.bpimg.com/1949/fd50282ef55e2941.png)
Browser进程收到用户请求，首先UI线程处理，转交给IO线程，随后通过RendererHost接口转交给Renderer进程
Renderer进程的Renderer接口收到消息，IO线程简单处理后，交给渲染线程，进行HTML解析和DOM树构建，CSS解析，JS执行，RenderObject树构建，布局和绘制等过程，生成用户可见区域（ViewPort）的Bitmap。最后通过共享内存方式IPC给Browser进程
Browser进程使用Bitmap内存在界面上绘制出图像。

