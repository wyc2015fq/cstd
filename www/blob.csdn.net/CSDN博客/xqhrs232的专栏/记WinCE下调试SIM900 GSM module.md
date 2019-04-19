# 记WinCE下调试SIM900 GSM module - xqhrs232的专栏 - CSDN博客
2013年02月05日 11:18:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：796
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.csdn.net/aaa_tnt/article/details/6128793](http://blog.csdn.net/aaa_tnt/article/details/6128793)
 记WinCE下调试SIM900 GSM module
近几天在调试sim900，把遇到的一些问题总结下吧。
一、一些引脚的说说明。
Sim900有几个引脚需要了解下。
1.  CTS、RTS 由于使用了流控制，来提高传输效率。首先要清楚几个概念。
       什么是CTS、RTS：在sim900模块中，
       RTS是模块的输入端，用于MCU通知模块，MCU是否准备好，模块是否可向MCU发送信息，RTS的有效电平为低。
CTS是模块的输出端，用于模块通知MCU，模块是否准备好，MCU是否可向模块发送信息，CTS的有效电平为低
而在我们的MCU上面，一般RTS为输出端，CTS为输入端，所以，DCE（数据通信端）的RTS、CTS直连CTE(数据终端)的RTS、CTS。
AFC:自动流控制，在我用的2450中视这样描述的，
UART 0, UART 1 and UART 2 support auto flow control with nRTS and nCTS signals.  In AFC, nCTS signals
control the operation of the transmitter, and nRTS depends on the condition of the receiver.
The UART's transmitter transfers the data in FIFO only when nCTS signals are activated(Low) (In AFC, nCTS
means that other UART's FIFO is ready to receive data or not). 
Before the UART receives data, nRTS signal has to be activated(Low) when its receive FIFO has a spare space more than 32-byte(FIFO contains less than 32-byte). And nRTS signal has to be inactivated(High) when its receive FIFO has a spare under 32-byte(FIFO
 contains equal or more than 32-byte) in case of RTS trigger level is 32byte. (In AFC, nRTS means that its own receive FIFO is ready to receive data or not).
大意是在自动流控（AFC）模式下nRTS取决于接收端的状态，而nCTS控制了发送端的操作。具体地说：只有当nCTS有效（低电平）时（表明接收方的FIFO已经准备就绪来接收资料了），UART才会将FIFO中的资料发送出去。在UART接收资料之前，只要当接收FIFO有至少32-byte空余的时候，nRTS就会被置为有效。
       同样的，我们在串口初始化的时候需要配置寄存器以开启MCU的AFC自动流控制功能。
2.    DTR: 当设置AT+CSCLK=1 开启Sleep后（默认是关闭的）
When DTR is in high level and there is no on air and hardware interrupt (such as GPIO interrupt or data on serial port), SIM900 will enter SLEEP mode automatically.
当DTR为非激活状态（高电平），且没有硬件中断和串口数据时，sim900会自动进入sleep,所以当你不想进入Sleep时，需要保持激活DTR，即保持低电平。
其次可能你的串口驱动并没有实现DTR引脚，或者引脚对不上，因为在某些芯片上并没有像CTS、RTS一样的标准复用GPIO。
即在串口驱动的PDD层中重写这个MDD层的纯虚函数virtual void    SetDTR(BOOL bSet) = 0；
这样你就可以通过调用API EscapeCommFunction（），设置参数CLRDTR 和 SETDTR来控制DTR。
3.    RI: 在sim900中，RI在待机中保持高电平，当有语音呼入，或者短信接入的时候会发生跳变，应此，我们可以将它接MCU的EINT 外部中断引脚，以便当系统进入Sleep时，当有短信来电时，可以唤醒系统。
4.    NRESET：通过外部引脚可以使模块复位，低电平有效。
二、关于初始化。
1.Sim900默认是自动识别主机端波特率的，但是波特率必须小于115200bps，而为了使用多路复用，要设置到115200bps,关于为什么，后面会提到。
2.因为初始化应在sim900上电一段时间后进行，为了不影响系统的启动速度，我们可以将初始化和多路复用驱动加载的动作放到线程中，这样会在NK起来以后，线程还会对sim900进行初始化、并加载多路复用。这样开机启动时间就不会受到影响了。
初始化部分代码如下，模块上电后，我便会打开串口，来检测sim900是不是首次被初始化，因为sim900被设置为非自动识别波特率后，上电会往主机端发送“RDY”字符串。下面是一些src..比较散乱。
**[cpp]**[view
 plain](http://blog.csdn.net/aaa_tnt/article/details/6128793#)[copy](http://blog.csdn.net/aaa_tnt/article/details/6128793#)
- if(!OpenPort(TEXT("COM1:"), 115200, 8, ONESTOPBIT, NOPARITY))            
- 
- {     
- 
-         RETAILMSG(1, (TEXT("[GSM] Open Failed /r/n")));     
- 
- return FALSE;     
- 
- }     
- 
- //Activate nDTR   (hold low)   
- 
- EscapeCommFunction(g_hComm, SETDTR);     
- 
- 
- 
- Sleep(200);     
- 
- memset(RecvBuf,0,sizeof(RecvBuf));     
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);     
- 
- if(strstr(RecvBuf, "RDY") != NULL)     
- 
- {     
- 
-         RETAILMSG(1, (TEXT("[GSM] Sim9000 has been Initialized !/r/n")));     
- 
- goto Release;     
- 
- }     
- 
- if(!OpenPort(TEXT("COM1:"), 9600, 8, ONESTOPBIT, NOPARITY))        
- 
- {     
- 
-         RETAILMSG(1, (TEXT("[GSM] Open Failed /r/n")));     
- 
- }     
- 
- 
- 
- //Sync   
- 
- WriteFile(g_hComm, "A", 1, &dwActlen, NULL);             
- 
- WriteFile(g_hComm, "AT/r", 3, &dwActlen, NULL);     
- 
-  Sleep(200);     
- 
-  memset(RecvBuf,0,sizeof(RecvBuf));     
- 
-  ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);        
- 
- if(strstr(RecvBuf, "OK") != NULL)     
- 
-  {     
- 
-       RETAILMSG(1, (TEXT("[GSM] Sync ! /r/n")));     
- 
-   }     
- 
- }     
- 
- 
- 
-  如果是首次初始化则会设置开启睡眠模式，以及设置波特率115200bps     
- 
- //Enable sleep mode   
- 
- WriteFile(g_hComm, "AT+CSCLK=1/r", strlen("AT+CSCLK=1/r"), &dwActlen, NULL);     
- 
- Sleep(200);     
- 
- memset(RecvBuf,0,sizeof(RecvBuf));     
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);        
- 
- if(strstr(RecvBuf, "OK") != NULL)     
- 
- {     
- 
-         RETAILMSG(1, (TEXT("[GSM] Enable slow clock ! /r/n")));     
- 
- }     
- 
- 
- 
- //Set baud to 115200bps   
- 
- WriteFile(g_hComm, "AT+IPR=115200/r", strlen("AT+IPR=115200/r"), &dwActlen, NULL);     
- 
- Sleep(200);     
- 
- memset(RecvBuf,0,sizeof(RecvBuf));     
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);        
- 
- if(strstr(RecvBuf, "OK") != NULL)     
- 
- {     
- 
-         RETAILMSG(1, (TEXT("[GSM] Set baud to 115200bps ! /r/n")));      
- 
- }     
- 
- 
- if(!OpenPort(TEXT("COM1:"), 115200, 8, ONESTOPBIT, NOPARITY))         
- 
- {  
- 
-         RETAILMSG(1, (TEXT("[GSM] Open Failed /r/n")));  
- 
- return FALSE;  
- 
- }  
- 
- //Activate nDTR   (hold low)
- 
- EscapeCommFunction(g_hComm, SETDTR);  
- 
- 
- 
- Sleep(200);  
- 
- memset(RecvBuf,0,sizeof(RecvBuf));  
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);  
- 
- if(strstr(RecvBuf, "RDY") != NULL)  
- 
- {  
- 
-         RETAILMSG(1, (TEXT("[GSM] Sim9000 has been Initialized !/r/n")));  
- 
- goto Release;  
- 
- }  
- 
- if(!OpenPort(TEXT("COM1:"), 9600, 8, ONESTOPBIT, NOPARITY))     
- 
- {  
- 
-         RETAILMSG(1, (TEXT("[GSM] Open Failed /r/n")));  
- 
- }  
- 
- 
- 
- //Sync
- 
- WriteFile(g_hComm, "A", 1, &dwActlen, NULL);          
- 
- WriteFile(g_hComm, "AT/r", 3, &dwActlen, NULL);  
- 
-  Sleep(200);  
- 
-  memset(RecvBuf,0,sizeof(RecvBuf));  
- 
-  ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);     
- 
- if(strstr(RecvBuf, "OK") != NULL)  
- 
-  {  
- 
-       RETAILMSG(1, (TEXT("[GSM] Sync ! /r/n")));  
- 
-   }  
- 
- }  
- 
- 
- 
-  如果是首次初始化则会设置开启睡眠模式，以及设置波特率115200bps  
- 
- //Enable sleep mode
- 
- WriteFile(g_hComm, "AT+CSCLK=1/r", strlen("AT+CSCLK=1/r"), &dwActlen, NULL);  
- 
- Sleep(200);  
- 
- memset(RecvBuf,0,sizeof(RecvBuf));  
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);     
- 
- if(strstr(RecvBuf, "OK") != NULL)  
- 
- {  
- 
-         RETAILMSG(1, (TEXT("[GSM] Enable slow clock ! /r/n")));  
- 
- }  
- 
- 
- 
- //Set baud to 115200bps
- 
- WriteFile(g_hComm, "AT+IPR=115200/r", strlen("AT+IPR=115200/r"), &dwActlen, NULL);  
- 
- Sleep(200);  
- 
- memset(RecvBuf,0,sizeof(RecvBuf));  
- 
- ReadFile(g_hComm, RecvBuf, 255, &dwActlen, NULL);     
- 
- if(strstr(RecvBuf, "OK") != NULL)  
- 
- {  
- 
-         RETAILMSG(1, (TEXT("[GSM] Set baud to 115200bps ! /r/n")));   
- 
- }  
三、关于多路复用驱动。
厂家给了2个文件，mux_sim900.reg和mux_sim900.dll
我的mux_sim900.reg 设置如下：
; COM1 for underlying GSM 07.10
[HKEY_LOCAL_MACHINE/Software/Microsoft/GSM07_10]
        "ComPort"="COM1:"                      ；对应你连接sim900的物理UART
        "DefaultBaudRate"=dword:1C200         ; 115200   ；这2处的波特率只能为115200，原因请往下看。
        "BaudRate"=dword:1C200              ; 115200 
        "FrameOption"=dword:0               ; 1 - Advanced; 0 -Basic
        "FrameSize"=dword:0000007F          ; max data size
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_1/Unimodem]
        "Tsp"="Unimodem.dll"
        "FriendlyName"="SIM900 MUX Modem"
        "DeviceType"=dword:1
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_1/Unimodem/Init]
        "1"="AT<cr>"
        "2"="ATE0V1&C1&D2<cr>"
