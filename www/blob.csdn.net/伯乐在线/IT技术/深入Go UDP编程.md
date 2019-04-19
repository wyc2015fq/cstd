# 深入Go UDP编程 - 文章 - 伯乐在线
原文出处： [smallnest](http://colobu.com/2016/10/19/Go-UDP-Programming/)
用户数据报协议（User Datagram Protocol，缩写为UDP），又称用户数据报文协议，是一个简单的面向数据报(package-oriented)的传输层协议，正式规范为RFC 768。UDP只提供数据的不可靠传递，它一旦把应用程序发给网络层的数据发送出去，就不保留数据备份（所以UDP有时候也被认为是不可靠的数据报协议）。UDP在IP数据报的头部仅仅加入了复用和数据校验。
由于缺乏可靠性且属于非连接导向协议，UDP应用一般必须允许一定量的丢包、出错和复制粘贴。但有些应用，比如TFTP，如果需要则必须在应用层增加根本的可靠机制。但是绝大多数UDP应用都不需要可靠机制，甚至可能因为引入可靠机制而降低性能。流媒体（流技术）、即时多媒体游戏和IP电话（VoIP）一定就是典型的UDP应用。如果某个应用需要很高的可靠性，那么可以用传输控制协议（TCP协议）来代替UDP。
由于缺乏拥塞控制（congestion control），需要基于网络的机制来减少因失控和高速UDP流量负荷而导致的拥塞崩溃效应。换句话说，因为UDP发送者不能够检测拥塞，所以像使用包队列和丢弃技术的路由器这样的网络基本设备往往就成为降低UDP过大通信量的有效工具。数据报拥塞控制协议（DCCP）设计成通过在诸如流媒体类型的高速率UDP流中，增加主机拥塞控制，来减小这个潜在的问题。
典型网络上的众多使用UDP协议的关键应用一定程度上是相似的。这些应用包括域名系统（DNS）、简单网络管理协议（SNMP）、动态主机配置协议（DHCP）、路由信息协议（RIP）和某些影音流服务等等。
UDP报头
![table1](http://jbcdn2.b0.upaiyun.com/2016/10/8c2cd6854c4b8e0c3ff95c4e5dc46a0b.png)
**IPv4伪头部![table2](http://jbcdn2.b0.upaiyun.com/2016/10/97bb0b7d7da40643dadd6dccc44516c4.png)**
**IPv6伪头部![table3](http://jbcdn2.b0.upaiyun.com/2016/10/39566e408035d3601a0f0a98ea0a5203.png)**
以上大段的背景介绍引自维基百科。
而TCP是面向连接(connection-oriented)的协议，可以提供可靠的数据传输。
本文讲介绍Go语言的UDP库及其使用方法，以及了解使用过程中的细节和陷阱。
## 一个简单的例子
首先看一个简单的UDP的例子，这个例子演示了Go UDP通过`Dial`方式发送数据报的例子。

C
```
package main
import (
	"fmt"
	"net"
)
func main() {
	listener, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 9981})
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("Local: <%s> \n", listener.LocalAddr().String())
	data := make([]byte, 1024)
	for {
		n, remoteAddr, err := listener.ReadFromUDP(data)
		if err != nil {
			fmt.Printf("error during read: %s", err)
		}
		fmt.Printf("<%s> %s\n", remoteAddr, data[:n])
		_, err = listener.WriteToUDP([]byte("world"), remoteAddr)
		if err != nil {
			fmt.Printf(err.Error())
		}
	}
}
```

C
```
package main
import (
	"fmt"
	"net"
)
func main() {
	sip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	conn.Write([]byte("hello"))
	fmt.Printf("<%s>\n", conn.RemoteAddr())
}
```
可以看到, Go UDP的处理类似TCP的处理，虽然不像TCP面向连接的方式`ListenTCP`和`Accept`的方式建立连接,但是它通过`ListenUDP`和`ReadFromUDP`可以接收各个客户端发送的数据报，并通过`WriteToUDP`写数据给特定的客户端。
我们稍微修改一下client1.go,让它保持UDP Socket文件一直打开：

C
```
func main() {
	ip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	b := make([]byte, 1)
	os.Stdin.Read(b)
	conn.Write([]byte("hello"))
	fmt.Printf("<%s>\n", conn.RemoteAddr())
}
```
使用 `netstat`可以看到这个网络文件描述符(因为我在同一台机器上运行服务器，所以你会看到两条记录，一个是服务器打开的，一个是客户端打开的)。

C
```
udp4       0      0  localhost.54676        localhost.9981
udp4       0      0  localhost.9981         *.*
```
或者使用`lsof`命令查看：

C
```
server1   59312 smallnest    3u  IPv4 0xad793a9a54467f61      0t0  UDP localhost:9981
client1   59323 smallnest    3u  IPv4 0xad793a9a544681c1      0t0  UDP localhost:54676->localhost:9981
```
## 更复杂的例子
我们还可以将上面的例子演化一下，实现双向的读写。
服务器端代码不用修改，因为它已经实现了读写，读是通过`listener.ReadFromUDP`,写通过`listener.WriteToUDP`。
客户端修改为读写：

C
```
package main
import (
	"fmt"
	"net"
)
func main() {
	ip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	conn.Write([]byte("hello"))
	data := make([]byte, 1024)
	n, err := conn.Read(data)
	fmt.Printf("read %s from <%s>\n", data[:n], conn.RemoteAddr())
}
```
这里client的写是`Write`,读是`Read`。
## 等价的客户端和服务器
下面这个是两个服务器通信的例子，互为客户端和服务器，在发送数据报的时候，我们可以将发送的一方称之为源地址，发送的目的地一方称之为目标地址。

C
```
package main
import (
	"fmt"
	"net"
	"os"
	"time"
)
func read(conn *net.UDPConn) {
	for {
		data := make([]byte, 1024)
		n, remoteAddr, err := conn.ReadFromUDP(data)
		if err != nil {
			fmt.Printf("error during read: %s", err)
		}
		fmt.Printf("receive %s from <%s>\n", data[:n], remoteAddr)
	}
}
func main() {
	addr1 := &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 9981}
	addr2 := &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 9982}
	go func() {
		listener1, err := net.ListenUDP("udp", addr1)
		if err != nil {
			fmt.Println(err)
			return
		}
		go read(listener1)
		time.Sleep(5 * time.Second)
		listener1.WriteToUDP([]byte("ping to #2: "+addr2.String()), addr2)
	}()
	go func() {
		listener1, err := net.ListenUDP("udp", addr2)
		if err != nil {
			fmt.Println(err)
			return
		}
		go read(listener1)
		time.Sleep(5 * time.Second)
		listener1.WriteToUDP([]byte("ping to #1: "+addr1.String()), addr1)
	}()
	b := make([]byte, 1)
	os.Stdin.Read(b)
}
```
## Read和Write方法集的比较
前面的例子中客户端有时使用`DialUDP`建立数据报的源对象和目标对象(地址和端口), 它会创建UDP Socket文件描述符,然后调用内部的`connect`为这个文件描述符设置源地址和目标地址，这时Go将它称之为`connected`,尽管我们知道UDP是无连接的协议，Go这种叫法我想根源来自Unix/Linux的UDP的实现。这个方法返回`*UDPConn`。
有的时候却可以通过`ListenUDP`返回的`*UDPConn`直接往某个目标地址发送数据报，而不是通过`DialUDP`方式发送，原因在于两者返回的`*UDPConn`是不同的。前者是`connected`，后者是`unconnected`。
你必须清楚知道你的UDP是连接的(connected)还是未连接(unconnected)的，这样你才能正确的选择的读写方法。
如果`*UDPConn`是`connected`,读写方法是`Read`和`Write`。
如果`*UDPConn`是`unconnected`,读写方法是`ReadFromUDP`和`WriteToUDP`（以及`ReadFrom`和`WriteTo`)。
事实上Go的这种设计和Unix/Linux设计一致，下面是Linux关于UDP的文档：
> 
When a UDP socket is created, its local and remote addresses are unspecified. Datagrams can be sent immediately using `sendto` or `sendmsg` with a valid destination address as an argument. When `connect` is called on the socket, the default destination address is set and datagrams can now be sent using `send` or `write` without specifying a destination address. It is still possible to send to other destinations by passing an address to `sendto` or `sendmsg`. In order to receive packets, the socket can be bound to a local address first by using `bind`. Otherwise, the socket layer will automatically assign a free local port out of the range defined by `/proc/sys/net/ipv4/ip_local_port_range` and bind the socket to INADDR_ANY.
`ReadFrom`和`WriteTo`是为了实现`PacketConn`接口而实现的方法，它们的实现基本上和`ReadFromUDP`和`WriteToUDP`一样，只不过地址换成了更通用的`Addr`,而不是具体化的`UDPAddr`。
还有几种情况需要弄清楚:
**1、**
因为`unconnected`的`*UDPConn`还没有目标地址，所以需要把目标地址当作参数传入到`WriteToUDP`的方法中，但是`unconnected`的`*UDPConn`可以调用`Read`方法吗？
答案是**可以**,但是在这种情况下，客户端的地址信息就被忽略了。

C
```
func main() {
	listener, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 9981})
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("Local: <%s> \n", listener.LocalAddr().String())
	data := make([]byte, 1024)
	for {
		n, err := listener.Read(data)
		if err != nil {
			fmt.Printf("error during read: %s", err)
		}
		fmt.Printf("<%s>\n", data[:n])
	}
}
```
**2、**
`unconnected`的`*UDPConn`可以调用`Write`方法吗？
答案是**不可以**， 因为不知道目标地址。

C
```
func main() {
	listener, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.ParseIP("127.0.0.1"), Port: 9981})
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("Local: <%s> \n", listener.LocalAddr().String())
	_, err = listener.Write([]byte("hello"))
	if err != nil {
		fmt.Printf(err.Error())
	}
}
```
出错：

C
```
write udp 127.0.0.1:9981: write: destination address requiredsmallnestMBP:udp smallnest
```
**3、**
`connected`的`*UDPConn`可以调用`WriteToUDP`方法吗？
答案是**不可以**， 因为目标地址已经设置。
即使是相同的目标地址也**不可以**。

C
```
func main() {
	ip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	_, err = conn.WriteToUDP([]byte("hello"), dstAddr)
	if err != nil {
		fmt.Println(err)
	}
}
```
报错:

C
```
write udp 127.0.0.1:50141->127.0.0.1:9981: use of WriteTo with pre-connected connection
```
**4、**
`connected`的`*UDPConn`如果调用`Closed`以后可以调用`WriteToUDP`方法吗？
答案是**不可以**。

C
```
func main() {
	ip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	err = conn.Close()
	if err != nil {
		fmt.Println(err)
	}
	_, err = conn.WriteToUDP([]byte("hello"), dstAddr)
	if err != nil {
		fmt.Println(err)
	}
}
```
同样的报错：

C
```
write udp 127.0.0.1:59074->127.0.0.1:9981: use of WriteTo with pre-connected connection
```
**5、**
`connected`的`*UDPConn`可以调用`ReadFromUDP`方法吗？
答案是**可以**,但是它的功能基本和`Read`一样，只能和它`connected`的对端通信。
看下面的client的例子：

C
```
func main() {
	ip := net.ParseIP("127.0.0.1")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	go func() {
		data := make([]byte, 1024)
		for {
			n, remoteAddr, err := conn.ReadFromUDP(data)
			if err != nil {
				fmt.Printf("error during read: %s", err)
			}
			fmt.Printf("<%s> %s\n", remoteAddr, data[:n])
		}
	}()
	conn.Write([]byte("hello"))
	b := make([]byte, 1)
	os.Stdin.Read(b)
}
```
**6、**
`*UDPConn`还有一个通用的`WriteMsgUDP(b, oob []byte, addr *UDPAddr)`，同时支持`connected`和`unconnected`的UDPConn:
- 如果`UDPConn`还未连接，那么它会发送数据报给addr
- 如果`UDPConn`已连接，那么它会发送数据报给连接的对端，这种情况下addr应该为nil
## 通用多播编程
Go标准库也支持多播，但是我们首先我们看通用的多播是如何实现的，它使用`golang.org/x/net/ipv4`或者`golang.org/x/net/ipv6`进行控制。
首先找到要进行多播所使用的网卡,然后监听本机合适的地址和服务端口。
将这个应用加入到多播组中，它就可以从组中监听包信息，当然你还可以对包传输进行更多的控制设置。
应用收到包后还可以检查包是否来自这个组的包。
完整的代码如下：

C
```
package main
import (
	"fmt"
	"net"
	"golang.org/x/net/ipv4"
)
func main() {
	//1. 得到一个interface
	en4, err := net.InterfaceByName("en4")
	if err != nil {
		fmt.Println(err)
	}
	group := net.IPv4(224, 0, 0, 250)
	//2. bind一个本地地址
	c, err := net.ListenPacket("udp4", "0.0.0.0:1024")
	if err != nil {
		fmt.Println(err)
	}
	defer c.Close()
	//3.
	p := ipv4.NewPacketConn(c)
	if err := p.JoinGroup(en4, &net.UDPAddr{IP: group}); err != nil {
		fmt.Println(err)
	}
	//4.更多的控制
	if err := p.SetControlMessage(ipv4.FlagDst, true); err != nil {
		fmt.Println(err)
	}
	//5.接收消息
	b := make([]byte, 1500)
	for {
		n, cm, src, err := p.ReadFrom(b)
		if err != nil {
			fmt.Println(err)
		}
		if cm.Dst.IsMulticast() {
			if cm.Dst.Equal(group) {
				fmt.Printf("received: %s from <%s>\n", b[:n], src)
				n, err = p.WriteTo([]byte("world"), cm, src)
				if err != nil {
					fmt.Println(err)
				}
			} else {
				fmt.Println("Unknown group")
				continue
			}
		}
	}
}
```
同一个应用可以加入到多个组中，多个应用也可以加入到同一个组中。
多个UDP listener可以监听同样的端口，加入到同一个group中。
> 
It is possible for multiple UDP listeners that listen on the same UDP port to join the same multicast group. The net package will provide a socket that listens to a wildcard address with reusable UDP port when an appropriate multicast address prefix is passed to the net.ListenPacket or net.ListenUDP.


C
```
c1, err := net.ListenPacket("udp4", "224.0.0.0:1024")
if err != nil {
	// error handling
}
defer c1.Close()
c2, err := net.ListenPacket("udp4", "224.0.0.0:1024")
if err != nil {
	// error handling
}
defer c2.Close()
p1 := ipv4.NewPacketConn(c1)
if err := p1.JoinGroup(en0, &net.UDPAddr{IP: net.IPv4(224, 0, 0, 248)}); err != nil {
	// error handling
}
p2 := ipv4.NewPacketConn(c2)
if err := p2.JoinGroup(en0, &net.UDPAddr{IP: net.IPv4(224, 0, 0, 248)}); err != nil {
	// error handling
}
```
还支持`Source-specific multicasting`特性。
## 标准库多播编程
标准库的多播编程简化了上面的操作，当然也减少了更多的控制。如果想实现一个简单的多播程序，可以使用这样的方法。
服务器端的代码：

C
```
func main() {
	//如果第二参数为nil,它会使用系统指定多播接口，但是不推荐这样使用
	addr, err := net.ResolveUDPAddr("udp", "224.0.0.250:9981")
	if err != nil {
		fmt.Println(err)
	}
	listener, err := net.ListenMulticastUDP("udp", nil, addr)
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("Local: <%s> \n", listener.LocalAddr().String())
	data := make([]byte, 1024)
	for {
		n, remoteAddr, err := listener.ReadFromUDP(data)
		if err != nil {
			fmt.Printf("error during read: %s", err)
		}
		fmt.Printf("<%s> %s\n", remoteAddr, data[:n])
	}
}
```
写个客户端测试一下：

C
```
func main() {
	ip := net.ParseIP("224.0.0.250")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.DialUDP("udp", srcAddr, dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	defer conn.Close()
	conn.Write([]byte("hello"))
	fmt.Printf("<%s>\n", conn.RemoteAddr())}
```
## 广播
广播的编程方式和多播的编程方式有所不同。简单说，广播意味着你吼一嗓子，局域网内的所有的机器都会收到。
服务器端代码：

C
```
func main() {
	listener, err := net.ListenUDP("udp", &net.UDPAddr{IP: net.IPv4zero, Port: 9981})
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("Local: <%s> \n", listener.LocalAddr().String())
	data := make([]byte, 1024)
	for {
		n, remoteAddr, err := listener.ReadFromUDP(data)
		if err != nil {
			fmt.Printf("error during read: %s", err)
		}
		fmt.Printf("<%s> %s\n", remoteAddr, data[:n])
		_, err = listener.WriteToUDP([]byte("world"), remoteAddr)
		if err != nil {
			fmt.Printf(err.Error())
		}
	}
}
```
客户端代码有所不同，它不是通过`DialUDP` “连接” 广播地址，而是通过`ListenUDP`创建一个`unconnected`的 `*UDPConn`,然后通过`WriteToUDP`发送数据报，这和你脑海中的客户端不太一致：

C
```
func main() {
	ip := net.ParseIP("172.24.14.255")
	srcAddr := &net.UDPAddr{IP: net.IPv4zero, Port: 0}
	dstAddr := &net.UDPAddr{IP: ip, Port: 9981}
	conn, err := net.ListenUDP("udp", srcAddr)
	if err != nil {
		fmt.Println(err)
	}
	n, err := conn.WriteToUDP([]byte("hello"), dstAddr)
	if err != nil {
		fmt.Println(err)
	}
	data := make([]byte, 1024)
	n, _, err = conn.ReadFrom(data)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("read %s from <%s>\n", data[:n], conn.RemoteAddr())
	b := make([]byte, 1)
	os.Stdin.Read(b)
}
```
你局域网内的广播地址可能和例子中的不同，你可以通过`ifconfig`查看。
广播地址(Broadcast Address)是专门用于同时向网络中所有工作站进行发送的一个地址。在使用TCP/IP 协议的网络中，主机标识段host ID 为全1 的IP 地址为广播地址，广播的分组传送给host ID段所涉及的所有计算机。例如，对于10.1.1.0 （255.255.255.0 ）网段，其广播地址为10.1.1.255 （255 即为2 进制的11111111 ），当发出一个目的地址为10.1.1.255 的分组（封包）时，它将被分发给该网段上的所有计算机。
## 任播
在互联网中，通常使用边界网关协议来实现任播。比如域名根服务器就是通过任播的方式提供。13台根服务器使用13个任播地址，但是有500多台实际服务器。你可以通过单播的方式发送数据包，只有最快的(最近的)的一个UDP服务器接收到。
Anycasting最初是在RFC1546中提出并定义的，它的最初语义是，在IP网络上通过一个Anycast地址标识一组提供特定服务的主机，同时服务访问方并不关心提供服务的具体是哪一台主机(比如DNS或者镜像服务)，访问该地址的报文可以被IP网络路由到这一组目标中的任何一台主机上，它提供的是一种无状态的、尽力而为的服务。
RFC2373(IP Version 6 Addressing Architecture, July 1998)提供了较新的说明和动机：任播地址的一个期望应用是标识属于某个提供互联网服务的机构的路由器集合。这种地址可以用作IPv6路由标题的中间地址,使数据分组通过某一聚合或聚合序列传递。其他可能的用途是标识属于某一子网的路由器组或提供进入某一路由范围入口的路由器组。
RFC2373标准对任播的定义是，当一个单播地址被分配到多于一个的接口上时，发到该接口的报文被网络路由到由路由协议度量的“最近”的目标接口上。与Unicast和Multicast类似，Anycast也是IP网络的一种通信模式。Unicast允许源结点向单一目标结点发送数据报，Multicast允许源结点向一组目标结点发送数据报，而Anycast则允许源结点向一组目标结点中的一个结点发送数据报，而这个结点由路由系统选择，对源结点透明；同时，路由系统选择“最近”的结点为源结点提供服务，从而在一定程序上为源结点提供了更好的服务也减轻了网络负载。
## 参考文档
- [https://zh.wikipedia.org/wiki/用户数据报协议](https://zh.wikipedia.org/wiki/%E7%94%A8%E6%88%B7%E6%95%B0%E6%8D%AE%E6%8A%A5%E5%8D%8F%E8%AE%AE)
- [https://golang.org/pkg/net/](https://golang.org/pkg/net/)
- [http://man7.org/linux/man-pages/man7/udp.7.html](http://man7.org/linux/man-pages/man7/udp.7.html)
- [https://godoc.org/golang.org/x/net/ipv4](https://godoc.org/golang.org/x/net/ipv4)
- [https://github.com/golang/go/issues/13391](https://github.com/golang/go/issues/13391)
- [http://baike.baidu.com/view/473043.htm](http://baike.baidu.com/view/473043.htm)
- [http://baike.baidu.com/view/2032315.htm](http://baike.baidu.com/view/2032315.htm)
