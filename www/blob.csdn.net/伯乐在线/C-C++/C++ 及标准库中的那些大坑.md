# C++ 及标准库中的那些大坑 - 文章 - 伯乐在线
原文出处： [二叉白](https://segmentfault.com/a/1190000004933446)
## 1. 变量初始化
这是使用 C++11 codecvt 时遇到的一个坑，转换编码时，mbstate_t 这个中间状态变量，必须初始化为0，否则运行出错，即：


```
// 不可以！
mbstate_t mbst;
// 这样可以
mbstate mbst = {0};
// 这样也行
mbstate mbst = mbstate_t();
```
这是第一个坑，并不算太坑，还比较容易调试和发现，也怪自己大意了。
经验：C++中的变量一定要初始化后再使用。
## 2. 匿名 std::thread 对象
这个坑要和 boost 进行比较，在 boost 中，是可以创建匿名 thread 对象的，并且这样的匿名对象跟 future、promise是可以正常配合使用的（《Boost标准库完全开发指南》一书中的示例代码就是这样写的）。
但是，在 C++ 标准库中不能这么干，会出现莫名其妙的错误，调试时也不会显示任何有价值的信息，最终确定这个问题真是费了我好大劲，因为根本没想到会是这个问题，毕竟 boost 里都正常使用了。
经验：尽量不使用匿名对象，如果想要用完立即释放，可以使用单独的代码块包裹。
## 3. 线程局部存储（TLS）
这是一个坑了我一天的大坑。
C++11 中，新引入了 thread_local 存储类型，等同于之前的 __declspec(thread)，由于其具有真正的可移植性，所以我就尝试使用了，但这也是噩梦的开始。
我有一段代码，如果编译为 exe，在 xp 系统上能正常运行，但如果编译为 dll，在 xp 上运行就出错。由于 xp 上不能安装 VS 这种高科技玩意，只能用 x32_dbg 凑合调试，发现是空指针异常，指针来源为 fs:[2c]，这是 TLS 指针啊，然后百度，找到了微软的文档 https://msdn.microsoft.com/en-us/library/y5f6w579 ：
> 
On XP systems, thread_local may not function correctly if a DLL uses thread_local data and it is loaded dynamically via LoadLibrary.
是的，如果 dll 中使用了 thread_local，这个 dll 将不能在 xp 上通过 LoadLibrary 动态加载。
解决办法也是有的：
既然不能通过 LoadLibrary 动态加载，那我静态加载不就行了，只要在编译 exe 时静态链接 dll，即 dll 在 exe 的导入表中，那就可以正常运行（这也要求 exe 必须是自己可编译的）
在 DllMain 中使用 TLS 相关的 API 手动初始化
祈祷 xp 早日完蛋
经验：或许我应该抛弃 xp 了。
## 4. dll 中的静态对象
这个坑跟上个坑是同时出现的，只是我当时用了静态链接的方式后，就运行正常了，也就没在意。直到后来又想在 C# 中调用 dll，这回没办法静态链接了。为了先实现功能，我选择了暂时删除 thread_local，但是在 xp 上依然运行出错，错误原因跟之前一样！卧槽，我特么明明都删掉了 thread_local 呀，为何还这样！！
又经过2个小时的调试，最终确定问题出在 C++17 标准库中的 std::experimental::filesystem::exists() 函数，但是经过我单步调试发现，这个函数并没有使用 TLS，只用到了一些全局静态对象，莫非是全局静态对象的问题？
于是还是找文档吧，跟上个问题同一个网址 https://msdn.microsoft.com/en-us/library/y5f6w579 ：
> 
Starting in C++11, a static local variable initialization is guaranteed to be thread-safe. This feature is sometimes called magic statics. However, in a multithreaded application all subsequent assignments must be synchronized. The thread-safe statics feature can be disabled by using the /Zc:threadSafeInit- flag to avoid taking a dependency on the CRT.
在 C++11 中，静态变量的初始化是线程安全的，这个所谓的“线程安全”，就是引入了 TLS 来进行一些额外的检查，好在这个特性是可以禁用的，编译时添加 /Zc:threadSafeInit- 选项即可（注意最后的减号），禁用后就不会使用 TLS 了，也就可以在 xp 上动态加载了。
经验：xp 去死吧！去死吧！去死吧！
注：这些问题在 VS2015 Update 2 中发现，应该也会持续存在于之后的 VS 版本中。
