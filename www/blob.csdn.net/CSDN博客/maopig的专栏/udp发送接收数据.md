# udp发送接收数据 - maopig的专栏 - CSDN博客
2011年08月18日 23:13:11[maopig](https://me.csdn.net/maopig)阅读数：995标签：[buffer																[string																[byte																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
个人分类：[网络](https://blog.csdn.net/maopig/article/category/869532)
using System;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Collections.Generic;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Text;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Net;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Net.Sockets;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Threading;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)namespace jieshou
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)class Program
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)staticvoid Main(string[] args)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                Jie();
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)staticvoid Jie()
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)int i =0;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)             Socket socket =new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);//初始化一个Scoket协议
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            IPEndPoint iep =new IPEndPoint(IPAddress.Any, 3000);//初始化一个侦听局域网内部所有IP和指定端口
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            EndPoint ep = (EndPoint)iep;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            socket.Bind(iep);//绑定这个实例
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)while (true)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)byte[] buffer =newbyte[1024];//设置缓冲数据流
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                socket.ReceiveFrom(buffer, ref ep);//接收数据,并确把数据设置到缓冲流里面
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                Console.WriteLine(Encoding.Unicode.GetString(buffer).TrimEnd('\u0000') +""+ i.ToString());
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                i++;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Collections.Generic;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Text;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Net;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Net.Sockets;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)using System.Threading;
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)namespace guangbo
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)class Program
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//private Thread trd;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)publicint num =0;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)staticvoid Main(string[] args)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//Thread trd = new Thread(new ThreadStart(guangbo()));
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//trd.IsBackground = true;
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//trd.Start();
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)for (int i =0; i <10000; i++)
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                Console.WriteLine(i);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//Thread.Sleep(10000);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)                Guang();
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)            }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            Console.ReadKey();
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)staticvoid Guang()
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cnblogs.com/Images/OutliningIndicators/ContractedSubBlock.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            Socket sock =new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);//初始化一个Scoket实习,采用UDP传输
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            IPAddress ip = IPAddress.Parse("192.168.111.195");
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//ip可以用IPAddress.Any来代替全发送。
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            IPEndPoint iep =new IPEndPoint(ip, 3000);//初始化一个发送广播和指定端口的网络端口实例
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            sock.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);//设置该scoket实例的发送形式
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)string request ="你好，hi";//初始化需要发送而的发送数据
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)byte[] buffer = Encoding.Unicode.GetBytes(request);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            sock.SendTo(buffer, iep);
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)            sock.Close();            
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//Console.WriteLine("nihao");
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
