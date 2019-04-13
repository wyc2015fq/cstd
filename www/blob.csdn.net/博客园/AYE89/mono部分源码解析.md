
# mono部分源码解析 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[mono部分源码解析](https://www.cnblogs.com/eniac1946/p/7217683.html)
|Posted on|2017-07-21 14:58|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7217683)|[收藏](#)
|一、源码结构
|这里仅列举几个重要的目录：
|mcs：
|mcs： Mono实现的基于Ecma标准的C\#编译器。
|class： CLI的C\#级的实现。类似于Android中的Java层，应用程序看到的是这一层提供的接口。这一层是平台无关的。
|ilasm： 反汇编器，将Native code反汇编成bytecode。
|mono：
|mini： JIT编译器，将bytecode编译成native code。
|metadata： Mono的runtime，CLI的Native级的实现。
|io-layer： 与操作系统相关的接口实现，像socket，thread，mutex这些。
|libgc： GC实现的一部分。
|mono的结构：
|C\#编译器mcs                产生：DLL形式的bytecode（具体位置为Managed文件夹下UnityEngine.dll）
|JIT编译器|
|runtime|
|操作系统接口|
|产生：DLL形式的native code（Mono文件夹下的mono.dll）
|二、mono主要工作框架
|mini/|main.c: main()
    mono_main_with_options()
        mono_main() 
            mini_init() 
            mono_assembly_open()
            main_thread_handler()|//|assembly（也就是bytecode）的编译执行|mini_cleanup()
            
main_thread_handler()
    mono_jit_exec() 
        mono_assembly_get_image()|//|得到image信息，如"test.exe"|mono_image_get_entry_point()|//|得到类，方法信息|mono_runtime_run_main(method, argc, argv, NULL)
            mono_thread_set_main(mono_thread_current())|//|将当前线程设为主线程|mono_assembly_set_main()
            mono_runtime_exec_main()|//|编译及调用目标方法|mono_runtime_exec_main()
    mono_runtime_invoke(method, NULL, pa, exc)|//|要调用的方法，如"ClassName::Main()"|default_mono_runtime_invoke()|//|实际上是调用了mono_jit_runtime_invoke()|info->compiled_method = mono_jit_compile_method_with_opt(method)|//|编译目标函数|info->runtime_invoke = mono_jit_compile_method()|//|编译目标函数的runtime wrapper|mono_jit_compile_method_with_opt(method, default_opt, &|ex)
            runtime_invoke|= info->|runtime_invoke
            runtime_invoke(obj,|params|, exc, info->compiled_method)|//|调用wrapper，wrapper会调用目标方法|mono_jit_compile_method_with_opt() 
    mono_jit_compile_method_inner() 
        mini_method_compile(method, opt, target_domain, TRUE, FALSE,|0|)|//|通过JIT编译给定方法|mono_runtime_class_init_full()|//|初始化方法所在对象|method = mono_class_get_cctor()|//|得到类的构造函数|if|(do_initialization)|//|对象需要初始化|mono_runtime_invoke()|//|调用相应构造函数来构造对象，如"System.console:.cctor()"|mono_jit_runtime_invoke()
|运行过程中会调用到mono_jit_runtime_invoke()







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
