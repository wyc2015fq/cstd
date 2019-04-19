# 【调试】VS中的调试技巧(2) - 三少GG - CSDN博客
2011年05月27日 14:44:00[三少GG](https://me.csdn.net/scut1135)阅读数：1630
[http://msdn.microsoft.com/en-us/library/ff541809](http://msdn.microsoft.com/en-us/library/ff541809)
# Debug | Run to Cursor
Click **Run to Cursor** on the **Debug** menu to resume running on the target. If you insert the cursor on an instruction in the[Disassembly window](http://msdn.microsoft.com/en-us/library/ff541965)
 or a[Source window](http://msdn.microsoft.com/en-us/library/ff556911) and then execute this action, WinDbg executes all instructions from the current instruction up to the instruction you have selected.
This command is equivalent to pressing F7 or CTRL+F10 or clicking the**Run to cursor (Ctrl+F10 or F7)**button (![Screen shot of the Run to Cursor button](http://i.msdn.microsoft.com/dynimg/IC69354.bmp)) on the toolbar.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://www.blogjava.net/tidelgl/archive/2008/08/19/223051.html](http://www.blogjava.net/tidelgl/archive/2008/08/19/223051.html)
**Visual C++ 6.0调试功能**
这是我自己在学习时整理的,希望对大家有用.其中有几个部分的内容,如下: 
**※My Note **
全部由我截图并参考资料进行说明,这其中有大量的内容是调试过程中体会总结才写的.有不当之处请大家更正.
**※From 《Visual C++ Debugger》 **
这本书写得非常的不错,从里面截了一些有用的图,主要是强烈推荐大家阅读这本书. 
**※Debug Menu From MSDN **
这是我从Visual C++ 6.0 MSDN Library October 2001中查来的,在My Note中有很多是从那里参考的.强烈推荐大家看这份英文说明.
**※From 《visual C++ 6.0开发工具与调试》 **
这是我从网络上搜索到的,问题很经典,强烈推荐大家参看学习,避免不必要的错误. 
**My Note **
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_1.png)
○常用功能: 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_2.png)
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_3.png)**Restart**(Ctrl+shift+F5):此debugger功能将从程序的开始(第一有效行)处全速执行,而不是从当前所跟踪的位置开始调
试,这时所有变量的当前值都将被丢弃,debugger会自动停在程序的main()开始处.这时如果选择**Step Over**(F10)就可以逐步执行main()函数了.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_4.png)**Stop Debugging**(Shift+F5):此debugger功能将终止(所有)调试,并返回到常规编辑状态.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_5.png)**Break**(此功能常常在遇到调用函数的语句时可见.):此功能将在调试过程中的debugger当前位置挂起程序的执行,然后就可以在调
试状态一修改程序的代码,接着可以用**Apply Code Changes**(Alt+F10)来应用修改的代码到正在调试的程序当中.如果,当前(需要,待)可以(从DOS
等窗口)输入值,挂起后将不能再输入. 
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_6.png)**Apply Code Changes**(Alt+F10):此功能可以在程序正在调试程序过程中应用(挂起)修改后的源代码.如,选择**Break**功能并修改代
码后,只要选择**Apply Code Changes**(Alt+F10)就能将修改后的代码应用到正在调试的程序当中.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_7.png)**Show Next Statement**(Alt+Num*):此功能将显示程序代码的下一条语句,如果源代码中找不到,则在Disassembly窗口中显示语句.
当在Disassembly窗口中显示时,可以单击![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_8.png)Disassembly 返回到源代码窗口.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_9.png)**Step Into**(F11):此功能可以单步进入到在调试过程中所跟踪的调用函数的语句的函数内部.如,当前语句是"d.Display()",
选择**Step Into**(F11)后,Debugger将进入Display()函数内部并停在Display()函数内部的第一条语句上.(此时,
就可以**Step Over**(F10)对Display()函数进行单步调试了.)
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_10.png)**Step Over**(F10):此功能可以单步对所在函数单步调试,如果调试的语句是一个调用函数的语句时,
Debugger将全速执行所调用的函数,单步(一步)通过所调用的函数,Debugger停该调用语句的下一条语句上.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_11.png)**Step Out**(Shift+F11):此功能将使Debugger切换回全速执行到被调用函数结束,并停在该函数调用语句的下一条语句上.
当确定所调用的函数没有问题时可以用这个功能全速执行被调用函数. 
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_12.png)**Run to Cursor**(Ctrl+F10):此功能将全速执行到包含插入点光标所在的行,可以作为在插入点光标处设置常规断点的一种选择.
注意,当光标处不是一个有效的执行语句时此功能将不起作用.
- 
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_13.png)**Go**(F5):此功能将全速执行程序直到遇到一个断点或程序结束,或直到程序暂停等待用户输入.
注意,此功能最能有效的调试循环,常将断点设置在循环体内,重复的按F5全速执行循环体可以测试循环过程中的产生的变化.
- 
Step Into Specific Function:此功能可以可以单步通过程序中的指令,并进入指定的函数调用,此功能对于函数的嵌套层不限.

