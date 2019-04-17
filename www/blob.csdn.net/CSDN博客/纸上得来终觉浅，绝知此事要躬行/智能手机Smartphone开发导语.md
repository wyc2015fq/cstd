# 智能手机Smartphone开发导语 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年06月23日 14:53:21[boonya](https://me.csdn.net/boonya)阅读数：882








文章转自：[http://dev.yesky.com/481/2044981.shtml](http://dev.yesky.com/481/2044981.shtml)


Micrsoft的彩色小旗在[台式机](http://product.yesky.com/pc/)、[笔记本](http://product.yesky.com/notebookpc/)、[掌上电脑](http://product.yesky.com/pda/)、[平板电脑](http://product.yesky.com/tabletpc/)上四处飘扬，如今，[Microsoft](http://product.yesky.com/database/microsoft/)又把旗帜插入了[手机](http://product.yesky.com/mobilephone/)领域，这就是今天我们要谈的主角，Microsoft的智能手机操作平台Smartphone。本文将从程序开发角度对Smartphone进行整体的说明，希望能给你一个Smartphone开发的大概印象。

**从开发角度看Smartphone**


　　Smartphone是一个智能手机操作平台，它基于Microsoft的嵌入式实时[操作系统](http://product.yesky.com/os/) Windows CE进行设计，因为它的[应用](http://search.yesky.com/search.do?wd=%E5%BA%94%E7%94%A8)定位在手机上，因此Microsoft在底层对它的无线通讯以及[语音](http://search.yesky.com/search.do?wd=%E8%AF%AD%E9%9F%B3)文本信息通讯模块做了大量的优化。下面是一款使用Smartphone
 2003简体中文版的设备，多普达565的主界面。

|![](http://dev.yesky.com/imagelist/05/07/x049j7yy8108.gif)|
|----|


　　对开发人员来说，在开发过程中，我们能够看到熟悉的开发环境界面、熟悉的消息体系、熟悉的API函数、以及熟悉的[内存](http://product.yesky.com/memory/)管理与[进程管理](http://mydown.yesky.com/soft/261/261328.html)，根据个人经验，我认为如果你曾经在桌面Windows或者Pocket
 PC上开发过软件，那么你所需要关注的只是一些注意事项再加上一个入门，然后就可以象进行桌面程序开发一样进行Smartphone的开发了，当然，时刻参考MSDN肯定是少不了的，呵呵，这个相信大家都很熟悉。


　　1、Smartphone平台的基础架构


　　下面是Smartphone平台的基础架构图：

|![](http://dev.yesky.com/imagelist/05/07/g94m98992szl.gif)|
|----|


　　无线堆栈层：手机的本质功能当然是无线通讯了，这个层次负责最终的数据发送与接收处理，它对进行接收和发送的硬件细节进行控制，突出无线通讯这个核心内容，是Smartphone操作平台最低的一层。


　　核心API：和我们普通开发所说的API大致是一样的，开发人员利用它们来进行与无线通讯与语音服务相关的开发。这部分内容可以在开发的时候参照MSDN来使用。


　　逻辑：这一层在核心API层的上面，系统为了简化开发，会将一些API包装成各个功能块供你调用，类似于我们平常所说的控件，这一层就是提供各种控件的接口。


　　应用层：系统中面向用户提供服务的应用程序。


　　2、Smartphone编程的整体框架


　　Smartphone开发框架和一般的桌面Windows开发区别不大，你可以使用可视化编译工具（eMbedded[Visual C++](http://mydown.yesky.com/soft/255/255680.html) 或者Visual Studio 2003.net中的VB.net、[C](http://product.yesky.com/product/225/225032/)#）结合Smartphone的SDK来开发适用于Smartphone手机的软件。


　　主框架处理是：


　　·注册窗口类，连接窗口过程


　　·初始化应用程序实例


　　·建立程序主窗口


　　·进入消息循环


　　消息循环相关处理是：


　　·收到消息分发到窗口过程处理


　　·窗口过程处理自己需要处理的消息


　　·不需要处理的消息则调用系统缺省处理函数来实现


　　·如果接收到消息是退出消息，就退出消息循环，结束程序


　　编程工作主要是：


　　·为自己需要处理的消息增加消息映射入口


　　·实现消息处理函数


　　3、Smartphone开发与PocketPC开发


　　因为两者都基于Windows CE操作系统而设计，因此Pocket PC和Smartphone在开发上是非常相像的，有许多Pocket PC的程序可以轻松移植到Smartphone上来，对于开发而言，两者最重要的相同点是：Smartphone的开发环境和所使用的Win CE API与Pocket PC是相同的。


　　两者之间的不同主要表现在用户界面的关联上，这主要表现在两个方面，一个是分辨率： Pocket PC具有分辨率为240*320像素的界面而Smartphone则是176*220像素。另外一个需要注意的是应用程序导航，Pocket PC使用[触摸屏](http://product.yesky.com/touchscreen/)和触摸笔而Smartphone则使用硬件按钮导航。

|![](http://dev.yesky.com/imagelist/05/07/68i737afi201.jpg)|
|----|


　　如果你关注并且修改了相关的代码，你可以发现，你的Pocket PC代码大部分可以移植到Smartphone设备上来。


　　3、Smartphone开发与windows桌面开发


　　如果你有过Windows桌面开发经验，你会发现Smartphone开发具有大致相同的应用程序结构，但是，你需要注意Smartphone上适用的API（称之为Win CE API），比起桌面Windows所提供的API来，有很多功能被精简，有些API甚至彻底消失了，所有这些意味着你不能随心所欲使用你在Windows桌面开发所获得的经验；你需要始终在Windows CE帮助和Smartphone SDK 帮助中获得这些API的精确信息。


　　另外，你也会发觉程序的用户界面设计会有些不同，比如Smartphone的分辨率被规定为176*220像素，比如你需要注意用户输入字母和文字比在桌面Windows中困难得多等，这些下面我会进行详细地分析，当然，也不用太担心，因为有很多知识是照样可用的。

**Smartphone软件开发原则**


　　1、审视Smartphone硬件


　　开发基于Smartphone操作平台的软件，第一步应该是了解Smartphone设备，这主要是因为在手机上面的程序开发与硬件本身有非常密切的关系，当然我们重点关注的是与开发有关系的硬件内容，这主要表现在硬件控制与显示屏幕两个部分。



　　1) 硬件控制


　　Smartphone设计的宗旨是方便用户使用单手进行操作，因此，Smartphone放弃了触摸笔，改用按键来控制导航。


　　Microsoft 规定，使用Smartphone的手机必须具备如下9个程序控制按键，当然，因为各手机厂家设计风格不同，按键的位置形状可以由厂家自行设置。我们设计程序时可以充分利用这9个按键的功能。


　　·五向控制键：包括上、下、左、右四个方向键 和 中间的一个"执行"键；中间的这个"执行"键用来执行当前的操作。 


　　·主页键：按此按钮可以返回手机的主屏幕（就是手机刚刚启动时所显示的那个屏幕，类似于windows的桌面）。


　　·后退键：退回到上一个状态（或者说上一个屏幕）。它有一个特殊的作用，就是在编辑框中时，它的作用是删除前一个输入的字符，想想也有道理，因为这时我们可以认为上一个屏幕就是输入当前字符之前的那个屏幕^_^。



　　·两个多功能键：手机上有左、右两个多功能键，常常被称为"软"键，这是因为他们的功能会随着使用场合的不同而动态变化，对应执行的功能一般会在上面运行的软件底部显示。

|![](http://dev.yesky.com/imagelist/05/07/w8olo3h1xwx8.jpg)|
|----|


　　开发的时候要充分利用这些硬件控制键的功能，但因为这些控制键意义明确，所以，一般不能改变这些按键的默认功能。 

此外，对于程序开发来说，"*"和"#"这两个按键也可以在程序作灵活运用，因为这两个键在程序中一般没有系统默认的含义（例外是在有字符输入的时候），赋予它们自己需要的功能不会引起用户的疑惑。


　　2) 显示屏幕

|![](http://dev.yesky.com/imagelist/05/07/9145i7xxh419.jpg)|
|----|


　　对开发人员而言，屏幕的分辨率是对开发最有影响的因素，Microsoft规定使用Smartphone的手机必须具备一个176*220分辨率的LCD屏幕，颜色为16位色。在开发的时候我们需要时时提醒自己这一点，比较极端的做法是，在设计Smartphone上的程序时，将显示屏幕的设计尺度精确到像素级别。



　　2、Smartphone上的软件开发原则


　　大致了解了Smartphone的硬件后，结合Microsoft对Smartphone上开发的设计白皮书，我们来看看在Smartphone上开发所应该遵循的原则：


　　1) 保持软件的一致性原则


　　让用户"学一次，到处可用"是[微软](http://product.yesky.com/officesoftware/microsoft/)桌面操作系统Windows大获成功的一个关键，要保持软件的一致性，最主要的就是大家都要遵循一定的开发规则，下面我略谈一下，给大家一个初步的印象，具体请参考Smartphone 2003 SDK帮助中"Smartphone User Interface
 Guidelines"一章。


　　1、 每一个程序最下面都固定有两个内容敏感的按钮，这两个按钮使用手机上的两个软键对应触发。


　　原则是：需要直接执行的操作或者当前场景下最常用的操作一般放在左软键，需要提供弹出菜单选择的操作一般放在右软键；不要使用这两个软键（对应手机下面的两个硬件button）作程序的导航使用，导航最好使用五方向的导航按钮。

|![](http://dev.yesky.com/imagelist/05/07/5z7hz6751ei3.jpg)|
|----|


　　2、每一个form都用menu来完成


　　除了[Internet explorer](http://mydown.yesky.com/soft/255/255343.html)页面中，普通的button不被使用在Smartphone平台上，左右软键菜单项被使用在一些需要随着环境而变得场合，超过两种则弹出菜单，否则，平铺（如按钮）菜单。


　　2) 时刻考虑硬件限制条件原则


　　在手机上[开发软件](http://product.yesky.com/developsoftware/)，硬件限制必须要被时刻考虑，这个限制主要有如下几种：



　　限制1、手机内存小，[CPU](http://product.yesky.com/cpu/)速度相对慢。这要求程序效率高内存节省。


　　限制2、屏幕比较小。


　　限制3、手机操作比使用[键盘](http://product.yesky.com/keyboard/)和鼠标困难的多。因此我们需要考虑我们的软件是否用起来足够简单，从一定意义上讲，这是手机上开发软件的首要原则。


　　下面是一些依据本原则而来的建议：


　　1、 MFC不被Smartphone支持，因为限制1。


　　2、 Smartphone是个多任务处理系统，但程序被切换到后台执行时，最好释放多余的资源，因为限制1。


　　3、 Smartphone上的message box，要求显示在全屏窗口，因为限制2.。


　　4、 控件排列规则：需要放置多个控件的时候，最好每行只放置一个控件，这样用户可以只是使用上下键来切换控件，如果有标签之类，最好将标签单独放置一行。另外，在每个控件中，最好只有单个方向（或者上下/或者左右）的操作，因此在Smartphone中不提供combox控件。因为限制3。


　　5、 窗口布局规则：为了简单，每个窗口中的控件不要太多，如果需要更多的控件，你可以使用滚动式的窗口，不推荐使用层叠（tabbed）窗口，这也是SDK中不提供TAB控件的原因。因为限制2与3，操作要简单，窗口布局要简单&清晰，这也是我们在Smartphone上进行程序设计的原则。


　　上面是进行Smartphone开发最重要的两条原则，如果你想要更详细的解释，请看"Smartphone SDK"上的"UI Guidelines "。



