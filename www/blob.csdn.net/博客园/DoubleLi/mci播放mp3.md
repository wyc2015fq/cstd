# mci播放mp3 - DoubleLi - 博客园






1
MIDI的播放
---- 乐器数字化接口（MIDI）是由音乐界的一些大公司（包括生产电子音乐合成器的公司）制订的一项协议，后来被计算机产业所采用并成为多媒体音乐文件的标准格式。MIDI文件一般较小，对硬件设备的要求高。 
---- 一、 原理 

---- 虽然MicroSoft支持MIDI文件，然而Visual C++或MFC并没有创建任何组件来实现这种支持，但是MicroSoft API提供了三种不同的方法来实现MIDI的播放： 

MCI（The Media Control Interface）。这是最基本的方法，本文将详细讨论这种方法。

流缓冲器。这种格式允许应用程序为MIDI数据分配缓冲器。在需要精确控制MIDI播放的时候，流缓冲器将很有用处。

低级MIDI设备。需要完全控制MIDI数据的应用程序可以使用这种方法。 
---- MCI可以通过mciSendCommand（）和mciSendString（）来完成，本文仅使用mciSendCommand（）函数。 

---- 原型：DWORD mciSendCommand（UINT wDeviceID，UINT wMessage，DWORD dwParam1，DWORD dwParam2）； 

参数： wDeviceID：接受消息的设备ID
wMessage：MCI命令消息
dwParam1：命令的标志位
dwParam2：所使用参数块的指针

---- 返值：调用成功，返回零；否则，返回双字中的低字存放有错误信息。 

二MIDI的播放控制
---- 1． 打开设备 
MCI_OPEN_PARMS OpenParms;
OpenParms.lpstrDeviceType =
(LPCSTR) MCI_DEVTYPE_SEQUENCER;//MIDI类型
OpenParms.lpstrElementName = (LPCSTR) Filename;
OpenParms.wDeviceID = 0;
mciSendCommand (NULL, MCI_OPEN,
MCI_WAIT | MCI_OPEN_TYPE |
MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
(DWORD)(LPVOID) &OpenParms)

---- MCI设备ID指明打开了哪个设备，当发送了MCI_OPEN命令时，这个值在参数块中返回——应被保存备用。 

---- 2． 关闭设备 

mciSendCommand (m_wDeviceID, MCI_CLOSE, NULL, NULL);
---- 3． 播放 

MCI_PLAY_PARMS PlayParms;
PlayParms.dwFrom = 0;
// 指定从什么地方（时间）播放
mciSendCommand (m_wDeviceID, MCI_PLAY,
MCI_FROM, (DWORD)(LPVOID)
&PlayParms))；

---- 4． 暂停 

MCI_PLAY_PARMS PlayParms;
mciSendCommand (m_wDeviceID, MCI_PAUSE, 0,
(DWORD)(LPVOID) &PlayParms)；

---- 5． 停止 

mciSendCommand (m_wDeviceID, MCI_STOP, NULL, NULL);

---- 6． 跳跃 

* 跳转到任意地方
MCI_SEEK_PARMS SeekParms;
SeekParms.dwTo = (nMinute * 60 + nSecond) * 1000;
//跳转的目标时间，时间单位为毫秒
mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_TO
| MCI_WAIT,(DWORD)(LPVOID)
&SeekParms)；
* 跳到文件头
mciSendCommand (m_wDeviceID, MCI_SEEK,
MCI_SEEK_TO_START, NULL);
* 跳到文件尾
mciSendCommand (m_wDeviceID, MCI_SEEK,
MCI_SEEK_TO_END, NULL);

---- 7． 查询当前信息 

MCI_STATUS_PARMS StatusParms;
StatusParms.dwItem = MCI_SEQ_STATUS_DIVTYPE;
mciSendCommand (m_wDeviceID, MCI_STATUS,
MCI_WAIT | MCI_STATUS_ITEM,
(DWORD)(LPVOID) &StatusParms)；
返回信息存放于StatusParms.dwReturn中。
MCI_STATUS标志
MCI_STATUS_LENGTH 获得文件长度
MCI_STATUS_MODE 获得文件播放的当前状态
MCI_STATUS_POSITION 获得文件播放的当前位置
MCI_STATUS_TIME_FORMAT 获得当前的时间格式
MCI_SEQ_STATUS_DIVTYPE 判断文件是PPQN类型还是SMPTE类型
MCI_SEQ_STATUS_TEMPO 获得当前播放速度，PQRN类型，
此值为节拍/分，SMPTE类型，此值为祯/秒

---- 8． 设置时间格式及播放速度 

MCI_SET_PARMS SetParms;
SetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
//设置时间单位为毫秒
mciSendCommand (m_wDeviceID,
MCI_SET, MCI_SET_TIME_FORMAT,
(DWORD)(LPVOID) &SetParms)；
MCI_SEQ_SET_TEMPO 设置播放速度，
PQRN类型，此值为节拍/分，
SMPTE类型，此值为祯/秒









