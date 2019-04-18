# VC中IDE的一些使用小技巧 - L_Andy的专栏 - CSDN博客

2012年10月09日 11:19:30[卡哥](https://me.csdn.net/L_Andy)阅读数：1097


                
1.检测程序中的括号是否匹配 

把光标移动到需要检测的括号（如大括号{}、方括号[]、圆括号（）和尖括号<>）前面，键入快捷键“Ctrl＋]”。如果括号匹配正确，光标就跳到匹配的括号处，否则光标不移动，并且机箱喇叭还会发出一声警告声。

2.查看一个宏（或变量、函数）的宏定义 

把光标移动到你想知道的一个宏上，就比如说最常见的DECLARE_MAP_MESSAGE上按一下F12(或右键菜单中的Go To Defition Of …),如果没有建立Browse files，会出现提示对话框，确定，然后就会跳到定义那些东西的地方。

3.格式化一段乱七八糟的源代码 

选中那段源代码，按ATL+F8。 

4.在编辑状态下发现成员变量或函数不能显示 

删除该项目扩展名为.ncb文件，重新打开该项目。 

5.如何整理ClassView视图中大量的类 

可以在classview 视图中右键新建文件夹（new folder），再把具有相近性质的类拖到对应的文件夹中，使整个视图看上去清晰明了 

. 

6.定位预处理指定 

在源文件中定位光标到对称的#if, #endif,使用Ctrl+K. 

7.如何添加系统中Lib到当前项目 

在Project | Settings | Link | Object/library modules：输入Lib名称，不同的Lib之间用空格格开. 

8.如何添加系统中的头文件(.h)到当前项目. 

#include <FileName.h>,告诉编译到VC系统目录去找;使用#include "FileName.h"，告诉编译在当前目录找. 

9.如何在Studio使用汇编调试 

在WorkBench的Debugger状态下按CTRL+F7. 

10.怎样处理ClassZiard找不到的系统消息 

如果要在ClassWizard中处理WM_NCHITTEST等系统消息，请在ClassWizard中Class Info页中将Message filter改为Window就有了.

11.如何干净的删除一个类 

先从Workspace中的FileView中删除对应的.h和.cpp文件,再关闭项目，从实际的文件夹中删除对应的.h和.cpp文件与.clw文件。 

12.如果让控制台应用程序支持mfc类库 

可 以在控制台应用程序中include 来引入mfc库，但是控制台应用程序缺省是单线程的，mfc是多线程的，为解决该矛盾，在project setting->c/c++ 选项，选择code generation,在use run-time library 下拉框中选择debug multithread。

13.如何汉化只有可执行代码的.exe 文件 

在nt 下利用vc open file 以resources方式打开*.exe 文件，直接修改资源文件，然后保存即可。 

**14.VC常用快捷键**

**Alt+F7**：Project Settings

**Ctrl+J/K**：寻找上一个/下一个预编译条件

**Ctrl+Shift+J/K**：寻找上一个/下一个预编译条件并将这一块选定

**Ctrl+Tab**：当前打开文档和上一次打开文档切换。注

Ctrl+TAB 切换打开的文件视图,(如果按住Ctrl,顺序向后切换)

Ctrl+Shift+TAB切换打开的文件视图,(如果按住Ctrl,顺序向前切换)

**Alt+O**：头文件与cpp文件的交互显示

**F12**：Goto definition

**Shift+F12**：Goto reference

Ctrl+Home：文档头

Ctrl+Shift+Home：选定从当前位置到文档头

Ctrl+End：文档尾

Ctrl+Shift+End：选定从当前位置到文档尾

**Alt+F8**：自动格式重排

**Ctrl+F4**：关闭当前文档

Ctrl+Z/Alt+Backspace：Undo

Ctrl+Shift+Z/Ctrl+Y：Redo

Ctrl+B/Alt+F9：编辑断点

Ctrl+Shift+F9：去掉所有断点

**Ctrl+D**：定位工具条查找输入栏

Alt+F3/Ctrl+F：当前文档查找

Ctrl+F3：查找下一个

Ctrl+Shift+F3：查找上一个

Ctrl+H：替换

**Ctrl+]**：括号匹配

Ctrl+↑：向上滚屏

Ctrl+↓：向下滚屏

Ctrl+←：移到前一个单词

Ctrl+→：移到后一个单词

Ctrl+Shift+←：选定当前位置到前一个单词

Ctrl+Shift+→：选定当前位置到后一个单词

**Ctrl+Shift+]**：括号匹配块选

**Ctrl+Shift+F8**：配合鼠标和方向键实现竖直块选

Ctrl+Backspace：删除当前单词的前半截（以光标为分割）

Ctrl+Del：删除当前单词的后半截（以光标为分割）

**Ctrl+L**：剪切当前行

**Ctrl+Shift+L**：删除当前行

F2：上一个书签

Shift+F2：上一个书签

Alt+F2：编辑书签

Ctrl＋F2：添加/删除一个书签

Ctrl+Shift+F2：清除所有书签

**15.智能提示的选择**

对于弹出函数智能提示这种情况，可能有重载函数，由于参数有多种选择，那么可以通过“上下左右”来选择，因为如果通过鼠标来点击太麻烦。

