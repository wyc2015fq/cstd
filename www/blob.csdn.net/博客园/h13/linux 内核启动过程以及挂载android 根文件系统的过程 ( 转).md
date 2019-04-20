# linux 内核启动过程以及挂载android 根文件系统的过程  ( 转) - h13 - 博客园
 主要介绍linux 内核启动过程以及挂载android 根文件系统的过程，以及介绍android 源代码中文件系统部分的浅析。
 主要源代码目录介绍
Makefile （全局的Makefile）
bionic （Bionic 含义为仿生，这里面是一些基础的库的源代码）
bootable （引导加载器）
build （build 目录中的内容不是目标所用的代码，而是编译和配置所需要的脚本和工具）
dalvik （JAVA 虚拟机）
development （程序开发所需要的模板和工具）  external （目标机器使用的一些库）
frameworks （应用程序的框架层）
hardware （与硬件相关的库）
packages （Android 的各种应用程序）
prebuilt （Android 在各种平台下编译的预置脚本）
recovery （与目标的恢复功能相关）
system （Android 的底层的一些库）
out (编译完成后产生的目录，也就是我们移植文件系统需要的目录)
host 目录的结构如下所示：
out/host/
|-- common
| `-- obj （JAVA 库）
`-- linux-x86
|-- bin （二进制程序）
|-- framework （JAVA 库,＊.jar 文件）
|-- lib （共享库*.so）
`-- obj （中间生成的目标文件）
host 目录是一些在主机上用的工具，有一些是二进制程序,有一些是JAVA 的程序。
target 目录的结构如下所示：
out/target/
|-- common
| |-- R （资源文件）
| |-- docs
| `-- obj （目标文件）
`-- product
`-- generic
其中common 目录表示通用的内容，product 中则是针对产品的内容。
在common 目录的obj 中，包含两个重要的目录：
APPS 中包含了JAVA 应用程序生成的目标，每个应用程序对应其中一个子目录，将结合每个应用程序的原始文件生成Android 应用程序的APK 包。  JAVA_LIBRARIES 中包含了JAVA 的库，每个库对应其中一个子目录。
所以，我们提取文件系统主要是在/out/target/product/generic 目录下，我们可以看到里面有obj 目录，进入obj 目录看看，里面是android 文件系统非常重要的内容：
/obj
APPS (文件系统下/system/apps 目录下的各种应用程序)
SHARED_LIBRARIES （存放所有动态库）
STATIC_LIBRARIES（存放所有静态库）
EXECUTABLES （存放各种可执行文件）
Linux 内核启动挂载android根文件系统过程分析
顺便罗列一下内核启动流程：
/arch/arm/boot/compressed/head.S：
Start:
Decompressed_kernel()  //在/arch/arm/boot/compressed/misc.c 中
Call_kernel()
Stext:
/init/main.c
Start_kernel()
Setup_arch()
…
Rest_init()
Init()
Do_basic_setup（）
Prepare_namespace()
看到了这里，我已激动得说不出话了，因为来到我与挂载根文件系统最重要的接口函数。
static int noinline init_post(void)
{
free_initmem();
unlock_kernel();
mark_rodata_ro();
system_state = SYSTEM_RUNNING;
numa_default_policy();
if (sys_open((const char __user *) "/dev/console", O_RDWR, 0) < 0)
printk(KERN_WARNING "Warning: unable to open an initial console.\n");
(void) sys_dup(0);
(void) sys_dup(0);
current->signal->flags |= SIGNAL_UNKILLABLE;
if (ramdisk_execute_command) {
run_init_process(ramdisk_execute_command);
printk(KERN_WARNING "Failed to execute %s\n",ramdisk_execute_command);
}
if (execute_command) {
run_init_process(execute_command);
printk(KERN_WARNING "Failed to execute %s. Attempting ""defaults...\n",
execute_command);
}
run_init_process("/sbin/init");
run_init_process("/etc/init");
run_init_process("/bin/init");
run_init_process("/bin/sh");
panic("No init found. Try passing init= option to kernel.");
}
其中，我们看到行代码run_init_process(execute_command);
execute_command 是从UBOOT 传递过来的参数，一般为/init，也就是调用文件系统里的init 初始化进程。如果找不到init 文件就会在
run_init_process("/sbin/init");
run_init_process("/etc/init");
run_init_process("/bin/init");
run_init_process("/bin/sh");
中找，否则报错。
在这里由于我们的根文件系统是从/linuxrc开始的，所以我硬性把它改为
if (execute_command) {
run_init_process("/linuxrc");
printk(KERN_WARNING "Failed to execute %s. Attempting "
"defaults...\n", execute_command);
}
Android 文件系统初始化核心Init.c文件分析
 上面我们说的init 这个文件是由android 源代码编译来的，编译后在/out/target/product/generic/root/
