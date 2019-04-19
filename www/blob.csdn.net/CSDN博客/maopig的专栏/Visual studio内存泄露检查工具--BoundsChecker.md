# Visual studio内存泄露检查工具--BoundsChecker - maopig的专栏 - CSDN博客
2011年11月29日 23:01:36[maopig](https://me.csdn.net/maopig)阅读数：1683
BoundsChecker是一个Run-Time错误检测工具，它主要定位程序在运行时期发生的各种错误。   
BoundsChecker能检测的错误包括：
1）指针操作和内存、资源泄露错误，比如：内存泄露；资源泄露；对指针变量的错误操作。
    2）内存操作方面的错误，比如：内存读、写溢出；使用未初始化的内存。
3）API函数使用错误。
    使用BoundsChecker对程序的运行时错误进行检测，有两种使用模式可供选择。一种模式叫做ActiveCheck，一种模式叫做FinalCheck。下面分别进行介绍。
    1）ActiveCheck是BoundsChecker提供的一种方便、快捷的错误检测模式，它能检测的错误种类有限，只包括：内存泄露错误、资源泄露错误、API函数使用错误。
    要想使用ActiveCheck模式来检测程序的运行时错误，只需在VC++集成开发环境中打开BoundsChecker功能，然后从调试状态运行程序即可。此时ActiveCheck会在后台自动运行，随时检测程序是否发生了错误。下面说一下具体的使用步骤。
    首先，在VC++集成开发环境中打开你要对其进行测试的程序，同时保证项目处于Debug编译状态下。
   其 次，确保VC++集成开发环境中[BoundsChecker/Integrated Debugging]菜单项和[BoundsChecker/Report Errors and Events]菜单项处于被选中的状态。只有这两项被选中，BoundsChecker才会在程序运行过程中发挥作用。
最后，在VC++集成开发环境中选择[Build/ Start Debug/Go]菜单命令，在Debug状态下运行程序，ActiveCheck也在后台开始运行了。
     2）FinalCheck具有BoundsChecker提供的所有检错功能。 FinalCheck 是ActiveCheck的超集，它除了能够检测出ActiveCheck能够检测出的错误，还能发现很多 ActiveCheck 不能检测到的错误，包括：指针操作错误、内存操作溢出、使用未初始化的内存等等，并且，对于ActiveCheck能检测出的错误，FinalCheck 能够给出关于错误更详细的信息。所以，我们可以把FinalCheck认为是ActiveCheck的功能增强版。我们付出的代价是：程序的运行速度会变
 慢，有时甚至会变的很慢。
      要 想在FinalCheck 模式下测试程序，不能使用VC++集成开发环境提供的编译连接器来构造程序，而必须要使用BoundsChecker提供的编译连接器来编译连接程序。当 BoundsChecker的编译连接器编译连接程序时，会向程序中插装一些错误检测代码，这也就是FinalCheck能够比ActiveCheck找 到更多错误的原因。
下面就介绍一下如何在FinalCheck模式下对程序进行测试：
 1）在VC++集成开发环境中打开你所要测试的项目。
  2）由于要使用BoundsChecker的编译连接器重新编译连接程序，所以我们为BoundsChecker独自构造一个文件夹。在VC++集成开发环境中，具体操作方法是：
    A)点击[ Build/Configurations...]菜单命令。
    B)在弹出的对话框中点击 Add 按钮。在Configuration 编辑框中添入你为BoundsChecker创建的文件夹的名称，这个名称是任意的，比如我们取名为BoundChecker。
C)在 Copy settings from组合框中选中XXX—Win32 Debug项，然后点击OK按钮，接着点击Close按钮。
现在，我们已经为FinalCheck构造好了一个文件夹。
   3） 点击[Build/Set Active Configuration…] 菜单命令，选中你刚才为BoundsChecker建的文件夹，然后点击OK按钮。这样BoundsChecker编译连接程序时生成的中间文件、可执行程序，都会被放到该文件夹下。
    4）选择[BoundsChecker/Rebuild All with BoundsChecker] 菜单命令，对程序重新进行编译连接，也就是在这时，BoundsChecker向被测程序的代码中加入了错误检测码。编译连接完成后， BoundsChecker会在你为BoundsChecker构造的文件夹中生成可执行文件。
      在FinalCheck模式下对程序进行检测的准备工作都已经做好，这时可以启动程序开始测试了，作步骤与在ActiveChecker模式下没什么区别。具体步骤如下：
确保VC++集成开发环境中[BoundsChecker/ Integrated Debugging]菜单项和[BoundsChecker/Report Errors and Events]菜单项处于选中状态。     好 了，BoundsChecker在FinalCheck模式下的使用也介绍完了。ActiveChecker、FinalCheck这两种模式，比较而言 各有长短。ActiveChecker使用方便，只需在Debug状态下直接运行程序即可，并且程序的运行速度较快，但检测的错误种类有限；
 FinalCheck模式下，需要使用BoundsChecker的编译连接器重新编译连接生成可执行程序，并且程序的运行速度比较慢，但检测的错误种 类、提供的错误相关信息要多于ActiveChecker。所以，何时使用何种模式，应根据当时的具体情况而定。
