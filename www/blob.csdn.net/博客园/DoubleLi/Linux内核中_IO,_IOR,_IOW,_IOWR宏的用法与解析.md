# Linux内核中_IO,_IOR,_IOW,_IOWR宏的用法与解析 - DoubleLi - 博客园






在驱动程序里， ioctl() 函数上传送的变量 cmd 是应用程序用于区别设备驱动程序请求处理内容的值。cmd除了可区别数字外，还包含有助于处理的几种相应信息。 cmd的大小为 32位，共分 4 个域：
     bit31~bit30 2位为 “区别读写” 区，作用是区分是读取命令还是写入命令。
     bit29~bit15 14位为 "数据大小" 区，表示 ioctl() 中的 arg 变量传送的内存大小。
     bit20~bit08  8位为 “魔数"(也称为"幻数")区，这个值用以与其它设备驱动程序的 ioctl 命令进行区别。
     bit07~bit00   8位为 "区别序号" 区，是区分命令的命令顺序序号。
像 命令码中的 “区分读写区” 里的值可能是 _IOC_NONE （0值）表示无数据传输，_IOC_READ (读)，_IOC_WRITE (写) ， _IOC_READ|_IOC_WRITE (双向)。
内核定义了 _IO() , _IOR() , IOW() 和 _IOWR() 这 4 个宏来辅助生成上面的 cmd 。下面分析 _IO() 的实现，其它的类似：

**在 asm-generic/ioctl.h 里可以看到 _IO() 的定义**：

      #define _IO(type,nr)        _IOC(_IOC_NONE,(type),(nr),0)

**再看 _IOC() 的定义**：

     #define _IOC(dir,type,nr,size) \
                   (((dir)  << _IOC_DIRSHIFT) | \
                   ((type) << _IOC_TYPESHIFT) | \
                   ((nr)   << _IOC_NRSHIFT) | \
                   ((size) << _IOC_SIZESHIFT))

可见，_IO() 的最后结果由 _IOC() 中的 4 个参数移位组合而成。
**再看 _IOC_DIRSHIT 的定义**：

            #define _IOC_DIRSHIFT    (_IOC_SIZESHIFT+_IOC_SIZEBITS)

**      _IOC_SIZESHIFT 的定义**：

           #define _IOC_SIZESHIFT    (_IOC_TYPESHIFT+_IOC_TYPEBITS)

**      _IOC_TYPESHIF 的定义**：

           #define _IOC_TYPESHIFT    (_IOC_NRSHIFT+_IOC_NRBITS)

**      _IOC_NRSHIFT 的定义**：

           #define _IOC_NRSHIFT    0

**      _IOC_NRBITS 的定义**：

          #define _IOC_NRBITS    8

**      _IOC_TYPEBITS 的定义**：

         #define _IOC_TYPEBITS    8

**由上面的定义，往上推得到**：

      引 用

       _IOC_TYPESHIFT = 8

       _IOC_SIZESHIFT = 16

       _IOC_DIRSHIFT = 30

所以，(dir)  << _IOC_DIRSHIFT) 表是 dir 往左移 30 位，即移到 bit31~bit30 两位上，得到方向(读写)的属性；
       (size) << _IOC_SIZESHIFT) 位左移 16 位得到“数据大小”区；
       (type) << _IOC_TYPESHIFT) 左 移 8位得到"魔数区" ；
       (nr)   << _IOC_NRSHIFT)      左移 0 位( bit7~bit0) 。
这样，就得到了 _IO() 的宏值。

**这几个宏的使用格式为**：
- _IO (魔数， 基数);
- _IOR (魔数， 基数， 变量型)
- _IOW  (魔数， 基数， 变量型)
- _IOWR (魔数， 基数，变量型 )

**魔数 (magic number)**
      魔数范围为 0~255 。通常，用英文字符 "A" ~ "Z" 或者 "a" ~ "z" 来表示。设备驱动程序从传递进来的命令获取魔数，然后与自身处理的魔数想比较，如果相同则处理，不同则不处理。魔数是拒绝误使用的初步辅助状态。设备驱动 程序可以通过 _IOC_TYPE (cmd) 来获取魔数。不同的设备驱动程序最好设置不同的魔数，但并不是要求绝对，也是可以使用其他设备驱动程序已用过的魔数。
**基(序列号)数**
      基数用于区别各种命令。通常，从 0开始递增，相同设备驱动程序上可以重复使用该值。例如，读取和写入命令中使用了相同的基数，设备驱动程序也能分辨出来，原因在于设备驱动程序区分命令时 使用 switch ，且直接使用命令变量 cmd值。创建命令的宏生成的值由多个域组合而成，所以即使是相同的基数，也会判断为不同的命令。设备驱动程序想要从命令中获取该基数，就使用下面的宏：
_IOC_NR (cmd)
通常，switch 中的 case 值使用的是命令的本身。
**变量型**
      变量型使用 arg 变量指定传送的数据大小，但是不直接代入输入，而是代入变量或者是变量的类型，原因是在使用宏创建命令，已经包含了 sizeof() 编译命令。比如 _IOR() 宏的定义是：

      引用

      #define _IOR(type,nr,size)    _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))

而 _IOC_TYPECHECK() 的定义正是：

      引用

      #define _IOC_TYPECHECK(t) (sizeof(t))

设备驱动程序想要从传送的命令获取相应的值，就要使用下列宏函数：
      _IOC_SIZE(cmd)

**_IO 宏**

      该宏函数没有可传送的变量，只是用于传送命令。例如如下约定：

      引用

      #define TEST_DRV_RESET _IO ('Q', 0)

此时，省略由应用程序传送的 arg 变量或者代入 0 。在应用程序中使用该宏时，比如：

      ioctl (dev, TEST_DEV_RESET, 0)   或者  ioctl (dev, TEST_DRV_RESET) 。
这是因为变量的有效因素是可变因素。只作为命令使用时，没有必要判 断出设备上数据的输出或输入。因此，设备驱动程序没有必要执行设备文件大开选项的相关处理。

**_IOR 宏**
     该函数用 于创建从设备读取数据的命令，例如可如下约定：

     引用

     #define TEST_DEV_READ  _IRQ('Q', 1, int)

这说明应用程序从设备读取数据的大小为 int 。下面宏用于判断传送到设备驱动程序的 cmd 命令的读写状态：
     _IOC_DIR (cmd)
运行该宏时，返回值的类型 如下：
- _IOC_NONE                             :  无属性
- _IOC_READ                             :  可读属性
- _IOC_WRITE                           : 可写属性
- _IOC_READ | _IOC_WRITE : 可读，可写属性

使用该命令时，应用程序的 ioctl() 的 arg 变量值指定设备驱动程序上读取数据时的缓存(结构体)地址。
**_IOW ****宏**
      用于创建设 备上写入数据的命令，其余内容与 _IOR 相同。通常，使用该命令时，ioctl() 的 arg 变量值指定设备驱动程序上写入数据时的缓存(结构体)地址。
**_IOWR ****宏**
      用于创建设备上读写数据的命令。其余内 容与 _IOR 相同。通常，使用该命令时，ioctl() 的 arg 变量值指定设备驱动程序上写入或读取数据时的缓存 (结构体) 地址。
**_IOR() , _IOW(), IORW() ****的定义**：
      #define _IOR(type,nr,size)    _IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
      #define _IOW(type,nr,size)    _IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
      #define _IOWR(type,nr,size)    _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))









