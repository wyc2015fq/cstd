# 《Linux程序设计》——终端 - 一世豁然的专栏 - CSDN博客





2014年09月22日 22:21:48[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1628








一、对终端进行读写

 1、标准模式和非标准模式

   1）、只有在用户按下回车键后，程序才能读取到程序的输入。非标准模式则相反。

   2）、Linux终端处理程序能够把中断字符转换为相应的信号。




 2、书代码menu1.c的两个问题

   1）、不管输入的正确或错误的选项，都会显示错误信息。

     I、产生原因：Linux会暂存用户输入的内容，知道用户按下回车键，然后将用户选择的字符以及紧随其后的回车符一起传递给程序。

    II、UNIX用一个单独的换行符来表示一行的结束，而其他的操作系统用回车符和换行符两个字符来表示一行的结束。

   III、就解决办法。（书中P147阴影代码）


   2）、只有在按下回车键后程序才会读取输入。




 3、处理重定向输出

  1）、Linux程序，甚至是交互式的Linux程序，经常会把它们的输入或输出重定向到文件或其他程序中。（**>**或**<**）

  2）、isatty函数

**  #include<unistd.h>**

**        int  isatty(int  fd);**

       I、检测标准输出是否被重定向。

      II、如果打开的文件描述符fd连接到一个终端，则系统调用isatty返回1，否则返回0.

     III、如果使用的参数是文件流，则需要与file弄函数一起使用。

     IV、如果标准输出已被重定向，则将消息写到stderr（标准错误输出），他不会被shell的>命令重定向。







二、与终端进行对话

 1、当不希望程序中与用户交互的部分被重定向，但允许其他的输入和输出被重定向，此时可以用特殊设备/dev/tty，该设备始终指向当前终端或当前的登录会话。

 2、理解并熟悉menu3.c代码。








三、终端驱动程序和通用终端接口（略）







四、termios结构

 1、通过设置termois类型额结构数据中的值和使用一小组函数调用，就可以对终端接口进行控制。




 2、termios结构（P153）。




 3、tcgetattr函数

**#include<termios.h>**

**    int  tcgetattr(int  fd,  const  termios  *termios_p);**

   I、可以调用此函数把当前终端接口变量写入termios_p参数指向的结构。

   II、可以调用此函数来初始化与一个终端对应的termois结构。




 4、tcsetattr函数

**#include<termios.h>**

**    int  tcsetattr(int  fd,   int  actions,  const  termios  *termios_p);**

  I、如果结构被修改，可以调用此函数重新配置此接口。

  II、参数actions控制修改方式。（P153）




5、输入模式

 1）、输入模式控制输入数据（终端驱动程序从串口或键盘接受到的字符）在被传递给程序之前的处理方式。

  2）、可以通过termios结构中c_iflag成员的标志对他们进行控制。所有的标志都被定义为宏，并可通过按位或的方式结合起来。

  3）、iflag成员宏（P154）。




 6、输出模式

  1）、输出模式控制输出字符的处理方式，即由程序发送出去的字符在传递到串行口或屏幕之前是如何处理的。

  2）、可以通过termios结构中c_oflag成员的标志对输出模式进行控制。

  3）、c_oflag成员宏。




 7、控制模式

  1）、控制模式控制终端的硬件特性。

  2）、可以通过termios结构中c_oflag成员的标志对输出模式进行配置。

  3）、c_cflag成员宏。




 8、本地模式

  1）、本地模式控制终端额各种特性。

  2）、可以通过termios结构中c_lflag成员的标志对输出模式进行配置。

  3）、c_lflag成员宏。




 9、特殊控制字符

  1）、特殊控制字符是一些字符组合。

  2）、termios结构中c_cc数组成员将给中特殊控制字符映射到对应的支持函数。每个字符的位置（它在数组中的下标）是由一个宏定义的，但并不限制这些字符必须是控制字

          符。

  3）、根据终端是否被设置为标准模式，c_cc数组有标准和非标准模式两种用法。

         I、c_cc数组标准模式数组下标。（P156）

        II、c_cc数组非标准模式数组下标。（P156）

  4）、字符

       I、特殊字符和非标准值表。（P153  表5-1）

  5）、TIME和MIN值（P157）

  6）、通过shell访问终端模式。

 7）、在命令提示符下设置终端模式。




 10、终端速度

**  #include<termios.h> **

**       speed_t  cfgetispeed(const  struct  termios *);**

**      speed_t  cfgetospeed(const  struct  termios *);**

**       int  cfsetispeed(struct  termios *, speed_t  speed);**

**       int  cfsetospeed(struct  termios *, speed_t  speed);**

      1）、以上函数用于控制终端的速度。

      2）、以上函数用于termios结构，而不是直接作用于接口。要想设置新的终端速度，必须先调用函数tcgetattr函数获取当前终端，然后使用上述函数之一设置终端速度，最


                后使用函数tcsetattr函数写回termios结构。只用在调用了函数tcsetattr之后，终端速度才会改变。

      3）、参数speed（P158-159）




 11、其他函数

**    #include<termios.h>**

**       int  tcdrain(int  fd);**

**       int  tcflow(int  fd, int  flowtype);**

**       int  tcflush(int  fd, int  int_out_selector);     **

       1）、以上函数可以直接对文件描述符进行操作，不需要读写termios结构。

        2）、函数tcdrain的作用是让调用程序一直等待，直到所有排队的输出都已经发送完毕。

        3）、函数tcflow用于暂停或重新开始输出。

        4）、函数tcflush用于清空输入、输出或者两者都清空。




 12、termios结构另一个常见用法是将终端设置为：一旦输入字符，程序就立即读取它。







五、终端的输出

 1、终端的类型

  1）、终端有各种各样的类型，所以程序员必须针对相对应的终端编写相应的代码。但termios软件包可以解决这个问题。

  2）、为了使用termios函数，你通常需要在程序中包括curses头文件curses.h和terminfo自己的头文件term.h。在一些Linux系统上有时不得不使用ncurses的curses实现，并

            在程序中包含ncurses.h头文件以提供对terminfo函数原型定义。




 2、识别终端类型

  1）、Linux程序包含一个变量TERM，它的值被设置为当前正在使用的终端类型。

  2）、terminfo软件包包含一个由大量不同类型终端的功能标志和escape转义序列等信息构成的数据库，并且为使用它们提供了一个统一的编程接口。

  3）、每个终端类型对应一个定义器功能标志和如何访问器特征的terminfo文件。文件格式为可读的源代码，然后通过tic命令将源文件编译为更加紧凑、有效的格式，以方便应


            用程序的使用。

  4）、每个terminfo定义由3种类型的数据项构成。每个数据项被称为capname，它们分别用于定义终端的一种功能标志。

    I、布尔功能标志指出终端是否支持某个特定的功能。

   II、数值功能标志定义长度。

   III字符串功能标志用来定义两种截然不同的终端功能：用于访问终端功能的输出字符串和当用户按下特定按键时终端接收到的输入字符串。

   IV、特殊键的定义也采用如字符串功能键标志类似的方法。




 3、使用terminfo功能标志

  1）、setupterm函数

**#include<term.h>**

**         int  setupterm(char  *term, int fd,int  *erret);**

         I、此函数可以用来初始化TERMINFO结构。

        II、setupterm函数将当前终端类型设置为参数term指向的值，如果term是空指针，就用环境变量TERM的值。参数fd为一个打开的文件描述符，它用于向终端写数据。如果

              参数erret不是一个空指针，则函数的返回值保存在该参数指向的整型变量中。

       III、errret可能写入的值：-1（terminfo数据库不存在），0（terminfo数据库中没有匹配的数据项），1（成功）。

  2）、tigetflag、tigetnum、tigetstr函数

**  #include<term.h>**

**        int  tigetflag(char  *capname);**

**        int  tigetnum(char  *capname);**

**        int  tigetstr(char  *capname);**

       I、以上函数用来访问terminfo的功能标志，每个函数对应一个功能标志类型。

      II、tigetflag、tigetnum、tigetstr函数分别返回terminfo中的布尔功能标志、数值功能标志和字符串功能标志的值。失败时，tigetflag返回-1，tigetnum函数返回-2，tigetstr函

           数返回（char  *）-1。

  3）、tparm函数

**#include<term.h>**

**        char  *tparm(char  *cap,  long  p1,  long  p2,.........，long  p9);**

        I、此函数可用实际的数值替换功能标志中的参数，依次最多可以替换九个参数，并返回一个可用的escape转义序列。

    4）、putp、tpus函数

**   #include<term.h>**

**           int  putp(char  *const  str);**

**           int  tputs(char  *const  str,  int  attcnt,  int  (*putfunc)(int));**


       I、当用tparm函数构造好终端的escape转义序列后，必须将其发送到终端。


      II、putp函数在成功时返回OK，失败时返回ERR。它以一个控制字符串为参数，并发送到标准输出到stdout。

     III、tputs函数是为不能通过标准stdout访问终端的情况准备的，它可以制定一个用于输出字符的函数。tputs函数的返回值是用户指定的函数putfunc的返回结果。参数affcnt的

           作用是表明受这一种变化影响的行数，它一般被设置为1。







六、检测击键动作（P170-171代码）




七、虚拟控制台



八、伪终端













