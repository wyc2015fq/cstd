# RTMPdump 使用说明 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月06日 16:24:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：28









RTMPDump v2.42012-07-24RTMPDUMP(1)

**名称**
 rtmpdump - (RTMP流媒体客户端)RTMP streaming media client

**使用参数**
 rtmpdump -r url [-n hostname] [-c port] [-l protocol] [-S host:port] [-a app] [-t tcUrl] [-p pageUrl] [-s swfUrl] [-f flashVer] [-u auth] [-C conndata] [-y playpath] [-Y] [-v] [-R] [-d subscription] [-e] [-k skip] [-A start] [-B stop] [-b buffer] [-m timeout] [-T key] [-j JSON] [-w swfHash] [-x swfSize] [-W swfUrl] [-X swfAge] [-o output] [-#] [-q] [-V] [-z]
 rtmpdump -h

**说明**
 rtmpdump 是一个可以通过RTMP协议下载流媒体的工具.
 rtmpdump 通过给定的URL建立连接到指定的RTMP服务器. 这URL应该如下：
 rtmp[t][e]://hostname[:port][/app[/playpath]]
 支持一般以及加密的RTMP会话

**选项 网络参数**
 这些选项定义了如何连接到服务器


 --rtmp-r url
 连接到服务器的url
 --host-n hostname
 覆盖 RTMP URL的主机地址.
 --port-c port
 覆盖 RTMP URL的端口号.
 --protocol-l number
 覆盖 RTMP URL的协议.
 0 = rtmp
 1 = rtmpt
 2 = rtmpe
 3 = rtmpte
 4 = rtmps
 5 = rtmpts
 --socks-S host:port
 使用指定 SOCKS4 代理.


**连接参数**
 这些选项定义了RTMP请求连接的数据包内容，如果不提供正确的值，服务器将拒绝连接尝试

 --app-a app
 连接到RTMP的应用名，覆盖RTMP URL中的app。有时rtmpdump URL 无法正确自动解析app名称。这时必须使用该选项。
 --tcUrl-t url
 目标流的URL。默认格式 rtmp[e]://host[:port]/app/playpath.
 --pageUrl-p url
 媒体嵌入网页的URL，默认情况下没有被发送的价值
 --swfUrl-s url
 该媒体swf播放器的URL. 默认情况下没有被发送的价值.
 --flashVer-f version
 swf播放器使用的Flash版本. 默认是"LNX 10,0,32,18".
 --auth-u string
 增加到connect的验证字符串，使用此选项将增加一个bool值TRUE，然后是指定的字符串，此选项仅用于一些特定的服务器，已经过时了，更一般情况，--conn选项应该被代替使用
 --conn-C type:data
 任意AMF数据追加到connect，类型说明，
B 布尔型
N 数字
S 字符串
O 对象
Z 空
 对于布尔型必须是0或1作为FALSE或TRUE，
 对于对象数据必须以0或1分别作为结束和开始的标制，子对象中数据项前加前缀类型N，并指定值名称，例如：
 -C B:1 -C S:authMe -C O:1 -C NN:code:1.23 -C NS:flag:ok -C O:0

**会话选项**
 这些选项在连接成功后生效

 --playpath-y path
 覆盖RTMP URL解析的playpath，rtmpdump有时不能正确解析，通过该选项明确
 --playlist -Y
 在play命令之前发生set_playlist命令。否则播放列表将会值包含playpath
 --live -v
 指定媒体是实时流。在实时流中没有恢复和搜索。
 --subscribe-d stream
 订阅的实时流名称。默认 playpath.
 --realtime -R
 实时下载时试图使用暂停和恢复命令("the BUFX hack")，服务器可以调到后面再使用恢复命令，恢复和搜索在实时流中还是可以实现的
 --resume -e
 恢复一个不完整的rtmp流下载
 --skip-k num
 恢复时寻找最后一个关键帧跳过的关键帧数。如果经常尝试恢复失败，这可能有用，默认0。
 --start-A num
 开始到流的秒数，实时流无效
 --stop-B num
 该流停止秒数
 --buffer-b num
 设置缓冲时间，单位毫秒. The default is 36000000.
 --timeout-m num
 num秒后没有收到任何数据会话超时，默认值120。

**安全参数**
 这些选项处理额外的身份验证，来自服务器的请求
 --token-T key
 输入安全令牌响应，如果服务器需要使用安全令牌验证
 --jtv-j JSON
 JSON令牌用于传统Justin.tv服务器 ，调用NetStream.Authenticate.UsherToken
 --swfhash-w hexstring
 swf文件的SHA256 hash 数值. 如果服务器使用swf验证，该选项会被用到, 看下面 --swfVfy 选项. 该哈希值是32字节，并且必须以十六进制表示. 始终与 --swfsize 选项一起使用.
 --swfsize-x num
 解压后SWF文件大小. 服务器swf验证时可能需要此选项，看下面--swfVfy 选项.始终与 --swfhash 选项一起使用.
 --swfVfy-W url
 swf播放器的URL，此选项将替换所以三个--swfUrl, --swfhash, and --swfsize选项。使用此选项时，swf播放器将从指定URL检索，并自动计算哈希和大小。此外信息缓存在一个swfinfo文件在用户主目录，所以它在每次rtmpdump运行时，并不需要检索和重新计算。swfinfo记录URL，生成时间，修改SWF文件时间，它的大小，它的哈希，默认情况下，缓冲信息用于30天，然后重新检测。
 --swfAge-X days
 指定使用缓存的swf信息天数，然后重新检查，使用0为经常检查，如果检查显示swf具有相同的修改时间戳，它不会被再次检索

**杂项**
 --flv-o output
 指定输出文件名，如果这个名字是-或被省略，流写入到stdout
 --hashes -#
 显示流进度，一个哈希标记没1%进度，而不是字节计数器
 --quiet -q
 制止所以命令的输出
 --verbose -V
 详细输出.
 --debug -z
 debug输出等级，最详细可输出所有分组数据
 --help -h
 打印命令选项摘要

**退出状态**
 0
 程序成功运行
 1
 不可知错误
 2
 不完全转移，可能会得到进一步恢复


 作者
 Andrej Stepanchuk, Howard Chu, The Flvstreamer Team

<http://rtmpdump.mplayerhq.hu>



英文原文：[http://rtmpdump.mplayerhq.hu/rtmpdump.1.html](http://rtmpdump.mplayerhq.hu/rtmpdump.1.html)



中文原文：[http://blog.163.com/zhujiatc@126/blog/static/18346382013713114012167/](http://blog.163.com/zhujiatc@126/blog/static/18346382013713114012167/)



RTMPdump Windows版本下载：[http://download.csdn.net/detail/leixiaohua1020/6512579](http://download.csdn.net/detail/leixiaohua1020/6512579)




