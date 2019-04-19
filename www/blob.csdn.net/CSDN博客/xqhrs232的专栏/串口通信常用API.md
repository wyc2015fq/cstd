# 串口通信常用API - xqhrs232的专栏 - CSDN博客
2013年04月02日 14:04:50[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：794
原文地址::[http://blog.csdn.net/vodomine/article/details/6542089](http://blog.csdn.net/vodomine/article/details/6542089)
1-CreateFile() 
用途：打开串口 
原型：HANDLE CreateFile(LPCTSTR lpFileName, 
DWORD dwDesired[Access](http://www.hur.cn/db/Access/Index.html), 
DWORD dwShareMode, 
LPSECUR[IT](http://www.hur.cn/)Y_ATTRIBUTES
 lpSecurityAttributes, 
DWORD dwCreationDistribution, 
DWORD dwFlagsAndAttributes, 
HANDLE hTemplateFile); 
参数说明： 
-lpFileName：要打开的文件名称。对串口通信来说就是COM1或COM2。 
-dwDesired[Access](http://www.hur.cn/db/Access/Index.html):读写模式设置。此处应该用GENERIC_READ及GENERIC_WR[IT](http://www.hur.cn/)E。 
-dwShareMode：串口共享模式。此处不允许其他应用程序共享，应为0。 
-lpSecurityAttributes：串口的安全属性，应为0，表示该串口不可被子程序继承。 
-dwCreationDistribution：创建文件的性质，此处为OPEN_EXISTING. 
-dwFlagsAndAttributes：属性及相关标志，这里使用异步方式应该用FILE_FLAG_OVERLAPPED。 
-hTemplateFile:此处为0。 
操作说明：若文件打开成功，串口即可使用了，该函数返回串口的句柄，以后对串口操作时 
即可使用该句柄。 
举例：HANDLE hComm; 
hComm=CreateFile("COM1", //串口号 
GENERIC_READ|GENERIC_WR[IT](http://www.hur.cn/)E,
 //允许读写 
0, //通讯设备必须以独占方式打开 
NULL, //无安全属性 
OPEN_EXISTING, //通讯设备已存在 
FILE_FLAG_OVERLAPPED, //异步I/O 
0); //通讯设备不能用模板打开 
hComm即为函数返回的串口1的句柄。
**2-CloseHandle() **
用途：关闭串口 
原型：BOOL CloseHandle(HANDLE hObjedt) 
参数说明： 
-hObjedt：串口句柄 
操作说明：成功关闭串口时返回true,否则返回false 
举例：CloseHandle(hComm); 
3-GetCommState() 
用途：取得串口当前状态 
原型：BOOL GetCommState(HANDLE hFile, 
LPDCB lpDCB); 
参数说明： 
-hFile：串口句柄 
-lpDCB：设备控制块(Device Control Block)结构地址。此结构中含有和设备相关的 
参数。此处是与串口相关的参数。由于参数非常多，当需要设置串口参数 
时，通常是先取得串口的参数结构，修改部分参数后再将参数结构写入。 
在此仅介绍少数的几个常用的参数： 
DWORD BaudRate：串口波特率 
DWORD fParity：为1的话激活奇偶校验检查 
DWORD Parity：校验方式，值0~4分别对应无校验、奇校验、偶校验、校验 
置位、校验清零 
DWORD ByteSize：一个字节的数据位个数，范围是5~8 
DWORD StopBits：停止位个数，0~2分别对应1位、1.5位、2位停止位 
操作举例：DCB ComDCB; //串口设备控制块 
GetCommState(hComm,&ComDCB); 
4-SetCommState() 
用途：设置串口状态，包括常用的更改串口号、波特率、奇偶校验方式、数据位数等 
原型：BOOL SetCommState(HANDLE hFile, 
LPDCB lpDCB); 
参数说明： 
-hFile:串口句柄 
-lpDCB：设备控制块(Device Control Block)结构地址。要更改的串口参数包含在此结构中。 
操作举例：DCB ComDCB; 
GetCommState(hComm,&ComDCB);//取得当前串口状态 
ComDCB.BaudRate=9600;//更改为9600bps,该值即为你要修改后的波特率 
SetCommState(hComm,&ComDCB;//将更改后的参数写入串口 
5-WriteFile() 
用途：向串口写数据 
原型：BOOL WriteFile(HANDLE hFile, 
LPCVOID lpBuffer, 
DWORD nNumberOfBytesToWrite, 
LPDWORD lpNumberOfBytesWritten, 
LPOVERLAPPED lpOverlapped); 
参数说明： 
-hFile：串口句柄 
-lpBuffer：待写入数据的首地址 
-nNumberOfBytesToWrite：待写入数据的字节数长度 
-lpNumberOfBytesWritten：函数返回的实际写入串口的数据个数的地址，利用此变量可判断 
实际写入的字节数和准备写入的字节数是否相同。 
-lpOverlapped:重叠I/O结构的指针 
操作举例：DWORD BytesSent=0; 
unsigned char SendBytes[5]={1,2,3,4,5}; 
OVERLAPPED ov_Write; 
ov_Write.Offset=0; 
ov_Write.OffsetHigh=0; 
WriteFile(hComm, //调用成功返回非零，失败返回零 
SendBytes, //输出缓冲区 
5, //准备发送的字符长度 
&BytesSent, //实际发出的字符数 
&ov_Write); //重叠结构 
如果函数执行成功的话检查BytesSent的值应该为5，此函数是WriteFile函数执行完毕后 
自行填充的，利用此变量的填充值可以用来检查该函数是否将所有的数据成功写入串口
**6-ReadFile() **
用途：读串口数据 
原型：BOOL ReadFile(HANDLE hFile, 
LPVOID lpBuffer, 
DWORD nNumberOfBytesToRead, 
lpNumberOfBytesRead, 
lpOverlapped); 
参数说明： 
-hFile：串口句柄 
-lpBuffer：存储被读出数据的首地址 
-nNumberOfBytesToRead：准备读出的字节个数 
-NumberOfBytesRead：实际读出的字节个数 
-lpOverlapped：异步I/O结构， 
操作举例：unsigned char ucRxBuff[20]; 
COMSTAT ComStat; 
DWORD dwError=0; 
DWORD BytesRead=0; 
OVERLAPPED ov_Read; 
ov_Read.hEvent=CreateEvent(NULL, true, false, NULL);//必须创建有效事件 
ClearCommError(hComm,&dwError,&ComStat);//检查串口接收缓冲区中的数据个数 
bResult=ReadFile(hComm, //串口句柄 
ucRxBuff, //输入缓冲区地址 
ComStat.cbInQue, //想读入的字符数 
&BytesRead, //实际读出的字节数的变量指针 
&ov_Read); //重叠结构指针 
假如当前串口中有5个字节数据的话，那么执行完ClearCommError()函数后，ComStat 
结构中的ComStat.cbInQue将被填充为5，此值在ReadFile函数中可被直接利用。 
7-ClearCommError() 
用途：清除串口错误或者读取串口现在的状态 
原型：BOOL ClearCommError(HANDLE hFile, 
LPDWORD lpErrors, 
LPCOMATAT lpStat 
); 
参数说明: 
-hFile:串口句柄 
-lpErrors：返回错误数值，错误常数如下： 
1-CE_BREAK:检测到中断信号。意思是说检测到某个字节数据缺少合法的停止位。 
2-CE_FRAME:硬件检测到帧错误。 
3-CE_IOE:通信设备发生输入/输出错误。 
4-CE_MODE:设置模式错误，或是hFile值错误。 
5-CE_OVERRUN:溢出错误，缓冲区容量不足，数据将丢失。 
6-CE_RXOVER:溢出错误。 
7-CE_RXPAR[IT](http://www.hur.cn/)Y:硬件检查到校验位错误。 
8-CE_TXFULL:发送缓冲区已满。 
-lpStat:指向通信端口状态的结构变量，原型如下： 
typedef struct _COMSTAT{ 
... 
... 
DWORD cbInQue; //输入缓冲区中的字节数 
DWORD cbOutQue;//输出缓冲区中的字节数 
}COMSTAT,*LPCOMSTAT; 
该结构中对我们很重要的只有上面两个参数，其他的我们可以不用管。 
操作举例：COMSTAT ComStat; 
DWORD dwError=0; 
ClearCommError(hComm,&dwError,&ComStat); 
上式执行完后，ComStat.cbInQue就是串口中当前含有的数据字节个数，我们利用此 
数值就可以用ReadFile()函数去读串口中的数据了。
8-PurgeComm() 
用途：清除串口缓冲区 
原型：BOOL PurgeComm(HANDLE hFile, 
DWORD dwFlags 
); 
参数说明： 
-hFile:串口句柄 
-dwFlags：指定串口执行的动作，由以下参数组成： 
-PURGE_TXABORT:停止目前所有的传输工作立即返回不管是否完成传输动作。 
-PURGE_RXABORT:停止目前所有的读取工作立即返回不管是否完成读取动作。 
-PURGE_TXCLEAR:清除发送缓冲区的所有数据。 
-PURGE_RXCLEAR:清除接收缓冲区的所有数据。 
操作举例：PurgeComm(hComm, PURGE_RXCLEAR|PURGE_TXCLEAR|PURGE_RXABORT|PURGE_TXABORT); 
清除串口的所有操作。 
9-SetCommMask() 
用途：设置串口通信事件。 
原型：BOOL SetCommMask(HANDLE hFile, 
DWORD dwEvtMask 
); 
参数说明： 
-hFile：串口句柄 
-dwEvtMask：准备监视的串口事件掩码 
注：在用api函数撰写串口通信函数时大体上有两种方法，一种是查寻法，另外一种是事件通知法。 
这两种方法的区别在于收串口数据时，前一种方法是主动的周期性的查询串口中当前有没有 
数据；后一种方法是事先设置好需要监视的串口通信事件，然后依靠单独开设的辅助线程进行 
监视该事件是否已发生，如果没有发生的话该线程就一直不停的等待直到该事件发生后，将 
该串口事件以消息的方式通知主窗体，然后主窗体收到该消息后依据不同的事件性质进行处理。 
比如说当主窗体收到监视线程发来的RX_CHAR(串口中有数据)的消息后，就可以用ReadFile() 
函数去读串口。该参数有如下信息掩码位值： 
EV_BREAK:收到BREAK信号 
EV_CTS:CTS(clear to send)线路发生变化 
EV_DSR:DST(Data Set Ready)线路发生变化 
EV_ERR:线路状态错误，包括了CE_FRAME/CE_OVERRUN/CE_RXPAR[IT](http://www.hur.cn/)Y
 3钟错误。 
EV_RING:检测到振铃信号。 
EV_RLSD:CD(Carrier Detect)线路信号发生变化。 
EV_RXCHAR:输入缓冲区中已收到数据。 
EV_RXFLAG:使用SetCommState()函数设置的DCB结构中的等待字符已被传入输入缓冲区中。 
EV_TXEMPTY:输出缓冲区中的数据已被完全送出。 
操作举例：SetCommMask(hComm,EV_RXCHAR|EV_TXEMPTY); 
上面函数执行完毕后将监视串口中有无数据和发送缓冲区中的数据是否全部发送完毕。 
10-WaitCommEvent() 
用途：用来判断用SetCommMask()函数设置的串口通信事件是否已发生。 
原型：BOOL WaitCommEvent(HANDLE hFile, 
LPDWORD lpEvtMask, 
LPOVERLAPPED lpOverlapped 
); 
参数说明： 
-hFile：串口句柄 
-lpEvtMask:函数执行完后如果检测到串口通信事件的话就将其写入该参数中。 
-lpOverlapped：异步结构，用来保存异步操作结果。 
操作举例：OVERLAPPED os; 
DWORD dwMask,dwTrans,dwError=0,err; 
memset(&os,0,sizeof(OVERLAPPED)); 
os.hEvent=CreateEvent(NULL,TRUE,FALSE,NULL); 
if(!WaitCommEvent(hComm,&dwMask,&os)){ 
//如果异步操作不能立即完成的话,函数返回FALSE,并且调用GetLastError()函 
//数分析错误原因后返回ERROR_IO_PENDING,指示异步操作正在后台进行.这种情 
//况下,在函数返回之前系统设置OVERLAPPED结构中的事件为无信号状态,该函数 
//等待用SetCommMask()函数设置的串口事件发生，共有9种事件可被监视： 
//EV_BREAK，EV_CTS，EV_DSR，EV_ERR，EV_RING，EV_RLSD，EV_RXCHAR， 
//EV_RXFLAG，EV_TXEMPTY；当其中一个事件发生或错误发生时，函数将 
//OVERLAPPED结构中的事件置为有信号状态，并将事件掩码填充到dwMask参数中 
if(GetLastError()==ERROR_IO_PENDING){ 
/**************************************************************/ 
/*在此等待异步操作结果,直到异步操作结束时才返回.实际上此时 */ 
/*WaitCommEvent()函数一直在等待串口监控的事件之一发生,当事件发*/ 
/*生时该函数将OVERLAPPED结构中的事件句柄置为有信号状态,此时 */ 
/*GetOverlappedResult()函数发现此事件有信号后马上返回,然后下面*/ 
/*的程序马上分析WaitCommEvent()函数等到的事件是被监视的串口事 */ 
/*件中的哪一个,然后执行相应的动作并发出相应消息. */ 
/**************************************************************/ 
GetOverlappedResult(hComm,&os,&dwTrans,true); 
switch(dwMask){ 
case EV_RXCHAR: 
PostMessage(Parent,WM_COMM_RXCHAR,0,0); 
break; 
case EV_TXEMPTY: 
PostMessage(Parent,WM_COMM_TXEMPTY,0,0); 
break; 
case EV_ERR: 
switch(dwError){ 
case CE_FRAME: 
err=0; 
break; 
case CE_OVERRUN: 
err=1; 
break; 
case CE_RXPAR[IT](http://www.hur.cn/)Y: 
err=2; 
break; 
default:break; 
} 
PostMessage(Parent,WM_COMM_ERR,(WPARAM)0,(LPARAM)err); 
break; 
case EV_BREAK: 
PostMessage(Parent,WM_COMM_BREAK,0,0); 
break; 
case ...://其他用SetCommMask()函数设置的被监视的串口通信事件。 
... ... 
break; 
default:break; 
} 
} 
