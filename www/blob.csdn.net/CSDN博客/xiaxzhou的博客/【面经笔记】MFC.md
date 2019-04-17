# 【面经笔记】MFC - xiaxzhou的博客 - CSDN博客





2017年07月27日 09:57:28[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：428








### MFC下多线程的API
- 
createthread（） 

创建新线程，返回句柄

- 
closehandle（） 

关闭句柄，减少线程内核对象的引用计数，当线程运行结束且引用计数为0 ，系统释放内核对象。

- 
waitforsingleobject（） 

等待线程结束


互斥锁：
- 
createmutex（） 

创建互斥对象，返回句柄，第二个参数指定互斥对象是否为当前线程拥有

- 
waitforsingleobject（） 

等待上锁

- 
releasemutex（） 

解锁


对互斥对象，谁拥有谁释放，系统会对比当前释放操作线程ID与拥有互斥对象的线程ID是否为同一个。 

如果线程完成了其代码，但是释放其拥有的互斥对象，则操作系统会将结束的线程拥有的互斥对象的线程ID和计数均置为0；

保证应用程序只有一个实例：通过命名互斥对象实现

事件对象：
- 
CreateEvent（） 

创建事件对象

- 
waitforsingleobject（） 

等待事件通知

- 
setevent（） 

事件通知

- 
resetevet() 

设置事件对象无信号状态


人工重置的事件对象：所有等待事件通知的线程均变可调用状态。 

自动重置的事件对象：只有一个等待事件通知的线程均变可调用状态。

临界区：
- 
initializecriticalsection（） 

创建临界区对象

- 
deletecriticalsection（） 

释放没有被线程使用的临界区对象的所有资源

- 
enterciticalsection（） 

进入临界区

- 
leavecriticalsection（） 

离开临界区


线程退出但没有释放临界区对象的所有权，则其他线程不能再得到该所有权

### 消息机制

> 
[http://blog.csdn.net/evankaka/article/details/44456661](http://blog.csdn.net/evankaka/article/details/44456661)

[http://blog.csdn.net/fysy0000/article/details/6616906](http://blog.csdn.net/fysy0000/article/details/6616906)


Windows系统的整个消息系统分为3个层级：

```
① Windows内核的系统消息队列

② App的UI线程消息队列

③ 处理消息的窗体对象
```

Windows内核维护着一个全局的系统消息队列；按照线程的不同，系统消息队列中的消息会分发到应用程序的UI线程的消息队列中；

应用程序的每一个UI线程都有自己的消息循环，会不停地从自己的消息队列取出消息，并发送给Windows窗体对象；

每一个窗体对象都使用窗体过程函数（WindowProc）来处理接收到的各种消息。
- 不进队消息和进队消息

不进队消息：是指由**Windows直接调用**消息处理函数，把消息直接交给其处理。

进队消息：是指Windows将消息放入到程序中的**消息队列**中取，由程序通过getmessage把消息取出，然后由函数**DispathMessage函数**将消息分发给消息处理函数处理，进队消息基本上是用户的输入。

发送消息：SendMessage 和 PostMessage

> 
[http://blog.csdn.net/zkybeck_ck/article/details/46540235](http://blog.csdn.net/zkybeck_ck/article/details/46540235)

**SendMessage**为发送“不进队消息”，直接调用处理函数处理，返回处理函数处理结果。**PostMessage**为发送“进队消息”。PostThreadMessage为向线程发消息

- 消息传递过程

1、应用程序产生窗口消息，向上传递给操作系统，操作系统将消息投递到应用程序的消息队列 

2、应用程序调用getmessage从消息队列取出消息 

3、应用程序调用dispatchmessage对消息进行处理，分配消息到窗口过程。dispatchmessage实质是将消息传回给操作系统，此消息中包含接收消息的窗口句柄，操作系统通过句柄调用对应的窗口过程函数对消息进行处理。- 消息映射机制

在每个能接收和处理消息的类中，定义一个消息与消息函数的映射表，当有消息需要处理时，搜索消息映射表，调用相应的消息处理函数。

窗口消息(Windows Message) 与窗口的内部运作有关，如创建窗口，绘制窗口，销毁窗口等。 

命令消息与处理用户请求有关， 如单击菜单项或工具栏或控件时， 就会产生命令消息。 

 通知消息(Notification   message)是指这样一种消息，一个窗口内的子控件发生了一些事情，需要通知父窗口。- MFC各种类的作用：

视图类（） 

文档类（主要对数据进行存储加载）、 

框架类（主要完成对窗口类的产生、创建）、 

应用程序类（主要完成对类的设计和注册，是整个MFC程序的入口一个程序只能有一个对象）。
### MFC中不使用资源视图，如何动态创建一个窗口，使用到那些API
- 设计窗口类
- 注册窗口类：registerclass
- 创建窗口：createwindow
- 显示更新窗口：showwindow、updatewindow

### 图像在MFC框架中显示的流程及API
- 
定义一个CImage类对象，然后调用**CImage::Load**方法装载一个外部图像文件 

- 
图片加载完成后需要调用view类：Invalidate(); // 强制调用OnDraw

- 
在OnDraw函数中调用CImage::Draw（）

- 
指定绘制的目标设备环境句柄，调用**CImage::Draw**方法绘制图像：参数：绘制的目标设备环境句柄，位置，高度和宽度。

- 
之后修改图片数据后，调用Invalidate()可完成重绘。