[HKEY_LOCAL_MACHINE/Drivers/BuiltIn/GSM0710_1/Unimodem/Settings]
        "Prefix"="AT"
        "Terminator"="<cr>"
        "DialPrefix"="D"
        "DialSuffix"=";"
        "Pulse"="P"
        "Tone"="T"
        "Blind_Off"="X4"
        "Blind_On"="X3"
        "CallSetupFailTimeout"="ATS7=<#><cr>"
        "Reset"="AT<cr>"   
        "EscapeDelay"=dword:3E8
        "EscapeWait"=dword:3E8
[HKEY_LOCAL_MACHINE/Drivers/GSM0710_1]
        "Prefix"="COM"
        "Dll"="mux_sim900.dll"
        "FriendlyName"="SIM900 MUX Port on COM3:"
        "DeviceArrayIndex"=dword:1
        "Index"=dword:3
        "Order"=dword:2
        ; power manager enabled device driver
        "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"  
        ; GSM 07.10 specific
        "GSM_0710_DLCI"=dword:1     ; Rem: Dlci also define priority (lowest => low prio)
        "GSM_0710_TYPE"=dword:1     ; boolean: 1 (true) => AT cmd, 0 (false) => Data   
        ; monitor channel activity
        "GSM_0710_ACTIVITY_TIMEOUT"=dword:2710      ; time-out on channel activity in ms
                                                    ; remark:
                                                    ; the Mux driver will notify the device
                                                    ; when this time-out expire
                                                    ; if there is no activity       
