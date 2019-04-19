# 基于Unity的编辑器开发(二): 进程间通信 - 逍遥剑客 - CSDN博客
2018年05月09日 20:15:14[xoyojank](https://me.csdn.net/xoyojank)阅读数：2582

- [共享代码](#共享代码)
- [进程间通信](#进程间通信)
- [通信协议](#通信协议)
- [其它](#其它)
# 共享代码
首先要做的, 是需要编辑器和Unity共享一部部分代码, 至少协议定义和解析我不想写两遍. 虽然有protobuf这样的工具库, 但是如果不是跨语言的话, 我觉得没必要引入另一套流程. 所以我就想能不能让一个C# dll库可以同时被Winforms的编辑器和Unity脚本引用呢? 考查了一下还是可以的:
- Unity的.Net默认是Subset, 需要改成全的
- Unity的.Net默认是C#3.5版本兼容的, 一些新的语法(如async)不支持
- Unity中如果要引用Visual Studio编译出来的C# dll, 需要把Target framework改成”Unity 3.5 .net full Base Class Libraries”
- 把VS编译出来的dll, 拷到Unity的Asset目录, mono脚本里就可以直接引用了
同时, 编辑器这边也需要知道一些游戏的数据类型和接口的定义, 实验了一下, UnityEngine.dll, Assembly-CSharp.dll, Assembly-CSharp-firstpass.dll可以直接被VS这边的C#工程引用, 只要不执行Unity特有的方法(会报”ECall 方法必须打包到系统模块中”的异常), 就可以在Winforms工程中安全地复用游戏脚本中的代码了. 
经过这样的设置, 编辑器和Unity游戏可以共享一个dll库, 从代码上做到了共享, 这就为我们的代码复用和通信协议定义提供了基础保障. 
# 进程间通信
为了达成这一通信需求, 首先做了一些搜索:
- [unity3D 命名管道 进程通信](https://blog.csdn.net/nicepainkiller/article/details/53642833)
- [Unity3D应用嵌入WPF应用并实现通信](https://blog.csdn.net/lordwish/article/details/72905055)
- [桌面WPF程序嵌入Unity3D引擎（standalone）并实现通讯](http://www.songshizhao.com/blog/blogPage/78.html)
总结下来, 要么是基于NamedPipe, 要么是基于Socket.  
尝试使用[Full Duplex Asynchronous Read/Write with Named Pipes - CodeProject](https://www.codeproject.com/Articles/1179195/Full-Duplex-Asynchronous-Read-Write-with-Named-Pip)里的基于NamedPipe方法, 遇到一些问题:
- Unity这边会报异常, Unity的mono对NamePipe支持不是很好
- 有时候会连不上, 比如管道被占用
所以又换了一个不依赖mono那个不靠谱.net framework的方案, 搜了搜看起来NNanomsg不错, 使用起来够简单, 不过也遇到一些问题:
- NNanomsg如果要在Unity用使用, 需要做一点修改, 主要是native dll的载入: [https://github.com/xoyojank/NNanomsg](https://github.com/xoyojank/NNanomsg)
- 使用ipc协议也会出现莫名其妙连不上的问题, 本质上底层还是走的NamePipe, 换成tcp协议就好了
- 错误信息不够直观, 所以我又在NNanomsg里加了nanomsg的一些调试用的函数的接口
- 一次性发送大量数据(比如几MB), 会导致链接断开或卡死, 问了作者说实现机制的问题, 让我尝试[nanomsg next gen](https://github.com/nanomsg/nng), 不过这个问题暂时可以绕过
用nanomsg的好处就是连接的建立/发送/接收等不用自己操心, 可以直接连接UnityEditor进行双端开发, 对于调试修改非常方便: 
![这里写图片描述](https://img-blog.csdn.net/20180509195550994?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 通信协议
通常网络通信都需要定义协议, protobuf是最常用的. 不过既然我们做到了两个进程的代码共享, 那完全可以直接把消息的定义直接写在里面, 类似这样:[https://stackoverflow.com/questions/13558422/trying-to-design-a-small-message-handler-class-to-simulate-c-sharp-events-what](https://stackoverflow.com/questions/13558422/trying-to-design-a-small-message-handler-class-to-simulate-c-sharp-events-what)
```
class IntMessage : Message
{
    public int Value = 100;
}
class StringMessage : Message
{
    public string Value = "a string";
}
static void Main(string[] args)
{
    MessageHandler.Subscribe((StringMessage m) => Console.WriteLine("String : " + m.Value));
    MessageHandler.Subscribe((StringMessage m) => Console.WriteLine("2nd String : " + m.Value));
    MessageHandler.Subscribe((IntMessage m) => Console.WriteLine("Int : " + m.Value));
    MessageHandler.Subscribe((IntMessage m) => Console.WriteLine("2nd Int : " + m.Value));
    MessageHandler.Publish(new IntMessage());
    MessageHandler.Publish(new StringMessage());
}
```
Message直接序列化后就可以发送到另一端进程了. 不过这样还是有点繁琐, 所以我照着Unity的RPC山寨了个LPC: 
![这里写图片描述](https://img-blog.csdn.net/20180509200214555?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180509200230698?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
本质上的实现也是把MethodCall给序列化了, 走的还是SendMessage的流程:
```
public static void SendMessage(Message message)
        {
            var stream = new MemoryStream();
            binaryFormatter.Serialize(stream, message);
            if (!IPC.SendImmediate(stream.GetBuffer()))
            {
                string log = String.Format("{0} : {1}", message.ToString(), NN.StrError(NN.Errno()));
                LogCore(LogType.Error, log);
            }
        }
        public static void ProcedureCall(string className, string methodName, params object[] args)
        {
            SendMessage(new ProcedureCallMessage { ClassName = className, MethodName = methodName, Arguments = args });
        }
```
收到消息后通过预先注册好的MethodInfo直接Invoke调用执行就可以了. 
# 其它
还有更高级的需求, 那就是跨进程的对象属性编辑. 目前的思路是这样的:
- Unity这边的数据对象序列化, 发送到编辑器
- 编辑器收到数据, 反序列化出数据对象(不能依赖Unity的方法, 否则会抛异常)
- 编辑器修改后的对象序列化后发送到Unity
- Unity这边反序列化出修改后的对象, 把属性值拷贝到当前编辑对象上去
![这里写图片描述](https://img-blog.csdn.net/20180509201407389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hveW9qYW5r/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
虽然简单暴力, 但也是行之有效的做法, IPC也不用过多考虑数据流量的问题, 当然比较极致一点是实现一套像WPF那样的DataBinding, 针对每个变化的属性做进程间同步, 有时间可以尝试下. 