**16.快速消除注释**

有时经常你会写下面的注释：即把//
放在语句第一个字母的前面，而VA所给的注释则是在每一行的开头，去除注释的时候也要求注释符号//位于每行最前面。那么如何利用VA来去除这段“非标准”的注释呢？我们可以先选中该段代码，然后按住Shift+tab（注意是tab键的反功能键），直到所有的//都在每行最前面，然后用VA就可以去除注释了。

**17.定制工作台（Workspace）**

---- 有没有想过在启动VC时将上一次退出时的工程自动载入并打开退出时处于编辑态的文档？试一试下面的方法。

---- （1）选择Tools菜单中的Cutomize...菜单项； 

---- （2）选择Workspace属性页，现在我们有多种选择： 

---- <1>选中Reload last workspace at startup可以让VC在启动时自动载入上次编辑的工程。

---- <2>选中Reload documents when opening workspace可以在打开工作台时自动打开退出时处于编辑状态的文件。否则在打开工作台时所有的文件都不处于编辑状态。

---- <3>选中Show recently used items on submenus将把最近编辑的文件和工作台的列表分别放在File菜单的

---- <4>Recent Files和 Recent Workspaces菜单项中。否则将直接放在File菜单中。

---- <5>属性页的下方可以指定Window菜单可显示的窗口数、最近使用的文件数和最近使用的工作台数。

18.取得源程序预处理后的结果：

在Studio里，可以在->PROJECT->SETTINGS->C/C++->Project Options中，在最后加上 /P /EP这两个编译开关即可做到"只进行预处理".就可以了。编译以后就可以在源程序目录中发现“文件名.I ”的文本文件。这就是预处理后的结果。

（注意注：区分大小定，请用大定/P）

19.在Debug模式中查看WINAPI调用后的返回值：

很简单，且实用：在watch中加入@hr,err。在CSDN的文档中心有一篇讲得更细，请参考。

20.产生指定源程序文件的汇编代码：

从IDE菜单的Project->Setting打开项目设置，按如下文件做：

1.先在左边选择指定文件，可以多选。

2. 在右边的C++属性页中，在category中选择List Files,接着在下面的List Files Type中选择Assembly and source code（或选择其它），最后在List File Name中输入在个C/C++源文件产生的相应的汇编代码的文件。

3.编译整个工程。

21.手工编译纯资源成dll：

Rc.exe /v data.rc

Cvtres.exe /machine:ix86 data.res

Link /SUBSYSTEM:WINDOWS /DLL /NOENTRY data.res ;编译成DLL文件

这种方式创建的DLL是最小的，比起你用Win 32 Dynamic Libray等产生的更小。

22：怎样快速生成一个与现有项目除了项目名外完全相同的新项目? 

利用File菜单下生成新项目中的Custom AppWizard ，选择 An existing Project ,然后选择现有项目的项目文件名（*.dsp）Finish,编译后就生成一个可以生成与现有项目相同但可以重新取名的项目的AppWizard。你可以象用MFC AppWizard一样用它。如果不想用了，可以在VC 安装目录下Common/MSDev98/Template目录中删除该Wizard中.awx和 .pdb文件。　　

23:如果想把整个项目拷贝到软盘，那些文件可以删掉? 　 

　　除了项目文件夹中debug文件夹可以删除外，.ncb,.clw,.opt 等文件也可以删除，这些文件Rebuilt all后可以重新生成。

附：VC项目文件说明 

.opt 工程关于开发环境的参数文件。如工具条位置等信息； 

.aps (AppStudio File),资源辅助文件,二进制格式,一般不用去管他. 

.clw ClassWizard信息文件,实际上是INI文件的格式,有兴趣可以研究一下.有时候ClassWizard出问题,手工修改CLW文件可以解决.如果此文件不存在的话,每次用ClassWizard的时候绘提示你是否重建.

.dsp (DeveloperStudio Project):项目文件,文本格式,不过不熟悉的话不要手工修改.DSW(DeveloperStudio Workspace)是工作区文件,其他特点和DSP差不多.

.plg 是编译信息文件,编译时的error和warning信息文件（实际上是一个html文件）,一般用处不大.在Tools->Options里面有个选项可以控制这个文件的生成.

.hpj (Help Project)是生成帮助文件的工程,用microsfot Help Compiler可以处理. 

.mdp (Microsoft DevStudio Project)是旧版本的项目文件,如果要打开此文件的话,会提示你是否转换成新的DSP格式. 

.bsc 是用于浏览项目信息的,如果用Source Brower的话就必须有这个文件.如果不用这个功能的话,可以在Project Options里面去掉Generate Browse Info File,可以加快编译速度.

.map 是执行文件的映像信息纪录文件,除非对系统底层非常熟悉,这个文件一般用不着. 

.pch (Pre-Compiled File)是预编译文件,可以加快编译速度,但是文件非常大. 

.pdb (Program Database)记录了程序有关的一些数据和调试信息,在调试的时候可能有用. 

.exp 只有在编译DLL的时候才会生成,记录了DLL文件中的一些信息.一般也没什么用. 

.ncb 无编译浏览文件(no compile browser)。当自动完成功能出问题时可以删除此文件。build后会自动生成。

