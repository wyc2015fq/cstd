# Windbg 32位版本和64位版本的选择 - Ms.Fox的专栏 - CSDN博客
2018年01月02日 15:47:05[lulu-lu](https://me.csdn.net/smbluesky)阅读数：83
个人分类：[qt](https://blog.csdn.net/smbluesky/article/category/7377296)
习惯了Vsiual Studio的兄弟们可能会因为先入为主的原因以为所有的调试器都应该像它那样，其实不然，当你安装Debugging Tools for Windows的时候，你将发现有两个系列的工具，一系列32位的工具和一系列64位的工具。这让人觉得和费解，因为在我们安装Microsoft Visual Studio的时候你根本不需要考虑32位还是64位。
![](http://images.cnblogs.com/cnblogs_com/pugang/Windbg.jpg)
如果你正使用windbg调试工具集（WinDbg, KD, CDB, or NTSD）中的一个，你必须得自己做出选择。为了选择合适的调试工具，你需要知道以下两点：
1. 你的调试主机的处理器类型。
2. 你的调试主机运行的是32位版本的windows还是64位版本的。
PS：运行调试器的计算机我们成为调试主机，被调试的计算机我们称为目标机。
调试主机运行32位版本的windows
如果你的调试主机运行的是32位版本的windows，使用32位的调试工具（不管此时被调试的目标机是x86-based 还是 x64-based）。
#### x64-based调试主机运行64位的windows
#### 如果你的调试主机使用x64-based的处理器，并且运行64位的windows，请参考如下规则：
- 
如果你在分析dump文件，你可以使用32位或者64位的调试工具集。（不管dump文件是用户态的还是内核态的，也不管这个dump文件是在x86-based 还是 x64-based的平台上抓的。）
- 
如果你在进行实时内核调试，你可以使用32位或者64位的调试工具集（不管此时被调试的目标机是x86-based 还是 x64-based）
- 
如果你在进行实时用户态调试，并且调试器也在同一台机器上，对于64位的代码和32位的WOW64代码都需要使用64位的调试工具集。使用[.effmach](http://msdn.microsoft.com/en-us/library/windows/hardware/ff562949(v=vs.85).aspx)命令设置调试器的模式。
- 
如果你在实时调试32位的用户态代码，但是这些代码运行在一个单独的目标机器上，使用32位的调试工具集。
- 
- 
总结
只有在实时用户态调试，并且调试器也在同一台64位机器上的情况下必须用64位的调试工具集！
https://github.com/EasyDarwin/Tools/blob/master/Windbg_x86_x64/dbg_amd64.msi