其源码在/system/core/init/init.c
Init.c 主要功能：
（1）安装SIGCHLD 信号。（如果父进程不等待子进程结束，子进程将成为僵尸进程（zombie）从而占用系统资源。因此需要对SIGCHLD 信号做出处理，回收僵尸进程的资源，避免造成不必要的资源浪费。）
（2）对umask 进行清零。
 何为umask，请看[http://www.szstudy.cn/showArticle/53978.shtml](http://www.szstudy.cn/showArticle/53978.shtml)
（3）为rootfs 建立必要的文件夹，并挂载适当的分区。
/dev （tmpfs）
/dev/pts (devpts)
/dev/socket
/proc (proc)
/sys (sysfs)
（4）创建/dev/null 和/dev/kmsg 节点。
（5）解析/init.rc，将所有服务和操作信息加入链表。
（6）从/proc/cmdline 中提取信息内核启动参数,并保存到全局变量。
（7）先从上一步获得的全局变量中获取信息硬件信息和版本号，如果没有则从/proc/cpuinfo 中提取,并保存到全局变量。
（8）根据硬件信息选择一个/init.(硬件).rc，并解析，将服务和操作信息加入链表。
在G1 的ramdisk 根目录下有两个/init.(硬件).rc：init.goldfish.rc 和init.trout.rc，init 程序会根据上一步获得的硬件信息选择一个解析。
（9）执行链表中带有“early-init”触发的的命令。
（10）遍历/sys 文件夹，是内核产生设备添加事件（为了自动产生设备节点)。
（11）初始化属性系统，并导入初始化属性文件。
（12）从属性系统中得到ro.debuggable，若为1，則初始化keychord 監聽。
（13）打開console,如果cmdline 中沒有指定console 則打開默認的 /dev/console
（14）讀取/initlogo.rle（一張565 rle 壓縮的位圖），如果成功則在
/dev/graphics/fb0 顯示Logo,如果失敗則將/dev/tty0 設為TEXT 模式并打開/dev/tty0,輸出文“ANDROID”字樣。
（15）判斷cmdline 中的參數，并设置属性系统中的参数:
1、 如果 bootmode 為
- factory,設置ro.factorytest 值為1
- factory2,設置ro.factorytest 值為2
- 其他的設ro.factorytest 值為0
2、如果有serialno 参数，則設置ro.serialno，否則為""
3、如果有bootmod 参数，則設置ro.bootmod，否則為"unknown"
4、如果有baseband 参数，則設置ro.baseband，否則為"unknown"
5、如果有carrier 参数，則設置ro.carrier，否則為"unknown"
6、如果有bootloader 参数，則設置ro.bootloader，否則為"unknown"
7、通过全局变量（前面从/proc/cpuinfo 中提取的）設置ro.hardware 和
ro.version。
（16）執行所有触发标识为init 的action。
（17）開始property 服務，讀取一些property 文件，這一動作必須在前面
那些ro.foo 設置后做，以便/data/local.prop 不能干預到他們。
- /system/build.prop
- /system/default.prop
- /data/local.prop
- 在讀取默認的 property 后讀取 presistent propertie，在 /data/property 中
（18）為 sigchld handler 創建信號機制
（19）確認所有初始化工作完成：
device_fd(device init 完成)
property_set_fd(property server start 完成)
signal_recv_fd (信號機制建立)
（20） 執行所有触发标识为early-boot 的action
（21） 執行所有触发标识为boot 的action
（22）基于當前property 狀態，執行所有触发标识为property 的action
（23）注冊輪詢事件:
- device_fd
- property_set_fd
-signal_recv_fd
-如果有keychord，則注冊keychord_fd
（24）如果支持BOOTCHART,則初始化BOOTCHART
（25）進入主進程循環:
- 重置輪詢事件的接受狀態，revents 為0
- 查詢action 隊列，并执行。
- 重啟需要重啟的服务
- 輪詢注冊的事件
- 如果signal_recv_fd 的revents 為POLLIN，則得到一個信號，獲取并處
理
- 如果device_fd 的revents 為POLLIN,調用handle_device_fd
- 如果property_fd 的revents 為POLLIN,調用handle_property_set_fd
- 如果keychord_fd 的revents 為POLLIN,調用handle_keychord
到了这里，整个android 文件系统已经起来了。
初始化核心的核心init.rc文件分析
在上面红色那一行（5）解析/init.rc，将所有服务和操作信息加入链表。
 parse_config_file("/init.rc");//在init.c 中代码 （有关 /init.rc的脚本我就不贴出来了）
