# boost:进程管理 - DoubleLi - 博客园







## 概述

Boost.Process提供了一个灵活的C++ 进程管理框架。它允许C++ developer可以像Java和.Net程序developer那样管理进程。它还提供了管理当前执行进程上下文、创建子进程、用C++ 流和异步I/O进行通信的能力。
该库以完全透明的方式将所有进程管理的抽象细节呈现给使用者，且该库是跨平台的。

## 特点

###### 进程管理

Boost.Process的长期目标是提供一个抽象于操作系统之上的，可以管理任何运行的进程的框架。由于提供这样的API比较困难，所以现在只专注于管理。Boost.Process的最重要的特征就是启动一个外部应用、控制它们并与它们交互。传统上讲，对于C和C++ 来说，就比较困难了，因为它们要启动新进程、执行外部程序、建立匿名管道来交互、等待进程结束、检查进程退出码等。更糟糕的是不同操作系统，相关的进程模块和API是不同的。所以，Boost.Process的出现就提供了便利条件。

###### 输入输出重定向

一般来说一个应用启动了子进程，它们可能会通过传输数据来交流。这种进程间通信是文件句柄层面的，通常涉及stdin、stdout、stderr。如果操作系统支持，那么就需要可重定向的流。不过这对C++ 来说是很容易的。

###### 不同操作模式

支持同步、异步、分离

###### 管道管理

这样就可以实现一个进程的输出可以作为另一个进程的输入。

## 库的设计图

