# C#脚本实践(五): 调试器 - 逍遥剑客 - CSDN博客
2011年12月04日 20:49:53[xoyojank](https://me.csdn.net/xoyojank)阅读数：4147
- 以前用过lua, 调试时很痛苦. 当然现在已经有比较成熟的调试器了, 比如decoda
- 在没有调试器之前都是怎么调试的呢?- printf打印到控制台之类的输出窗口. 如果做了热更新的话可以边改边看
- 输入命令, 让控制台显示结果. 对于C++来说控制台是个很不爽的东西, 因为每个指令都要写个解析函数. 而脚本则可以直接把脚本语句当指令敲进去, 比如[CsharpRepl](http://www.mono-project.com/CsharpRepl)
- 当然, 最理想的情况还是能够下断点, 单步跟踪
- Unity使用MonoDevelop作为调试器, 幸运的是, MonoDevelop和UnityDebugger插件都是开源的, 所以很容易山寨过来
- MonoDevelop有两种方式启动调试:- 直接执行, 就像VS里按F5一样. 
- 挂接到进程, 就是先启动程序, 然后把调试器挂上去. 这种对于脚本来说比较实用, 因为可以修改一下继续调试, 不用重新启动游戏
- Mono调试时是有个调试服务器的, 可以通过socket连接客户端与服务器. 如果从MD里启动, 则MD是调试服务器. 如果是Attach方式, 则游戏是那个server.
- 以Attach方式为例:
```cpp
int port = (int)(56000 + (GetCurrentProcessId() % 1000));
 char options[255];
 sprintf_s(options, 255, "--debugger-agent=transport=dt_socket,address=127.0.0.1:%d,server=y", port);
 char* args = options;
 mono_jit_parse_options (1, &args);
 mono_debug_init (MONO_DEBUG_FORMAT_MONO);
```
- MD那边可以直接使用Unity的debugger插件, 当然它只认"unity"开头的进程:
![](http://hi.csdn.net/attachment/201112/4/0_13230030893QPe.gif)
- 另外需要注意的是, 调试的DLL必须有mdb文件, 相当于pdb文件. MD默认是生成pdb文件的, 所以需要改成mdb的调试信息才可以.
- 之后就一切顺利了, 断点, 单步跟踪, 监视器什么的都很完美:
![](http://hi.csdn.net/attachment/201112/4/0_13230031136bb4.gif)
[C#脚本实践(四): 反射与序列化](http://blog.csdn.net/xoyojank/article/details/7015001)
