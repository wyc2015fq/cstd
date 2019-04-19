# 如何选择VC界面库产品？（二）– DSkinLite界面库介绍 - xqhrs232的专栏 - CSDN博客
2012年08月22日 21:59:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1776
原文地址::[http://www.uieasy.com/cn/blog/?p=473](http://www.uieasy.com/cn/blog/?p=473)
客户经常来信询问该如何选择界面库产品： 
“你们的产品跟其他同类产品相比有什么优势？” 
“你们的DSkinLite界面库跟DirectUI界面库有什么不同，哪个产品更适合我们公司？”
前面的三篇文章分别介绍了：
[如何选择VC界面库产品（一）– 选择界面库产品需考虑的因素](http://www.uieasy.com/cn/blog/?p=462)
本文主要介绍UIEASY DSkinLite库界面产品的特点及优势。
## DSkinLite界面库产品
      DSkinLite 是一款轻量级的C++界面库，未使用复杂的Hook操作，仅使用替换窗口过程的方式（SubclassWindow）来处理控件界面绘制。使用XML文件管理GDI资源如颜色，字体，图片，并描述界面构成，最大程度的将界面和程序逻辑分离，同时为软件皮肤更换，换色等提供基础支持。 
      同时UIEASY首次创造性的将界面构成元素抽象为线条元素，矩形元素，图片元素，文本元素，并提供相应规则来使用这些元素“组合” 界面。这极大的提高了界面库产品的灵活性，使得界面库产品可以随意构造出多种多样的控件界面。 
      DSkinlite融合了DirectUI元素并提供多款自绘控件。
    DSkinLite秉承UIEASY界面产品设计理念- 高效，简单，灵活，稳定。拥有以下显著特点：
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb1.png) 简单易用
         DSkinLite界面库API及XML配置语法简单，开发人员一般可以在2-3天内熟悉使用方法。DSkinLite界面库API共20个左右，常用API函数应该在5左右。同时DSkinLite采用C++编写，专为Visual Studio开发者设计，开发者可以很容易的将DSkinLite整合到已有软件工程中或者迅速开发新的软件。 
         DSkinLite使用XML配置界面样式，定义界面资源（字体，颜色，图片）等。借助于XML的灵活的语法，可以描述多种多样的控件界面风格。因此不管您是否有意选择DSkinLite界面产品，您都可以下载我们的使用试用版，了解DSkinLite界面库的设计风格，相信会让您了解一种全新的界面产品设计理念，给您的界面开发带来启发。
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb2.png) 界面配置灵活
      界面配置灵活。DSkinLite界面库采用XML管理GDI元素，并独创了将界面元素抽象为图片，矩形，线条，文字等元素。任何一个控件界面均可以由这些元素来组合，使用DSkinLite可以轻松配置各种界面效果。如下图所示：
![image](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image17_thumb.png)
由此在一个控件界面中，可以灵活的配置界面元素，可以满足绝大多数界面设计需要。
动态的控制界面元素。同时DSkinLite提供相应的API可以控制这些元素（image，text，rect）的显示/隐藏，同时修正某些属性，如image的picfile属性即更换图片，text的content属性即文本内容。这种界面需求在界面开发中十分常见。如：
![image](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb7.png)
|`01`|`if````( bVoice )```|
|`02`|`{`|
|`03`|`    ````dsSetDrawItemVisible( GetDlgItem( IDC_BUTTON8)->GetSafeHwnd(), 101,101, FALSE, FALSE);```|
|`04`|`    ````dsSetDrawItemVisible( GetDlgItem( IDC_BUTTON8)->GetSafeHwnd(), 100,100, TRUE, TRUE);```|
|`05`|`    ````//Because there are tow calls for dsSetDrawItemVisible,```|
|`06`|`    ````//it only need redraw the window after the last time.```|
|`07`|`    ````bVoice = FALSE;```|
|`08`|`}`|
|`09`|`else`|
|`10`|`{`|
|`11`|`    ````dsSetDrawItemVisible( GetDlgItem( IDC_BUTTON8)->GetSafeHwnd(), 101,101, TRUE, FALSE);```|
|`12`|`    ````dsSetDrawItemVisible( GetDlgItem( IDC_BUTTON8)->GetSafeHwnd(), 100,100, FALSE, TRUE);```|
|`13`|`    ````bVoice = TRUE;```|
|`14`|`}`|
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb3.png) 控件支持全面
    DSkinLite除支持一般控件之外，还完整支持各种控件的内置滚动条。同时还采用可以配置listctrl，listbox，treectrl等控件的item内容的绘制，无需额外编写绘制代码。具体请参见：
