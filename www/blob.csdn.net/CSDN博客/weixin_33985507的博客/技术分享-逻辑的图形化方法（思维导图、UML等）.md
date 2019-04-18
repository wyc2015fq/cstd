# 技术分享-逻辑的图形化方法（思维导图、UML等） - weixin_33985507的博客 - CSDN博客
2016年11月10日 23:51:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
### 写在前头
- 本文编辑于**2016年11月10日**，请注意技术的时效性
- 请带着批判的态度阅读
#### 序言
读完本篇文章，你将看到Google绘图、思维导图的运用，了解UML 类图、时序图、状态图的符号含义，还有QQ截图的绘图功能。
目的是能帮助你在框架设计、阅读源码、逻辑描述、业务事项中，多一种简单的表达方式。
##### 一，Google绘图
我用[Google绘图](https://link.jianshu.com?t=https://chrome.google.com/webstore/detail/google-drawings/mkaakpdehdafacodkgkpghoibnmamcme?hl=zh-CN)的场景一般是用于框架设计、架构理解。常用来描述模块间的依赖关联。
随手贴一个示例（**本图内容来源于<深入理解Java虚拟机>一书**）：
![1914449-6a83afe274bcc230.png](https://upload-images.jianshu.io/upload_images/1914449-6a83afe274bcc230.png)
Java虚拟机运行时数据区
再比如我写的demo中网络框架的设计：
![1914449-4cd589030beaf6a3.png](https://upload-images.jianshu.io/upload_images/1914449-4cd589030beaf6a3.png)
Ok 网络框架
看看它的工具栏，一般的框架设计、流程图，用这个工具绘画，还是挺美观的，除了它要翻墙...
![1914449-e9f12b052eda5b49.png](https://upload-images.jianshu.io/upload_images/1914449-e9f12b052eda5b49.png)
Paste_Image.png
##### 二，思维导图
思维导图，很多人在用了，功能介绍，我也不说了， 我常用的工具是[XMind 7](https://link.jianshu.com?t=http://www.xmind.net/)
我主要用思维导图的场景，一般是理清需求列表，整理待办事项，以及技术选型。
也丢一个例子，可耻的丢一个自己的文章。
![1914449-c0f9682c26f4fe90.png](https://upload-images.jianshu.io/upload_images/1914449-c0f9682c26f4fe90.png)
技术选型
思维导图，除了提炼逻辑以外，会比文字描述来得更清晰，甚至可以使用自定义图标来加强阅读体验。
##### 三，UML统一建模语言
这个工具--[StarUML](https://link.jianshu.com?t=http://staruml.io/)，我从大学开始使用的，一开始是学校课程，后来发现挺好用的，所以一直沿用了。
这个东西，其实是我用得最多的，不过比起它一堆规范，我往往用得很随性，比如什么包含啊、聚合啊、太多概念规则，我是不理会的。
因为限制于概念，反而会增加了绘画的难度；
其中，我用得最多的是类图、时序图。偶尔有用到状态图，其他图从来没用过。。。
###### 3.1 类图
类图，描述类之间的关系。
举个例子：（**以下内容摘自<大话设计模式>一书**）
![1914449-02453dc79ac6b74b.png](https://upload-images.jianshu.io/upload_images/1914449-02453dc79ac6b74b.png)
类图学习
是不是看着有点复杂，我换一个简单点的应用场景：
![1914449-beceb4c06f049d8a.png](https://upload-images.jianshu.io/upload_images/1914449-beceb4c06f049d8a.png)
一个计算器类的实现
常用的是继承关系，实现关系，关联关系。
再给个工具栏的示意图（*有标注的是常用的，没标注的 我从来没用过...*）：
![1914449-808eb0cc79540dcd.png](https://upload-images.jianshu.io/upload_images/1914449-808eb0cc79540dcd.png)
StarUml中的工具栏示意图
###### 3.2 时序图
时序图，描述一个操作的执行顺序。
可耻的再拿自己的图片：
![1914449-3ade27573e2ef6f4.png](https://upload-images.jianshu.io/upload_images/1914449-3ade27573e2ef6f4.png)
时序图示例
其中工具栏的示意图如下：
![1914449-8e394af630a84e0c.png](https://upload-images.jianshu.io/upload_images/1914449-8e394af630a84e0c.png)
时序图工具栏示意图
而对于调用的线条展示效果，在右边的属性框中：
![1914449-7469a95765ba0fec.png](https://upload-images.jianshu.io/upload_images/1914449-7469a95765ba0fec.png)
修改线条的展示效果
###### 3.3 状态图
状态图，用于描述一个对象在不同时期的不同表现。
状态图的应用场景，我们用线程的五态图来表示：
![1914449-85ea74e5d1c6d026.png](https://upload-images.jianshu.io/upload_images/1914449-85ea74e5d1c6d026.png)
线程的五态图
其中工具栏的示意图如下：
![1914449-9702c6c79d4a825a.png](https://upload-images.jianshu.io/upload_images/1914449-9702c6c79d4a825a.png)
状态图的示意图
##### 四，QQ、微信的截图功能
很多人都安装了QQ、微信的PC端，但是好像不是很多人知道截图上是有绘画功能的，废话不多说，上图：
![1914449-87d2d3096b4dacf9.png](https://upload-images.jianshu.io/upload_images/1914449-87d2d3096b4dacf9.png)
QQ截图的特殊功能
##### And Then~
[StarUml 百度云盘](https://link.jianshu.com?t=http://pan.baidu.com/s/1miNNFRy)
[Xmind官网](https://link.jianshu.com?t=http://www.xmindchina.net/)
