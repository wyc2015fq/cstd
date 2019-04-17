# 服务器tcp连接timewait过多优化及详细分析 - DoubleLi - 博客园






【背景说明】

在7层负载均衡上，查询网络状态发现timewait太多，于是开始准备优化事宜

整体的拓扑结构，前面是lvs做dr模式的4层负载均衡，后端使用（nginx、or haproxy）做7层负载均衡

【优化效果】

修改前，建立连接的有29个，timewait的就达到了900个，如下图所示

![wKiom1Ty5IfhAB9FAAEI5rK5xCU064.jpg](http://www.2cto.com/uploadfile/Collfiles/20150309/20150309093505470.jpg)

修改后，建立连接的有32个，timewait的从900降低到了49个，如下图所示

![wKioL1Ty5aOCeWn5AAEKHpcJW_c047.jpg](http://www.2cto.com/uploadfile/Collfiles/20150309/20150309093505471.jpg)



【具体优化方案】

注意：前端使用nat时，不适用本策略。详细“方案详细介绍”会说明

修改7层负载所在机器，/etc/sysctl.conf

net.ipv4.tcp_tw_reuse = 1

net.ipv4.tcp_tw_recycle = 1

net.ipv4.tcp_timestamps = 1

net.ipv4.tcp_fin_timeout = 20

保存后sysctl -p生效



【方案详细介绍】

```python
net.ipv4.tcp_tw_reuse=1
```



#表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；该文件表示是否允许重新应用处于TIME-WAIT状态的socket用于新的TCP连接(这个对[快速重启](https://www.baidu.com/s?wd=%E5%BF%AB%E9%80%9F%E9%87%8D%E5%90%AF&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)动某些服务,而启动后提示端口已经被使用的情形非常有帮助)



```python
net.ipv4.tcp_tw_recycle=1
```



#表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。



net.ipv4.tcp_timestamps 开启时，net.ipv4.tcp_tw_recycle开启才能生效,原因可以参考以下代码
- 



if(tcp_death_row.sysctl_tw_recycle&&tp->rx_opt.ts_recent_stamp)recycle_ok=icsk->icsk_af_ops->remember_stamp(sk);


- 



if(recycle_ok){


- 



tw->tw_timeout=rto;


- 



}


- 



else{tw->tw_timeout=TCP_TIMEWAIT_LEN;


- 



if(state==TCP_TIME_WAIT)


- 



timeo=TCP_TIMEWAIT_LEN;


- 



}





如果[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)身处NAT环境，安全起见，通常要禁止tcp_tw_recycle，如果nat下，开启了tcp_tw_recycle，可能会导致部分用户无法连接服务器的情况：在nat模式下（服务器一般会用到dnat，用户一般会用到snat），nat设备（or服务器）会修改目的ip和源ip，以屏蔽内部信息。试想很多用户snat出来，通过dnat访问网站，在dnat这层，时而会产生时间戳错乱的问题，那么基于tcp的时间戳的tcp_tw_recycle，就会出错。具体可参考

fc1323的扩展的说明
RFC1323TCPExtensionsforHighPerformanceMay1992


discardedwhenaconnectionisclosed.

AnadditionalmechanismcouldbeaddedtotheTCP,aper-host
cacheofthelasttimestampreceivedfromanyconnection.
ThisvaluecouldthenbeusedinthePAWSmechanismtoreject
oldduplicatesegmentsfromearlierincarnationsofthe
connection,ifthetimestampclockcanbeguaranteedtohave
tickedatleastoncesincetheoldconnectionwasopen.This
wouldrequirethattheTIME-WAITdelayplustheRTTtogether
mustbeatleastonetickofthesender'stimestampclock.
SuchanextensionisnotpartoftheproposalofthisRFC.

Notethatthisisavariantonthemechanismproposedby
Garlick,Rom,andPostel[Garlick77],whichrequiredeach
hosttomaintainconnectionrecordscontainingthehighest
sequencenumbersoneveryconnection.Usingtimestamps
instead,itisonlynecessarytokeeponequantityperremote
host,regardlessofthenumberofsimultaneousconnectionsto
thathost.
大致意思为：tcp会记录每个连接的时间戳，如果后续时间戳比之前记录的时间戳小，就会认为这是错误的连接，拒绝这个连接。如果tcp_tw_recycle开启，那么这种规则就会被激活（那样才能快速回收连接）。所以在lvs使用nat的情况下，用户请求到lvs，LVS会修改地址数据后将请求转发给后端服务器，但不会修改时间戳（因为nat的机制就是只修改源地址和目的地址）。在后端服务器看来，请求的源地址永远都是LVS的地址，并且端口复用，原本不同客户端的请求经过LVS的转发，就可能会被认为是同一个连接，加之不同客户端的时间可能不一致，所以就会出现时间戳错乱的现象，于是后面的数据包就被丢弃了，具体的表现通常是是客户端明明发送的SYN，但服务端就是不响应ACK，还可以通过下面命令来确认数据包不断被丢弃的现象。就会出现部分用户能连接服务器，部分用户不能连接服务器的情况。



但在LVS使用用dr模式情况下，lvs只会修改mac和ip地址的映射关系，后端服务器看到的还是不通的用户ip，所以激活这一规则，也不会有问题。我们这里能使用这个策略，最大的原因也是在这里。

```python
net.ipv4.tcp_timestamps=1
```

#表示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。



net.ipv4.tcp_fin_timeout = 15;这个参数是用来设置保持在FIN_WAIT_2状态的时间。tcp4此挥手，正常的处理流程就是在FIN_WAIT_2情况下接收到FIN进入到TIME_WAIT的情况，tcp_fin_timeout参数对处于TIME_WAIT状态的时间没有任何影响。但是如果这个参数设的比较小，会缩短从FIN_WAIT_2到TIME_WAIT的时间，从而使连接更早地进入TIME_WAIT状态。状态开始的早，等待相同的时间，结束的也早，客观上也加速了TIME_WAIT状态套接字的清理速度。

tcp连接的断开，可参考以下状态机：

![wKiom1Ty5LmjaqrnAAHQ0NimpRk554.jpg](http://www.2cto.com/uploadfile/Collfiles/20150309/20150309093505472.jpg)



【补充说明】

如果变更后运行命令netstat -s|grep timestamp

发现packets rejects in established connections because of timestamp

数值增加的很快，你可能得回滚这个变更了：说明使用snat访问你网站的人很多



因为：虽然服务器端没有使用nat，但是客户端使用snat的情况很多，如果后发现packets rejects in established connections because of timestamp增长很快，建议将这个方案回滚。那时，可使用修改net.ipv4.tcp_max_tw_buckets（[centos](https://www.baidu.com/s?wd=centos&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)默认似乎是 262144）可调整至100000。其实也说明，timeout数量不大的时候，其实可以不用调整tcp_tw_recycle参数（风险很大）。

![wKiom1Ty5J6j_VO0AACyl62w9hk303.jpg](http://www.2cto.com/uploadfile/Collfiles/20150309/20150309093505473.jpg)



【总结】

一个小小的变更，背后涉及的知识是异常多的，所以需要

1、不能随意找个方案就使用，需要深入理解。就像说这个A药可以治疗B症状，但是本质是A药可以治疗C病因情况下得B症状，需要把病因搞清楚了再吃药。就算侥幸治疗好了，也不能永远都是报这种侥幸心理。

2、对于内核参数调整，需要对每个参数都了解之后再行动，否则可能会有悲剧。

3、变更的时候，需要有一个灰度过程，需要观察一段时间后，再大面积修改。









