# C# 使用消息队列，包括远程访问 - 文章 - 伯乐在线
原文出处： [无线电静默](http://www.cnblogs.com/80X86/p/5557801.html)
近期做一个小的功能需求，用到了队列，用的时候出了很多问题，现在总结一下，希望能对有需要的人提供帮助。
我的需求很简单，就是多个客户端连接到我的一个小型的数据转发服务器上，开始使用的是Socket通信实现这个功能，一旦数据服务器接收到来自不同客户端发来的消息，就对这些消息进行处理（我这里是将数据接收到后再转发到另一个服务器上），但考虑到客户端是每隔一个很短的时间周期向服务器发送信息，并且连接客服端数量比较多的时候，担心会产生并发访问的问题，也希望避免 数据转发服务器 频繁地从多个不同线程获取信息而出现其他未知问题，所以在处理客户端向数据转发服务器发送信息的时候采取队列的方式。
一般情况下，使用MSMQ，要先安装消息服务，这个直接百度就行；
在VS里添加 Messaging引用，就可以使用MessageQueue这个类了；接下来就要思考清楚你的数据（消息）的流向问题，之前因为自己对队列的错误认识，对到底在哪创建队列，队列的消息又由谁去发送和接收没有弄清除，还有参考的一些写得不是太清晰地博文，绕了好大一圈，所以今天在这里以我自己的项目需求为例子，说明 **1、如何创建队列 2、如何向队列发送消息 3、 如何获取队列中的消息**
首先、创建队列：根据我的需求，我要通过Socket通信将信息发送至数据转发服务器，因此为了避免并发访问问题的产生，消息队列应当建立在数据转发服务器上；


C#
```
MessageQueue myqueue = null;
　　　　　　　string queuepath = @".private$queuedemo";
            if (!MessageQueue.Exists(queuepath))
            {
                myqueue = MessageQueue.Create(queuepath);
            }
            myqueue = new MessageQueue(queuepath);
```
这样就在数据转发服务器端创建了一个名为queuedemo的消息队列；从客户端要发送的消息就保存在这个队列里，你可以通过计算机管理->服务和应用下的消息队列中看到你创建的queuedemo队列，private$关键字是说明队列为专用队列，如果没有这个关键字还要配置域服务器，还是挺麻烦，这个还是借助百度吧，前面的“.”代表创建的队列目录是本机，这个队列一旦创建成功，就是系统的事了，接下来要做的就是你怎么去把消息写进这个队列，或者读取队列的值 **这里要特别注意**，**不要将queuepath路径字符串写成**


C#
```
string queuepath = @"FormatName:Direct=TCP:192.168.1.153private$queuedemo";
```
这样写的话是用于远程计算机对这个队列进行访问的，因为MessageQueue的Create()和Exisit()方法是没办法去识别上述FormatName格式的,还有要确保Create()函数要被执行了之后再用MessageQueue实例去引用；这样服务器端队列的创建就完成了；
在客户端中，向队列发送信息；


C#
```
string s = "客户端往队列里发送的信息");
            System.Messaging.Message msg = new System.Messaging.Message();
            msg.Body = s;
            msg.Formatter = new XmlMessageFormatter(new Type[] { typeof(string) });
            MessageQueue mq = new MessageQueue(@"FormatName:Direct=TCP:192.168.1.153private$queuedemo");
            mq.Send(msg);
```
在客户端中，用一个MessageQueue实例**指向服务器本机上创建的队列路径**，这时，MessageQueue实例的构造函数里的路径就一定要用FormatName格式，指明是TCP通信还是HTTP还是Machine如我上面代码所示，然后调用Send()方法，将消息写进队列，这个要求发送的对象要以序列化的方式写进去，所以要设置formatter,这里用的是XmlMessageFormatter 还有BinaryMessageFormatter等等 注意保存你消息的 消息体Body是Object类型的 因此可以将你写的任何一个类的对象发送至消息队列
在服务器中接收消息队列


C#
```
MessageQueue mq = new MessageQueue(@".private$queuedemo");
            mq.Formatter = new XmlMessageFormatter(new Type[] { typeof(string) });
            Thread th = new Thread(() =>
                {
                    while (true)
                    {
                        System.Messaging.Message msg = mq.Receive();
                        if (msg != null)
                        {
                            MessageBox.Show(msg.Body.ToString());
                        }
                    }
                });
            th.IsBackground = true;
            th.Start();
```
在本机上可以新创建一个队列实例指向本机的队列，然后按照之前约定的序列化格式反序列化消息体所以将新的队列实例的foarmatter属性赋值为发送时的formatter属性如代码所示，这个时候就直接用Receive()得到消息体，然后对消息体里的信息做处理，我这里是开启一个线程显示队列的消息，只要有新的消息写入，我就在消息框中输出
这个时候可能客户端无法向远程服务器成功发送消息，原因基本权限问题 服务器的消息队列的权限没有对未验证的客户端开放  你要在服务器队列里分配对应权限 如果你想读取队列的内容 还需要加系统变量
问题解决办法
1. 服务器端
- 服务器上消息队列权限设置：给ANONYMOUS LOGON赋予所有权限；![](http://jbcdn2.b0.upaiyun.com/2016/06/934a7090a8d37bc7e9ced8ade51e1b98.jpeg)
- 修改服务器的注册表，允许非验证客户端访问
- 注册表新增HKLMSoftwareMicrosoftMSMQParameterssecurityAllowNonauthenticatedRpc项，设置其DWORD值为1
- 注册表新增HKLMSoftwareMicrosoftMSMQParameterssecurityNewRemoteReadServerDenyWorkgroupClient项，设置其DWORD值为1MSMQ的安全访问控制说明参见：[http://msdn.microsoft.com/en-us/library/4108f68e-80f5-40e1-b3df-b713cc4dff79(prot.20).aspx](http://msdn.microsoft.com/en-us/library/4108f68e-80f5-40e1-b3df-b713cc4dff79%28prot.20%29.aspx)
- ![](http://jbcdn2.b0.upaiyun.com/2016/06/e969026c029918df7ff9362a6cd36aa0.jpeg)
这样客户端就可以读取服务器里的队列信息了 当然一般业务逻辑上不这么做 因为他只负责发送消息 ，综上，就是使用消息队列 跨服务器读写的 最基本的用法
