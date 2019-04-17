# 从Zygote说到View（一）Zygote的启动流程及运行机制 - 我的学习笔记 - CSDN博客





2018年12月04日 00:06:47[zouzhiheng](https://me.csdn.net/u011330638)阅读数：51








# 前言

计划写一个系列文章，从 Zygote 开始，说到 Activity，再到 View 的显示及事件分发等，意在把 Android 开发中最核心的一些的知识点串成线，看看 Android 是怎么把它们组织到一起的，希望能写好。

本文是第一篇，以“Zygote 的启动流程及运行机制”为题， 将打通“虚拟机-Zygote-应用进程-ActivityThread”这一条线。

Zygote 的中文意思是受精卵、合子，可以理解为孵化器——Android 中大多数应用进程和系统进程都是通过 Zygote 来生成的。

PS：源码基于 Android API 27。

# Zygote 是怎么启动的？

## init

Android 的第一个进程为 init，init 通过解析 init.rc 来陆续启动其它关键的系统服务进程——其中最重要的是 ServiceManager、Zygote 和 SystemServer。下面以 init.zygote64.rc 为例开始分析：

```
## 服务名、路径、参数
service zygote /system/bin/app_process64 -Xzygote /system/bin --zygote --start-system-server
    class main
    socket zygote stream 660 root system
    onrestart write /sys/android_power/request_state wake
    onrestart write /sys/power/state on
    onrestart restart audioserver
    onrestart restart cameraserver
    onrestart restart media
    onrestart restart netd
    writepid /dev/cpuset/foreground/tasks
```

通过指定 --zygote 参数，app_process 可以识别出是否需要启动 zygote。

## 虚拟机的启动

下面开始分析路径 app_process 中的文件 app_main.cpp ：

```
// app_process/app_main.cpp
int main(int argc, char* const argv[])
{

    // 创建 Android 虚拟机对象
    AppRuntime runtime(argv[0], computeArgBlockSize(argc, argv));
    
    ...
    
    // Parse runtime arguments.  Stop at first unrecognized option.
    bool zygote = false;
    bool startSystemServer = false;
    bool application = false;
    String8 niceName;
    String8 className;

    ++i;  // Skip unused "parent dir" argument.
    while (i < argc) {
        const char* arg = argv[i++];
        if (strcmp(arg, "--zygote") == 0) { // // init.rc 指定了参数--zygote，因此这里为 true
            zygote = true;
            niceName = ZYGOTE_NICE_NAME;
        } else if (strcmp(arg, "--start-system-server") == 0) { // init.rc 指定了参数--start-system-server，因此这里也为 true
            startSystemServer = true;
        } else if (strcmp(arg, "--application") == 0) {
            application = true;
        } else if (strncmp(arg, "--nice-name=", 12) == 0) {
            ...
        } else {
            ...
        }
    }

    Vector<String8> args;
    if (!className.isEmpty()) {
        ...
    } else {
        if (startSystemServer) {
            args.add(String8("start-system-server")); // 添加 SystemServer 参数
        }
        ...
    }

    ...

    if (zygote) {
        // 启动虚拟机及 Zygote，注意包名
        runtime.start("com.android.internal.os.ZygoteInit", args, zygote); 
    } else if (className) {
        runtime.start("com.android.internal.os.RuntimeInit", args, zygote);
    } else {
        fprintf(stderr, "Error: no class name or --zygote supplied.\n");
        app_usage();
        LOG_ALWAYS_FATAL("app_process: no class name or --zygote supplied.");
    }
}
```

根据以上代码，可以知道 Zygote 运行于 Android 虚拟机上，因为 AppRuntime 继承于 AndroidRuntime：

```
class AppRuntime : public AndroidRuntime
{
public:
    AppRuntime(char* argBlockStart, const size_t argBlockLength)
        : AndroidRuntime(argBlockStart, argBlockLength)
        , mClass(NULL)
    {
    }
    
    ...
    
}
```

AppRuntime 更多是处理一些事件完成后的回调，主要的实现依然在 AndroidRuntime 中。因此下面直接看 AndroidRumtime::start 的实现：

```
/*
* Start the Android runtime.  This involves starting the virtual machine
* and calling the "static void main(String[] args)" method in the class
* named by "className".
*
* Passes the main function two arguments, the class name and the specified
* options string.
*
* 该方法用于启动 Android 虚拟机，并调用 className 对应的类的 main 方法
*/
void AndroidRuntime::start(const char* className, const Vector<String8>& options, bool zygote)
{
    ...

    /* 启动虚拟机 */
    JniInvocation jni_invocation;
    jni_invocation.Init(NULL); // 初始化虚拟机环境
    JNIEnv* env;
    if (startVm(&mJavaVM, &env, zygote) != 0) { // 启动
        return;
    }
    onVmCreated(env); // 回调，通知虚拟机已成功启动

    /*
     * 注册 Android 的 native 函数
     */
    if (startReg(env) < 0) {
        ALOGE("Unable to register all android natives\n");
        return;
    }

    /*
     * We want to call main() with a String array with arguments in it.
     * At present we have two arguments, the class name and an option string.
     * Create an array to hold them.
     *
     * 调用 className 对应的类的 main 方法，并传入相应的参数
     */
    jclass stringClass;
    jobjectArray strArray;
    jstring classNameStr;

    stringClass = env->FindClass("java/lang/String");
    strArray = env->NewObjectArray(options.size() + 1, stringClass, NULL);
    classNameStr = env->NewStringUTF(className);
    env->SetObjectArrayElement(strArray, 0, classNameStr);

    // 设置参数
    for (size_t i = 0; i < options.size(); ++i) {
        jstring optionsStr = env->NewStringUTF(options.itemAt(i).string());
        env->SetObjectArrayElement(strArray, i + 1, optionsStr);
    }

    /*
     * Start VM.  This thread becomes the main thread of the VM, and will
     * not return until the VM exits.
     *
     * 当前线程会成为虚拟机的主线程，除非虚拟机退出，否则 main 方法会一直阻塞
     */
    char* slashClassName = toSlashClassName(className != NULL ? className : "");
    jclass startClass = env->FindClass(slashClassName); // 查找 className 对应的类
    if (startClass == NULL) {
        ...
    } else {
        jmethodID startMeth = env->GetStaticMethodID(startClass, "main",
            "([Ljava/lang/String;)V"); // 查找 startClass 对应的 main 方法
        if (startMeth == NULL) { // 未找到 main 方法
            ...
        } else {
            env->CallStaticVoidMethod(startClass, startMeth, strArray); // 调用 main 方法
        }
    }
    
    // 结束虚拟机运行
    ALOGD("Shutting down VM\n");
    if (mJavaVM->DetachCurrentThread() != JNI_OK)
        ALOGW("Warning: unable to detach main thread\n");
    if (mJavaVM->DestroyJavaVM() != 0)
        ALOGW("Warning: VM did not shut down cleanly\n");
}
```

上面的代码还是很好懂的，主要作用是：
- 初始化虚拟机环境
- 启动虚拟机
- 注册 Android 的 native 接口，主要包括 Binder、OpenGL、Bitmap、Camera、AudioRecord 等
- 找到 ZygoteInit，传递参数，调用它的 main 方法，main 方法会一直阻塞直到虚拟机退出
- 虚拟机退出时销毁相关资源

虚拟机的相关流程忽略，假设 VM 成功启动，那么 com.android.internal.os.ZygoteInit（这个包名是 app_main.cpp 文件传过来的） 的 main 方法将会被调用，此时可以认为 Zygote 也随之启动了，即 Zygote 是由 Android 的第一个进程 init 解析文件 init.rc 时启动的。其中首先启动的是 Android 虚拟机，接着才是 Zygote，ZygoteInit 的 main 函数会一直阻塞执行，直到虚拟机退出。

# Zygote 是如何孵化子进程的？

下面开始分析 ZygoteInit 的 main 方法：

```
/**
* Startup class for the zygote process.
*/
public class ZygoteInit {

    public static void main(String argv[]) {
        ZygoteServer zygoteServer = new ZygoteServer(); // 创建 Zygote 的 Server Socket 对象

        ...

        final Runnable caller;
        try {
            ...

            boolean startSystemServer = false;
            String socketName = "zygote";
            String abiList = null;
            boolean enableLazyPreload = false;
            for (int i = 1; i < argv.length; i++) {
                if ("start-system-server".equals(argv[i])) { // init.rc 指定了参数--start-system-server
                    startSystemServer = true;
                } else if ("--enable-lazy-preload".equals(argv[i])) {
                    ...
                } else {
                    ...
                }
            }
            
            // 注册 Socket
            zygoteServer.registerServerSocket(socketName);
            if (!enableLazyPreload) {
                // 预加载所有应用需要的公共资源
                preload(bootTimingsTraceLog);
            } else {
                Zygote.resetNicePriority();
            }

            ...

            if (startSystemServer) {
                // fork 一个 SystemServer 进程
                Runnable r = forkSystemServer(abiList, socketName, zygoteServer);

                // {@code r == null} in the parent (zygote) process, and {@code r != null} in the
                // child (system_server) process.
                if (r != null) {
                    r.run();
                    return;
                }
            }

            // The select loop returns early in the child process after a fork and
            // loops forever in the zygote.
            // Zygote 进程会在一个死循环上等待连接
            caller = zygoteServer.runSelectLoop(abiList);
        } catch (Throwable ex) {
            Log.e(TAG, "System zygote died with exception", ex);
            throw ex;
        } finally {
            zygoteServer.closeServerSocket(); // Zygote 退出了死循环，关闭 Socket
        }

        // We're in the child process and have exited the select loop. Proceed to execute the command.
        // 当前是子进程，且退出了循环，则继续执行命令
        if (caller != null) {
            caller.run();
        }
    }
    
}
```

从代码中可以看出，ZygoteInit 主要完成 4 项工作：
- 注册一个 Socket。Zygote 是一个孵化器，一旦有新程序需要运行时，系统会通过这个 Socket 在第一时间通知它孵化一个进程
- 预加载各类资源
- 启动 SystemServer。Zygote 只会初始化一次，因此它需要新建一个专门的进程来承载系统服务的运行
- 进入死循环，不断地处理 Socket 请求

由此可以知道，Zygote 主要是通过 Socket 来处理客户端的请求的。下面将对以上几项工作逐个进行分析。

## 注册 Socket

```
class ZygoteServer {

    private static final String ANDROID_SOCKET_PREFIX = "ANDROID_SOCKET_";

    private LocalServerSocket mServerSocket;

    void registerServerSocket(String socketName) {
        if (mServerSocket == null) {
            int fileDesc;
            final String fullSocketName = ANDROID_SOCKET_PREFIX + socketName; // socketName 为 "zygote"
            try {
                String env = System.getenv(fullSocketName); 
                fileDesc = Integer.parseInt(env);
            } catch (RuntimeException ex) {
                throw new RuntimeException(fullSocketName + " unset or invalid", ex);
            }

            try {
                FileDescriptor fd = new FileDescriptor();
                fd.setInt$(fileDesc);
                mServerSocket = new LocalServerSocket(fd); // 创建 Server Socket
            } catch (IOException ex) {
                throw new RuntimeException(
                        "Error binding to local socket '" + fileDesc + "'", ex);
            }
        }
    }
    
}
```

LocalServerSocket 的构造方法如下：

```
/**
* Non-standard class for creating an inbound UNIX-domain socket
* in the Linux abstract namespace.
*/
public class LocalServerSocket {

    private final LocalSocketImpl impl;
    private final LocalSocketAddress localAddress;

    public LocalServerSocket(String name) throws IOException {
        impl = new LocalSocketImpl(); // Socket 实现类

        impl.create(LocalSocket.SOCKET_STREAM); // TCP 连接

        localAddress = new LocalSocketAddress(name);
        impl.bind(localAddress); // 绑定

        impl.listen(LISTEN_BACKLOG); // 监听
    }
    
}
```

可以看到，registerServerSocket 的作用是创建一个名为 “ANDROID_SOCKET_zygote” 的 Socket 对象，并绑定对应的端口，开始监听来自其他进程的请求。根据注释可以看出，这个 Socket 是一个 UNIX-domain Socket，这种 Socket 适用于单个设备内的进程间通信，这是除 Binder 外，Android 执行进程间通信时最常用的一种方式。

## preload

下面简单看一下 preload 方法：

```
public class ZygoteInit {

    static void preload(TimingsTraceLog bootTimingsTraceLog) {
        Log.d(TAG, "begin preload");
        beginIcuCachePinning();
        
        preloadClasses();
        
        preloadResources();
        
        nativePreloadAppProcessHALs();
        
        preloadOpenGL();
        
        preloadSharedLibraries();
        
        preloadTextResources();
        
        // Ask the WebViewFactory to do any initialization that must run in the zygote process,
        // for memory sharing purposes.
        WebViewFactory.prepareWebViewInZygote();
        
        endIcuCachePinning();
        
        warmUpJcaProviders();
        Log.d(TAG, "end preload");

        sPreloadComplete = true;
    }
    
}
```

可以看到，preload 主要的工作是预加载各种类、HAL 层资源、OpenGL 环境、共享库、文字资源、WebView 资源等。

## forkSystemServer

现在看 forkSystemServer，它用于启动 SystemServer 进程，SystemServer 是 Android framework 中特别关键的一个类，用于启动各种系统服务：

```
public class ZygoteInit {

    /**
     * Prepare the arguments and forks for the system server process.
     *
     * Returns an {@code Runnable} that provides an entrypoint into system_server code in the
     * child process, and {@code null} in the parent.
     */
    private static Runnable forkSystemServer(String abiList, String socketName,
            ZygoteServer zygoteServer) {
        
        ...
        
        /* Hardcoded command line to start the system server */
        String args[] = {
            "--setuid=1000",
            "--setgid=1000",
            "--setgroups=1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1018,1021,1023,1032,3001,3002,3003,3006,3007,3009,3010",
            "--capabilities=" + capabilities + "," + capabilities,
            "--nice-name=system_server",
            "--runtime-args",
            "com.android.server.SystemServer", // 注意这个参数
        };
        ZygoteConnection.Arguments parsedArgs = null;

        int pid;

        try {
            // 编译参数到 ZygoteConnection 中
            parsedArgs = new ZygoteConnection.Arguments(args);
            ZygoteConnection.applyDebuggerSystemProperty(parsedArgs);
            ZygoteConnection.applyInvokeWithSystemProperty(parsedArgs);

            /* Request to fork the system server process */
            // 请求 fork 一个 SystemServer 进程
            pid = Zygote.forkSystemServer(
                    parsedArgs.uid, parsedArgs.gid,
                    parsedArgs.gids,
                    parsedArgs.debugFlags,
                    null,
                    parsedArgs.permittedCapabilities,
                    parsedArgs.effectiveCapabilities);
        } catch (IllegalArgumentException ex) {
            throw new RuntimeException(ex);
        }

        /* For child process */
        // 子进程，即 SystemServer 的进程
        if (pid == 0) { 
            if (hasSecondZygote(abiList)) {
                waitForSecondaryZygote(socketName);
            }

            zygoteServer.closeServerSocket();
            return handleSystemServerProcess(parsedArgs); // 启动 SystemServer
        }

        return null;
    }
    
}
```

可以看到，forkSystemServer 先是通过 Zygote fork 了一个进程，接着调用 handleSystemServerProcess 来启动 SystemServer。Zygote 的实现如下：

```
public final class Zygote {

    /**
     * Special method to start the system server process.
     */
    public static int forkSystemServer(int uid, int gid, int[] gids, int debugFlags,
            int[][] rlimits, long permittedCapabilities, long effectiveCapabilities) {
        ...
        int pid = nativeForkSystemServer(
                uid, gid, gids, debugFlags, rlimits, permittedCapabilities, effectiveCapabilities);
        ...
        return pid;
    }
    
}
```

fork 进程的操作是通过本地代码实现的：

```
// com_android_internal_os_Zygote.cpp

static const char kZygoteClassName[] = "com/android/internal/os/Zygote";
static jclass gZygoteClass;
static jmethodID gCallPostForkChildHooks;

static jint com_android_internal_os_Zygote_nativeForkAndSpecialize(...) {
    ...
    return ForkAndSpecializeCommon(env, uid, gid, gids, debug_flags,
            rlimits, capabilities, capabilities, mount_external, se_info,
            se_name, false, fdsToClose, fdsToIgnore, instructionSet, appDataDir);
}

// Utility routine to fork zygote and specialize the child process.
static pid_t ForkAndSpecializeCommon(...) {
    ...
    pid_t pid = fork(); // 这里才真正 fork 了一个新的进程
    if (pid == 0) {
        ...
        // 回调 Zygote 的 callPostForkChildHooks，通知虚拟机子进程已创建完毕
        env->CallStaticVoidMethod(gZygoteClass, gCallPostForkChildHooks, debug_flags,
                                  is_system_server, instructionSet);
    } else if (pid > 0) {
        // 父进程，什么都不做
    }
    return pid;
}
```

可以看到，本地代码调用的是 Linux 的 fork 函数，此时一个新的进程就已经创建完毕了，下面看 handleSystemServerProcess：

```
public class ZygoteInit {

    /**
     * Finish remaining work for the newly forked system server process.
     * 注意参数 parsedArgs 是 forkSystemServer 传过来的，包含了 SystemServer 的包名
     */
    private static Runnable handleSystemServerProcess(ZygoteConnection.Arguments parsedArgs) { 
        ...

        if (parsedArgs.invokeWith != null) { // 比较 forkSystemServer 中的参数，可知这里为 null
            ...
        } else {
            ClassLoader cl = null;
            if (systemServerClasspath != null) { // 找到 SystemServer 对应的 ClassLoader
                cl = createPathClassLoader(systemServerClasspath, parsedArgs.targetSdkVersion);

                Thread.currentThread().setContextClassLoader(cl);
            }

            /*
             * Pass the remaining arguments to SystemServer.
             */
            return ZygoteInit.zygoteInit(parsedArgs.targetSdkVersion, parsedArgs.remainingArgs, cl);
        }
    }
    
    public static final Runnable zygoteInit(int targetSdkVersion, String[] argv, ClassLoader classLoader) {
        ...
        ZygoteInit.nativeZygoteInit();
        return RuntimeInit.applicationInit(targetSdkVersion, argv, classLoader);
    }
    
    private static final native void nativeZygoteInit();
    
}
```

handleSystemServerProcess 主要做了 3 件事：
- 找到 SystemServer 对应的 ClassLoader
- 调用 nativeZygoteInit
- 调用 RuntimeInit.applicationInit

下面看 nativeZygoteInit：

```
// framework/base/core/jni/AndroidRuntime.cpp
static void com_android_internal_os_ZygoteInit_nativeZygoteInit(JNIEnv* env, jobject clazz)
{
    gCurRuntime->onZygoteInit();
}
```

onZygoteInit 等方法回调是 AndroidRuntime 子类 AppRuntime 实现的：

```
class AppRuntime : public AndroidRuntime
{
public:
    ...
    virtual void onZygoteInit()
    {
        sp<ProcessState> proc = ProcessState::self();
        proc->startThreadPool();
    }
    
}
```

startThreadPool 将开启 Binder 线程池以保证其它进程可以正确访问到 Zygote 所提供的服务，startThreadPool 的源码实现跳过，继续看 RuntimeInit.applicationInit：

```
public class RuntimeInit {

    protected static Runnable applicationInit(int targetSdkVersion, String[] argv,
            ClassLoader classLoader) {
        ...
        // Remaining arguments are passed to the start class's static main
        return findStaticMain(args.startClass, args.startArgs, classLoader);
    }
    
    private static Runnable findStaticMain(String className, String[] argv,
            ClassLoader classLoader) {
        Class<?> cl;

        try {
            cl = Class.forName(className, true, classLoader); // 加载 SystemServer 类
        } catch (ClassNotFoundException ex) {
            ...
        }

        Method m;
        try {
            m = cl.getMethod("main", new Class[] { String[].class }); // 找到 SystemServer 类的 main 方法
        } catch (NoSuchMethodException ex) {
            ...
        }

        return new MethodAndArgsCaller(m, argv);
    }
    
    static class MethodAndArgsCaller implements Runnable {
        /** method to call */
        private final Method mMethod;

        /** argument array */
        private final String[] mArgs;

        public MethodAndArgsCaller(Method method, String[] args) {
            mMethod = method;
            mArgs = args;
        }

        public void run() {
            try {
                mMethod.invoke(null, new Object[] { mArgs }); // 调用 SystemServer 的 main 方法
            } catch (IllegalAccessException ex) {
                ...
            }
        }
    }
    
}
```

可以看到，applicationInit 的作用就是返回一个能够调用 SystemServer 的 main 方法的 Runnable，这个 Runnable 将会在 ZygoteInit 的 main 方法中被调用。也就是说，applicationInit 返回的 Runnable 对象会被马上执行，此时 SystemServer 的 main 方法会被调用，并启动众多系统服务。

这里对  forkSystemServer 的作用做一个小结：
- fork 一个新的进程
- 找到 SystemServer 对应的类加载器
- 调用 SystemServer 的 main 方法，启动 SystemServer

## runSelectLoop

forkSystemServer 分析完毕，接下来看 runSelectLoop：

```
class ZygoteServer {

    private LocalServerSocket mServerSocket;

    /**
     * Runs the zygote process's select loop. Accepts new connections as
     * they happen, and reads commands from connections one spawn-request's
     * worth at a time.
     */
    Runnable runSelectLoop(String abiList) {
        ...
        while (true) {
            ...
            for (int i = pollFds.length - 1; i >= 0; --i) {
                if ((pollFds[i].revents & POLLIN) == 0) {
                    continue;
                }

                if (i == 0) {
                    ...
                } else {
                    try {
                        ZygoteConnection connection = peers.get(i);
                        final Runnable command = connection.processOneCommand(this);

                        if (mIsForkChild) { // 如果当前是子进程
                            // We're in the child. We should always have a command to run at this
                            // stage if processOneCommand hasn't called "exec".
                            // 在执行 exec 命令之前，子进程的 command 不应该为 null
                            if (command == null) {
                                throw new IllegalStateException("command == null");
                            }
                            return command;
                        } else { // 父进程
                            // We're in the server - we should never have any commands to run.
                            // 在 server 进程中，不应该有任何需要运行的命令
                            if (command != null) {
                                throw new IllegalStateException("command != null");
                            }

                            // We don't know whether the remote side of the socket was closed or
                            // not until we attempt to read from it from processOneCommand. This shows up as
                            // a regular POLLIN event in our regular processing loop.
                            // 如果连接关闭了，则移除资源
                            if (connection.isClosedByPeer()) {
                                connection.closeSocket();
                                peers.remove(i);
                                fds.remove(i);
                            }
                        }
                    } catch (Exception e) {
                        ...
                    }
                }
            }
        }
    }
    
}
```

可以看到，runSelectLoop 就是一个死循环，它会不断地获取 ZygoteConnection，并执行对应的命令，直到出现异常，或连接关闭。下面看它是如何执行命令的：

```
class ZygoteConnection {

    Runnable processOneCommand(ZygoteServer zygoteServer) {
        ...

        // fork 一个进程，这个方法和前面介绍过的 forkSystemServer 基本是一致的
        pid = Zygote.forkAndSpecialize(parsedArgs.uid, parsedArgs.gid, parsedArgs.gids,
                parsedArgs.debugFlags, rlimits, parsedArgs.mountExternal, parsedArgs.seInfo,
                parsedArgs.niceName, fdsToClose, fdsToIgnore, parsedArgs.instructionSet,
                parsedArgs.appDataDir);

        try {
            if (pid == 0) {
                // in child 标记为子进程
                zygoteServer.setForkChild();

                zygoteServer.closeServerSocket();
                serverPipeFd = null;

                // 新创建的进程需要运行应用程序本身的代码
                return handleChildProc(parsedArgs, descriptors, childPipeFd);
            } else {
                ...
                // 父进程的扫尾工作，包括：将子进程加入进程组、正确关闭文件、调用方返回结果值等
                handleParentProc(pid, descriptors, serverPipeFd);
                return null;
            }
        } finally {
            ...
        }
    }
    
}
```

简单看一下 handleChildProc：

```
class ZygoteConnection {

    /**
     * Handles post-fork setup of child proc, closing sockets as appropriate,
     * reopen stdio as appropriate, and ultimately throwing MethodAndArgsCaller
     * if successful or returning if failed.
     */
    private Runnable handleChildProc(Arguments parsedArgs, FileDescriptor[] descriptors,
            FileDescriptor pipeFd) {

        closeSocket();
        ...

        if (parsedArgs.niceName != null) {
            Process.setArgV0(parsedArgs.niceName);
        }

        if (parsedArgs.invokeWith != null) {
            ... // 拼接 init.rc 命令，启动对应的服务
        } else {
            // 前面分析过
            return ZygoteInit.zygoteInit(parsedArgs.targetSdkVersion, parsedArgs.remainingArgs,
                    null /* classLoader */);
        }
    }
    
}
```

可以看到，和 handleSystemServerProc 类似，handleChildProc 最终会调用 ZygoteInit.zygoteInit，以开启一个 Binder 线程池，并执行对应的类的 main 方法。

小结一下，runSelectLoop 的作用为：不断处理来自 Socket 的请求。对于每个请求，Zygote 会 fork 一个进程，并开启 Binder 线程池，以便子进程能够和系统进程进行交互，最后调用指定的类的 main 方法——实际上，一个新的 Application 启动时，被调用的是 ActivityThread 的 main 方法。

如此，从 Android 虚拟机的启动，到 Zygote 的启动，再到应用进程的启动、ActivityThread 的执行，这一条线就基本连起来了。

# 总结

连线中的一些关键的点：
- Android 虚拟机和 Zygote 是 init 进程通过解析 init.rc 文件启动的，init.rc 文件指定了参数 Zygote 及 SystemServer
- Android 虚拟机成功启动后，会调用 ZygoteInit 的 main 方法，main 方法会阻塞执行直到虚拟机退出
- ZygoteInit 的 main 方法首先会注册一个 UNIX-domain Socket，用于监听来自客户端的请求
- 接着预加载各类资源，包括 HAL 层资源、OpenGL 环境以及各种类等
- 然后 fork 一个进程用于运行 SystemServer，并调用 SystemServer 的 main 方法，启动各类服务
- 最后进入一个死循环，不断地处理 Socket 请求
- 当有新的请求到来时，Zygote 会 fork 一个进程，并开启 Binder 线程池，以便子进程能够和系统进程进行交互，最后调用 ActivityThread 的 main 方法

流程图：
![Zygote](https://upload-images.jianshu.io/upload_images/2176079-cb1b408d7b60f6b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



