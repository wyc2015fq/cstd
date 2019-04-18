# boost库使用总结 - 深之JohnChen的专栏 - CSDN博客

2017年06月05日 17:53:23[byxdaz](https://me.csdn.net/byxdaz)阅读数：2819


1、VC6中min/max的灾难

当在VC中使用boost库，或者其他库的时候会有一些小的问题。

在Windows的头文件中已经定义了min和 max宏，所以在STL中的这两个函数就调用不到了，例如在MFC中就是这样，但是在Boost中，都是使用的std::命名空间下的函数，使用Windows的函数不能够接受不同类型的参数在模板中使用，但是许多库都依赖这些。

虽然Boost尽量处理这些问题，但有时候遇到这样的问题的时候就需要在自己的代码中加入像下面的代码在第一个#include前加入#define _NOMINMAX。

#define _NOMINMAX            // disable windows.h defining min and max as macros

#include "boost/config.hpp"  // include boosts compiler-specific "fixes"

using std::min;              // makle them globally available

using std::max;

这样操作并不是在任何时候都需要，而只有我们碰到使用了就需要加入这段代码。

2、字符串查找函数性能

字符串查找的时候，boost库提供了一个boost::contains和boost::icontains方法，后者忽略字符串大小写。但是在[测试](http://lib.csdn.net/base/softwaretest)性能的时候，这两个函数对性能影响巨大，使用string/wstring自带的find函数，性能会提高不少。boost::icontain耗性能的主要原因查看了源码，发现主要是在内部做了多次is_equal()的调用，且每次is_equal时都会进行to_upper()或者to_lower()一类的操作。所以非常考虑性能的话建议使用string自带的find。

resolver的异步查询（async_resolve)让我搞了1个小时，虽然教程上说根据说明文档很容易使用async_resolve，但是实际编写时就遇到问题了。
bug：异步完成句柄被调用时始终传入error。
但从网上查资料，基本跟我写的一样啊，没什么区别。最后各种尝试，终于找出里面的原因来了。
解决方法：io_service必须要在socket,resolver初始化之后才能调用run()函数，否则异步查询始终出错。

3、any
这个类提供了像shared_ptr一样的功能：能够包含任意类型。但是any不是一个模板类。所以我们可以在stl的容器中使用其作为类型参数，这样我们就可以实现在stl容器中包含任意类型的目的了。any中的any_cast是其精髓，想要访问any中的实例都需要这个接口去获得新的实例拷贝。

4、operator
operators头文件中包含了很多有便扩展操作符重载的解决方案，就像书中写的，如果我们提供operator<，那么我么应该提供operator<=,>=,>等操作符，但是其实我们发现，通过opertor<我们能够推出operator<=,operator>=,operator>等操作符，但是我们却往往忽略，或者怕出错误，以至于我们类的操作符重载不够“友好”，用户使用不能像预想的那样。opertors库，让这些依赖于主操作符重载的辅操作符重载更简单，而且更加概念化，系统化。通过派生于less_than_comparable<typename T>，我们就能实现比较，通过其他概念，我们将更加简单而且清晰的实现各类应该具备的操作符。

5、解决boost包含boost/algorithm/string.hpp造成的__int64错误

使用boost的string库进行跨平台操作,包含文件

#include <boost/algorithm/string.hpp>

结果遇到编译错误

error C2632: '__int64' followed by '__int64' is illegal

发现在config-win32.h已经定义过宏,在boost\cstdint.hpp又使用了一次typedef,解决方法，增加解决方法，增加#undefint64_t

#undef int64_t

#include <boost/algorithm/string.hpp>

6、解决boost包含#include<boost/asio/ip/tcp.hpp>造成的“error C2632:“__int64”后面的“__int64”非法”错误

解决方法，增加#undefint64_t

#undefint64_t

#include<boost/asio/ip/tcp.hpp>

#include<boost/asio.hpp>

#include<boost/bind.hpp>

#include<boost/enable_shared_from_this.hpp>

#include<boost/shared_ptr.hpp>

#include<boost/array.hpp>

#include<boost/function.hpp>

7、使用智能指针的几个注意点

智能指针是一种像指针的C++对象，但它能够在对象不使用的时候自己销毁掉。

我们知道在C++中的对象不再使用是很难定义的，因此C++中的资源管理是很复杂的。各种智能指针能够操作不同的情况。当然，智能指针能够在任务结束的时候删除对象，除了在程序之外。

许多库都提供了智能指针的操作，但都有自己的优点和缺点。Boost库是一个高质量的开源的C++模板库，很多人都考虑将其加入下一个C++标准库的版本中。

boost定义智能指针种类。
|shared_ptr<T>|内部维护一个引用计数器来判断此指针是不是需要被释放。是boost中最常用的智能指针了。|
|----|----|
|scoped_ptr<t>|当这个指针的作用域消失之后自动释放|
|intrusive_ptr<T>|也维护一个引用计数器，比shared_ptr有更好的性能。但是要求T自己提供这个计数器。|
|weak_ptr<T>|弱指针，要和shared_ptr结合使用|
|shared_array<T>|和shared_ptr相似，但是访问的是数组|
|scoped_array<T>|和scoped_ptr相似，但是访问的是数组|

下面是几个使用智能指针需要注意的地方：

声明一个智能指针的时候要立即给它实例化， 而且一定不能手动释放它。

…_ptr<T>不是T*类型。所以：

               a: 声明的时候要…_ptr<T>而不是….._ptr<T*>

               b：不能把T*型的指针赋值给它

               c: 不能写ptr=NULl,而用ptr.reset()代替。

不能循环引用。

不要声明临时的share_ptr， 然后把这个指针传递给一个函数

// shared_ptr<T>用法实例

```cpp
#include <stdio.h>
#include <boost/shared_ptr.hpp>
 
class A {
public:
    void print() {
       printf("class A print!\n");
    }
};
 
int main(int argc, char **argv) {
   boost::shared_ptr<A> a1(new A());
    a1->print();
}
```

8、获取对象地址

&作为取址符号用来获取对象的地址，但由于c++太灵活，通过重载operator&可以改变operator&原有的语意。当你需要使用对象的真实地址时，这种情况下boost库中addressof函数。

```cpp
#include"stdafx.h"
#include<iostream>
#include<boost/utility.hpp>
usingnamespaceboost;
usingnamespacestd;
 
classObject
{
public:
    Object(){m_age = 22;}
    intoperator&()
    {
        return0;
    }
    intGetAge(){returnm_age;}
 
protected:
    intm_age;
};
 
intmain()
{
    Objectobj;
    cout<<&obj<<endl;//输出
    Object *p=addressof(obj);
    cout<<p<<endl;//输出obj真实地址
    cout<<p->GetAge()<<endl;
}
```

9、boost::asio async_write也不能保证一次发完所有数据

看过basic_stream_socket的文档，里面提到async_write_some不能保证将所有要发送的数据都发出去。并且提到如果想这样做，需要使用boost asio的async_write

[http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/reference/basic_stream_socket/async_write_some.html](http://www.boost.org/doc/libs/1_53_0/doc/html/boost_asio/reference/basic_stream_socket/async_write_some.html)

async_write有时也无法一次发完数据。因此只好自己写了一个重试发送的递归函数。也很简单，通过bind,每次传递想要发送的字节数木和发送开始位置给异步回调函数。

代码参考如下：

```cpp
void Sign::AfterWriteMessage(error_code const& ec,size_t bytes_transferred, size_t expected_size, size_t offset) {
  if (ec) {
   BOOSTER_ERROR("AfterWriteMessage") << "writemessage failed, error code:" << ec.value()
                       << " category name:"<< ec.category().name()
                       << " id_:" << id_
                       << " address:" <<address  
                       << " message:" <<ec.message();
    Close();
    return;
  }
 
 BOOSTER_DEBUG("AfterWriteMessage") << "thread id:" << this_thread::get_id() << " send_buffer: "<< PrintBytesAsHexString(send_buffer, bytes_transferred) << "sent size:" << bytes_transferred;
 BOOSTER_DEBUG("AfterWriteMessage") << "thread id:" << this_thread::get_id() << " send_buffer: "<< PrintBytesAsHexString(send_buffer, expected_size) << "expected size:" << expected_size;
  
  size_tresend_size = expected_size - bytes_transferred;
  if (resend_size> 0) {
    size_tnew_offset = offset + bytes_transferred;
   async_write(socket, buffer((void*)&send_buffer[new_offset],resend_size),
        strand_.wrap(bind(&Sign::AfterWriteMessage,shared_from_this(), _1, _2, resend_size, new_offset)));
    return;
  }
 
  // do yourbusiness after send succeeds
  
}
 
void Sign::SendMessage(size_t size) {
  //  BOOSTER_DEBUG("SendMessage")<< "thread id: " << this_thread::get_id() << "send_buffer: " << PrintBytesAsHexString(send_buffer, size) <<" size:" << size;
 async_write(socket, buffer(send_buffer, size),
          strand_.wrap(bind(&Sign::AfterWriteMessage,shared_from_this(), _1, _2, size, 0)));
}
```

10、使用enable_shared_from_this

继承该类就可以进行基于当前子类进行安全的weap_ptr到shared_ptr的转换...
代码实例
以下代码中Y类继承enable_shared_from_this,,从而我们可以直接在函数中调用shared_from_this获得该对象的shared_ptr

``

```cpp
class Y: publicenable_shared_from_this<Y>
{
public:

    shared_ptr<Y> f()
    {
        returnshared_from_this();
    }
}

int main()
{
    shared_ptr<Y> p(newY);
// 调用f获得shared_ptr

    shared_ptr<Y> q =p->f();
    assert(p == q);
    assert(!(p < q || q <p)); // p and q must share ownership

}
```

11、boost锁

boost读写锁，共享-独占锁，当读写锁以读模式锁住时，它是以共享模式锁住的，当它以写模式锁住时，它是以独占模式锁住的。

typedefboost::shared_lock<boost::shared_mutex> readLock;

typedef boost::unique_lock<boost::shared_mutex> writeLock;

boost::shared_mutex rwmutex;

void readOnly( )

{

readLock rdlock(rwmutex);

// do something

}

void writeOnly( )

{

writeLock wtlock(rwmutex);

// do something

}

对同一个rwmutex，线程可以同时有多个readLock，这些readLock会阻塞任意一个企图获得writeLock的线程，直到所有的readLock对象都析构。如果writeLock首先获得了rwmutex，那么它会阻塞任意一个企图在rwmutex上获得readLock或者writeLock的线程。

互斥锁

typedef boost::unique_lock<boost::mutex> exclusiveLock;

递归式的互斥量

boost::recursive_mutex提供一个递归式的互斥量。对于一个实例最多允许一个线程拥有其锁定，如果一个线程已经锁定一个boost::recursive_mutex实例，那么这个线程可以多次锁定这个实例。

12、boost::asio中async_read与async_read_some的区别

asio::async_read通常用户读取指定长度的数据，读完或出错才返回。 而socket的async_read_some读取到数据或出错就返回，不一定读完了整个包！

void async_read(AsyncReadStream& s, constMutableBufferSequence& buffers,ReadHandler handler)时，必须等到填满Buffer，否则即使收到数据包，也不会没有调用ReadHandler句柄。

boost::asio::async_read(m_socket,boost::asio::buffer(recvData,nNeedRecvLen),
boost::bind(&ClientSession::handle_read,this,boost::asio::placeholders::error));

boost::array<char,512>m_recvBuf;

ip::tcp::socket m_socket;

//同步

```cpp
boost::system::error_codeerror;
std::size_tnSize = m_socket.read_some(boost::asio::buffer(m_recvBuf),error);
```
//异步

```cpp
m_socket.async_read_some(boost::asio::buffer(m_recvBuf),
            boost::bind(&TcpConnection::handle_read, this,
            boost::asio::placeholders::error, 
            boost::asio::placeholders::bytes_transferred));
```

sync_read_some函数一次只能接收大约1k数据，如果要接收大数据，需要反复接收。

13、boost库各个版本下载地址

Boost库官网：[http://www.boost.org/](http://www.boost.org/)

Boost库各个版本下载地址：

[https://sourceforge.net/projects/boost/files/boost-binaries/](https://sourceforge.net/projects/boost/files/boost-binaries/)

