# 被动操作系统扫描软件 p0f 指纹库文件分析 - 爱拍凯特的专栏 - CSDN博客
2016年06月28日 16:59:37[爱拍凯特](https://me.csdn.net/apacat)阅读数：1760
p0f.fp指纹文件分析
==============================================
一段指纹信息：
; ==================
; TCP SYN signatures
; ==================
[tcp:request]
; -----
; Linux
; -----
label = s:unix:Linux:3.11 and newer
sig   = *:64:0:*:mss*20,10:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*20,7:mss,sok,ts,nop,ws:df,id+:0
label = s:unix:Linux:3.1-3.10
sig   = *:64:0:*:mss*10,4:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,5:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*10,7:mss,sok,ts,nop,ws:df,id+:0
; Fun fact: 2.6 with ws=7 seems to be really common for Amazon EC2, while 8 is
; common for Yahoo and Twitter. There seem to be some other (rare) uses, though,
; so not I'm not flagging these signatures in a special way.
label = s:unix:Linux:2.6.x
sig   = *:64:0:*:mss*4,6:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,7:mss,sok,ts,nop,ws:df,id+:0
sig   = *:64:0:*:mss*4,8:mss,sok,ts,nop,ws:df,id+:0
.............
.............
.............
==============================================
分析：
指纹文件以section为组织，划分section的方式是通过签名可以应用到的传输类型，section以方括号作为标志如下
; ==================
; TCP SYN signatures
; ==================
[tcp:request] ==== [模块:方向][module:direction],MTU direction会省略direction
sig = ver:ittl:olen:mss:wsize,scale:olayout:quirks:pclass
例子：
sig   = *:64:0:*:mss*10,4:mss,sok,ts,nop,ws:df,id+:0
ver: 4/6/*
ittl: 大部分的OS取值为：64/128/255，古老版本的windows：32，一些不确定的系统为：60 这个字段一般的格式是：ttl+跳数，p0f探测不出来的情况下赋值"nnn+?",
      少量的会产生随机的TTL，这种情况去一个最大的ttl值，然后在后面加一个 -
olen：IPV4的选项字段或者IPV6的扩展头部的长度，多数情况下为0
mss:  最大分段大小,（TCP选项部分）一般情况下用*代替
wsize：窗口大小
scale：窗口比例因子，（TCP选项部分）一般是固定值或者用*来表示
olayout：逗号分隔布局和排列TCP选项部分
支持的值有：
               eol+n  - explicit（明确的） end of options, followed by n bytes of padding（填充）
               nop    - no-op option （无操作选项）
               mss    - maximum segment size（最大分组大小选项）
               ws     - window scaling（窗口缩放）
               sok    - selective ACK permitted（选择确认允许）
               sack   - selective ACK (should not be seen)（选择确认，（不应该存在））
               ts     - timestamp
               ?n     - unknown option ID n
quirks：逗号分隔IP或TCP头部的特性和观察到的特殊情况
               df     - "don't fragment" set (probably PMTUD); ignored for IPv6（不分片位设置，IPV6中忽略此项）
               id+    - DF set but IPID non-zero; ignored for IPv6 （DF位设置，但是IPID非0，IPV6中忽略此项）
               id-    - DF not set but IPID is zero; ignored for IPv6（DF位不设置，但是IPID=0，IPV6中忽略此项）
               ecn    - explicit congestion notification support （明确的拥塞通知支持）
               0+     - "must be zero" field not zero; ignored for IPv6 (必须非0的字段却设置为0，IPV6中忽略此项)
               flow   - non-zero IPv6 flow ID; ignored for IPv4 （非0的IPV6 流ID，IPV4忽略此项）
               seq-   - sequence number is zero （顺序号为0）
               ack+   - ACK number is non-zero, but ACK flag not set （ack号非0，但是ACK标记没有设置）
               ack-   - ACK number is zero, but ACK flag set （ack号=0，但是ACK标记设置1）
               uptr+  - URG pointer is non-zero, but URG flag not set（URG 非0，但是URG标记没有设置）
               urgf+  - URG flag used （URG标记位置1）
               pushf+ - PUSH flag used （push标记位置1）
               ts1-   - own timestamp specified as zero （自己的时间戳指定为0）
               ts2+   - non-zero peer timestamp on initial SYN （对方第一个syn包的时间戳非0）
               opt+   - trailing non-zero data in options segment （选项部分有非0数据）
               exws   - excessive window scaling factor (> 14) （较大的窗口缩放因子）
               bad    - malformed TCP options （不正常的TCP选项）
pclass：有效载荷大小分类 如果是0 ->'0' 如果非0 ->'+',任意 ->'*'
