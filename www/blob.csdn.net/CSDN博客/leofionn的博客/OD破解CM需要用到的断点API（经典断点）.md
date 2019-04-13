
# OD破解CM需要用到的断点API（经典断点） - leofionn的博客 - CSDN博客


2018年10月27日 21:34:51[leofionn](https://me.csdn.net/qq_36142114)阅读数：66


拦截窗口：
bp CreateWindow 创建窗口
bp CreateWindowEx(A) 创建窗口
bp ShowWindow 显示窗口
bp UpdateWindow 更新窗口
bp GetWindowText(A) 获取窗口文本
拦截消息框：
bp MessageBox(A) 创建消息框
bp MessageBoxExA 创建消息框
bp MessageBoxIndirect(A) 创建定制消息框
拦截警告声：
bp MessageBeep 发出系统警告声(如果没有声卡就直接驱动系统喇叭发声)
拦截对话框：
bp DialogBox 创建模态对话框
bp DialogBoxParam(A) 创建模态对话框
bp DialogBoxIndirect 创建模态对话框
bp DialogBoxIndirectParam(A) 创建模态对话框
bp CreateDialog 创建非模态对话框
bp CreateDialogParam(A) 创建非模态对话框
bp CreateDialogIndirect 创建非模态对话框
bp CreateDialogIndirectParam(A) 创建非模态对话框
bp GetDlgItemText(A) 获取对话框文本
bp GetDlgItemInt 获取对话框整数值
拦截剪贴板：
bp GetClipboardData 获取剪贴板数据
拦截注册表：
bp RegOpenKey(A) 打开子健
bp RegOpenKeyEx 打开子健
bp RegQueryValue(A) 查找子健
bp RegQueryValueEx 查找子健
bp RegSetValue(A) 设置子健
bp RegSetValueEx(A) 设置子健
功能限制拦截断点：
bp EnableMenuItem 禁止或允许菜单项
bp EnableWindow 禁止或允许窗口
拦截时间：
bp GetLocalTime 获取本地时间
bp GetSystemTime 获取系统时间
bp GetFileTime 获取文件时间
bp GetTickCount 获得自系统成功启动以来所经历的毫秒数
bp GetCurrentTime 获取当前时间（16位）
bp SetTimer 创建定时器
bp TimerProc 定时器超时回调函数
拦截文件：
bp CreateFileA 创建或打开文件 (32位)
bp OpenFile 打开文件        (32位)
bp ReadFile 读文件          (32位)
bp WriteFile 写文件          (32位)
bp GetPrivateProfileStringA (ini文件)
拦截驱动器：
bp GetDriveTypeA 获取磁盘驱动器类型
bp GetLogicalDrives 获取逻辑驱动器符号
bp GetLogicalDriveStringsA 获取当前所有逻辑驱动器的根驱动器路径
★★VB程序专用断点★★
bp __vbaStrCmp 比较字符串是否相等
bp __vbaStrComp 比较字符串是否相等
bp __vbaVarTstNe 比较变量是否不相等
bp __vbaVarTstEq 比较变量是否相等
bp __vbaStrCopy 复制字符串
bp __vbaStrMove 移动字符串
bp MultiByteToWideChar ANSI字符串转换成Unicode字符串
bp WideCharToMultiByte Unicode字符串转换成ANSI字符串
解自校验
bpx CreateFileA
bpx GetFileSize
bpx SetFilePointer
bpx ExitProcess
F12堆栈调用
破解思路
C类
Point-H法
bp GetDlgItem(断输入框)
bp MessageBoxA(断对话框)
字符串法
F12堆栈调用
B、D类
DEDE、PE Explorer作为强有力的辅助工具
关键还是找按妞事件
Point-H法
bp GetDlgItem(断输入框)
bp MessageBoxA(断对话框)
字符串法
F12堆栈调用
V类
VBExplorer、GetVBRes、SmatCheck作为强有力的辅助工具
关键还是找按妞事件
bp rtcMsgBox(断对话框)
---
如果是重启验证就使用最开始的那些断点
C类语言破解
1、bp MessageBoxA(W)(断对话框)—Ctrl+N
2、Point-H法
3、bp GetDlgItem/GetWindowTextA(W)/GetWindowTextLengthA(W) (断输入框)
4、字符串法—插件/搜索所有参考文本
Delphi/BC++语言破解
1、DEDE结合PE Explorer找按妞事件
2、Point-H法
3、bp GetDlgItem/GetWindowTextA(W)/GetWindowTextLengthA(W) (断输入框)
4、bp MessageBoxA(W)(断对话框)—Ctrl+N
5、字符串法—插件/搜索所有参考文本
6、如果程序界面标题有[未注册/注册/VIP版/标准版/钻石版] 之类字样，可以通过DEDE查找FormCreate/FormShow，找到关键标志位
VB语言破解
1、VBExplorer查找按钮事件
2、有提示框则   bp rtcMsgBox(断对话框)
3、通过bp __vbaStrCmp/__vbaStrComp/__vbaVarTstEq
4、万能断点法(816C24法)
5、字符串法—插件/搜索所有参考文本
易语言破解
1、借助E-Code Explorer查找按钮事件
2、下消息断点，查看堆栈再返回
3、eCode法–断按钮事件
4、字符串法—插件/搜索所有参考文本
按钮事件固定模式：
0040EC78      837D F4 00       cmp dword ptr ss:[ebp-C],0          ；关键判断
0040EC7C      0F84 3B000000    je dcse.0040ECBD                     ；关键跳
0040EC82      68 04000080      push 80000004
0040EC87      6A 00            push 0
0040EC89      68 EC904000      push dcse.004090EC
0040EC8E      68 01030080      push 80000301
0040EC93      6A 00            push 0
00 40EC95      68 00000000      push 0
0040EC9A      68 04000080      push 80000004
0040EC9F      6A 00            push 0
0040ECA1      68 F1904000      push dcse.004090F1
0040ECA6      68 03000000      push 3
0040ECAB      BB 00030000      mov ebx,300
0040ECB0      E8 92000000      call dcse.0040ED47
0040ECB5      83C4 28          add esp,28
0040ECB8      E9 36000000      jmp dcse.0040ECF3
0040ECBD      68 04000080      push 80000004
0040ECC2      6A 00            push 0
0040ECC4      68 CC904000      push dcse.004090CC
0040ECC9      68 01030080      push 80000301
0040ECCE      6A 00            push 0
0040ECD0      68 00000000      push 0
0040ECD5      68 04000080      push 80000004
0040ECDA      6A 00            push 0
0040ECDC      68 02914000      push dcse.00409102
0040ECE1      68 03000000      push 3
0040ECE6      BB 00030000      mov ebx,300
0040ECEB      E8 57000000      call dcse.0040ED47
0040ECF0      83C4 28          add esp,28
0040ECF3      8BE5             mov esp,ebp
0040ECF5      5D               pop ebp
0040ECF6      C3               retn
---
按钮事件
1、有注册错误/正确提示
bp MessageBoxA
bp rtcMsgBox
如果事先找不到按钮事件，可以通过下消息断点，返回后回溯即可找到按钮事件起始位置
2、无任何提示
bp GetDlgItem
可以通过bp GetDlgItem获取按钮事件代码
3、未注册一启动或者关闭就跳出个注册框或者提示框
bp RegOpenKey(A)
bp CreateFileA
bp GetPrivateProfileStringA
文件: reg/ini/dll/其他
至于保存到什么文件，可以使用以下方法
1、查找字符串，看是否有可疑文件名或者注册表键名
2、猜。。。下断点观察
3、按钮事件跟踪
4、未注册一启动或者关闭就打开网页链接
bp ShellExecuteA
类似的组合
Cmp/test/其他判断
Je/jne/jne/jz XXXXXXXX
软件启动—>判断是否注册—>是否Open
断下后回溯代码即可找到关键点，常用的方法，转存跟踪法
5、未注册就功能使用限制
判断是否注册—>某种功能是否让你使用,如果不能够用，一定会有提示的，或是错误提示或是弹出注册框等，那么从提示入手即可找到解除限制的关键
不完美破解：解除功能限制
6、未注册就日期限制
bp GetLocalTime 获取本地时间
bp GetSystemTime 获取系统时间
bp GetFileTime 获取文件时间
一般下这几个断点比较难分析关键
捷径:查找字符串–找可疑文件–一般以DLL多见
只要不让它读取到这个DLL即可解除限制
7、Demo(演示试用版)–功能残缺
这个和上面的功能限制不一样
功能限制是软件本身就有这个功能，对程序而言，相对应的功能代码也存在
Demo即是没这个功能，空架子一个而已
一句话：破解也无用！
8、网络验证
无法登陆有错误提示者：下消息断点回溯代码，找按钮事件，从头来过，从按钮事件开始跟踪，找网络验证CALL(所需要登陆的地址在这个CALL里面)，接下来就是分析返回值或者改登陆地址为本地(127.0.0.1)，再后面就需要改某些跳转了
无法登陆自动退出者：下bp ExitProcess断下(一般可以断下)回溯代码，找按钮事件，从来来过，从按钮事件开始跟踪，找网络验证CALL(所需要登陆的地址在这个CALL里面)，接下来就是分析返回值或者改登陆地址为本地(127.0.0.1)，再后面就需要改某些跳转了
9、狗加密
一般狗加密软件，一启动就会检测所需要的狗文件，若没有狗文件，提示错误
这里我们有两个入手点
1、“一启动就会检测所需要的狗文件”，下bp CreateFileA等断点，断下后，回溯
2、“若没有狗文件，提示错误”，下bp MessageBoxA，断下后，回溯
总结：
从上面的介绍说明可以看出，有这么一个共同点—按钮事件，可以这么说，按钮事件是我们的思路之门
按钮事件可以这样得来：
1、通过下相应断点，回溯代码
2、通过辅助工具快捷的得到（VBExplorer、DEDE）
---
重启验证
80%-90%的软件基本都是重启验证类型
1、注册表类型
Bpx RegOpenKeyA(W)
Bpx RegOpenKeyExA(W)
2、ini文件类型(*.reg/*.ini)
Bpx GetPrivateProfileStringA
3、其他文件类型(*.dat/*.lic…)
Bpx CreateFileA(W)
Bpx ReadFile
4、DLL文件操作类型
如果没有什么有效的拦截函数，不妨试一下Bpx CreateFileA(W).余下的就是通过你的经验去判断了(例如：35课)
注意：建议使用Bpx断点，这样，比较快捷、准确。尚若Bpx失效，再尝试bp
方便断点设置的有以下3个断点插件，APIBreak中国版(不带Point-H)，APIBreak英文版(带Point-H)、+BP-Olly
---
去nag框的方法
1、若是Delphi&BCB程序，可以通过FormCreate法查找到FormCreate，再单步跟踪，找到窗口的调用CALL，一般它的具体形式是call dword ptr ds:[edx+E8]
2、OD载入程序后，单步跟踪，找到窗口的调用CALL
吾爱的好帖子，希望能保存一波：[https://www.52pojie.cn/forum.php?mod=viewthread&tid=806855&extra=page%3D1%26filter%3Dauthor%26orderby%3Ddateline](https://www.52pojie.cn/forum.php?mod=viewthread&tid=806855&extra=page%3D1%26filter%3Dauthor%26orderby%3Ddateline)

