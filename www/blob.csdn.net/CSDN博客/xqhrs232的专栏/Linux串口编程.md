# Linux串口编程 - xqhrs232的专栏 - CSDN博客
2010年12月09日 15:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：667标签：[linux																[delay																[character																[input																[tabs																[output](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=tabs&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=character&t=blog)](https://so.csdn.net/so/search/s.do?q=delay&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.cppblog.com/amazon/archive/2010/01/28/106644.html](http://www.cppblog.com/amazon/archive/2010/01/28/106644.html)
**1.串口操作需要的头文件**#include <stdio.h>         //标准输入输出定义
#include <stdlib.h>        //标准函数库定义
#include <unistd.h>       //Unix标准函数定义
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          //文件控制定义
#include <termios.h>     //POSIX中断控制定义
#include <errno.h>        //错误号定义
**2.打开串口**
串口位于/dev中，可作为标准文件的形式打开，其中：
串口1 /dev/ttyS0
串口2 /dev/ttyS1
代码如下：
int fd;
fd = open(“/dev/ttyS0”, O_RDWR);
if(fd ==-1)
{
    Perror(“串口1打开失败！”);
}
//else
//fcntl(fd, F_SETFL, FNDELAY);
除了使用O_RDWR标志之外，通常还会使用O_NOCTTY和O_NDELAY这两个标志。
O_NOCTTY：告诉Unix这个程序不想成为“控制终端”控制的程序，不说明这个标志的话，任何输入都会影响你的程序。
O_NDELAY：告诉Unix这个程序不关心DCD信号线状态，即其他端口是否运行，不说明这个标志的话，该程序就会在DCD信号线为低电平时停止。
**3.设置波特率**
最基本的串口设置包括波特率、校验位和停止位设置，且串口设置主要使用termios.h头文件中定义的termios结构，如下：
struct termios
{
   tcflag_t  c_iflag;   //输入模式标志
   tcflag_t  c_oflag;  //输出模式标志
   tcflag_t  c_cflag;  //控制模式标志
   tcflag_t  c_lflag;   //本地模式标志
   cc_t   c_line;              //line discipline
   cc_t   c_cc[NCC];    //control characters
}
代码如下：
int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200, 9600, 4800, 2400, 1200, 300, };
void SetSpeed(int fd, int speed)
{
int i;
struct termios Opt;    //定义termios结构
if(tcgetattr(fd, &Opt) !=0)
    {
        perror(“tcgetattr fd”);
return;
    }
for(i =0; i <sizeof(speed_arr) /sizeof(int); i++)
    {
if(speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);
            cfsetospeed(&Opt, speed_arr[i]);
if(tcsetattr(fd, TCSANOW, &Opt) !=0)
            {
                perror(“tcsetattr fd”);
return;
            }
            tcflush(fd, TCIOFLUSH);
        }
    }
}
注意tcsetattr函数中使用的标志：
TCSANOW：立即执行而不等待数据发送或者接受完成。
TCSADRAIN：等待所有数据传递完成后执行。
TCSAFLUSH：Flush input and output buffers and make the change
**4.设置数据位、停止位和校验位**
以下是几个数据位、停止位和校验位的设置方法：（以下均为1位停止位）
8位数据位、无校验位：
Opt.c_cflag &= ~PARENB;
Opt.c_cflag &= ~CSTOPB;
Opt.c_cflag &= ~CSIZE;
Opt.c_cflag |= CS8;
7位数据位、奇校验：
Opt.c_cflag |= PARENB;
Opt.c_cflag |= PARODD;
Opt.c_cflag &= ~CSTOPB;
Opt.c_cflag &= ~CSIZE;
Opt.c_cflag |= CS7;
7位数据位、偶校验：
Opt.c_cflag |= PARENB;
Opt.c_cflag &= ~PARODD;
Opt.c_cflag &= ~CSTOPB;
Opt.c_cflag &= ~CSIZE;
Opt.c_cflag |= CS7;
7位数据位、Space校验：
Opt.c_cflag &= ~PARENB;
Opt.c_cflag &= ~CSTOPB;
Opt.c_cflag &= ~CSIZE;
Opt.c_cflag |= CS7;
代码如下：
int SetParity(int fd, int databits, int stopbits, int parity)
{
struct termios Opt;
if(tcgetattr(fd, &Opt) !=0)
    {
        perror("tcgetattr fd");
return FALSE;
    }
   Opt.c_cflag |= (CLOCAL | CREAD);        //一般必设置的标志
switch(databits)        //设置数据位数
    {
case7:
        Opt.c_cflag &=~CSIZE;
        Opt.c_cflag |= CS7;
break;
case8:
        Opt.c_cflag &=~CSIZE;
        Opt.c_cflag |= CS8;
        berak;
default:
        fprintf(stderr, "Unsupported data size./n");
return FALSE;
    }
switch(parity)            //设置校验位
    {
case'n':
case'N':
        Opt.c_cflag &=~PARENB;        //清除校验位
        Opt.c_iflag &=~INPCK;        //enable parity checking
break;
case'o':
case'O':
        Opt.c_cflag |= PARENB;        //enable parity
        Opt.c_cflag |= PARODD;        //奇校验
        Opt.c_iflag |= INPCK            //disable parity checking
break;
case'e':
case'E':
        Opt.c_cflag |= PARENB;        //enable parity
        Opt.c_cflag &=~PARODD;        //偶校验
        Opt.c_iflag |= INPCK;            //disable pairty checking
break;
case's':
case'S':
        Opt.c_cflag &=~PARENB;        //清除校验位
        Opt.c_cflag &=~CSTOPB;        //??????????????
        Opt.c_iflag |= INPCK;            //disable pairty checking
break;
default:
        fprintf(stderr, "Unsupported parity./n");
return FALSE;    
    }
switch(stopbits)        //设置停止位
    {
case1:
        Opt.c_cflag &=~CSTOPB;
break;
case2:
        Opt.c_cflag |= CSTOPB;
break;
default:
        fprintf(stderr, "Unsupported stopbits./n");
return FALSE;
    }
    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_lflag &=~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag &=~OPOST;
    opt.c_oflag &=~(ONLCR | OCRNL);    //添加的
    opt.c_iflag &=~(ICRNL | INLCR);
    opt.c_iflag &=~(IXON | IXOFF | IXANY);    //添加的
    tcflush(fd, TCIFLUSH);
    Opt.c_cc[VTIME] =0;        //设置超时为15sec
    Opt.c_cc[VMIN] =0;        //Update the Opt and do it now
if(tcsetattr(fd, TCSANOW, &Opt) !=0)
    {
        perror("tcsetattr fd");
return FALSE;
    }
return TRUE;
}
**5.某些设置项**
在第四步中我们看到一些比较特殊的设置，下面简述一下他们的作用。
c_cc数组的VSTART和VSTOP元素被设定成DC1和DC3，代表ASCII标准的XON和XOFF字符，如果在传输这两个字符的时候就传不过去，需要把软件流控制屏蔽，即：
Opt.c_iflag &= ~ (IXON | IXOFF | IXANY);
有时候，在用write发送数据时没有键入回车，信息就发送不出去，这主要是因为我们在输入输出时是按照规范模式接收到回车或换行才发送，而更多情况下我们是不必键入回车或换行的。此时应转换到行方式输入，不经处理直接发送，设置如下：
Opt.c_lflag &= ~ (ICANON | ECHO | ECHOE | ISIG);
还存在这样的情况：发送字符0X0d的时候，往往接收端得到的字符是0X0a，原因是因为在串口设置中c_iflag和c_oflag中存在从NL-CR和CR-NL的映射，即串口能把回车和换行当成同一个字符，可以进行如下设置屏蔽之：
Opt.c_iflag &= ~ (INLCR | ICRNL | IGNCR);
Opt.c_oflag &= ~(ONLCR | OCRNL);
**6.读写串口**
发送数据方式如下，write函数将返回写的位数或者当错误时为-1。
char buffer[1024];
int length;
int nByte;
nByte = write(fd, buffer, length);
读取数据方式如下，原始数据模式下每个read函数将返回实际串口收到的字符数，如果串口中没有字符可用，回叫将会阻塞直到以下几种情况：有字符进入；一个间隔计时器失效；错误发送。
在打开串口成功后，使用fcntl(fd, F_SETFL, FNDELAY)语句，可以使read函数立即返回而不阻塞。FNDELAY选项使read函数在串口无字符时立即返回且为0。
char buffer[1024];
int length;
int readByte;
readByte = read(fd, buffer, len);
注意：设置为原始模式传输数据的话，read函数返回的字符数是实际串口收到的字符数。Linux下直接用read读串口可能会造成堵塞，或者数据读出错误，此时可使用tcntl或者select等函数实现异步读取。用select先查询com口，再用read去读就可以避免上述错误。
**7.关闭串口**
串口作为文件来处理，所以一般的关闭文件函数即可：
close(fd);
**8.例子**
这个例子中，需要打开串口1，设置9600波特率、8位数据位、1位停止位以及空校验，之后利用while语句循环判断串口中是否可以读出数据，将串口中数据连续读出后重新写回到串口中。
该程序可与minicom联合测试。
#include     <stdio.h>
#include     <stdlib.h>
#include     <unistd.h>
#include     <sys/types.h>
#include     <sys/stat.h>
#include     <fcntl.h>
#include     <termios.h>
#include     <errno.h>
main()
{
int fd;
int i;
int len;
int n =0;      
char read_buf[256];
char write_buf[256];
struct termios opt; 
    fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);    //默认为阻塞读方式
if(fd ==-1)
    {
        perror("open serial 0/n");
        exit(0);
    }
    tcgetattr(fd, &opt);      
    cfsetispeed(&opt, B9600);
    cfsetospeed(&opt, B9600);
if(tcsetattr(fd, TCSANOW, &opt) !=0 )
    {     
       perror("tcsetattr error");
return-1;
    }
    opt.c_cflag &=~CSIZE;  
    opt.c_cflag |= CS8;   
    opt.c_cflag &=~CSTOPB; 
    opt.c_cflag &=~PARENB; 
    opt.c_cflag &=~INPCK;
    opt.c_cflag |= (CLOCAL | CREAD);
    opt.c_lflag &=~(ICANON | ECHO | ECHOE | ISIG);
    opt.c_oflag &=~OPOST;
    opt.c_oflag &=~(ONLCR | OCRNL);    //添加的
    opt.c_iflag &=~(ICRNL | INLCR);
    opt.c_iflag &=~(IXON | IXOFF | IXANY);    //添加的
    opt.c_cc[VTIME] =0;
    opt.c_cc[VMIN] =0;
    tcflush(fd, TCIOFLUSH);
    printf("configure complete/n");
if(tcsetattr(fd, TCSANOW, &opt) !=0)
    {
        perror("serial error");
return-1;
    }
    printf("start send and receive data/n");
while(1)
    {    
        n =0;
        len =0;
        bzero(read_buf, sizeof(read_buf));    //类似于memset
        bzero(write_buf, sizeof(write_buf));
while( (n = read(fd, read_buf, sizeof(read_buf))) >0 )
        {
for(i = len; i < (len + n); i++)
            {
                write_buf[i] = read_buf[i - len];
            }
            len += n;
        }
        write_buf[len] ='/0';
        printf("Len %d /n", len);
        printf("%s /n", write_buf);
        n = write(fd, write_buf, len);
        printf("write %d chars/n",n);
        sleep(2);
    }
}
**9.附录**
c_cflag用于设置控制参数，除了波特率外还包含以下内容： 
EXTA         External rate clock
EXTB         External rate clock
CSIZE         Bit mask for data bits
CS5         5个数据位
CS6         6个数据位
CS7         7个数据位
CS8         8个数据位
CSTOPB         2个停止位（清除该标志表示1个停止位
PARENB         允许校验位
PARODD         使用奇校验（清除该标志表示使用偶校验）
CREAD         Enable receiver
HUPCL         Hangup (drop DTR) on last close
CLOCAL         Local line – do not change “owner” of port
LOBLK         Block job control outpu
c_cflag标志可以定义CLOCAL和CREAD，这将确保该程序不被其他端口控制和信号干扰，同时串口驱动将读取进入的数据。CLOCAL和CREAD通常总是被是能的。
c_lflag用于设置本地模式，决定串口驱动如何处理输入字符，设置内容如下：
ISIG            Enable SIGINTR, SIGSUSP, SIGDSUSP, and SIGQUIT signals 
ICANON         Enable canonical input (else raw) 
XCASE         Map uppercase /lowercase (obsolete) 
ECHO         Enable echoing of input characters 
ECHOE         Echo erase character as BS-SP-BS 
ECHOK         Echo NL after kill character 
ECHONL         Echo NL 
NOFLSH         Disable flushing of input buffers after interrupt or quit characters
IEXTEN         Enable extended functions 
ECHOCTL         Echo control characters as ^char and delete as ~? 
ECHOPRT         Echo erased character as character erased 
ECHOKE         BS-SP-BS entire line on line kill 
FLUSHO         Output being flushed 
PENDIN         Retype pending input at next read or input char 
TOSTOP         Send SIGTTOU for background output
c_iflag用于设置如何处理串口上接收到的数据，包含如下内容：
INPCK         Enable parity check 
IGNPAR         Ignore parity errors 
PARMRK      Mark parity errors 
ISTRIP         Strip parity bits 
IXON         Enable software flow control (outgoing) 
IXOFF         Enable software flow control (incoming) 
IXANY         Allow any character to start flow again 
IGNBRK         Ignore break condition 
BRKINT         Send a SIGINT when a break condition is detected 
INLCR         Map NL to CR 
IGNCR         Ignore CR 
ICRNL         Map CR to NL 
IUCLC         Map uppercase to lowercase 
IMAXBEL      Echo BEL on input line too long
c_oflag用于设置如何处理输出数据，包含如下内容：
OPOST         Postprocess output (not set = raw output) 
OLCUC         Map lowercase to uppercase 
ONLCR         Map NL to CR-NL 
OCRNL         Map CR to NL 
NOCR         No CR output at column 0 
ONLRET      NL performs CR function 
OFILL         Use fill characters for delay 
OFDEL         Fill character is DEL 
NLDLY         Mask for delay time needed between lines 
NL0            No delay for NLs 
NL1            Delay further output after newline for 100 milliseconds 
CRDLY      Mask for delay time needed to return carriage to left column 
CR0            No delay for CRs 
CR1            Delay after CRs depending on current column position 
CR2            Delay 100 milliseconds after sending CRs 
CR3            Delay 150 milliseconds after sending CRs 
TABDLY      Mask for delay time needed after TABs 
TAB0            No delay for TABs 
TAB1         Delay after TABs depending on current column position 
TAB2         Delay 100 milliseconds after sending TABs 
TAB3         Expand TAB characters to spaces 
BSDLY      Mask for delay time needed after BSs 
BS0         No delay for BSs 
BS1         Delay 50 milliseconds after sending BSs 
VTDLY      Mask for delay time needed after VTs 
VT0         No delay for VTs 
VT1         Delay 2 seconds after sending VTs 
FFDLY      Mask for delay time needed after FFs 
FF0         No delay for FFs 
FF1         Delay 2 seconds after sending FFs
c_cc定义了控制字符，包含以下内容：
VINTR  Interrupt  CTRL-C 
VQUIT  Quit    CTRL-Z 
VERASE   Erase    Backspace (BS) 
VKILL   Kill-line   CTRL-U 
VEOF   End-of-file   CTRL-D 
VEOL   End-of-line   Carriage return (CR) 
VEOL2   Second    end-of-line Line feed (LF) 
VMIN   Minimum number of characters to read  
VSTART   Start flow   CTRL-Q (XON) 
VSTOP   Stop flow   CTRL-S (XOFF) 
VTIME   Time to wait for data (tenths of seconds) 
注意：控制符VTIME和VMIN之间有复杂的关系。VTIME定义要求等待的时间（百毫米，通常是unsigned char变量），而VMIN定义了要求等待的最小字节数（相比之下，read函数的第三个参数指定了要求读的最大字节数）。
如果VTIME=0，VMIN=要求等待读取的最小字节数，read必须在读取了VMIN个字节的数据或者收到一个信号才会返回。
如果VTIME=时间量，VMIN=0，不管能否读取到数据，read也要等待VTIME的时间量。
如果VTIME=时间量，VMIN=要求等待读取的最小字节数，那么将从read读取第一个字节的数据时开始计时，并会在读取到VMIN个字节或者VTIME时间后返回。
如果VTIME=0，VMIN=0，不管能否读取到数据，read都会立即返回。

