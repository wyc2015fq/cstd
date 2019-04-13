
# Nginx-（三）支持Socket转发过程详解 - wangzhiguo9261的博客 - CSDN博客


2018年03月02日 10:02:06[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：140


# [Nginx支持Socket转发过程详解](http://www.cnblogs.com/knowledgesea/p/6497783.html)
# 序言
一网友在群中问，nginx支持socket转发吗？
实话说，我没做过socket转发，但是我知道socket跟http一样都是通过tcp或者udp通信的，我猜测啦一下nginx应该支持吧，然后又在网上查啦一下，网上说支持，也有支持的相关官方文档，且有理有据，于是我秉承不瞎扯，知之为知之的科学态度，我只对网友回啦两个字：“可以”，就不过多扯，此时群中也有网友为他解答问题啦，我就继续干我的活啦，干活，干活，干活，干活，没有过多的关注这个问题，没有过多的关注这个问题。
一小时后，半天后，半个月后，一年后 ，十年后  ...
此去经年，我又来到群中：哇靠，这货竟还在执着于同样的问题，不停的问：“怎么弄，怎么弄？” ！！ 感觉杨过的黯然销魂掌都练成啦，这个货为什么还没有半点进展。难道他在参悟天书5卷，准备修仙成佛！
于是我就对此问题产生啦极大的兴趣，开始搞。从搞到搞到结束不过30分钟，不过30分钟，搞定，搞定。下面的篇幅我会详解整个过程。
说此段子大家勿喷我，但是我只想对群中的此类网友说：
1、你们在问怎么做之前，官方文档有见到过吗，百度过相关文章吗？
2、百度文档后，有亲自操刀过吗？想游泳不下水行吗？
3、自己操刀过程中，一旦出错，你们有没有认真对待过错误信息？
4、在解锁一门招式的时候，有没有思考过类似的招式是怎么样的？或者你来设计这个招式会是什么样子的？思考？思考？关联？关联？
话有点过分，希望能引起你的关注，如果与你不对症，绕过即可，勿喷。
# 前10分钟，先写一个socket的服务端与客户端可以通信
实现服务端可以向客户端发送信息，客户端接受到信息后，显示出来。用c\#控制台程序实现。
服务端监听本机ip与端口，服务端代码如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
classProgram
    {staticSocket sck =null;staticvoidMain(string[] args)
        {             
            sck=newSocket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//监听本机ipIPAddress ip = IPAddress.Parse("172.18.8.196");           
            IPEndPoint endPoint=newIPEndPoint(ip,int.Parse("11911"));            
            sck.Bind(endPoint);
            sck.Listen(10);
            Console.WriteLine("开启监听！");
            Thread thread=newThread(JtSocket);
            thread.IsBackground=true;
            thread.Start();while(true)
            {varmsg =Console.ReadLine().Trim();if(msg !="")
                {byte[] buffer = System.Text.Encoding.ASCII.GetBytes(msg);//将要发送的数据，生成字节数组。accSck.Send(buffer);
                    Console.WriteLine("向客户端发送了:"+msg);
                }
            }
        }staticSocket accSck =null;staticvoidJtSocket()
        {while(true)
            {               
                accSck=sck.Accept();               
                Console.WriteLine("链接成功！");
            }
        }
    }![复制代码](http://common.cnblogs.com/images/copycode.gif)
客户端连接ip为nginx代理服务器ip，客户端如下：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
classProgram
    {staticSocket clientSocket =null;staticThread thread =null;staticvoidMain(string[] args)
        {
            clientSocket=newSocket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);//这里联通nginx代理服务器地址ipIPAddress ip = IPAddress.Parse("172.18.8.252");
            IPEndPoint endpoint=newIPEndPoint(ip, Convert.ToInt32("11911"));
            clientSocket.Connect(endpoint);
            thread=newThread(ReceMsg);
            thread.IsBackground=true;
            thread.Start();
            Console.WriteLine("123");
            Console.ReadKey();
        }staticvoidReceMsg()
        {while(true)
            {byte[] buffer =newbyte[1024*1024*2];
                clientSocket.Receive(buffer);stringReceiveMsg = System.Text.Encoding.UTF8.GetString(buffer).Substring(0,30);
                Console.WriteLine("接收到数据:"+ReceiveMsg);
            }
        }
    }![复制代码](http://common.cnblogs.com/images/copycode.gif)

# 中间10分钟，搭建nginx服务器
准备一台linux服务器。nginx官网：[http://nginx.org/](http://nginx.org/)。在网上搜到大致用的是[ngx_stream_core_module](http://nginx.org/en/docs/stream/ngx_stream_core_module.html)这个模块，这里你也可以关注一下官方文档中的其他模块都是做什么的，那么这有相关的启用配置说明，与示例配置。
第一句便是：该`ngx_stream_core_module`模块是自1.9.0版本。此模块不是默认构建的，应使用配置参数启用`--with-stream`。
那好吧，我们就安装nginx，搞这个的，安装应该都会吧。
[root@localhost /]\#cd /usr/local/src[root@localhost src]\#wget http://nginx.org/download/nginx-1.11.10.tar.gz
然后解压，解压完，根据文档提示需要使用这个参数--with-stream 来启用功能。
[root@localhost src]\#./configure  --prefix=/usr/local/nginx --with-stream
然后，make，make install。
完成之后就是nginx配置配置文件啦，这个文档中有示例，可知与events模块平级，按照这做就好啦。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
worker_processes  1;\#error_log  logs/error.log;\#error_log  logs/error.log  notice;\#error_log  logs/error.log  info;\#pid        logs/nginx.pid;events {
    worker_connections1024;
}
stream{
    upstream abc{
        server172.18.8.196:11911;
    }
    server{
        listen11911;
        proxy_pass abc;
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
ok,保存退出，重启nginx使配置生效即可。到这里所有的操作就都完成啦，让我们拭目以待，看下结果吧。
# 最后10分钟，测试nginx转发socket结果
启动服务端--->显示：开启监听--->启动客户端----> 客户端显示：连接成功---->服务端输入：abc----->服务端：点击回车---->客户端显示：abc。如下
服务端：![](https://images2015.cnblogs.com/blog/398358/201703/398358-20170303173517360-996840859.png)客户端：![](https://images2015.cnblogs.com/blog/398358/201703/398358-20170303173539907-1579755675.png)
至此所有的都已整个过程都已结束，功能实现。
## 我这里只是想抛砖引玉，勿喷！！

原文地址：http://www.cnblogs.com/knowledgesea/p/6497783.html