**调试常用快捷键 **
|**单步进入**|**F11**|
|----|----|
|**单步跳过**|**F10**|
|**单步跳出**|**SHIFT+F11**|
|**运行到光标**|**CTRL+F10**|
|**开关断点**|**F9**|
|**清除断点**|**CTRL+SHIFT+F9**|
|**Breakpoints(断点管理)**|**CTRL+B 或ALT+F9**|
|**GO**|**F5**|
|**Compile(编译,生成.obj文件)**|**CTRL+F7**|
|**Build(组建,先Compile生成.obj再Link生成.exe)**|**F7**|
**From 《Visual C++ Debugger》 **
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_14.png)
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_15.png)
![](http://www.blogjava.net/images/blogjava_net/tidelgl/081908_0841_16.png)
**Debug Menu From MSDN **
**Start Debug Commands (Build menu)**
|**Menucommand**|**Action**|
|----|----|
|**Go**|Executes code from the current statement until a breakpoint or the end of the program is reached, or until the application pauses for user input. (Equivalent to the**Go** button on the toolbar.)|
|**Step Into**|Single-steps through instructions in the program, and enters each function call that is encountered.|
|**Run to Cursor**|Executes the program as far as the line that contains the insertion point. This is equivalent to setting a temporary breakpoint at the insertion point location.|
|**Attach to Process**|Attaches the debugger to a process that is running. Then you can break into the process and perform debugging operations like normal.|
When you begin debugging, the **Debug** menu appears, replacing the**Build** menu on the menu bar. You can then control program execution using the commands
listed in the following table. 
**Debug Menu Commands that Control Program Execution**
|**Debug menucommand**|**Action**|
|----|----|
|**Go**|Executes code from the current statement until a breakpoint or the end of the program is reached, or until the application pauses for user input. (Equivalent to the**Go** button on the Standard toolbar.) When the**Debug** menu is not available, you can choose**Go** from the**Start Debug** submenu of the **Build** menu.|
|**Restart**|Resets execution to the first line of the program. This command reloads the program into memory, and discards the current values of all variables (breakpoints and watch expressions still apply). It automatically halts at the**main( )** or **WinMain( )** function.|
|**Stop Debugging**|Terminates the debugging session, and returns to a normal editing session.|
|**Break**|Halts the program at its current location.|
|**Step Into**|Single-steps through instructions in the program, and enters each function call that is encountered.  When the**Debug** menu is not available, you can choose**Step Into** from the**Start Debug** submenu of the **Build** menu.|
|**Step Over**|Single-steps through instructions in the program. If this command is used when you reach a function call, the function is executed without stepping through the function instructions.|
|**Step Out**|Executes the program out of a function call, and stops on the instruction immediately following the call to the function. Using this command, you can quickly finish executing the current function after determining that a bug is not present in the function.|
|**Run to Cursor**|Executes the program as far as the line that contains the insertion point. This command is equivalent to setting a temporary breakpoint at the insertion point location. When the**Debug** menu is not available, you can choose **Run To Cursor** from the**Start Debug** submenu of the**Build** menu.|
|**Step Into Specific Function**|Single steps through instructions in the program, and enters the specified function call. This works for any number of nesting levels of functions.|
The following additional commands appear on the Debug menu:
|**Exceptions**|Displays the Exceptions dialog, which you can use to specify how you want the debugger to handle your program exceptions.|
|----|----|
|**Threads**|Displays the Threads dialog, which you can use to suspend, resume, or set focus to progam threads.|
|**Show Next Statement**|Shows the next statement in your program code. If source code is not available, displays the statement within the Disassembly window.|
|**QuickWatch**|Displays the Quick Watch window, where you can work with expressions.|
**From 《visual C++ 6.0开发工具与调试》**
1、如何快速地规范代码缩进格式
选中所需要规范的代码，按shift+F8
2、如何在Release状态下进行调试
Project－>Setting＝>ProjectSetting对话框，选择Release状态。C/C++标签中的Category选General，Optimizations选Disable(Debug)，
Debut info选Program Database。在Link标签中选中Generate
 debug info复选框。
注：只是一个介乎Debug合Release的中间状态，所有的ASSERT、VERIFY都不起作用，函数调用方式已经是真正的调用，而不查表，
但是这种状态下QuickWatch、调用队列跟踪功能仍然有效，和Debug版一样。
3、Release和Debug有什么不同。
Release版称为发行版，Debug版称为调试版。
Debug中可以单步执行、跟踪等功能，但生成的可执行文件比较大，代码运行速度较慢。Release版运行速度较快，可执行文件较小，
但在其编译条件小无法执行调试功能。
Release的exe文件链接的是标准的MFC DLL(Use MFC in a shared or static dll),比如MFC42.DLL。这些DLL在安装Windows的时候，
已经配置，所以这些程序能够在没有安装Visual
 C++ 6.0的机器上运行。而Debug版本的exe链接了调试版本的MFC DLL文件，如MFC42D.DLL。
在没有安装Visual C++6.0的机器上不能运行，因为缺MFC42D.DLL等，除非选择use
 static dll when link。
4、ASSERT和VERIFY有什么区别
ASSERT里面的内容在Release版本中不编译，VERIFY里面的内容仍然翻译，但不再判断真假。所以后者更安全一点。
例如ASSERT(file.Open(strFileName))。
一旦到了Release版本中，这一行就忽略了，file根本就不Open()了，而且没有任何出错的信息。如果用VERIFY()就不会有这个问题。
5、Workspace和Project之间是什么样的关系
每个Workspace可以包括几个project，但只有一个处于Active状态，各个project之间可以有依赖关系，在project的Setting..中可以设定，
比如那个Active状态的project可以依赖于其他的提供其函数调用的静态库。
6、如何在非MFC程序中使用ClassWizard
在工程目录下新建一个空的.RC文件，然后加入到工程中就可以了。
7、如何设置断点
按F9在当前光标处增加一个断点和取消一个断点。
另外，在编辑状态下，按Ctrl+B组合键，弹出断点设置对话框。然后单击【Condition…】按钮弹出设置断点条件的对话框进行设置。
8、在编辑状态下发现成员变量或函数不能显示提示是如何打开显示功能
这似乎是目前这个Visual C++ 6.0版本的一个bug，可按如下步骤使其正常，如再出现，可如法炮制：
(1)关闭Project
(2)删除"工程名.ncb"文件
(3)重新打开工程
9、如何将一个通过ClassWizard生成的类彻底删除
首先在工作区的FileView中选中该类的.h和.cpp文件，按delete删除，然后在文件管理器中将这两个文件删除，再运行ClassWizard，
这时出现是否移走该类的提示，选择remove就可以了。
10、如何将再workspace中消失的类找出来
打开该类对应的头文件，然后将其类名随便改一下，这个时候工作区就会出现新的类，再将这个类改回原来的名字就可以了。
11、如何清除所有的断点
菜单【Edit】－>【Breakpoints…】，打开"Breakpoints"对话框，单击【Remove
 All】按钮即可。
快捷键是"Ctrl + Shift + F8"。
12、如何再ClassWizard中选择未列出的信息
打开"ClassWizard"对话框，然后切换到"Class Info"页面。改变"Message
 filter"，如选择"Window"，"Message"页面就会出现Window的信息。
13、如何检测程序中的括号是否匹配
把光标移动到需要检测的括号前面，按快捷键"Ctrl
 + ]"。如果括号匹配正确，光标就跳到匹配的括号处，否则光标不移动，
