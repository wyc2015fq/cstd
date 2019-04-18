# MCI编程--VC++6.0下MIDI、WAV及CD的播放 - 深之JohnChen的专栏 - CSDN博客

2005年12月20日 11:33:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3603


使用MCI API，源文件中需要包含头文件 Mmsystem.h，在Project->Settings->Link->Object/libraymodule中加入库 Winmm.lib。 

１、MCI简介 

　　MCI(MediaControl Interface，媒体控制接口)向Windows程序提供了在高层次上控制媒体设备接口的能力。程序不必关心具体设备，就可以对激光唱机（CD）、视盘机、波形音频设备、视频播放设备和MIDI设备等媒体设备进行控制。 

　　对于程序员来说，可以把MCI理解为设备面板上的一排按键，通过选择不同的按键（发送不同的MCI命令）可以让设备完成各种功能，而不必关心设备内部实现。 

　　比如，对于play，视盘机和CD机有不同的反应（一个是播放视频，一个播放音频），而对用户来说却只需要按同一按钮。 

　　应用程序通过向MCI发送命令来控制媒体设备。MCI命令接口分命令字符串和命令消息两种，两者具有相同的功能。命令字符串具有使用简单的特点，但是它的执行效率不如命令消息。 

　　所有的MCI命令字符串都是通过多媒体API函数mciSendString传递给MCI的，该函数的声明为： 

　　　　MCIERRORmciSendString( 

　　　　　　LPCTSTRlpszCommand, 　　　//MCI命令字符串 

　　　　　　LPTSTR　lpszReturnString,　//存放反馈信息的缓冲区 

　　　　　　UINT　　cchReturn, 　　　　//缓冲区的长度 

　　　　　　HANDLE　hwndCallback 　　　//回调窗口的句柄，一般为NULL 

　　　　); //若成功则返回0，否则返回错误码。 

　　该函数返回的错误码可以用mciGetErrorString函数进行分析，该函数的声明为： 

　　　　BOOLmciGetErrorString( 

　　　　　　DWORD　fdwError,　　　//函数mciSendString返回的错误码 

　　　　　　LPTSTRlpszErrorText, //接收描述错误的字符串的缓冲区

　　　　　　UINT　 cchErrorText　 //缓冲区的长度 

　　　　); 

　　下面是使用mciSendString函数的一个简单例子： 

　　　　charbuf[50]; 

　　　　MCIERRORmciError; 

　　　　mciError=mciSendString("opencdaudio",buf,strlen(buf),NULL); 

　　　　if(mciError)

　　　　{ 

　　　　　　mciGetErrorString(mciError,buf,strlen(buf));

　　　　　　AfxMessageBox(buf);

　　　　　　return; 

　　　　} 

　　open cdaudio命令打开CD播放器，如果出错（如驱动器内没有CD）则返回错误码，此时可以用mciGetErrorString函数取得错误信息字符串。 

２、MCI设备 

　　open是MCI打开设备的命令，cdaudio是MCI设备名。MCI的设备类型如下： 

　　　　animation　　动画设备 

　　　　cdaudio　　　CD播放器 

　　　　dat　　　　　数字音频磁带机 

　　　　digitalvideo某一窗口中的数字视频（不基于GDI） 

　　　　other　　　　未定义的MCI设备 

　　　　overlay　　　重叠设备（窗口中的模拟视频） 

　　　　scanner　　　图象扫描仪 

　　　　sequencer　　MIDI序列器 

　　　　videodisc　　视盘机 

　　　　waveaudio　　播放数字波形文件的音频设备 

　　设备名是在注册表或SYSTEM.INI的[mci]部分定义的，典型的[mci]段如下： 

　　　　[mci] 

　　　　cdaudio=mcicda.drv

　　　　sequencer=mciseq.drv

　　　　waveaudio=mciwave.drv

　　　　avivideo=mciavi.drv

　　　　videodisc=mcipionr.drv

　　等号的左边是设备名，右边是对应的MCI驱动程序。当安装了新的MCI驱动程序时，系统要用不同的设备名来区分。 

３、MCI命令 

　　使用MCI设备一般包括打开、使用和关闭三个过程，常用的MCI命令有： 

　　　　open　　　　打开设备 

　　　　close　　　 关闭设备 

　　　　play　　　　开始设备播放 

