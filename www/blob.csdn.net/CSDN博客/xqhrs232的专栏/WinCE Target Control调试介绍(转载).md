# WinCE Target Control调试介绍(转载) - xqhrs232的专栏 - CSDN博客
2010年01月15日 16:30:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：996标签：[wince																[profiler																[buffer																[profiling																[properties																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=profiling&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=profiler&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
Target Control是一种基于命令行的调试工具，在WinCE启动以后，可以在Platform Builder中启动Target Control，通过命令行来调试WinCE。Target Control支持很多强大的命令，通过这些命令可以查看内存，启动进程等。
1. 配置Target Control
如果想在WinCE中支持Target Control功能，那么在编译WinCE系统之前要进行一些设置，首先在WinCE的组件中选择“Target Control Support”，如图：
默认情况下，Target Control调试是基于KITL的，所以要在菜单中选择“Project”，选择“Properties”，弹出属性对话框，然后选择“Build Options”，选择“Enable KITL”编译选项。
2. 启动Target Control
在重新编译WinCE系统之后，将WinCE image下载到目标板上并运行，等WinCE启动后，在VS2005的菜单中选择“Target”，然后选择“Target Control”会弹出一个命令行窗口，如图：
在命令行窗口中可以看到“Windows CE>”提示符，可以先键入问号(?)然后回车来查询目前所支持的命令。
3. Target Control命令
这里将对Target Control所支持的命令进行介绍：
？：列出所支持的命令
格式：?
dd：打印某一段内存的数据
格式：dd Address [Size]
                    Address：          内存地址
                    Size：                内存大小
df：将某一段内存的数据保存到文件
格式：df Filename Address Size
                    Filename：         文件名
                    Address：          内存地址
                    Size：                内存大小
Dis：强迫操作系统交换出内存中所有丢弃的内存页
格式：Dis
gi：获得进程及线程有关模块，临界区和事件的信息
格式：gi proc | thrd | mod | delta | all
                    proc：             列出系统中所有的进程
                    thrd：              列出系统中所有的线程
                    mod：             列出所有被加载的模块
                    delta：            列出随处理器时间变化的线程
                    all：                列出所有
Kp：结束一个进程
格式：Kp Pid | Pindex
                    Pid：                进程的PID
                    Pindex：           进程的索引号，这个索引号是通过gi proc命令得到的
log：显示和设置Celog事件跟踪子系统的区域
格式：log [logoption] [HexWindowsCEZone [ HexUserZone [ HexProcessMask ]]]
                    logoption：                            手动设置Celog的信息
                        -bufsize：                          用于Celog的Buffer大小
                        -clear：                             清除Celog的Buffer
                        -flush Filename.log：         将Celog的Buffer中的数据存到文件中
                    HexWindowsCEZone：        WinCE系统的Log区域掩码
                    HexUserZone：                   用户定义的Log区域掩马       
                    HexProcessMask：              需要被记录日志的进程
Mi：显示内存信息
格式：Mi kernel | full
                    Kernel：                显示内核内存信息
                    full：                     显示系统中所有进程的内存信息
prof：控制kernel profiler，只有系统支持profiling功能时，该命令才有用
格式：prof on | off [ Datatype ] [ Storagetype ] [Otheroptions]
                    on：                 启动kernel profiler
                    off：                停止kernel profiler
                    Datatype：       描述要收集的数据类型
                        -m：       收集蒙特卡洛profiling的相关数据
                        -s：       收集系统调用的相关数据
                        -k：               收集内核调用的相关数据
                    Storagetype：    如何存储收集的数据
                        -b：              带Buffer的存储模式
                        -u：              不带Buffer的存储模式
                        -l：               发送数据到Celog事件跟踪子系统
                    Otheroptions： 设置如何收集数据
                        -buf size：    设置Celog的Buffer的大小
                        -clear：        清除Celog的Buffer
                        -flush Filename.clg：停止kernel profiler，把数据存到文件中
                        -i：               控制profiler的采样时间
s：启动一个进程
格式：s ProcessName
                    ProcessName：    进程的名字
run：运行一个批处理文件
格式：run BatchFileName
                    BatchFileName：批处理文件名
win：列出目前在目标设备上的显示的窗口列表
格式：win
priority：改变shell线程的优先级
格式：priority [N]
                    N：                    线程的优先级
suspend：挂起设备
格式：suspend
tp：设置或查询线程的优先级
格式：tp <tid> [prio]
                    tid：                  线程的ID
                    prio：                线程的优先级
Break：打断kernel debugger程序
格式：Break
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/nanjianhui/archive/2008/12/11/3499807.aspx](http://blog.csdn.net/nanjianhui/archive/2008/12/11/3499807.aspx)

