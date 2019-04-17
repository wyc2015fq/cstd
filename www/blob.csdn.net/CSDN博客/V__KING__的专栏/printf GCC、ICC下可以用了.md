# printf GCC、ICC下可以用了 - V__KING__的专栏 - CSDN博客





2014年04月09日 18:33:14[v__king__](https://me.csdn.net/V__KING__)阅读数：1482









http://jesserei.blog.163.com/blog/static/12141168920096227471677/

http://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gab599ddf60819df4cc993c724a83cb1a4




1,GCC默认情况下是不可以用的

加上这句话fdevopen(putchar,0); 

其中putchar是你定义的串口通信程序  

2。ICC6.13后默认都是不可以用的哦出现错误提示

E:\PROGRA~1\ICC-AV~2\bin\imakew -f CH375.mak

    iccavr -o CH375 -g -e:0x20000 -ucrtatmega.o -bfunc_lit:0x8c.0x20000 -dram_end:0x10ff -bdata:0x100.0x10ff -dhwstk_size:30 -beeprom:0.4096 -fihx_coff -S2 @CH375.lk  E:\avrdesign\ch375\CH375HFB.A   -lcatmega

!ERROR Function 'putchar' not defined. This is a hardware dependent function,

  and it is not included in the ICCAVR default library. Please implement

  this function to match your target requirements. You can find examples

  of this function under the <install root>\examples.AVR directory. Make

  a copy of a suitable example, add it to your project and modify it if

  needed. The source code must #include the appropriate C header file

  (e.g. stdio.h) as it contains the special compiler pragma for 'putchar'. 

E:\PROGRA~1\ICC-AV~2\bin\imakew.exe: Error code 1

Done: there are error(s). Exit code: 1. Wed May 20 09:50:54 2009

原因是没有putchar ，printf默认是要调用putchar的哦，所以你自己定义一个就行了

#include <stdio.h>

static int uart_putchar(char c, FILE *stream);

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,

_FDEV_SETUP_WRITE);

static int

uart_putchar(char c, FILE *stream)

{

if (c == ’\n’)

uart_putchar(’\r’, stream);

loop_until_bit_is_set(UCSRA, UDRE);

UDR = c;

return 0;

}

int

main(void)

{

init_uart();

stdout = &mystdout;

printf("Hello, world!\n");

return 0;

}

在默认情况下，fdevopen 以及浮点数转换的printf和scanf 都需要动态内存分配（malloc）,

这将连接动态内存分配模块，导致代码膨胀，如果系统中不使用浮点数转换，通过使用上面

的方法连接设备可以避免连接动态内存分配模块，这对于资源有限的单片机系统是难得的。

替代 fdevopen 函数的是宏FDEV_SETUP_STREAM ，它将初始化一个标准的i/o 对象（上

例中为mystdout），由于这个对象是用户定义的变量，固然无需动态分配内存。FILE 是标准

I/O 流使用的数据结构，stdin,stdout,stderr 均为指向FILE 结构的指针。正如上面的main 函

数中那样标准流对象stdout 指向用户定义的流对象数据结构后printf 可以正常使用了。此时

不能用fclose 关闭，如果需要关闭的话使用fdev_close。

关于宏 FDEV_SETUP_STREAM 和相关参数说明请参考avr-libc 用户手册。

1. 使用 WinAVR20060421 之前版本编译。

2. 包含 stdio.h 文件之前定义宏 __STDIO_FDEVOPEN_COMPAT_12

如：

#define __STDIO_FDEVOPEN_COMPAT_12

#include <stdio.h>

。。。。。。。。。。。。。。

这一种方法与前一种方法效果是一样的。意思是该宏的意思是兼容 avr-libc1.2 版本。

3．修改代码使用新的方法连接设备.


第一步：在全局声明 FILE 变量并用宏FDEV_SETUP_STREAM 初始化

static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,_FDEV_SETUP_WRITE);

第二步：改变 uart_putchar 函数参数列表

int uart_putchar(char c, FILE *stream)

第三步：用 stdout = &mystdout;替换fdevopen 的调用。

第三种方法（也是建议在新程序中使用的）它生成的代码量会比前两种方法生成的会更

小。










/** \file */

