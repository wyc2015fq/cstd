# 安装Visual C ++进行跨平台移动开发 - DoubleLi - 博客园







**Visual Studio 2015**






Visual Studio文档的新家是docs.microsoft.com上的[Visual Studio 2017文档](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://docs.microsoft.com/visualstudio&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhrwI-iIF6vxTF1vUSUr2FMoTWimg) 。

有关Visual Studio 2017的最新文档，请参阅在docs.microsoft.com上[安装跨平台移动开发的Visual C ++](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://docs.microsoft.com/visualstudio/cross-platform/install-visual-cpp-for-cross-platform-mobile-development&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjVZDPQGJi00uiaVSe8DMdVDCHZJw) 。 [用于跨平台移动开发的Visual C ++](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D536383&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhggWTApThGfnvDKEMG-tWSmaXXN8A)是Visual Studio 2015的一个可安装组件。它包含跨平台的Visual Studio模板，并安装跨平台工具和SDK以快速入门，无需自己定位，下载和配置它们。 您可以在Visual Studio中使用这些工具轻松创建，编辑，调试和测试跨平台项目。 本主题介绍如何安装使用Visual Studio开发跨平台应用程序所需的工具和第三方软件。 有关该组件的概述，请参阅[Visual C ++跨平台移动](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D536387&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjMJx70t6vIjYg1rIWzbCWrBXtDhQ)

[要求](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#Requirements)
[获取工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#GetTheTools)
[安装工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#InstallTheTools)
[安装iOS工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#InstallForiOS)
[手动安装或更新依赖项](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#ThirdParty)


## 要求





- 
有关安装要求，请参阅[Visual Studio 2015系统要求](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://www.visualstudio.com/visual-studio-2015-system-requirements-vs&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhgYxJe2w-lKZP7qOnlSvtHvpwqC6g) 。

|![System_CAPS_ICON_important.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC160177.jpeg) 重要|
|----|
|如果您使用的是Windows 7或Windows Server 2008 R2，则可以开发适用于iOS的经典Windows应用程序，Android Native Native应用程序和库以及应用程序和代码库的代码，但不适用于Windows应用商店或通用Windows应用程序。|



要为特定设备平台构建应用程序，还需要一些附加要求：
- 
Windows Phone模拟器和适用于Android的Microsoft Visual Studio模拟器需要可运行Hyper-V的计算机。 必须先启用Windows中的Hyper-V功能才能安装和运行仿真器。 有关更多信息，请参阅仿真器的[系统要求](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/ff626524.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhimes7W8-0AXySBNiLbppBkuojwXQ) 。

- 
Android SDK附带的x86 Android模拟器在可运行英特尔HAXM驱动程序的计算机上工作效果最佳。 该驱动程序需要带有VT-x和执行禁用位支持的英特尔x64处理器。 有关更多信息，请参阅[英特尔®硬件加速执行管理器的安装说明 - Microsoft Windows](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D536385&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjuxWDokkTmnklek5AJq1p8R-xKew) 。

- 
iOS构建代码需要Apple ID，iOS开发人员计划帐户和可在OS X Mavericks或更高版本上运行[Xcode 6](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D536387&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhjMJx70t6vIjYg1rIWzbCWrBXtDhQ)或更高版本的Mac计算机。 有关简单的安装步骤，请参阅[安装iOS工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#InstallForiOS) 。



## 获取工具






用于跨平台移动开发的Visual C ++是包含在Visual Studio Community，Professional和Enterprise版本中的可安装组件。 要获得Visual Studio，请转到[Visual Studio 2015下载](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3Flinkid%3D517106&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiizBPdnA0bD2uoXSjRjr15dSI_6g)页面，并使用Update 2或更高版本下载Visual Studio 2015。


## 安装工具






Visual Studio 2015的安装程序包含一个为跨平台移动开发安装Visual C ++的选项。 这将安装所需的C ++语言工具，Visual Studio的模板和组件，Android构建和调试所需的GCC和Clang工具集，以及与iOS开发的Mac进行通信的组件。 它还安装了支持iOS和Android应用程序开发所需的所有第三方工具和软件开发工具包。 大多数这些第三方工具都是Android平台支持所需的开源软件。
- 
构建面向Android平台的C ++代码需要Android Native Development Kit（NDK）。

- 
Android构建过程需要Android SDK，Apache Ant和Java SE Development Kit。

- 
适用于Android的Microsoft Visual Studio Emulator是可选的高性能模拟器，可用于测试和调试代码。


#### 为跨平台移动开发和第三方工具安装Visual C ++
- 
运行您在[获取工具中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#GetTheTools)的链接之后下载的Visual Studio 2015安装程序。 要安装可选组件，请选择“ **自定义”**作为安装类型。 选择**下一步**以选择要安装的可选组件。

- 
在选择功能中，展开**跨平台移动开发**并检查**Visual C ++移动开发** 。

![Select Visual C++ Mobile Development](https://i-msdn.sec.s-msft.com/dynimg/IC829297.jpeg)

默认情况下，当您选择**Visual C ++ Mobile Development时** ，将**编程语言**选项设置为安装**Visual C ++** ，并将**通用工具和软件开发工具包**选项设置为安装所需的第三方组件。 如果您需要它们，您可以选择其他组件。 默认情况下，也会选择**Microsoft Visual Studio Emulator for Android**。 已经安装的组件在列表中显示为不活动。

要构建通用Windows应用程序并在它们与Android和iOS项目之间共享代码，请在**选择功能**中展开**Windows和Web开发，**然后选中**通用Windows应用程序开发工具** 。 如果您不打算构建通用Windows应用程序，则可以跳过此选项。

选择**下一步**继续。

- 
第三方组件有自己的许可条款。 您可以通过选择每个组件旁边的**许可条款**链接来查看**许可条款** 。 选择**安装**以添加组件并安装Visual Studio和Visual C ++以进行跨平台移动开发。

- 
安装完成后，关闭安装程序，然后重新启动计算机。 在计算机重新启动之前，第三方组件的某些设置操作才会生效。

|![System_CAPS_ICON_important.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC160177.jpeg) 重要|
|----|
|您必须重新启动以确保一切安装正确。|


如果Microsoft Visual Studio Emulator for Android组件安装失败，您的计算机可能没有启用Hyper-V。 使用**打开或关闭**控制面板应用程序的**Windows功能**来启用Hyper-V，然后再次运行Visual Studio安装程序。

|![System_CAPS_ICON_note.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC101471.jpeg) 注意|
|----|
|如果您的计算机或Windows版本不支持Hyper-V，则无法使用Microsoft Visual Studio Emulator for Android组件。 Windows Home Edition不包含Hyper-V支持。|


- 
打开Visual Studio。 如果这是您第一次运行Visual Studio，则可能需要一些时间才能配置和登录。Visual Studio准备就绪后，请在**工具**菜单上选择**扩展和更新** ， **更新** 。 如果Visual Studio更新适用于跨平台移动开发的Visual C ++或适用于Android的Microsoft Visual Studio模拟器，请安装它们。



## 安装iOS工具






您可以使用Visual C ++进行跨平台移动开发，以编辑，调试和将iOS代码部署到iOS模拟器或iOS设备，但由于许可限制，代码必须在Mac上远程构建。 要使用Visual Studio构建和运行iOS应用程序，您必须在Mac上设置和配置远程代理。 有关详细的安装说明，先决条件和配置选项，请参阅[安装和配置要使用iOS构建的工具](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ) 。 如果你不是为iOS构建的，你可以跳过这一步。


## 手动安装或更新依赖项






如果您决定在安装Visual C ++ Mobile Development选项时不使用Visual Studio安装程序安装一个或多个第三方依赖项，则可以稍后使用[安装工具中](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/library/dn707598(v%3Dvs.140).aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhh_kJsNSUpDgzTxiQUPROHL2mUpvw#InstallTheTools)的步骤安装它们。 您也可以独立于Visual Studio安装或更新它们。

|![System_CAPS_ICON_caution.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC1429.jpeg) 警告|
|----|
|除Java之外，您可以按任何顺序安装依赖项。 在安装Android SDK之前，您必须安装并配置JDK。|


阅读以下信息并使用这些链接手动安装依赖关系。
- 
[Java SE开发工具包](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhUT_Hkw2F6s749gfBplP8ryAE84A)

默认情况下，安装程序将Java工具放在C：\ Program Files（x86）\ Java中。

- 
[Android SDK](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.android.com/sdk/index.html&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhibDO39Xsju1HEK4jxVX9HGjoEO7A#Other)

在安装过程中，按照建议更新API。 确保至少安装了适用于Android 5.0 Lollipop（API级别21）的SDK。 默认情况下，安装程序将Android SDK放在C：\ Program Files（x86）\ Android \ android-sdk中。

您可以再次在Android SDK目录中运行SDK Manager应用程序以更新SDK并安装可选工具和其他API级别。 除非您**以管理员**身份运行SDK Manager应用程序，否则更新可能无法安装。 如果您在构建Android应用程序时遇到问题，请检查SDK管理器以获取已安装SDK的更新。

要使用Android SDK附带的一些Android模拟器，您必须安装可选的Intel HAXM驱动程序。 您可能需要从Windows中删除Hyper-V功能才能成功安装英特尔HAXM驱动程序。 您必须还原Hyper-V功能才能使用Windows Phone模拟器和适用于Android的Microsoft Visual Studio模拟器。

- 
[Android NDK](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://developer.android.com/tools/sdk/ndk/index.html&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhgG5YzlAi5VNSWVxpXZPHzANNhyQ)

默认情况下，安装程序将Android NDK放入C：\ ProgramData \ Microsoft \ AndroidNDK中。 您可以再次下载并安装Android NDK以更新NDK安装。

- 
[Apache Ant](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://ant.apache.org/bindownload.cgi&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhhyfWDIRQq_4n4XPnTWwDTdPSb3CA)

默认情况下，安装程序将Apache Ant放在C：\ Program Files（x86）\ Microsoft Visual Studio 14.0 \ Apps中。

- 
[适用于Android的Microsoft Visual Studio模拟器](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=http://go.microsoft.com/fwlink/p/%3FLinkId%3D536390&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhi9K9ulPTgiKKPJiImnK2Nb8xgDNw)

您可以从Visual Studio库安装和更新适用于Android的Microsoft Visual Studio模拟器。


在大多数情况下，Visual Studio可以检测您安装的第三方软件的配置，并在内部环境变量中维护安装路径。 您可以在Visual Studio IDE中覆盖这些跨平台开发工具的默认路径。

#### 为第三方工具设置路径
- 
在Visual Studio菜单栏上，选择**工具** ， **选项** 。

- 
在“ **选项”**对话框中，展开“ **跨平台”** ， **C ++** ，然后选择“ **Android”** 。

![Android tool path options](https://i-msdn.sec.s-msft.com/dynimg/IC829298.jpeg)

- 
要更改工具使用的路径，请选中路径旁边的复选框，然后在文本框中编辑文件夹路径。 您也可以使用浏览按钮（ **...** ）打开**选择位置**对话框来选择文件夹。

- 
选择**确定**以保存自定义工具文件夹位置。



## 也可以看看






[安装并配置工具以使用iOS进行构建](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://msdn.microsoft.com/en-us/library/mt147405.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiR1Ot0NaIw2nZ2jHcQh3IqtCOuVQ)
[Visual C ++跨平台移动](https://translate.googleusercontent.com/translate_c?depth=2&hl=en&ie=UTF8&prev=_t&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://www.visualstudio.com/explore/cplusplus-mdd-vs.aspx&xid=17259,15700021,15700105,15700124,15700149,15700168,15700173,15700201&usg=ALkJrhiBwNvWoww-7RKp9kKBYxrGlHKmnQ)











