# Linux Eclipse代码提示功能设置(Java & C/C++) - DoubleLi - 博客园






最近在Linux下开发，由于长期使用Visual Studio 2010，对代码提示功能情有独钟，现在在Linux下，使用Eclipse做开发，当然免不了怀念Visual Studio强悍的代码提示，于是在网上搜索了一些文章，整理出关于Eclipse代码提示功能设置的方法。



Java:

增强Eclipse 的代码提示功能，具体怎么样来配置？下面开始说步骤：

1. 打开Eclipse，然后“window”→“Preferences”

2. 选择“java”，展开，“Editor”，选择“Content Assist”。

3. 选择“Content Assist”，然后看到右边，右边的“Auto-Activation”下面的“Auto Activation triggers for java”这个选项。其实就是指触发代码提示的就是“.”这个符号。

4. “Auto Activation triggers for java”这个选项，在“.”后加abc字母，方便后面的查找修改。然后“apply”，点击“OK”。

5. 然后，“File”→“Export”，在弹出的窗口中选择“Perferences”，点击“下一步”。

6. 选择导出文件路径，本人导出到桌面，输入“test”作为文件名，点击“保存”。

7. 在桌面找到刚在保存的文件“test.epf”,右键选择“用记事本打开”。

8. 可以看到很多配置Eclipse的信息

9. 按“ctrl + F”快捷键，输入“.abc”，点击“查找下一个”。

10. 查找到“.abc”的配置信息如下：

11. 把“.abc”改成“.abcdefghijklmnopqrstuvwxyz(,”，保存，关闭“test.epf”。

12. 回到Eclipse界面，“File”→“Import”，在弹出的窗口中选择“Perferences”，点击“下一步”，选择刚在已经修改的“test.epf”文件，点击“打开”，点击“Finish”。该步骤和上面的导出步骤类似。

13. 最后当然是进行代码测试了。随便新建一个工程，新建一个类。在代码输入switch，foreach等进行 测试。你立即会发现，果然出了提示，而且无论是敲哪个字母都会有很多相关的提示了，很流畅，很方便。



C/C++:



打开终端：输入：$ gcc- v

得到类似的：gcc 版本 4.1.3 20070929 (prerelease) (Ubuntu 4.1.2-16ubuntu2)

很容易就看到你当前使用的版本了。

启动Eclipse.进入：Windows-->Preferences-->C/C++找到Environment。增加两个变量：

CPLUS_INCLUDE_PATH: /usr/include/c++/4.1.3(我的gcc版本) 

C_INCLUDE_PATH: /usr/include



C/C++的代码提示只在.和->下触发，虽然没有Java下这么强大，不过也够了。因为这里是代码提示最容易发挥威力的地方。



补充：Ctrl+Space也能触发代码提示，而且功能更强，几乎总能给出符合需求的提示，不过我不知道如何才能让Eclipse在更多情况下自动触发提示，如果能做到，Eclipse就和Vistual Studio一样强了。



本文参考：

http://developer.51cto.com/art/200907/136242.htm

http://blog.chinaunix.net/u/21684/showart_462486.html





补充：alt+/  也能提示  不知道 Ctrl+Space  在mac 下怎么操作，因这这个组合键默认是被搜索功能占用了，commanc+space 被切换输入法占用了，不过alt+/ 能用也不错了 











