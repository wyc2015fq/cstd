
# 加速scp传输速度 - 高科的专栏 - CSDN博客

2015年03月13日 16:26:06[高科](https://me.csdn.net/pbymw8iwm)阅读数：4209


当需要在机器之间传输400GB文件的时候，你就会非常在意传输的速度了。默认情况下(约125MB带宽，网络延迟17ms，Intel E5-2430，本文后续讨论默认是指该环境)，scp的速度约为40MB，传输400GB则需要170分钟，约3小时，如果可以加速，则可以大大节约工程师的时间，让攻城师们有更多时间去看个电影，陪陪家人。
目录
1. 结论
2. 测试数据：加密算法和压缩的影响
3. 关于是否启用压缩
4. "压缩级别"对传输速度影响不大
5. 测试数据：完整性校验算法MACs选择
6. 参考阅读

#### 1. 结论
**声明：这里给出的测试数据不具有一般性，仅供参考。测试与数据本身特性有很大关系，本文使用InnoDB的redo log作为测试数据。**
* 改变ssh加密算法，可以让速度更快；**通常**，越弱的加密算法，速度越快
***通常压缩会降低scp速度**，但这与数据类型有很大关系，对压缩率非常高的数据启用压缩，可以加速
* 压缩级别对传输效率影响很小
* 用于完整性校验的不同[MAC( message authentication code)](http://en.wikipedia.org/wiki/Message_authentication_code)算法，对性能约有10%-20%的影响。
所以，简单尝试如下，让你的SCP速度double一下：
scp -r -c arcfour128 ...
scp -r -c aes192-cbc ...
scp -r -c arcfour128 -o "MACs umac-64@openssh.com" ...
注：启用压缩使用参数： -o "Compression yes"
#### 2. 测试数据：加密算法和压缩的影响
这里对比了12种ssh中实现的加密算法和是否使用压缩的传输效率，测试文件使用的是InnoDB的1GB*4的日志文件(注意：不同类型的文件测试结果会很不同)，这里纵坐标单位为MB/s，数据分为压缩传输和不压缩传输两组：
![screen-scp-compare-cipher-compression](http://www.linuxidc.com/upload/2014_01/140120193723603.jpg)
原始数据：[scp_speed.txt](http://www.orczhou.com/docs/scp_speed.txt)
可以看到，不同加密算法传输速度相差很大；使用了压缩之后，速度下降很多，也看到不同加密算法加密后区别并不大。
#### 3. 关于是否启用压缩
* 压缩只有在网络传输速度非常慢，以致于压缩后节省的传输时间大于压缩本身的时间，这时才有效果，所以是否启用压缩，需要实际测试
* 压缩比很低的数据，不要再启用压缩(例如已经压缩过的数据、视频等)
* 通常建议，传输前先压缩，而不是使用ssh的压缩；建议使用pigz/lbizp2等并行压缩工具
* 数据中大量重复、空洞，这类适合压缩的数据，可以尝试压缩选项，例如如下是一组，大量"空洞"数据的测试：
![chart_1](http://www.linuxidc.com/upload/2014_01/140120193723602.jpg)
看到，压缩大大提高了传输效率
#### 4. "压缩级别"对传输速度影响不大
最后一组对比是，将压缩级别从1改到9，对比传输速度，纵坐标单位MB/s，对12种加密算法分别使用了测试9个压缩级别，数据如下：
![screen-scp-compare-compression-level](http://www.linuxidc.com/upload/2014_01/140120193723601.jpg)
原始数据:
MB/s            with-compression  without-compressoin
3des-cbc        17.525            13.5
aes256-ctr      20.325            30.2
aes192-ctr      20.275            35.1
aes128-ctr      20.275            38.5
cast128-cbc     20.825            38.9
blowfish-cbc    20.8              43.1
arcfour         21.975            74.2
arcfour128      21.725            75
arcfour256      22.025            75.8
aes128-cbc      21.6              75.8
aes256-cbc      21.325            80.1
aes192-cbc      21.725            85.2可以看到，压缩级别对传输影响较小。ssh使用的默认压缩级别是6。
#### 5. 测试数据：完整性校验算法MACs选择
通过选项Macs可以设置对应的哈希算法，man ssh_config可以看到支持哪些哈希算法。这里对了比了12中加密算法下使用不用的完整性校验算法的性能情况：
![screen-scp-compare-macs-all](http://www.linuxidc.com/upload/2014_01/140120193723604.jpg)
看到，绝大数情况下"umac-64@openssh.com"([关于此哈希](http://www.openssh.com/txt/draft-miller-secsh-umac-01.txt))性能都更好，所以建议尝试使用此哈希算法做验证，看看你的场景下速度是否与提升。也可以看到，默认的hmac-md5哈希在默认的加密aes128-ctr下表现比较好；

