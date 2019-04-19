# Qt网络编程错误总结 - fanyun的博客 - CSDN博客
2018年05月02日 22:31:36[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：199
1.报错提示
```cpp
QObject::connect: Cannot queue arguments of type 'QAbstractSocket::SocketError'  
(Make sure 'QAbstractSocket::SocketError' is registered using qRegisterMetaType  
  
().)
```
错误原因：
因为没有指定 connect的链接方向 Qt::DirectConnection
默认会发送给主线程即默认的连接Qt::AutoConnection ，如果是多线程的话就是Qt::QueuedConnection了，所以要明确指定。
2.报错提示
```cpp
QNativeSocketEngine::write() was not called in QAbstractSocket::ConnectedState
```
错误原因：
       QTcpSocket 中的connectToHost() 是void类型的，所以调用这个函数后无论是否连上就发送数据的话，在未连接的情况下会出现如下错误。
3.报错提示
```cpp
QObject：Cannot create children for a parent that is in a different thread
```
错误原因：
       开始是将QTcpSocket对象作为主界面的一个成员变量，因此在构造函数内初始化（new），然后在QThread线程内连接服务端以及进行数据发送。QTcpSocket对象创建和使用要在同一个线程，否则报错。将初始化和连接都放在QThread内，并建立一个长连接，并采用一个标志位表示连接状态，发送前判断连接标志位，以此来进行连接或者发送。
4.报错提示
```cpp
QObject：：connect: Cannot queue arguments of type 'QAbstractSocket::SocketError' (Make sure ‘QAbstractSocket::SocketError' is registered using qRegisterMetaType(). )
```
错误原因：
在QThread内将QTcpSocket对象信号（connected、error、stateChangeed）与槽函数连接报数据类型未注册。connect自动连接在线程类中的信号默认是排队模式QueuedConnection，因此需要注册参数类型。将默认connect的连接方式改为Qt::DirectConnection，就不需要类型信息；
5.错误提示
```cpp
QNativeSocketEngine::write() was not called in QAbstractSocket::ConnectedState
```
错误原因：
错误为不是在QTcpSocket连接状态下调用write()函数，即调用write()时连接尚未建立。
6.错误提示
```cpp
QTcpSocket Connection Refused
```
错误原因：
服务端没开启返回连接被拒绝，网线没连接则会超时，也有可能是网路不通。
