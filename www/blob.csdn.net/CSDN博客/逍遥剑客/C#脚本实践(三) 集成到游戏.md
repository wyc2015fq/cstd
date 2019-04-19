# C#脚本实践(三): 集成到游戏 - 逍遥剑客 - CSDN博客
2011年11月16日 23:02:10[xoyojank](https://me.csdn.net/xoyojank)阅读数：6687标签：[脚本																[c#																[dll																[domain																[assembly																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=assembly&t=blog)](https://so.csdn.net/so/search/s.do?q=domain&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
个人分类：[山口山寨																[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)](https://blog.csdn.net/xoyojank/article/category/488683)
- 编译- 冒似不编译没有.lib可以用
- 从官方上下载稳定版本的代码, 不要去GitHub直接拉
- 默认的工程有几个路径和宏之类的不对, 小改一下就可以
- 如果缺文件, 可以到GitHub的历史版本里去找
- 编译好了就一个lib, 一个dll, pdb看需要
- 环境配置- C++这边肯定要链接mono.lib, 所以dll要拷贝到exe的目录
- 除了mono本身外, 还需要.net的runtime assembly, 最精简的情况只需要一个mscorlib.dll, 其它的用到再加进去就可以
- 脚本的编译可以用mono的SDK里的msc, 也可以直接用VS或者MonoDevelop编译, 只要版本(3.5/4.0)一致就没问题
- Interop- C++调用脚本(DLL)里的东西比较方便, mono的embed api很全
- C#调用C++的话, 需要把C++的类在C#这边做一下映射, 有两种方式:- InternalCall: 直接用mono api注册, Unity应该就是这种方法. 缺点是粘合层的代码现在没有比较好的自动生成方案, 只有一个类似luabind的monobind, 但是C#那边还是要声明一遍. 自己去解析C++头文件再生成两边的代码的话, 可以参考一下doxybind.
- P/Invoke: 最通用的方案, .Net圈子里用得很多. 缺点是C++这边必须编译成DLL, 导出很多函数. 优点是有自动生成的方案, 最常见的是SWIG. 用SWIG的话只需要定义一个文件指明需要把哪些头文件导出给C#用, 然后粘合层的cpp和C#的包装代码都可以自动生成, 再集成到VC的build event中, 编译好工程直接就生成好C#这边引用的DLL了.
- 然后用VisualStudio/MonDevelop建立好工程, 添加引用, 就可以写脚本了. SWIG生成的包装类名和方法名都没变化, 用起来很爽. VS/MD会自动解析引用的DLL, 写代码时的因为有强大的IDE, 智能提示, 语法纠错, 编译检查都是非常好用的.
- 热更新- 虽然.net也支持直接从代码转换成可以运行的东西, 但是这里还是参考Unity把脚本都编译成DLL
- 热更新的基本原理: 检测到脚本的DLL发生改变就重新载入
- mono是以domain为单位load/unload的, 所以要为脚本运行创建一个单独的domain
- 脚本的domain卸载前, dll是不可写的, 这是由于mono做了file mapping. 解决办法有两个: 一是从内存加载, 二是先卸载再编译
- 加载后需要还原卸载前的对象状态, 这可以通过序列化来实现
- C#的编译时间基本上可以忽略, 游戏运行时这边脚本一改, 卡个几帧就可以继续用新的脚本继续跑了
- 多语言支持- 用Mono可以支持很多语言, Unity支持了三种: C#, Boo(Python), UnityScript(JavaScript)
- Boo是Mono SDK里就带的语言扩展
- UnityScript是用Boo实现的, 这也解释了为什么Unity会支持Boo...具体的实现在GitHub上可以找到
- 虽然JS和Boo比C#更像脚本, 但它们都是强类型的, 除了写代码时敲的字符少点我觉得不如C#, 因为啥? C#有强大的IDE支持
- lua的话有个lua2il的东西
- Java的话Mono里有个IKVM估计就是支持它的
至此, C#做为脚本已经完全可行了: 可嵌入, 跨平台, 高效率, 热更新, 几乎可以忽略的编译时间, 强大的IDE支持, 丰富的第三方库, 部分动态语言特性的支持, 可以接受的体积与内存占用
还剩一个很重要的部分: 调试. Unity是用MonoDevelop Attach到进程进行调试的. 目前山寨进度70%中...
[C#脚本实践(一)](http://blog.csdn.net/xoyojank/article/details/6038664)
[C#脚本实践(二): Unity脚本机制分析](http://blog.csdn.net/xoyojank/article/details/6041310)