　　　　stop　　　　停止设备的播放或记录 

　　　　record　　　开始记录 

　　　　save　　　　保存设备内容 

　　　　pause　　　 暂停设备的播放或记录 

　　　　resume　　　恢复暂停播放或记录的设备 

　　　　seek　　　　改变媒体的当前位置 

　　　　capacility　查询设备能力 

　　　　info　　　　查询设备的信息 

　　　　status　　　查询设备状态信息 

　　MCI的大部分命令可以控制不同的媒体设备，但其中record和save命令并不是所有MCI设备都可以使用。 

　　MCI命令的使用是很随意的，只要先打开，最后关闭，中间可以随意调用各种命令。 

(1) open　打开设备 

　　MCI设备使用前必须先打开，当然，使用后也必须要关闭，以免影响他人的使用。 

　　opendevice_name type device_type alias device_alias 

　　　　device_name　 　　　要使用的设备名，通常是文件名。 

　　　　typedevice_type　　设备类型，例如waveaudio或sequencer，可省略。 

　　　　aliasdevice_alias　设备别名，指定后可在其他命令中代替设备名。 

(2) play　开始设备播放 

　　MCI设备打开后即可以播放，可使用设备名或别名。 

　　playdevice_alias from pos1 to pos2 wait repeat 

　　　　若省略from则从当前磁道开始播放，若省略to则播放到结束。 

　　　　若指明wait则等到播放完毕命令才返回。 

　　　　若指明repeat则会不停的重复播放。 

　　　　若同时指明wait和repeat则命令不会返回，本线程产生堵塞，通常会引起程序失去响应。 

(3) 播放CD 

　　voidCTttView::OnOpenCD() 

　　{ 

　　　　mciSendString("opencdaudio",NULL,0,NULL); 

　　　　mciSendString("playcdaudio",NULL,0,NULL); 

　　); 

　　voidCTttView::OnStopCD() 

　　{ 

　　　　mciSendString("stopcdaudio",NULL,0,NULL); 

　　　　mciSendString("closecdaudio",NULL,0,NULL); 

　　); 

　　还可以： 

　　　　pausecdaudio 　暂停播放。 

　　　　resumecdaudio　继续被暂停的播放。 

　　　　seekcdaudio to <位置>　移动到指定磁道。 

　　　　setcdaudio door open/closed　弹出或缩进CD盘。 

(4) 播放多媒体文件 

　　voidCTttView::OnMyMenu() 

　　{ 

　　　　mciSendString("openmyfolder\\tada.wav alias aa",NULL,0,NULL); 

　　或　mciSendString("openmyfolder\\flourish.mid alias aa",NULL,0,NULL); 

　　或　mciSendString("openmyfolder\\clock.avi alias aa",NULL,0,NULL); 

　　　　mciSendString("playaa wait",NULL,0,NULL); 

　　　　mciSendString("closeaa",NULL,0,m_hWnd); 

　　); 

(5) 录制声音 

　　voidCTttView::OnStartRecord() 

　　{ 

　　　　mciSendString("opennew type waveaudio alias aa",NULL,0,NULL); 

　　　　mciSendString("recordaa",NULL,0,NULL); 

　　); 

　　voidCTttView::OnStopRecord() 

　　{ 

　　　　mciSendString("saveaa c:\\aaa.wav wait",NULL,0,NULL); 

　　　　mciSendString("closeaa",NULL,0,NULL); 

　　); 

加入音乐是增强应用程序功能的所有方法中最简单的一个。几乎每个计算机游戏或多 媒体程序都以某种MIDI或CD音乐为背景。音乐可以使用户心情愉快；在合适的场合播 放恰当的音乐能够使程序员和他的VC++程序焕发光彩。 

第一部分 MIDI的播放 
---- 乐器数字化接口（MIDI）是由音乐界的一些大公司（包括生产电子音乐合成器的公司） 制订的一项协议，后来被计算机产业所采用并成为多媒体音乐文件的标准格式。MIDI文件 一般较小，对硬件设备的要求高。 

---- 一、 原理 

---- 虽然MicroSoft支持MIDI文件，然而Visual C++或MFC并没有创建任何组件来实现 这种支持，但是MicroSoft API提供了三种不同的方法来实现MIDI的播放： 

