# MFC疑难注解：CAsyncSocket及CSocket - 三少GG - CSDN博客
2012年08月02日 21:00:02[三少GG](https://me.csdn.net/scut1135)阅读数：867标签：[mfc																[socket																[多线程																[任务																[delete																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
MFC疑难注解：CAsyncSocket及CSocket
MFC对SOCKET编程的支持其实是很充分的，然而其文档是语焉不详的。以至于大多数用VC编写的功能稍复杂的网络程序，还是使用API的。故CAsyncSocket及CSocket事实上成为疑难，群众多敬而远之。余好事者也，不忍资源浪费，特为之注解。
一、CAsyncSocket与CSocket的区别
前者是异步通信，后者是同步通信；前者是非阻塞模式，后者是阻塞模式。另外，异步非阻塞模式有时也被称为长连接，同步阻塞模式则被称为短连接。为了更明白地讲清楚两者的区别，举个例子：设想你是一位体育老师，需要测验100位同学的400米成绩。你当然不会让100位同学一起起跑，因为当同学们返回终点时，你根本来不及掐表记录各位同学的成绩。
如果你每次让一位同学起跑并等待他回到终点你记下成绩后再让下一位起跑，直到所有同学都跑完。恭喜你，你已经掌握了同步阻塞模式。
你设计了一个函数，传入参数是学生号和起跑时间，返回值是到达终点的时间。你调用该函数100次，就能完成这次测验任务。这个函数是同步的，因为只要你调用它，就能得到结果；这个函数也是阻塞的，因为你一旦调用它，就必须等待，直到它给你结果，不能去干其他事情。
如果你一边每隔10秒让一位同学起跑，直到所有同学出发完毕；另一边每有一个同学回到终点就记录成绩，直到所有同学都跑完。恭喜你，你已经掌握了异步非阻塞模式。
你设计了两个函数，其中一个函数记录起跑时间和学生号，该函数你会主动调用100次；另一个函数记录到达时间和学生号，该函数是一个事件驱动的callback函数，当有同学到达终点时，你会被动调用。你主动调用的函数是异步的，因为你调用它，它并不会告诉你结果；这个函数也是非阻塞的，因为你一旦调用它，它就马上返回，你不用等待就可以再次调用它。但仅仅将这个函数调用100次，你并没有完成你的测验任务，你还需要被动等待调用另一个函数100次。
当然，你马上就会意识到，同步阻塞模式的效率明显低于异步非阻塞模式。那么，谁还会使用同步阻塞模式呢？
不错，异步模式效率高，但更麻烦，你一边要记录起跑同学的数据，一边要记录到达同学的数据，而且同学们回到终点的次序与起跑的次序并不相同，所以你还要不停地在你的成绩册上查找学生号。忙乱之中你往往会张冠李戴。
你可能会想出更聪明的办法：你带了很多块秒表，让同学们分组互相测验。恭喜你！你已经掌握了多线程同步模式！每个拿秒表的同学都可以独立调用你的同步函数，这样既不容易出错，效率也大大提高，只要秒表足够多，同步的效率也能达到甚至超过异步。
可以理解，你现的问题可能是：既然多线程同步既快又好，异步模式还有存在的必要吗？
很遗憾，异步模式依然非常重要，因为在很多情况下，你拿不出很多秒表。你需要通信的对端系统可能只允许你建立一个SOCKET连接，很多金融、电信行业的大型业务系统都如此要求。
现在，你应该已经明白了：CAsyncSocket用于在少量连接时，处理大批量无步骤依赖性的业务。CSocket用于处理步骤依赖性业务，或在可多连接时配合多线程使用。
二、CAsyncSocket异步机制
当你获得了一个异步连接后，实际上你扫除了发送动作与接收动作之间的依赖性。所以你随时可以发包，也随时可能收到包。发送、接收函数都是异步非阻塞的，顷刻就能返回，所以收发交错进行着，你可以一直工，保持很高的效率。但是，正因为发送、接收函数都是异步非阻塞的，所以仅调用它们并不能保障发送或接收的完成。例如发送函数Send，调用它可能有4种结果：
1、错误，Send()==SOCKET_ERROR，GetLastError()!=WSAEWOULDBLOCK，这种情况可能由各种网络问题导致，你需要马上决定是放弃本次操作，还是启用某种对策。
2、忙，Send()==SOCKET_ERROR，GetLastError()==WSAEWOULDBLOCK，导致这种情况的原因是，你的发送缓冲区已被填满或对方的接受缓冲区已被填满。这种情况你实际上不用马上理睬。因为AsyncSocket会记得你的Send WSAEWOULDBLOCK了，待发送的数据会写入CAsyncSocket内部的发送缓冲区，并会在不忙的时候自动调用OnSend，发送内部缓冲区里的数据。
3、部分完成，0<Send(pBuf,nLen)<nLen，导致这种情况的原因是，你的发送缓冲区或对方的接收缓冲区中剩余的空位不足以容纳你这次需要发送的全部数据。 处理这种情况的通常做法是继续发送尚未发送的数据直到全部完成或WSAEWOULDBLOCK。这种情况很容易让人产生疑惑，既然缓冲区空位不足，那么本次发送就已经填满了缓冲区，干嘛还要继续发送呢，就像WSAEWOULDBLOCK了一样直接交给OnSend去处理剩余数据的发送不是更合理吗？然而很遗憾，CAsyncSocket不会记得你只完成了部分发送任务从而在合适的时候触发OnSend，因为你并没有WSAEWOULDBLOCK。你可能认为既然已经填满缓冲区，继续发送必然会WSAEWOULDBLOCK，其实不然，假如WSAEWOULDBLOCK是由于对方读取接收缓冲区不及时引起的，继续发送的确很可能会WSAEWOULDBLOCK，但假如WSAEWOULDBLOCK是由于发送缓冲区被填满，就不一定了，因为你的网卡处理发送缓冲区中数据的速度不见得比你往发送缓冲区拷贝数据的速度更慢，这要取决与你竞争CPU、内存、带宽资源的其他应用程序的具体情况。假如这时候CPU负载较大而网卡负载较低，则虽然刚刚发送缓冲区是满的，你继续发送也不会WSAEWOULDBLOCK。
4、完成，Send(pBuf,nLen)==nLen与OnSend协助Send完成工作一样，OnRecieve、OnConnect、OnAccept也会分别协助Recieve、Connect、Accept完成工作。这一切都通过消息机制完成：
在你使用CAsyncSocket之前，必须调用AfxSocketInit初始化WinSock环境，而AfxSocketInit会创建一个隐藏的CSocketWnd对象，由于这个对象由Cwnd派生，因此它能够接收Windows消息。所以它能够成为高层CAsyncSocket对象与WinSock底层之间的桥梁。例如某CAsyncSocket在Send时WSAEWOULDBLOC了，它就会发送一条消息给CSocketWnd作为报告，CSocketWnd会维护一个报告登记表，当它收到底层inSock发出的空闲消息时，就会检索报告登记表，然后直接调用报告者的OnSend函数。所以前文所说的CAsyncSocket会自动调用OnXxx，实际上是不对的，真正的调用者是CSocketWnd——它是一个CWnd对象，运行在独立的线程中。
使用CAsyncSocket时，Send流程和Recieve流程是不同的，不理解这一点就不可能顺利使用CAsyncSocket。MSDN对CAsyncSocket的解释很容易让你理解为：只有OnSend被触发时你Send才有意义，你才应该Send，同样只有OnRecieve被触发时你才应该Recieve。很不幸，你错了：
你会发现，连接建立的同时，OnSend就第一次被触发了，嗯，这很好，但你现在还不想Send，你让OnSend返回，干点其他的事情，等待下一次OnSend试试看？实际上，你再也等不到OnSend被触发了。因为，除了第一次以外，OnSend的任何一次触发，都源于你调用了Send，但碰到了WSAEWOULDBLOCK！
所以，使用CAsyncSocket时，针对发送的流程逻辑应该是：你需两个成员变量，一个发送任务表，一个记录发送进度。你可以，也应该，在任何你需要的时候，主动调用Send来发送数据，同时更新任务表和发送进度。而OnSend，则是你的负责擦屁股工作的助手，它被触发时要干的事情就是根据任务表和发送进度调用Send继续发。若又没能将任务表全部发送完成，更新发送进度，退出，等待下一次OnSend；若任务表已全部发送完毕，则清空任务表及发送进度。
使用CAsyncSocket的接收流程逻辑是不同的：你永远不需要主动调用Recieve，你只应该在OnRecieve 中等待。由于你不可能知道将要抵达的数据类型及次序，所以你需要定义一个已收数据表作为成员变量来存储已收到但尚未处理的数据。每次OnRecieve被触发，你只需要被动调用一次Recieve来接受固定长度的数据，并添加到你的已收数据表后。然后你需要扫描已收数据表，若其中已包含一条或数条完整的可解析的业务数据包，截取出来，调用业务处理窗口的处理函数来处理或作为消息参数发送给业务处理窗口。而已收数据表中剩下的数据，将等待下次OnRecieve中被再次组合、扫描并处理。
在长连接应用中，连接可能因为各种原因中断，所以你需要自动重连。你需要根据 CAsyncSocket的成员变量m_hSocket来判断当前连接状态：if(m_hSocket==INVALID_SOCKET)。当然，很奇怪的是，即使连接已经中断，OnClose也已经被触发，你还是需要在OnClose中主动调用Close，否则m_hSocket并不会被自动赋值为INVALID_SOCKET。
在很多长连接应用中，除建立连接以外，还需要先Login，然后才能进行业务处理，连接并Login是一个步骤依赖性过程，用异步方式处理反而会很麻烦，而CAsyncSocket是支持切换为同步模式的，你应该掌握在适当的时候切换同异步模式的方法：
DWORD dw;
//切换为同步模式
dw=0;
IOCtl(FIONBIO,&dw);
...
//切换回异步模式
dw=1;
IOCtl(FIONBIO,&dw);
三、CSocket的用法
CSocket在CAsyncSocket的基础上，修改了Send、Recieve等成员函数，帮你内置了一个用以轮询收发缓冲区的循环，变成了同步短连接模式。短连接应用简单明了，CSocket经常不用派生就可以直接使用，但也有些问题：
1、用作监听的时候
曾经看到有人自己创建线程，在线程中创建CSocket对象进行Listen、Accept，若Accept成功则再起一个线程继续Listen、Accept。可以说他完全不理解CSocket，实际上CSocket的监听机制已经内置了多线程机制，你只需要从CSocket派生，然后重载OnAccept：
//CListenSocket头文件
class CListenSocket : public CSocket
{
public:
    CListenSocket(HWND hWnd=NULL);
    HWND m_hWnd; //事件处理窗口
    virtual void OnAccept(int nErrorCode);
};
//CListenSocket实现文件
#include "ListenSocket.h"
CListenSocket::CListenSocket(HWND hWnd){m_hWnd=hWnd;}
void CListenSocket::OnAccept(int nErrorCode)
{
    SendMessage(m_hWnd,WM_SOCKET_MSG,SOCKET_CLNT_ACCEPT,0);
    CSocket::OnAccept(nErrorCode);
}
//主线程
...
m_pListenSocket=new CListenSocket(m_hWnd);
m_pListenSocket->Create(...);
m_pListenSocket->Listen();
...
LRESULT CXxxDlg::OnSocketMsg(WPARAM wParam, LPARAM lParam)
{
    UINT type=(UINT)wParam;
    switch(type)
    {
    case SOCKET_CLNT_ACCEPT:
        {
            CSocket* pSocket=new CSocket;
            if(!m_pListenSocket->Accept(*pSocket))
            {
                delete pSocket;
                break;
            }
            ...
        }
    ...
    }
}
2、用于多线程的时候
常看到人说CSocket在子线程中不能用，其实不然。实际情况是：
直接使用CSocket动态创建的对象，将其指针作为参数传递给子线程，则子线程中进行收发等各种操作都没问题。但如果是使用CSocket派生类创建的对象，就要看你重载了哪些方法，假如你仅重载了OnClose，则子线程中你也可以正常收发，但不能Close!
因为CSocket是用内部循环做到同步的，并不依赖各OnXxx，它不需要与CSocketWnd交互。但当你派生并重载OnXxx后，它为了提供消息机制就必须与CSocketWnd交互。当你调用AfxSocketInit时，你的主线程会获得一个访问CSocketWnd的句柄，对CSocketWnd的访问是MFC自动帮你完成的，是被隐藏的。而你自己创建的子线程并不自动具备访问CSocketWnd的机制，所以子线程中需要访问CSocketWnd的操作都会失败。
常看到的解决办法是给子线程传递SOCKET句柄而不是 CSocket对象指针，然后在子线程中创建CSocket临时对象并Attach传入的句柄，用完后再Dettach并delete临时对象。俺没有这么干过，估计是因为Attach方法含有获取CSocketWnd句柄的内置功能。
俺的解决方案还是使用自定义消息，比如俺不能在子线程中Close，那么，俺可以给主线程发送一条消息，让主线程的消息处理函数来完成Close，也很方便。
CSocket一般配合多线程使用，只要你想收发数据，你就可以创建一个CSocket对象，并创建一个子线程来进行收发。所以被阻塞的只是子线程，而主线程总是可以随时创建子线程去帮它干活。由于可能同时有很多个CSocket对象在工作，所以你一般还要创建一个列表来储存这些CSocket对象的标识，这样你可能通过在列表中检索标识来区分各个CSocket对象，当然，由于内存地址的唯一性，对象指针本身就可以作为标识。
相对CAsyncSocket而言，CSocket的运作流程更直观也更简单，至于CSocketFile、CArchive之类的，似乎
也不需要多说什么，就这样结束吧。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/fangle6688/archive/2006/07/27/986808.aspx](http://blog.csdn.net/fangle6688/archive/2006/07/27/986808.aspx)
