# 《BREW进阶与精通——3G移动增值业务的运营、定制与开发》连载之14---BREW精要之事件驱动模型 - 我相信...... - CSDN博客





2009年07月21日 09:32:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1299标签：[brew																[windows																[command																[任务																[dialog																[ui](https://so.csdn.net/so/search/s.do?q=ui&t=blog)
个人分类：[消失的技术](https://blog.csdn.net/wireless_com/article/category/597607)







       BREW应用程序的模型是基于一个事件驱动的协作式多任务模型。事件处理机制的核心问题是程序应该只处理需要的事件，对于不需要处理的事件，需要返回给系统处理。应用在加载之后可以通过
HandleEvent()函数接收所有输入的事件，然后会通过返回TRUE（已处理）或FALSE（未处理）指示是否处理事件。AEE层存在一个全局的事件队列，所有的事件都存储在该队列中，如果队列中的事件在分发后处理完毕或者无人处理，该事件将被从事件队列中删除。

       BREW中的事件主要有三类：系统事件，预定义的事件和。系统事件的事件代码范围为0~7,即从EVT_APP_START 到 EVT_APP_BROWSE_FILE。预定义事件的事件代码范围是0x0008-0x4fff，即BREW AEE 层和OEM层预定义或者预留的事件。自定义的事件是指 BREW应用可以自定义自己的事件，自定义事件应该不小于EVT_USER（0x5000）。BREW应用可以发送任何事件给应用自身，或者发给在同一进程中的其他应用。如果发送事件给不同进程的应用（主要是针对BREW
4.X及以上版本），这时需要特殊的应用权限。

BREW环境要求及时地处理事件。简而言之，如果应用执行处理事件HandleEvent()调用后没能在合适的时间返回，AEE可能就会关闭应用以保护设备相应其他请求。有些操作，比如说从网络套接字中读取数据，也许耗时过长，无法在一次调用事件处理器内完成。这时就采用回调机制，以便在操作完成之后通知该应用。

#### 4.3.2.1事件处理器

        AEE执行环境调用BREW
应用自身的事件处理器来传递关于一系列事件的消息。有过windows编程经历的读者都会清楚这种机制，Windows下消息处理机制：当在交互中进行一个操作（信号，输入，等等），windows将产生相应的事件，通过window的事件分发机制，相应的窗口或者应用得到该事件，从而触发相应的事件处理器进行处理。BREW中事件处理机制与其相似，即BREW环境捕捉到事件后，分发到相应的应用或者控件，由应用或者控件的事件处理器进行处理。

以下是BREW中事件处理器接口的示例：

*boolean MyApp_HandleEvent(IApplet *pIApp, *

*AEEEventeCode, *

*uint16wParam,*

*uint32wParam)*

在该示例中，变量pIApp实际上指明了应用的结构，也就是AEEApplet的一个指针。许多应用将其结构定义为AEEApplet的超集，而pIApp也能指向该结构。

eCode变量是说明应用接收的事件类型，如EVT_APP_START、EVT_KEY和EVT_ALARM等典型事件。

wParam和dwParam参数是依据接收的事件而定义的短数据和长数据值。这些值取决于事件本身，根据事件自身来定义。对于某些事件，短数据和长数据字段中都包含事件数据；而对于另一些事件，长短字段中仅有一个字段，甚至没有字段。两个数据字段均不包含数据的事件有EVT_APP_START、EVT_APP_STOP、EVT_APP_SUSPEND和EVT_APP_RESUME。两个数据字段中都包含数据的典型事件有EVT_DIALOG_START和EVT_COMMAND。仅在短数据字段中包含数据的典型事件有EVT_ALARM，仅在长数据字段中包含数据的典型事件有EVT_NET_STATUS和EVT_CTL_CHANGING。

按键事件作为EVT_KEY 事件发送给应用。短数据字段包含主键代码；比如说如果用户按下按键符合“2”，就包含AVK_2这一主键代码。AVK_2的值由AEEVCodes.h头文件定义。

在Emulator中，与按键符号相对应的主键代码由设备配置文件确定，也可经由设备配置器进行修改。在手机上，主键代码由设备厂商决定。

### 4.3.2.2事件处理的提示

     执行应用时，仅考虑处理应用可能需要处理的事件。许多事件可以被忽略。举例来说，如果执行一个游戏应用时，仅需使用上下左右箭头键，则可忽略接收到的0-9按键事件。

     但是如果接收到关键事件，则无论应用处于何种状态都不能忽略。如EVT_START、 EVT_STOP、EVT_SUSPEND和EVT_RESUME等系统事件就是在任何情况下都会影响应用的例子，所以不能忽略。需要特别注意的是，无论应用给定状态如何，必须接收所有的关键事件。某些事件在应用特别指示需要此类通知时才会发送。应用必须为这些通知事件注册，可以在MIF编辑器中指定MIF文件的通知事件注册，或者使用ISHELL_RegisterNotify()进行动态注册。

     作为一个约定，应用在处理EVT_START分配的任何数据，在处理EVT_STOP时都应该释放出去。但是，在AEEClsCreateInstance()中分配的内存数据，一般必须通过FreeAppData()机制来释放。

### 4.3.2.3事件分发与代理

     当控件激活时，事件应该传递到激活的控件，使控件进行自我更新，又叫做事件代理。例如，如果菜单控件处于激活状态，应该将事件传递到IMENUCTL_HandleEvent()；如果文本控件处于激活上，应传递到ITEXTCTL_HandleEvent()，然后控件就会采取相应的操作。以菜单控件来说，它就会改变被选项目，重新绘制菜单。

     正如事件处理函数将TRUE或FALSE返回AEE执行环境那样，控件返回TRUE或FALSE则表示它们处理的事件。每个控件类型只会处理必要的事件。标准菜单控件只处理“上”、“下”和其他部分关键事件，而软键菜单控件则处理“左“、“右”和其他关键事件。如果一个控件从发放的事件中返回的是TRUE，应用就可以早一点从事件处理函数中退出来，但还可以执行额外的处理。如果一个控件从发放的事件中返回的是FALSE，应用一般应该继续处理该事件。如果该控件接收的是从事件处理器返回的FALSE，BREW就要执行默认的处理。

      当用户按下“选择”键进行选择时，菜单控件使用EVT_COMMAND来通知应用。在这种情况下，一个来自“选择”键下放的EVT_KEY按键事件就由菜单控件处理。此外，对于菜单控件任何视图更新，菜单控件将EVT_COMMAND事件发回给应用。事实上，所有控件类型都能提供代理机制。



**图 4-7****： BREW****中的事件分发示例**



事件的分发代理机制非常灵活，你可以在消息循环中按照你的需要自由处理（图。



**图 4-8****： BREW****中的事件代理机制**

BREW下的消息处理机制与Windows下的消息处理机制的区别在于：BREW的体系结构采用了COM方式，即具有面向对象的类层次结构，从而其具体的事件处理机制也是作为各个接口外露的接口函数的形式被运用。一个应用本质上来说就是一个实例化的IAPPLET类，所以这样就统一了所有在应用中运用的事件处理机制都是各个接口外露接口函数的说法。具体而言，这些事件处理器还是有区别的，主要是IAPPLET_HandleEvent和其他接口的HandleEvent的区别。

IAPPLET_Handleevent是通过在AEEClsCreateInstance中的AEEApplet_New函数被注册实例化的，AEEApplet_New函数实例化应用同时也通过传入USERAPP_HandleEvent参数实例化了IAPPLET_Handleevent。

除了IAPPLET具有handleevent外，所有的继承IControl的接口也具有事件处理函数，允许处理事件。这些各种具体的IControl_handleEvent有两种方式被调用。一种是在应用的handleevent中由开发者显式的调用，如：

switch (eCode) 

{ 

case EVT_APP_START:

return(TRUE); 

case EVT_APP_STOP:

………. 

Case EVT_KEY: 

IMENU_Handleevent…. 

ItextCtl_Handleevent…. 

}

另一种是当这些控件包含于对话框中，且处于聚焦状态时，这些事件处理函数的触发是隐式的，是由AEE机制自动触发的，无需在代码中显式的调用这些handleevent。 IDialog接口没有外露的handleevent接口函数，但是允许通过IDialog_seteventhandle来注册一个该对话框的事件处理函数。需要注意的是该事件处理函数是何时被触发的：一旦当一个对话框处于激活时，AEE层将会把所有的事件直接发往该对话框，该对话框会自动的调用处于焦点控件的handleevent来处理该事件，只有当该控件没有处理该事件时，对话框注册的事件处理函数才会被调用。

当BREW运行后，首先操作系统中的ui 任务会捕捉到各种事件，此时ui 任务通过aee_dispatch将事件分发至BREW环境中。BREW环境再通过aee_sentevent具体分发事件至目的地，在两种不同的情况下将走不同的流程。

如果当前没有激活的对话框，则紧接着IAPPLET_Handleevent被BREW自动调用来处理事件，而此时调用的IAPPLET_Handleevent其实就是用户注册的app_handleevent。从而实现了允许用户的应用捕捉到事件并处理的机制。在用户的app_handleevent中，用户可以将事件继续下发，比如通过调用IMENU_handleevent等将事件下发给各种控件处理。

如果当前有激活的对话框，则基于对话框的事件被BREW自动调用，从而使得事件被对话框最先截获，而对话框之后的处理是检查包含的控件中哪个处于焦点，并将事件下发给它的handleevent来处理，同时根据其返回值来判断其是否已经处理了该事件，当其返回False后，对话框将该事件继续转发至该对话框注册的handleevent（如果有的话），如果该handleevent仍然返回false，BREW继续将该事件转发至app_handleevent。这种机制使得当以对话框方式来创建应用时，各种事件被自动的处理，从而简化了代码量，但也使得事件流程更加晦涩，用户的应用程序不能直接的控制它。](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




