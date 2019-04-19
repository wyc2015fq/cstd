# 做了Nebula3的应用程序向导 - 逍遥剑客 - CSDN博客
2009年06月08日 23:32:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2397
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
使用方法:
1. 配置环境变量:
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090608/N3Wizard_Path.JPG)
2. 把下载的zip解压到一个合适的位置
3. 把N3ConsoleAppWizard.ico, N3ConsoleAppWizard.vsdir, N3ConsoleAppWizard.vsz三个文件拷贝到Visual Studio 8/VC/vcprojects/下, 并更改N3ConsoleAppWizard.vsz中的路径为第2步中的解压路径
4. 打开VS2005, 新建工程就可以看到了
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090608/N3Wizard_NewProject.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090608/N3Wizard_WizardSettings.JPG)
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090608/N3Wizard_Code.JPG)
附: 
Nebula3工程编译选项(以April2009为准):
Debug
SubSystem
Not set
Release:
SubSystem
Not Set
6/22/2009: 发现几个问题
- stdafx.h第一次编译会说找不到, 再编译一次才通过
- 由于源代码的SDK的pdb文件没有引入, 所以没法调试SDK相关的代码
- 输出目录应该为bin/win32/(或者"XXXX/win32"), 不然export.zip和export_win32.zip无法加载, 会导致RenderApplication的默认资源创建失败. 
对于1和2, 我没找到什么好方法, 有人共享下没? 3到是好解决, 把$(N3SDK)设置成code的父目录, 这样直接把输出目录设置好就可以了.
