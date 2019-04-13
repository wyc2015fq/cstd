
# tcp-ip--面试中遇到几个问题 - 3-Number - CSDN博客


2017年12月25日 16:07:20[庞叶蒙](https://me.csdn.net/pangyemeng)阅读数：391


## 0x01 缘由
前段时间遇到的几个问题，现在简单总结和分析下：
1、tcp三次握手过程中交换了哪些信息？
2、一次tcp会话过程中序列号初始值是固定还是随机？
3、tcp三次握手过程中，三次握手的最后一个过程数据包在网络中丢掉，该如何处理？
4、TIME_WATE的作用？
5、保证tcp可靠传输的方法？
以上问题在实际应用中还是使用较少，但在排查关键问题时，通过wireshark抓包分析通讯过程，还是有较大的帮助。
## 0x02 问题解析
我将结合Linux tcp/ip协议栈源码和抓包分析，去解答如上问题。
![](https://img-blog.csdn.net/20171225160722155)
**问题1：tcp三次握手过程中交换了哪些信息？**
Windows size（滑动窗口）：告诉对端我这边能接收多少数据，从而达到一定的流控的效果。
MSS（最大报文长度）:每一个TCP报文段中数据字段的最大长度，注意：只是数据部分的字段，不包括TCP的头部。TCP在三次握手中，每一方都会通告其期望收到的MSS（MSS只出现在SYN数据包中）如果一方不接受另一方的MSS值则定位默认值536byte。
WS（窗口因子 window scale）：TCP发送端在发送一个满窗口长度（最大65535字节）的数据后必须等待对端的ACK更新窗口后才能继续发送数据。在广域网中传输数据时，由于往返时间较长，发送端等待的时间也会较长，这样会使得TCP数据交互的速度大大降低（长肥管道现象）。使用窗口扩大选项可以使得发送端得到更大的通告窗口，这样就可以在ACK到来前发送更多的数据，减少了等待的时间，提高了数据传输效率。
SACK_PERM:TCP收到乱序数据后，会将其放入乱序队列中，然后发送重复ACK给对端。对端收到多个重复的ACK后，就会推测到可能发生了数据丢失，再重传数据。如果乱序的数据比较零散，则这种机制的效率会很低。使用SACK选项可以告知发包方收到了哪些数据，发包方收到这些信息后就会知道哪些数据丢失，然后立即重传缺失部分即可。这就大大提高了数据重传的速度。
**问题2：一次tcp会话过程中序列号初始值是固定还是随机？**
TCP初始化序列号不能设置为一个固定值，因为这样容易被攻击者猜出后续序列号，从而遭到攻击。
RFC1948中提出了一个较好的初始化序列号ISN随机生成算法。
ISN = M + F(localhost, localport, remotehost, remoteport).
M是一个计时器，这个计时器每隔4毫秒加1。
F是一个Hash算法，根据源IP、目的IP、源端口、目的端口生成一个随机数值。要保证hash算法不能被外部轻易推算得出，用MD5算法是一个比较好的选择。
看来此处是个随机值，之前只了解只是一个随机值，没有进一步去查看随机算法。
![](https://img-blog.csdn.net/20171225160745257)
实现：
```python
__u32 secure_tcp_sequence_number(__be32 saddr, __be32 daddr,
                 __be16 sport, __be16 dport)
{
    __u32 seq;
    __u32 hash[4];
    struct keydata *keyptr = get_keyptr();
    /*
     *  Pick a unique starting offset for each TCP connection endpoints
     *  (saddr, daddr, sport, dport).
     *  Note that the words are placed into the starting vector, which is
     *  then mixed with a partial MD4 over random data.
     */
    hash[0] = (__force u32)saddr;
    hash[1] = (__force u32)daddr;
    hash[2] = ((__force u16)sport << 16) + (__force u16)dport;
    hash[3] = keyptr->secret[11];
    seq = half_md4_transform(hash, keyptr->secret) & HASH_MASK;
    seq += keyptr->count;
    /*
     *    As close as possible to RFC 793, which
     *    suggests using a 250 kHz clock.
     *    Further reading shows this assumes 2 Mb/s networks.
     *    For 10 Mb/s Ethernet, a 1 MHz clock is appropriate.
     *    For 10 Gb/s Ethernet, a 1 GHz clock should be ok, but
     *    we also need to limit the resolution so that the u32 seq
     *    overlaps less than one time per MSL (2 minutes).
     *    Choosing a clock of 64 ns period is OK. (period of 274 s)
     */
    seq += ktime_to_ns(ktime_get_real()) >> 6;
    return seq;
}
```
**问题3：****tcp三次握手过程中，三次握手的最后一个过程数据包在网络中丢掉，该如何处理？**
服务器收到SYN包后发出SYN+ACK数据包，服务器进入SYN_RECV状态。而这个时候客户端发送ACK给服务器失败了，服务器没办法进入ESTABLISH状态，这个时候肯定不能传输数据的，不论客户端主动发送数据与否，服务器都会有定时器发送第二步SYN+ACK数据包，如果客户端再次发送ACK成功，建立连接。如果一直不成功，服务器肯定会有超时设置，超时之后会给客户端发RTS报文，进入CLOSED状态，这个时候客户端应该也会关闭连接。
**问题4：TIME_WATE的作用？**
等待2MSL时间，在到CLOSED状态。可靠地终止TCP连接、保证让迟来的TCP报文段有足够的时间被识别并丢弃。
**问题5：保证tcp可靠传输的方法？**
确认机制、滑动窗口、合理分片和排序、数据校验和、流控、拥塞避免等方式。具体的算法和实现较多，在实践过程中暂未遇到需要实现和利用这部分的知识点，最多做了滑动窗口的实现的问题。
## 0x03 总结
虽然这些问题，或多或少了解一点，但是了解的不够深入不全面。