[HKEY_LOCAL_MACHINE/Drivers/GSM0710_2]
        "Prefix"="COM"
        "Dll"="mux_sim900.dll"
        "DeviceArrayIndex"=dword:1
        "Index"=dword:4
        "FriendlyName"="SIM900 MUX Port on COM4:"
        "Order"=dword:3
        ; power manager enabled device driver
        "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"  
        ; GSM 07.10 specific
        "GSM_0710_DLCI"=dword:2     ; Rem: Dlci also define priority (lowest => low prio)
        "GSM_0710_TYPE"=dword:1     ; boolean: 1 (true) => AT cmd, 0 (false) => Data
        ; monitor channel activity
        "GSM_0710_ACTIVITY_TIMEOUT"=dword:4E20      ; time-out on channel activity in ms
                                                    ; remark:
                                                    ; the Mux driver will notify the device
                                                    ; when this time-out expire
                                                    ; if there is no activity
[HKEY_LOCAL_MACHINE/Drivers/GSM0710_3]
        "Prefix"="COM"
        "Dll"="mux_sim900.dll"
        "DeviceArrayIndex"=dword:1
        "Index"=dword:5
        "FriendlyName"="SIM900 MUX Port on COM5:"
        "Order"=dword:4
        ; power manager enabled device driver
        "IClass"="{A32942B7-920C-486b-B0E6-92A702A99B35}"  
        ; GSM 07.10 specific
        "GSM_0710_DLCI"=dword:3     ; Rem: Dlci also define priority (lowest => low prio)
        "GSM_0710_TYPE"=dword:1     ; boolean: 1 (true) => AT cmd, 0 (false) => Data
        ; monitor channel activity
        "GSM_0710_ACTIVITY_TIMEOUT"=dword:7530   ; time-out on channel activity in ms
                                                    ; remark:
                                                    ; the Mux driver will notify the device
                                                    ; when this time-out expire
                                                    ; if there is no activity
以上，我复用了3个端口，COM3,COM4,COM5,其中COM3 用于GPRS拨号，
把他们放到Drivers/下而不是Drivers/BuiltIn下的原因是，因为我会在上面的线程中手动加载，这样便于我灵活的控制sim900模块，而且大大提高了wince启动速度。
四、关于波特率。
之前设置的波特率为19200，发现多路复用起来以后，并不能正常工作，现象比较搞笑，我发什么数据复用端口就返回什么数据。
后来把串口驱动的收发数据全部打印了出来，对照sim900的多路复用协议，看出了一些蹊跷，发现我发出的AT指令，接受数据中包含了正常数据。后来发现多路复用驱动在初始化建立通道的时候会发送AT+CMUX，并把多路通道的波特率锁死到了115200bps,而不是根据注册表的那2个值。
经过一番折腾之后，sim900的多路复用终于正常工作了。