![](http://192.168.2.3/download/attachments/19302055/design.png?version=1&modificationDate=1415789173000)

最重要的类就是Context和Process。Context提供了进程运行的上下文。pistream和postream是为了交互。父进程还可以等待子进程退出，并检查进程退出码。如果有例如包含管道的shell命令要执行，那么pipeline_entry就应运而生了，它可以实现前一个子进程的输出是下一个子进程的输入。

###### 使用步骤

1、创建上下文Context
2、创建子进程，获得子进程对象
3、如果有重定向，可以访问到stdin、stdout、stderr
4、进程结束，检查进程退出码

## 教程

一个最简单的例子




|```cpp#include <boost/filesystem.hpp>``````cpp#include <boost/process.hpp>``````cpp#include <string>``````cpp#include <vector>``````cppnamespace``````cppbp = ::boost::process;``````cppbp::child start_child()``````cpp{``````cpp``````cppstd::string exec =``````cpp"bjam"``````cpp;``````cpp``````cppstd::vector<std::string> args;``````cpp``````cppargs.push_back(``````cpp"bjam"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--version"``````cpp);``````cpp``````cppbp::context ctx;``````cpp``````cppctx.stdout_behavior = bp::capture_stream();``````cpp``````cppreturn``````cppbp::launch(exec, args, ctx);``````cpp}``````cppint``````cppmain()``````cpp{``````cpp``````cppbp::child c = start_child();``````cpp``````cppbp::pistream &is = c.get_stdout();``````cpp``````cppstd::string line;``````cpp``````cppwhile``````cpp(std::getline(is, line))``````cpp``````cppstd::cout << line << std::endl;``````cpp``````cppbp::status s = c.wait();``````cpp``````cppreturn``````cpps.exited() ? s.exit_status() : EXIT_FAILURE;``````cpp}```|
|----|





下面再看一个异步的例子




|```cpp#include <boost/filesystem.hpp>``````cpp#include <boost/asio.hpp>``````cpp#include <boost/process.hpp>``````cpp#include <boost/array.hpp>``````cpp#include <boost/bind.hpp>``````cpp#include <string>``````cpp#include <vector>``````cpp#include <iostream>``````cppnamespace``````cppbp = ::boost::process;``````cppnamespace``````cppba = ::boost::asio;``````cppba::io_service io_service;``````cppboost::array<``````cppchar``````cpp, 4096> buffer;``````cppba::posix::stream_descriptor in(io_service);``````cppbp::child start_child()``````cpp{``````cpp``````cppstd::string exec =``````cpp"bjam"``````cpp;``````cpp``````cppstd::vector<std::string> args;``````cpp``````cppargs.push_back(``````cpp"bjam"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--version"``````cpp);``````cpp``````cppbp::context ctx;``````cpp``````cppctx.stdout_behavior = bp::capture_stream();``````cpp``````cppctx.environment = bp::self::get_environment();``````cpp``````cppreturn``````cppbp::launch(exec, args, ctx);``````cpp}``````cppvoid``````cppend_read(``````cppconst``````cppboost::``````cppsystem``````cpp::error_code &ec, std::``````cppsize_t``````cppbytes_transferred);``````cppvoid``````cppbegin_read()``````cpp{``````cpp``````cppin.async_read_some(boost::asio::buffer(buffer),``````cpp``````cppboost::bind(&end_read, ba::placeholders::error, ba::placeholders::bytes_transferred));``````cpp}``````cppvoid``````cppend_read(``````cppconst``````cppboost::``````cppsystem``````cpp::error_code &ec, std::``````cppsize_t``````cppbytes_transferred)``````cpp{``````cpp``````cppif``````cpp(!ec)``````cpp``````cpp{``````cpp``````cppstd::cout << std::string(buffer.data(), bytes_transferred) << std::flush;``````cpp``````cppbegin_read();``````cpp``````cpp}``````cpp}``````cppint``````cppmain()``````cpp{``````cpp``````cppbp::child c = start_child();``````cpp``````cppbp::pistream &is = c.get_stdout();``````cpp``````cppin.assign(is.handle().release());``````cpp``````cppbegin_read();``````cpp``````cppio_service.run();``````cpp``````cppc.wait();``````cpp}```|
|----|





这个例子中用到了asio库，涉及到许多回调函数。关于异步(asio)暂时不做讲解，写这个例子是为了展示该库的异步功能。对异步感兴趣的同学可以看一下《[Boost.Asio C++ Network Programming](http://192.168.2.3/download/attachments/19302055/a.pdf?version=1&modificationDate=1415789173000)》



## 部分文件和类

###### stream_behaviour.hpp文件

对于流的描述，可分为六种类型

|序号|流描述|含义|
|----|----|----|
|1|capture|父子进程之间通过无名管道相互接收数据|
|2|close|启动时关闭|
|3|inherit|父子进程共用一个，也即继承|
|4|redirect_to_stdout|主要用在stderr时，重定向到stdout|
|5|silence|输出重定向到/dev/null|
|6|posix_redirect|将输出重定向到指定的文件描符，是对redirect_to_stdout的扩展|


以下是等价的
boost::process::child::get_stdin()	<==>	boost::process::posix_child::get_input(STDIN_FILENO)
boost::process::child::get_stdout()	<==>	boost::process::posix_child::get_output(STDOUT_FILENO)
boost::process::child::get_stderr()	<==>	boost::process::posix_child::get_output(STDERR_FILENO)


**重定向的例子**



|```cpp#include <boost/process.hpp>``````cpp#include <boost/filesystem.hpp>``````cpp#include <string>``````cpp#include <vector>``````cpp#include <iostream>``````cpp#include <cstdlib>``````cpp#include <unistd.h>``````cppnamespace``````cppbp = ::boost::process;``````cppbp::posix_child start_child()``````cpp{``````cpp``````cppstd::string exec = bp::find_executable_in_path(``````cpp"dbus-daemon"``````cpp);``````cpp``````cppstd::vector<std::string> args;``````cpp``````cppargs.push_back(``````cpp"dbus-daemon"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--fork"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--session"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--print-address=3"``````cpp);``````cpp``````cppargs.push_back(``````cpp"--print-pid=4"``````cpp);``````cpp``````cppbp::posix_context ctx;``````cpp``````cppctx.output_behavior.insert(bp::behavior_map::value_type(STDOUT_FILENO, bp::inherit_stream()));``````cpp``````cppctx.output_behavior.insert(bp::behavior_map::value_type(STDERR_FILENO, bp::inherit_stream()));``````cpp``````cppctx.output_behavior.insert(bp::behavior_map::value_type(3, bp::capture_stream()));``````cpp``````cppctx.output_behavior.insert(bp::behavior_map::value_type(4, bp::capture_stream()));``````cpp``````cppreturn``````cppbp::posix_launch(exec, args, ctx);``````cpp}``````cppint``````cppmain()``````cpp{``````cpp``````cpptry``````cpp``````cpp{``````cpp``````cppbp::posix_child c = start_child();``````cpp``````cppstd::string address;``````cpp``````cpppid_t pid;``````cpp``````cppc.get_output(3) >> address;``````cpp``````cppc.get_output(4) >> pid;``````cpp``````cppbp::status s = c.wait();``````cpp``````cppif``````cpp(s.exited())``````cpp``````cpp{``````cpp``````cppif``````cpp(s.exit_status() == EXIT_SUCCESS)``````cpp``````cpp{``````cpp``````cppstd::cout <<``````cpp"D-BUS daemon's address is: "``````cpp<< address << std::endl;``````cpp``````cppstd::cout <<``````cpp"D-BUS daemon's PID is: "``````cpp<< pid << std::endl;``````cpp``````cpp}``````cpp``````cppelse``````cpp``````cppstd::cout <<``````cpp"D-BUS daemon returned error condition: "``````cpp<< s.exit_status() << std::endl;``````cpp``````cpp}``````cpp``````cppelse``````cpp``````cppstd::cout <<``````cpp"D-BUS daemon terminated abnormally"``````cpp<< std::endl;``````cpp``````cppreturn``````cpps.exited() ? s.exit_status() : EXIT_FAILURE;``````cpp``````cpp}``````cpp``````cppcatch``````cpp(boost::filesystem::filesystem_error &ex)``````cpp``````cpp{``````cpp``````cppstd::cout << ex.what() << std::endl;``````cpp``````cppreturn``````cppEXIT_FAILURE;``````cpp``````cpp}``````cpp}```|
|----|





###### boost::process::context类




|```cpptemplate``````cpp<``````cppclass``````cppPath>``````cppclass``````cppbasic_context :``````cpppublic``````cppbasic_work_directory_context<Path>,``````cpppublic``````cppenvironment_context``````cpp{``````cpppublic``````cpp:``````cpp``````cpp/**``````cpp``````cpp* Child's stdin behavior.``````cpp``````cpp*/``````cpp``````cppstream_behavior stdin_behavior;``````cpp``````cpp/**``````cpp``````cpp* Child's stdout behavior.``````cpp``````cpp*/``````cpp``````cppstream_behavior stdout_behavior;``````cpp``````cpp/**``````cpp``````cpp* Child's stderr behavior.``````cpp``````cpp*/``````cpp``````cppstream_behavior stderr_behavior;``````cpp};``````cpptypedef``````cppbasic_context<std::string> context;```|
|----|





而basic_work_directory_context是用来设置工作目录的；environment_context实质上是个包装了boost::process::environment的类，boost::process::environment是一个map<string, string>,用以保存环境变量。

###### boost::process::posix_context类




|```cpptypedef``````cppstd::map<``````cppint``````cpp, stream_behavior> behavior_map;``````cpptemplate``````cpp<``````cppclass``````cppPath>``````cppclass``````cppposix_basic_context :``````cpppublic``````cppbasic_work_directory_context<Path>,``````cpppublic``````cppenvironment_context``````cpp{``````cpppublic``````cpp:``````cpp``````cpp/**``````cpp``````cpp* Constructs a new POSIX-specific context.``````cpp``````cpp*``````cpp``````cpp* Constructs a new context. It is configured as follows:``````cpp``````cpp* * All communcation channels with the child process are closed.``````cpp``````cpp* * There are no channel mergings.``````cpp``````cpp* * The initial work directory of the child processes is set to the``````cpp``````cpp*   current working directory.``````cpp``````cpp* * The environment variables table is empty.``````cpp``````cpp* * The credentials are the same as those of the current process.``````cpp``````cpp*/``````cpp``````cppposix_basic_context()``````cpp``````cpp: uid(::getuid()),``````cpp``````cppeuid(::geteuid()),``````cpp``````cppgid(::getgid()),``````cpp``````cppegid(::getegid())``````cpp``````cpp{``````cpp``````cpp}``````cpp``````cpp/**``````cpp``````cpp* List of input streams that will be redirected.``````cpp``````cpp*/``````cpp``````cppbehavior_map input_behavior;``````cpp``````cpp/**``````cpp``````cpp* List of output streams that will be redirected.``````cpp``````cpp*/``````cpp``````cppbehavior_map output_behavior;``````cpp``````cpp/**``````cpp``````cpp* The user credentials.``````cpp``````cpp*``````cpp``````cpp* UID that specifies the user credentials to use to run the %child``````cpp``````cpp* process. Defaults to the current UID.``````cpp``````cpp*/``````cpp``````cppuid_t uid;``````cpp``````cpp/**``````cpp``````cpp* The effective user credentials.``````cpp``````cpp*``````cpp``````cpp* EUID that specifies the effective user credentials to use to run``````cpp``````cpp* the %child process. Defaults to the current EUID.``````cpp``````cpp*/``````cpp``````cppuid_t euid;``````cpp``````cpp/**``````cpp``````cpp* The group credentials.``````cpp``````cpp*``````cpp``````cpp* GID that specifies the group credentials to use to run the %child``````cpp``````cpp* process. Defaults to the current GID.``````cpp``````cpp*/``````cpp``````cppgid_t gid;``````cpp``````cpp/**``````cpp``````cpp* The effective group credentials.``````cpp``````cpp*``````cpp``````cpp* EGID that specifies the effective group credentials to use to run``````cpp``````cpp* the %child process. Defaults to the current EGID.``````cpp``````cpp*/``````cpp``````cppgid_t egid;``````cpp``````cpp/**``````cpp``````cpp* The chroot directory, if any.``````cpp``````cpp*``````cpp``````cpp* Specifies the directory in which the %child process is chrooted``````cpp``````cpp* before execution. Empty if this feature is not desired.``````cpp``````cpp*/``````cpp``````cppPath chroot;``````cpp};``````cpp/**``````cpp``````cpp* Default instantiation of posix_basic_context.``````cpp``````cpp*/``````cpptypedef``````cppposix_basic_context<std::string> posix_context;```|
|----|





函数boost::process::self::get_environment()可以得到当前进程的环境变量。
我们可以对环境变量进行修改，如
boost::process::environment_context env;
env.insert(boost::process::environment::valuetype(“A”, “a”));

###### 进程结束码类信息




|```cppclass``````cppstatus``````cpp{``````cpp``````cppfriend``````cppclass``````cppchild;``````cpppublic``````cpp:``````cpp``````cpp/**``````cpp``````cpp* 进程是否正常退出``````cpp``````cpp*/``````cpp``````cppbool``````cppexited()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 进程返回值``````cpp``````cpp*/``````cpp``````cppint``````cppexit_status()``````cppconst``````cpp;``````cppprotected``````cpp:``````cpp``````cppstatus(``````cppint``````cppflags);``````cpp``````cpp...``````cpp};``````cppclass``````cppposix_status :``````cpppublic``````cppstatus``````cpp{``````cpppublic``````cpp:``````cpp``````cppposix_status(``````cppconst``````cppstatus &s);``````cpp``````cpp/**``````cpp``````cpp* 进程是否因为信号终止``````cpp``````cpp*/``````cpp``````cppbool``````cppsignaled()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 如果因为信号终止，那么是因为哪个信号终止的``````cpp``````cpp*/``````cpp``````cppint``````cppterm_signal()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 是否core dump了``````cpp``````cpp*/``````cpp``````cppbool``````cppdumped_core()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 进程是否因为收到信号停止``````cpp``````cpp*/``````cpp``````cppbool``````cppstopped()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 如果进程因为收到信号停止，那么信号是哪个``````cpp``````cpp*/``````cpp``````cppint``````cppstop_signal()``````cppconst``````cpp;``````cpp}```|
|----|





###### 进程类对象信息




|```cppclass``````cppprocess``````cpp{``````cpppublic``````cpp:``````cpp``````cpptypedef``````cpppid_t id_type;``````cpp``````cppprocess(id_type id);``````cpp``````cpp/**``````cpp``````cpp* Returns the process' identifier.``````cpp``````cpp*/``````cpp``````cppid_type get_id()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 强制终止一个进程，force为真则用SIGKILL杀死，否则用SIGTERM杀死``````cpp``````cpp*/``````cpp``````cppvoid``````cppterminate(``````cppbool``````cppforce =``````cppfalse``````cpp)``````cppconst``````cpp;``````cppprivate``````cpp:``````cpp``````cpp...``````cpp};``````cppclass``````cppchild :``````cpppublic``````cppprocess``````cpp{``````cpppublic``````cpp:``````cpp``````cpp/**``````cpp``````cpp* 获得标准输出``````cpp``````cpp*/``````cpp``````cpppostream &get_stdin()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 获得标准输入``````cpp``````cpp*/``````cpp``````cpppistream &get_stdout()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 获得标准错误输入``````cpp``````cpp*/``````cpp``````cpppistream &get_stderr()``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 阻塞等待进程退出，返回状态码对象``````cpp``````cpp*/``````cpp``````cppstatus wait()；``````cpp``````cpp/**``````cpp``````cpp* 创建一个子进程对象``````cpp``````cpp*/``````cpp``````cppchild(id_type id, detail::file_handle fhstdin, detail::file_handle fhstdout, detail::file_handle fhstderr, detail::file_handle fhprocess = detail::file_handle());``````cppprivate``````cpp:``````cpp``````cpp...``````cpp};``````cppclass``````cppposix_child :``````cpppublic``````cppchild``````cpp{``````cpppublic``````cpp:``````cpp``````cpp/**``````cpp``````cpp* 从指定描述符获得一个输出流``````cpp``````cpp*/``````cpp``````cpppostream &get_input(``````cppint``````cppdesc)``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp* 从指定描述符获得一个输入流``````cpp``````cpp*/``````cpp``````cpppistream &get_output(``````cppint``````cppdesc)``````cppconst``````cpp;``````cpp``````cpp/**``````cpp``````cpp*构造函数``````cpp``````cpp*/``````cpp``````cppposix_child(id_type id, detail::info_map &infoin, detail::info_map &infoout);``````cppprivate``````cpp:``````cpp``````cpp...``````cpp};```|
|----|





###### children类

children类实际上std::vector<child>。children的启动方式是一个输出流被链接到下一个子进程的输入流上。




|```cpp#include <boost/process.hpp>``````cpp#include <string>``````cpp#include <vector>``````cpp#include <iostream>``````cpp#include <fstream>``````cpp#include <cstdlib>``````cppnamespace``````cppbp = ::boost::process;``````cppbp::children start_children()``````cpp{``````cpp``````cppbp::context ctxin;``````cpp``````cppctxin.stdin_behavior = bp::capture_stream();``````cpp``````cppbp::context ctxout;``````cpp``````cppctxout.stdout_behavior = bp::inherit_stream();``````cpp``````cppctxout.stderr_behavior = bp::redirect_stream_to_stdout();``````cpp``````cppstd::string exec1 = bp::find_executable_in_path(``````cpp"cut"``````cpp);``````cpp``````cppstd::vector<std::string> args1;``````cpp``````cppargs1.push_back(``````cpp"cut"``````cpp);``````cpp``````cppargs1.push_back(``````cpp"-d "``````cpp);``````cpp``````cppargs1.push_back(``````cpp"-f2-5"``````cpp);``````cpp``````cppstd::string exec2 = bp::find_executable_in_path(``````cpp"sed"``````cpp);``````cpp``````cppstd::vector<std::string> args2;``````cpp``````cppargs2.push_back(``````cpp"sed"``````cpp);``````cpp``````cppargs2.push_back(``````cpp"s,^,line: >>>,"``````cpp);``````cpp``````cppstd::string exec3 = bp::find_executable_in_path(``````cpp"sed"``````cpp);``````cpp``````cppstd::vector<std::string> args3;``````cpp``````cppargs3.push_back(``````cpp"sed"``````cpp);``````cpp``````cppargs3.push_back(``````cpp"s,$,<<<,"``````cpp);``````cpp``````cppstd::vector<bp::pipeline_entry> entries;``````cpp``````cppentries.push_back(bp::pipeline_entry(exec1, args1, ctxin));``````cpp``````cppentries.push_back(bp::pipeline_entry(exec2, args2, ctxout));``````cpp``````cppentries.push_back(bp::pipeline_entry(exec3, args3, ctxout));``````cpp``````cppreturn``````cppbp::launch_pipeline(entries);``````cpp}``````cppint``````cppmain(``````cppint``````cppargc,``````cppchar``````cpp*argv[])``````cpp{``````cpp``````cpptry``````cpp``````cpp{``````cpp``````cppif``````cpp(argc < 2)``````cpp``````cpp{``````cpp``````cppstd::cerr <<``````cpp"Please specify a file name"``````cpp<< std::endl;``````cpp``````cppreturn``````cppEXIT_FAILURE;``````cpp``````cpp}``````cpp``````cppstd::ifstream file(argv[1]);``````cpp``````cppif``````cpp(!file)``````cpp``````cpp{``````cpp``````cppstd::cerr <<``````cpp"Cannot open file"``````cpp<< std::endl;``````cpp``````cppreturn``````cppEXIT_FAILURE;``````cpp``````cpp}``````cpp``````cppbp::children cs = start_children();``````cpp``````cppbp::postream &os = cs.front().get_stdin();``````cpp``````cppstd::string line;``````cpp``````cppwhile``````cpp(std::getline(file, line))``````cpp``````cppos << line << std::endl;``````cpp``````cppos.close();``````cpp``````cppbp::status s = bp::wait_children(cs);``````cpp``````cppreturn``````cpps.exited() ? s.exit_status() : EXIT_FAILURE;``````cpp``````cpp}``````cpp``````cppcatch``````cpp(boost::filesystem::filesystem_error &ex)``````cpp``````cpp{``````cpp``````cppstd::cout << ex.what() << std::endl;``````cpp``````cppreturn``````cppEXIT_FAILURE;``````cpp``````cpp}``````cpp}```|
|----|





需要注意的是，wait_children出错时，返回第一个子进程的退出码，所有子进程都正常退出时，返回最后一个子进程的退出码。



master3中大量用到进程管理这个库。这个Boost.Process库可以在这里获得[点这里](http://192.168.2.3/download/attachments/19302055/boost_process.tar.gz?version=1&modificationDate=1415789173000)。












