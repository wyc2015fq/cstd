# rtpsession详解 - 建建的博客 - CSDN博客
2017年08月05日 16:44:27[纪建](https://me.csdn.net/u013898698)阅读数：159
个人分类：[Darwin](https://blog.csdn.net/u013898698/article/category/7009805)
                
以下过程仅分析推送视频模式：
服务器监听rtsp端口号，当有客户端连接后，会创建RTSPSession，在客户端请求过程中的announce时，会创建RTPSession，
在RTSPSession中维护了一个RTPSession成员。
RTSPSession和RTPSession在QTSServerInterface::GetServer()中都有一个map相关联。
RTPSession内维护了一组RTPStream，RTPSession::AddStream。
RTSPSession内部维护了UDPSocketPair，用来创建rtp和rtcp套接字，但是仅在以udp方式发送数据给客户端时才会创建UDPSocketPair，tcp方式不会创建
QTSS_Error RTPStream::Setup(RTSPRequestInterface* request, QTSS_AddStreamFlags inFlags)中
**[cpp]**[view plain](http://blog.csdn.net/sunxiaopengsun/article/details/71192044#)[copy](http://blog.csdn.net/sunxiaopengsun/article/details/71192044#)
- // Check to see if this RTP stream should be sent over TCP.
- if (fTransportType == qtssRTPTransportTypeTCP)  
- {  
-     fIsTCP = true;  
-     fSession->GetOverbufferWindow()->SetWindowSize(kUInt32_Max);  
- 
- // If it is, get 2 channel numbers from the RTSP session.
-     fRTPChannel = request->GetSession()->GetTwoChannelNumbers(fSession->GetValue(qtssCliSesRTSPSessionID));  
-     fRTCPChannel = fRTPChannel+1;  
- 
- // If we are interleaving, this is all we need to do to setup.
- return QTSS_NoErr;  
- }  
