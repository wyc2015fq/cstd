# BREW应用向其他平台的移植 - 我相信...... - CSDN博客





2011年03月19日 11:43:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1037标签：[brew																[平台																[symbian																[parameters																[callback																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)




把一个BREW上功能移植到Symbian平台为例，看一下具体的实现方式。由于网络应用的重要地位，这里先使用BREW3.x中ISockPort建立一个TCP的连接。首先，初始化服务器的地址：

pME->m_saSockAddr.wFamily = AEE_AF_INET;

pME->m_saSockAddr.inet.port = HTONS(SERVER_PORT);

INET_PTON(pMe->saSockAddr.wFamily, SERVER_ADDR, &(pMe->saSockAddr.inet.addr));

然后创建并打开ISockPort，

ret = ISHELL_CreateInstance(pME->m_pIShell, AEECLSID_SOCKPORT, (void**)&(pME->m_pISockPort));

ret = ISOCKPORT_OpenEx(pME->m_pISockPort, AEE_AF_INET, AEE_SOCKPORT_STREAM, 0);

接下来建立TCP连接，

ret = ISOCKPORT_Connect(pME->m_pISockPort, &pME->m_saSockAddr);

if (AEEPORT_WAIT == ret){

ISOCKPORT_WriteableEx(pME->m_pISockPort,&pME->m_cbWriteCallback, MyApp_TryConnect, pME);

return;

}

建立连接成功后，就可以从服务器读写数据了

ret=ISOCKPORT_Write(pME->m_pISockPort,pME->m_caWriteBuffer + pME->m_nBytesWritten, BUFFER_SIZE - pME->m_nBytesWritten);

// retry later

if (AEEPORT_WAIT == ret){

ISOCKPORT_WriteableEx(pME->m_pISockPort, &pME->m_cbWriteCallback, CApp_TryWrite, pME);

return;

}

最后，取消回调并释放ISockPort接口。

CALLBACK_Cancel(&pME->m_cbReadCallback);

CALLBACK_Cancel(&pME->m_cbWriteCallback);

IBASE_Release((IBase*)(pME->m_pISockPort));



Symbian 是使用C++的，移植以上功能的时候，需要使用Symbian OS 中客户端服务器框架。首先从 CActive 创建自己的对象:

#include <e32base.h>

#include <in_sock.h>

#include <es_sock.h>

class CTCPConnector : public CActive

{

private:

// these are some of the classes relevant to opening a TCP connection:

TInt iState;

RSocket iSocket;

RSocketServ iSocketServer;

RHostResolver iResolver;

TInetAddr iAddress;

…

}

然后定义服务方法

void CTCPConnector::MakeOutgoingConnectionL(const TDesC& aHost, TInt aPort){

...

iState = EGetByName;

iResolver.GetByName( /* parameters required for resolving a host */);

...

}

接下来实现建立连接的方法

void CSEIConnector::ConnectSocketL(void){

...

iSocketServer.Connect();

...

iSocket.Open(iSocketServer, KAfInet, KSockStream, KProtocolInetTcp);

...

iSocket.Connect(/*parameters required connecting */);

...

iSocket.Connect(iAddress, iStatus);

iState = ESocketConnect;

...

}

最后实现RunL() 来处理事件通知:

void CTCPConnector::RunL(){

TInt error = KErrNone;

switch(iState)

{

case EGetByName:

{

ConnectSocketL();

break;

}

case ESocketConnect:

{

ProcessRequestL();

break;

}

}

}

在Symbian中的工作流程是这样的，客户端调用MakeOutgoingConnectionL()， MakeOutgoingConnectionL()首先发起 DNS 查询，如果域名被成功解析，则开始调用CTCPConnector::RunL()。在CTCPConnector::RunL()中，先要检查请求的状态，如果当前状态值是EGetByName就可以调用ConnectSocketL()了。在ConnectSocketL()中，要创建客户端和服务器的类以及相应的RSocket 和 RSocketServer。最后，调用RSocket::Connect() 来创建正在的TCP连接，无论连接成功或者失败，都将再一次运行RunL()，具体的数据读写过程以此类推。



本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/wireless_com/archive/2010/11/02/5981443.aspx](http://blog.csdn.net/wireless_com/archive/2010/11/02/5981443.aspx)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=symbian&t=blog)](https://so.csdn.net/so/search/s.do?q=平台&t=blog)](https://so.csdn.net/so/search/s.do?q=brew&t=blog)




