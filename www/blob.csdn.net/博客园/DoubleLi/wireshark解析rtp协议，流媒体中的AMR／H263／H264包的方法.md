# wireshark解析rtp协议，流媒体中的AMR／H263／H264包的方法 - DoubleLi - 博客园






抓到完整的流媒体包之后，用wireshark打开，其中的包可能不会自动映射成RTP＋AMR／H263／H264的包，做如下修改操作即可：
1.  把UDP 包解析成RTP／RTCP包。选中UDP包，右键，选择Decode As，选RTP
2.  把RTP Payload映射成实际的媒体格式。首先查看RTSP中的SDP的payload_type，比如96是H263，97是AMR。那么点击菜单Edit -> Preference -> Protocol, AMR payload_type number填写97， H263填写96，那么payload就可以自动解析了。

需要注意的是：
wireshark 1.0版本以上才支持H263和H264。









