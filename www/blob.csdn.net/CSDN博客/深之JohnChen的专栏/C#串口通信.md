# C#串口通信 - 深之JohnChen的专栏 - CSDN博客

2011年09月04日 23:40:28[byxdaz](https://me.csdn.net/byxdaz)阅读数：32437


本文主要是讲述如何通过C# .net1.1平台调用API操作串口，.net2.0已经内置串口操作类！ 

通常，在C#中实现串口通信，我们有四种方法： 

第一：通过MSCOMM[控件](http://www.programbbs.com/com/)这是最简单的，最方便的方法。可功能上很难做到控制自如，同时这个[控件](http://www.programbbs.com/com/)并不是系统本身所带，所以还得注册。可以访问 
[http://www.devhood.com/tutorials/tutorial_details.aspx?tutorial_id=320](http://www.devhood.com/tutorials/tutorial_details.aspx?tutorial_id=320)
一个外国人写的[教程](http://www.programbbs.com/tech/)
第二：微软在.NET新推出了一个串口[控件](http://www.programbbs.com/com/)，基于.NET的P/Invoke调用方法实现，详细的可以访问微软网站 
Serial Comm 
Use P/Invoke to Develop a .NET Base Class Library for Serial Device Communications 

[http://msdn.microsoft.com/msdnmag/issues/02/10/netserialcomm/](http://msdn.microsoft.com/msdnmag/issues/02/10/netserialcomm/)
第三：就是用第三方[控件](http://www.programbbs.com/com/)啦，可一般都要付费的，不太合实际，何况楼主不喜欢，不作考虑 
第四：自己用API写串口通信，这样难度高点，但对于我们来说，可以方便实现自己想要的各种功能。 

我们采用第四种方法来实现串口通信，用现成的已经封装好的类库，常见两个串口操作类是JustinIO和SerialStreamReader。介绍JustinIO的使用方法： 

打开串口： 

函数原型：public void Open() 

说明：打开事先设置好的端口 

示例： 

```
using JustinIO; 

static JustinIO.CommPort ss_port = new JustinIO.CommPort(); 
ss_port.PortNum = COM1; //端口号 
ss_port.BaudRate = 19200; //串口通信波特率 
ss_port.ByteSize = 8; //数据位 
ss_port.Parity = 0; //奇偶校验 
ss_port.StopBits = 1;//停止位 
ss_port.ReadTimeout = 1000; //读超时 
try 
{ 
if (ss_port.Opened) 
{ 
ss_port.Close(); 
ss_port.Open(); //打开串口 
} 
else 
{ 
ss_port.Open();//打开串口 
} 
return true; 
} 
catch(Exception e) 
{ 
MessageBox.Show("错误：" + e.Message); 
return false; 
}
```

写串口： 

函数原型：public void Write(byte[] WriteBytes) 

WriteBytes 就是你的写入的字节，注意，字符串要转换成字节数组才能进行通信 

示例： 

ss_port.Write(Encoding.ASCII.GetBytes("AT+CGMI\r")); //获取手机品牌 

读串口： 

函数原型：public byte[] Read(int NumBytes) 

NumBytes 读入缓存数，注意读取来的是字节数组，要实际应用中要进行字符转换 

示例： 

string response = Encoding.ASCII.GetString(ss_port.Read(128)); //读取128个字节缓存 

关闭串口： 

函数原型：ss_port.Close() 

示例： 

ss_port.Close(); 

整合代码： 

```
using System; 
using System.Runtime.InteropServices; 

namespace JustinIO { 
class CommPort { 

public int PortNum; 
public int BaudRate; 
public byte ByteSize; 
public byte Parity; // 0-4=no,odd,even,mark,space 
public byte StopBits; // 0,1,2 = 1, 1.5, 2 
public int ReadTimeout; 

//comm port win32 file handle 
private int hComm = -1; 

public bool Opened = false; 

//win32 api constants 
private const uint GENERIC_READ = 0x80000000; 
private const uint GENERIC_WRITE = 0x40000000; 
private const int OPEN_EXISTING = 3; 
private const int INVALID_HANDLE_VALUE = -1; 

[StructLayout(LayoutKind.Sequential)] 
public struct DCB { 
//taken from c struct in platform sdk 
public int DCBlength; // sizeof(DCB) 
public int BaudRate; // current baud rate 
/* these are the c struct bit fields, bit twiddle flag to set 
public int fBinary; // binary mode, no EOF check 
public int fParity; // enable parity checking 
public int fOutxCtsFlow; // CTS output flow control 
public int fOutxDsrFlow; // DSR output flow control 
public int fDtrControl; // DTR flow control type 
public int fDsrSensitivity; // DSR sensitivity 
public int fTXContinueOnXoff; // XOFF continues Tx 
public int fOutX; // XON/XOFF out flow control 
public int fInX; // XON/XOFF in flow control 
public int fErrorChar; // enable error replacement 
public int fNull; // enable null stripping 
public int fRtsControl; // RTS flow control 
public int fAbortOnError; // abort on error 
public int fDummy2; // reserved 
*/ 
public uint flags; 
public ushort wReserved; // not currently used 
public ushort XonLim; // transmit XON threshold 
public ushort XoffLim; // transmit XOFF threshold 
public byte ByteSize; // number of bits/byte, 4-8 
public byte Parity; // 0-4=no,odd,even,mark,space 
public byte StopBits; // 0,1,2 = 1, 1.5, 2 
public char XonChar; // Tx and Rx XON character 
public char XoffChar; // Tx and Rx XOFF character 
public char ErrorChar; // error replacement character 
public char EofChar; // end of input character 
public char EvtChar; // received event character 
public ushort wReserved1; // reserved; do not use 
} 

[StructLayout(LayoutKind.Sequential)] 
private struct COMMTIMEOUTS { 
public int ReadIntervalTimeout; 
public int ReadTotalTimeoutMultiplier; 
public int ReadTotalTimeoutConstant; 
public int WriteTotalTimeoutMultiplier; 
public int WriteTotalTimeoutConstant; 
} 

[StructLayout(LayoutKind.Sequential)] 
private struct OVERLAPPED { 
public int Internal; 
public int InternalHigh; 
public int Offset; 
public int OffsetHigh; 
public int hEvent; 
} 

[DllImport("kernel32.dll")] 
private static extern int CreateFile( 
string lpFileName, // file name 
uint dwDesiredAccess, // access mode 
int dwShareMode, // share mode 
int lpSecurityAttributes, // SD 
int dwCreationDisposition, // how to create 
int dwFlagsAndAttributes, // file attributes 
int hTemplateFile // handle to template file 
); 
[DllImport("kernel32.dll")] 
private static extern bool GetCommState( 
int hFile, // handle to communications device 
ref DCB lpDCB // device-control block 
); 
[DllImport("kernel32.dll")] 
private static extern bool BuildCommDCB( 
string lpDef, // device-control string 
ref DCB lpDCB // device-control block 
); 
[DllImport("kernel32.dll")] 
private static extern bool SetCommState( 
int hFile, // handle to communications device 
ref DCB lpDCB // device-control block 
); 
[DllImport("kernel32.dll")] 
private static extern bool GetCommTimeouts( 
int hFile, // handle to comm device 
ref COMMTIMEOUTS lpCommTimeouts // time-out values 
); 
[DllImport("kernel32.dll")] 
private static extern bool SetCommTimeouts( 
int hFile, // handle to comm device 
ref COMMTIMEOUTS lpCommTimeouts // time-out values 
); 
[DllImport("kernel32.dll")] 
private static extern bool ReadFile( 
int hFile, // handle to file 
byte[] lpBuffer, // data buffer 
int nNumberOfBytesToRead, // number of bytes to read 
ref int lpNumberOfBytesRead, // number of bytes read 
ref OVERLAPPED lpOverlapped // overlapped buffer 
); 
[DllImport("kernel32.dll")] 
private static extern bool WriteFile( 
int hFile, // handle to file 
byte[] lpBuffer, // data buffer 
int nNumberOfBytesToWrite, // number of bytes to write 
ref int lpNumberOfBytesWritten, // number of bytes written 
ref OVERLAPPED lpOverlapped // overlapped buffer 
); 
[DllImport("kernel32.dll")] 
private static extern bool CloseHandle( 
int hObject // handle to object 
); 
[DllImport("kernel32.dll")] 
private static extern uint GetLastError(); 

public void Open() { 

DCB dcbCommPort = new DCB(); 
COMMTIMEOUTS ctoCommPort = new COMMTIMEOUTS(); 

// OPEN THE COMM PORT. 

hComm = CreateFile("COM" + PortNum ,GENERIC_READ | GENERIC_WRITE,0, 0,OPEN_EXISTING,0,0); 

// IF THE PORT CANNOT BE OPENED, BAIL OUT. 
if(hComm == INVALID_HANDLE_VALUE) { 
throw(new ApplicationException("Comm Port Can Not Be Opened")); 
} 

// SET THE COMM TIMEOUTS. 

GetCommTimeouts(hComm,ref ctoCommPort); 
ctoCommPort.ReadTotalTimeoutConstant = ReadTimeout; 
ctoCommPort.ReadTotalTimeoutMultiplier = 0; 
ctoCommPort.WriteTotalTimeoutMultiplier = 0; 
ctoCommPort.WriteTotalTimeoutConstant = 0; 
SetCommTimeouts(hComm,ref ctoCommPort); 

// SET BAUD RATE, PARITY, WORD SIZE, AND STOP BITS. 
GetCommState(hComm, ref dcbCommPort); 
dcbCommPort.BaudRate=BaudRate; 
dcbCommPort.flags=0; 
//dcb.fBinary=1; 
dcbCommPort.flags|=1; 
if (Parity>0) 
{ 
//dcb.fParity=1 
dcbCommPort.flags|=2; 
} 
dcbCommPort.Parity=Parity; 
dcbCommPort.ByteSize=ByteSize; 
dcbCommPort.StopBits=StopBits; 
if (!SetCommState(hComm, ref dcbCommPort)) 
{ 
//uint ErrorNum=GetLastError(); 
throw(new ApplicationException("Comm Port Can Not Be Opened")); 
} 
//unremark to see if setting took correctly 
//DCB dcbCommPort2 = new DCB(); 
//GetCommState(hComm, ref dcbCommPort2); 
Opened = true; 

} 

public void Close() { 
if (hComm!=INVALID_HANDLE_VALUE) { 
CloseHandle(hComm); 
} 
} 
public byte[] Read(int NumBytes) { 
byte[] BufBytes; 
byte[] OutBytes; 
BufBytes = new byte[NumBytes]; 
if (hComm!=INVALID_HANDLE_VALUE) { 
OVERLAPPED ovlCommPort = new OVERLAPPED(); 
int BytesRead=0; 
ReadFile(hComm,BufBytes,NumBytes,ref BytesRead,ref ovlCommPort); 
OutBytes = new byte[BytesRead]; 
Array.Copy(BufBytes,OutBytes,BytesRead); 
} 
else { 
throw(new ApplicationException("Comm Port Not Open")); 
} 
return OutBytes; 
} 

public void Write(byte[] WriteBytes) { 
if (hComm!=INVALID_HANDLE_VALUE) { 
OVERLAPPED ovlCommPort = new OVERLAPPED(); 
int BytesWritten = 0; 
WriteFile(hComm,WriteBytes,WriteBytes.Length,ref BytesWritten,ref ovlCommPort); 
} 
else { 
throw(new ApplicationException("Comm Port Not Open")); 
} 
} 
} 

} 
}
```

[**C#中使用SerialPort类实现串口通信**](http://blog.csdn.net/cy757/article/details/4474930)

 在.NET Framework 2.0中提供了SerialPort类，该类主要实现串口数据通信等。本文章将本人在学习过程中从网络上搜集到的相关信息写出来供大家参考。

下面主要介绍该类的主要属性（表1）和方法（表.2）。

如果需要了解更多的信息请登录[http://msdn.microsoft.com/zh-cn/library/system.io.ports.serialport(VS.80).aspx](http://msdn.microsoft.com/zh-cn/library/system.io.ports.serialport(VS.80).aspx)查看。

**相关文章**

《使用System.IO.Ports读取COM口数据》

[http://www.devasp.net/net/articles/display/727.html](http://www.devasp.net/net/articles/display/727.html)

表1                                                      SerialPort类的常用属性

|名　　称|说　　明|
|----|----|
|BaseStream|获取 SerialPort 对象的基础 Stream 对象|
|BaudRate|获取或设置串行波特率|
|BreakState|获取或设置中断信号状态|
|BytesToRead|获取接收缓冲区中数据的字节数|
|BytesToWrite|获取发送缓冲区中数据的字节数|
|CDHolding|获取端口的载波检测行的状态|
|CtsHolding|获取“可以发送”行的状态|
|DataBits|获取或设置每个字节的标准数据位长度|
|DiscardNull|获取或设置一个值，该值指示 Null 字节在端口和接收缓冲区之间传输时是否被忽略|
|DsrHolding|获取数据设置就绪 (DSR) 信号的状态|
|DtrEnable|获取或设置一个值，该值在串行通信过程中启用数据终端就绪 (DTR) 信号|
|Encoding|获取或设置传输前后文本转换的字节编码|
|Handshake|获取或设置串行端口数据传输的握手协议|
|IsOpen|获取一个值，该值指示 SerialPort 对象的打开或关闭状态|
|NewLine|获取或设置用于解释 ReadLine( )和WriteLine( )方法调用结束的值|
|Parity|获取或设置奇偶校验检查协议|
续表

|名　　称|说　　明|
|----|----|
|ParityReplace|获取或设置一个字节，该字节在发生奇偶校验错误时替换数据流中的无效字节|
|PortName|获取或设置通信端口，包括但不限于所有可用的 COM 端口|
|ReadBufferSize|获取或设置 SerialPort 输入缓冲区的大小|
|ReadTimeout|获取或设置读取操作未完成时发生超时之前的毫秒数|
|ReceivedBytesThreshold|获取或设置 DataReceived 事件发生前内部输入缓冲区中的字节数|
|RtsEnable|获取或设置一个值，该值指示在串行通信中是否启用请求发送 (RTS) 信号|
|StopBits|获取或设置每个字节的标准停止位数|
|WriteBufferSize|获取或设置串行端口输出缓冲区的大小|
|WriteTimeout|获取或设置写入操作未完成时发生超时之前的毫秒数|
表2                                                     SerialPort类的常用方法

|方 法 名 称|说　　明|
|----|----|
|Close|关闭端口连接，将 IsOpen 属性设置为False，并释放内部 Stream 对象|
|Open|打开一个新的串行端口连接|
|Read|从 SerialPort 输入缓冲区中读取|
|ReadByte|从 SerialPort 输入缓冲区中同步读取一个字节|
|ReadChar|从 SerialPort 输入缓冲区中同步读取一个字符|
|ReadLine|一直读取到输入缓冲区中的 NewLine 值|
|ReadTo|一直读取到输入缓冲区中指定 value 的字符串|
|Write|已重载。将数据写入串行端口输出缓冲区|
|WriteLine|将指定的字符串和 NewLine 值写入输出缓冲区|

**使用SerialPort类的方法：**

**方法一：**

首先要添加

using System.IO;
using System.IO.Ports;

1...在类的内部定义SerialPort com;

2...打开串口

            com = new SerialPort();
            com.BaudRate = 115200;
            com.PortName = "COM1";
            com.DataBits = 8;
            com.Open();//打开串口

3...发送数据

            Byte[] TxData ={1,2,3,4,5,6,7,8 };
            com.Write(TxData, 0, 8);

4...接收数据

     4.1使用事件接收

     this.com.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.OnDataReceived);

private void OnDataReceived(object sender, SerialDataReceivedEventArgs e)

    4.2使用线程接收

     接收数据启动一个线程，使其接收。

在类的内部定义

        Thread _readThread;
        bool _keepReading;

打开串口后启动线程

            _keepReading = true;
            _readThread = new Thread(ReadPort);
            _readThread.Start();

线程函数

```
private void ReadPort()
        {
            while (_keepReading)
            {
                if (com.IsOpen)
                {
                    byte[] readBuffer = new byte[com.ReadBufferSize + 1];
                    try
                    {
                        // If there are bytes available on the serial port,
                        // Read returns up to "count" bytes, but will not block (wait)
                        // for the remaining bytes. If there are no bytes available
                        // on the serial port, Read will block until at least one byte
                        // is available on the port, up until the ReadTimeout milliseconds
                        // have elapsed, at which time a TimeoutException will be thrown.
                        int count = com.Read(readBuffer, 0, com.ReadBufferSize);
                        String SerialIn = System.Text.Encoding.ASCII.GetString(readBuffer, 0, count);
                        if (count != 0)
                            //byteToHexStr(readBuffer);
                            ThreadFunction(byteToHexStr(readBuffer,count));
                    }
                    catch (TimeoutException) { }
                }
                else
                {
                    TimeSpan waitTime = new TimeSpan(0, 0, 0, 0, 50);
                    Thread.Sleep(waitTime);
                }
            }
        }
```


**方法二：**使用C#自带的SerialPor控件。

1...在“工具箱”的“组件”中选择SerialPor控件添加。

2...设置串口并打开

serialPort1.PortName = "COM1";

serialPort1.BaudRate = 9600;

serialPort1.Open();

3...写入数据可以使用Write或者下面的函数

serialPort1.WriteLine(str);

4...添加数据接收的事件

private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)

**使用中的一些常见问题**

C#中SerialPort类中DataReceived事件GUI实时处理方法（来自wanglei_wan@yahoo.com.cn 的看法）
MSDN：从 SerialPort 对象接收数据时，将在辅助线程上引发 DataReceived 事件。由于此事件在辅助线程而非主线程上引发，因此尝试修改主线程中的一些元素（如 UI 元素）时会引发线程异常。如果有必要修改主 Form 或 Control 中的元素，必须使用 Invoke 回发更改请求，这将在正确的线程上执行.进而要想将辅助线程中所读到的数据显示到主线程的Form控件上时，只有通过Invoke方法来实现 
下面是代码实例： 

```
private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
{
   int SDateTemp = this.serialPort1.ReadByte();
   //读取串口中一个字节的数据
   this.tB_ReceiveDate.Invoke(   
	//在拥有此控件的基础窗口句柄的线程上执行委托Invoke(Delegate)
	//即在textBox_ReceiveDate控件的父窗口form中执行委托.
	new MethodInvoker(            
	/*表示一个委托，该委托可执行托管代码中声明为 void 且不接受任何参数的任何方法。 在对控件的 Invoke 	方法进行调用时或需要一个简单委托又不想自己定义时可以使用该委托。*/
	delegate{                 
    /*匿名方法,C#2.0的新功能，这是一种允许程序员将一段完整代码区块当成参数传递的程序代码编写技术，通过此种方法可 	以直接使用委托来设计事件响应程序以下就是你要在主线程上实现的功能但是有一点要注意，这里不适宜处理过多的方法，因为C#消息机制是消息流水线响应机制，如果这里在主线程上处理语句的时间过长会导致主UI线程阻塞，停止响应或响应不顺畅,这时你的主form界面会延迟或卡死      */                   
	this.tB_ReceiveDate.AppendText(SDateTemp.ToString());//输出到主窗口文本控件
	this.tB_ReceiveDate.Text += " ";}
    )
    );
}
```

**如何知道当前电脑有哪个串口**

在窗体上添加一个comboBox控件。

然后使用comboBox1.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());  或者

 string[] portList = System.IO.Ports.SerialPort.GetPortNames();
            for (int i = 0; i < portList.Length; ++i)
            {
                string name = portList[i];
                comboBox1.Items.Add(name);
            }

 具体请参考[http://msdn.microsoft.com/zh-tw/library/system.io.ports.serialport.getportnames.aspx](http://msdn.microsoft.com/zh-tw/library/system.io.ports.serialport.getportnames.aspx)

