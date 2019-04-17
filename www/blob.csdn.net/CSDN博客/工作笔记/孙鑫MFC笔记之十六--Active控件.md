# 孙鑫MFC笔记之十六--Active控件 - 工作笔记 - CSDN博客





2012年07月01日 15:28:55[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8559








基本概念：

容器和服务器程序

      容器应用程序时可以嵌入或链接对象的应用程序。Word 就是容器应用程序。服务器应用程序是创建对象并且当对象被双击时，可以被启动的应用程序。Excel 就是服务器应用程序。ActiveX 控件不能独立运行，它必须被嵌入容器应用程序中，和容器应用程序一起运行。


---------------------------------------------------------------------------------

编写一个 ActiveX 时钟控件

      1.利用 MFC ActiveX ContrlWizard 新建一个 Clock 工程

      2.在新建的工程中有三个类，其中 CClockApp 从 COleControlModule 中派生出来的，

可以将其看作是应用程序类，它的一个实例表示控件程序本身。


      COleControl 从 CWnd 派生得到，也是一个窗口类，

      CClockCtrl 相当于单文档程序的主窗口类。

      这个类包含重绘用的 OnDraw 函数，也包含了一些消息映射，包括调度映射。

      Dispatch maps 调度映射，主要是 MFC 提供让外部应用程序可以访问控件的属性和方法。

      Event maps 事件映射，控件向包含它的容器发送事件通知。


CClockPropPage 类由 COlePropertyPage 派生而来：


      COlePropertyPage


      The COlePropertyPage class is used to display the properties of a custom control in a graphical interface, similar to a dialog box. 被用来显示一个自定义控件的属性，类似于一个对话框。它是对话框类，于是 enum { IDD = IDD_PROPPAGE_CLOCK }给它关联了一个对话框资源。


      在工程中，还有两个全局函数：STDAPI DllRegisterServer(void)；将控件信息写入注册表中 STDAPI DllUnregisterServer(void)卸载注册信息。

      3.在三个类之上，还有类似小勺的图标 Dclock,_DClockEvents，它们表示接口。

      接口是外部程序和控件进行通信的协议，可以把接口看作是函数的集合，外部程序通过接口提供的方法，去访问控件的属性和方法。也可以将接口看作抽象基类，接口中所定义的所有函数都是纯虚函数， 这些函数的实现都是在 CClockCtrl 类中实现，MFC 通过底层的封装使 CClockCtrl 类继承 Dclock 接口。所以调用接口，事实上调用的是 CClockCtrl 类中的函数。由于封装，底层的通信细节我们看不到，如果对这个感兴趣，可以看一些 COM 编程的资料。


      4.如果些时编译一下工程，会生成一个 Clock.ocx 文件，它就是 ActiveX 控件的文件。

       要用控件的时候，只需要把这个文件传递给对方。注意：ActiveX 控件不能单独使用，必须嵌入到一个窗口当中一直运行。当我们用 VB 添加控件时，发现了我们刚才编译的控件，但是 VB 怎么知道我们新建的控件的位置呢？ 我们发现编译时，输出窗口的最后一行 “Registering ActiveX Control...”说明编译时会注册控件。事实上编译之后，VC 会调用 regsvr32 注册控件，并将信息写入到注册表中，VB 在加载 ActiveX 控件时，会从注册表中搜寻相关的 ActiveX
 控件信息。注意：ActiveX 控件在使用之前都需要注册。

      如果想卸载控件，可以在“运行”中输入命令：

      “regsvr32 /u 控件文件完整路径名”，事实上是调用工程中全局函数 DllUnregisterServer 来完成卸载的。

      如果想再次注册控件，可以在“运行”中输入下面命令：

      “regsvr32 控件文件完整路径名”事实上是调用工程中全局函数 DllRegisterServer 来完成注册的。


      5.下面实现在控件上输出当时系统时间。

      可以在 OnDraw 函数中完成这个功能。


      这样就能做出一个静态的时间控件，如果我们想使控件实时显示时间，需要添加两个消息响应函数  WM_CREATE,WM_TIMER.要使时间可以每秒更新，我们先在 CClockCtrl 类中添加 WM_CREATE 消息处理，在其响应函数 OnCreate()中设置一个计时器：SetTimer(1,1000,NULL);接着增加一个 WM_TIMER 消息响应处理，在 OnTimer 中写上 Invalidate(); 使窗口发生重绘。也可以调用 InvalidateControl()强制控件重绘自身。


      6.在 VB 测试中发现，其他很多控件可以修改控件的背景色、前景色和字体颜色，而我们还不行。

      ActiveX 控件有四种属性：

      Stock：为每个控件提供的标准属性，如字体或颜色。

      Ambient：围绕控件的环境属性——已被置入容器的属性。

      这些属性不能被修改，但控件可以使用它们调整自己的属性。

      Extended：这些是由容器处理的属性，一般包括大小和在屏幕上的位置。

      Custom：由控件开发者添加的属性。


      7.这时在 VB 测试中，我们的控件也可以修改控件的背景色和前景色了，但是设置完以后没有效果，因为还要 OnDraw 函数中自己编写代码来完成这些改变。要得到控件标准属性，要通过一些函数来完成，如COleControl::GetForeColor 得到前景色，COleControl::GetBackColor 得到背景色，不过要注意的是它们得到的是 OLE_COLOR 的颜色，还需要通过 TranslateColor 方法转换成 COLORREF。


      8.控件一般都会有属性页，当我们右键点击控件时会弹出这个属性页方便对控件属性的设置，我们的控件已经有一个属性页，通过 CClockPropPage 类来实现的，显示的面容是对话框资源的内容，下面修改控件属性页：

      属性页之所以可以在窗口中看得到，是因为在 ClockCtl.cpp 中的 BEGIN_PROPPAGEIDS(CClockCtrl, 1)与 END_PROPPAGEIDS(CClockCtrl)之间调用了 PROPPAGEID(CClockPropPage::guid)，其中的 guid 表示全局唯一标识符，它是一个128位的整数，用来唯一地标识一个组件或者一个接口。我们可以用 PROPPAGEID 宏增加属性页。


      增加颜色属性页

      首先在 BEGIN_PROPPAGEIDS(CClockCtrl, 1)与END_PROPPAGEIDS(CClockCtrl)之间添加代码 PROPPAGEID(CLSID_CColorPropPage)来添加属性页。


      CLSID_CColorPropPage 属性表单是控件自身提供的， 我们添加之后不用作任何处理，就可以使用，效果如下：


      注意：

      BEGIN_PROPPAGEIDS(CClockCtrl, 2)中的数字2表示有多少个属性页代码要显示，

      如果增加了属性页数字一定要加1，不然如果没有修改或修改错误，会产生不可预料错误。


      9.增加一个自定义属性也是通过 MFC ClassWizard 来完成，与第6步增加前景色与背景色的步骤相同。

      1)下面增加设置时间间隔的属性，用这个属性来控件时间刷新频率：


      属性添加成功以后，在_DClock 接口中增加了 Interval 属性，同时在 CClockCtrl 类中增加了一个

 成员变量 m_interval 和 OnIntervalChanged()方法（当外部属性修改之后就会调用这个方法）。

 而且它们都在调度映射当中：


      2)在 CClockCtrl::OnIntervalChanged()中添加属性处理程序代码


      10.使增加的自定义属性在属性表单中设置

      在对话框资源中添加一个编辑框，再为这个编辑框关联一个变量， 注意，我们在为编辑框关联一个变量 m_updateInterval 的同时也关联了一个属性是，这样我们不需要增加代码就能把控件和自定义属性相关联。在 void CClockPropPage::DoDataExchange(CDataExchange* pDX)中会生成下面代码：


      这样，我们就可以在属性页里面设置时间间隔了。


      11.为控件添加一个方法：

      为控件增加函数，MFC ClassWizard-->Automation-->Add Method Class Name 要选择CClockCtrl 输入函数名，之后就可以在 CClockCtrl 类中找到了。方法添加成功以后，在_DClock 接口中增加了 Hello 方法，同时在 CClockCtrl 类中增加了 Hello 方法。接下来，我们可以在 CClockCtrl 类中增加了 Hello 方法添加自己的代码就可以了。

      12.为控件添加一个标准事件

      我们选择 MFC ClassWizard-->ActiveX Events--->Add Event。事件添加成功以后,会在_DClockEvents 中增加一个事件 Click，DClockEvents 接口是源接口，控件将用这个接口发送通知事件，它不是控件本身实现的接口，这个接口是通过容器来实现的


      13.增加一个自定义事件：当秒数为0时，发出一个 NewMinute 事件。

                   1）增加一个自定义事件的过程与增加一个标准事件的步骤相同，也可以这样添加在事件接口_DClockEvents 上点击右键，选择增加事件，效果一样，都会弹出 Add Event 对话框。事件添加成功以后,会在_DClockEvents 中增加一个事件NewMinute事件，同时在在 CClockCtrl 类中增加了void FireNewMinute(),也就是在控件内部可以调用 FireNewMinute()向容器发送事件通知，而这个函数内部会调用接口的 void
 NewMinute()向容器发出事件通知。


                    2）在 OnDraw()方法添加代码 if(0 == time.GetSecond())FireNewMinute();使当秒数为0时，向容器发出一个 NewMinute 事件通知。标准消息不需要另外写代码向容器发出事件通知，VC在底层代码内部实现了这个过程。

      14.将我们自定义的控件属性在修改之后永久保存下来，用户打开程序之后，控件的属性都是使用原先保存的值。需要在 void CClockCtrl::DoPropExchange(CPropExchange* pPX)加入如下代码 PX_Short(pPX,"Interval",m_interval,1000);之后再在程序中 OnCreate()方法中将 SetTimer(1,1000,NULL); 修改代码为 SetTimer(1,m_interval,NULL);


      15.在属性页中修改自定义控件属性的时候，通知容器做相应的调整，从而使容器属性列表中实时地显示我们对属性所作的修改。在 void CClockCtrl::OnIntervalChanged() 中加入如下代码：BoundPropertyChanged(0x1);   //通知容器调度 ID 为1的属性发生了改变


      16.前面都是让控件在容器设计时改变控件属性，如果希望用户在设计模式时时钟控件停止运行，而在用户模式下时间会跳动，在控件内部可以通过 AmbientUserMode()方法得到当前控件是处于设计状态还是运行状态。在 void CClockCtrl::OnTimer(UINT nIDEvent)下修改代码如下：

      if(AmbientUserMode())  InvalidateControl();

      17.编写完控件以后，我们可以选择 Win32 Release 方式进行编译，生成发行版 ActiveX 控件。在开发的时候通常是以 Win32 Debug 模式下编译的，这种模式下开发有助于我们开发过程的产生的错误，例如非法内存访问错误；还可以帮助我们调试程序，跟踪程序进而排查错误。但是在调试模式下生成的文件比较大，因为在文件中包含了调试的信息。

      而当我们开发完成后，在 Release 模式下进行编译时，VC 编译器会在代码生成上、执行速度上做一些优化，同时生成的可执行程序或控件文件会比较小。

-------------------------------------------------------------------

在 VC 中编写一个客户端调用 ActiveX 控件：

1.新建一个基于 MFC 对话框的 ClockTest 工程项目

2.点击右键，选择“插入 ActiveX 控件”，然后在弹出的对话框中选择刚才我们创建的控件。

也可以通过菜单的方式增加控件：

 “工程”->增加到工程->Componets and Controls

 选择已注册控件"Registered ActiveX Contrlos"，找到我们自己控件，再按下插入。


通过这种方式插入 ActiveX 控件时，会在工程中自动生成一个类 CCock，其基类为 CWnd。这个类是一个封装类，封装了对 ActiveX 控件进行访问的一些操作。同时在 VC 的工具箱上面也增加一个时钟控件，可以直接将一个时钟控件拖放到窗体上。

3.用第二种方法插入的控件，除了将控件手动插入到窗体以外，我们也可以通过代码动态生成一个控件。

1）CClockTestDlg 增加一个成员变量：CClock m_clock;

2）在 CClockTestDlg 的头文件中包含一个 clock.h

3）接下来就要以在一个按钮的单击事件中增加创建控件的代码：

4）在设计时，可以点击右键为控件添加事件响应。



