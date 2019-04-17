# rtsp/rtp over http - DoubleLi - 博客园







转载：http://linux-expert.blog.163.com/blog/static/764585292008530912712/

### rtsp/rtp over http




C->S (get)
    GET /1.3gp HTTP/1.0
    User-Agent: ./openRTSP (LIVE555 Streaming Media v2007.07.09)
    x-sessioncookie: 5ba041fbf06108c6e9565a1
    Accept: application/x-rtsp-tunnelled
    Pragma: no-cache
    Cache-Control: no-cache

S->C(reply)
    HTTP/1.0 200 OK\r\n
    Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )\r\n
    Content-Type: application/x-rtsp-tunnelled\r\n


C->S (post)
    POST /1.3gp HTTP/1.0
    User-Agent: ./openRTSP (LIVE555 Streaming Media v2007.07.09)
    x-sessioncookie: 5ba041fbf06108c6e9565a1
    Content-Type: application/x-rtsp-tunnelled
    Pragma: no-cache
    Cache-Control: no-cache
    Content-Length: 32767
    Expires: Sun, 9 Jan 1972 00:00:00 GMT



C->S(option)

T1BUSU9OUyBydHNwOi8vMTcyLjE2LjEwMC43OC8xLjNncCBSVFNQLzEuMA0KQ1NlcTogMQ0KVXNlci1BZ2VudDogLi9vcGVuUlRTUCAoT

ElWRTU1NSBTdHJlYW1pbmcgTWVkaWEgdjIwMDcuMDcuMDkpDQoNCg==

这种通过HTTPchannel方式的区别就是它将C->S的请求(POST), 通过base64编码之后再发给服务器。这样中间路由防火墙
就看不到这个RTSP请求了。


S->C(reply)
    RTSP/1.0 200 OK
    Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )
    Cseq: 1
    Public: DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, OPTIONS, ANNOUNCE, RECORD


C->S (describe)
REVTQ1JJQkUgcnRzcDovLzE3Mi4xNi4xMDAuNzgvMS4zZ3AgUlRTUC8xLjANCkNTZXE6IDINCkFjY2VwdDogYXBwbGljYXRpb24vc2RwD

QpVc2VyLUFnZW50OiAuL29wZW5SVFNQIChMSVZFNTU1IFN0cmVhbWluZyBNZWRpYSB2MjAwNy4wNy4wOSkNCg0K

S->C (reply)
RTSP/1.0 200 OK
Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )
Cseq: 2
Last-Modified: Thu, 19 Jul 2007 01:41:09 GMT
Cache-Control: must-revalidate
Content-length: 504
Date: Mon, 23 Jul 2007 02:23:42 GMT
Expires: Mon, 23 Jul 2007 02:23:42 GMT
Content-Type: application/sdp
x-Accept-Retransmit: our-retransmit
x-Accept-Dynamic-Rate: 1
Content-Base: rtsp://172.16.100.78/1.3gp/

v=0
o=StreamingServer 3394146222 1184809269000 IN IP4 172.16.100.78
s=/1.3gp
u=http:///
e=admin@
c=IN IP4 0.0.0.0
b=AS:50
t=0 0
a=control:*
a=range:npt=0- 70.00000
m=video 0 RTP/AVP 96
b=AS:41
a=rtpmap:96 MP4V-ES/90000
a=control:trackID=3
a=cliprect:0,0,144,176
a=framesize:96 176-144
a=fmtp:96 profile-level-id=1;config=000001B008000001B50EE040C0CF0000010000000120008440FA282C2090A21F
m=audio 0 RTP/AVP 97
b=AS:8
a=rtpmap:97 AMR/8000/1
a=control:trackID=4
a=fmtp:97 octet-align=1



C->S (SETUP)
U0VUVVAgcnRzcDovLzE3Mi4xNi4xMDAuNzgvMS4zZ3AvdHJhY2tJRD0zIFJUU1AvMS4wDQpDU2VxOiAzDQpUcmFuc3BvcnQ6IFJUUC9BV

lAvVENQO3VuaWNhc3Q7aW50ZXJsZWF2ZWQ9MC0xDQpVc2VyLUFnZW50OiAuL29wZW5SVFNQIChMSVZFNTU1IFN0cmVhbWluZyBNZWRpYS

B2MjAwNy4wNy4wOSkNCg0K

