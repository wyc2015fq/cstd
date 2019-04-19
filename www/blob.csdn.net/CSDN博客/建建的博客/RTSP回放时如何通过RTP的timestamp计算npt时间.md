# RTSP回放时如何通过RTP的timestamp计算npt时间 - 建建的博客 - CSDN博客
2017年10月06日 22:18:23[纪建](https://me.csdn.net/u013898698)阅读数：281
客户端通过RTSP的PLAY方法开始播放、定位播放、快速/慢速播放（Scale）某非实时视频时，客户端会通过Range头字段指定npt时间，即让服务器开始从npt指定时刻开始播放视频（npt即播放位置离文件开始部分的相对时间）。
播放开始后，下一次发送PLAY命令前，客户端需要根据服务器发来的RTP timestamp计算当前收到的帧的npt时间。
### 客户端与服务器同步npt时间
客户端可以在每次收到PLAY响应消息时与服务器同步npt时间，响应消息里可带RTP-Info:rtptime和Range:npt参数。
客户端发往服务器RTSP消息例子：
      PLAY rtsp://foo.com/test.wav RTSP/1.0
     CSeq: 3
     Session: 2034820394
     Range: npt=30.6-3508
     Scale: 2
服务器响应消息的例子：
     RTSP/1.0 200 OK
     CSeq: 3
     Session: 2034820394
     Range: npt=30.6-3508
     Scale: 2
     RTP-Info: url=rtsp://foo.com/test.wav/streamid=0;
     seq=981888;rtptime=3781123
其中响应消息中Range:npt的第一个参数表示客户端紧接着收到的timestamp为RTP-Info:rtptime指定的RTP包时的npt时间。
### 当前npt时间计算公式
公式为：
**  当前npt = begin_npt + ((timestamp_len * reset_counter + current_timestamp - start_timestamp) / clock_rate) * scale**
其中各项含义：
      begin_npt: PLAY返回消息里Range: npt=18.8-3600中的开始时间（秒）
      timestamp_len: RTP timestamp的最大表示个数，4个字节，所以是2^32即0x100000000
      reset_counter: RTP timestamp的归零次数
      current_timestamp: 当前收到的RTP的timestamp值
      start_timestamp: PLAY返回消息里的RTP-Info里的rtptime(指明此后第一个收到的RTP包的timestamp值)
      clock_rate: 时钟频率，即SDP a=rtpmap:96 H264/90000里的90000
      scale: PLAY里请求和返回中的Scale值（以返回为准）
### npt与RTP timestamp的进一步讨论
rtp的timestamp里的wallclock应该是表示RTP会话开始到当前的绝对elapsed时间，与npt不是对应的。（比如PAUSE以后，重新PLAY时npt是影片开始的相对时间，而rtp timestamp是rtp会话开始的相对时间）
加速播放时（RTSP scale=2），在PLAY返回让npt与rtptime映射后，npt计算时要加倍计算，而rtptime间隔保持不变。
另外，关于RTP timestamp与帧率等关系可参考《[RTP timestamp与帧率及时钟频率的关系](http://blog.csdn.net/jasonhwang/article/details/7316128)》([http://blog.csdn.net/jasonhwang/article/details/7316128](http://blog.csdn.net/jasonhwang/article/details/7316128))。
### 参考资料
##### [1] RFC2326 (Appendix B: Interaction with RTP):
For scaling (see Section 12.34), RTP timestamps should correspond to
the playback timing. For example, when playing video recorded at 30
frames/second at a scale of two and speed (Section 12.35) of one, the
server would drop every second frame to maintain and deliver video
packets **with the normal timestamp spacing of 3,000 per frame**, but NPT
would increase by 1/15 second for each video frame.
The client can maintain a correct display of NPT by noting the RTP
timestamp value of the first packet arriving after repositioning. The
sequence parameter of the RTP-Info (Section 12.33) header provides
the first sequence number of the next segment.
##### [2] 3GPP 26.234 (A.3.2.4 Timestamp handling after PAUSE/PLAY requests):
The description below intends to clarify how RTP timestamps are specified within the 3GPP PSS when a client sends a PLAY request following a PAUSE request. The RTP timestamp space must be continuous
 along time during a session and then reflect the actual time elapsed since the beginning of the session. A server must reflect the actual time interval elapsed between the last RTP packets sent before the reception of the PAUSE request and the first RTP packets
 sent after the reception of the PLAY request in the RTP timestamp. A client will need to compute the mapping between NPT time and RTP timestamp each time it receives a PLAY response for on-demand content. This means that a client must be able to cope with
 any gap in RTP timestamps after a PLAY request.
The PLAY request can include a Range header if the client wants to seek backward or forward in the media, or without a Range header if the client only wants to resume the paused session.
Example:
In this example Client C plays a media file from Server S. RTP timestamp rate in this example is 1000Hz for clarity.
C -> S: PLAY rtsp://example.com/mediastream RTSP/1.0
CSeq: 2
Session: 123456
Range: npt=1.125-
S -> C: RTSP/1.0 200 OK
CSeq: 2
Session: 123456
Range: npt=1.120-
RTP-Info: url=rtsp://example.com/mediastream;seq=1000;rtptime=5000
S -> C: RTP packet - seq = 1000 - rtptime = 5000 - corresponding media time (NPT time) = 1120ms
S -> C: RTP packet - seq = 1001 - rtptime = 5040 - corresponding media time (NPT time) = 1160ms
S -> C: RTP packet - seq = 1002 - rtptime = 5080 - corresponding media time (NPT time) = 1200ms
S -> C: RTP packet - seq = 1003 - rtptime = 5120 - corresponding media time (NPT time) = 1240ms
C -> S: PAUSE rtsp://example.com/mediastream RTSP/1.0
CSeq: 3
Session: 123456
S -> C: RTSP/1.0 200 OK
CSeq: 3
Session: 123456
[10 seconds elapsed]
C -> S: PLAY rtsp://example.com/mediastream RTSP/1.0
CSeq: 4
Session: 123456
S -> C: RTSP/1.0 200 OK
CSeq: 4
Session: 123456
Range: npt=1.280-
RTP-Info: url=rtsp://example.com/mediastream;seq=1004;rtptime=15160
S -> C: RTP packet - seq = 1004 - rtptime = 15160 - corresponding media time (NPT time) = 1280ms
S -> C: RTP packet - seq = 1005 - rtptime = 15200 - corresponding media time (NPT time) = 1320ms
S -> C: RTP packet - seq = 1006 - rtptime = 15240 - corresponding media time (NPT time) = 1360ms
C -> S: PAUSE rtsp://example.com/mediastream RTSP/1.0
CSeq: 5
Session: 123456
S -> C: RTSP/1.0 200 OK
CSeq: 5
Session: 123456
C -> S: PLAY rtsp://example.com/mediastream RTSP/1.0
CSeq: 6
Session: 123456
Range: npt=0.5-
[55 milliseconds elapsed during request processing]
S -> C: RTSP/1.0 200 OK
CSeq: 6
Session: 123456
Range: npt=0.480-
RTP-Info: url=rtsp://example.com/mediastream;seq=1007;rtptime=15295
S -> C: RTP packet - seq = 1007 - rtptime = 15295 - corresponding media time (NPT time) = 480ms
S -> C: RTP packet - seq = 1008 - rtptime = 15335 - corresponding media time (NPT time) = 520ms
S -> C: RTP packet - seq = 1009 - rtptime = 15375 - corresponding media time (NPT time) = 560ms