MCI（The Media Control Interface）。这是最基本的方法，本文将详细讨论这种方法。 

流缓冲器。这种格式允许应用程序为MIDI数据分配缓冲器。在需要精确控制MIDI播放的时候，流缓冲器将很有用处。 

低级MIDI设备。需要完全控制MIDI数据的应用程序可以使用这种方法。 
---- MCI可以通过mciSendCommand（）和mciSendString（）来完成，本文仅使用mciSendCommand（）函数。 

---- 原型：DWORD mciSendCommand（UINT wDeviceID，UINT wMessage，DWORD dwParam1，DWORD dwParam2）； 

参数：  wDeviceID：接受消息的设备ID
wMessage：MCI命令消息
dwParam1：命令的标志位
dwParam2：所使用参数块的指针

---- 返值：调用成功，返回零；否则，返回双字中的低字存放有错误信息。 

二MIDI的播放控制 
---- 1． 打开设备 

MCI_OPEN_PARMS OpenParms;
OpenParms.lpstrDeviceType = 
(LPCSTR) MCI_DEVTYPE_SEQUENCER;//MIDI类型
OpenParms.lpstrElementName = (LPCSTR) Filename;
OpenParms.wDeviceID = 0;
mciSendCommand (NULL, MCI_OPEN, 
MCI_WAIT | MCI_OPEN_TYPE | 
MCI_OPEN_TYPE_ID | MCI_OPEN_ELEMENT,
(DWORD)(LPVOID) &OpenParms)

---- MCI设备ID指明打开了哪个设备，当发送了MCI_OPEN命令时，这个值在参数块中返回——应被保存备用。 

---- 2． 关闭设备 

mciSendCommand (m_wDeviceID, MCI_CLOSE, NULL, NULL);

---- 3． 播放 

MCI_PLAY_PARMS PlayParms;
PlayParms.dwFrom = 0;
// 指定从什么地方（时间）播放
mciSendCommand (m_wDeviceID, MCI_PLAY, 
MCI_FROM, (DWORD)(LPVOID) 
&PlayParms))；

---- 4． 暂停 

MCI_PLAY_PARMS PlayParms;
mciSendCommand (m_wDeviceID, MCI_PAUSE, 0, 
(DWORD)(LPVOID) &PlayParms)；

---- 5． 停止 

mciSendCommand (m_wDeviceID, MCI_STOP, NULL, NULL);

---- 6． 跳跃 

* 跳转到任意地方
MCI_SEEK_PARMS SeekParms;
SeekParms.dwTo = (nMinute * 60 + nSecond) * 1000;
//跳转的目标时间，时间单位为毫秒
mciSendCommand (m_wDeviceID, MCI_SEEK, MCI_TO 
| MCI_WAIT,(DWORD)(LPVOID) 
&SeekParms)；
* 跳到文件头
mciSendCommand (m_wDeviceID, MCI_SEEK, 
MCI_SEEK_TO_START, NULL);
* 跳到文件尾
mciSendCommand (m_wDeviceID, MCI_SEEK,
MCI_SEEK_TO_END, NULL);

---- 7． 查询当前信息 

MCI_STATUS_PARMS StatusParms;
StatusParms.dwItem = MCI_SEQ_STATUS_DIVTYPE;
mciSendCommand (m_wDeviceID, MCI_STATUS, 
MCI_WAIT | MCI_STATUS_ITEM,
(DWORD)(LPVOID) &StatusParms)；
返回信息存放于StatusParms.dwReturn中。
MCI_STATUS标志
MCI_STATUS_LENGTH  获得文件长度
MCI_STATUS_MODE  获得文件播放的当前状态
MCI_STATUS_POSITION  获得文件播放的当前位置
MCI_STATUS_TIME_FORMAT  获得当前的时间格式
MCI_SEQ_STATUS_DIVTYPE  判断文件是PPQN类型还是SMPTE类型
MCI_SEQ_STATUS_TEMPO  获得当前播放速度，PQRN类型，
此值为节拍/分，SMPTE类型，此值为祯/秒

---- 8． 设置时间格式及播放速度 

MCI_SET_PARMS SetParms;
SetParms.dwTimeFormat = MCI_FORMAT_MILLISECONDS;
//设置时间单位为毫秒
mciSendCommand (m_wDeviceID, 
MCI_SET, MCI_SET_TIME_FORMAT, 
(DWORD)(LPVOID) &SetParms)；
MCI_SEQ_SET_TEMPO  设置播放速度，
PQRN类型，此值为节拍/分，
SMPTE类型，此值为祯/秒