名词解释：
 Android 初始化語言由四大类声明组成：行为类(Actions)、命令类(Commands)、服务类(Services)、选项类(Options)。
 初始化语言以行为单位，由以空格间隔的语言符号組成。C 风格的反斜杠转义符可以用来插入空白到语言符号。双引号也可以用来防止文本被空格分成多个语言符号。当反斜杠在行末时，作为换行符。
 * 以#开始(前面允许空格)的行为注释。
 * Actions 和Services 隐含声明一个新的段落。所有该段落下Commands 或 Options 的声明属于该段落。第一段落前的Commands 或Options 被忽略。
 * Actions 和Services 拥有唯一的命名。在他们之后声明相同命名的类将被当作错误并忽略。
 Actions 是一系列命令的命名。Actions 拥有一个触发器(trigger)用来決定action 何時执行。当一个action 在符合触发条件被执行时，如果它还没被加入到待执行队列中的话，則加入到队列最后。队列中的action 依次执行，action 中的命令也依次执行。
 Init 在执行命令的中间处理其他活动(设备创建/销毁,property 设置，进程重启)。
 Actions 的表现形式:
 on <trigger>
 <command>
 <command>
 <command>
 重要的数据结构两个列表，一个队列。
static list_declare(service_list)；
static list_declare(action_list)；
static list_declare(action_queue)；
 *.rc 脚本中所有 service 关键字定义的服务将会添加到 service_list 列表中。
 *.rc 脚本中所有 on 关键开头的项将会被会添加到 action_list 列表中。每个action 列表项都有一个列表，此列表用来保存该段落下的 Commands。
脚本解析过程：
parse_config_file("/init.rc")
int parse_config_file(const char *fn)
{
 char *data;
 data = read_file(fn, 0);
 if (!data)  return -1;
 parse_config(fn, data);
 DUMP();
 return 0;
}
static void parse_config(const char *fn, char *s)  {
 ...
 case T_NEWLINE:
 if (nargs) {
 int kw = lookup_keyword(args[0]);
 if (kw_is(kw, SECTION)) {
 state.parse_line(&state, 0, 0);
 parse_new_section(&state, kw, nargs, args);
 } else {
 state.parse_line(&state, nargs, args);
 }
 nargs = 0;
 }
...
｝
parse_config 会逐行对脚本进行解析，如果关键字类型为 SECTION ，那么将会执行parse_new_section();
类型为 SECTION 的关键字有： on 和 sevice
关键字类型定义在 Parser.c (system\core\init) 文件中
Parser.c (system\core\init)
#define SECTION 0x01
#define COMMAND 0x02
#define OPTION 0x04
关键字  属性
capability,  OPTION, 0, 0)
class,  OPTION, 0, 0)
class_start,  COMMAND, 1, do_class_start)
class_stop,  COMMAND, 1, do_class_stop)
console,  OPTION, 0, 0)
critical,  OPTION, 0, 0)
disabled,  OPTION, 0, 0)
domainname,  COMMAND, 1, do_domainname)
exec,  COMMAND, 1, do_exec)
export,  COMMAND, 2, do_export)
group,  OPTION, 0, 0)
hostname,  COMMAND, 1, do_hostname)
ifup,  COMMAND, 1, do_ifup)
insmod,  COMMAND, 1, do_insmod)
import,  COMMAND, 1, do_import)
keycodes,  OPTION, 0, 0)
mkdir,  COMMAND, 1, do_mkdir)
mount,  COMMAND, 3, do_mount)
on,  SECTION, 0, 0)
oneshot,  OPTION, 0, 0)
onrestart,  OPTION, 0, 0)
restart,  COMMAND, 1, do_restart)
service,  SECTION, 0, 0)
setenv,  OPTION, 2, 0)
setkey,  COMMAND, 0, do_setkey)
setprop,  COMMAND, 2, do_setprop)
setrlimit,  COMMAND, 3, do_setrlimit)
socket,  OPTION, 0, 0)
start,  COMMAND, 1, do_start)
stop,  COMMAND, 1, do_stop)
trigger,  COMMAND, 1, do_trigger)
symlink,  COMMAND, 1, do_symlink)
sysclktz,  COMMAND, 1, do_sysclktz)
user,  OPTION, 0, 0)
write,  COMMAND, 2, do_write)
chown,  COMMAND, 2, do_chown)
chmod,  COMMAND, 2, do_chmod)
loglevel,  COMMAND, 1, do_loglevel)
device,  COMMAND, 4, do_device)
parse_new_section()中再分别对 service 或者 on 关键字开头的内容进行解
析。
...
case K_service:
 state->context = parse_service(state, nargs, args);
 if (state->context) {
 state->parse_line = parse_line_service;
 return;
 }
 break;
