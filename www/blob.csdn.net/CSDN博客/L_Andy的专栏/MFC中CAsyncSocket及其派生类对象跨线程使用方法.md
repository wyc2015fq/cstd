# MFC中CAsyncSocket及其派生类对象跨线程使用方法 - L_Andy的专栏 - CSDN博客

2015年01月15日 15:39:06[卡哥](https://me.csdn.net/L_Andy)阅读数：1079



MFC中CAsyncSocket及其派生类对象跨线程使用方法

存在的现象

在MFC中用多线程方法开发WinSocket通讯程序时，如果你的的是API方式，自然没有以下说的问题。但如果当你使用CAsyncSocket及其派生类(CSocket或是你自己的写的)来开发的话，会发现在不同线程中使用CAsyncSocket及其派生类对象时，会出现程序崩溃。这里所说的跨线程，是指该对象在一个线程中调用Create/Close/Attach/Detach函数，然后在另外一个线程中调用其他成员函数如Receive/ReceiveFrom/Send/SendTo等。下面是一个典型的导致崩溃的例子程序(片断):

CAsyncSocket async_socket;

UINT ThreadProc(LPVOID) //出错:1

{

   TRACE("======== ThreadDeattch出错:1 ========/n");

    async_socket.Close (); //错误发生在这个调用上:要进行关闭async_socket对象.

    return 0;

}

void CTsAsncSocketDlg::OnBnClickedButtonCreate() //出错:1

{

    async_socket.Create(0);//async_socket对象在主线程中被建立

    ::AfxBeginThread(ThreadProc,0,0,0,0,0); //async_socket对象在主线程中被传递到另外的线程:ThreadProc

}

要想知道错误的原因，可以跟进去分析。这样说起来又太繁琐了，网上有不少类似的文章，问题的关键是不要在不同线程中进行Create/Close调用。

首先，当你的程序有问题的时候，怎样来判断是因为这个问题?这是关键一步。调试/断点/观测等有很多手段，VC++的IDE在个这个方面是非常出色的。这里还有个简单的方法。比如有如下代码:

class CSocketP3PServer : public CAsyncSocket

{

public:

 CSocketP3PServer ();

 ~ CSocketP3PServer ();

public:

    .......... //others

#if _DEBUG

private:

  DWORD m_sockCurrentThreadId;

#endif //_DEUBG

};

void CSocketP3PServer::CSocketP3PServer()

{

#if _DEBUG

      m_sockCurrentThreadId = ::GetCurrentThreadId();       

#endif //_DEBUG

}

CSocketP3PServer::~CSocketP3PServer()

{

#if _DEBUG

 if(m_sockCurrentThreadId != ::GetCurrentThreadId())

 {

  //如果程序走到这里来了，那么应该是跨线程使用造成的问题

  TRACE("======= CSocketP3PServer 对象(Address:%d)在不同线程调用了 ========/n",this);

 }

#endif //_DEBUG

}

知道了问题所在，解决的办法就很好说了，网上也有很多相关的介绍文章。这里是把这些解决方法整理一下。

解决的思路主要有2条：

 1：避免这个问题的出现，也就是说避免在跨线程中使用CAsyncSocket及其派生类对象。

           关于这点有很多的招数，比如说可以 A线程专门作收发工作，而其它线程把相关的数据通过数据共享或是其它手段在彼此之间传递。这个就是一个<读者－作者>之间的关系。还有其它的办法，总之，就是要把socket对象整个生命期都在单一的线程上。

 2：实在是没有办法避免不在跨线程中使用CAsyncSocket及其派生类对象的，可以通过在线程之间传递SOCKET <变量>来实现，这里给个例子，代码如下：

UINT ThreadDeattchMe(LPVOID pv) //正确的在跨线程使用as_socket，这里是工作线程A

{

 TRACE("======== ThreadDeattch 正确的在跨线程使用SOCKET ========/n");

 as_socket.Attach((SOCKET)pv); //<---------- 注意这里传递的是 SOCKET <变量>

     as_socket.Close ();

 return 0;

}

void CTsAsncSocketDlg::OnBnClickedButtonCreate()//正确的在跨线程使用SOCKET,这里是主线程

{

     as_socket.Create(0);

 AfxBeginThread(ThreadDeattchMe,(LPVOID)as_socket.Detach(),0,0,0,0);//<---------- 注意这里传递的是 SOCKET <变量>

}

