# UDP数据段格式 - 深之JohnChen的专栏 - CSDN博客

2007年08月01日 13:31:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3921


　　UDP是一种不可靠的、无连接的数据报服务。源主机在传送数据前不需要和目标主机建立连接。数据被冠以源、目标端口号等UDP报头字段后直接发往目的主机。这时，每个数据段的可靠性依靠上层协议来保证。在传送数据较少、较小的情况下，UDP比TCP更加高效。

　　如图2-6所示，是UDP头部结构（RFC 793、1323）：
![](http://tk.files.storage.msn.com/x1pnp_rgmi5o53-DbTRc2xctX-C3HuMNBMVP1pRExFS1dgrjHGnI3XP3qBXgiSyVfRpYxJy6hSgsr6i1GI1r65eLlV9g_pfeM-pRpQMwJO4DiX4l-OddOnJQ4rnNMyaTu_OyoZnq--sfG8)
　　 图2-6　 UDP数据段格式

　　●源、目标端口号字段：占16比特。作用与TCP数据段中的端口号字段相同，用来标识源端和目标端的应用进程。

　　●长度字段：占16比特。标明UDP头部和UDP数据的总长度字节。

　　●校验和字段：占16比特。用来对UDP头部和UDP数据进行校验。和TCP不同的是，对UDP来说，此字段是可选项，而TCP数据段中的校验和字段是必须有的。

　套接字

　　在每个TCP、UDP数据段中都包含源端口和目标端口字段。有时，我们把一个IP地址和一个端口号合称为一个套接字（Socket），而一个套接字对（Socket pair）可以唯一地确定互连网络中每个TCP连接的双方（客户IP地址、客户端口号、服务器IP地址、服务器端口号）。

　　如图2-7所示，是常见的一些协议和它们对应的服务端口号。
![](http://tk.files.storage.msn.com/x1pnp_rgmi5o53-DbTRc2xctfc6I7HWClGfeBY1u5t31HOXTCSYDJMcdEvzqFge-MB4OoPB5aOGQOOa-MU_jn8zmd8Mv41vXIwo8EKfiuBpTIWlqgJTlCvVauOZ3NeA7kEPeV7XA-suDi0)
　　 图2-7　 常见协议和对应的端口号 

　　需要注意的是，不同的应用层协议可能基于不同的传输层协议，如FTP、TELNET、SMTP协议基于可靠的TCP协议。TFTP、SNMP、RIP基于不可靠的UDP协议。

　　同时，有些应用层协议占用了两个不同的端口号，如FTP的20、21端口，SNMP的161、162端口。这些应用层协议在不同的端口提供不同的功能。如FTP的21端口用来侦听用户的连接请求，而20端口用来传送用户的文件数据。再如，SNMP的161端口用于SNMP管理进程获取SNMP代理的数据，而162端口用于SNMP代理主动向SNMP管理进程发送数据。

　　还有一些协议使用了传输层的不同协议提供的服务。如DNS协议同时使用了TCP 53端口和UDP 53端口。DNS协议在UDP的53端口提供域名解析服务，在TCP的53端口提供DNS区域文件传输服务。            

