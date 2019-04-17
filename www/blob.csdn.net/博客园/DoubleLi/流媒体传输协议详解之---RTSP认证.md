# 流媒体传输协议详解之---RTSP认证 - DoubleLi - 博客园







from:http://blog.csdn.net/machh/article/details/52121648

Rtsp认证主要分为两种：



基本认证（basic authentication）和摘要认证（ digest authentication　）。



基本认证是http 1.0提出的认证方案，其消息传输不经过加密转换因此存在严重的安全隐患。

摘要认证是http 1.1提出的基本认证的替代方案，其消息经过MD5哈希转换因此具有更高的安全性。下面主要介绍摘要认证：

## 1.基本认证 （basic 认证）



1.  客户端发送DESCRIBE请求到服务端，


- DESCRIBE  rtsp://192.168.1.55:554/11
- RTSP/1.0\r\n
- CSeq: 1\r\n
- Accept: application/sdp\r\n
- User-agent: Realplayer\r\n\r\n



2：RTSP服务端认为没有通过认证，发出WWW-Authenticate认证响应



RTSP/1.0 401 Unauthorized\r\n
CSeq: 1\r\n
**WWW-Authenticate: ****Basic** realm="RTSPD"\r\n\r\n



   此时客户端程序应该如果弹出密码认证窗口 ，提示用户名，输入认证信息，密码认证窗口

从响应消息中进行判断，如果发现是**Basic 认证，按如下方式处理**

步骤3：客户端携带**Authorization**串再次发出DESCRIBE请求




DESCRIBE rtsp://192.168.1.55:554/live/1/video.sdp?token=A00453FR805a54C8
RTSP/1.0\r\n
CSeq: 2\r\n
Accept: application/sdp\r\n
User-Agent: RealMedia Player HelixDNAClient/12.0.1.647 (win32)\r\n
Authorization: Basic YWRtaW46YWRtaW4=\r\n\r\n



其中“YWRtaW46YWRtaW4=”是通过对 **username:password 进行base64编码所得**



参考代码：




**[cpp]**[view plain](http://blog.csdn.net/machh/article/details/52121648#)[copy](http://blog.csdn.net/machh/article/details/52121648#)

[print](http://blog.csdn.net/machh/article/details/52121648#)[?](http://blog.csdn.net/machh/article/details/52121648#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- char sztemp[64] = {0};  
- sprintf( sztemp, "%s:%s", m_szAuthorName, m_szPwd); //"admin","admin" );  //  
- string strOutBase64;  
- BaseEncoder::Base64Encode( (BYTE*)sztemp, strlen(sztemp),&strOutBase64 );  








## 2. **摘要认证 Digest authentication**





1.客户端发送DESCRIBE请求



DESCRIBE rtsp://192.168.123.158:554/11 RTSP/1.0

CSeq: 2

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2012.09.13)

Accept: application/sdp



**服务器端返回401错误，提示未认证并以nonce质询：**

RTSP/1.0 401 Unauthorized

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 2

**WWW-Authenticate:Digest**  realm="HipcamRealServer", **nonce**="3b27a446bfa49b0c48c3edb83139543d"



**2.客户端以用户名，密码，nonce，HTTP方法，请求的URI等信息为基础产生response信息进行反馈**

DESCRIBE rtsp://192.168.123.158:554/11 RTSP/1.0

CSeq: 3

**Authorization:**** Digest** username="admin",realm="Hipcam RealServer", nonce="3b27a446bfa49b0c48c3edb83139543d",uri="rtsp://192.168.123.158:554/11", response="258af9d739589e615f711838a0ff8c58"

User-Agent: LibVLC/2.0.5(LIVE555 Streaming Media v2016.06.13)

Accept: application/sdp



**服务器对客户端反馈的response进行校验，通过则返回如下字段：**

**RTSP/1.0 200 OK**

Server: HiIpcam/V100R003 VodServer/1.0.0

Cseq: 3

Content-Type: application/sdp

Cache-Control: must-revalidate

Content-length: 306

Content-Base: rtsp://192.168.123.158:554/11/



v=0

o=StreamingServer 3331435948 1116907222000 IN IP4192.168.123.158

s=\11

c=IN IP4 0.0.0.0

b=AS:1032

t=0 0

a=control:*

m=video 0 RTP/AVP 96

b=AS:1024

a=control:trackID=0

a=rtpmap:96 H264/90000

a=fmtp:96 packetization-mode=1;sprop-parameter-sets=Z0LgHtoCgPRA,aM4wpIA=

a=framesize:96 640-480



**说明：**



**response**计算方法如下：



**RTSP客户端应该使用username + password并计算response如下:**

(1)当password为MD5编码,则

   response = md5( password:nonce:md5(public_method:url)  );

(2)当password为ANSI字符串,则

    response= md5( md5(username:realm:password):nonce:md5(public_method:url) );


客户端在每次发起不同的请求方法时都需要计算response字段，同样在服务器端校验时也默认采取同样的计算方法。








本文为《流媒体开发实战进阶---rtsp视频播放器》视频课程，第一章第二节配套讲义，

更多内容请收看视频讲解！

[http://edu.csdn.net/course/detail/2744](http://edu.csdn.net/course/detail/2744)









