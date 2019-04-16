# 一些VC的快捷键以及调试技巧 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月15日 12:10:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：26









1.检测程序中的括号是否匹配 
 把光标移动到需要检测的括号（如大括号{}、方括号[]、圆括号（）和尖括号<>）前面，键入快捷键“Ctrl＋]”。如果括号匹配正确，光标就跳到匹配的括号处，否则光标不移动，并且机箱喇叭还会发出一声警告声。 

 2.查看一个宏（或变量、函数）的宏定义 
 把光标移动到你想知道的一个宏上，就比如说最常见的DECLARE_MAP_MESSAGE上按一下F12(或右键菜单中的Go To Defition Of …),如果没有建立Browse files，会出现提示对话框，确定，然后就会跳到定义那些东西的地方。 

 3.格式化一段乱七八糟的源代码 
 选中那段源代码，按ATL+F8。 

 4.在编辑状态下发现成员变量或函数不能显示 
 删除该项目扩展名为.ncb文件，重新打开该项目。 

 5.如何整理ClassView视图中大量的类 
 可以在classview 视图中右键新建文件夹（new folder），再把具有相近性质的类拖到对应的文件夹中，使整个视图看上去清晰明了. 

 6.定位预处理指定 
 在源文件中定位光标到对称的#if, #endif,使用Ctrl+K. 

 7.如何添加系统中Lib到当前项目 
 在Project | Settings | Link | Object/library modules：输入Lib名称，不同的Lib之间用空格格开. 

 8.如何添加系统中的头文件(.h)到当前项目. 
 #include ,告诉编译到VC系统目录去找;使用#include "FileName.h"，告诉编译在当前目录找. 

 9.如何在Studio使用汇编调试 
 在WorkBench的Debugger状态下按CTRL+F7. 

 10.怎样处理ClassZiard找不到的系统消息 
 如果要在ClassWizard中处理WM_NCHITTEST等系统消息，请在ClassWizard中Class Info页中将Message filter改为Window就有了. 

 11.如何干净的删除一个类 
 先从Workspace中的FileView中删除对应的.h和.cpp文件,再关闭项目，从实际的文件夹中删除对应的.h和.cpp文件与.clw文件。 

 12.如果让控制台应用程序支持mfc类库 
 可以在控制台应用程序中include 来引入mfc库，但是控制台应用程序缺省是单线程的，mfc是多线程的，为解决该矛盾，在project setting->c/c++ 选项，选择code generation,在use run-time library 下拉框中选择debug multithread。 

 13.如何汉化只有可执行代码的.exe 文件 
 在nt 下利用vc open file 以resources方式打开*.exe 文件，直接修改资源文件，然后保存即可。 

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

 // 选择字符 
 1. (COOL)按下Alt 键不放，点击鼠标左键拖动，可以选择文本块、可选择列。 
 2. 安着Ctrl键不放，单击一个单词，可以选择一个单词，或双击。 
 3. 将光标移在开始位置，按住shift点击鼠标左键可选择一段（在IE浏览其中照样可 
 用，看不到光标而已, 
 4. 双击鼠标左键可选择一个单词。 
 5．按住shift+上下方向键 可选择行。 
 6．按住 ctrl+shift+左右方向键 可选择一个单词。 
 7. 按Ctrl + C 可COPY光标所在的这一行。 
 8. 按住shift+[End] 可选择本行。 

 //////////////////////////////////////

 F1 显示帮助,如果光标停在代码的某个字符上,显示MSDN中相应的帮助内容
 F2 书签功能: Ctrl+F2 --在某行设置一个书签(再按一次次是取消)
 F2 --跳到下一个书签位置
 Shift+F2 --跳到上一个书签位置
 Ctrl+Shift+F2 --删除所有书签
 F3 查找: Ctrl+F3 --在文件中查找,如果当前光标在一个字符串上,
 那么自动查找此字符串.相似的有Ctrl+F
 F3 --查找文件中下一个串
 Shift+F3 --查找文件中上一个串
 F4(不是流行花园那个) 如果是编译后或者Find in Files后,可以逐条定位.
 Ctrl+F4 --关闭文件
 Alt+F4 --关闭VC(跟WINDOWS定义的一样)
 F5编译并执行 F5 --编译并通过VC执行
 Ctrl+F5 --不经过VC,直接执行编译后的exe
 Shift+F5 --F5运行后,直接从VC中停止程序(可以模拟当机情况)
 Ctrl+Shift+F5 --重新开始运行
 F6切换窗口 SplitWindow后,可以转换光标所在的窗口
 F7编译 F7 编译工程
 Ctrl+F7 编译当前文件
 Alt+F7 工程设置对话框
 F8选择的粘滞键 其实更常用的是按住Shift+方向键
 Alt+F8 选中的代码书写格式对齐
 F9设置断点 Ctrl+F9删除所有断点
 Alt+F9 显示编辑断点的对话框
 Ctrl+F9断点无效
 F10单步执行(Debug时)
 Ctrl+F10 执行到光标所在行
 (试试看Shift+F10什么效果)
 F11跟踪时进入函数内部
 Shift+F11跳到上一层调用栈
 F12跳到函数定义初(需要带Browse Info选项编译,建议不选,生成文件很大)
 编辑常用
 Ctrl+W 显示ClassWizard,边界面程序用的比较多
 Ctrl+Z/Ctrl+Y Undo/Redo
 Ctrl+U 字母转化为小写(有的VC没有设置)
 Ctrl+Shift+U 字母转化为大写(有的VC没有设置)
 Ctrl+S 保存(强烈建议编码时经常按两下,省得掉电死机时后悔,呵呵,
 该键在多数编辑软件都适用)
 Ctrl+D 查找(嗯,如果有SOFTICE,那么就是呼叫它了)
 Ctrl+F 正宗的查找快捷键
 Ctrl+G 跳到文件中第n行
 Ctrl+Shift+G 光标在一个文件名上,直接跳到指定文件
 Ctrl+H 替换
 Ctrl+J,K #ifdef...#endif查找配对
 Ctrl+L 剪切一行
 Ctrl+} 匹配括号(),{}
 调试常用
 Shift+F9 QuickWatch,并显示关标所在处的变量
 Alt+3 Watch 查看窗口
 Alt+4 Variables 监视变量(常用)
 Alt+5 显示寄存器
 Alt+6 显示内存(常用)
 Alt+7 显示堆栈情况(当机了多看看)
 Alt+8 显示汇编码
 还有一些常用的
 Ctrl+TAB 切换打开的文件视图,(如果按住Ctrl,顺序向后切换)
 Ctrl+Shift+TAB切换打开的文件视图,(如果按住Ctrl,顺序向前切换)
 TAB 选中后,整体后移一个制表符,
 Shift+TAB 选中后,整体前移一个制表符,
 Alt+0 将焦点移到工作区(ESC回编辑区)
 Alt+2 将焦点移到输出区(ESC回编辑区)
 Ctrl+PgUp 逆序切换工作区视图
 Ctrl+PgDn 顺序切换工作区视图
 另外,VC没有设置打开,关闭工作区,和打开关闭输出区的快捷键,如果觉得不方便,
 可以自己注册(我设置的是 Ctrl+,和Ctrl+/).

注册在Tools => Customize => KeyBoard



原文地址：[http://www.cppblog.com/kenbardy/articles/4188.html](http://www.cppblog.com/kenbardy/articles/4188.html)