S->C(reply)
RTSP/1.0 200 OK
Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )
Cseq: 3
Last-Modified: Thu, 19 Jul 2007 01:41:09 GMT
Cache-Control: must-revalidate
Session: 2110769224986338178
Date: Mon, 23 Jul 2007 02:23:42 GMT
Expires: Mon, 23 Jul 2007 02:23:42 GMT
Transport: RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=771536A8


C->S(setup)
U0VUVVAgcnRzcDovLzE3Mi4xNi4xMDAuNzgvMS4zZ3AvdHJhY2tJRD00IFJUU1AvMS4wDQpDU2VxOiA0DQpUcmFuc3BvcnQ6IFJUUC9BV

lAvVENQO3VuaWNhc3Q7aW50ZXJsZWF2ZWQ9Mi0zDQpTZXNzaW9uOiAyMTEwNzY5MjI0OTg2MzM4MTc4DQpVc2VyLUFnZW50OiAuL29wZW

5SVFNQIChMSVZFNTU1IFN0cmVhbWluZyBNZWRpYSB2MjAwNy4wNy4wOSkNCg0K

S->C(reply)
RTSP/1.0 200 OK
Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )
Cseq: 4
Session: 2110769224986338178
Last-Modified: Thu, 19 Jul 2007 01:41:09 GMT
Cache-Control: must-revalidate
Date: Mon, 23 Jul 2007 02:23:42 GMT
Expires: Mon, 23 Jul 2007 02:23:42 GMT
Transport: RTP/AVP/TCP;unicast;interleaved=2-3;ssrc=4CAA823B


C->S (play)
UExBWSBydHNwOi8vMTcyLjE2LjEwMC43OC8xLjNncC8gUlRTUC8xLjANCkNTZXE6IDUNClNlc3Npb246IDIxMTA3NjkyMjQ5ODYzMzgxN

zgNClJhbmdlOiBucHQ9MC4wMDAtDQpVc2VyLUFnZW50OiAuL29wZW5SVFNQIChMSVZFNTU1IFN0cmVhbWluZyBNZWRpYSB2MjAwNy4wNy

4wOSkNCg0K

S->C (reply)
RTSP/1.0 200 OK
Server: DSS/5.5.5 (Build/489.16; Platform/Linux; Release/Darwin; state/beta; )
Cseq: 5
Session: 2110769224986338178
Range: npt=0.00000-70.00000
RTP-Info: 

url=rtsp://172.16.100.78/1.3gp/trackID=3;seq=11720;rtptime=1814783692,url=rtsp://172.16.100.78/1.3gp/trac

kID=4;seq=53596;rtptime=894859716



S->C (data)
2400017580e02deb6c2e9b1c771536a8000001b65567809f5b1f7c49120bbea0bc4b1f2affc46f2b2ff7bff456b8d3e81cf0301b1

2d5095b1983f2f2fc5c1b0200f8491271382161281ab947c2489410d50191f7ef2acacb95f95096252b80a61247c5cac90e3d86ce

7f30ffd780b27197c296044540f13005aad022a3741884d9d7a01a9cfac51a701c189a2459b25823ac99fe9548e164d53bbe157e6

b497ad493b6a2728aa390459642b7acfa4c7f402c9d935bcf7970f5fecea6604227719835d87202dc6b98da4942d966ec5cffcd5f

9989e7126f4602936e804f9c3bbce8da434061b5d3402c1f396c98bc89d98969691c94fec3d34b0b2e6402facb720dca88c1cfe92

7454787068ffe029e1612ce8d5fd3c0490054eb098052636f6b4458053be4145a19ba9e062e02c6a94608cad0fa8aecc32f3663d3

56d46f0a518baa87557e93ef3d9fe7a360f3b931e3a029302afe3ca46d1381c3ee79bf21fec753df9abd318f37c209e74db04c21e

71816d3e75ef5b7fffe

我们只关心前面4个字节，根据rfc2326 10.12, 这种交织方式第一个字节是一个美元符号($),ASCII码为24.第二个字节是交织channel的ID，这里是0，然后两个字节就表示接下来的数据的长度。表示接下来的数据长度是0x0175.
channel为0表示为RTP数据，即接下来的数据是一个RTP数据包，因此开始部分也就是这个RTP包的头部。也是可以给解析

的。










