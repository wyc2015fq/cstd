# vld for memory leak detector (release version) - DoubleLi - 博客园






有没有这样的情况，无法静态的通过启动和退出来查找内存泄露，比如网络游戏，你总不能直接关游戏那玩家怎么办? 现在vld支持release，我们可以动态的找。

1.在release版本使用vld了。<PS1>

#define VLD_FORCE_ENABLE

#include <vld.h>

2.支持gm命令来开启和关闭vld，并且report

__declspec(dllimport) void VLDDisable ();

__declspec(dllimport) void VLDEnable ();

__declspec(dllimport) UINT VLDReportLeaks ();

3. 修改一下vld.ini
VLD = off

ReportFile = .\memory_leak_report.txt

ReportTo = file

4. 运行release版，等启动完毕稳定了

运行gm命令VLDEnable

如果在本机，就上线打一会下线

如果在服务器，就等个1分钟

运行gm命令VLDReportLeaks

运行gm命令VLDDisable关闭

然后打开memory_leak_report.txt看

5. 如果太多了

修改vld.ini

AggregateDuplicates = yes

MaxDataDump = 32

MaxTraceFrames = 5

从block id最小的地方看起，因为那是入口，最有可能是泄露的地方

6. vld网上的版本没有对block id排序，所以找最小的id有难度，可以改下源代码，参考我的方法。[http://vld.codeplex.com/discussions/271640](http://vld.codeplex.com/discussions/271640)



VLD下载地址

[http://vld.codeplex.com/releases](http://vld.codeplex.com/releases)









