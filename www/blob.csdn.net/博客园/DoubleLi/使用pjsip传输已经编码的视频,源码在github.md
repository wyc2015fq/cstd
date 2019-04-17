# 使用pjsip传输已经编码的视频,源码在github - DoubleLi - 博客园






pjsip功能很强，做sip rtp语音通话库首选。在2.0之后，也支持视频。不过，它的视频功能缺省是从视频设备采集，然后进行编译，再发送出去的。假设，我们已经有了视频源，比如IP摄像机，不需要采集和编码这个过程，怎么处理呢？假设我们采用pjsip附带的pjsua为例。

通常的方法：
1 把视频源当然文件来处理，sample有。不过这种方法用的不多。
2 修改vid_stream.c，在put_frame和get_frame里，换上我们自己的视频源。这种方法使用的最多，很多人在1.x版本里支持视频，就用这种方法。
3 本文采用的:重新构造sdp，自己创建rtp通道。

前两个方法思路直接，代码量都不小，尤其是第二种，需要修改pj底层代码。
如果基于pjsua做一个简单的视频通信，可以采用本文中的方法。其实代码量也不小，不过sample提供了参考，实现起来也比较容易。下面简单说明：

1 关键点在sdp上，pjsua_call_make_call这个函数非常方便，直接呼叫对方。不过它在底层做了太多工作，比如启动了声卡。而不用这个函数，直接用比较底层的pjsip_inv_send_msg，自己处理的工作相对比较多（但不难，不过这样就不需要pjsua这个现成的程序了，所以我们继续用pjsua_call_make_call）。
不过还好，pj库提供了大量的回调，其中一个：on_call_sdp_created，就是在创建sdp后回调上来，由我们自己再修改。比如我们自己定义rtp的端口g_local_port。

void on_call_sdp_created(pjsua_call_id call_id,
                                   pjmedia_sdp_session *sdp,
                                   pj_pool_t *pool,
                                   const pjmedia_sdp_session *rem_sdp)
{
int nPort;
if (sdp != NULL)
{

pjmedia_sdp_media *m = sdp->media[sdp->media_count-1];
m->desc.port = g_local_port;


pjmedia_sdp_conn *c = sdp->conn;
char* addr;
if (c)
addr= c->addr.ptr;
else
{

const pj_str_t *hostname;
pj_sockaddr_in tmp_addr;
char *addr;

hostname = pj_gethostname();
pj_sockaddr_in_init(&tmp_addr, hostname, 0);
addr = pj_inet_ntoa(tmp_addr.sin_addr);
sdp->conn = (pjmedia_sdp_conn *)pj_pool_zalloc (pool, sizeof(pjmedia_sdp_conn));
sdp->conn->net_type = pj_str("IN");
sdp->conn->addr_type = pj_str("IP4");
sdp->conn->addr = pj_str(addr);
}

sdp->origin.addr = *pj_gethostname();
}
}

同样，这里还可以修改payload type等。

这是发起呼叫时的，接收方收到后的回应之后，也会触发这个回调，自己设定RTP端口，payload type就可以了。

2 
呼叫成功后，双方建立起连接关系，这时需要传rtp数据了。pjsua把这些工作都放在底层了，不做任何修改，只需要在发送和接收时，自己做一些处理就行。
先说接收方(参考siprtp.c源码)：
pj_status_t init_local_rtp()
{
if (m_bInitMedia)
{
destroy_media();
}


//g_local_port = local_port;


pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
    pool = pj_pool_create(&(cp.factory), "test", 1000, 512, NULL);


int status;

//status = pjmedia_endpt_create(&cp.factory,  pjsip_endpt_get_ioqueue(pjsua_get_pjsip_endpt()), 0, &med_endpt);


status = pjmedia_endpt_create(&cp.factory, NULL, 1, &med_endpt);


status = pjmedia_rtp_session_init(&video.out_sess, 97, pj_rand());
status = pjmedia_rtp_session_init(&video.in_sess, 97, 0);


status = pjmedia_transport_udp_create(med_endpt, NULL, g_local_port,  0, &video.transport);


m_bInitMedia = true;
video.active = true;
return 0;
}

这段代码是本地启动rtp一个端口用来接收视频数据。
然后，从sdp得到对方发送的ip和端口，调用pjmedia_transport_attach，建立关联就可以了。


发送方同样调用上面函数，初始化本地端口，但不需要pjmedia_transport_attach。（假定视频传输是单方向的）
发送动作就比较简单了，先把要发的数据分包，大小不超过1400，然后pjmedia_rtp_encode_rtp，再pjmedia_transport_send_rtp。


上面还没有讲接收方是怎么接数据的，这里也用到了pj提供的回调机制：
status = pjmedia_transport_attach(video.transport, &video, 
//&info.rem_addr, 
&remote_addr,
NULL, 
sizeof(pj_sockaddr_in),
&on_rx_rtp,
NULL);

这里面的on_rx_rtp就是接收RTP的回调。

video.transport等定义：


struct media_stream
{
    /* Static: */
    unsigned	call_index;	   /* Call owner.	*/
    unsigned	media_index;	   /* Media index in call.	*/
    pjmedia_transport   *transport;	   /* To send/recv RTP/RTCP	*/


    /* Active? */
    pj_bool_t	active;	   /* Non-zero if is in call.	*/


    /* Current stream info: */
    pjmedia_stream_info	si;	   /* Current stream info.	*/


    /* More info: */
    unsigned	clock_rate;	   /* clock rate	*/
    unsigned	samples_per_frame; /* samples per frame	*/
    unsigned	bytes_per_frame;   /* frame size.	*/


    /* RTP session: */
    pjmedia_rtp_session	out_sess;	   /* outgoing RTP session	*/
    pjmedia_rtp_session	in_sess;	   /* incoming RTP session	*/


    /* RTCP stats: */
    pjmedia_rtcp_session rtcp;	   /* incoming RTCP session.	*/


    /* Thread: */
    pj_bool_t	thread_quit_flag;  /* Stop media thread.	*/
    pj_thread_t	*thread;	   /* Media thread.	*/
};

struct media_stream video;


源码在：https://github.com/sxcong/pjsipvideo_demo
视频源RTSP，可以直接使用IPCAM，比如海康摄像机。
SIP SERVER是开源的resiprocate，编译出来可直接使用。
DEMO程序是vc2008写的，包括SIP的登录，发送请求，发送和接收视频并解码播放。可在同一台机器上运行两个实例测试。
不过毕竟是DEMO，只是演示怎么使用，细节还有很多问题需要修改。



from:http://blog.chinaunix.net/uid-15063109-id-4445165.html