第二部分 WAV文件的播放 
---- 一、原理 

---- MicroSoft API提供了三种不同的方法来实现WAV的播放： 

PlaySound（）函数。它可以通过单行编码来播放Wave格式的声音。此函数有两个限制：必须将声音数据完整地载入物理内存；数据格式必须被所配置的某一音频驱动器支 持。根据经验，PlaySound（）适用于100K以下的文件。 

MCI（The Media Control Interface），与上一章播放MIDI文件相似，可以播放100K 以上的文件。 

低级Wave音频设备。用这些设备可以运行完全控制Wave数据的应用文件。 
---- 二、 WAV文件播放控制 

---- 因为同样使用MCI，与上一章相同，只列出不同的部分。 

---- 1． 打开设备 

---- 将MIDI的MCI_DEVTYPE_SEQUENCER 改为"waveaudio" 

---- 2． 录音 

MCI_RECORD_PARMS RecordParms;
mciSendCommand (m_wDeviceID, MCI_RECORD, 
NULL, (DWORD)(LPVOID) 
&RecordParms)；

---- 3． 保存录音 

MCI_SAVE_PARMS SaveParms;
SaveParms.lpfilename = (LPCSTR) Filename;
mciSendCommand (m_wDeviceID, MCI_SAVE,
MCI_SAVE_FILE | MCI_WAIT, 
(DWORD)(LPVOID) &SaveParms)；

第三部分 CD的播放 
---- CD的独特优势在于，它由作曲家设计，并由音乐厂家生产。不同的计算机播放MIDI 文件时，声音效果也不一样，但是CD的声音效果总是相同的。高品质的音频对计算机用 户产生的效果会使你感到吃惊。 我们依然采用MCI播放CD，大部分的播放控制与前两部分相同，只列出不同的部分 

---- 1． 开光驱门 

mciSendCommand (m_wDeviceID, MCI_SET,
MCI_SET_DOOR_OPEN, NULL);

---- 2． 关光驱门 

mciSendCommand (m_wDeviceID, MCI_SET, 
MCI_SET_DOOR_CLOSED, NULL);

---- 3． 打开设备 

将MIDI的MCI_DEVTYPE_SEQUENCER 改为MCI_DEVTYPE_CD_AUDIO

---- 4． 播放 

---- 指定播放起点必须经过MCI_MAKE_TMSF(Track，Minute，Second，Frame)转化 

---- 5． 查询当前信息 

MCI_STATUS_CURRENT_TRACK  得到当前曲目
MCI_STATUS_LENGTH  得到CD或指定曲目长度
MCI_STATUS_MODE  得到驱动器的当前状态
MCI_STATUS_NUMBER_OF_TRACKS  得到CD曲目的数目
MCI_STATUS_POSITION  得到当前格式下的位置
MCI_STATUS_READY  检查设备是否就绪
MCI_STATUS_TIME_FORMAT  得到当前时间格式
MCI_STATUS_MEDIA_PRESENT  检查以确认CD是否在驱动器内
MCI_CDA_STATUS_TYPE_TRACK 检查已确认某曲目是否为音频曲目

---- 注意： 

使用MCI_STATUS_LENGTH参数查询CD 及曲目长度，返回值通过调用MCI_MSF_MINUTE（），MCI_MSF_SECOND（）转换为分、秒。 

MCI_STATUS_POSITION参数返回值调用MCI_TMSF_TRACK（）, MCI_TMSF_MINUTE（）， MCI_TMSF_SECOND（），MCI_TMSF_FRAME才能得到当前的位置的道、分、秒、帧。 
---- 6． 跳跃 

---- 跳转的目标必须经过MCI_MAKE_TMSF(Track，Minute，Second，Frame)转化最好将上述三种格式分开建类，或做成动态连接库。在 Project-- >Setting-- >Link-- >Object/library modules中加入winmm.lib，源程序中包含。 

---- MCI调用简单，功能强大，可以满足日常多媒体编程的基本需要。但是，MCI一次只能播放一个文件，使用DirectSound技术可以实现八个以上WAV文件的同时播放。