并且机箱喇叭还会发出一声警告。
14、如何查看一个宏(或变量、函数)的定义
把光标移动到要查看的一个宏上，就比如说最常见的DECLARE_MAP_MESSAGE上按一下F12(或右键菜单中的相关菜单)，
如果没有建立浏览文件，就会出现提示对话框，按【确定】按钮，然后就会跳到该宏(或变量、函数)定义的地方。
15、如何添加Lib文件到当前工程
单击菜单【Project】－>【Settings…】弹出"Project Setting"对话框，切换到"Link"标签页，在"Object/library
 modules"处输入Lib文件名称， 
不同的Lib之间用空格格开。
16、如何快速删除项目下的Debug文件夹中临时文件
在工作区的FileView视图中选中对应的项目，单击右键弹出菜单，选择【Clean(selection only)】菜单即可。
17、如何快速生成一个现有工程除了工程名外完全相同的新工程。
在新建工程的"New"对话框中选择"Custom Appwizard"项，输入新工程的名字，单击【OK】按钮。出现"Custom
 AppWizard"项， 
输入新工程的名字，单击【OK】按钮。出现"Custom AppWizard－Step 1 of 2"对话框，选择"An
 existing Project"项，单击【Next】按钮。 
出现"Custom AppWizard－Step 2 of 2"对话框，选择现有工程的工程文件名，最后单击【Finish】按钮。编译后就生成一个与现有工程相同
但可以重新取名的工程AppWizard。
现在就可以项用MFC AppWizard一样用这个定制的向导。如果不想用了，可以在Visual C++ 6.0安装目录下Common/MSDev98/Template目录
中删除该Wizard对应的.awx和.pdb文件。
18、如何解决Visual C++ 6.0不正确连接的问题
情景：明明之间改动了一个文件，却要把整个项目全部重新编译链接一次。刚刚链接好，一运行，有提示重新编译链接一次。
这是因为出现了未来文件(修改时间和创建时间比系统时间晚)的缘故。可以这样处理：找到工程文件夹下的debug目录，
将创建和修改时间都比系统时间的文件全部删除，然后再从新"Rebuild All"一次。
19、引起LNK2001的常见错误都有哪些
遇到的LNK2001错误主要为：unresolved external symbol"symbol"
如果链接程序不能在所有的库和目标文件内找到所引用的函数、变量或标签，将产生此错误信息。
一般来说，发生错误的原因有两个：一时所引用的函数、变量不存在，拼写不正确或者使用错误；其次可能使用了不同版本的链接库。
一下是可能产生LNK2001错误的原因：
<1>由于编码错误导致的LNK2001错误
- 
不相匹配的程序代码或模块定义(.DEF)文件导致LNK2001。例如，如果在C++源文件了内声明了一变量"var1"，
却视图在另一个文件内以变量"var1"访问改变量。
(2)如果使用的内联函数是在.cpp文件内定义的，而不是在头文件内定义将导致LNK2001错误。
(3)调用函数是如果所用的参数类型头函数声明是的类型不符将会产生LNK2001错误。
(4)视图从基类的构造函数或析构函数中调用虚拟函数时将会导致LNK2001错误。
(5)要注意函数和变量的可公用性，只有全局变量、函数时可公用的。静态函数和静态变量具有相同的使用范围限制。
当试图从文件外部方位任何没有在该文件内声明的静态变量时将导致编译错误或LNK2001错误。
<2>由于编译和联机的设置而造成的LNK2001错误
- 
如果编译时使用的时/NOD(/NODERAULTLIB)选项，程序所需要的运行库和MFC时将得到又编译器写入目标文件模块，
但除非在文件中明确包含这些库名，否则这些库不会北链接进工程文件。这种情况下使用/NOD将导致LNK2001错误
- 
如果没有为wWinMainCRTStartup设定程序入口，在使用Unicode和MFC时讲的到
"unresolved external on _WinMain@16"的LNK2001错误信息。
- 
使用/MD选项编译时，既然所有的运行库都被保留在动态链接库之内，源文件中对"func"的引用，
在目标文件里即对"__imp__func"的引用。如果试图使用静态库LIBC.LIB或LIBCMT.LIB进行链接，将在__imp__func上发生LNK2001错误。如果不使用/MD选项编译，在使用MSVCxx.LIB链接时也会发生LNK2001错误。
(4)使用/ML选项编译时，如用LIBCMT.LIB链接回在_errno上发生LNK2001错误。
(5)当编译调试版的应用程序时，如果采用发行版模态库进行链接也会产生LNK2001错误；同样，
使用调试版模态库链接发行版应用程序时也会产生相同的错误。
(6)不同版本的库和编译器的混合使用也能产生问题，因为新版的库里可能包含早先的版本没有的符号和说明。
(7)在不同的模块中使用内联和非内联的编译选项能够导致LNK2001错误。如果创建C++库时打开了
函数内联(/Ob1或/Ob2)，但是在描述该函数的相应头问卷安里却关闭了函数内联(没有inline关键字)，
只是将得到错误信息。为避免该问题的发生，应该在相应的头文件中用inline关键字标志为内联函数。
(8)不正确的/SUBSYSTEM或ENTRY设置也能导致LNK2001错误。
20、如何调试一个没有源码的exe文件调用的dll
在Visual C++ 6.0中，进入"Project Setting"对话框然后选择Debug标签页。通常Visual
 Studio默认"executable for debug session"为可执行文件名，
