# 如何选择VC界面库产品？（三）— DirectUI 界面库介绍 - xqhrs232的专栏 - CSDN博客
2012年08月22日 22:01:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2031
原文地址::[http://www.uieasy.com/cn/blog/?p=479](http://www.uieasy.com/cn/blog/?p=479)
客户经常来信询问该如何选择界面库产品： 
“你们的产品跟其他同类产品相比有什么优势？” 
“你们的DSkinLite界面库跟DirectUI界面库有什么不同，哪个产品更适合我们公司？” 
上一篇文章： 
[如何选择VC界面库产品（二）–- DSkinLite界面库介绍](http://www.uieasy.com/cn/blog/?p=473)
    本文主要介绍UIEASY DirectUI库界面产品的特点及优势。
## ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb.png) UIEASY DirectUI界面库产品
DirectUI界面库背景知识 
        DirectUI界面库使用XML来描述界面风格，界面布局，使用脚本语言（如vbscript，javascript，lua）等来作为界面元素和程序逻辑的交互。从这方面来看，DirectUI界面库可以看成是一个迷你的微软WPF，mozilla XUL，或者Adobe的Flex，这些平台代表的应用程序的发展方向，同时也有很多相似的架构，比如多采用类似XML的方式描述定义界面，使用脚本语言 （xul使用javascript，flex使用Action Script）来作为控制界面元素并和程序逻辑之间交互。
        DirectUI 界面库采用相似的设计，使用xml描述界面，同时使用javascript
 / vbscript来作为界面和逻辑的粘合。在控件设计方 面，DirectUI界面库更多的融入了XUL，WPF，Flex等控件设计理念，并没有一味的照搬window标准控件，以期望其成为一款真正使用实用的界面库。
UIEASY DirectUI界面库 
    UIEASY于2008年正式推出DirectUI界面库产品，是国内外最早推出的DirectUI类界面产品。DirectUI界面库深受欢迎，同时其首创的界面设计思路影响了众多国内外界面开发人员，目前大多数国内外DirectUI类型界面产品大多都采用了我们的界面设计思路。
     UIEASY DirectUI界面库产品不仅是一款界面库产品，也是一种全新的界面库设计框架。如果您是界面开发人员，无论您否有意选择我们的产品，都可以下载我们的产品，了解我们的产品设计思路，相信会对您的界面开发带来全新的思路。
## ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb.png)UIEASY DirectUI界面库有以下主要特点：
### XML配置界面
支持XML配置应用程序界面的虚拟控件及Win32控件，同时可以布局界面。在DirectUI界面库中，界面元素分为虚拟控件（Windowless 控件）和Win32控件。如下图所示：
![image](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb8.png)
可在XML中定义Windowsless控件，Win32 控件，并定义界面布局；同时也可以在XML中定义控件元素风格。
### 事件与脚本
          支持脚本语言。通过使用脚本语言来实现界面元素和程序逻辑之间交互，可以简化代码的编写与调试。 
- 为什么需要脚本语言？ 
       DirectUI界面库内部会创建一些虚拟控件，如label，button，checkbox等，这些虚拟控件主要用以显示界面和响应事件。同时我们在程序中，也需要改变这些控件属性，如显示/隐藏，位置变化等。如果像MFC等框架那样处理，则应用程序需要直接操作这些虚拟控件类，造成程序逻辑与界面操作深度交叉，违背了我们将程序与脚本分离的初衷；同时应用程序将依赖一系列新的UI类，程序变得复杂，也带来不稳定性。 
      鉴于这种情况，很多界面框架都引入脚本，用以控制界面元素并响应事件。在UIEASY的DirectUI界面库中，我们也引入的脚本控制。每个界面控件都会对应一个脚本对象，这样开发人员可以在脚本中操作此控件。 
      通过脚本的引入，内部虚拟控件对应用程序来说是不可见的。这样极大的减少应用程序对界面库的依赖，简化了程序。
- 脚本控制界面控件属性 
     脚本一个重要作用就是控制界面控件的属性。如下脚本代码： 
|`01`|`function``Window::OnInit()`|
|`02`|`{`|
|`03`|`   ````edit1.title =``````"DirectUI is powerful gui library! "````;`|
|`04`|`   ````edit2.title =``````"DirectUI is powerful gui library!"````;`|
|`05`|`   ````search_edit2.title =``````"DirectUI is powerful gui library!"````;`|
|`06`|`   ````win32button1.title =````"NotifyScript1"``;`|
|`07`|`   ````win32button2.title =````"NotifyScript2"``;`|
|`08`||
|`09`|`}`|
|`10`|`function``Button2::onClick(cx,cy)`|
|`11`|`{`|
|`12`|`    ````//Hide the CheckBoxGroup```|
|`13`|`    ``if````(progress1.value >= 100)```|
|`14`|`       ````progress1.value = 0;```|
|`15`|`    ``else`|
|`16`|`       ````progress1.value = progress1.value + 10;```|
|`17`||
|`18`|`    ``if````(progress2.value >= 100)```|
|`19`|`       ````progress2.value = 0;```|
|`20`|`    ``else`|
|`21`|`       ````progress2.value = progress2.value + 10;```|
|`22`||
|`23`|`    ``if````(progress3.value >= 100)```|
|`24`|`       ````progress3.value = 0;```|
|`25`|`    ``else`|
|`26`|`       ````progress3.value = progress3.value + 10;```|
|`27`|`}`|
以上代码在相应的程序中，设置控件edit title，设置progress控件value。 在DirectUI界面库中，脚本支持控制虚拟控件属性，也支持控制Win32控件属性。
3.    响应来自虚拟控件的事件
        虚拟控件为DirectUI创建的逻辑控件，应用程序无法获得其事件。这样我们通过脚本将将相应的事件传递出来，发送到应用程序中。如以下代码：
|`1`|`function````linkSkinCmd1::onClick(cx, cy)```|
|`2`|`{`|
|`3`|`   ``var````str =````"[http://www.uieasy.com](http://www.uieasy.com/)"``;`|
|`4`|`   ````Window.OnSkinCmd (101, str);```|
|`5`|`}`|
以上代码相应linkSkinCmd1虚拟控件的click消息，并通知应用程序。
4.  响应来自应用程序的事件
     在实际程序开发中，有些时候需要根据程序逻辑来控制界面虚拟控件的属性等。这种需求也通过脚本来实现。如以下代码：
|`01`|`function````Window::OnNotify(code, var1)```|
|`02`|`{`|
|`03`|`    ``if````(code == 1000)```|
|`04`|`    ``{`|
|`05`|`       ``if````( Button1.title ==````"HideCheck"``)`|
|`06`|`       ````Button1.title = var1;```|
|`07`|`       ``else``if````( Button1.title ==``````"New Text"````)`|
|`08`|`       ````Button1.title =````"HideCheck"`|
|`09`|`    ``}`|
|`10`|`    ``else``if````( code == 1001)```|
|`11`|`    ``{`|
|`12`|`      ``if````(slider1.pos >= 100)```|
|`13`|`       ````slider1.pos = 0;```|
|`14`|`      ``else`|
|`15`|`       ````slider1.pos = slider1.pos + var1```|
|`16`|`    ``}`|
|`17`|`}`|
以上代码根据程序发送过来的消息，设置虚拟控件的titile，slider控件的pos等。
下一篇文章：
[如何选择VC界面库产品？（四）— DSkinLite vs DirectUI](http://www.uieasy.com/cn/blog/?p=486)
