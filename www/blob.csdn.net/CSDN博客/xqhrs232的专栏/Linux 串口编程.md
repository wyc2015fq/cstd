# linux 串口编程 - xqhrs232的专栏 - CSDN博客
2010年12月09日 15:12:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：794
原文地址::[http://blog.csdn.net/freezgw1985/archive/2010/08/04/5789190.aspx](http://blog.csdn.net/freezgw1985/archive/2010/08/04/5789190.aspx)
在嵌入式Linux中，串口是一个字设备，访问具体的串行端口的编程与读／写文件的操作类似，只需打开相应的设备文件即可操作。串口编程特殊在于串口通信时相关参数与属性的设置。嵌入式Linux的串口编程时应注意，若在根文件中没有串口设备文件，应使用mknod命令创建，这这里假设串口设备是/dev/ttyS0，介绍一下串口的编程过程。
＃mknod /dev/ttyS0 c 4 64
1、打开串口
       打开串口设备文件的操作与普通文件的操作类似，都采用标准的I/O操作函数open()。
       fd = open("/dev/ttyS0",O_RDWR|O_NDELAY|O_NOCTTY);
    open()函数有两个参数，第一个参数是要打开的文件名（此处为串口设备文件/dev/ttyS0);第二个参数设置打开的方式，O_RDWR表示打开的文件可读/写，O_NDELAY表示以非阻塞方式打开，O＿NOCTTY表示若打开的文件为终端设备，则不会将终端作为进程控制终端。
2、设置串口属性
     串口通信时的属性设置是串口编程的关键问题，许多串口通信时的错误都与串口的设置相关，所以编程时应特别注意这些设置，最常见的设置包括波特率、奇偶校验和停止位以及流控制等。
    在Linux中，串口被作为终端I/O，它的参数设置需要使用struct termios结构体，这个结构体在termio.h文件中定义，且应在程序中包含这个头文件。
          typedef unsigned char         cc_t ;
          typedef unsigned int            speed_t ;
          typedef unsigned int            tcflag_t ;
          struct termios
          {
             tcflag_t      c_iflag ;          /*输入模式标志*/ 
             tcflag_t      c_oflag ;         /*输出模式标志*/
             tcflag_t      c_cflag ;            /*控制模式标志*/
             tcflag_t      c_lflag ;            /*本地模式标志*/
             tcflag_t      c_line ;            /*行规程类型，一般应用程序不使用*/
             cc_t         c_cc[NCC];      /*控制字符*/
             speed_t      c_ispeed ;      /*输入数据波特率*/
             speed_t      c_ospeed ;      /*输出数据波特率*/
       };
       串口的设置主要是设置这个结构体的各成员值，然后利用该结构体将参数传给硬件驱动程序。在Linux中，串口以串行终端的方式进行处理，因而，可以使用tcgetattr()/tcsetattr()函数获取/设置串口的参数。
       int tcgetattr( int fd, struct termios *termios_p );
       int tcsetattr( int fd, int optional_actions , struct termios *termios_p );
       这两个参数都有一个批向termios结构体的指针作为参数，用于返回当前终端的属性或设置该终端的属性。参数fd就是用open()函数打开的终端文件句柄，而串口就是用open()打开的串口设备文件句柄。tcsetattr()函数的optional_action参数用于指定新设定的参数起作用的时间，其设定值可以为：
       TCSANOW         改变立即生效
       TCSADRAIN      在所有的输出都被传输后改变生效，适用于更改影响输出参数的情况。
       TCSAFLUSH    在所有输出都被传输后改变生效，丢弃所有末读入的输入（清空输入缓                                存）。
（1）设置波特率
       使用cfsetospeed()/cfsetispeed()函数设置波特率，它们分别用于在termios结构体中设置输出和输入的波特率。设置波特率可以使用波特率常数，其定义为字母“B＋速率”，如B19200就是波特率为19200bps，B115200就是波特率为115200bps。
int cfsetispeed( struct termios *termios_p, speed_t speed );     //speed为波特率常数
int cfsetospeed( struct termios *termios_p, speed_t speed );
例 ：
cfsetispeed( ttys0_opt, B115200 );
cfsetospeed( ttys0_opt, B115200 );
（2）设置控制模式标志
       控制模式标志c_cflag主要用于设置串口对DCD信号状态检测、硬件流控制、字符位宽、停止位和奇偶校验等，常用标志位如下：
