# Sendip 命令行发包工具，支持IP、TCP、UDP等 - xcw0754 - 博客园
# [Sendip 命令行发包工具，支持IP、TCP、UDP等](https://www.cnblogs.com/xcw0754/p/8449779.html)
[Sendip](https://github.com/rickettm/SendIP)是一个linux平台的命令行发数据包工具，目前(2018年2月)支持的协议有**ipv4、ipv6、icmp、tcp、udp、bgp、rip、ntp**，作者表示其他协议将会后面支持，当他有空写的时候。**Sendip**很强大，它支持自定义头部和数据(也就是IP层以上的整个包)，没有过多的限制，所以连源IP都可以随意写，而且里面也提供了一些默认的选项，可以择需而发，非常方便。又因为它是命令行的，还支持从文件中直接读取整个packet发送，所以适合用脚本批量测试。它只是个发包工具，对于发出去的包，收到了什么样的回复，Sendip是不知道的，所以要得到回复的包就只能用抓包工具了。它的源码也很简单，没有过多平台相关的代码，如果不理解也可以看源码。
其实这个工具已经满足了我的要求，之前下载的`PackETH`是GUI版的，连数据链路层(link layer)都支持，也要求必须提供这一层的相关信息才能发包，这一层一般不太关心，它又不提供自动封装，总的来说不太适合用。
### Sendip简单使用
常见协议报头可参考[常见协议TCP、UDP、IP图](http://www.cnblogs.com/xcw0754/p/8449341.html)
通用选项：
```
-d    要携带的数据。rN随机产生N个字节，0x之后带十六进制，0之后带8进制。
-f    从文件中读取要携带的数据。
-p    加载协议模块，只有加载了才能使用。
-v    打印整个发出的包。
```
ipv4模块：
```
-iv x      版本               Default: 4
-ih x      首部长度       Default: Correct
-iy x      区分服务       Default: 0
-il x      总长度           Default: Correct
----------------------------------------------32bit
-ii x      标识               Default: Random
-ifr x     标志               Default: 0 (options are 0,1,r)
-if x      片偏移           Default: 0
----------------------------------------------32bit
-it x      生存时间       Default: 255
-ip x      协议             Default: 0, or set by underlying protocol
-ic x      首部检验和   Default: Correct
----------------------------------------------32bit
-is x      源地址          Default: 127.0.0.1
----------------------------------------------32bit
-id x      目的地址      Default: Correct
----------------------------------------------32bit
下面全是可选字段(比较少用，不译)：
-ifd x IP don't fragment flag (see README)
       Default: 0 (options are 0,1,r)
-ifm x IP more fragments flag (see README)
       Default: 0 (options are 0,1,r)
-ionum x
       IP option as string of hex bytes (length is always correct)
       Default: (no options)
-ioeol IP option: end of list
-ionop IP option: no-op
-iorr x
       IP option: record route. Format: pointer:addr1:addr2:...
-iots x
       IP option: timestamp. Format: pointer:overflow:flag:(ip1:)ts1:(ip2:)ts2:...
-iolsr x
       IP option: loose source route. Format: pointer:addr1:addr2:...
-iosid x
       IP option: stream identifier
-iossr x
       IP option: strict source route. Format: pointer:addr1:addr2:...
```
tcp模块：
```
-ts x  源端口       Default: 0
-td x  目的端口       Default: 0
----------------------------------------------32bit
-tn x  序号       Default: Random
----------------------------------------------32bit
-ta x  确认号       Default: 0
----------------------------------------------32bit
-tt x  数据偏移       Default: Correct
-tr x  保留(ECN、CWR看rfc2481)       Default: 0
-tfu x URG       Default: 0, or 1 if -tu specified (options are 0,1,r)
-tfa x ACK       Default: 0, or 1 if -ta specified (options are 0,1,r)
-tfp x PSH       Default: 0 (options are 0,1,r)
-tfr x RST       Default: 0 (options are 0,1,r)
-tfs x SYN       Default: 1 (options are 0,1,r)
-tff x FIN       Default: 0 (options are 0,1,r)
-tw x  窗口       Default: 65535
----------------------------------------------32bit
-tc x  检验和       Default: Correct
-tu x  紧急指针       Default: 0
----------------------------------------------32bit
下面全是可选字段(比较少用，不译)：
-tonum x       TCP option as string of hex bytes (length is always correct)
       Default: (no options)
-toeol TCP option: end of list
-tonop TCP option: no op
-tomss x
       TCP option: maximum segment size
-towscale x
       TCP option: window scale (rfc1323)
-tosackok
       TCP option: allow selective ack (rfc2018)
-tosack x
       TCP option: selective ack (rfc2018), format is l_edge1:r_edge1,l_edge2:r_edge2...
-tots x
       TCP option: timestamp (rfc1323), format is tsval:tsecr
```
udp模块：
```
-us x  源端口    Default: 0
-ud x  目的端口    Default: 0
-ul x 长度    Default: Correct
-uc x  检验和    Default: Correct
```
要注意，按照从左到右的顺序依次封装报文，所以ip报文必须写在其他报文之前。如果协议中需要**检验和**之类的就按默认的就行了，省去计算的痛苦。下面有几个例子：
- `sendip -v  -p ipv4  -id 14.215.177.39  -p udp  -f test www.baidu.com`
- `sendip -p ipv4 -is 192.168.1.2 -id 192.168.1.1 -p icmp -d 0x89ABCDEF www.google.com`
其中主要的结构是`sendip 网络层 上一层 数据 domain`，domain是目的主机，可以是`www.baidu.com`或`192.168.1.1`之类的。如果出现什么错误就会打印出帮助信息，里面有一行是提示错误原因，别漏看了。至于能不能发不规则的包(如数据与报文长度不符合、校验和乱写之类的)，实际会不会发出去就没进行测试了。

