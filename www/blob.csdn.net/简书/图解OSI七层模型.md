# 图解OSI七层模型 - 简书


# 图解OSI七层模型



七层模型，亦称OSI（Open System Interconnection）参考模型，是参考模型是国际标准化组织（ISO）制定的一个用于计算机或通信系统间互联的标准体系。它是一个七层的、抽象的模型体，不仅包括一系列抽象的术语或概念，也包括具体的协议。

七层模型从上到下依次是：
- **应用层**：协议有：HTTP FTP TFTP SMTP SNMP DNS TELNET HTTPS POP3 DHCP
- **表示层**：数据的表示、安全、压缩。格式有，JPEG、ASCll、DECOIC、加密格式等
- **会话层**：建立、管理、终止会话。对应主机进程，指本地主机与远程主机正在进行的会话
- **传输层**：定义传输数据的协议端口号，以及流控和差错校验。协议有：TCP UDP，数据包一旦离开网卡即进入网络传输层
- **网络层**：进行逻辑地址寻址，实现不同网络之间的路径选择。协议有：ICMP IGMP IP（IPV4 IPV6） ARP RARP
- **数据链路层**：建立逻辑连接、进行硬件地址寻址、差错校验等功能。将比特组合成字节进而组合成帧，用MAC地址访问介质，错误发现但不能纠正。
- **物理层**：建立、维护、断开物理连接。

## 七层模型图示





![](https://upload-images.jianshu.io/upload_images/7541336-b87f3c6f5235b56d.png)

七层模型1.png






![](https://upload-images.jianshu.io/upload_images/7541336-8dfb37470d86e251.png)

七层模型2.png


**七层模型传输数据过程：**






![](https://upload-images.jianshu.io/upload_images/7541336-906a34d0af992f70.png)

七层模型传输数据.png