/** \defgroup avr_stdio <stdio.h>: Standard IO facilities

    \code #include <stdio.h> \endcode


    <h3>Introduction to the Standard IO facilities</h3>


    This file declares the standard IO facilities that are implemented

    in \c avr-libc.  Due to the nature of the underlying hardware,

    only a limited subset of standard IO is implemented.  There is no

    actual file implementation available, so only device IO can be

    performed.  Since there's no operating system, the application

    needs to provide enough details about their devices in order to

    make them usable by the standard IO facilities.


    Due to space constraints, some functionality has not been

    implemented at all (like some of the \c printf conversions that

    have been left out).  Nevertheless, potential users of this

    implementation should be warned: the \c printf and \c scanf families of functions, although

    usually associated with presumably simple things like the

    famous "Hello, world!" program, are actually fairly complex

    which causes their inclusion to eat up a fair amount of code space.

    Also, they are not fast due to the nature of interpreting the

    format string at run-time.  Whenever possible, resorting to the

    (sometimes non-standard) predetermined conversion facilities that are

    offered by avr-libc will usually cost much less in terms of speed

    and code size.


    <h3>Tunable options for code size vs. feature set</h3>


    In order to allow programmers a code size vs. functionality tradeoff,

    the function vfprintf() which is the heart of the printf family can be

    selected in different flavours using linker options.  See the

    documentation of vfprintf() for a detailed description.  The same

    applies to vfscanf() and the \c scanf family of functions.


    <h3>Outline of the chosen API</h3>


    The standard streams \c stdin, \c stdout, and \c stderr are

    provided, but contrary to the C standard, since avr-libc has no

    knowledge about applicable devices, these streams are not already

    pre-initialized at application startup.  Also, since there is no

    notion of "file" whatsoever to avr-libc, there is no function

    \c fopen() that could be used to associate a stream to some device.

    (See \ref stdio_note1 "note 1".)  Instead, the function \c fdevopen()

    is provided to associate a stream to a device, where the device

    needs to provide a function to send a character, to receive a

    character, or both.  There is no differentiation between "text" and

    "binary" streams inside avr-libc.  Character \c \\n is sent

    literally down to the device's \c put() function.  If the device

    requires a carriage return (\c \\r) character to be sent before

    the linefeed, its \c put() routine must implement this (see

    \ref stdio_note2 "note 2").


    As an alternative method to fdevopen(), the macro

    fdev_setup_stream() might be used to setup a user-supplied FILE

    structure.


    It should be noted that the automatic conversion of a newline

    character into a carriage return - newline sequence breaks binary

    transfers.  If binary transfers are desired, no automatic

    conversion should be performed, but instead any string that aims

    to issue a CR-LF sequence must use <tt>"\r\n"</tt> explicitly.


    For convenience, the first call to \c fdevopen() that opens a

    stream for reading will cause the resulting stream to be aliased

    to \c stdin.  Likewise, the first call to \c fdevopen() that opens

    a stream for writing will cause the resulting stream to be aliased

    to both, \c stdout, and \c stderr.  Thus, if the open was done

    with both, read and write intent, all three standard streams will

    be identical.  Note that these aliases are indistinguishable from

    each other, thus calling \c fclose() on such a stream will also

    effectively close all of its aliases (\ref stdio_note3 "note 3").


    It is possible to tie additional user data to a stream, using

    fdev_set_udata().  The backend put and get functions can then

    extract this user data using fdev_get_udata(), and act

    appropriately.  For example, a single put function could be used

    to talk to two different UARTs that way, or the put and get

    functions could keep internal state between calls there.


    <h3>Format strings in flash ROM</h3>


    All the \c printf and \c scanf family functions come in two flavours: the

    standard name, where the format string is expected to be in

    SRAM, as well as a version with the suffix "_P" where the format

    string is expected to reside in the flash ROM.  The macro

    \c PSTR (explained in \ref avr_pgmspace) becomes very handy

    for declaring these format strings.


    \anchor stdio_without_malloc

    <h3>Running stdio without malloc()</h3>


    By default, fdevopen() requires malloc().  As this is often

    not desired in the limited environment of a microcontroller, an

    alternative option is provided to run completely without malloc().


    The macro fdev_setup_stream() is provided to prepare a

    user-supplied FILE buffer for operation with stdio.


    <h4>Example</h4>


    \code

    #include <stdio.h>


    static int uart_putchar(char c, FILE *stream);


    static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,

                                             _FDEV_SETUP_WRITE);


    static int

    uart_putchar(char c, FILE *stream)

    {


      if (c == '\n')

        uart_putchar('\r', stream);

      loop_until_bit_is_set(UCSRA, UDRE);

      UDR = c;

      return 0;

    }


    int

    main(void)

    {

      init_uart();

      stdout = &mystdout;

      printf("Hello, world!\n");


      return 0;

    }

    \endcode


    This example uses the initializer form FDEV_SETUP_STREAM() rather

    than the function-like fdev_setup_stream(), so all data

    initialization happens during C start-up.


    If streams initialized that way are no longer needed, they can be

    destroyed by first calling the macro fdev_close(), and then

    destroying the object itself.  No call to fclose() should be

    issued for these streams.  While calling fclose() itself is

    harmless, it will cause an undefined reference to free() and thus

    cause the linker to link the malloc module into the application.


    <h3>Notes</h3>


    \anchor stdio_note1 \par Note 1:

    It might have been possible to implement a device abstraction that

    is compatible with \c fopen() but since this would have required

    to parse a string, and to take all the information needed either

    out of this string, or out of an additional table that would need to be

    provided by the application, this approach was not taken.


    \anchor stdio_note2 \par Note 2:

    This basically follows the Unix approach: if a device such as a

    terminal needs special handling, it is in the domain of the

    terminal device driver to provide this functionality.  Thus, a

    simple function suitable as \c put() for \c fdevopen() that talks

    to a UART interface might look like this:


    \code

    int

    uart_putchar(char c, FILE *stream)

    {


      if (c == '\n')

        uart_putchar('\r');

      loop_until_bit_is_set(UCSRA, UDRE);

      UDR = c;

      return 0;

    }

    \endcode


    \anchor stdio_note3 \par Note 3:

    This implementation has been chosen because the cost of maintaining

    an alias is considerably smaller than the cost of maintaining full

    copies of each stream.  Yet, providing an implementation that offers

    the complete set of standard streams was deemed to be useful.  Not

    only that writing \c printf() instead of <tt>fprintf(mystream, ...)</tt>

    saves typing work, but since avr-gcc needs to resort to pass all

    arguments of variadic functions on the stack (as opposed to passing

    them in registers for functions that take a fixed number of

    parameters), the ability to pass one parameter less by implying

    \c stdin will also save some execution time.

*/




