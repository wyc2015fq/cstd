# TCP和UDP的区别 - L_Andy的专栏 - CSDN博客

2012年03月14日 14:46:00[卡哥](https://me.csdn.net/L_Andy)阅读数：762标签：[tcp																[attributes																[structure																[网络																[audio																[methods](https://so.csdn.net/so/search/s.do?q=methods&t=blog)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)
个人分类：[网络工程](https://blog.csdn.net/L_Andy/article/category/1925965)



TCP与UDP区别

TCP---传输控制协议,提供的是面向连接、可靠的字节流服务。当客户和服务器彼此交换数据前，必须先在双方之间建立一个TCP连接，之后才能传输数据。TCP提供超时重发，丢弃重复数据，检验数据，流量控制等功能，保证数据能从一端传到另一端。

UDP---用户数据报协议，是一个简单的面向数据报的运输层协议。UDP不提供可靠性，它只是把应用程序传给IP层的数据报发送出去，但是并不能保证它们能到达目的地。由于UDP在传输数据报前不用在客户和服务器之间建立一个连接，且没有超时重发等机制，故而传输速度很快

#### *Overview*

TCP (Transmission Control Protocol) is the most commonly used protocol on the Internet. The reason for this is because TCP offers error correction. When the TCP protocol is used there is a "guaranteed delivery." This is due largely in part to a method called
 "flow control." Flow control determines when data needs to be re-sent, and stops the flow of data until previous packets are successfully transferred. This works because if a packet of data is sent, a collision may occur. When this happens, the client re-requests
 the packet from the server until the whole packet is complete and is identical to its original.

UDP (User Datagram Protocol) is anther commonly used protocol on the Internet. However, UDP is never used to send important data such as webpages, database information, etc; UDP is commonly used for streaming audio and video. Streaming media such as Windows
 Media audio files (.WMA) , Real Player (.RM), and others use UDP because it offers speed! The reason UDP is faster than TCP is because there is no form of flow control or error correction. The data sent over the Internet is affected by collisions, and errors
 will be present. Remember that UDP is **only** concerned with speed. This is the main reason why streaming media is not high quality.

![](http://www.skullbox.net/udp.jpg)
![](http://www.skullbox.net/tcp.jpg)

On the contrary, UDP has been implemented among some trojan horse viruses. Hackers develop scripts and trojans to run over UDP in order to mask their activities. UDP packets are also used in DoS (Denial of Service) attacks. It is important to know the difference
 between TCP port 80 and UDP port 80. If you don't know what ports are go [here](http://www.skullbox.net/ports.php).

#### *Frame Structure*

As data moves along a network, various attributes are added to the file to create a*frame*. This process is called
*encapsulation*. There are different methods of encapsulation depending on which protocol and[topology](http://www.skullbox.net/ntoplogy.php) are being used. As a result, the frame structure of these packets
 differ as well. The images below show both the TCP and UDP frame structures.

## TCP FRAME STRUCTURE
![](http://www.skullbox.net/diagrams/tcppacket.gif)
## UDP FRAME STRUCTURE
![](http://www.skullbox.net/diagrams/udppacket.gif)

The *payload* field contains the actually data. Notice that TCP has a more complex frame structure. This is largely due to the fact the TCP is a connection-oriented protocol. The extra fields are need to ensure the "guaranteed delivery" offered by TCP.

**UDP**

    UDP 与 TCP 的主要区别在于 UDP 不一定提供可靠的数据传输。事实上，该协议不能保证数据准确无误地到达目的地。UDP 在许多方面非常有效。当某个程序的目标是尽快地传输尽可能多的信息时（其中任意给定数据的重要性相对较低），可使用 UDP。ICQ 短消息使用 UDP 协议发送消息。 

    许多程序将使用单独的TCP连接和单独的UDP连接。重要的状态信息随可靠的TCP连接发送，而主数据流通过UDP发送。
**TCP**

    TCP的目的是提供可靠的数据传输，并在相互进行通信的设备或服务之间保持一个虚拟连接。TCP在数据包接收无序、丢失或在交付期间被破坏时，负责数据恢复。它通过为其发送的每个数据包提供一个序号来完成此恢复。记住，较低的网络层会将每个数据包视为一个独立的单元，因此，数据包可以沿完全不同的路径发送，即使它们都是同一消息的组成部分。这种路由与网络层处理分段和重新组装数据包的方式非常相似，只是级别更高而已。

    为确保正确地接收数据，TCP要求在目标计算机成功收到数据时发回一个确认（即 ACK）。如果在某个时限内未收到相应的 ACK，将重新传送数据包。如果网络拥塞，这种重新传送将导致发送的数据包重复。但是，接收计算机可使用数据包的序号来确定它是否为重复数据包，并在必要时丢弃它。

**TCP与UDP的选择**

    如果比较UDP包和TCP包的结构，很明显UDP包不具备TCP包复杂的可靠性与控制机制。与TCP协议相同，UDP的源端口数和目的端口数也都支持一台主机上的多个应用。一个16位的UDP包包含了一个字节长的头部和数据的长度，校验码域使其可以进行整体校验。（许多应用只支持UDP，如：多媒体数据流，不产生任何额外的数据，即使知道有破坏的包也不进行重发。）  

    很明显，当数据传输的性能必须让位于数据传输的完整性、可控制性和可靠性时，TCP协议是当然的选择。当强调传输性能而不是传输的完整性时，如：音频和多媒体应用，UDP是最好的选择。在数据传输时间很短，以至于此前的连接过程成为整个流量主体的情况下，UDP也是一个好的选择，如：DNS交换。把SNMP建立在UDP上的部分原因是设计者认为当发生网络阻塞时，UDP较低的开销使其有更好的机会去传送管理数据。TCP丰富的功能有时会导致不可预料的性能低下，但是我们相信在不远的将来，TCP可靠的点对点连接将会用于绝大多数的网络应用。