[Item控件（List control, List Box, Tree
 Control）绘制(一)–DrawItem配置](http://www.uieasy.com/cn/blog/?p=296)
      同时针对Win32控件本身的一个局限，DSkinLite中加入了一些自绘的控件，来解决使用标准Win32控件无法解决的界面需求。详细了解，WidgetTree自绘控件，WidgetTab自绘控件。
[DSkinLite 新特性– 自绘树形控件(QQ好友列表)](http://www.uieasy.com/cn/blog/?p=183)
[DSkinLite新特性—WidgetTab](http://www.uieasy.com/cn/blog/?p=371)
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb3.png)高效图形绘制模块
    DSkinLite未使用GDI+等图形库，使用针对公司开发的图形库来绘制界面，图形库对界面编程优化并扩展；支持各种paintmode（参考[DSkinLite
 使用技巧(六) – 绘制元素](http://www.uieasy.com/cn/blog/?p=391)），同时支持png，bmp，gif, jpeg等图形格式。
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb3.png)与其他产品组成界面解决方案
     DSkinLite界面库周边产品完善。
DSkinControl( For MFC ) 是一款MFC扩展控件库，同时使用DSkinLIte处理界面绘制。了解更多关于DSkinControl 控件库：
[UIEASY 控件库DSkinControl ( For MFC) V1.0
 发布](http://www.uieasy.com/cn/blog/?p=359)
[DSkinControl( For MFC) 新控件
 StackWnd](http://www.uieasy.com/cn/blog/?p=378#more-378)
ekRichEdit控件为一款RichEdit扩展控件，主要对微软RichEdit控件做扩展，实现显示图片动画，文本，超链接等，满足Windows客户端软件对富文本控件的需求。同时我们针对IM等聊天窗口中的需求，封装相应的类来支持；可以满足IM软件对RichEdit的基本需求。了解更多关于ekRichEdit控件：
[RichEdit扩展控件–ekRichEdit简介](http://www.uieasy.com/cn/blog/?p=419)
[RichEdit扩展控件– ekRichEdit API 说明](http://www.uieasy.com/cn/blog/?p=426)
    DSkinLite界面库与这些控件产品完全兼容，可以同时在软件产品中使用。
### ![image_thumb10](http://www.uieasy.com/cn/blog/wp-content/uploads/2010/11/image_thumb10_thumb3.png)产品稳定
    DSkinLite界面库产品自2008年推出以来，先后经历三次大的版本发布，并持续更新至今，最新版本为V3.7.5.3。未来还将有更多的新特性加入DSkinLite。
     DSkinLite产品从推出以来，受到市场广泛好评。企业客户遍及中国，韩国，日本，美国，印度，欧洲。欢迎您与我们联系，索取成功案例信息。
## DSkinLite详细功能特性
界面库统一特性：
|功能特性|详细描述|
|----|----|
|界面色调动态调整|支持界面整体换色调，同时可自由控制图片，文字，控件等是否换色。可通过HSL和color两种方式来更换色调。|
|多种皮肤加载方式|支持从文件夹，压缩文件，VC资源三种方式加载皮肤|
|全面支持滚动条|除支持list box, edit,rich edit, list control, tree control等滚动条外，也支持windows内置的滚动条。|
|皮肤文件打包|可使用我们提供的SkinBuilder工具，将皮肤文件整体打包。|
|高速图片解析库|支持多种图片格式支持bitmap，png带透明通道，gif，jpeg等格式。采用公司研发的图片解析库，针对界面编程做了诸多优化和扩充。|
|支持多个开发平台|支持MFC，ATL，WTL，win32等多个开发环境，同时支持VC6-VS 2010 多个平台。|
|静态库|可提供VS 2003 – 2010 各个平台下的静态库版本|
|Unicode|支持Unicode/多字节开发环境|
|支持控件自绘|提供控件背景自绘功能，用户可以DSkinLite绘制控件前或者绘制控件后，根据提供的接口绘制控件界面。同时支持list control，listbox，tree control等控件自绘功能。|
|动态控制界面显示效果|可以根据程序逻辑动态控制控件界面的显示效果，如更换控件背景，控件文本等。|
|支持动画显示|支持动画显示，并通过API控制动画显示。支持gif格式，和多张png图片格式组成的动画。|
|自定义数据支持|可以在XML中配置控件的自定义的属性。例如：控件大小，行高以及每行的显示内容等，这样可以大大的方便换肤以后的整体的效果。|
|动态加载xml配置文件|完全支持插件式开发。在插件式界面开发中，某些界面可能是后来通过升级等方式添加的功能模块，DSkinLite可以动态加载这些插件界面的xml配置文件。|
|支持控件、子窗口透明|DSkinLite高效支持大部分控件，子窗口透明，以及多层窗口透明。|
|ToolTips支持|可以自由在XML定义控件tooltips提示语，也可以通过函数dsSetToolTips动态设置。|
|标题栏添加控件|支持在窗口标题栏区域，添加控件。|
|支持美化的MessageBox|DSkinLite通过API dsMessageBox支持MessageBox的美化，您可以完全配置MessageBox窗口背景，提示icon图标等。|
控件类型支持
|功能特性|详细描述|
|----|----|
|支持标准控件|支持static，button，checkbox,radiobox,edit,listbox,list control, tree controlcombobox, header control, group box, slider,spin, tab control, progress,toolbar control.menu, richedit,datetime control.|
|支持Split button|Split button，是指一个button有两部分，左右均可响应事件。|
|完全支持Menu的绘制|支持Menu背景，item绘制，同时支持设置item图标ico。|
|支持Item绘制|支持根据XML定义list control,tree control, list box等item的风格，并绘制item。同时支持定义eventitem响应事件。|
|[dsTreeControl自绘控件](http://www.uieasy.com/cn/blog/?p=183)|dsTreeControl（WidgetTree）控件是一款类似Tree control的树形控件，主要支持一些标准树形控件不能完成的功能，如设置不同item的不同高度。|
|[dsTabControl自绘控件](http://www.uieasy.com/cn/blog/?p=371)|dsTabControl（WidgetTab）控件是一款类似Tab control的树形控件，主要是为了解决标准Tab控件的限制，满足更多需求。|
下一篇文章：
[如何选择VC界面库产品（三）–- DirectUI界面库产品](http://www.uieasy.com/cn/blog/?p=479)