case K_on:
 state->context = parse_action(state, nargs, args);
 if (state->context) {
 state->parse_line = parse_line_action;
 return;
 }
 break;
...
对 on 关键字开头的内容进行解析
static void *parse_action(struct parse_state *state, int nargs, char **args)
{
 ...
 act = calloc(1, sizeof(*act));
 act->name = args[1];
 list_init(&act->commands);
 list_add_tail(&action_list, &act->alist);
 ...
}
对 service 关键字开头的内容进行解析
static void *parse_service(struct parse_state *state, int nargs, char **args)
{
 struct service *svc;
 if (nargs < 3) {
 parse_error(state, "services must have a name and a program\n");
 return 0;
 }
 if (!valid_name(args[1])) {
 parse_error(state, "invalid service name '%s'\n", args[1]);
 return 0;
 }
//如果服务已经存在service_list 列表中将会被忽略
 svc = service_find_by_name(args[1]);
 if (svc) {
 parse_error(state, "ignored duplicate definition of service '%s'\n", args[1]);
 return 0;
 }
 nargs -= 2;
 svc = calloc(1, sizeof(*svc) + sizeof(char*) * nargs);
 if (!svc) {
 parse_error(state, "out of memory\n");
 return 0;
 }
 svc->name = args[1];
 svc->classname = "default";
 memcpy(svc->args, args + 2, sizeof(char*) * nargs);
 svc->args[nargs] = 0;
 svc->nargs = nargs;
 svc->onrestart.name = "onrestart";
 list_init(&svc->onrestart.commands);
//添加该服务到 service_list 列表
 list_add_tail(&service_list, &svc->slist);
 return svc;
}
服务的表现形式:
service <name> <pathname> [ <argument> ]*
<option>
<option>
...
 申请一个service 结构体，然后挂接到service_list 链表上，name 为服务的名称，pathname 为执行的命令，argument为命令的参数。之后的 option 用来控制这个service 结构体的属性，parse_line_service 会对 service 关键字后的内容进行解析并填充到 service 结构中，当遇到下一个service 或者on 关键字的时候此service 选项解析结束。
例如：
service zygote /system/bin/app_process -Xzygote /system/bin --zygote --start-system-server
 socket zygote stream 666
 onrestart write /sys/android_power/request_state wake
服务名称为： zygote
启动该服务执行的命令： /system/bin/app_process
命令的参数： -Xzygote /system/bin --zygote --start-system-server
socket zygote stream 666： 创建一个名为：/dev/socket/zygote 的 socket ，
类型为：stream
转自：[http://blog.sina.com.cn/s/blog_68bc1cab0100j7mv.html](http://blog.sina.com.cn/s/blog_68bc1cab0100j7mv.html)
