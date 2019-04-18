# socks v5 协议解析 - xcw0754 - 博客园
# [socks v5 协议解析](https://www.cnblogs.com/xcw0754/p/8414038.html)
[socks v5](http://www.ietf.org/rfc/rfc1928.txt)是一种用于代理的协议，就是说client用这种协议与server沟通，让server帮忙代访问remote后再将结果通过此协议返给client，所以一般是涉及到3个端，分别是client客户端、server代理端、remote请求目的地。通常因为client无法直接访问remote，有可能是由于防火墙限制，所以需要经过一个中间的server代理来访问remote。著名的**shawsocks**(client)简称ss就是基于这种协议的，工作于传输层之上，传输层可基于TCP或UDP。原理是这样的，ss和浏览器(可以是其他软件)运行在同一PC上，暂合称为client端，浏览器发出的数据全部转发给ss，然后由ss转发给server，server再进行转发，所以表现出来是这样的:`browser ==> shawdowsocks ==> server ==> remote`，数据返回时按原路返回。需要注意的是，server不会对应用层协议进行拆分，而是只进行转发，所以remote收到的数据中仍包含client端的地址、浏览器信息等等。
![](https://images2017.cnblogs.com/blog/641737/201802/641737-20180204214632201-1717947374.png)
本文所列的表格通常长这样的：
```
┌────────┬────────┬────────┐
│ field1 │ field2 │ field3 │
├────────┼────────┼────────┤
│      1 │      2 │      3 │
└────────┴────────┴────────┘
```
其中field就是协议头的域。其下面的数字表示该域所占字节数量，**而不是其表示内容**；如果是"variable"，表示可变的，一般在其前面会有个域能指定其长度；如果是0x00这样的十六进制则表示该域内容固定为0x00；。
### 协议内容 (基于TCP)
- **Step 1：client to server**
```
┌─────┬──────────┬─────────┐
│ VER │ NMETHODS │ METHODS │
├─────┼──────────┼─────────┤
│   1 │        1 │ 1~255   │
└─────┴──────────┴─────────┘
```
用途:确立连接认证方法协商，client告知server其所支持的认证方法，server可从中挑选一个。其中
`ver` socks协议的版本号，常用的是socks5版本的，故可以为`0x05`。
`nmethods` 后面的域methods有n个字节
`methods` 每个字节即为一个认证方法，供server挑选
- **Step 2：server to client**
```
┌──────┬──────────┐
│ VER  │  METHOD  │
├──────┼──────────┤
│  1   │    1     │
└──────┴──────────┘
```
用途：告知client需要使用哪种认证方法进行认证。
`ver` 版本，即`0x05`。
`method` 认证方法，从Step 1中的METHODS中选一个。
- **Step 3：client to server**
```
┌──────┬──────┬────────┬───────┬───────────┬──────────┐
│ VER  │ CMD  │  RSV   │ ATYP  │ DST.ADDR  │ DST.PROT │
├──────┼──────┼────────┼───────┼───────────┼──────────┤
│   1  │   1  │ 0x00   │    1  │ Variable  │        2 │
└──────┴──────┴────────┴───────┴───────────┴──────────┘
```
用途：告知server要连接的remote相关信息。
`ver` 版本，即`0x05`
`cmd` 命令，可以是 1)`0x01`CONNECT 2) `0x02`BIND 3) `0x03`UDP ASSOCIATE
`rsv` 保留，就是暂时没用的
`atyp` 指定紧跟的域的地址类型，可以是 1) `0x01` IPv4 2) `0x03` DOMAINNAME 3) `0x04` IPv6
`dst.addr` 地址
`dst.prot` 端口号
注：如果atyp是domainname，那么长度(以字节为单位)得写在`dst.addr`的首个字节里边，否则无法知道域名长度；如果是ipv4就4字节，如果是ipv6就16字节。 cmd的3个命令在最下面讲。
- **Step 4：server to client**
```
┌──────┬──────┬────────┬───────┬───────────┬──────────┐
│ VER  │ REP  │  RSV   │ ATYP  │ BND.ADDR  │ BND.PORT │
├──────┼──────┼────────┼───────┼───────────┼──────────┤
│   1  │   1  │ 0x00   │    1  │ Variable  │        2 │
└──────┴──────┴────────┴───────┴───────────┴──────────┘
```
用途：回复client。
`ver` 版本，即`0x05`
`rep` 回复，可以是下面的一项：
o `0x00` succeeded
o `0x01` general SOCKS server failure
o `0x02` connection not allowed by ruleset
o `0x03` Network unreachable
o `0x04` Host unreachable
o `0x05` Connection refused
o `0x06` TTL expired
o `0x07` Command not supported
o `0x08` Address type not supported
o `0x09` to `0xFF` unassigned
`rsv` 保留。
`atyp` 跟上面讲过的一样，包括接下来的两个域。
### 协议内容 (基于UDP)
- **client to server**
```
┌──────┬───────┬───────┬───────────┬───────────┬──────────┐
│ RSV  │ FRAG  │ ATYP  │ DST.ADDR  │ DST.PORT  │   DATA   │
├──────┼───────┼───────┼───────────┼───────────┼──────────┤
│   2  │    1  │    1  │ Variable  │        2  │ Variable │
└──────┴───────┴───────┴───────────┴───────────┴──────────┘
```
`rsv` 保留。
`frag` 当前数据包的fragment号，这是传输层的分片，要自己实现的，而不是网络层的分片。但可以选择不实现分片，直接把frag非零的数据报丢掉。
`ATYP` 跟上面讲过的一样，包括接下来的两个域。
`data` 数据。
### 细节详解
关于`Step 1`中的METHODS字段有如下可选：
o `0x00` NO AUTHENTICATION REQUIRED（常用）
o `0x01` GSSAPI
o `0x02` USERNAME/PASSWORD（常用）
o `0x03 to 0x7F` IANA ASSIGNED
o `0x80 to 0xFE` RESERVED FOR PRIVATE METHODS
o `0xFF` NO ACCEPTABLE METHODS （常用）
注：0x01必须支持，0x02应该要支持。注意了，认证方法是其他独立的协议，比如[USERNAME/PASSWORD](https://www.rfc-editor.org/pdfrfc/rfc1929.txt.pdf)，这与本协议无关。
关于cmd中的3个选项：
- CONNECT
In the reply to a CONNECT, BND.PORT contains the port number that the server assigned to connect to the target host, while BND.ADDR contains the associated IP address. The supplied BND.ADDR is often different from the IP address that the client uses to reach the SOCKS server, since such servers are often multi-homed. It is expected that the SOCKS server will use DST.ADDR and DST.PORT, and the client-side source address and port in evaluating the CONNECT request.
- BIND
The BIND request is used in protocols which require the client to accept connections from the server. FTP is a well-known example, which uses the primary client-to-server connection for commands and status reports, but may use a server-to-client connection for transferring data on demand (e.g. LS, GET, PUT).
It is expected that the client side of an application protocol will use the BIND request only to establish secondary connections after a primary connection is established using CONNECT. In is expected that a SOCKS server will use DST.ADDR and DST.PORT in evaluating the BIND request.
Two replies are sent from the SOCKS server to the client during a BIND operation. The first is sent after the server creates and binds a new socket. The BND.PORT field contains the port number that the SOCKS server assigned to listen for an incoming connection. The BND.ADDR field contains the associated IP address. The client will typically use these pieces of information to notify (via the rimary or control connection) the application server of the rendezvous address. The second reply occurs only after the anticipated incoming connection succeeds or fails.
- UDP ASSOCIATE
The UDP ASSOCIATE request is used to establish an association within the UDP relay process to handle UDP datagrams. The DST.ADDR and DST.PORT fields contain the address and port that the client expects to use to send UDP datagrams on for the association. The server MAY use this information to limit access to the association. If the client is not in possesion of the information at the time of the UDP ASSOCIATE, the client MUST use a port number and address of all zeros.
A UDP association terminates when the TCP connection that the UDP ASSOCIATE request arrived on terminates.
In the reply to a UDP ASSOCIATE request, the BND.PORT and BND.ADDR fields indicate the port number/address where the client MUST send UDP request messages to be relayed.
### 其他
- [RFC 1928翻译](https://www.cnblogs.com/happyhotty/articles/2181522.html)
- [RFC 1928](http://www.faqs.org/rfcs/rfc1928.html)

