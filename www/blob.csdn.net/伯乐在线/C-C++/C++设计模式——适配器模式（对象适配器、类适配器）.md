# C++设计模式——适配器模式（对象适配器、类适配器） - 文章 - 伯乐在线
原文出处： [swqqcs，2014-09-09](http://blog.csdn.net/swqqcs/article/details/39160129)
#### 前言
现在的笔记本都只存在USB接口，用来口，电脑上存在的是USB接口，两者如何通信呢？可以使用USB转串口线连接电脑和嵌入式设备,其中USB一端连接电脑、串口和外部设备进行通信。然而在一些嵌入式设备上(例如ARM9)，通常使用串口和电脑进行通信。嵌入式设备上存在的是串一端连接嵌入式设备。本来电脑和嵌入式设备由于接口不兼容，无法进行通信，而使用USB转串口线这个适配器，两者之间就可以正常进行数据通信。在设计模式中，也存在一种类似的模式，存在两个接口不同的类，可以使用一个适配器类来将一个接口转换为客户希望的另一个接口，称为适配器模式。
#### 1、适配器模式
在适配器模式中引入了一个被称为适配器(Adapter)的包装类，而它所包装的对象称为适配者(Adaptee)，即被适配的类。适配器的实现就是把客户类的请求转化为对适配者的相应接口的调用。也就是说：当客户类调用适配器的方法时，在适配器类的内部将调用适配者类的方法，而这个过程对客户类是透明的，客户类并不直接访问适配者类。因此，适配器让那些由于接口不兼容而不能交互的类可以一起工作。
适配器模式可以将一个类的接口和另一个类的接口匹配起来，而无须修改原来的适配者接口和抽象目标类接口。适配器模式定义如下：
> 
适配器模式(Adapter Pattern)：将一个接口转换成客户希望的另一个接口，使接口不兼容的那些类可以一起工作，其别名为包装器(Wrapper)。适配器模式既可以作为类结构型模式，也可以作为对象结构型模式。
在适配器模式中，我们通过增加一个新的适配器类来解决接口不兼容的问题，使得原本没有任何关系的类可以协同工作。根据适配器类与适配者类的关系不同，适配器模式可分为对象适配器和类适配器两种，在对象适配器模式中，适配器与适配者之间是关联关系；在类适配器模式中，适配器与适配者之间是继承（或实现）关系。在实际开发中，对象适配器的使用频率更高。
![1362066399_9469](http://jbcdn2.b0.upaiyun.com/2016/12/fccdc95e04d0386ed53700a94e3d218e.jpg)
对象适配器模式结构图
在对象适配器模式结构图中包含如下几个角色：
**Target（目标抽象类）**：目标抽象类定义客户所需接口，可以是一个抽象类或接口，也可以是具体类。
**Adapter（适配器类）**：适配器可以调用另一个接口，作为一个转换器，对Adaptee和Target进行适配，适配器类是适配器模式的核心，在对象适配器中，它通过继承Target并关联一个Adaptee对象使二者产生联系。
**Adaptee（适配者类）**：适配者即被适配的角色，它定义了一个已经存在的接口，这个接口需要适配，适配者类一般是一个具体类，包含了客户希望使用的业务方法，在某些情况下可能没有适配者类的源代码。
根据对象适配器模式结构图，在对象适配器中，客户端需要调用request()方法，而适配者类Adaptee没有该方法，但是它所提供的specificRequest()方法却是客户端所需要的。为了使客户端能够使用适配者类，需要提供一个包装类Adapter，即适配器类。这个包装类包装了一个适配者的实例，从而将客户端与适配者衔接起来，在适配器的request()方法中调用适配者的specificRequest()方法。因为适配器类与适配者类是关联关系（也可称之为委派关系），所以这种适配器模式称为对象适配器模式。
#### 2、Socket网络通信的设计与实现——对象适配器
使用TCP进行网络通信，一般都会包含创建套接字、绑定套接字、监听套接字、连接套接字等4个过程。某网络公司已经有一套成熟的套接字软件包，里面包含网络通信的4个过程。现欲开发一套新的聊天软件，决定复用已经成熟的套接字软件包。
![](http://img.blog.csdn.net/20140826214613843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBlTGlmZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
SocketPackage就是成熟的套接字软件包，也就是Adaptee适配者类，包含创建套接字、绑定套接字、监听套接字、连接套接字等成熟方法； Socket是目标抽象类Target，定义了客户希望的方法； SocketAdapter为适配器类，将成熟软件包里面的方法转为Socket类中客户希望的方法。
##### 2.1 待适配的类——成熟的套接字
成熟的套接字软件包实现代码如下：


```
#ifndef _SOCKET_PACKAGE_H_  
#define _SOCKET_PACKAGE_H_  
  
#include <iostream>  
#include <string>  
using namespace std;  
  
  
//套接字包(可以被复用)  
class SocketPackage  
{  
public:  
    void CreateSpecificSocket()  
    {  
        cout << "创建套接字" << endl;  
    }  
  
    void BindSpecificSocket()  
    {  
        cout << "绑定套接字" << endl;  
    }  
  
    void ListenSpecificSocket()  
    {  
        cout << "监听套接字" << endl;  
    }  
  
    void ConnecSpecifictSocket()  
    {  
        cout << "连接套接字" << endl;  
    }  
  
};  
  
#endif
```
现欲开发的聊天软件定义了一个抽象类Socket，也就是目标抽象类Target，里面包含创建套接字、绑定套接字、监听套接字、连接套接字等客户希望的方法，但这些方法和成熟软件包中的方法不同。如：Socket类创建套接字方法为CreateSocket,而SocketPackage类创建套接字方法为CreateSpecificSocket。这两个方法不同，无法进行通信，为了复用成熟套接字软件包里面的方法，可以提供一个适配器类，继承于抽象的Socket类。在适配器中的方法将调用套接字软件包里面的方法。
##### 2.2 适配器类——新的套接字
套接字适配器类实现代码如下:


```
#ifndef _SOCKET_H_  
#define _SOCKET_H_  
  
#include "SocketPackage.h"  
  
//抽象套接字类  
class Socket  
{  
public:  
    //创建套接字  
    virtual void CreateSocket() = 0;  
  
    //绑定套接字  
    virtual void BindSocket() = 0;  
  
    //监听套接字  
    virtual void ListenSocket() = 0;  
  
    //连接套接字  
    virtual void ConnectSocket() = 0;  
};  
  
  
  
//套接字适配器  
class SocketAdapter : public Socket  
{  
private:  
    SocketPackage * m_pSocketPackage;  
public:  
    //构造函数，创建一个需要复用的套接字包对象  
    SocketAdapter()  
    {  
        m_pSocketPackage = new SocketPackage();  
    }  
  
    //销毁需要复用的套接字包对象  
    ~SocketAdapter()  
    {  
        if( NULL != m_pSocketPackage )  
        {  
            delete m_pSocketPackage;  
            m_pSocketPackage = NULL;  
        }  
    }  
  
    //创建套接字  
    void CreateSocket()  
    {  
        m_pSocketPackage->CreateSpecificSocket();  
    }  
      
    //绑定套接字  
    void BindSocket()  
    {  
        m_pSocketPackage->BindSpecificSocket();  
    }  
      
    //监听套接字  
    void ListenSocket()  
    {  
        m_pSocketPackage->ListenSpecificSocket();  
    }  
      
    //连接套接字  
    void ConnectSocket()  
    {  
        m_pSocketPackage->ConnecSpecifictSocket();  
    }  
};  
  
  
#endif
```
##### 2.3 测试
测试代码实现如下:


```
#include <iostream>  
#include "Socket.h"  
  
  
using namespace std;  
  
  
int main()  
{  
    //创建套接字适配器对象  
    Socket * pSocketAdapter = new SocketAdapter();  
      
    //使用适配器进行套接字操作  
    pSocketAdapter->CreateSocket();  
    pSocketAdapter->BindSocket();  
    pSocketAdapter->ListenSocket();  
    pSocketAdapter->ConnectSocket();  
  
  
    //销毁操作  
    delete pSocketAdapter;  
    pSocketAdapter = NULL;  
      
    return 0;  
}
```
编译并执行，结果如下:
![](http://img.blog.csdn.net/20140826220651636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBlTGlmZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
SocketAdapter套接字适配器类和SocketPackage套接字包是一种组合的关系，当调用套接字适配器类中的相应方法时，将调用套接字包对应的方法，通过组合的方式，实现对套接字软件包模块的复用。客户端不需要直接操作这个已经成熟的套接字软件包，而是由套接字适配器类进行委托操作，降低了客户端和适配者类的耦合。
对象适配器模式中，适配器类Adapter和适配者类Adatpee类是一种关联关系，或者组合关系。适配器类维护一个适配者类的引用，在适配器的方法中调用相应的适配者类中的方法，实现对适配者功能的复用，这个过程对客户端是透明的，客户类不直接访问适配者类。
#### 3、Socket网络通信的设计与实现——类适配器
除了对象适配器模式之外，适配器模式还有一种形式，那就是类适配器模式，类适配器模式和对象适配器模式最大的区别在于适配器和适配者之间的关系不同，对象适配器模式中适配器和适配者之间是关联关系，而类适配器模式中适配器和适配者是继承关系。不管是对象适配器还是类适配器，都是为了复用适配者类的功能。
![](http://img.blog.csdn.net/20140826223625562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBlTGlmZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
SocketPackage就是成熟的套接字软件包，也就是Adaptee适配者类，包含创建套接字、绑定套接字、监听套接字、连接套接字等成熟方法； Socket是目标抽象类Target，定义了客户希望的方法； SocketAdapter为适配器类，继承自Socket类和SocketPackage类，将成熟软件包里面的方法转为Socket类中客户希望的方法。
##### 3.1 带适配类——成熟套接字
成熟的套接字软件包实现代码如下:


```
#ifndef _SOCKET_PACKAGE_H_  
#define _SOCKET_PACKAGE_H_  
  
#include <iostream>  
#include <string>  
using namespace std;  
  
  
//套接字包(可以被复用)  
class SocketPackage  
{  
public:  
    void CreateSpecificSocket()  
    {  
        cout << "创建套接字" << endl;  
    }  
  
    void BindSpecificSocket()  
    {  
        cout << "绑定套接字" << endl;  
    }  
  
    void ListenSpecificSocket()  
    {  
        cout << "监听套接字" << endl;  
    }  
  
    void ConnecSpecifictSocket()  
    {  
        cout << "连接套接字" << endl;  
    }  
  
};  
  
#endif
```
SocketAdapter套接字适配器类和SocketPackage套接字包类不在是组合的关系，SocketAdapter继承于Socket类和SocketPackage，当调用SocketAdapter套接字适配器类中的方法，将会调用其基类SocketPackage的相应方法，通过继承的方式实现对适配者类的功能复用。
##### 3.2 适配类——采用（多）继承
套接字适配器类实现代码如下:


```
#ifndef _SOCKET_H_  
#define _SOCKET_H_  
  
#include "SocketPackage.h"  
  
//抽象套接字类  
class Socket  
{  
public:  
    //创建套接字  
    virtual void CreateSocket() = 0;  
  
    //绑定套接字  
    virtual void BindSocket() = 0;  
  
    //监听套接字  
    virtual void ListenSocket() = 0;  
  
    //连接套接字  
    virtual void ConnectSocket() = 0;  
};  
  
  
  
//套接字适配器(类适配器)  
class SocketAdapter : public Socket, SocketPackage  
{  
public:  
  
    //创建套接字  
    void CreateSocket()  
    {  
        CreateSpecificSocket();  
    }  
      
    //绑定套接字  
    void BindSocket()  
    {  
        BindSpecificSocket();  
    }  
      
    //监听套接字  
    void ListenSocket()  
    {  
        ListenSpecificSocket();  
    }  
      
    //连接套接字  
    void ConnectSocket()  
    {  
        ConnecSpecifictSocket();  
    }  
};  
  
  
#endif
```
##### 3.3 测试
测试程序实现代码如下：


```
#include <iostream>  
#include "Socket.h"  
  
using namespace std;  
  
int main()  
{  
    //创建套接字适配器对象  
    Socket * pSocketAdapter = new SocketAdapter();  
      
    //使用适配器进行套接字操作  
    pSocketAdapter->CreateSocket();  
    pSocketAdapter->BindSocket();  
    pSocketAdapter->ListenSocket();  
    pSocketAdapter->ConnectSocket();  
  
    //销毁操作  
    delete pSocketAdapter;  
    pSocketAdapter = NULL;  
      
    return 0;  
}
```
编译并执行，程序结果如下:
![](http://img.blog.csdn.net/20140826224828374?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBlTGlmZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
适配器类继承于目标抽象类和适配者类，当调用适配器类的方法时，在该方法内部将调用其基类，也就是适配者类中相应的方法，通过继承的方式，实现复用适配者类的功能。
不管是对象适配器还是类适配器，都是为了实现功能的复用。
#### 4、适配器模式总结
在对象适配器模式中，适配器与适配者之间是关联关系；在类适配器模式中，适配器与适配者之间是继承关系。不论是对象适配器还是类适配器，适配器模式都将现有接口转化为客户类所期望的接口，实现了对现有类的复用。为了避免两个或者多个事物之间直接耦合，应该如何分配职责? 大多数情况都可以通过增加一层间接性来解决，将职责分配给中介对象，使其作为其他构件或服务之间的媒介，以避免他们之间的直接耦合。在适配器模式中，为了避免客户类和与适配者类直接耦合，也就是客户类直接访问适配者类，可以增加一个适配器类，降低客户类与适配者类的耦合性。当客户类调用适配器的方法时，在适配器类的内部将调用适配者类的方法，而这个过程对客户类是透明的，客户类并不直接访问适配者类。它是一种使用频率非常高的设计模式，在软件开发中得以广泛应用。
##### 4.1 主要优点
无论是对象适配器模式还是类适配器模式都具有如下优点：
(1) 将目标类和适配者类解耦，通过引入一个适配器类来重用现有的适配者类，无须修改原有结构。
(2) 增加了类的透明性和复用性，将具体的业务实现过程封装在适配者类中，对于客户端类而言是透明的，而且提高了适配者的复用性，同一个适配者类可以在多个不同的系统中复用。
(3) 灵活性和扩展性都非常好，通过使用配置文件，可以很方便地更换适配器，也可以在不修改原有代码的基础上增加新的适配器类，完全符合“开闭原则”。
具体来说，类适配器模式还有如下优点：
由于适配器类是适配者类的子类，因此可以在适配器类中置换一些适配者的方法，使得适配器的灵活性更强。
对象适配器模式还有如下优点：
(1) 一个对象适配器可以把多个不同的适配者适配到同一个目标；
(2) 可以适配一个适配者的子类，由于适配器和适配者之间是关联关系，根据“里氏代换原则”，适配者的子类也可通过该适配器进行适配。
4.2 主要缺点
类适配器模式的缺点如下：
(1) 对于Java、C#等不支持多重类继承的语言，一次最多只能适配一个适配者类，不能同时适配多个适配者；
对象适配器模式的缺点如下：
与类适配器模式相比，要在适配器中置换适配者类的某些方法比较麻烦。如果一定要置换掉适配者类的一个或多个方法，可以先做一个适配者类的子类，将适配者类的方法置换掉，然后再把适配者类的子类当做真正的适配者进行适配，实现过程较为复杂。
4.3 适配器模式具体应用
(1)复用已经写好的功能模块,例如:已经写好了一个员工信息模块，要重新开发一个新的企业管理系统，可以复用员工信息模块。
(2)程序运行时的日志信息，可以记录到数据库，也可以记录到Txt文件中。现在变更需求，使日志信息能够同时记录到数据库和Txt文件中。可以添加一个适配器类，适配数据库操作和文件操作。
(3)在软件开发过程中，我们会开发出一些功能性模块。如排序算法模块、查找模块、或者封装链表、队列、树等操作。在需要复用这些功能的时候，直接使用，无需重新开发这些功能模块。
(4)网上银行加密模块开发: 已经开发好了一套网银加密模块，现在需要重新开发一个类似的软件，可以直接复用已经开发好的加密模块。
(5)某公司已经开发了一套火车票识别软件，包含二值化、去噪、版面分析、栏目提取等模块。现在要开发银行卡识别项目，则可以直接复用火车票识别中的功能模块。
(6)某公司想要开发一套输入法软件，可以购买搜狗输入法的搜索模块，直接加以复用，无需自己重头开发。
(7)生活中的适配器: 电源适配器，USB转串口线，水管专修工人使用各种接口连接本来没法进行相连的管道。