但可以将他改成任何你想要的程序。甚至可以指定不同的工作目录以及传递参数到你的程序。这个技术常用来调试Dlls、名字空间扩展、
COM对象和其他从某些EXE以及从第三方的EXE中调用的plug-in程序。
21、Visual C++ 6.0工程中的项目文件都表示什么。
.opt：工程关于开发化境的参数文件。如工具条位置等信息。
.aps(AppStudio File)资源辅助文件，二进制格式，一般不用去管他。
.clw：ClassWizard信息文件，实际上是INI文件格式，又兴趣可以研究一下。有时候ClassWizard出了问题，手工修改CLW文件可以解决。
如果此文件不存在的话，每次用ClassWizard的时候回提示是否重建。
.dsp(DevelopStudio Project)：项目文件，文本格式，不过不熟悉的或不要手工修改。
.dsw(DevelopStudio Workspace)：是工作区文件，其他特点和.dsp差不多。
.plg：是编译信息文件，编译时的error和warning信息文件(实际上时一个html文件)，一般用处不大。在单击菜单【Tool】－>【Option】
弹出的对话框里面有个选项可以控制这个文件的生成。
.hpj(Help Project)：是生成帮助文件的工程，用microsoft Help Compiler可以处理。
.mdp(Microsoft DevStudio Project)：是旧版本的项目文件，如果要打开此文件的话，回提示你是否转换成新的.dsp格式。
.bsc：是用于浏览项目信息的，如果用Source Brower的话旧必须又这个文件。如果不用这个功能的话，可以在Project Options里面去掉
Generate Browse Info File，这样可以加快编译速度。
.map是执行文件的影像信息记录文件，除非对系统底层，这个文件一般用不着。
.pch(Pre-Compiled File)：是与编译文件，可以加快编译速度，但是文件非常大。
.pdb(Program Database),记录了程序有关的一些数据和调试信息，在调试的时候可能有用。
.exp：只有在编译DLL的时候才会生成，记录了DLL文件的一些信息，一般也没有用。
.ncb：无编译浏览文件(no compile browser)。当自动完成功能出问题时可以删除此文件。编译工程后回自动生成。
                                                                                                  声明:以上为从的个人初学时的学习笔记中整理制作而成,有不当之处请自行更正.
                                                                     ∪∩BUG
