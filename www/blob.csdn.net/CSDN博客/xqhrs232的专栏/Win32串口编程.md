# Win32串口编程 - xqhrs232的专栏 - CSDN博客
2013年01月18日 22:04:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：558
个人分类：[串口技术/无线串口技术](https://blog.csdn.net/xqhrs232/article/category/1347902)
原文地址::[http://www.vckbase.com/index.php/wv/1439](http://www.vckbase.com/index.php/wv/1439)
相关网帖
1、Win32串口编程详解（一）----[http://www.360doc.com/content/11/0827/13/7023119_143649079.shtml](http://www.360doc.com/content/11/0827/13/7023119_143649079.shtml)
2、Win32串口编程详解（二）----[http://www.360doc.com/content/11/0827/13/7023119_143649230.shtml](http://www.360doc.com/content/11/0827/13/7023119_143649230.shtml)
3、[[转载]Win32 串口编程（一）](http://blog.chinaunix.net/uid-20717979-id-3430903.html) ----[http://blog.chinaunix.net/uid-20717979-id-3430903.html](http://blog.chinaunix.net/uid-20717979-id-3430903.html)
在工业控制中，工控机（一般都基于Windows平台）经常需要与智能仪表通过串口进行通信。串口通信方便易行，应用广泛。
一般情况下，工控机和各智能仪表通过RS485总线进行通信。RS485的通信方式是半双工的，只能由作为主节点的工控PC机依次轮询网络上的各智能控制单元子节点。每次通信都是由PC机通过串口向智能控制单元发布命令，智能控制单元在接收到正确的命令后作出应答。
在Win32下，可以使用两种编程方式实现串口通信，其一是使用ActiveX控件，这种方法程序简单，但欠灵活。其二是调用Windows的API函数，这种方法可以清楚地掌握串口通信的机制，并且自由灵活。本文我们只介绍API串口通信部分。
串口的操作可以有两种操作方式：同步操作方式和重叠操作方式（又称为异步操作方式）。同步操作时，API函数会阻塞直到操作完成以后才能返回（在多线程方式中，虽然不会阻塞主线程，但是仍然会阻塞监听线程）；而重叠操作方式，API函数会立即返回，操作在后台进行，避免线程的阻塞。
无论那种操作方式，一般都通过四个步骤来完成：
（1） 打开串口
（2） 配置串口
（3） 读写串口
（4） 关闭串口
（1） 打开串口
Win32系统把文件的概念进行了扩展。无论是文件、通信设备、命名管道、邮件槽、磁盘、还是控制台，都是用API函数CreateFile来打开或创建的。该函数的原型为：
`1.``HANDLE``CreateFile( ``LPCTSTR``lpFileName,`
`2.``DWORD``dwDesiredAccess,`
`3.``DWORD``dwShareMode,`
`4.`
```
LPSECURITY_ATTRIBUTES
 lpSecurityAttributes,
```
`5.``DWORD``dwCreationDistribution,`
`6.``DWORD``dwFlagsAndAttributes,`
`7.``HANDLE``hTemplateFile);`
lpFileName：将要打开的串口逻辑名，如“COM1”； dwDesiredAccess：指定串口访问的类型，可以是读取、写入或二者并列； dwShareMode：指定共享属性，由于串口不能共享，该参数必须置为0； lpSecurityAttributes：引用安全性属性结构，缺省值为NULL； dwCreationDistribution：创建标志，对串口操作该参数必须置为OPEN_EXISTING； dwFlagsAndAttributes：属性描述，用于指定该串口是否进行异步操作，该值为FILE_FLAG_OVERLAPPED，表示使用异步的I/O；该值为0，表示同步I/O操作；
 hTemplateFile：对串口而言该参数必须置为NULL；
同步I/O方式打开串口的示例代码：
`01.``HANDLE``hCom;  ``//全局变量，串口句柄`
`02.``hCom=CreateFile(``"COM1"``,``//COM1口`
`03.``GENERIC_READ|GENERIC_WRITE, ``//允许读和写`
`04.``0, ``//独占方式`
`05.``NULL,`
`06.``OPEN_EXISTING, ``//打开而不是创建`
`07.``0, ``//同步方式`
`08.``NULL);`
`09.``if``(hCom==(``HANDLE``)-1)`
`10.``{`
`11.``AfxMessageBox(``"打开COM失败!"``);`
`12.``return``FALSE;`
`13.``}`
`14.``return``TRUE;`
重叠I/O打开串口的示例代码：
`01.``HANDLE``hCom;  ``//全局变量，串口句柄`
`02.`
```
hCom
 =CreateFile(
```
`"COM1"``,  ``//COM1口`
`03.``GENERIC_READ|GENERIC_WRITE, ``//允许读和写`
`04.``0,  ``//独占方式`
`05.``NULL,`
`06.``OPEN_EXISTING,  ``//打开而不是创建`
`07.``FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, ``//重叠方式`
`08.``NULL);`
`09.``if`
```
(hCom
 ==INVALID_HANDLE_VALUE)
```
`10.``{`
`11.``AfxMessageBox(``"打开COM失败!"``);`
`12.``return``FALSE;`
`13.``}`
`14.``return``TRUE;`
（2）、配置串口
在打开通讯设备句柄后，常常需要对串口进行一些初始化配置工作。这需要通过一个DCB结构来进行。DCB结构包含了诸如波特率、数据位数、奇偶校验和停止位数等信息。在查询或配置串口的属性时，都要用DCB结构来作为缓冲区。
一般用CreateFile打开串口后，可以调用GetCommState函数来获取串口的初始配置。要修改串口的配置，应该先修改DCB结构，然后再调用SetCommState函数设置串口。
DCB结构包含了串口的各项参数设置，下面仅介绍几个该结构常用的变量：
`01.``typedef``struct``_DCB{`
`02.``………`
`03.``//波特率，指定通信设备的传输速率。这个成员可以是实际波特率值或者下面的常量值之一：`
`04.``DWORD``BaudRate;`
`05.`
```
CBR_110，CBR_300，CBR_600，CBR_1200，CBR_2400，CBR_4800，CBR_9600，CBR_19200，
 CBR_38400，
```
`06.`
```
CBR_56000，
 CBR_57600， CBR_115200， CBR_128000， CBR_256000， CBR_14400
```
`07.`
`08.``DWORD``fParity; `
```
//
 指定奇偶校验使能。若此成员为1，允许奇偶校验检查
```
`09.``…`
`10.``BYTE``ByteSize; `
```
//
 通信字节位数，4—8
```
`11.``BYTE``Parity; ``//指定奇偶校验方法。此成员可以有下列值：`
`12.`
```
EVENPARITY
 偶校验     NOPARITY 无校验
```
`13.`
```
MARKPARITY
 标记校验   ODDPARITY 奇校验
```
`14.``BYTE``StopBits; ``//指定停止位的位数。此成员可以有下列值：`
`15.`
```
ONESTOPBIT
 1位停止位   TWOSTOPBITS 2位停止位
```
`16.`
```
ONE5STOPBITS  
 1.5位停止位
```
`17.``………`
`18.`
```
}
 DCB;
```
`19.``winbase.h文件中定义了以上用到的常量。如下：`
`20.`
```
#define
 NOPARITY            0
```
`21.`
```
#define
 ODDPARITY           1
```
`22.`
```
#define
 EVENPARITY          2
```
`23.`
```
#define
 ONESTOPBIT          0
```
`24.`
```
#define
 ONE5STOPBITS        1
```
`25.`
```
#define
 TWOSTOPBITS         2
```
`26.`
```
#define
 CBR_110             110
```
`27.`
```
#define
 CBR_300             300
```
`28.`
```
#define
 CBR_600             600
```
`29.`
```
#define
 CBR_1200            1200
```
`30.`
```
#define
 CBR_2400            2400
```
`31.`
```
#define
 CBR_4800            4800
```
`32.`
```
#define
 CBR_9600            9600
```
`33.`
```
#define
 CBR_14400           14400
```
`34.`
```
#define
 CBR_19200           19200
```
`35.`
```
#define
 CBR_38400           38400
```
`36.`
```
#define
 CBR_56000           56000
```
`37.`
```
#define
 CBR_57600           57600
```
`38.`
```
#define
 CBR_115200          115200
```
`39.`
```
#define
 CBR_128000          128000
```
`40.`
```
#define
 CBR_256000          256000
```
GetCommState函数可以获得COM口的设备控制块，从而获得相关参数：
`01.``BOOL``GetCommState(`
`02.``HANDLE``hFile, ``//标识通讯端口的句柄`
`03.`
```
LPDCB
 lpDCB
```
`//指向一个设备控制块（DCB结构）的指针`
`04.``);`
`05.``SetCommState函数设置COM口的设备控制块：`
`06.``BOOL``SetCommState(`
`07.``HANDLE``hFile,`
`08.`
```
LPDCB
 lpDCB
```
`09.``);`
除了在BCD中的设置外，程序一般还需要设置I/O缓冲区的大小和超时。Windows用I/O缓冲区来暂存串口输入和输出的数据。如果通信的速率较高，则应该设置较大的缓冲区。调用SetupComm函数可以设置串行口的输入和输出缓冲区的大小。
`1.``BOOL``SetupComm(`
`2.`
`3.``HANDLE``hFile,   `
```
//
 通信设备的句柄
```
`4.``DWORD``dwInQueue,    `
```
//
 输入缓冲区的大小（字节数）
```
`5.``DWORD``dwOutQueue    `
```
//
 输出缓冲区的大小（字节数）
```
`6.``);`
在用ReadFile和WriteFile读写串行口时，需要考虑超时问题。超时的作用是在指定的时间内没有读入或发送指定数量的字符，ReadFile或WriteFile的操作仍然会结束。
要查询当前的超时设置应调用GetCommTimeouts函数，该函数会填充一个COMMTIMEOUTS结构。调用SetCommTimeouts可以用某一个COMMTIMEOUTS结构的内容来设置超时。
读写串口的超时有两种：间隔超时和总超时。间隔超时是指在接收时两个字符之间的最大时延。总超时是指读写操作总共花费的最大时间。写操作只支持总超时，而读操作两种超时均支持。用COMMTIMEOUTS结构可以规定读写操作的超时。
COMMTIMEOUTS结构的定义为：
`1.``typedef``struct`
```
_COMMTIMEOUTS
 {
```
`2.``DWORD``ReadIntervalTimeout; ``//读间隔超时`
`3.``DWORD``ReadTotalTimeoutMultiplier; ``//读时间系数`
`4.``DWORD``ReadTotalTimeoutConstant; ``//读时间常量`
`5.``DWORD``WriteTotalTimeoutMultiplier; `
```
//
 写时间系数
```
`6.``DWORD``WriteTotalTimeoutConstant; ``//写时间常量`
`7.`
```
}
 COMMTIMEOUTS,*LPCOMMTIMEOUTS;
```
COMMTIMEOUTS结构的成员都以毫秒为单位。总超时的计算公式是：
总超时＝时间系数×要求读/写的字符数＋时间常量
例如，要读入10个字符，那么读操作的总超时的计算公式为：
读总超时＝ReadTotalTimeoutMultiplier×10＋ReadTotalTimeoutConstant
可以看出：间隔超时和总超时的设置是不相关的，这可以方便通信程序灵活地设置各种超时。
如果所有写超时参数均为0，那么就不使用写超时。如果ReadIntervalTimeout为0，那么就不使用读间隔超时。如果ReadTotalTimeoutMultiplier 和 ReadTotalTimeoutConstant 都为0，则不使用读总超时。如果读间隔超时被设置成MAXDWORD并且读时间系数和读时间常量都为0，那么在读一次输入缓冲区的内容后读操作就立即返回，而不管是否读入了要求的字符。
在用重叠方式读写串口时，虽然ReadFile和WriteFile在完成操作以前就可能返回，但超时仍然是起作用的。在这种情况下，超时规定的是操作的完成时间，而不是ReadFile和WriteFile的返回时间。
配置串口的示例代码：
`01.``SetupComm(hCom,1024,1024); ``//输入缓冲区和输出缓冲区的大小都是1024`
`02.`
`03.`
```
COMMTIMEOUTS
 TimeOuts;
```
`04.``//设定读超时`
`05.``TimeOuts.ReadIntervalTimeout=1000;`
`06.``TimeOuts.ReadTotalTimeoutMultiplier=500;`
`07.``TimeOuts.ReadTotalTimeoutConstant=5000;`
`08.``//设定写超时`
`09.``TimeOuts.WriteTotalTimeoutMultiplier=500;`
`10.``TimeOuts.WriteTotalTimeoutConstant=2000;`
`11.``SetCommTimeouts(hCom,&TimeOuts); ``//设置超时`
`12.`
`13.`
```
DCB
 dcb;
```
`14.``GetCommState(hCom,&dcb);`
`15.``dcb.BaudRate=9600; ``//波特率为9600`
`16.``dcb.ByteSize=8; ``//每个字节有8位`
`17.``dcb.Parity=NOPARITY; ``//无奇偶校验位`
`18.``dcb.StopBits=TWOSTOPBITS; ``//两个停止位`
`19.``SetCommState(hCom,&dcb);`
`20.`
`21.``PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);`
在读写串口之前，还要用PurgeComm()函数清空缓冲区，该函数原型：
`1.``BOOL``PurgeComm(`
`2.`
`3.``HANDLE``hFile,   ``//串口句柄`
`4.``DWORD``dwFlags   `
```
//
 需要完成的操作
```
`5.``);  `
参数dwFlags指定要完成的操作，可以是下列值的组合：
`1.`
```
PURGE_TXABORT    
 中断所有写操作并立即返回，即使写操作还没有完成。
```
`2.`
```
PURGE_RXABORT    
 中断所有读操作并立即返回，即使读操作还没有完成。
```
`3.`
```
PURGE_TXCLEAR    
 清除输出缓冲区
```
`4.`
```
PURGE_RXCLEAR    
 清除输入缓冲区
```
（3）、读写串口
我们使用ReadFile和WriteFile读写串口，下面是两个函数的声明：
`01.``BOOL``ReadFile(`
`02.`
`03.``HANDLE``hFile,   ``//串口的句柄`
`04.`
`05.`
```
//
 读入的数据存储的地址，
```
`06.`
```
//
 即读入的数据将存储在以该指针的值为首地址的一片内存区
```
`07.``LPVOID``lpBuffer,   `
`08.``DWORD``nNumberOfBytesToRead, `
```
//
 要读入的数据的字节数
```
`09.`
`10.`
```
//
 指向一个DWORD数值，该数值返回读操作实际读入的字节数
```
`11.``LPDWORD``lpNumberOfBytesRead,   `
`12.`
`13.`
```
//
 重叠操作时，该参数指向一个OVERLAPPED结构，同步操作时，该参数为NULL。
```
`14.`
```
LPOVERLAPPED
 lpOverlapped
```
`15.``);  `
`16.``BOOL``WriteFile(`
`17.`
`18.``HANDLE``hFile,   ``//串口的句柄`
`19.`
`20.`
```
//
 写入的数据存储的地址，
```
`21.`
```
//
 即以该指针的值为首地址的nNumberOfBytesToWrite
```
`22.`
```
//
 个字节的数据将要写入串口的发送数据缓冲区。
```
`23.``LPCVOID``lpBuffer,  `
`24.`
`25.``DWORD``nNumberOfBytesToWrite,    ``//要写入的数据的字节数`
`26.`
`27.`
```
//
 指向指向一个DWORD数值，该数值返回实际写入的字节数
```
`28.``LPDWORD``lpNumberOfBytesWritten,`
`29.`
`30.`
```
//
 重叠操作时，该参数指向一个OVERLAPPED结构，
```
`31.`
```
//
 同步操作时，该参数为NULL。
```
`32.`
```
LPOVERLAPPED
 lpOverlapped
```
`33.``);`
在用ReadFile和WriteFile读写串口时，既可以同步执行，也可以重叠执行。在同步执行时，函数直到操作完成后才返回。这意味着同步执行时线程会被阻塞，从而导致效率下降。在重叠执行时，即使操作还未完成，这两个函数也会立即返回，费时的I/O操作在后台进行。
ReadFile和WriteFile函数是同步还是异步由CreateFile函数决定，如果在调用CreateFile创建句柄时指定了FILE_FLAG_OVERLAPPED标志，那么调用ReadFile和WriteFile对该句柄进行的操作就应该是重叠的；如果未指定重叠标志，则读写操作应该是同步的。ReadFile和WriteFile函数的同步或者异步应该和CreateFile函数相一致。
ReadFile函数只要在串口输入缓冲区中读入指定数量的字符，就算完成操作。而WriteFile函数不但要把指定数量的字符拷入到输出缓冲区，而且要等这些字符从串行口送出去后才算完成操作。
如果操作成功，这两个函数都返回TRUE。需要注意的是，当ReadFile和WriteFile返回FALSE时，不一定就是操作失败，线程应该调用GetLastError函数分析返回的结果。例如，在重叠操作时如果操作还未完成函数就返回，那么函数就返回FALSE，而且GetLastError函数返回ERROR_IO_PENDING。这说明重叠操作还未完成。
同步方式读写串口比较简单，下面先例举同步方式读写串口的代码：
`01.``//同步读串口`
`02.``char``str[100];`
`03.``DWORD``wCount;``//读取的字节数`
`04.``BOOL``bReadStat;`
`05.``bReadStat=ReadFile(hCom,str,100,&wCount,NULL);`
`06.``if``(!bReadStat)`
`07.``{`
`08.``AfxMessageBox(``"读串口失败!"``);`
`09.``return``FALSE;`
`10.``}`
`11.``return``TRUE;`
`12.`
`13.``//同步写串口`
`14.`
`15.``char``lpOutBuffer[100];`
`16.``DWORD``dwBytesWrite=100;`
`17.`
```
COMSTAT
 ComStat;
```
`18.``DWORD``dwErrorFlags;`
`19.``BOOL``bWriteStat;`
`20.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`21.`
```
bWriteStat=WriteFile(hCom,lpOutBuffer,dwBytesWrite,&
 dwBytesWrite,NULL);
```
`22.``if``(!bWriteStat)`
`23.``{`
`24.``AfxMessageBox(``"写串口失败!"``);`
`25.``}`
`26.`
```
PurgeComm(hCom,
 PURGE_TXABORT|
```
`27.``PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);`
在重叠操作时,操作还未完成函数就返回。
重叠I/O非常灵活，它也可以实现阻塞（例如我们可以设置一定要读取到一个数据才能进行到下一步操作）。有两种方法可以等待操作完成：一种方法是用象WaitForSingleObject这样的等待函数来等待OVERLAPPED结构的hEvent成员；另一种方法是调用GetOverlappedResult函数等待，后面将演示说明。
下面我们先简单说一下OVERLAPPED结构和GetOverlappedResult函数：
OVERLAPPED结构
OVERLAPPED结构包含了重叠I/O的一些信息，定义如下：
`1.``typedef``struct`
```
_OVERLAPPED
 {
```
```
//
 o
```
`2.``DWORD``Internal;`
`3.``DWORD``InternalHigh;`
`4.``DWORD``Offset;`
`5.``DWORD``OffsetHigh;`
`6.``HANDLE``hEvent;`
`7.`
```
}
 OVERLAPPED;
```
在使用ReadFile和WriteFile重叠操作时，线程需要创建OVERLAPPED结构以供这两个函数使用。线程通过OVERLAPPED结构获得当前的操作状态，该结构最重要的成员是hEvent。hEvent是读写事件。当串口使用异步通讯时，函数返回时操作可能还没有完成，程序可以通过检查该事件得知是否读写完毕。
当调用ReadFile, WriteFile 函数的时候，该成员会自动被置为无信号状态；当重叠操作完成后，该成员变量会自动被置为有信号状态。
`01.``GetOverlappedResult函数`
`02.``BOOL``GetOverlappedResult(`
`03.``HANDLE``hFile,   `
```
//
 串口的句柄
```
`04.`
`05.`
```
//
 指向重叠操作开始时指定的OVERLAPPED结构
```
`06.`
```
LPOVERLAPPED
 lpOverlapped,
```
`07.`
`08.`
```
//
 指向一个32位变量，该变量的值返回实际读写操作传输的字节数。
```
`09.``LPDWORD``lpNumberOfBytesTransferred,`
`10.`
`11.`
```
//
 该参数用于指定函数是否一直等到重叠操作结束。
```
`12.`
```
//
 如果该参数为TRUE，函数直到操作结束才返回。
```
`13.`
```
//
 如果该参数为FALSE，函数直接返回，这时如果操作没有完成，
```
`14.`
```
//
 通过调用GetLastError()函数会返回ERROR_IO_INCOMPLETE。
```
`15.``BOOL``bWait `
`16.``);  `
该函数返回重叠操作的结果，用来判断异步操作是否完成，它是通过判断OVERLAPPED结构中的hEvent是否被置位来实现的。
异步读串口的示例代码：
`01.``char``lpInBuffer[1024];`
`02.``DWORD``dwBytesRead=1024;`
`03.`
```
COMSTAT
 ComStat;
```
`04.``DWORD``dwErrorFlags;`
`05.`
```
OVERLAPPED
 m_osRead;
```
`06.``memset``(&m_osRead,0,``sizeof``(OVERLAPPED));`
`07.``m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);`
`08.`
`09.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`10.``dwBytesRead=min(dwBytesRead,(``DWORD``)ComStat.cbInQue);`
`11.``if``(!dwBytesRead)`
`12.``return``FALSE;`
`13.``BOOL``bReadStatus;`
`14.``bReadStatus=ReadFile(hCom,lpInBuffer,`
`15.``dwBytesRead,&dwBytesRead,&m_osRead);`
`16.`
`17.``if``(!bReadStatus) ``//如果ReadFile函数返回FALSE`
`18.``{`
`19.``if``(GetLastError()==ERROR_IO_PENDING)`
`20.``//GetLastError()函数返回ERROR_IO_PENDING,表明串口正在进行读操作   `
`21.``{`
`22.``WaitForSingleObject(m_osRead.hEvent,2000);`
`23.``//使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟`
`24.``//当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号`
`25.`
```
PurgeComm(hCom,
 PURGE_TXABORT|
```
`26.``PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);`
`27.``return``dwBytesRead;`
`28.``}`
`29.``return``0;`
`30.``}`
`31.`
```
PurgeComm(hCom,
 PURGE_TXABORT|
```
`32.``PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);`
`33.``return``dwBytesRead;`
对以上代码再作简要说明： 在使用ReadFile 函数进行读操作前，应先使用ClearCommError函数清除错误。ClearCommError函数的原型如下：
`1.``BOOL``ClearCommError(`
`2.`
`3.``HANDLE``hFile,   `
```
//
 串口句柄
```
`4.``LPDWORD``lpErrors,   `
```
//
 指向接收错误码的变量
```
`5.`
```
LPCOMSTAT
 lpStat
```
```
//
 指向通讯状态缓冲区
```
`6.``);  `
该函数获得通信错误并报告串口的当前状态，同时，该函数清除串口的错误标志以便继续输入、输出操作。
参数lpStat指向一个COMSTAT结构，该结构返回串口状态信息。 COMSTAT结构 COMSTAT结构包含串口的信息，结构定义如下：
`01.``typedef``struct`
```
_COMSTAT
 {
```
```
//
 cst
```
`02.``DWORD`
```
fCtsHold
 : 1;
```
```
//
 Tx waiting for CTS signal
```
`03.``DWORD`
```
fDsrHold
 : 1;
```
```
//
 Tx waiting for DSR signal
```
`04.``DWORD`
```
fRlsdHold
 : 1;
```
```
//
 Tx waiting for RLSD signal
```
`05.``DWORD`
```
fXoffHold
 : 1;
```
```
//
 Tx waiting, XOFF char rec''d
```
`06.``DWORD`
```
fXoffSent
 : 1;
```
```
//
 Tx waiting, XOFF char sent
```
`07.``DWORD`
```
fEof
 : 1;
```
```
//
 EOF character sent
```
`08.``DWORD`
```
fTxim
 : 1;
```
```
//
 character waiting for Tx
```
`09.``DWORD`
```
fReserved
 : 25;
```
```
//
 reserved
```
`10.``DWORD``cbInQue;        `
```
//
 bytes in input buffer
```
`11.``DWORD``cbOutQue;       `
```
//
 bytes in output buffer
```
`12.`
```
}
 COMSTAT, *LPCOMSTAT;
```
本文只用到了cbInQue成员变量，该成员变量的值代表输入缓冲区的字节数。
最后用PurgeComm函数清空串口的输入输出缓冲区。
这段代码用WaitForSingleObject函数来等待OVERLAPPED结构的hEvent成员，下面我们再演示一段调用GetOverlappedResult函数等待的异步读串口示例代码：
`01.``char``lpInBuffer[1024];`
`02.``DWORD``dwBytesRead=1024;`
`03.``BOOL``bReadStatus;`
`04.``DWORD``dwErrorFlags;`
`05.`
```
COMSTAT
 ComStat;
```
`06.`
```
OVERLAPPED
 m_osRead;
```
`07.`
`08.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`09.``if``(!ComStat.cbInQue)`
`10.``return``0;`
`11.``dwBytesRead=min(dwBytesRead,(``DWORD``)ComStat.cbInQue);`
`12.`
```
bReadStatus=ReadFile(hCom,
 lpInBuffer,dwBytesRead,
```
`13.``&dwBytesRead,&m_osRead);`
`14.``if``(!bReadStatus) ``//如果ReadFile函数返回FALSE`
`15.``{`
`16.``if``(GetLastError()==ERROR_IO_PENDING)`
`17.``{`
`18.``GetOverlappedResult(hCom,`
`19.``&m_osRead,&dwBytesRead,TRUE);`
`20.`
```
//
 GetOverlappedResult函数的最后一个参数设为TRUE，
```
`21.``//函数会一直等待，直到读操作完成或由于错误而返回。`
`22.`
`23.``return``dwBytesRead;`
`24.``}`
`25.``return``0;`
`26.``}`
`27.``return``dwBytesRead;`
异步写串口的示例代码：
`01.``char``buffer[1024];`
`02.``DWORD``dwBytesWritten=1024;`
`03.``DWORD``dwErrorFlags;`
`04.`
```
COMSTAT
 ComStat;
```
`05.`
```
OVERLAPPED
 m_osWrite;
```
`06.``BOOL``bWriteStat;`
`07.`
`08.``bWriteStat=WriteFile(hCom,buffer,dwBytesWritten,`
`09.``&dwBytesWritten,&m_OsWrite);`
`10.``if``(!bWriteStat)`
`11.``{`
`12.``if``(GetLastError()==ERROR_IO_PENDING)`
`13.``{`
`14.``WaitForSingleObject(m_osWrite.hEvent,1000);`
`15.``return``dwBytesWritten;`
`16.``}`
`17.``return``0;`
`18.``}`
`19.``return``dwBytesWritten;`
（4）、关闭串口
利用API函数关闭串口非常简单，只需使用CreateFile函数返回的句柄作为参数调用CloseHandle即可：
`1.``BOOL``CloseHandle(`
`2.``HANDLE``hObject; `
```
//handle
 to object to close
```
`3.``);`
串口编程的一个实例
为了让您更好地理解串口编程,下面我们分别编写两个例程（见附带的源码部分）,这两个例程都实现了工控机与百特显示仪表通过RS485接口进行的串口通信。其中第一个例程采用同步串口操作,第二个例程采用异步串口操作。
我们只介绍软件部分，RS485接口接线方法不作介绍，感兴趣的读者可以查阅相关资料。
例程1
打开VC++6.0，新建基于对话框的工程RS485Comm，在主对话框窗口IDD_RS485COMM_DIALOG上添加两个按钮，ID分别为IDC_SEND和IDC_RECEIVE，标题分别为“发送”和“接收”；添加一个静态文本框IDC_DISP，用于显示串口接收到的内容。
在RS485CommDlg.cpp文件中添加全局变量：
`1.``HANDLE``hCom;  ``//全局变量，串口句柄`
在RS485CommDlg.cpp文件中的OnInitDialog()函数添加如下代码：
`01.`
```
//
 TODO: Add extra initialization here
```
`02.``hCom=CreateFile(``"COM1"``,``//COM1口`
`03.``GENERIC_READ|GENERIC_WRITE, ``//允许读和写`
`04.``0, ``//独占方式`
`05.``NULL,`
`06.``OPEN_EXISTING, ``//打开而不是创建`
`07.``0, ``//同步方式`
`08.``NULL);`
`09.``if``(hCom==(``HANDLE``)-1)`
`10.``{`
`11.``AfxMessageBox(``"打开COM失败!"``);`
`12.``return``FALSE;`
`13.``}`
`14.`
`15.``SetupComm(hCom,100,100); ``//输入缓冲区和输出缓冲区的大小都是1024`
`16.`
`17.`
```
COMMTIMEOUTS
 TimeOuts;
```
`18.``//设定读超时`
`19.``TimeOuts.ReadIntervalTimeout=MAXDWORD;`
`20.``TimeOuts.ReadTotalTimeoutMultiplier=0;`
`21.``TimeOuts.ReadTotalTimeoutConstant=0;`
`22.``//在读一次输入缓冲区的内容后读操作就立即返回，`
`23.``//而不管是否读入了要求的字符。`
`24.`
`25.`
`26.``//设定写超时`
`27.``TimeOuts.WriteTotalTimeoutMultiplier=100;`
`28.``TimeOuts.WriteTotalTimeoutConstant=500;`
`29.``SetCommTimeouts(hCom,&TimeOuts); ``//设置超时`
`30.`
`31.`
```
DCB
 dcb;
```
`32.``GetCommState(hCom,&dcb);`
`33.``dcb.BaudRate=9600; ``//波特率为9600`
`34.``dcb.ByteSize=8; ``//每个字节有8位`
`35.``dcb.Parity=NOPARITY; ``//无奇偶校验位`
`36.``dcb.StopBits=TWOSTOPBITS; ``//两个停止位`
`37.``SetCommState(hCom,&dcb);`
`38.`
`39.``PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);`
分别双击IDC_SEND按钮和IDC_RECEIVE按钮，添加两个按钮的响应函数：
`01.``void``CRS485CommDlg::OnSend()`
`02.``{`
`03.`
```
//
 TODO: Add your control notification handler code here
```
`04.`
```
//
 在此需要简单介绍百特公司XMA5000的通讯协议：
```
`05.``//该仪表RS485通讯采用主机广播方式通讯。`
`06.``//串行半双工，帧11位，1个起始位(0)，8个数据位，2个停止位(1)`
`07.`
```
//如：读仪表显示的瞬时值，主机发送：DC1
 AAA BB ETX
```
`08.``//其中：DC1是标准ASCII码的一个控制符号，码值为11H(十进制的17)`
`09.``//在XMA5000的通讯协议中，DC1表示读瞬时值`
`10.``//AAA是从机地址码，也就是XMA5000显示仪表的通讯地址`
`11.``//BB为通道号，读瞬时值时该值为01`
`12.``//ETX也是标准ASCII码的一个控制符号，码值为03H`
`13.``//在XMA5000的通讯协议中，ETX表示主机结束符`
`14.`
`15.``char``lpOutBuffer[7];`
`16.``memset``(lpOutBuffer,``''``\0``''``,7); ``//前7个字节先清零`
`17.``lpOutBuffer[0]=``''``\x11``''``;  ``//发送缓冲区的第1个字节为DC1`
`18.``lpOutBuffer[1]=``''``0``''``;  ``//第2个字节为字符0(30H)`
`19.``lpOutBuffer[2]=``''``0``''``; ``//第3个字节为字符0(30H)`
`20.``lpOutBuffer[3]=``''``1``''``; `
```
//
 第4个字节为字符1(31H)
```
`21.``lpOutBuffer[4]=``''``0``''``; ``//第5个字节为字符0(30H)`
`22.``lpOutBuffer[5]=``''``1``''``; ``//第6个字节为字符1(31H)`
`23.``lpOutBuffer[6]=``''``\x03``''``; ``//第7个字节为字符ETX`
`24.``//从该段代码可以看出，仪表的通讯地址为001`
`25.``DWORD``dwBytesWrite=7;`
`26.`
```
COMSTAT
 ComStat;
```
`27.``DWORD``dwErrorFlags;`
`28.``BOOL``bWriteStat;`
`29.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`30.`
```
bWriteStat=WriteFile(hCom,lpOutBuffer,dwBytesWrite,&
 dwBytesWrite,NULL);
```
`31.``if``(!bWriteStat)`
`32.``{`
`33.``AfxMessageBox(``"写串口失败!"``);`
`34.``}`
`35.`
`36.``}`
`37.``void``CRS485CommDlg::OnReceive()`
`38.``{`
`39.`
```
//
 TODO: Add your control notification handler code here
```
`40.`
`41.``char``str[100];`
`42.``memset``(str,``''``\0``''``,100);`
`43.``DWORD``wCount=100;``//读取的字节数`
`44.``BOOL``bReadStat;`
`45.``bReadStat=ReadFile(hCom,str,wCount,&wCount,NULL);`
`46.``if``(!bReadStat)`
`47.``AfxMessageBox(``"读串口失败!"``);`
`48.`
```
PurgeComm(hCom,
 PURGE_TXABORT|
```
`49.``PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);`
`50.``m_disp=str;`
`51.``UpdateData(FALSE);`
`52.`
`53.``}`
您可以观察返回的字符串，其中有和仪表显示值相同的部分，您可以进行相应的字符串操作取出仪表的显示值。
打开ClassWizard,为静态文本框IDC_DISP添加CString类型变量m_disp，同时添加WM_CLOSE的相应函数：
`1.``void``CRS485CommDlg::OnClose()`
`2.``{`
`3.`
```
//
 TODO: Add your message handler code here and/or call default
```
`4.``CloseHandle(hCom);  ``//程序退出时关闭串口`
`5.``CDialog::OnClose();`
`6.``}`
程序的相应部分已经在代码内部作了详细介绍。连接好硬件部分，编译运行程序，细心体会串口同步操作部分。
例程2
打开VC++6.0，新建基于对话框的工程RS485Comm，在主对话框窗口IDD_RS485COMM_DIALOG上添加两个按钮，ID分别为IDC_SEND和IDC_RECEIVE，标题分别为“发送”和“接收”；添加一个静态文本框IDC_DISP，用于显示串口接收到的内容。 在RS485CommDlg.cpp文件中添加全局变量：
`1.``HANDLE``hCom; ``//全局变量，`
串口句柄在RS485CommDlg.cpp文件中的OnInitDialog()函数添加如下代码：
`01.``hCom=CreateFile(``"COM1"``,``//COM1口`
`02.``GENERIC_READ|GENERIC_WRITE, ``//允许读和写`
`03.``0, ``//独占方式`
`04.``NULL,`
`05.``OPEN_EXISTING, ``//打开而不是创建`
`06.``FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED, ``//重叠方式`
`07.``NULL);`
`08.``if``(hCom==(``HANDLE``)-1)`
`09.``{`
`10.``AfxMessageBox(``"打开COM失败!"``);`
`11.``return``FALSE;`
`12.``}`
`13.`
`14.``SetupComm(hCom,100,100); ``//输入缓冲区和输出缓冲区的大小都是100`
`15.`
`16.`
```
COMMTIMEOUTS
 TimeOuts;
```
`17.``//设定读超时`
`18.``TimeOuts.ReadIntervalTimeout=MAXDWORD;`
`19.``TimeOuts.ReadTotalTimeoutMultiplier=0;`
`20.``TimeOuts.ReadTotalTimeoutConstant=0;`
`21.``//在读一次输入缓冲区的内容后读操作就立即返回，`
`22.``//而不管是否读入了要求的字符。`
`23.`
`24.`
`25.``//设定写超时`
`26.``TimeOuts.WriteTotalTimeoutMultiplier=100;`
`27.``TimeOuts.WriteTotalTimeoutConstant=500;`
`28.``SetCommTimeouts(hCom,&TimeOuts); ``//设置超时`
`29.`
`30.`
```
DCB
 dcb;
```
`31.``GetCommState(hCom,&dcb);`
`32.``dcb.BaudRate=9600; ``//波特率为9600`
`33.``dcb.ByteSize=8; ``//每个字节有8位`
`34.``dcb.Parity=NOPARITY; ``//无奇偶校验位`
`35.``dcb.StopBits=TWOSTOPBITS; ``//两个停止位`
`36.``SetCommState(hCom,&dcb);`
`37.`
`38.``PurgeComm(hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);`
分别双击IDC_SEND按钮和IDC_RECEIVE按钮，添加两个按钮的响应函数：
`01.``void``CRS485CommDlg::OnSend()`
`02.``{`
`03.`
```
//
 TODO: Add your control notification handler code here
```
`04.`
```
OVERLAPPED
 m_osWrite;
```
`05.``memset``(&m_osWrite,0,``sizeof``(OVERLAPPED));`
`06.``m_osWrite.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);`
`07.`
`08.`
`09.``char``lpOutBuffer[7];`
`10.``memset``(lpOutBuffer,``''``\0``''``,7);`
`11.``lpOutBuffer[0]=``''``\x11``''``;`
`12.``lpOutBuffer[1]=``''``0``''``;`
`13.``lpOutBuffer[2]=``''``0``''``;`
`14.``lpOutBuffer[3]=``''``1``''``;`
`15.``lpOutBuffer[4]=``''``0``''``;`
`16.``lpOutBuffer[5]=``''``1``''``;`
`17.``lpOutBuffer[6]=``''``\x03``''``;`
`18.`
`19.``DWORD``dwBytesWrite=7;`
`20.`
```
COMSTAT
 ComStat;
```
`21.``DWORD``dwErrorFlags;`
`22.``BOOL``bWriteStat;`
`23.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`24.``bWriteStat=WriteFile(hCom,lpOutBuffer,`
`25.`
```
dwBytesWrite,&
 dwBytesWrite,&m_osWrite);
```
`26.`
`27.``if``(!bWriteStat)`
`28.``{`
`29.``if``(GetLastError()==ERROR_IO_PENDING)`
`30.``{`
`31.``WaitForSingleObject(m_osWrite.hEvent,1000);`
`32.``}`
`33.``}`
`34.`
`35.``}`
`36.`
`37.``void``CRS485CommDlg::OnReceive()`
`38.``{`
`39.`
```
//
 TODO: Add your control notification handler code here
```
`40.`
```
OVERLAPPED
 m_osRead;
```
`41.``memset``(&m_osRead,0,``sizeof``(OVERLAPPED));`
`42.``m_osRead.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);`
`43.`
`44.`
```
COMSTAT
 ComStat;
```
`45.``DWORD``dwErrorFlags;`
`46.`
`47.``char``str[100];`
`48.``memset``(str,``''``\0``''``,100);`
`49.``DWORD``dwBytesRead=100;``//读取的字节数`
`50.``BOOL``bReadStat;`
`51.`
`52.``ClearCommError(hCom,&dwErrorFlags,&ComStat);`
`53.`
```
dwBytesRead=min(dwBytesRead,
 (
```
`DWORD``)ComStat.cbInQue);`
`54.``bReadStat=ReadFile(hCom,str,`
`55.``dwBytesRead,&dwBytesRead,&m_osRead);`
`56.``if``(!bReadStat)`
`57.``{`
`58.``if``(GetLastError()==ERROR_IO_PENDING)`
`59.``//GetLastError()函数返回ERROR_IO_PENDING,表明串口正在进行读操作`
`60.``{`
`61.``WaitForSingleObject(m_osRead.hEvent,2000);`
`62.``//使用WaitForSingleObject函数等待，直到读操作完成或延时已达到2秒钟`
`63.``//当串口读操作进行完毕后，m_osRead的hEvent事件会变为有信号`
`64.``}`
`65.``}`
`66.`
`67.`
```
PurgeComm(hCom,
 PURGE_TXABORT|
```
`68.``PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);`
`69.``m_disp=str;`
`70.``UpdateData(FALSE);`
`71.``}`
打开ClassWizard,为静态文本框IDC_DISP添加CString类型变量m_disp，同时添加WM_CLOSE的相应函数：
[view
 source](http://www.vckbase.com/index.php/wv/1439#viewSource)
[print](http://www.vckbase.com/index.php/wv/1439#printSource)[?](http://www.vckbase.com/index.php/wv/1439#about)
`1.``void``CRS485CommDlg::OnClose()`
`2.``{`
`3.`
```
//
 TODO: Add your message handler code here and/or call default
```
`4.``CloseHandle(hCom);  ``//程序退出时关闭串口`
`5.``CDialog::OnClose();`
`6.``}`
您可以仔细对照这两个例程，细心体会串口同步操作和异步操作的区别。
好了，就到这吧，祝您好运。
//===================================================
备注::
1.串口的操作可以有两种操作方式：同步操作方式和重叠操作方式（又称为异步操作方式）。同步操作时，API函数会阻塞直到操作完成以后才能返回（在多线程方式中，虽然不会阻塞主线程，但是仍然会阻塞监听线程）；而重叠操作方式，API函数会立即返回，操作在后台进行，避免线程的阻塞。
2.怎么都不见WaitCommEvent函数的使用？？？
3.怎么在一些书籍上看到在什么NT内核的WINDOWS系统上不支持串口的同步操作方式
4.我的ClearCommError函数老是获不得正确的数据个数，不知道为什么，不知道那里没设置好
5.串口通信出错网帖
   1》串口收数据丢失----[http://bbs.csdn.net/topics/120014380](http://bbs.csdn.net/topics/120014380)
   2》串口通信接收数据丢失----[http://bbs.csdn.net/topics/70081340](http://bbs.csdn.net/topics/70081340)
3》MSDN Serial
 Communications----[http://msdn.microsoft.com/en-us/library/ff802693.aspx](http://msdn.microsoft.com/en-us/library/ff802693.aspx)
 4》ClearCommError函数问题----[http://bbs.csdn.net/topics/390147651?page=1#post-394097054](http://bbs.csdn.net/topics/390147651?page=1#post-394097054)
