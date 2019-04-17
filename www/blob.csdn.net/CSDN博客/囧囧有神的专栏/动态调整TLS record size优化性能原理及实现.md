# 动态调整TLS record size优化性能原理及实现 - 囧囧有神的专栏 - CSDN博客





2017年03月24日 16:33:45[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：1420








在ssl连接的不同阶段，通过动态调整TLS record size来优化性能，这种思路其实已经出来几年了（至少13年就有相关文章）。在google大神 ILYA GRIGORIK的书<High Performance Browser Networking> 中，可以看到有相关描述。相应的实现也不少，例如go1.7中增加了动态调整tls record size， 详情可见： [https://github.com/golang/go/commit/ae00df128d8434dd37f73123aaea09de786d0275](https://github.com/golang/go/commit/ae00df128d8434dd37f73123aaea09de786d0275)

nginx本身支持tls record size配置，但不支持动态调整，cloudflare贡献了一个patch以支持动态调整，相关代码见[https://github.com/cloudflare/sslconfig/blob/master/patches/nginx__dynamic_tls_records.patch](https://github.com/cloudflare/sslconfig/blob/master/patches/nginx__dynamic_tls_records.patch)

从公开的资料中，可以看到国内淘宝对tls record size有调整，但不知道具体细节。




# 原理简述

网络协议是一种分层的协议，如下图所示。每一层都不需要关注下一层是如何实现的。如果某一层性能不好，所有上层协议都会受到影响。

![](https://img-blog.csdn.net/20170324163035033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1aml5b25nNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)











TLS协议又由两层协议构成 ：TLS记录协议 (TISRecord)以及 TLS握手协议 ，较低的一层 为 TLS Record协议。

所有通过 TLS 交付的应用数据都会根据TLS Record协议传输。每条记录大小叫TLS Record size, 上限 为 16 KB。Size值如果过小，头部负载比重就会过大，最高可达6%。Size值如果过大，那单个Record在TCP层会被分成多个包发送。浏览器必须等待这些全部达到后，才能解密，一旦出现丢包、拥塞、重传、甚至重新建立的情况，时延就会被相应增加。




**如何设置TLS Record size大小呢？**

对于性能，我们关心两个指标，时延以及吞吐率

tls record size如果设置的较大，单个record会在TCP层拆成多个包发送， 那么时延就会增大，吞吐率却会增加（record头部占比较小）

tls record size如果设置的较小，单个的record在TCP层存在于一个包中，时延就会比较小，吞吐率却会下降（record头部占比较大）




如果关心时延，那么是不是record size越小越好呢？ 也不是。同样一个tcp包，包的长度并不会影响传输时间，都是一个RTT。但是record size越小，record头部越大，会导致吞吐率下降。

**所以在时延敏感的情况下，record size的大小建议是MTU - record头部 - IP/TCP头部**




在吞吐率敏感的情况下，是record size越大，record头部的占比越小，吞吐率会越高。

**所以在吞吐率敏感的情况下，record size的大小建议设置成最大值16kb**







在连接的不同时机，对性能的要求也不一样。用户发起访问一个https网页，需要尽快看到网页，就会对时延比较敏感。在 TCP 连接刚开始时，会产生 TCP 慢启动（Slow Start），这使得被划分为多个 TCP 段的 TLS 记录只能缓慢交付。在整个 TCP 连接中，组成 TLS 记录的任何一个 TCP 段的丢失，都会导致整个记录被延迟，直到丢失的段被重传。

**因此，更好的做法是不要使用固定的 TLS 记录大小，而是根据 TCP 连接速度的提升（或者收到拥塞控制而下降）动态调整记录大小。在每个连接刚开始时，使用较小的记录大小，使之能与 TCP 能够发送的大小相匹配。一旦连接运行起来，就可以增加记录大小。**







# 具体实现

## golang1.7的实现方式




算法规则

1. 对于一个新连接，采用一个较小的tls record size，    

2. 发送完1MB数据后，转为一个较大的tls record size  16kb







较小的值的计算方式：



```
payloadBytes := tcpMSSEstimate - recordHeaderLen - explicitIVLen
 	if c.out.cipher != nil {
 		switch ciph := c.out.cipher.(type) {
 		case cipher.Stream:
 			payloadBytes -= macSize
 		case cipher.AEAD:
 			payloadBytes -= ciph.Overhead()
 		case cbcMode:
 			blockSize := ciph.BlockSize()
 			// The payload must fit in a multiple of blockSize, with
 			// room for at least one padding byte.
 			payloadBytes = (payloadBytes & ^(blockSize - 1)) - 1
 			// The MAC is appended before padding so affects the
 			// payload size directly.
 			payloadBytes -= macSize
 		default:
 			panic("unknown cipher type")
 		}
 	}
```



tcpMSSEstimate值为1208，为ipv6的最小MTU 1280，减去ipv6的头部大小 40 和tcp头部大小 32 得到的。

然后在计算中，再减去相应的tls头部长度，就得到一个record能够承载的数据大小




## nginx的实现方式




算法规则

1. 对于1个新连接，采用一个较小的tls record size。（默认1369，通过配置ssl_dyn_rec_size_lo更改）

2. 发送一定数量（默认40，ssl_dyn_rec_threshold配置更改）的record后，开始采用较大的tls record size（默认4229，ssl_dyn_rec_size_hi更改）。

3. 再次发送一定数量（默认40，ssl_dyn_rec_threshold配置更改）的record后，采用最大tls record size（默认16kb，ssl_buffer_size配置）

4. 在连接是idle状态且持续了一段时间（默认1s，ssl_dyn_rec_timeout更改），会重新开始发送小包（ssl_dyn_rec_timeout设置为0，会禁用这个逻辑），从步骤1开始。







## 比较两种方式

nginx比golang实现的算法更复杂些，也提供了配置参数进行调整。

哪种方式更好，还是得看具体业务场景，需要更多的测试。










[https://www.igvita.com/2013/10/24/optimizing-tls-record-size-and-buffering-latency/](https://www.igvita.com/2013/10/24/optimizing-tls-record-size-and-buffering-latency/)

[https://blog.wilddog.com/?p=210](https://blog.wilddog.com/?p=210)

[https://blog.cloudflare.com/optimizing-tls-over-tcp-to-reduce-latency/](https://blog.cloudflare.com/optimizing-tls-over-tcp-to-reduce-latency/)

[https://hpbn.co/transport-layer-security-tls/#tls-record-protocol](https://hpbn.co/transport-layer-security-tls/#tls-record-protocol)

[http://velocity.oreilly.com.cn/2015/ppts/lizhenyu.pdf](http://velocity.oreilly.com.cn/2015/ppts/lizhenyu.pdf)



