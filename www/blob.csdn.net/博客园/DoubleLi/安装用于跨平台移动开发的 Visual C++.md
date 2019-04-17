# 安装用于跨平台移动开发的 Visual C++ - DoubleLi - 博客园






# 安装用于跨平台移动开发的 Visual C++


Visual Studio 2015






若要了解有关 Visual Studio 2017 RC 的最新文档，请参阅 [Visual Studio 2017 RC 文档](http://docs.microsoft.com/visualstudio)。

[用于跨平台移动开发的 Visual C++](http://go.microsoft.com/fwlink/p/?LinkId=536383) 是 Visual Studio 2015 的可安装组件。 它包括跨平台 Visual Studio 模板，并安装了跨平台工具和 SDK 以快速启动，而无需自行查找、下载和配置它们。 你可以在 Visual Studio 中使用这些工具轻松创建、编辑、调试和测试跨平台项目。 本主题介绍了如何安装使用 Visual Studio 开发跨平台应用所需的工具和第三方软件。 有关组件的概述，请参阅 [Visual C++ 跨平台移动](https://www.visualstudio.com/explore/cplusplus-mdd-vs.aspx)

[要求](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#Requirements)
[获取工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#GetTheTools)
[安装工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#InstallTheTools)
[安装针对 iOS 的工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#InstallForiOS)
[手动安装或更新依赖项](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#ThirdParty)


要求







- 
有关安装要求，请参阅 [Visual Studio 2015 系统要求](https://www.visualstudio.com/visual-studio-2015-system-requirements-vs)。

|![System_CAPS_ICON_important.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC160177.jpeg) 重要事项|
|----|
|如果使用的是 Windows 7 或 Windows Server 2008 R2，则可以针对经典 Windows 应用程序、Android Native Activity 应用和库以及适用于 iOS 的应用和代码库开发代码，但不能针对 Windows 应用商店应用或通用 Windows 应用开发代码。|



若要为特定的设备平台创建应用，还需要满足一些附加要求：
- 
Windows Phone 仿真程序和适用于 Android 的 Microsoft Visual Studio 仿真程序需要可以运行 Hyper-V 的计算机。 有关详细信息，请参阅仿真程序的[系统要求](https://msdn.microsoft.com/zh-cn/library/ff626524.aspx)。

- 
Android SDK 附带的 x86 Android 仿真程序在可以运行 Intel HAXM 驱动程序的计算机上工作性能最好。 此驱动程序需要具有 VT-x 和执行禁用位支持的 Intel x64 处理器。 有关详细信息，请参阅 [Intel® 硬件加速执行管理器安装说明 - Microsoft Windows](http://go.microsoft.com/fwlink/p/?LinkId=536385)。

- 
构建适用于 iOS 的代码需要 Apple ID、iOS 开发人员程序帐户和可在 OS X Mavericks 或更高版本上运行 [Xcode 6](http://go.microsoft.com/fwlink/p/?LinkId=536387) 的 Mac 计算机。 有关简单的安装步骤，请参阅[安装针对 iOS 的工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#InstallForiOS)。



获取工具








用于跨平台移动开发的 Visual C++ 是包含在 Visual Studio 中的可安装组件。 若要获取 Visual Studio，请转到 [Visual Studio 2015 下载](http://go.microsoft.com/fwlink/p/?linkid=517106)页面，并下载 Visual Studio 2015。


安装工具








Visual Studio 2015 的安装程序包括安装用于跨平台移动开发的 Visual C++ 的选项。 这将安装所需的 Visual Studio 模板和组件、Android 生成和调试需要的 GCC 和 Clang 工具集，以及与用于 iOS 开发的 Mac 进行通信的组件。 它还会安装所有第三方工具和支持 iOS 和 Android 应用开发所需的软件开发工具包。 这些大部分第三方工具都是 Android 平台支持所需的开放源代码软件。
- 
构建面向 Android 平台的 C++ 代码需要 Android 本机开发工具包。

- 
Android 生成过程需要 Android SDK、Apache Ant 和 Java SE 开发工具包。

- 
适用于 Android 的 Microsoft Visual Studio 仿真程序是用于测试和调试你的代码的可选高性能仿真程序。


#### 要安装用于跨平台移动开发的 Visual C++ 和第三方工具
- 
运行跟随 [获取工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#GetTheTools) 中的链接下载的 Visual Studio 2015 安装程序。 若要安装可选组件，请选择“自定义”作为安装类型。 选择“下一步”以选择要安装的可选组件。

- 
Visual C++ 现在是 Visual Studio 的可选组件。 如果它尚未安装，请在“选择功能”中展开“编程语言”并勾选“Visual C++”。

- 
在“选择功能”中，展开“跨平台移动开发”，然后勾选“Visual C++ 移动开发”。

![Select Visual C++ Mobile Development](https://i-msdn.sec.s-msft.com/dynimg/IC829297.jpeg)

默认情况下，当你选择“Visual C++ 移动开发”时，“常用的工具和软件开发工具包”选项也被设置为安装所需的第三方组件。 适用于 Android 的 Microsoft Visual Studio 仿真程序也处于选中状态。 已安装的组件在列表中显示为非活动状态。

选择“下一步”继续。

- 
第三方组件都具有其自己的许可条款。 可以通过选择各个组件旁边的“许可条款”链接查看许可条款。 选择“安装”以添加组件并安装 Visual Studio。

安装完成后，重新启动计算机。

|![System_CAPS_ICON_important.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC160177.jpeg) 重要事项|
|----|
|你必须重新启动以确保所有软件都得到了正确安装。|


- 
打开 Visual Studio。 如果这是你第一次运行 Visual Studio，则可能需要一些时间来配置和登录。 Visual Studio 准备就绪后，在“工具”菜单上选择“扩展和更新”、“更新”。 如果有用于跨平台移动开发的 Visual C++ 或适用于 Android 的 Microsoft Visual Studio 仿真程序的 Visual Studio 可用更新，则安装它们。



安装针对 iOS 的工具








可以使用用于跨平台移动开发的 Visual C++ 来编辑、调试 iOS 代码，并将其部署到 iOS 仿真程序或 iOS 设备，但由于许可限制，该代码必须在 Mac 上远程生成。 若要使用 Visual Studio 生成和运行 iOS 应用，必须在 Mac 上安装并配置远程代理。 有关详细的安装说明、先决条件和配置选项信息，请参阅[安装并配置使用 iOS 进行生成的工具](https://msdn.microsoft.com/zh-cn/library/mt147405.aspx)。 如果你不是针对 iOS 构建，则可以跳过此步骤。


手动安装或更新依赖项








如果在安装 Visual C++ 移动开发选项时，你决定不使用 Visual Studio 安装程序安装一个或多个第三方依赖项，则可以通过使用 [安装工具](https://msdn.microsoft.com/zh-cn/library/dn707598.aspx#InstallTheTools) 中的步骤稍后安装它们。 你还可以独立于 Visual Studio 安装或更新它们。

|![System_CAPS_ICON_caution.jpg](https://i-msdn.sec.s-msft.com/dynimg/IC1429.jpeg) 小心|
|----|
|你可以按照任何顺序安装依赖项（不包括 Java）。 必须先安装并配置 JDK 才能安装 Android SDK。|


阅读以下信息并使用这些链接来手动安装依赖项。
- 
[Java SE 开发工具包](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)

默认情况下，安装程序将 Java 工具放置在以下路径：C:\Program Files (x86)\Java。

- 
[Android SDK](https://developer.android.com/sdk/index.html#Other)

在安装过程中按照推荐更新 API。 确保至少安装了适用于 Android 4.4.x (API 级别 19) 的 SDK。 默认情况下，安装程序将 Android SDK 放置在以下路径 C:\Program Files (x86)\Android\android-sdk。

你可再次运行 Android SDK 目录中的 SDK Manager 应用，以更新 SDK 并安装可选工具。 除非你使用“以管理员身份运行”运行 SDK Manager 应用，否则安装更新可能会失败。 如果构建 Android 应用存在问题，请检查已安装的 SDK 的 SDK Manager 更新。

若要使用某些 SDK 附带的 Android 仿真程序，则需安装可选的 Intel HAXM 驱动程序。 你可能需要从 Windows 中暂时移除 HYPER-V 功能才能成功安装 Intel HAXM 驱动程序。 必须还原 HYPER-V 功能，以使用 Android 的 Windows Phone 仿真程序和 Microsoft Visual Studio Emulator for Android。

- 
[Android NDK](https://developer.android.com/tools/sdk/ndk/index.html)

默认情况下，安装程序将 Android NDK 放置在以下路径：C:\ProgramData\Microsoft\AndroidNDK。 你可再次下载和安装 Android NDK，以更新 NDK 安装。

- 
[Apache Ant](http://ant.apache.org/bindownload.cgi)

默认情况下，安装程序将 Apache Ant 1.9.3 放置在以下路径：C:\Program Files (x86)\Microsoft Visual Studio 14.0\Apps\apache-ant-1.9.3。

- 
[适用于 Android 的 Microsoft Visual Studio 仿真程序](http://go.microsoft.com/fwlink/p/?LinkId=536390)

可以从 Visual Studio 库安装和更新适用于 Android 的 Microsoft Visual Studio 仿真程序。


在大多数情况下，Visual Studio 可以检测到已安装的第三方软件的配置，并维护内部环境变量中的安装路径。 可以覆盖 Visual Studio IDE 中的这些跨平台开发工具的默认路径。

#### 若要设置第三方工具的路径
- 
在 Visual Studio 菜单栏上依次选择“工具”、“选项”。

- 
在“选项”对话框框中，展开”跨平台”、“C++”，然后选择“Android”。

![Android tool path options](https://i-msdn.sec.s-msft.com/dynimg/IC829298.jpeg)

- 
若要更改工具使用的路径，请选中该路径旁的复选框，并在文本框中编辑文件夹路径。 还可以使用浏览按钮 (...) 打开“选择位置”对话框以选择文件夹。

- 
选择“确定”以保存自定义工具文件夹位置。






from:https://msdn.microsoft.com/zh-cn/library/dn707598.aspx









