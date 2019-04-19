# Kconfig文件结构(图文)简介 - maopig的专栏 - CSDN博客
2013年06月16日 16:28:17[maopig](https://me.csdn.net/maopig)阅读数：1431
个人分类：[u-boot/驱动开发/linux																[Linux](https://blog.csdn.net/maopig/article/category/850267)](https://blog.csdn.net/maopig/article/category/930963)

1 Kconfig和Makefile
毫不夸张地说，Kconfig和Makefile是我们浏览内核代码时最为依仗的两个文件。基本上，Linux 内核中每一个目录下边都会有一个Kconfig文件和一个Makefile文件。Kconfig和Makefile就好似一个城市的地图，地图引导我们去 认识一个城市，而Kconfig和Makefile则可以让我们了解一个内核目录下面的结构。在希望研究内核的某个子系统、某个驱动或其他某个部分时，都 有必要首先仔细阅读一下相关目录下的Kconfig和Makefile文件。
分布到各目录的Kconfig构成了一个分布式的内核配置数据库，每个Kconfig分别描述了所属目录源文档相关的内核配置菜单。在内核配置make menuconfig时，从Kconfig中读出菜单，用户选择后保存到.config的内核配置文档中。 在内核编译时，主Makefile调用这个.config，就知道了用户的选择。
假如想使这个驱动被编译，则要修改Makefile文件，因此，需要添加新的驱动时，需要修改的文件有两个:Kconfig,Makefile.
2 菜单组织结构
一般一个Kconfig文件表示的就是一个菜单，一个菜单由多个菜单项组成,其格式如下:
menu 菜单名  
菜单项或菜单链接1  
菜单项或菜单链接2  
...  
菜单项或菜单链接n  
endmenu  
*复制代码*
其中菜单项就是指菜单的子菜单，所谓菜单链接就是指链接到另一个Kconfig文件，如此一下，菜单就可以实现随意嵌套了.
例如:
# drivers/Kconfig  
menu "Device Drivers"  
source "drivers/base/Kconfig"  
source "drivers/connector/Kconfig"  
source "drivers/mtd/Kconfig"  
source "drivers/of/Kconfig"  
source "drivers/parport/Kconfig"  
source "drivers/pnp/Kconfig"  
source "drivers/block/Kconfig"  
source "drivers/hello/Kconfig"  
config test  
  bool "提示字符串"  
  default y  
  ...  
endmenu   
*复制代码*
2.1 菜单项
语法:
config <symbol>  
<config options>  
*复制代码*
<symbol>为一符号，就好像代码中的局部变量x一样,可用于后边的表达式中.
例如:
config UEVENT_HELPER_PATH  
       string "path to uevent helper"  
       depends on HOTPLUG  
       default "/sbin/hotplug"  
       help  
      Path to uevent helper program forked by the kernel for  
      every uevent.  
*复制代码*
上面菜单项的属性string表示菜单的类型，每一个菜单项必须有一个类型.
注:每个config菜单项都会产生一个配置选项CONFIG_XXX, XXX即为<symbol>. 如上，则会产生一个配置项:CONFIG_UEVENT_HELPER_PATH,此配置项的值记录在内核根目录下的隐藏文件.config内， 例:~/WORKING_DIRECTORY/kernel/goldfish/.config文件内.
**2.2 菜单链接**
菜单链接的格式如下:
source "路径"  
*复制代码*
如:
source "drivers/pnp/Kconfig"  
*复制代码*
2.3 菜单属性2.3.1 类型
类型可以是:bool、tristate、string、hex和int。
bool类型的只能选中或不选中，选中为y,不选中为n.
tristate类型的菜单项为值可为三种值，多了编译成内核模块的选项。其值可为y,n,m.
string类型表示需要用户输入一串字符串。
hex类型则需要用户输入一个16进制数。
int类型表示用户输入一个整型.
总结:
菜单类型属性就好比一个控件，bool相当于单选框，trstate相当于有三种状态的复选框，string相当于供用户输入字符串的文本编辑框，hex相当于供用户输入16进制数的文本编辑框，而int就相当于供用户输入整型数的文本编辑框。
类型关键字后边可跟随提示字符，也可以不跟随,取决于情况。如：
string "path to uevent helper"  
*复制代码*
bool "Prevent firmware from being built"
*复制代码*
注:每一个菜单项必须有类型属性。
**2.3.2 默认值**
默认值属性default一般在类型属性后边，如:
config UEVENT_HELPER_PATH  
    string "path to uevent helper"  
    default "/sbin/hotplug"  
*复制代码*
表示当前菜单项若用户没有选择或输入任何值时，所取的默认值.上述所示为当前的默认值为"/sbin/totplug".
**2.3.3 依赖**
依赖可以是"depends on"或"requires".
语法:
depends on/requires <expr>
*复制代码*
<expre>为表达式，可为之前定义的菜单项名.
如:
depends on HOTPLUG  
*复制代码*
**表示此菜单项显示与否取决于另外一个菜单项HOTPLUG ，只有当菜单项HOTPLUG这个菜单项有效显示，当前菜单项才会显示。**
例如:
config MODULES    
    bool "Enable loadable module support"    
config MODVERSIONS    
    bool "Set version information on all module symbols"    
    depends on MODULES    
    comment "module support disabled"    
    depends on !MODULES   
*复制代码*
菜单项MODVERSIONS的显示与否取决于菜单项MODULES。这种信赖关系常用在子菜单项中。
**2.3.4 选择**
语法:
choice  
选择项  
..  
endchoice
*复制代码*
2.3.5 提示
语法:
comment "提示信息字符串"  
comment选项  
*复制代码*
comment只是用来给用户提示信息的，后跟字符串，此字符串也可以在终端中显示。
comment选项只可以是deponds on。
2.3.6 帮助
语法:
help/---help--- <字符串>  
*复制代码*
例如:
config EXTRA_FIRMWARE_DIR  
    string "Firmware blobs root directory"  
    depends on EXTRA_FIRMWARE != ""  
    default "firmware"  
    help  
      This option controls the directory in which the kernel build system  
      looks for the firmware files listed in the EXTRA_FIRMWARE option.  
      The default is the firmware/ directory in the kernel source tree,  
      but by changing this option you can point it elsewhere, such as  
      the /lib/firmware/ directory or another separate directory  
      containing firmware files.  
*复制代码*
help相当于注释一样，在给编辑Kconfig文件的人看的，这样可以保持其可读性.
3 举例
Kconfig:
# drivers/Kconfig  
menu "Device Drivers"  
source "drivers/base/Kconfig"  
source "drivers/connector/Kconfig"  
source "drivers/mtd/Kconfig"  
...  
endmenu  
*复制代码*
其对应的make menuconfig界面如下图所示:![2.gif](http://b.hiphotos.baidu.com/space/pic/item/bd3eb13533fa828b477d6747fd1f4134970a5a3c.jpg)
source "drivers/base/Kconfig"中的Kconfig内容如下:即对应着上图中的第一项"Generic Driver Option"的子菜单内容:
这里不再列出来！
显示效果如下图所示:
![3.gif](http://e.hiphotos.baidu.com/space/pic/item/bf096b63f6246b6060dc5bc9ebf81a4c500fa2b9.jpg)
各个目录下的Kconfig文件经过最终配置，最终会在内核根目录下生成一个.config文件，这是个隐藏文件，这个文件记录着各个选项的配置及值。供Makefile文件使用. 使用命令：ls -al可看到！
如:
.config:
#  
# Automatically generated make config: don't edit  
# Linux kernel version: 2.6.29  
# Thu Dec 15 21:15:25 2011  
#  
CONFIG_ARM=y  
CONFIG_SYS_SUPPORTS_APM_EMULATION=y  
# CONFIG_GENERIC_GPIO is not set  
CONFIG_GENERIC_TIME=y  
CONFIG_GENERIC_CLOCKEVENTS=y  
CONFIG_MMU=y  
# CONFIG_NO_IOPORT is not set  
CONFIG_GENERIC_HARDIRQS=y  
CONFIG_STACKTRACE_SUPPORT=y  
CONFIG_HAVE_LATENCYTOP_SUPPORT=y  
CONFIG_LOCKDEP_SUPPORT=y  
CONFIG_TRACE_IRQFLAGS_SUPPORT=y  
CONFIG_HARDIRQS_SW_RESEND=y  
CONFIG_GENERIC_IRQ_PROBE=y  
CONFIG_RWSEM_GENERIC_SPINLOCK=y  
# CONFIG_ARCH_HAS_ILOG2_U32 is not set  
# CONFIG_ARCH_HAS_ILOG2_U64 is not set  
CONFIG_GENERIC_HWEIGHT=y  
CONFIG_GENERIC_CALIBRATE_DELAY=y  
CONFIG_GENERIC_HARDIRQS_NO__DO_IRQ=y  
CONFIG_VECTORS_BASE=0xffff0000  
CONFIG_DEFCONFIG_LIST="/lib/modules/$UNAME_RELEASE/.config"  
*复制代码*
每一个CONFIG_xxx记录着之前Kconfig文件内的菜单项的值.
