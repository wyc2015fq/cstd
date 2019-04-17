# c++语言特别之处(用C语言思维和C++比较) - V__KING__的专栏 - CSDN博客





2019年01月15日 15:38:59[v__king__](https://me.csdn.net/V__KING__)阅读数：50








#### 构造函数用new和不用new的区别

```
FrameDecoder *pDecoder = new CFrameDecoder();//指针
FrameDecoder Decoder = CFrameDecoder();//对象
```

#### 前置声明

c++与c的区别:

C可以前置声明struct A; enum E; 但是c++不可以，c++可以用class A作为前置声明。正确的前置声明这里不说明，请网上查找，这里列出一个错误的。

下面是一个错误的c++前置声明代码。

```
//main.cpp
#include <iostream>
#include "ltkcpp_connection.h"
using namespace std;
int main(int argc, char **argv) {
    cout << "Hello, world!" << endl;
    CPlatformSocket *pCPlatformSocket  = new CPlatformSocket(1);
    return 0;
}

//ltkcpp_connection.c
#include "ltkcpp_connection.h"
class CPlatformSocket
{
  public:
    int                         m_sock;
    CPlatformSocket(int sock);
};
CPlatformSocket::CPlatformSocket (int sock)
{
    m_sock = sock;
}

//ltkcpp_connection.h
#ifndef __ltkcpp_connection_H
#define __ltkcpp_connection_H
class CPlatformSocket;
#endif
```



