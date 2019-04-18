# printk解析 - TaigaComplex求职中 - 博客园







# [printk解析](https://www.cnblogs.com/TaigaCon/archive/2012/12/29/2839334.html)





以下源码基于Linux-2.6.32.2

printk是Linux内核空间的信息输出函数，printk入口位于kernel/Printk.c

以下是printk的流程解析：

```
printk
    vprintk
        /* Emit the output into the temporary buffer */
         * vsnprintf把printk的参数进行解析分类，然后用一个暂时缓冲区接受解析后的数据
         */
        vscnprintf(printk_buf + printed_len,sizeof(printk_buf) - printed_len, fmt, args);
        p = printk_buf;

        /* Do we have a loglevel in the string? 
         *提取printk信息级别：<c>     c就是级别
         */

        /*
         * Copy the output into log_buf.  If the caller didn't provide
         * appropriate log level tags, we insert them here
         * 把printk的输出信息放到全局缓冲区log_buf里面，
         *     static char *log_buf = __log_buf;
         */
        emit_log_char('<');
        emit_log_char(current_log_level + '0');
        emit_log_char('>');
        emit_log_char(*p);

        release_console_sem();
            call_console_drivers(_con_start, _log_end);
                _call_console_drivers(start_print, cur_index, msg_level);
                    /*如果printk的输出信息级别 < 控制台的信息级别，则输出*/
                    if ((msg_log_level < console_loglevel || ignore_loglevel) &&console_drivers && start != end)
                    __call_console_drivers(start, end);
                        /*对每个控制台调用写函数，输出printk信息*/
                        for_each_console(con)
                        con->write(con, &LOG_BUF(start), end - start);
```

以上流程可以知道，printk最终会调用到控制台的写函数，用于输出数据
另外，在设置bootargs的时候，设置了console=ttySAC0，也就是设置控制台为ttySAC0,通过下面的解析，得出name="ttySAC"等，并把数据放到console_cmdline数组内

```
static int __init console_setup(char *str)
    strncpy(buf, str, sizeof(buf) - 1);
    for (s = buf; *s; s++)
        if ((*s >= '0' && *s <= '9') || *s == ',')
            break;
    __add_preferred_console(buf, idx, options, brl_options);
        c = &console_cmdline[i];
        strlcpy(c->name, name, sizeof(c->name));
        c->options = options;
        c->index = idx;

__setup("console=", console_setup);    //当解析到console= 这个字符串的时候，调用console_setup函数进行处理
```



可以想象的是，这时候我们设置的是控制台的名字，当我们需要设置控制台的操作函数时，通过比对该名字，如果匹配名字则把操作函数设置为底层的操作函数，
在内核中搜索ttySAC,得到一个匹配项，位于drivers/serial/Samsung.c

```
#define S3C24XX_SERIAL_NAME    "ttySAC"

static struct console s3c24xx_serial_console = {
    .name        = S3C24XX_SERIAL_NAME,        //"ttySAC"
    .device        = uart_console_device,        //init进行，用户程序打开/dev/console时用到
    .flags        = CON_PRINTBUFFER,        //注册完成该结构体后，打印先前在log_buf中保存的信息，其实前面的数据都是还没能输出的，只是放在log_buf内
    .index        = -1,                //表示使用哪个串口命令由命令行参数决定
    .write        = s3c24xx_serial_console_write,    //串口控制台输出函数
    .setup        = s3c24xx_serial_console_setup    //串口控制台设置函数
};
```



以上就是控制台的各个属性跟操作函数，注册函数为
s3c24xx_serial_initconsole(struct platform_driver *drv,struct s3c24xx_uart_info *info)
	　　register_console(&s3c24xx_serial_console);





那么上面的函数是如何被调用的呢？
我们搜索s3c24xx_serial_initconsole，发现下面的宏定义：



```
#define s3c24xx_console_init(__drv, __inf)            \
static int __init s3c_serial_console_init(void)            \
{                                \
    return s3c24xx_serial_initconsole(__drv, __inf);    \
}                                \
                                \
console_initcall(s3c_serial_console_init)
```

在drivers/serial/S3c2440.c内有这么一行：

```
s3c24xx_console_init(&s3c2440_serial_driver, &s3c2440_uart_inf);
```



在include/linux/Init.h里面有这么一个宏定义（其中里面的initcall_t 为一个返回值为int的函数指针，__initcall_##fn中的##是一个连接符号）

```
#define console_initcall(fn) \
    static initcall_t __initcall_##fn \
    __used __section(.con_initcall.init) = fn
```



**也就是上面的宏会变成：**

```
static int __init s3c_serial_console_init(void)
{
    return s3c24xx_serial_initconsole(&s3c2440_serial_driver, &s3c2440_uart_inf);
}

static int __initcall_s3c_serial_console_init() __used __section(.con_initcall.init) = s3c_serial_console_init;
```

也就是在.con_initcall.init段内定义了一个函数，该函数指针指向s3c_serial_console_init，最终调用到s3c24xx_serial_initconsole，从而注册控制台



在linux源码搜索.con_initcall.init，得到如下信息

```
__con_initcall_start = .;
             *(.con_initcall.init)
         __con_initcall_end = .;
```

在arch/arm/kernel/vmlinux.lsd.S里面有定义.con_initcall.init 段，段首为__con_initcall_start，段尾为__con_initcall_end



这里我们可以猜想，linux在初始化的时候，调用该段内的所有函数，达到初始化控制台的目的

在linux源码搜索__con_initcall_start，
在driver/char/Tty_io.c内得到了如下结果

```
call = __con_initcall_start;
        while (call < __con_initcall_end) {
        (*call)();
        call++;
        }
```

印证了的上述猜想。



然后向上搜索，发现该函数是在console_init内的，而console是在start_kernel内调用的。因此可以得到控制台初始化的整个轮廓：

```
start_kernel(void)        //main.c
    console_init(void)    //driver/char/Tty_io.c
        call = __con_initcall_start;
        while (call < __con_initcall_end) {
        (*call)();
        call++;
        }
            /*然后就调用到了里面的函数
             *实际上是调用s3c_serial_console_init
             */
            __initcall_s3c_serial_console_init()    
                s3c24xx_serial_initconsole(&s3c2440_serial_driver, &s3c2440_uart_inf)
                    register_console(&s3c24xx_serial_console);    
                    /*该函数位于kernel/Printk.c内
                     *在该函数内比对我们输入的console（console_cmdline数组）跟所初始化的console
                     */
```














