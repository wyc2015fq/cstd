# Socket为解决粘包设计的环形缓冲区 - 建建的博客 - CSDN博客
2017年02月03日 11:54:52[纪建](https://me.csdn.net/u013898698)阅读数：1907
前段时间实现了一个小型的C/S[架构](http://lib.csdn.net/base/architecture)的多人在线即时通信工具，JIGQQ。其中对使用TCP通信有点心得。 记得在我大学时代，就用VB做过TCP的通信。当然那时候是很初级的，发送的数据量也很小的应用。当时就觉得，有时候发送的数据接收端不能接收到，有时候呢觉得一次性没有接受完毕。
前段时间实现了一个小型的C/S架构的多人在线即时通信工具，JIGQQ。其中对使用TCP通信有点心得。
记得在我大学时代，就用VB做过TCP的通信。当然那时候是很初级的，发送的数据量也很小的应用。当时就觉得，有时候发送的数据接收端不能接收到，有时候呢觉得一次性没有接受完毕，总会丢失一些内容。这和我从书本学到的TCP/IP可靠通信协议的知识完全不匹配，这让我疑惑了很久。直到后来接触的东西越来越多，眼界也逐渐开拓才慢慢意识到问题出在哪。
应用平台：Windows XP
开发工具：C++ Builder 6.0
**问题描述**
    我在使用Socket接收消息时，将会触发一个接收函数。（BCB中的是ClientRead函数）所以我在此函数处接收信息，并做相应处理。那问题来了：由于传输的数据包都是我自定义的，我明确的知道长度为多多少。可实际效果却是，有时候接不够我期待的长度，而甚至有时候一次接收的数据包长度竟然比我预期的要长10个字节。当时只有设定条件将不满足我预期长度的数据包丢弃。
**问题分析**
看了上面描述，想必大家也明白我的错误在哪了吧？实际是我对Socket的接收机制理解有误。TCP/IP只保证发送包按顺序到达目的地，但可能由于网络状况他会自动分包发送，这样就导致接收端的接受函数每次提交时只有若干数据，不一定是我预期的一个完整的包。可以这样理解，发过来的实际是一个‘流’。
看来要很好的解决这个问题，那就只有先将接收的数据保存起来，再来做处理。
**处理模型**
为了要保存接收数据，我们首先就要建立一个缓冲区。那第一个问题来了：由于我们要接收的信息是不可预知的，那难道这个缓冲区要无限的扩容？
可我们的实际PC内存肯定是有限的，所以我们必须建立一套内存缓冲区可以被反复利用的机制 —— 环形队列。
我们用图来说明环形队列的工作原理：
![](http://hi.csdn.net/attachment/201004/13/0_1271166778x12F.gif)
图1 蓝色为写入的数据，绿色为已经读取处理的数据
看上图1，在正常状态下：Write指针在写入数据，而Read指针在Write指针之前，说明缓冲区后端还有空余空间。
在指针回滚状态下：Write指针在Read指针之前，说明缓冲区的前端已经有空闲的空间。
除了这两种状态外，我们不得不再考虑一种即将错误状态：
![](http://hi.csdn.net/attachment/201004/13/0_12711667937QSf.gif)
图2蓝色为写入的数据，棕红色为未处理的数据
看图2，无足够空间：当Write指针回滚，发现无足够空间，将和Read指针发生交集（虚点部分）这显然是不合理的。一部分未处理数据将被覆盖破环。所以我们必须重新调整整个缓冲区。
重新分配调整：当遇到空间不足，不能实现Write指针回滚的情况，我们只有重新开辟一个更大一点的缓冲区，并把未处理数据（棕红色）和写入数据（蓝色）按顺序复制到新的缓冲区内，并调整好Read和Write指针的位置。最后释放掉原来的缓冲区。
我们可以看到，经过这样一个过程，我们的缓冲区，将在Read指针处理速度较慢并在处理信息量增大时，逐渐扩容。但是，当扩容到一定程度，将达到一个平衡。因为信息量不可能无限增大，当需处理信息量达到最大值再结合Read指针的不断处理，缓冲区的大小也将稳定下来。
我们一开头就给此缓冲区命名为‘环形队列’。从以上的图和文字，我们可以形象的理解：由于缓冲区大小最终将稳定，Write和Read指针将无障碍的在缓冲区中不断循环回滚，其运行轨迹，将是一个环形。
**其他知识**
为了要实现上述模型，我们必须要具备一些知识。
（Write和Read是相对缓冲区而言的！）
首先，**Wirte指针部分**，应该在Socket接收函数中去实现。他什么都不干，只管将接收到的数据往环形队列中存放就行了。
再一个，就是我们的**Read指针部分**。他需要尽可能快的在环形队列中读取已经储存好的数据，并解析数据后进行相关的操作。最重要一点是，这个过程必须是独立的，在主线程之外运行。
所以，我们的Write指针部分应该是在主线程，而Read指针部分就必须建立一个额外的线程来进行工作。
下面我们就看看C++ Builder 6.0下如何实现多线程和需注意的相关事宜。
1.深浅相关全局或私有变量
HANDLEDealInfHanld;        // 处理各类信息线程句柄
DWORDDealInfID;             // 处理各类信息线程ID
CRITICAL_SECTIONm_csLock; // 用于临界变量互锁
2.编写好一个线程处理函数实体
// 处理环形队列中各类信息
DWORD __stdcall DealInf(LPVOID)
{
// ……
}
3.利用CreateThread函数创建线程
在窗体开被创建时就创建好线程处理函数：
void __fastcall TForm_Main::FormCreate(TObject *Sender)
{
// ……
    ::InitializeCriticalSection(&m_csLock);
// 开辟各类信息处理线程
DealInfHanld = CreateThread(NULL, 0, DealInf, NULL, 0, &DealInfID);
}
进过上面3个步骤，函数 DealInf(LPVOID) 将被创建为一个线程函数，他将独立在主线程之外独立运行。
我们还需要注意一个很重要的问题：由于线程函数中操作的有关内存，很可能也是主线程中要操作的。比如我们的环形队列就是这样。其中Read指针在线程中读取数据进行处理，而Write指针却在主线程中储存数据。所以此两指针应该要是‘不可见’的，即他们不应该同时去操作同一数据区，总不可能要CPU同时对同一个内存地址又读又写吧？所以我们必须利用**互锁机制**，让Write和Read操作互斥。
这点很好实现，比如 DealInf(LPVOID) 函数，我们只需在函数主体的开始和结束处加锁定和解锁代码即可：
// 处理环形队列中各类信息
DWORD __stdcall DealInf(LPVOID)
{
    ::EnterCriticalSection(&m_csLock);   // 锁定
// ……
    ::LeaveCriticalSection(&m_csLock);   // 解锁
}
当然我们应该在主线程Sockte接收函数处也这样锁定和解锁。
**说明：**
下文代码中，DealInfMem为缓冲区，MemcpySize为缓冲区大小，ReadIndex为读游标，WriteIndex为写游标。SocketUS为数据包开头的TCustomWinSocket信息，BufferSize为数据包长度。
**具体Write部分实现**
首先我们还需要来讨论下，如何实现自定义数据包能在我们实际编码的时带来便利，并最好做到自定义数据包能无限扩展。
我个人认为，在自定义数据包时，必须将此数据包的实际长度信息包含进去，这样可以有利于信息在接收时能有判断的依据。
举例说明：
// 加为好友命令
typedef struct QQMAKEFRIEND
{
UINT32BufferLeng; // 本数据包长度
UINT8Order;       // 命令ID
UINT32QQNumMy;    // 自己的QQ号码
UINT32QQNumFD;    // 好友的QQ号码
} QQMakeFriend;
如上面的一个加好友命令，我们应该在数据包的最前端4字节用来标记数据包长度。这样我们在信息接收函数时，就可以有效判断我们是否已经将此数据包接收完毕。代码如下：
// 接收各客户端信息
// 获取此次接收到的全部数据
do
{
t++;
LengBuffer = Socket->ReceiveLength(); 
Socket->ReceiveBuf(ReadBuffer + LengOff + sizeof(TCustomWinSocket*), LengBuffer);  
LengOff += LengBuffer;    // 累计接收到的数据长度
// 获取此次数据包长度，并把Soket连接记录在数据包中去
    if (t == 1)
    {
memcpy(ReadBuffer, &Socket, sizeof(TCustomWinSocket*));
memcpy(&ReLeng, ReadBuffer + sizeof(TCustomWinSocket*), 4);
    }
}while (LengOff != ReLeng);    // 若此数据包接收完毕，跳出
LengBuffer = LengOff + sizeof(TCustomWinSocket*);
经过上面的代码，我们就可以将每次发送来的数据包完整的记录在字节数组ReadBuffer中，以便后面使用。
而且这样将数据包长度信息加在数据包中，还便于变长数据包的传输。比如当发送聊天信息的时候，由于我们的信息是不定长的，如果知道数据包长度便于我们完整接收数据包。并且这些信息在Read指针读取的时候也发挥巨大作用，后面我们将看到。
接下来，我们就是将接收到的数据合理的储存到环形队列中。
通过图1和图2我们清楚的知道在将数据包放入环形队列的时候，有3种情形：
1.环形队列后面还有足够空余空间（R追W）
2.环形队列后面没有足够空余空间但前端有，这样实现回滚（W超R不足一圈）
3.环形队列前后都没有足够的空余空间，这样重新开辟更大的缓冲区。复制好数据调整读写指针，并释放原来缓冲区。（W超R一圈）
我们就根据上面三中2情形实现代码如下：
// 向环形队列填塞信息
::EnterCriticalSection(&m_csLock); // 锁定
if (WriteIndex >= ReadIndex) // Write指针在Read指针之后
{
    if ((MemcpySize - WriteIndex) >= LengBuffer) // 说明环行队列后面有空闲位置
    {
memcpy(DealInfMem + WriteIndex, ReadBuffer, LengBuffer);
WriteIndex += LengBuffer;
    }
    else // 后面没有空闲位置，往前面找
    {
       if ((MemcpySize-WriteIndex+ReadIndex) > LengBuffer) // 如果前面有空闲位置
       {
memcpy(DealInfMem + WriteIndex, ReadBuffer, MemcpySize - WriteIndex);
memcpy(DealInfMem, ReadBuffer + (MemcpySize - WriteIndex), LengBuffer -
 (MemcpySize - WriteIndex));
WriteIndex = LengBuffer - (MemcpySize - WriteIndex);
       }
       else // 说明没有空余空间，重新开辟缓冲区
       {
           while ((DealInfMem2 = (UINT8*)malloc(MemcpySize + MEMSIZE + LengBuffer))
 == NULL);
memcpy(DealInfMem2, DealInfMem, MemcpySize);
MemcpySize += (MEMSIZE + LengBuffer);
free(DealInfMem);
DealInfMem = DealInfMem2;
memcpy(DealInfMem + WriteIndex, ReadBuffer, LengBuffer);
WriteIndex += LengBuffer;
       }
    }
}
else // Write指针在Read指针之前
{
    if ((ReadIndex-WriteIndex) > LengBuffer) // 有空余空间
    {
memcpy(DealInfMem+WriteIndex, ReadBuffer, LengBuffer);
WriteIndex += LengBuffer;
    }
    else // 无空余空间，重新开辟缓冲区
    {
       while ((DealInfMem2 = (UINT8*)malloc(MemcpySize + MEMSIZE + LengBuffer))
 == NULL);
memcpy(DealInfMem2, DealInfMem + ReadIndex, MemcpySize - ReadIndex);
memcpy(DealInfMem2 + MemcpySize - ReadIndex, DealInfMem, WriteIndex);
memcpy(DealInfMem2 + MemcpySize - ReadIndex + WriteIndex, ReadBuffer, LengBuffer);
WriteIndex = MemcpySize - ReadIndex + WriteIndex + LengBuffer;
MemcpySize += (MEMSIZE + LengBuffer);
ReadIndex = 0;
free(DealInfMem);
DealInfMem = DealInfMem2;
    }
}
::LeaveCriticalSection(&m_csLock); // 解锁
这样，我们就实现了Write指针储存数据的部分。
**具体Read部分实现**
    首先我们要看看Read指针在读取数据时，有不有特别指出。
1.按顺序读取数据；
2.如果数据有一部分在环形队列尾部，而其余回滚到前端，那我们必须获取这两部分合并成一个数据包再来使用；
3.如何确定当前数据包应该有多长？我们本来就记录当前数据包长度，我们只要根据此信息在环形队列中读取对应长度的数据就可以。
我们来看看具体代码实现：
while (1)
{
    ::EnterCriticalSection(&m_csLock); // 锁定
TimeSendHand++;
// 此处为定时挂起此线程，以便主线程响应消息，不然CPU占用比将飙升
    if (TimeSendHand >= 100000)
    {
TimeSendHand = 0;
Sleep(1);
    }
// 若发现读写指针重叠，说明当前没有可处理的数据，放回等待
    if (WriteIndex == ReadIndex)
    {
       ::LeaveCriticalSection(&m_csLock); // 解锁
Sleep(1);
       continue;
    }
    if (WriteIndex > ReadIndex) // 说明读取指针在写入指针之前（W > R）
    {
// 若目前接收的数据不够，返回等待
       if ((WriteIndex - ReadIndex) <= (sizeof(TCustomWinSocket*) + 4))
       {
           ::LeaveCriticalSection(&m_csLock); // 解锁
Sleep(1);
           continue;
       }
// 摘取Socket连接记录，用于对应发送
memcpy(&SocketUS, DealInfMem + ReadIndex, sizeof(TCustomWinSocket*));
// 摘取实际数据包长度记录（后续4个字节为长度信息）
memcpy(&BufferSize, DealInfMem + ReadIndex+sizeof(TCustomWinSocket*),
 4);
// 实际数据包内容不完整，返回等待
       if ((WriteIndex - ReadIndex - sizeof(TCustomWinSocket*)) < BufferSize)
       {
           ::LeaveCriticalSection(&m_csLock);
Sleep(1);
           continue;
       }
memcpy(XCBuffer, DealInfMem + ReadIndex + sizeof(TCustomWinSocket*), BufferSize); // 摘取实际数据包内容（包括长度信息）
ReadIndex = ReadIndex + sizeof(TCustomWinSocket *) + BufferSize; // 读指针后移
    }
    else // 说明读取指针在写入指针之后(W <= R则反超，若W == R则重叠，若W
 < R则W反追R)
    {
// 若目前接收的数据不够，返回等待
// MemcpySize - ReadIndex + WriteIndex为后面未读的加前面未读的
       if ((MemcpySize - ReadIndex + WriteIndex) <= (sizeof(TCustomWinSocket*) + 4))
       {
           ::LeaveCriticalSection(&m_csLock);
Sleep(1);
           continue;
       }
// 将 sizeof(TCustomWinSocket *)+4 的信息先放入XCBuffer，以便利用
       if ((MemcpySize - ReadIndex) < (sizeof(TCustomWinSocket*) + 4)) // 后不够
       {
memcpy(XCBuffer, DealInfMem + ReadIndex, MemcpySize - ReadIndex); //后
memcpy(XCBuffer + MemcpySize - ReadIndex, DealInfMem, sizeof(TCustomWinSocket*)
 + 4 - (MemcpySize -ReadIndex)); // 前
       }
       else // 后够
       {
memcpy(XCBuffer, DealInfMem + ReadIndex, sizeof(TCustomWinSocket*)
 + 4);
       }
// 摘取Socket
memcpy(&SocketUS, XCBuffer, sizeof(TCustomWinSocket*));
// 摘取实际数据包长度记录
memcpy(&BufferSize, XCBuffer + sizeof(TCustomWinSocket*), 4);
       if ((MemcpySize - ReadIndex + WriteIndex - sizeof(TCustomWinSocket*)) < BufferSize) // 实际数据包内容不完整，返回等待
       {
           ::LeaveCriticalSection(&m_csLock);
Sleep(1);
           continue;
       }
// 摘取实际数据包内容
// 若数据就完整的在一起
       if ((MemcpySize-ReadIndex) <= (sizeof(TCustomWinSocket*)))
       {
memcpy(XCBuffer, DealInfMem + sizeof(TCustomWinSocket*) - (MemcpySize - ReadIndex), BufferSize);
ReadIndex = sizeof(TCustomWinSocket*) - (MemcpySize - ReadIndex) + BufferSize;
       }
       else // 数据一部分在缓冲区后端，其他部分在缓冲区前端
       {
memcpy(XCBuffer, DealInfMem + ReadIndex + sizeof(TCustomWinSocket*),
MemcpySize - ReadIndex - sizeof(TCustomWinSocket*));
memcpy(XCBuffer + MemcpySize - ReadIndex - sizeof(TCustomWinSocket*), DealInfMem,  BufferSize -
 (MemcpySize- ReadIndex - sizeof(TCustomWinSocket*)));
ReadIndex = BufferSize - (MemcpySize - ReadIndex - sizeof(TCustomWinSocket*));
       }
    }
XCBufferOff = 0;
// ……
    // 根据信息获取命令号处理
memcpy(&Order, XCBuffer + 4 + XCBufferOff, 1);
    switch (Order)
    {
    case QQMSGHEAD_ORDER: // 接收消息命令
// ……
           break; // Break;
// ……
    }
    ::LeaveCriticalSection(&m_csLock); // 解锁
}
经过上面的代码，一个个实际数据包将被放入字节数组XCBuffer中。并获取了此条信息对应的Socket记录SocketUS。
好了，经过以上的讲解，我们这个TCP通信处理模型就建好了。这个模型可以固定不变的应用于C/S架构的通信。上面代码红色部分就是各命令信息处理的部分，并且我们的命令数据包可以任意自定义，只要确保数据包的前4字节记录好当前数据包长度。
下面给出具体的实际代码：（C++ Builder 6.0实现）
**全局变量**
HANDLEDealInfHanld;    // 处理各类信息线程句柄
DWORDDealInfID;        // 处理各类信息线程ID
// 环行队列指针
UINT8   *DealInfMem;             // 环形队列指针
UINT32MemcpySize = MEMSIZE;  // 环行队列尺寸
UINT32WriteIndex = 0, ReadIndex = 0; // 读写游标
CRITICAL_SECTIONm_csLock;             // 用于临界变量互锁
UINT8ReadBuffer[1024*1024];           // 用于接收消息的缓存（1M）
UINT8XCBuffer[1024*1024];             // 用于线程中的数据包摘取（1M）
**Write指针部分**
void __fastcall TForm_Main::ServerSocket1ClientRead(TObject *Sender, TCustomWinSocket *Socket)
{
// 接收各客户端信息
UINT32LengBuffer, LengOff = 0;
UINT32ReLeng;
UINT8t = 0;
UINT8   *DealInfMem2;
// 获取此次接收到的全部数据
do
    {
t++;
LengBuffer = Socket->ReceiveLength();
Socket->ReceiveBuf(ReadBuffer + LengOff + sizeof(TCustomWinSocket*), LengBuffer);
LengOff += LengBuffer;
        if (t == 1)
        {
memcpy(ReadBuffer, &Socket, sizeof(TCustomWinSocket*));
memcpy(&ReLeng, ReadBuffer + sizeof(TCustomWinSocket*), 4);
        }
    }while (LengOff != ReLeng);
LengBuffer = LengOff + sizeof(TCustomWinSocket*);   
// 向环形队列填塞信息
// 以下同具体Write部分实现中的代码
}
**Read指针部分**
// 处理环形队列中各类信息
DWORD __stdcall DealInf(LPVOID)
{
AnsiStringAddSQL;
    static UINT32TimeSendHand = 0;
UINT32BufferSize, XCBufferOff = 0;
UINT8Order;
TCustomWinSocket   *SocketUS;
// 以下同具体Read部分实现中的代码
}
感兴趣的朋友，可以直接把以上代码复制到C++ Builder 6.0中去实验。几乎不用该任何代码，就能为你实现一个使用TCP/IP协议接收处理的信息的通信系统。
**通信模块的使用说明**
（1）应用平台及开发工具：
Windows XP，C++ builder 6.0
（2）代码组成：
头文件：MyThread.h
Cpp：MyThread.cpp
（3）功能描述：
 以环形队列为储存基础和类MyThread为多线程基础的通信模块。由于有环形队列实现，以保证完全无遗漏的接收数据并等待处理。MyThread多线程类由BCB提供的类派生，保证了可操作性和稳定性。
（4）使用要点
 数据写入：用户在自己的数据接收函数处，调用MyThread类成员函数MyThread_WriteData(UINT32 SocketUSAdr, UINT8 *Buffer, UINT32 LengBuffer);即可将数据写入到环形队列中。
SocketUSAdr：用于接收的Socket控件的地址值
Buffer：接收到的数据
LengBuffer：接收到的数据长度
数据摘取使用：MyThread类中的函数指针MyThread_ReadData将被处理子线程自动调用。所以，用户需要在自己的程序中，声明一个函数实现，并将函数地址赋给MyThread_ReadData。如：
void __fastcall MyReadData(UINT32SocketUSAdr, UINT8 *XCBuffer); // 声明的函数
mythread->MyThread_ReadData = MyReadData; //将函数地址赋给函数指针
SocketUSAdr：记录了发送此信息的Socket地址
XCBuffer：实际信息数据包
（5）备注
1>.使用此通信模块，发送的数据包必须满足以下格式：
typedef struct QQMSGHEAD
{
UINT32BufferLeng;  // 本数据包长度
// ……
// ……
} QQMsgHead;
即首4字节必须为此次数据包的长度，以实现报文定界。数据摘取模块就是通过此信息准确提取出每个完整的数据包，并提交给mythread->MyThread_ReadData所指向的函数。
2>.环形队列中数据以以下形式存放：
Socket地址：4字节，是发送此消息的Socket地址。
实际数据：N个字节，建议不超过64K
数据包字节数：4字节
有用信息：N-4字节
由于记录了发送消息的Socket地址，所以此通信模块可以很轻易应用于多人即时在线聊天系统。（本人的JIGQQ已成功实现）
注意：环形队列中的数据是不用用户自己操作的，这里给以说明只是为了用户更好的理解本通信模块
3>.数据摘取函数MyThread_ReadData的实际实现不建议申请临时变量，最好不要。如果要用到变量，请将其申请为全局，或申请为所在类模块的内部成员。
