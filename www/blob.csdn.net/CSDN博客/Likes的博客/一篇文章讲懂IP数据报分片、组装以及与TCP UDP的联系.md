# 一篇文章讲懂IP数据报分片、组装以及与TCP/UDP的联系 - Likes的博客 - CSDN博客
2018年10月31日 21:40:41[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：390
[http://www.mamicode.com/info-detail-1465195.html](http://www.mamicode.com/info-detail-1465195.html)
[https://blog.csdn.net/leex_brave/article/details/51971053](https://blog.csdn.net/leex_brave/article/details/51971053)
[https://blog.csdn.net/buptzwp/article/details/5055487](https://blog.csdn.net/buptzwp/article/details/5055487)
# **一、术语：**
**![](https://img-blog.csdnimg.cn/20181031211115861.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NvbmdjaHV3YW5nMTg2OA==,size_16,color_FFFFFF,t_70)**
**a、MTU： **
        当两台远程PC互联的时候，它们的数据需要穿过很多的路由器和各种各样的网络媒介才能到达对端，网络中不同媒介的MTU各不相同，就好比一长段的水管，由不同粗细的水管组成(MTU不同 )通过这段水管最大水量就要由中间最细的水管决定。网络层IP协议会检查每个从上层协议下来的数据包的大小，并根据本机MTU的大小决定是否作“分片”处理。
**b、DF：**
        DF(Donot Fragment)。这是一个标志位，指明了不能进行IP数据包的分片。当这个IP数据包的DF设定为1，在一大段网络(水管里面)传输的时候，如果遇到MTU小于IP数据包的情况，转发设备就会根据要求丢弃这个数据包。然后返回一个错误信息给发送者。
** c、MSS：**
　　MSS就是TCP数据包每次能够传输的最大数据分段。为了达到最佳的传输效能TCP协议在建立连接的时候通常要协商双方的MSS值，这个值TCP协议在实现的时候往往用MTU值代替(需要减去IP数据包包头的大小20Bytes和TCP数据段的包头20Bytes)所以往往MSS为1460。通讯双方会根据双方提供的MSS值得最小值确定为这次连接的最大MSS值。
# 二、分片：
        IP报文在传输过程中，有些时候“水管”会越来越窄，也就是MTU会越来越小。IP报文也会一分再分。每个IP分片的报头基本相同，只是片偏移不一样
# 三、组装：
         IP报文在传输过程中可能会分成多个片，在整个传输过程中是不会组装的（路由器也没有这个能力组装），IP报文分片的组装统一在对端主机上进行。对端主机在接收到IP数据报分片后会对其进行排序，形成一个完整的IP数据报之后上传给传输层。但IP数据报之间是不保证有序的。
# 四、IP分片与TCP/UDP的联系：
**TCP：**
TCP三次握手时会商量好MSS值，其实也是根据下层的MTU值确定的。MSS指定了TCP数据包的大小，TCP流数据会按照MSS大小进行“打包”，打包后加上IP报头后就是一个IP报文。IP报文在传输过程可能会分片。IP报文分片到对端后会在IP层进行组装，组成一个完整的IP报文（也是一个TCP数据包）后上传到传输层。由于IP层不确保IP报文之间有序，TCP得到的数据包也是无序的，但是TCP协议会对其进行排序，进而提交给上层用户。如果因为某个IP报文分片丢失而导致IP层无法拼装成一个完成的IP报文，就是接向传输层报告，也就是TCP的“丢包”，TCP进行重传。
**UDP：**
UDP将整个报文直接传给IP层，IP层加上IP报头后就是一个IP报文，如果UDP报文很大，IP报文也会很大。当IP层发现MTU很小时，对IP报文进行切分。对端主机在IP层将IP分片组装成完整的IP报文后上传。在拼装IP报文分片时如果少了某些分片，整个UDP报文就废了。所以不应该将UDP报文的大小设定过大，越大，在传输过程中分片就会越多，丢失的可能性越大，发送成功的可能性也就越低。
# 五、总结：
1、IP报文在传输过程中可能会多次分片
2、IP报文分片再传输过程中只会越分越小，不会组装，组装统一在对端主机的IP层进行
3、IP层不保证IP报文之间有序。但是对端主机在IP层会拼装IP报文分片，形成一个完整的IP报文后上传。拼装过程其实就是对IP报文分片进行收集、排序的过程。
