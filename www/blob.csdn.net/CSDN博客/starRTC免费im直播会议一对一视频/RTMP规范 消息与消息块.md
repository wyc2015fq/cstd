# RTMP规范 消息与消息块 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年07月30日 10:22:49[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：141

Real Time Messaging Protocol（实时消息传输协议）
应用层协议
RTMP协议中， 基本数据单元称为消息（Message）。当RTMP协议在互联网中传输数据的时候，消息会被拆分成更小的单元，称为消息块（Chunk）
1 消息
不同种类的消息包含不同的Message Type ID，代表不同的功能。RTMP协议中一共规定了十多种消息类型。例如，1-7的消息用于协议控制，这些消息一般是RTMP协议自身管理要使用的消息，用户一般情况下无需操作其中的数据。
8，9的消息分别用于传输音频和视频数据。（flv中也是8 = audio,9 = video）
15-20的消息用于发送AMF编码命令，负责用户与服务器之间的交互，比如播放，暂停等等。
消息首部（Message Header）由四部分组成：
标志消息类型的Message Type ID，
标志消息长度的Payload Length，
标识时间戳的Timestamp，
标识消息所属媒体流的Stream ID
![](http://note.youdao.com/yws/res/36782/CF50A8B63AE54D7FACC0A92FB9686842)
2 消息块
消息块首部（Chunk Header）由三部分组成：
用于标识本块的Chunk Basic Header，
用于标识本块负载所属消息的Chunk Message Header，
以及当时间戳溢出时才出现的Extended Timestamp
![](http://note.youdao.com/yws/res/85442/D678C94803B94FE6B8308456FC267A88)
3 消息分块
在消息被分割成几个消息块的过程中，消息负载部分（Message Body）被分割成大小固定的数据块（默认是128字节，最后一个数据块可以小于该固定长度），并在其首部加上消息块首部（Chunk Header），就组成了相应的消息块。消息分块过程如图5所示，一个大小为307字节的消息被分割成128字节的消息块（除了最后一个）。
![](http://note.youdao.com/yws/res/85449/1C0A123177E0427F83CACD64C90B1B73)
其中1，2，3分别代表chunk header里面的...
发送端首先把数据封装成消息，然后把消息分割成消息块，最后将分割后的消息块通过TCP协议发送出去。接收端在通过TCP协议收到数据后，首先把消息块重新组合成消息，然后通过对消息进行解封装处理就可以恢复出媒体数据。
[http://blog.csdn.net/leixiaohua1020/article/details/11694129](http://blog.csdn.net/leixiaohua1020/article/details/11694129)
