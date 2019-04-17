# DllMain 用法 - DoubleLi - 博客园






1.1  DllMain简介 
跟exe有个main或者WinMain入口函数一样，DLL也有一个入口函数，就是DllMain。以“DllMain”为关键字，来看看MSDN帮助文档怎么介绍这个函数的。 
The DllMain function is an optional method of entry into a dynamic-link library (DLL)。（简要翻译：对于一个Dll模块，DllMain函数是可选的。）这句话很重要，很多初学者可能都认为一个动态链接库肯定要有DllMain函数。其实不然，像很多仅仅包含资源信息的DLL是没有DllMain函数的。 
1.2 何时调用DllMain 
       系统是在什么时候调用DllMain函数的呢？静态链接时，或动态链接时调用LoadLibrary和FreeLibrary都会调用DllMain函数。DllMain的第三个参数fdwReason指明了系统调用Dll的原因，它可能是DLL_PROCESS_ATTACH、DLL_PROCESS_DETACH、DLL_THREAD_ATTACH和DLL_THREAD_DETACH。以下从这四种情况来分析系统何时调用了DllMain。             
1.2.1 DLL_PROCESS_ATTACH 
       大家都知道，一个程序要调用Dll里的函数，首先要先把DLL文件映射到进程的地址空间。要把一个DLL文件映射到进程的地址空间，有两种方法：静态链接和动态链接的LoadLibrary或者LoadLibraryEx。 
       当一个DLL文件被映射到进程的地址空间时，系统调用该DLL的DllMain函数，传递的fdwReason参数为DLL_PROCESS_ATTACH。这种调用只会发生在第一次映射时。如果同一个进程后来为已经映射进来的DLL再次调用LoadLibrary或者LoadLibraryEx，操作系统只会增加DLL的使用次数，它不会再用DLL_PROCESS_ATTACH调用DLL的DllMain函数。不同进程用LoadLibrary同一个DLL时，每个进程的第一次映射都会用DLL_PROCESS_ATTACH调用DLL的DllMain函数。 
       可参考DllMainTest的DLL_PROCESS_ATTACH_Test函数。 
1.2.2 DLL_PROCESS_DETACH 
       当DLL被从进程的地址空间解除映射时，系统调用了它的DllMain，传递的fdwReason 
值是DLL_PROCESS_DETACH。当DLL处理该值时，它应该执行进程相关的清理工作。 
       那么什么时候DLL被从进程的地址空间解除映射呢？两种情况： 
       ◆FreeLibrary解除DLL映射（有几个LoadLibrary，就要有几个FreeLibrary） 
       ◆进程结束而解除DLL映射，当然是在进程结束前还没有这个解除DLL的映射的情况。（如果进程的终结是因为调用了TerminateProcess，系统就不会用DLL_PROCESS_DETACH来调用DLL的DllMain函数。这就意味着DLL在进程结束前没有机会执行任何清理工作。） 
       注意：当用DLL_PROCESS_ATTACH调用DLL的DllMain函数时，如果返回FALSE，说明没有初始化成功，系统仍会用DLL_PROCESS_DETACH调用DLL的DllMain函数。因此，必须确保没有清理那些没有成功初始化的东西。 
       可参考DllMainTest的DLL_PROCESS_DETACH_Test函数。 
1.2.3 DLL_THREAD_ATTACH 
       当进程创建一线程时，系统查看当前映射到进程地址空间中的所有DLL文件映像，并用值DLL_THREAD_ATTACH调用DLL的DllMain函数。 
新创建的线程负责执行这次的DLL的DllMain函数，只有当所有的DLL都处理完这一通知后，系统才允许线程开始执行它的线程函数。 
注意跟DLL_PROCESS_ATTACH的区别，我们在前面说过，第n(n>=2)次以后地把DLL映像文件映射到进程的地址空间时，是不再用DLL_PROCESS_ATTACH调用DllMain的。而DLL_THREAD_ATTACH不同，进程中的每次建立线程，都会用值DLL_THREAD_ATTACH调用DllMain函数，哪怕是线程中建立线程也一样。 
1.2.4 DLL_THREAD_DETACH 
       如果线程调用了ExitThread来结束线程（线程函数返回时，系统也会自动调用ExitThread），系统查看当前映射到进程空间中的所有DLL文件映像，并用DLL_THREAD_DETACH来调用DllMain函数，通知所有的DLL去执行线程级的清理工作。 
       注意：如果线程的结束是因为系统中的一个线程调用了TerminateThread，系统就不会用值DLL_THREAD_DETACH来调用所有DLL的DllMain函数。

1.3 函数原型及参数说明

    DllMain函数的原型

    BOOL   WINAPI   DllMain( HINSTANCE   hinstDLL,  DWORD   fdwReason,  LPVOID   lpvReserved  );

    BOOL   APIENTRY  DLLMain(HANDLE hModule,DWORD ul_reason_for_call,LPVOID  lpReserved)

    {

     switch(ul_reason_for_call)

     {

     case DLL_PROCESS_ATTACH:

     .......

     case DLL_THREAD_ATTACH:

     .......

     case DLL_THREAD_DETACH:

     .......

     case DLL_PROCESS_DETACH:

     .......

      }

    return TRUE;

    }

    参数：

    hModule：

    是动态库被调用时所传递来的一个指向自己的句柄(实际上，它是指向_DGROUP段的一个选择符)；

    ul_reason_for_call：

    是一个说明动态库被调原因的标志。当进程或线程装入或卸载动态连接库的时候，操作系统调用入口函数，并说明动态连接

    DLL_PROCESS_ATTACH 进程被调用
    DLL_THREAD_ATTACH 线程被调用
    DLL_PROCESS_DETACH 进程被停止
    DLL_THREAD_DETACH 线程被停止

    lpReserved：是一个被系统所保留的参数；

    DLL_PROCESS_ATTACH

    每个进程第一次调用DLL文件被映射到进程的地址空间时，传递的fdwReason参数为DLL_PROCESS_ATTACH。这进程再次调用操作系统只会增加DLL的使用次数。

    DLL_THREAD_ATTACH

    进程中的每次建立线程，都会用值DLL_THREAD_ATTACH调用DllMain函数，哪怕是线程中建立线程也一样。

    DLL_PROCESS_DETACH

    当DLL被从进程的地址空间解除映射时，系统调用了它的DllMain，传递的fdwReason值是DLL_PROCESS_DETACH。

           ◆FreeLibrary解除DLL映射（有几个LoadLibrary，就要有几个FreeLibrary）
           ◆进程结束而解除DLL映射，在进程结束前还没有解除DLL的映射，进程结束后会解除DLL映射。

    用TerminateProcess终结进程，系统就不会用DLL_PROCESS_DETACH来调用DLL的DllMain函数。

    注意：当用DLL_PROCESS_ATTACH调用DLL的DllMain函数时，如果返回FALSE，说明没有初始化成功，系统仍会用DLL_PROCESS_DETACH调用DLL的DllMain函数。

    DLL_THREAD_DETACH

    线程调用ExitThread来结束线程（线程函数返回时，系统也会自动调用ExitThread），用DLL_THREAD_DETACH来调用DllMain函数。

    注意：用TerminateThread来结束线程，系统就不会用值DLL_THREAD_DETACH来调DLL的DllMain函数。









