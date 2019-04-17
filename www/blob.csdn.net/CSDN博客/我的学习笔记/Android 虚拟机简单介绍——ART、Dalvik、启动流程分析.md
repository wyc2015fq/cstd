# Android 虚拟机简单介绍——ART、Dalvik、启动流程分析 - 我的学习笔记 - CSDN博客





2018年09月24日 16:38:25[zouzhiheng](https://me.csdn.net/u011330638)阅读数：359








Android 虚拟机方面的知识，我是通过《深入理解 Android 内核设计思想》来学习的，内容特别多（只有一章，但有 160 页），感觉和 Android 开发有些偏，因此很多内容都没有认真去看，比如 EFL 格式等，这里只是选取了一些感觉比较重要的做一个大致的简单的介绍。

# 虚拟机基础知识

## Java VM

详见《深入理解 Java 虚拟机》

## LLVM

LLVM 全称是 Low Level Virtual Machine，但和虚拟机没太大关系，官方定义是：The LLVM Project is a colection of modular and resuable compiler and toolchain technologies。即 LLVM 的价值在于可模块化、可重复使用。

LLVM 框架如下所示：

![LLVM](https://img-blog.csdn.net/20180924162519394?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

Frontend：负责分析源代码、检查错误，然后将源码编译成抽象语法树

Optimizer：通过多种优化手段来提高代码的运行效率，在一定程度上独立于具体的语言和目标平台

Backend：也被称为代码生成器，用于将前述的源码转化为目标前台的指令集

LLVM 的模块化：

![LLVM 的模块化](https://img-blog.csdn.net/20180924162502302?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

可以看出，如果要让基于 LLVM 框架的编译器支持一种新语言，那么所要做的可能仅仅是实现一个新的 Frontend，而已有的 Optimizer 和 Backend 则能做到重复使用。上述能力得到实现的关键在于 LLVM 的 Intermediate Representation(IR)，IR 能在 LLVM 的编译器中（具体在 Optimizer 阶段）以一种相对独立的方式来表述各种源代码，从而很好地剥离了各种不同语言间的差异，进而实现模块的复用。

LLVM 和 Android 的关系可以参考 RednaxelaFX 大神的一个回答：[Android 中的 LLVM 主要做什么？](https://www.zhihu.com/question/26646049)

简单来说，就是从 Jellybean MR1 版本开始，Google 将 LLVM 作为 Android 工具链和 Android NDK 的替代编译器，可以用于编译在 Android 应用程序中的 C/C++ 代码。

## Android 中的经典垃圾回收算法

Android 中不管是 Dalvik 还是 Art，它们所使用的垃圾回收算法都是基于 Mark-Sweep 的。

GC 的触发时机有：
- 
GC_FOR_MALLOC。堆内存已满的情况下程序尝试去分配新的内存块

- 
GC_CONCURRENT，堆内存超过特定阈值，触发并行的 GC 事件

- 
GC_HPROF_DUMP_HEAP，开发者主动请求创建 HPROF

- 
GC_EXPLICIT，程序主动调用 gc() 函数，尽量避免这种用法


## Art 和 Dalvik 之争

Dalvik 是 Android 4.4 之前的标准虚拟机，为了性能上的考虑，Dalvik 所做出的努力有：
- 
多个 Class 文件融合进一个 Dex 文件中，以节省内存空间

- 
Dex 文件可以在多个进程之间共享

- 
在应用程序运行之前完成字节码的检验操作，因为检验操作十分耗时

- 
优化字节码

- 
多个进程共享的代码不能随意编辑，这是为了保证安全性


但 Android 从诞生起就背负了“系统庞大，运行慢”的包袱，因此，从 Android 4.4 开始，Art 就以和 Dalvik 暂时共存的形式正式进入了人们的视野，而在 Android Lollipop 中正式取代了 Dalvik  的位置。

Art 相比 Dalvik 在性能上有着显著的优势，主要原因在于 Dalvik 虚拟机多数情况下还得通过解释器的方式来执行 Dex 数据（JIT 虽然能在一定程度上提高效率，但也仅仅是针对一小部分情况，作用有限）；而 Art 虚拟机则采用了 AOT(Ahead Of Time) 技术，从而大幅提高了性能。

Dalvik 中的 JIT只有在程序运行过程中才会将部分热点代码编译成机器码，这在某种程度上也加重了 CPU 的负担。而 AOT 则会提前将 Java 代码翻译成针对目标平台的机器码，虽然这也意味着编译时间有所增加，但 Android 系统的构建原本就慢，所以这点牺牲还是值得的。

## Art 虚拟机整体框架

无论是 Dalvik 还是 Art，或者未来可能出现的新型虚拟机，它们提供的功能将全部封装在一个 so 库中，并且对外需要暴露 JNI_GetDefaultVMInitArgs、JNI_CreateVM 和 JNI_GetCreatedJavaVMs 三个接口，使用者（比如 Zygote）只需要按照统一的接口标准就可以控制和使用所有类型的虚拟机了。

组成 Android 虚拟机的核心自系统包括但不限于 Runtime、ClassLoader System、Execution、Engine System、Heap Manager 和 GC 系统、JIT、JNI 环境等。

和标准的 JVM 一样，类加载器在 Android 虚拟机中也扮演者很重要的作用，可以分为 Boot ClassLoader、System ClassLoader、Dex ClassLoader 等，所有被加载的类和它们的组成元素都将由 ClassLinker 做统一的管理。

除了字节码解释执行的方式，Art 还支持通过 AOT 来直接执行字节码编译而成的机器码。AOT 的编译时机有两个：随 Android ROM 构建时一起编译、程序安装时执行编译（针对第三方应用程序）。Art 引入了新的存储格式，即 OAT 文件来存储编译后的机器代码。而 OAT 机器码的加载需要用到 ELF 的基础能力。

另外，由于一股脑地在程序安装阶段将 Dex 转化为 OAT 造成造成了一定的资源浪费，从 Android N 版本开始，Art 又改变了之前的 OAT 策略——程序在安装时不再统一执行 dex2oat，而改由根据程序的实际运行情况来决定有哪些部分需要被编译成本地代码，即恢复了 Interpreter、JIT、OAT 三足鼎立的局面。一方面，这种新变化大幅加快了程序的安装速度，解决了系统更新时用户需要经历漫长等待的问题；另一方面，由于程序的首次启动必须通过解释器来运行，Android N 版本必须采用多种手段（新的解释器，将 Verification 前移等）来保证程序的启动速度不受影响。

应用程序除了解释执行外，还会在运行过程中实时做 JIT 编译——不过它的结果并不会被持久化。另外，虚拟机会记录下应用程序在动态运行过程中被执行过的函数，并输出到 Profile 文件里。

AOT compile daemon 将在系统同时满足 idle 和充电状态两个条件时才会被唤醒，并按照一定的逻辑来遍历执行应用程序的 AOT 优化。由于参与 AOT 的函数数量通常只占应用程序代码的一小部分，所以整体而言 Android N 版本 AOT 结果所占用的空间大小比旧版本要小很多。

# Dex 字节码

Java 类文件和 DEX 文件对比：

![类文件 VS DEX 文件](https://img-blog.csdn.net/20180924162912343?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# ELF 文件格式

Art 虚拟机最大的特点就是通过 dex2oat 将 Dex 预编译为包含了机器指令的 oat 文件，从而显著提升了程序的执行效率。而 oat 文件本身是基于 Linux 的可执行文件格式——ELF 所做的扩展。

ELF 文件至少支持 3 中形态：可重定向文件（Relocatable File）、可执行文件（Executable File）、可共享的对象文件（Shared Object File）。

Relocatable File 的一个具体范例是 .o 文件，它是在编译过程中产生的中间文件。

Shared Object File 即动态链接库，通常以 “.so” 为后缀名。

静态链接库的特点是会在程序的编译链接阶段就完成函数和变量的地址解析工作，并使之成为可执行程序中不可分割的一部分。

动态链接库不需要在编译时就打包到可执行程序中，而是等到后者在运行阶段在实现动态的加载和重定位。动态链接库在被加载到内存中之后，操作系统需要为它执行动态连接操作。

动态链接库的处理过程如下：
- 
在编译阶段，程序经历了预编译、编译、汇编及链接操作后，最终形成一个 ELF 可执行程序。同时程序所依赖的动态库会被记录到 .dynamic 区段中；加载动态库所需的 Linker 由 .interp 来指示。

- 
程序运行起来后，系统首先会通过 .interp 区段找到连接器的绝对路径，然后将控制权交给它

- 
Linker 负责解析 .dynamic 中的记录，得出程序依赖的所有动态链接库

- 
动态链接库加载完成后，它们所包含的 export 函数在内存中的地址就可以确定下来了，Linker 通过预设机制（如 GOT）来保证程序中引用到外部函数的地方可以正常工作，即完成 Dynamic Relocation


# OAT

与 OAT 相关的文件格式后缀有几种：
- 
.art，这个文件也被称为 image，由 dex2oat 工具生成，它的内部包含了很多 Dex 文件，内部存储的是加载好的class信息以及一些事先创建好的对象，Zygote 在启动过程中会加载 boot.art

- 
.oat，由 dex2oat 工具生成，boot.oat 内部存储的是代码

- 
.odex，在 Dalvik 中，.odex 表示被优化后的 Dex 文件；Art 中也同样存在 odex 文件，但和 Dalvik 中的情况不同


应用程序的安装流程：

![应用程序的安装流程1](https://img-blog.csdn.net/20180924163216675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![应用程序的安装流程2](https://img-blog.csdn.net/2018092416315381?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# Android 虚拟机的典型启动流程

Android 虚拟机启动的大致流程图如下：

![Android 启动过程](https://img-blog.csdn.net/20180924163337114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面分析具体的代码执行过程，首先看脚本 init.rc 与 zygote 相关的内容：

```
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
    class main
    socket zygote stream 660 root system
    onrestart write /sys/android_power/request_state wake
    onrestart write /sys/power/state on
    onrestart restart media
    onrestart restart netd
```

app_process 是 zygote 的载体，其 main 函数如下：

```
int main(int argc, const char* const argv[])
{
    ...
    if (zygote) {
        runtime.start("com.android.internal.os.ZygoteInit",
                startSystemServer ? "start-system-server" : "");
    } else if (className) {
        ...
        runtime.start("com.android.internal.os.RuntimeInit",
                application ? "application" : "tool");
    } else {
        ...
    }
}
```

runtime 指 AndroidRuntime：

```
void AndroidRuntime::start(const char* className, const Vector<String8>& options, bool zygote)
{
    JniInvocation jni_invocation;
    jni_invocation.Init(NULL); // 初始化当前的运行环境
    
    JNIEnv* env;
    if (startVm(&mJavaVM, &env, zygote) != 0) { // 启动虚拟机
        return;
    }
    
    onVmCreated(env); // 虚拟机创建成功，执行回调函数通知调用者
    
    /*
     * 注册 native 函数
     */
    if (startReg(env) < 0) {
        ALOGE("Unable to register all android natives\n");
        return;
    }
    
    /*
     * 开始执行目标对象的主函数
     */
    char* slashClassName = toSlashClassName(className != NULL ? className : "");
    jclass startClass = env->FindClass(slashClassName);
    if (startClass == NULL) {
        ALOGE("JavaVM unable to locate class '%s'\n", slashClassName);
        /* keep going */
    } else {
        // 执行 main 函数
        jmethodID startMeth = env->GetStaticMethodID(startClass, "main",
            "([Ljava/lang/String;)V");
        if (startMeth == NULL) {
            ALOGE("JavaVM unable to find main() in '%s'\n", className);
            /* keep going */
        } else {
            env->CallStaticVoidMethod(startClass, startMeth, strArray);
#if 0
            if (env->ExceptionCheck())
                threadExitUncaughtException(env);
#endif
        }
    }
    
}
```

在启动虚拟机之前，需要初始化当前的运行环境，具体是由 JniInvocation::Init 实现的：

```
bool JniInvocation::Init(const char* library) {
    library = GetLibrary(library, buffer); // 获取虚拟机动态链接库的名称

    handle_ = dlopen(library, kDlopenFlags); // 打开虚拟机动态链接库

    ...

    // 分别查找 VM 库中的 3 个重要接口实现
    // 无论是 ART，还是 Dalvik，或者未来的其它虚拟机都需要实现这 3 个接口，它们是统一的接口标准
    // zygote 通过这 3 个接口控制 Android 的虚拟机
    if (!FindSymbol(reinterpret_cast<void**>(&JNI_GetDefaultJavaVMInitArgs_),
                  "JNI_GetDefaultJavaVMInitArgs")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&JNI_CreateJavaVM_),
                  "JNI_CreateJavaVM")) {
        return false;
    }
    if (!FindSymbol(reinterpret_cast<void**>(&JNI_GetCreatedJavaVMs_),
                  "JNI_GetCreatedJavaVMs")) {
        return false;
    }
    
    return true;
}
```

现在可以启动和初始化虚拟机了，核心工作是在 startVm 中完成的：

```
int AndroidRuntime::startVm(JavaVM** pJavaVM, JNIEnv** pEnv, bool zygote)
{
    ...
    if (JNI_CreateJavaVM(pJavaVM, pEnv, &initArgs) < 0) {
        ALOGE("JNI_CreateJavaVM failed\n");
        return -1;
    }
    return 0;
}
```

可以看到，它调用了 JniInvocation::Init 中找到的 JNI_CreateJavaVM 接口，该接口用于创建一个虚拟机：

```
extern "C" jint JNI_CreateJavaVM(JavaVM** p_vm, JNIEnv** p_env, void* vm_args) {
  ...
  
  RuntimeOptions options;
  for (int i = 0; i < args->nOptions; ++i) { // 解析所有的虚拟机选项
    JavaVMOption* option = &args->options[i];
    options.push_back(std::make_pair(std::string(option->optionString), option->extraInfo));
  }
  
  bool ignore_unrecognized = args->ignoreUnrecognized;
  
  if (!Runtime::Create(options, ignore_unrecognized)) { // 创建一个 Runtime 运行环境
    return JNI_ERR;
  }
  
  
  android::InitializeNativeLoader();
  Runtime* runtime = Runtime::Current();
  bool started = runtime->Start(); // 通过 Runtime 来启动其管理的虚拟机
  if (!started) { // 启动失败
    delete Thread::Current()->GetJniEnv();
    delete runtime->GetJavaVM();
    LOG(WARNING) << "CreateJavaVM failed";
    return JNI_ERR;
  }
  
  *p_env = Thread::Current()->GetJniEnv();
  *p_vm = runtime->GetJavaVM(); // 获得已经启动完成的虚拟机示例，并传递给调用者
  
  return JNI_OK;
}
```

Runtime::Start 成功启动了一个虚拟机后，会通过 Init 函数来初始化：

```
bool Runtime::Init(RuntimeArgumentMap&& runtime_options_in) {

  ...
  
  heap_ = new gc::Heap(...); // 创建堆管理对象
  
  ...
  
  java_vm_ = JavaVMExt::Create(this, runtime_options, &error_msg);  // 创建 Java 虚拟机对象
  
  
  Thread::Startup();
  Thread* self = Thread::Attach("main", false, nullptr, false); // 主线程 attach
  
  if (UNLIKELY(IsAotCompiler())) {
    class_linker_ = new AotClassLinker(intern_table_);
  } else {
    class_linker_ = new ClassLinker(intern_table_);
  }
  
  if (GetHeap()->HasBootImageSpace()) { // 当前 Heap 是否包含 Boot Image(比如 boot.art)
    bool result = class_linker_->InitFromBootImage(&error_msg);
    ...
  } else {
    
    if (runtime_options.Exists(Opt::BootClassPathDexList)) {
      boot_class_path.swap(*runtime_options.GetOrDefault(Opt::BootClassPathDexList));
    } else {
      OpenDexFiles(dex_filenames,
                   dex_locations,
                   runtime_options.GetOrDefault(Opt::Image),
                   &boot_class_path);
    }
    
    instruction_set_ = runtime_options.GetOrDefault(Opt::ImageInstructionSet);
    
    if (!class_linker_->InitWithoutImage(std::move(boot_class_path), &error_msg)) {
      LOG(ERROR) << "Could not initialize without image: " << error_msg;
      return false;
    }
    // TODO: Should we move the following to InitWithoutImage?
    SetInstructionSet(instruction_set_);
    ...
  }
  
  return true;
}
```

总结：
- 
Android 虚拟机是 init.rc 被解析的时候启动的

- 
init.rc 在解析 zygote 的时候，调用了 AndroidRuntime 的 start 方法来启动 Android 虚拟机

- 
AndroidRuntime 首先通过 JniInvocation::Init 初始化运行环境，找到 JNI_GetDefaultVMInitArgs、JNI_CreateVM 和 JNI_GetCreatedJavaVMs 三个标准接口

- 
接着执行 startVm，这个方法成功执行后，Android 虚拟机就被真正启动了

- 
startVm 内部调用之前找到的标准接口之一 JNI_CreateVM

- 
JNI_CreateVM 内部创建了一个 Runtime 运行环境，并通过 Runtime 启动了虚拟机

- 
虚拟机成功启动后，调用了 Runtime::Init 方法，用于初始化虚拟机，包括：创建 Java 虚拟机、创建 Heap 堆管理对象、加载主线程等

- 
最后，通过 onVmCreated 方法通知虚拟机已经成功创建