CLOCAL   忽略DCD信号，若不使用MODEM，或没有串口没有CD脚就设置此标志
CREAD    启用接收装置，可以接收字符
CRTSCTS启用硬件流控制，对于许多三线制的串不应使用，需设置～CRTCTS
CSIZE      字符位数掩码，常用CS8
CSTOPB   使用两个停止位，若用一位应设置～CSTOPB
PARENB   启用奇偶校验
       例如，下面的代码将串口设置为忽略DCD信号，启用接收装置，关闭硬件流控制，传输数据时使用8位数据位和一位停止位（8N1），不使用奇偶校验。
       struct temios ttys0
       ttyso_opt.c_cflag |= CLOCAL | CREAD ;      //将CLOCAL与CREAD位设置为1
       ttys0_opt.c_cflag &= ~CRTSCTS ;               //将硬件流控制位CRTSCTS清0，其他位不变
       ttys0_opt.c_cflag &＝ ～CSIZE ；               //清除数据位掩码
       ttys0_opt.c_cflag |= CS8 ;                           //设置8位数据位标志CS8
       ttys0_opt.c_cflag &= ~(PARENB|CSTOPB);//使用1位停止位，停用奇偶校验
（3）设置本地模式标志
       本地模式标志c_lflag主要用于设置终端与用户的交互方式，常见的设置标志位有ICAN－ON，ECHO和ECHOE等。其中，ICANON标志位用于实现规范输入，即read()读到行结束符后返回，常用于终端的处理；若串口用于发送/接收数据，则应清除此标志，使用非规范模式(raw mode)。非规范模式中，输入数据不组成行，不处规范模式中的特殊字符。在规范模式中，当设置ECHO标志位时，用户向终端输入的字符将被回传给用户；当设置ECHOE标志位时，用户输入退格键时，则回传“退格－空格－退格”序列给用户，使得退格键覆盖的字符从显示中消失，这样更符合用户的习惯（若未设置此标志，输入退格键时，则光标回退一个字符，但原有的字符未从显示中消失）。
（4）设置输入模式标志
       输入模式标志c_iflag主要用于控制串口的输入特性，常用的设置有IXOFF和IXON，分别用于软件流控制。其中，IXOFF用于防止输入缓冲区溢出；IXON则是在输入数据中识别软件流控制标志。由于许多嵌入式系统无法使用硬件流控制，因此，只能使用软件流控制数据传输的速度，但是，它可能降低串口数据传输效率。启用软件流控制的代码如下：
       ttys0_opt.c_iflag |= IXOFF|IXON ;
（5）设置输出模式标志
       输出模式标志c_oflag主要用于对串口在规范模式时输出的特殊字符处理，而对非规范模式无效。
（6）设置控制字符
       在非规范模式中，控制字符数组c_cc[]中的变量c_cc[VMIN]和c_cc[VTIME]用于设置read()返回前读到的最少字节数和读超时时间，其值分为四种情况：
    (a)c_cc[VMIN]＞0，c_cc[VTIME]>0
          读到一个字节后，启动定时器，其超时时间为c_cc[VTIME],read()返回的条件为至少读到c_cc[VMIN]个字符或定时器超期。
    (b)c_cc[VMIN]>0, c_cc[VTIME] ==0
          只要读到数据的字节数大于等于c_cc[VMIN]，则read()返回；否则，将无限期阻塞等待。
      (c)c_cc[VMIN] == 0, c_cc[VTIME]>0
          只要读到数据，则read()返回；若定时器超期（定时时间c_cc[VTIME]）却未读到数据，则read()返回0；
      (d)c_cc[VMIN] == 0, c_cc[VTIME] == 0
          若有数据，则read()读取指定数量的数据后返回；若没有数据，则read()返回0；
在termios结构体中填写完这些参数后，接下来就可以使用tcsetattr()函数设置串口的属性。
       tcsetattr( fd, &old_opt );         //将原有的设置保存到old_opt，以便程序结束后恢复
       tcsetattr( fd, TCSANOW, &ttsy0_opt );
3、清空发送/接收缓冲区
       为保证读/写操作不被串口缓冲区中原有的数据干拢，可以在读/写数据前用tcflush()函数清空串口发送/接收缓冲区。tcflush()函数的参数可为：
        TCIFLUSH      清空输入队列
        TCOFLUSH   清空输出队列
        TCIOFLUSH   同时清空输入和输出队列
4、从串口读写数据
        串口的数据读/写与普通文件的读/写一样，都是使用read()/write()函数实现。
        n = write( fd, buf, len );         //将buf中len个字节的数据从串口输出，返回输出的字节数
        n = read( fd, buf, len );         //从串口读入len个字节的数据并放入buf, 返回读取的字节数
5、关闭串口
    关闭串口的操作很简单，将打开的串口设备文件句柄关闭即可。
    close(fd);
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/freezgw1985/archive/2010/08/04/5789190.aspx](http://blog.csdn.net/freezgw1985/archive/2010/08/04/5789190.aspx)
