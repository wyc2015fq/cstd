# 深入研究Clang(九)   Clang代码阅读之打log读流程2 - SHINING的博客 - CSDN博客
2017年01月05日 11:42:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：2415
所属专栏：[深入研究Clang](https://blog.csdn.net/column/details/hacking-clang.html)
继续上一篇，同样的hello.c，同样的执行过程，只不过继续添加了一些log信息，而且对代码进行了更近一步的挖掘。先看输入和输出的log信息（前半部分）：
```cpp
shining@shining-VirtualBox:~/llvm-3.9.0/build/bin$ ./clang hello.c -o hello
clang/tools/driver/driver.cpp/main()_begin/shining_add
clang/lib/Driver/Driver.cpp/BuildCompilation()_begin/shining_add
clang/lib/Driver/ToolChains.cpp/Linux()_begin/shining_add
clang/lib/Driver/Tools.cpp/Clang::ConstructJob()_beforeCC1/shining_add
clang/lib/Driver/Driver.cpp/ExecuteCompilation()_begin/shining_add
clang/lib/Driver/Compilation.cpp/ExecuteJobs()_begin/shining_add
clang/lib/Driver/Compilation.cpp/ExecuteCommand()_begin/shining_add
clang/lib/Driver/Job.cpp/Execute()_begin/shining_add
llvm/lib/Support/Program.cpp/ExecuteAndWait()_begin/shining_add
llvm/lib/Support/Unix/Program.inc/Execute()_begin/shining_add
llvm/lib/Support/Unix/Program.inc/Execute()_begin/shining_add333
llvm/lib/Support/Unix/Program.inc/Execute()_begin/shining_add444
clang/tools/driver/driver.cpp/main()_begin/shining_add
clang/tools/driver/driver.cpp/ExecuteCC1Tool()/shining_add
clang/tools/driver/cc1_main.cpp/cc1_main()/shining_add
```
-----------------------------------------------------------------------------------------------------
这个时候，程序已经通过了对命令行输入的第一次初步执行，执行了clang/tools/driver/driver.cpp中的main()函数，并且添加了很多命令行的具体参数，并且新建线程开始第二次执行clang/tools/driver/driver.cpp中的main()函数。在第二次执行main()函数的时候，选择了和第一次执行完全不同的路径，这是因为第一遍执行main()函数的调用过程中，已经为命令行添加了-cc1的参数选项。所以，第二次执行main()函数的时候，会选择执行main()函数的调用ExecuteCC1Tool()的分支，具体代码如下：
```cpp
if (FirstArg != argv.end() && StringRef(*FirstArg).startswith("-cc1")) {
    // If -cc1 came from a response file, remove the EOL sentinels.
    if (MarkEOLs) {
      auto newEnd = std::remove(argv.begin(), argv.end(), nullptr);
      argv.resize(newEnd - argv.begin());
    }
    return ExecuteCC1Tool(argv, argv[1] + 4);
  }
```
-------------------------------------------------------------------------------------------------------
现在再回头对这个过程中的代码进行分析，第一次执行clang/tools/driver/driver.cpp中的main()函数的时候，下面的代码是核心部分：
```cpp
std::unique_ptr<Compilation> C(TheDriver.BuildCompilation(argv));
  int Res = 0;
  SmallVector<std::pair<int, const Command *>, 4> FailingCommands;
  if (C.get())
    Res = TheDriver.ExecuteCompilation(*C, FailingCommands);
```
主要是TheDriver的BuildCompilation和ExecuteCompilation的两个函数的执行，而且这两个是一个顺序的过程，先build再execute。其中，TheDriver是Driver类的一个具体对象，而Driver类的具体实现是在clang/lib/Driver/Driver.cpp中。也就是说，其实clang的代码里面，有两个driver相关目录，并各自包含一些文件，以driver直接命名的cpp文件举例：tools/driver/driver.cpp和lib/Driver/Driver.cpp，前者是驱动编译器的驱动，而后者是Driver类的具体实现。这两个具体文件的差别，基本上也代表了两个目录的主要用途。
------------------------------------------------------------------------------------------------------
Driver类的BuildCompilation方法，通过调用关系，最终调用了clang/lib/Driver/Tools.cpp里的Clang::ConstructJob()函数，这个函数很重要的一个动作就是为了命令行的参数列表添加了“-cc1”参数，这个动作为第二次执行clang/tools/driver/driver.cpp中的main()函数的路径起了决定性的作用。
```cpp
// Invoke ourselves in -cc1 mode.
  //
  // FIXME: Implement custom jobs for internal actions.
  CmdArgs.push_back("-cc1");
```
------------------------------------------------------------------------------------------------------
Driver类的ExecuteCompilation方法，通过调用关系，最终调用Compilation类的几个方法，并且在clang/lib/Driver/Job.cpp/Command::Execute()函数的内部，开始调用llvm部分的support代码llvm/lib/Support/Program.cpp中的llvm::sys::ExecuteAndWait()。
```cpp
//===----------------------------------------------------------------------===//
//=== WARNING: Implementation here must contain only TRULY operating system
//===          independent code.
//===----------------------------------------------------------------------===//
static bool Execute(ProcessInfo &PI, StringRef Program, const char **args,
                    const char **env, const StringRef **Redirects,
                    unsigned memoryLimit, std::string *ErrMsg);
int sys::ExecuteAndWait(StringRef Program, const char **args, const char **envp,
                        const StringRef **redirects, unsigned secondsToWait,
                        unsigned memoryLimit, std::string *ErrMsg,
                        bool *ExecutionFailed) {
  ProcessInfo PI;
  if (Execute(PI, Program, args, envp, redirects, memoryLimit, ErrMsg)) {
    if (ExecutionFailed)
      *ExecutionFailed = false;
    ProcessInfo Result = Wait(
        PI, secondsToWait, /*WaitUntilTerminates=*/secondsToWait == 0, ErrMsg);
    return Result.ReturnCode;
  }
  if (ExecutionFailed)
    *ExecutionFailed = true;
  return -1;
}
```
llvm::sys::ExecuteAndWait()调用了Execute()函数，而Execute()函数则会根据不同的操作系统有不同的实现。我使用的环境是Ubuntu16.10，所以Execute()函数的具体实现就在llvm/lib/Support/Unix/Program.inc中。相应的此处也有一个llvm/lib/Support/Windows/目录，存放Windows操作系统所对应的相关代码实现。在llvm/lib/Support/Unix/Program.inc中Execute()函数的具体实现过程中有
#ifdef HAVE_POSIX_SPAWN
......
#endif
这样的宏开关，这个宏开关在我的本地环境是可用的，所以具体执行了其中的
```cpp
int Err = posix_spawn(&PID, Program.str().c_str(), FileActions,
                          /*attrp*/nullptr, const_cast<char **>(args),
                          const_cast<char **>(envp));
```
根据Execute()函数中的相关注释：
```cpp
// If this OS has posix_spawn and there is no memory limit being implied, use
  // posix_spawn.  It is more efficient than fork/exec.
```
可以看出，这个实现方式是要比fork/exec的执行效率要高，所以在我的环境下，代码走的就是这个路径，而没有执行下面的
```cpp
#endif
  // Create a child process.
  int child = fork();
  switch (child) {
```
这部分代码。posix_spawn或者fork就是为了建立新的线程，从而重新执行clang/tools/driver/driver.cpp中的main()函数，所以才有了第二次执行clang/tools/driver/driver.cpp中的main()函数的出发点。
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
代码和修改后的代码，我也见了一个代码库，地址：[https://github.com/shining1984/clang_code_comment](https://github.com/shining1984/clang_code_comment)
