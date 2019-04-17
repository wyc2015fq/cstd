# Wireshark抓包常见问题解析 - DoubleLi - 博客园






**1.   tcp out-of-order****（tcp****有问题）**

解答：

1）、    应该有很多原因。但是多半是网络拥塞，导致顺序包抵达时间不同，延时太长，或者包丢失，需要重新组合数据单元 因为他们可能是通过不同的路径到达你电脑上面的。

2）、    CRM IT 同仁上礼拜来跟我反应一个问题，由他们客服系统藉由邮件主机要寄送给客户的信件，常常会有寄送失败的问题，查看了一下 Log，发现正常的信件在主机接收 DATA 完成后会记录收到的邮件大小，然后开始进行后续寄送出去的处理，但这些有问题的寄送，都会发生 DATA 没有传送完，Server 就记录已读取到 EOF，然后结束连线，也因此这封信就不算顺利的送到 Server 上来。

初步看了一下排除是 Timeout 问题，因为连线断的时间都还未达设定的连线 Timeout 时间，由于 CRM 系统是外面厂商写的，为了厘清问题我只好抓封包来看是不是用户端送出来结束传送的指令的。

抓了一下结果如下：

![](http://www.xianren.org/uploads/2012/05/bao1.jpg)

整封邮件的传送过程，包含了大量的 TCP Retransmission 或是 Segment Lost，到后来还有跑出 TCP Out-Of-Order，看起来是网路的问题，网路上对于 TCP Out-Of-Order 的建议是说，有些 Packet 可能 Lost，所以重新传送造成，另一个可能是因为 Client 到 Server 间有两条网路路径，像是 Load Balance 之类的架构，因此若两个封包走不同路径，晚送的封包却比早送的到达，就会发生 Out-Of-Order。

因此在断定有可能是网路造成，加上 CRM 系统上的网卡同事是把两张做成一张 Virtual，再请他拿掉 Bonding 只用单一张跑以后，问题就不存在了，观察流量还跑的比原本两张合起来的 Virtual 单张跑的高，所以 M$ 在 Bonding 网卡上是不是还有什么需要调整的就不得而之了，至少找出造成大量寄送失败的原因就好。

**2.   tcp segment of a reassembled PDU**

解答：1）在连个连接建立的时候，SYN包里面会把彼此TCP最大的报文段长度，在局域网内一般都是1460.如果发送的包比最大的报文段长度长的话就要分片了，被分片出来的包，就会被标记了“TCP segment of a reassembled PDU”，可以参考下图，看一下，被标记了的包的SEQ和ACK都和原来的包一致：

![](http://www.xianren.org/uploads/2012/05/bao2.jpg)

2）上周在公司里遇到一个问题，用wireshark抓系统给网管上报的数据发现里面有好多报文被标识为“TCP segment of a reassembled PDU”，并且每一段报文都是180Byte，当时看到这样的标识，觉得是IP报文分片，以为系统的接口MTU值为设置小了，通过命令查询发现是1500，没有被重设过，当时有点想不通。

回来查了一下，发现自己的理解是错的，“TCP segment of a reassembled PDU”指的不是IP层的分片，IP分片在wireshark里用“Fragmented IP protocol”来标识。详细查了一下，发现“TCP segment of a reassembled PDU”指TCP层收到上层大块报文后分解成段后发出去。于是有个疑问，TCP层完全可以把大段报文丢给IP层，让IP层完成分段，为什么要在TCP层分呢？其实这个是由TCP的MSS(Maximum Segment Size，最大报文段长度)决定的，TCP在发起连接的第一个报文的TCP头里通过MSS这个可选项告知对方本端能够接收的最大报文（当然，这个大小是TCP净荷的大小），以太网上这个值一般设置成1460，因为1460Byte净荷+20Byte TCP头+20Byte IP头＝ 1500字节，正好符合链路层最大报文的要求。

至于收到一个报文后如何确定它是一个”TCP segment”？如果有几个报文的ACK序号都一样，并且这些报文的Sequence Number都不一样，并且后一个Sequence Number为前一个Sequence Number加上前一个报文大小再加上1的话，肯定是TCP segment了，对于没有ACK标志时，则无法判断。

既然收到的TCP报文都是180Byte的segment，那么应该是协商的时候PC端告知了MSS为180Byte，至于为什么这样，只能等抓包后确认是MSS的问题再排查了。另外，有一种情况也可能导致这个问题：被测系统因为MTU为220Byte而设置MSS为180Byte，但是这种情况现在可以排除，因为前面讲过，已经查询过MTU值为1500。

**3.   Tcp previous segment lost****（tcp****先前的分片丢失）**

解答：

（1）、“TCP Previous segment lost” errors are not “fatal” errors. They simply indicate that the sequence number in the arriving packet is higher than the next-expected sequence number, indicating that at least one segment was dropped/lost. The receiving station remedies this situation by sending duplicate ACKs for each additional packet it receives until the sender retransmits the missing packet(s). TCP is designed to recover from this situation, which is why the image is downloaded correctly despite having a (briefly) missing packet.

If you are getting a large number of lost packets, then there is likely a communication problem between the sender and receiver. A common cause of this is un-matched duplex settings between the PC and the switch.

We (our lab) recently upgraded to Ethereal 0.10.14 with WinPCap 3.1.  If I remember correctly, we had previously been using 0.10.2 with WinPCap 3.0.  However, since the upgrade we have been noticing several issues.

The first issue is with “TCP Previous segment lost” and “TCP CHECKSUM INCORRECT” messages appearing in the Packet Listing window.  We do not remember seeing these in the previous version of Ethereal, or at least not nearly as many as we are seeing now.  For example, one task for the student instructional part of the lab involves visiting a website containing two images and observing the network activity.  After the two GET requests are sent for the images, it is not uncommon for one image to be returned with a typical 200 OK response packet, but the response packet for the other image will be displayed as “TCP Previous segment lost.”  However, both images are downloaded and displayed perfectly fine in the browser.  I would think that the segment lost error would mean the object wasn’t returned correctly and shouldn’t be able to be displayed, but apparently that is not the case.  (The cache had been cleared when this was performed, so it was not defaulting to a local copy of the image.)

Another problem we’ve been noticing is that some packets simply aren’t displayed in the Packet Listing window, even when they are obviously received.  Using the same example as above, after the two GET requests are sent for the images, it is not uncommon for one image to be returned with a typical 200 OK response, but the other response will not appear.  Yet both images are successfully displayed in the browser.  Is this a problem with Ethereal not detecting the packets?

I’m not sure how typical this is, but we seem to be experiencing these issues often with 0.10.14 while we never did with 0.10.2.  Could it also be an issue with WinPCap, and not necessarily Ethereal?  I’m just trying to find some answers as to why we are seeing a sudden abundance of TCP related errors and uncaptured packets.  Thanks.

（2）、I have a network client application that runs fine while I am debugging (no TCP errors),

but when I run the release version, it runs incredibly slow.  It runs as a series of

transactions, where each transaction is a separate connection to the server.  Wireshark

analysis has determined that about 50% of all transactions involve the series:

TCP Previous Segment Lost

TCP Dup ACK

RST

The RST consumes 3 seconds per transaction, which is a Big Deal.  So to prevent it, I must

prevent the initial “TCP Previous Segment Lost” (which seems, on the surface, to merely be

a time-out on a particular segment).

In the following clip, the SYN packet suffers from the “TCP Previous Segment Lost” condition.

0.000640 seconds seems like too short of a time to declare this condition, as many previous

successful transactions took much longer to be successfully SYN-ACK’ed.

Can somebody explain “TCP Previous Segment Lost” in this context to help me troubleshoot my

problem?

Any help would be appreciated.

Here is a clip of a problem transaction:

![fffgs](http://www.xianren.org/uploads/2012/05/fffgs.jpg)

**4.   Tcpacked lost segment****（tcp****应答丢失）**

**5.   Tcp window update****（tcp****窗口更新）**

**6.   Tcp dup ack****（tcp****重复应答）**

TCP may generate an immediate acknowledgment (a duplicate ACK) when an out- of-order segment is received. This duplicate ACK should not be delayed. The purpose of this duplicate ACK is to let the other end know that a segment was received out of order, and to tell it what sequence number is expected.

当收到一个出问题的分片，Tcp立即产生一个应答。这个相同的ack不会延迟。这个相同应答的意图是让对端知道一个分片被收到的时候出现问题，并且告诉它希望得到的序列号。

Since TCP does not know whether a duplicate ACK is caused by a lost segment or just a reordering of segments, it waits for a small number of duplicate ACKs to be received. It is assumed that if there is just a reordering of the segments, there will be only one or two duplicate ACKs before the reordered segment is processed, which will then generate a new ACK. If three or more duplicate ACKs are received in a row, it is a strong indication that a segment has been lost. TCP then performs a retransmission of what appears to be the missing segment, without waiting for a retransmission timer to expire.

**7.   Tcp keep alive****（tcp****保持活动）**

在TCP中有一个Keep-alive的机制可以检测死连接，原理很简单，TCP会在空闲了一定时间后发送数据给对方：

1.如果主机可达，对方就会响应ACK应答，就认为是存活的。

2.如果可达，但应用程序退出，对方就发RST应答，发送TCP撤消连接。

3.如果可达，但应用程序崩溃，对方就发FIN消息。

4.如果对方主机不响应ack, rst，继续发送直到超时，就撤消连接。这个时间就是默认

的二个小时。

uses WinSock2;

procedure TForm1.IdTCPServer1Connect(AThread: TIdPeerThread);

type

TCP_KeepAlive = record

OnOff: Cardinal;

KeepAliveTime: Cardinal;

KeepAliveInterval: Cardinal

end;

var

Val: TCP_KeepAlive;

Ret: DWord;

begin

Val.OnOff:=1;

Val.KeepAliveTime:=6000; //6s

Val.KeepAliveInterval:=6000; //6s

WSAIoctl(AThread.Connection.Socket.Binding.Handle, IOC_IN or IOC_VENDOR or 4,

@Val, SizeOf(Val), nil, 0, @Ret, nil, nil)

end;

——————————————————–

KeepAliveTime值控制 TCP/IP 尝试验证空闲连接是否完好的频率。如果这段时间内没有活动，则会发送保持活动信号。如果网络工作正常，而且接收方是活动的，它就会响应。如果需要对丢失接收方敏感，换句话说，需要更快地发现丢失了接收方，请考虑减小这个值。如果长期不活动的空闲连接出现次数较多，而丢失接收方的情况出现较少，您可能会要提高该值以减少开销。缺省情况下，如果空闲连接 7200000 毫秒（2 小时）内没有活动，Windows 就发送保持活动的消息。通常，1800000 毫秒是首选值，从而一半的已关闭连接会在 30 分钟内被检测到。

KeepAliveInterval值定义了如果未从接收方收到保持活动消息的响应，TCP/IP 重复发送保持活动信号的频率。当连续发送保持活动信号、但未收到响应的次数超出TcpMaxDataRetransmissions的值时，会放弃该连接。如果期望较长的响应时间，您可能需要提高该值以减少开销。如果需要减少花在验证接收方是否已丢失上的时间，请考虑减小该值或TcpMaxDataRetransmissions值。缺省情况下，在未收到响应而重新发送保持活动的消息之前，Windows 会等待 1000 毫秒（1 秒）。

KeepAliveTime根据你的需要设置就行，比如10分钟，注意要转换成MS。

XXX代表这个间隔值得大小

**8.   Tcp retransmission****（tcp****重传）**

作为一个可靠的传输协议，传输控制协议（TCP）在发送主机需要从目标主机收到一个包时确认。If the sender does not receive that acknowledgment within a certain amount of time, it acts under the assumption that the packet did not reach its destination and retransmits the packet.如果发件人没有收到的时间内一定之金额，确认，它的行为假设下，该数据包没有到达其目的地，以及转发数据包。









