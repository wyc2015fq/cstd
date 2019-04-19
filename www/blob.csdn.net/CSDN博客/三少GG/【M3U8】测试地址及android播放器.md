# 【M3U8】测试地址及android播放器 - 三少GG - CSDN博客
2013年02月07日 01:17:47[三少GG](https://me.csdn.net/scut1135)阅读数：25823
///////////////////
[http://www.cnblogs.com/over140/](http://www.cnblogs.com/over140/)
////////////////
### [HLS协议---学习笔记----M3U8](http://blog.csdn.net/test_1207/article/details/8160863)
分类： [视频编解码](http://blog.csdn.net/test_1207/article/category/1280817)
详情可参见“draft-pantos-http-live-streaming-08”
一、概念
1. Playlist file
> 
    一个M3U的 Playlist 就是一个由多个独立行组成的文本文件，每行由回车/换行区分。每一行可以是一个URI  空白行或
是以”#“号开头的字符串，并且空格只能存在于一行中不同元素间的分隔。
   一个URI 表示一个媒体段或是”variant Playlist file“（最多支持一层嵌套，即一个mm3u8文件中嵌套另一个m3u8），
以”#EXT“开头的表示一个”tag“，否则表示注释，直接忽略
2. Tags
  #EXTM3U:  每个M3U文件第一行必须是这个tag。
#EXTINF:指定每个媒体段(ts)的持续时间，这个仅对其后面的URI有效，每两个媒体段URI间被这个tag分隔开，其格式如下：
                     #EXTINF:<duration>,<title>  ：
> 
> 
  duration表示持续的时间（秒）”Durations MUST be integers if the protocol version of the Playlist file is less
than 3“，否则可以是浮点数。
 #EXT-X-BYTERANGE:表示媒体段是一个媒体URI资源中的一段，只对其后的media URI有效，格式如下：
                     #EXT-X-BYTERANGE:<n>[@o]：
> 
> 
  其中n表示这个区间的大小，o表在URI中的offset；”The EXT-X-BYTERANGE tag appeared in version 4 of the protocol“。
#EXT-X-TARGETDURATION：指定最大的媒体段时间长（秒）。所以#EXTINF中指定的时间长度必须小于或是等于这个最大
值。这个tag在整个PlayList文件中只能出现一 次（在嵌套的情况下，一般有真正ts url的m3u8才会出现该tag）。格式如下：
                     #EXT-X-TARGETDURATION:<s>：s表示最大的秒数。
  #EXT-X-MEDIA-SEQUENCE：每一个media URI 在 PlayList中只有唯一的序号，相邻之间序号+1。
                     #EXT-X-MEDIA-SEQUENCE:<number>：
                    一个media URI并不是必须要包含的，如果没有，默认为0。
  #EXT-X-KEY:表示怎么对media segments进行解码。其作用范围是下次该tag出现前的所有media URI，格式如下：
                     #EXT-X-KEY:<attribute-list>：
                    NONE 或者 AES-128。如果是NONE，则URI以及IV属性必须不存在，如果是AES-128(Advanced Encryption
                    Standard)，则URI必须存在，IV可以不存在。
                    对于AES-128的情况，keytag和URI属性共同表示了一个key文件，通过URI可以获得这个key，如果没有
                    IV（Initialization Vector）,则使用序列号作为IV进行编解码，将序列号的高位赋到16个字节的buffer中，左边补0；如果
                    有IV，则将改值当成16个字节的16进制数。
  #EXT-X-PROGRAM-DATE-TIME：将一个绝对时间或是日期和一个媒体段中的第一个sample相关联，只对下一个meida URI有效，格式如下：
                     #EXT-X-PROGRAM-DATE-TIME:<YYYY-MM-DDThh:mm:ssZ>
                     For example:
                     #EXT-X-PROGRAM-DATE-TIME:2010-02-19T14:54:23.031+08:00
 #EXT-X-ALLOW-CACHE：是否允许做cache，这个可以在PlayList文件中任意地方出现，并且最多出现一次，作用效果是所有的媒体段。格式如下：
                     #EXT-X-ALLOW-CACHE:<YES|NO>
 #EXT-X-PLAYLIST-TYPE： 提供关于PlayList的可变性的信息， 这个对整个PlayList文件有效，是可选的，格式如下：
                     #EXT-X-PLAYLIST-TYPE:<EVENT|VOD> ：如果是VOD，则服务器不能改变PlayList 文件；如果是EVENT，则
服务器不能改变或是删除PlayList文件中的任何部分，但是可以向该文件中增加新的一行内容。
#EXT-X-ENDLIST：表示PlayList的末尾了，它可以在PlayList中任意位置出现，但是只能出现一个，格式如下：
                     #EXT-X-ENDLIST
 #EXT-X-MEDIA：被用来在PlayList中表示相同内容的不用语种/译文的版本，比如可以通过使用3个这种tag表示3中不用语音的音
频，或者用2个这个tag表示不同角度的video在PlayLists中。这个标签是独立存在的，其格式如下：
                    #EXT-X-MEDIA:<attribute-list>：该属性列表中包含：URI、TYPE、GROUP-ID、LANGUAGE、NAME、DEFAULT、AUTOSELECT。
                    URI：如果没有，则表示这个tag描述的可选择版本在主PlayList的EXT-X-STREAM-INF中存在;
                    TYPE:AUDIO and VIDEO;
                    GROUP-ID:具有相同ID的MEDIAtag，组成一组样式；
                    LANGUAGE：identifies the primary language used in the rendition。
                    NAME：The value is a quoted-string containing a human-readable description of the rendition. If the LANGUAGE attribute is present then this description SHOULD be in that language。
                    DEFAULT： YES或是NO，默认是No，如果是YES，则客户端会以这种选项来播放，除非用户自己进行选择。
                    AUTOSELECT：YES或是NO，默认是No，如果是YES，则客户端会根据当前播放环境来进行选择（用户没有根据自己偏好进行选择的前提下）。
        The EXT-X-MEDIA tag appeared in version 4 of the protocol。
> 
o All EXT-X-MEDIA tags in the same group MUST have the same TYPE
attribute.
o All EXT-X-MEDIA tags in the same group MUST have different NAME
attributes.
o A group MUST NOT have more than one member with a DEFAULT
attribute of YES.
o All members of a group whose AUTOSELECT attribute has a value of
YES MUST have LANGUAGE [RFC5646] attributes with unique values.
o All members of a group with TYPE=AUDIO MUST use the same audio
sample format.
o All members of a group with TYPE=VIDEO MUST use the same video
sample format。
  #EXT-X-STREAM-INF：指定一个包含多媒体信息的 media URI 作为PlayList，一般做M3U8的嵌套使用，它只对紧跟后面的URI有
效，格式如下：
> 
#EXT-X-STREAM-INF:<attribute-list>
<URI>
有以下属性：
BANDWIDTH：带宽，必须有。
PROGRAM-ID：该值是一个十进制整数，惟一地标识一个在PlayList文件范围内的特定的描述。一个PlayList 文件中可
能包含多个有相同ID的此tag。
CODECS：不是必须的。
RESOLUTION：分辨率。
AUDIO：这个值必须和AUDIO类别的“EXT-X-MEDIA”标签中“GROUP-ID”属性值相匹配。
VIDEO：同上
(未完：待续...............)
**测试地址！！！**
[http://124.160.184.108/live/5/45/3bfabc1fe16a4282b50ea095928c1f60.m3u8](http://124.160.184.108/live/5/45/3bfabc1fe16a4282b50ea095928c1f60.m3u8)[](http://124.160.184.108/live/5/45/3bfabc1fe16a4282b50ea095928c1f60.m3u8) 湖北卫视
[http://124.160.184.108/live/5/45/847a4e1e97584d39a961f5604e90c1c4.m3u8](http://124.160.184.108/live/5/45/847a4e1e97584d39a961f5604e90c1c4.m3u8)浙江卫视
[http://124.160.184.108/live/5/45/3f8a56bdf383478b85fb380343587b43.m3u8](http://124.160.184.108/live/5/45/3f8a56bdf383478b85fb380343587b43.m3u8)广西卫视
Vlc地址大全
亚洲电影 http://223.27.170.210/channels/39/800.flv/live
**本港国际 rtsp://116.199.127.68/guoji**
台电视新闻 http://nextmedia-f.akamaihd.net/nexttvlivebk_1_300@31425
民视 http://211.79.36.141/iTV/segments/ch46/index.m3u8
天元围棋 http://live.gslb.letv.com/gslb?stream_id=tywq&tag=live&ext=m3u8&sign=live_ipad
乐视网 http://live.gslb.letv.com/gslb?stream_id=channel_1_pc&tag=live&ext=m3u8&sign=live_ipad
**CCTV1综合 http://live.gslb.letv.com/gslb?stream_id=cctv1&tag=live&ext=m3u8&sign=live_ipadCCTV2财经 http://live.gslb.letv.com/gslb?stream_id=cctv2&tag=live&ext=m3u8&sign=live_ipadCCTV3综艺 http://live.gslb.letv.com/gslb?stream_id=cctv3&tag=live&ext=m3u8&sign=live_ipadCCTV-4亚洲 http://live.gslb.letv.com/gslb?stream_id=cctv4&tag=live&ext=m3u8&sign=live_ipadCCTV5体育 http://live.gslb.letv.com/gslb?stream_id=cctv5_800&tag=live&ext=m3u8&sign=live_ipadCCTV6电影 http://live.gslb.letv.com/gslb?stream_id=cctv6&tag=live&ext=m3u8&sign=live_ipadCCTV7军事农业 http://live.gslb.letv.com/gslb?stream_id=cctv7&tag=live&ext=m3u8&sign=live_ipadCCTV8电视剧 http://live.gslb.letv.com/gslb?stream_id=cctv8&tag=live&ext=m3u8&sign=live_ipadCCTV9纪录 http://live.gslb.letv.com/gslb?stream_id=cctv9&tag=live&ext=m3u8&sign=live_ipadCCTV10科教 http://live.gslb.letv.com/gslb?stream_id=cctv10&tag=live&ext=m3u8&sign=live_ipad**
CCTV11戏曲 http://live.gslb.letv.com/gslb?stream_id=cctv11&tag=live&ext=m3u8&sign=live_ipad
CCTV12社会与法 http://live.gslb.letv.com/gslb?stream_id=cctv12&tag=live&ext=m3u8&sign=live_ipad
CCTV13新闻 http://live.gslb.letv.com/gslb?stream_id=cctvnew&tag=live&ext=m3u8&sign=live_ipad
CCTV14少儿 http://live.gslb.letv.com/gslb?stream_id=cctvshaoer&tag=live&ext=m3u8&sign=live_ipad
CCTV15音乐 http://live.gslb.letv.com/gslb?stream_id=cctvmusic&tag=live&ext=m3u8&sign=live_ipad
CCTV第一剧场 http://live.gslb.letv.com/gslb?stream_id=dyjc&tag=live&ext=m3u8&sign=live_ipad
CCTV风云音乐 http://live.gslb.letv.com/gslb?stream_id=fyyy&tag=live&ext=m3u8&sign=live_ipad
CCTV风云足球 http://live.gslb.letv.com/gslb?stream_id=fyzq&tag=live&ext=m3u8&sign=live_ipad
CCTV国防军事 http://live.gslb.letv.com/gslb?stream_id=guofangjunshi&tag=live&ext=m3u8&sign=live_ipad
CCTV怀旧剧场 http://live.gslb.letv.com/gslb?stream_id=hjjc&tag=live&ext=m3u8&sign=live_ipad
CCTV世界地理 http://live.gslb.letv.com/gslb?stream_id=shijiedili&tag=live&ext=m3u8&sign=live_ipad
CCTV央视精品 http://biz.vsdn.tv380.com/playlive.php?5B63686E5D445830303030303034367C343436367C317C313030307C4C4235302E434E7C687474707C74735B2F63686E5DVSDNSOOONERCOM00
CCTV央视台球 http://ims.veryhd.net/ty/ts.php?tsid=464691
空中课堂 rtmp://live.guoshi.com/live/kk
**凤凰卫视香港台 http://live.3gv.ifeng.com/live/hongkong.m3u8香港卫视 http://qqlive.rec.hdl.lxdns.com/2584831218.flv**
旅游卫视 [http://live.gslb.letv.com/gslb?stream_id=lvyou&tag=live&ext=m3u8](http://live.gslb.letv.com/gslb?stream_id=lvyou&tag=live&ext=m3u8)
南方卫视 http://live.gslb.letv.com/gslb?stream_id=nanfang&tag=live&ext=m3u8
华娱卫视 rtmp://119.164.219.215:80/live/hyws
星空卫视 http://live.gslb.letv.com/gslb?stream_id=xingkongweishi&tag=live&ext=m3u8
**江苏卫视 http://live.gslb.letv.com/gslb?stream_id=jiangsu&tag=live&ext=m3u8**
山东卫视 http://live.gslb.letv.com/gslb?stream_id=shandong&tag=live&ext=m3u8
陕西卫视 http://live.gslb.letv.com/gslb?stream_id=shanxi1&tag=live&ext=m3u8&sign=live_ipad
深圳卫视 http://live.gslb.letv.com/gslb?stream_id=shenzhen&tag=live&ext=m3u8
安徽卫视 http://live.gslb.letv.com/gslb?stream_id=anhui&tag=live&ext=m3u8
北京卫视 http://live.gslb.letv.com/gslb?stream_id=bjws&tag=live&ext=m3u8&sign=live_ipad
东方卫视 http://live.gslb.letv.com/gslb?stream_id=dongfang&tag=live&ext=m3u8
甘肃卫视 http://live.gslb.letv.com/gslb?stream_id=gansu&tag=live&ext=m3u8
广东卫视 http://live.gslb.letv.com/gslb?stream_id=guangdong&tag=live&ext=m3u8
广西卫视 http://live.gslb.letv.com/gslb?stream_id=guangxi&tag=live&ext=m3u8
贵州卫视 http://live.gslb.letv.com/gslb?stream_id=guizhou&tag=live&ext=m3u8
河北卫视 http://live.gslb.letv.com/gslb?stream_id=hebei&tag=live&ext=m3u8
河南卫视 http://live.gslb.letv.com/gslb?stream_id=henan&tag=live&ext=m3u8
黑龙江卫视 http://live.gslb.letv.com/gslb?stream_id=heilongjiang&tag=live&ext=m3u8
湖北卫视 http://live.gslb.letv.com/gslb?stream_id=hubei&tag=live&ext=m3u8
吉林卫视 http://live.gslb.letv.com/gslb?stream_id=jilin&tag=live&ext=m3u8
江西卫视 http://live.gslb.letv.com/gslb?stream_id=jiangxi&tag=live&ext=m3u8
康巴卫视 rtmp://rtmp.sctv.com/SRT_Live/KBTV_N
辽宁卫视 http://live.gslb.letv.com/gslb?stream_id=liaoning&tag=live&ext=m3u8
内蒙古卫视 http://live.gslb.letv.com/gslb?stream_id=neimenggu&tag=live&ext=m3u8
宁夏卫视 http://live.gslb.letv.com/gslb?stream_id=ningxia&tag=live&ext=m3u8
青海卫视 http://live.gslb.letv.com/gslb?stream_id=qinghai&tag=live&ext=m3u8
山东教育卫视 http://live.gslb.letv.com/gslb?stream_id=sdjy&tag=live&ext=m3u8
山西卫视 http://live.gslb.letv.com/gslb?stream_id=shanxi&tag=live&ext=m3u8
四川卫视 http://live.gslb.letv.com/gslb?stream_id=sichuan&tag=live&ext=m3u8
天津卫视 http://live.gslb.letv.com/gslb?stream_id=tianjin&tag=live&ext=m3u8
西藏卫视 http://live.gslb.letv.com/gslb?stream_id=xizang&tag=live&ext=m3u8
新疆卫视 http://live.gslb.letv.com/gslb?stream_id=xinjiang&tag=live&ext=m3u8
云南卫视 http://live.gslb.letv.com/gslb?stream_id=yunnan&tag=live&ext=m3u8
浙江卫视 http://live.gslb.letv.com/gslb?stream_id=zhejiang&tag=live&ext=m3u8&sign=live_ipad
重庆卫视 http://live.gslb.letv.com/gslb?stream_id=chongqing&tag=live&ext=m3u8
珠江卫视 http://qqlive.rec.hdl.lxdns.com/1926463423.flv
享旅游 rtmp://live121.cnlive.com:1935/live/xianglvyou
新影视 rtmp://live121.cnlive.com:1935/live/xinyingshi
新闻 rtmp://live121.cnlive.com:1935/live/news
音乐 rtmp://live121.cnlive.com:1935/live/inyuetai
大舞台 rtmp://live121.cnlive.com:1935/live/dawutai
潮体育 rtmp://live121.cnlive.com:1935/live/chaotiyu
中国爱综艺 rtmp://live121.cnlive.com:1935/live/izongyi
中国车世界 rtmp://live121.cnlive.com:1935/live/cheshijie
美尚生活 rtmp://live121.cnlive.com:1935/live/meishangshenghuo
中国热动漫 rtmp://live121.cnlive.com:1935/live/dongman
中国娱乐 rtmp://live121.cnlive.com:1935/live/eyule
中国记录片 rtmp://live121.cnlive.com:1935/live/leihejilu
中国搏击天下 rtmp://live121.cnlive.com:1935/live/bojitianxia
法制中国 rtmp://live121.cnlive.com:1935/live/fazhizhongguo
世界小姐 rtmp://live121.cnlive.com:1935/live/shijiexiaojie
欢乐颂 rtmp://live121.cnlive.com:1935/live/shixunhuanlesong
美丽中国 rtmp://live121.cnlive.com:1935/live/meilizhongguo
美星写真 rtmp://live121.cnlive.com:1935/live/meixingxiezhen
冰火新中超 rtmp://live121.cnlive.com:1935/live/jidongpindao
爱show视听 rtmp://live121.cnlive.com:1935/live/aishow
微直播 rtmp://live121.cnlive.com:1935/live/@tv
微游戏 rtmp://live121.cnlive.com:1935/live/weiyouxi
火星视频 rtmp://live121.cnlive.com:1935/live/huoxingshipin
梦莱坞原创 rtmp://live121.cnlive.com:1935/live/menglaiwu
西安新闻综合 http://113.140.13.197/live/2011/03/03/DE9CFF32-E76C-11E0-BC3F-A10582DB7B64?fmt=x264_500k_flv
西安白鸽都市 http://113.140.13.197/live/2011/03/03/0B153AA2-657E-11E0-9BC4-8191976A168D?fmt=x264_500k_flv
西安健康快乐 http://113.140.13.197/live/2011/03/03/B24443C5-6597-11E0-9BC4-8191976A168D?fmt=x264_500k_flv
西安商务资讯 http://113.140.13.197/live/2011/03/03/9971EE63-657E-11E0-9BC4-8191976A168D?fmt=x264_500k_flv
西安文化影视 http://113.140.13.197/live/2011/03/03/678744E4-6597-11E0-9BC4-8191976A168D?fmt=x264_500k_flv
西安音乐综艺 http://113.140.13.197/live/2011/03/03/E3922EB6-6597-11E0-9BC4-8191976A168D?fmt=x264_500k_flv
北京生活频道 http://itv.hdpfans.com/live?vid=8
北京文艺频道 http://live.gslb.letv.com/gslb?stream_id=bjwy&tag=live&ext=m3u8&sign=live_ipad
卡酷动画 http://live.gslb.letv.com/gslb?stream_id=bjkaku&tag=live&ext=m3u8&sign=live_ipad
上海新闻综合 http://live-cdn1.smgbb.tv/channels/bbtv/xwzh/flv:sd/live
上海纪实频道 http://124.95.140.231/live/jspd/workflow1.m3u8
生活时尚 http://live-cdn1.smgbb.tv/channels/bbtv/xsk/flv:sd/live
上海外语频道 http://live.bestvcdn.net/live/wypd/01.m3u8
东方购物 http://live-cdn1.smgbb.tv/channels/bbtv/xjpd/flv:sd/live.flv
上海星尚频道 http://live.bestvcdn.net/live/shss/workflow1.m3u8
上海艺术人文 http://live.bestvcdn.net/live/ysrw/01.m3u8
上海娱乐频道 http://live-cdn1.smgbb.tv/channels/bbtv/ylpd/flv:sd/live
炫动卡通 http://live.gslb.letv.com/gslb?stream_id=xdkt&tag=live&ext=m3u8&sign=live_ipad
动漫秀场 http://itv.hdpfans.com/play?sinaid=125
魅力时装 rtmp://live4.cqnews.net:1935/live/TVFLV4
魅力音乐 http://218.202.219.67/channels/xjyx/MeiLiYinYue-Suma/flv:300k_flv
看看新闻网 http://live-cdn1.smgbb.tv/channels/bbtv/kkxww/flv:sd/live
重庆新闻频道 rtmp://live5.cqnews.net/live/TVFLV13
重庆都市频道 rtmp://live5.cqnews.net/live/TVFLV11
重庆公共频道,rtmp://live5.cqnews.net:1935/live/TVFLV12
重庆科教频道 http://live1.cqnews.net/ts/111/111.m3u8
重庆魅力时装 http://live1.cqnews.net/ts/103/103.m3u8
重庆汽摩频道 rtmp://live4.cqnews.net/live/TVFLV5
重庆少儿频道 rtmp://live4.cqnews.net/live/TVFLV6
重庆时尚频道 rtmp://live4.cqnews.net/live/TVFLV8
重庆移动频道 rtmp://live5.cqnews.net/live/TVFLV1
重庆娱乐频道 http://123.147.168.27/yule/z.m3u8
重庆汽摩频道 rtmp://live4.cqnews.net:1935/live/TVFLV5
重庆手持电视 rtmp://123.147.160.15:1935/rtplive/scds.sdp
金鹰卡通 http://live.gslb.letv.com/gslb?stream_id=jykt&tag=live&ext=m3u8&sign=live_ipad
宝贝家 http://123.147.124.8/PLTV/88888888/224/3221226001/10000100000000060000000000083514_0.smil
长沙公共频道 rtmp://220.169.44.38/live/cstv5_800k
长沙嘉丽购物 rtmp://220.169.44.38/live/cstv6_800k
长沙经贸频道 rtmp://220.169.44.38/live/cstv4_800k
长沙女性频道 rtmp://220.169.44.38/live/cstv3_300k
长沙新闻频道 rtmp://220.169.44.38/live/cstv1_300k
长沙政法频道 rtmp://220.169.44.38/live/cstv2_300k
株洲法制 rtmp://media.zzbtv.com:1935/live/fz_mc43
株洲公共民生 rtmp://media.zzbtv.com:1935/live/gg_mc43
株洲新闻综合 rtmp://media.zzbtv.com:1935/live/xw_mc43
珠海第一频道 http://tsl5.hls.cutv.com/cutvlive/zZGtroL/hls/live_sd.m3u8
珠海第二频道 http://tsl5.hls.cutv.com/cutvlive/HtyU8ou/hls/live_sd.m3u8
惠州1台 http://live.hzrtv.cn/live/ac39790593344822b289c7af8c25bc9e?fmt=H264_800k_flv
惠州2台 http://live.hzrtv.cn/live/2da8f89ad3974579b573d45f73340d4a?fmt=H264_800K_flv
邮轮旅游 http://web-play.pptv.com/web-m3u8-300425.m3u8
广西综艺频道 http://222.216.111.87:5100/nn_live.flv?id=1005
广西都市频道 http://222.216.111.87:5100/nn_live.flv?id=1006
广西公共频道 http://222.216.111.87:5100/nn_live.flv?id=1000
广西国际频道 http://222.216.111.87:5100/nn_live.flv?id=1003
广西乐思购频道 http://222.216.111.87:5100/nn_live.flv?id=1002
广西影视频道 http://222.216.111.87:5100/nn_live.flv?id=1001
广西资讯频道 http://222.216.111.87:5100/nn_live.flv?id=1007 
梧州综合 rtmp://202.103.212.146/livepkgr/live1 
河北都市频道 rtmp://221.192.130.239:554/live/tv03
河北公共频道 rtmp://221.192.130.239:554/live/tv06
河北经济频道 rtmp://221.192.130.239:554/live/tv02
河北少儿科教 rtmp://221.192.130.239:554/live/tv05
河北影视频道 rtmp://221.192.130.239:554/live/tv04
石家庄都市频道 http://live.sjzntv.cn/channels/sjzntv/video_channel_04/flv:800k/live
石家庄生活频道 http://live.sjzntv.cn/channels/sjzntv/video_channel_03/flv:500k/live
石家庄新闻综合 http://live.sjzntv.cn/channels/sjzntv/video_channel_01/flv:800k/live
石家庄娱乐频道 http://live.sjzntv.cn/channels/sjzntv/video_channel_02/flv:500k/live
黑龙江都市 http://125.211.216.199/channels/hljtv/dspd/flv:sd/live
黑龙江公共 http://125.211.216.199/channels/hljtv/ggpd/flv:sd/live
黑龙江考试频道 http://125.211.216.199/channels/hljtv/kspd/flv:sd/live
黑龙江龙传媒 http://125.211.216.199/channels/hljtv/sjpd/flv:sd/live
黑龙江少儿 http://live2.hljtv.com/channels/hljtv/dqpd/flv:hd/live
黑龙江文艺 http://125.211.216.199/channels/hljtv/wypd/flv:sd/live
黑龙江新闻 http://125.211.216.199/channels/hljtv/fzpd/flv:sd/live
黑龙江影视 http://live2.hljtv.com/channels/hljtv/yspd/flv:hd
七台河电视 rtmp://vod.qthtv.com:1935/live/live2
湖北综合 http://tsl5.hls.cutv.com/cutvlive/nQ8zN75/hls/live_sd.m3u8
湖北影视 http://tsl5.hls.cutv.com/cutvlive/8cinuTa/hls/live_sd.m3u8
湖北体育生活 http://tsl5.hls.cutv.com/cutvlive/tjiVQ2w/hls/live_sd.m3u8
美嘉购物 http://live9.hbtv.com.cn/channels/zbk/mjgw/flv:sd/live
碟市 http://live9.hbtv.com.cn/channels/zbk/dspd/flv:sd/live
龚上频道 http://59.175.153.186/channels/zbk/gspd/flv:sd/live
孕育指南 http://live9.hbtv.com.cn/channels/zbk/yyzl/flv:sd/live
职业指南 http://live9.hbtv.com.cn/channels/zbk/zyzl/flv:sd/live
武汉新闻 http://tsl2.hls.cutv.com/cutvlive/jfDyuzx/hls/live_sd.m3u8
武汉少儿 http://tsl2.hls.cutv.com/cutvlive/vnLNskj/hls/live_sd.m3u8
巴东新闻综合 http://live.bdntv.cn/channels/bdtv/bdxw/flv:sd/live
随州综合 rtmp://221.235.153.58/live/zb
昆山新闻 http://stream.ksntv.com.cn/live/pc1?fmt=H264_450K_FLV
吉林东北戏曲 rtmp://video1.jlntv.cn:1935/dvrlive/dbxq
吉林都市频道 rtmp://video.jlntv.cn:1935/live/dspd
吉林法制频道 rtmp://video.jlntv.cn:1935/live/fzpd
吉林公共频道 rtmp://video.jlntv.cn:1935/live/ggpd
吉林篮球频道 rtmp://video1.jlntv.cn:1935/dvrlive/lqpd
吉林生活频道 rtmp://video.jlntv.cn:1935/dvrlive/shpd
吉林乡村频道 rtmp://video1.jlntv.cn:1935/dvrlive/xcpd
吉林影视频道 rtmp://video.jlntv.cn:1935/dvrlive/yspd
南京新闻综合 http://live.xwei.tv/channels/njtv/video_xwzh/flv:500k/live
南京教科频道 http://live.xwei.tv/channels/njtv/video_kj/flv:500k/live
南京少儿频道 http://live.xwei.tv/channels/njtv/video_shaoer/flv:500k/live
南京生活频道 http://live.xwei.tv/channels/njtv/video_shh/flv:500k/live
南京十八频道 http://live.xwei.tv/channels/njtv/video_18/flv:500k/live
南京娱乐频道 http://live.xwei.tv/channels/njtv/video_yl/flv:500k/live
连云港综合 rtmp://122.192.35.76:554/live/tv80
连云港影视综艺 rtmp://218.92.16.152:554/live/tv03
淮安公共频道 rtmp://218.2.25.44:1935/live/hagg
淮安新闻综合 rtmp://218.2.25.44:1935/live/live
淮安影视娱乐 rtmp://218.2.25.44:1935/live/test
南通江海明珠 http://media.ntjoy.com/channels/nttv/JHMZ/flv:mz/live
南通社教频道 http://media.ntjoy.com/channels/nttv/society/flv:sd/live
南通生活频道 http://media.ntjoy.com/channels/nttv/life/flv:sd/live
南通新闻频道 http://media.ntjoy.com/channels/nttv/news/flv:sd/live
南通信息频道 http://media.ntjoy.com/channels/nttv/information/flv:sd/live
如东生活频道 http://live.rdxmt.com/channels/rudong/shfw/flv:sd/live
如东新闻频道 http://live.rdxmt.com/channels/rudong/news/flv:sd/live
如东影视频道 http://live.rdxmt.com/channels/rudong/yswh/flv:sd/live
苏州社会经济 rtmp://streamrtsp.2500city.com/suzhoutv/livetv23
苏州文化生活 rtmp://streamrtsp.2500city.com/suzhoutv/livetv33
苏州生活资讯 rtmp://streamrtsp.2500city.com/suzhoutv/livetv53
苏州新闻综合 rtmp://122.192.35.80:554/live/tv60
无锡移动频道 http://live2.thmz.com/channels/iptv/wxmtv/flv:sd/.flv
无锡都市资讯 http://live2.thmz.com/channels/iptv/wxtv3/flv:sd/.flv
无锡经济频道 http://live2.thmz.com/channels/iptv/wxtv5/flv:sd/.flv
无锡生活频道 http://live2.thmz.com/channels/iptv/wxtv4/flv:sd/.flv
无锡新闻综合 http://live2.thmz.com/channels/iptv/wxntv/flv:sd/.flv
无锡娱乐频道 http://live2.thmz.com/channels/iptv/wxtv2/flv:sd/.flv
盐城城市公共 http://live.0515yc.tv/channels/yctv/yc2/flv:sd/live
盐城新闻综合 http://live.0515yc.tv/channels/yctv/yc3/flv:sd/live
盐城综合频道 rtmp://122.192.35.80:554/live/tv62
张家港社会生活 rtmp://3gvod.zjgonline.com.cn:1935/live/shehuishenghuo_300
张家港新闻综合 rtmp://3gvod.zjgonline.com.cn:1935/live/xinwenzonghe_300
宿迁综合频道 rtmp://122.192.35.76:554/live/tv81
徐州综合频道 rtmp://122.192.35.80:554/live/tv73
镇江综合频道 rtmp://122.192.35.76:554/live/tv72
九江一台 rtmp://bbs.jjcmw.cn/live/jjtv1
九江二台 rtmp://bbs.jjcmw.cn/live/jjtv2
九江三台 rtmp://bbs.jjcmw.cn/live/jjtv3
九江四台 rtmp://bbs.jjcmw.cn/live/jjtv4
南昌都市频道 rtmp://w.ncnews.com.cn:1935/live/nctv2
南昌公共频道 rtmp://w.ncnews.com.cn:1935/live/nctv4
南昌新闻综合 rtmp://w.ncnews.com.cn:1935/live/nctv1
南昌资讯政法 rtmp://w.ncnews.com.cn:1935/live/nctv3
宜春新闻 rtmp://live.ycstv.com:1935/live/live3
宁夏公共频道 rtmp://218.95.143.34/live/dslivehd10
宁夏经济频道 rtmp://218.95.143.35/live/dslivehd11
宁夏少儿频道 rtmp://218.95.143.35/live/dslivehd12
山东农科频道 rtmp://112.231.23.27:554/live/sdnk
山东齐鲁频道 rtmp://112.231.23.27:554/live/sdql
山东少儿频道 rtmp://112.231.23.27:554/live/sdse
山东生活频道 rtmp://112.231.23.27:554/live/sdsh
山东体育频道 rtmp://112.231.23.27:554/live/sdty
山东影视频道 rtmp://112.231.23.27:554/live/sdys
山东综艺频道 rtmp://112.231.23.27:554/live/sdzy
济南都市 http://tsl3.hls.cutv.com/cutvlive/p0hcE91/hls/live_sd.m3u8
济南少儿 http://tsl3.hls.cutv.com/cutvlive/1Ki5v7w/hls/live_sd.m3u8
济南生活 http://tsl3.hls.cutv.com/cutvlive/thfDjK5/hls/live_sd.m3u8
济南影视 http://tsl3.hls.cutv.com/cutvlive/kdgtM0l/hls/live_sd.m3u8
济南娱乐 http://tsl3.hls.cutv.com/cutvlive/xcisTAf/hls/live_sd.m3u8
乐享理财 rtmp://112.231.23.27:554/live/sdtvch01
乐享动漫 rtmp://112.231.23.27:554/live/*
乐享健康 rtmp://112.231.23.27:554/live/tv009
乐享旅游 rtmp://112.231.23.27:554/live/tv007
乐享笑林 rtmp://112.231.23.27:554/live/tv27
脱口秀 rtmp://112.231.23.27:554/live/tv25
东营公共 rtmp://live.hhek.cn:1935/live/live1
东营综合 rtmp://live.hhek.cn:1935/live/live2
泰山频道 rtmp://112.231.23.27:554/live/sdgj
泰州新闻综合 rtmp://122.192.35.76:554/live/tv71
威海公共频道 rtmp://olive.weihai.tv/live/msysnew
威海新闻综合 rtmp://olive.weihai.tv/live/xwzh
四川财经频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV3_N
四川文化旅游 rtmp://rtmp.sctv.com/SRT_Live/SCTV2_N
四川新闻资讯频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV4_N
四川影视文艺频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV5_N
四川星空购物频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV6_N
四川妇女儿童频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV7_N
四川公共频道 rtmp://rtmp.sctv.com/SRT_Live/SCTV9_N
四川科教 rtmp://rtmp.sctv.com/SRT_Live/SCJY_N
四川星空城市 rtmp://rtmp.sctv.com/SRT_Live/STAR_CITY_N
四川星空移动 rtmp://rtmp.sctv.com/SRT_Live/STAR_MOBILE_N
成都都市生活 http://itv.hdpfans.com/play?cdtvid=cdtv3
成都公共频道 http://itv.hdpfans.com/play?cdtvid=cdtv5
成都经济资讯 http://itv.hdpfans.com/play?cdtvid=cdtv2
成都少儿频道 http://itv.hdpfans.com/play?cdtvid=cdtv6
成都新闻综合 http://itv.hdpfans.com/play?cdtvid=cdtv1
成都影视文艺 http://itv.hdpfans.com/play?cdtvid=cdtv4
自贡公共频道 rtmp://live.zgbctv.com/zgtv/m11
自贡综合频道 rtmp://live.zgbctv.com/zgtv/m10
滨海一套 http://biz.vsdn.tv380.com/playlive.php?5B63686E5D445830303030303034367C343730317C317C313030307C54545A582E54567C687474707C74735B2F63686E5DVSDNSOOONERCOM00
滨海二套 http://biz.vsdn.tv380.com/playlive.php?5B63686E5D445830303030303034367C343636387C317C313030307C54545A582E54567C687474707C74735B2F63686E5DVSDNSOOONERCOM00
云南都市频道 rtmp://119.62.72.211:554/live/tv01
云南生活资讯,rtmp://119.62.72.211:554/live/tv02
云南五台 rtmp://119.62.72.211:554/live/tv04
云南娱乐频道 rtmp://119.62.72.211:554/live/tv03
昆明一台 rtmp://119.62.72.211:554/live/tv17
昆明二台 rtmp://119.62.72.211:554/live/tv05
昆明三台 rtmp://119.62.72.211:554/live/tv18
昆明四台 rtmp://119.62.72.211:554/live/tv16
昆明五台 rtmp://119.62.72.211:554/live/tv15
昆明六台 rtmp://119.62.72.211:554/live/tv06
玉溪公共 rtmp://old.yxtv.cn/live/live184
浙江公共频道 http://r1.cztv.com/channels/107/500.flv/live
浙江国际频道 http://r1.cztv.com/channels/110/500.flv/live
浙江经视频道 http://r1.cztv.com/channels/103/500.flv/live
浙江科教频道 http://r1.cztv.com/channels/104/500.flv/live
浙江民生频道 http://r1.cztv.com/channels/106/500.flv/live
浙江钱江频道 http://r1.cztv.com/channels/102/500.flv/live
浙江少儿频道 http://r1.cztv.com/channels/108/500.flv/live
浙江影视频道 http://r1.cztv.com/channels/105/500.flv/live
浙江手机台 http://c4.cztv.com/channels/304/512.flv/live
留学世界 http://r1.cztv.com/channels/109/500.flv/live
杭州新闻 http://live2.hoolo.tv/channels/hoolo/htv1/flv:sd/live
杭州导视记录 http://live2.hoolo.tv/channels/hoolo/954/flv:sd/live
杭州明珠 http://live2.hoolo.tv/channels/hoolo/htv2/flv:sd/live
杭州少儿频道 http://live2.hoolo.tv/channels/hoolo/htv5/flv:sd/live
杭州生活频道 http://live2.hoolo.tv/channels/hoolo/htv3/flv:500k_stream/live
杭州影视频道 http://live2.hoolo.tv/channels/hoolo/htv4/flv:500k_stream/live
义乌1新闻综合 http://live-01.ywcity.cn/channels/ywtv/video_channel_01/flv:sd/live
义乌2商贸频道 http://live-01.ywcity.cn/channels/ywtv/video_channel_02/flv:sd/live
义乌3公共文艺 [http://live-01.ywcity.cn/channels/ywtv/video_channel_03/flv:sd/live](http://live-01.ywcity.cn/channels/ywtv/video_channel_03/flv:sd/live)
长兴1综合 rtmp://live.d5media.net:554/live/cxtv01s1
长兴2文化 rtmp://live.d5media.net:554/live/cxtv02s1
绍兴县新闻 rtmp://www.scbtv.cn/live/new
安徽公共频道 http://livecdn.ahtv.cn/channels/1506/500.flv/live
安徽国际频道 http://livecdn.ahtv.cn/channels/1508/500.flv/live
安徽综艺频道 http://livecdn.ahtv.cn/channels/1504/500.flv/live
安徽经济频道 http://livecdn.ahtv.cn/channels/1503/500.flv/live
安徽科教频道 http://livecdn.ahtv.cn/channels/1505/500.flv/live
安徽人物频道 http://livecdn.ahtv.cn/channels/1507/500.flv/live
安徽影视频道 http://livecdn.ahtv.cn/channels/1502/500.flv/live
芜湖徽商频道 http://61.191.214.229/channels/1533/500.flv/live
芜湖生活频道 http://61.191.214.229/channels/1532/500.flv/live
芜湖新闻综合 http://61.191.214.229/channels/1531/500.flv/live
蚌埠生活频道 rtmp://vod.ahbbtv.com:1935/live/shpd1
蚌埠新闻频道 rtmp://vod.ahbbtv.com:1935/live/xwpd1
池州公共频道 rtmp://60.174.36.89:1935/live/vod3
池州新闻频道 rtmp://60.174.36.89:1935/live/vod4
淮北公共频道 rtmp://vod.0561rtv.com:1935/live/live2
淮北教育频道 rtmp://vod.0561rtv.com:1935/live/live3
淮北新闻频道 rtmp://vod.0561rtv.com:1935/live/live1
淮北影视综艺 rtmp://vod.0561rtv.com:1935/live/live4
蓉城先锋 http://itv.hdpfans.com/play?cdtvid=party
海南少儿 http://live.64ma.com/livePlay.asp?sohuId=66
新疆教育 http://218.202.219.67/channels/xjyx/XETV-Envivio/flv:300k_rtmp
新疆体育 http://218.202.219.67/channels/xjyx/XJTV10-Envivio/flv:300k_rtmp
乌鲁木齐1台 http://218.202.219.67/channels/xjyx/UTV1-Envivio/flv:300k_rtmp
乌鲁木齐2台 [http://218.202.219.67/channels/xjyx/UTV2-Envivio/flv:300k_rtmp](http://218.202.219.67/channels/xjyx/UTV2-Envivio/flv:300k_rtmp)
韩国电影频道 http://ladytv.tv/channels/39/800.flv/live
韩国女士频道 http://ladytv.tv/channels/1111/800.flv/live
MBN rtmp://lm02.everyon.tv:1935/ptv2/phd17
MTN rtmp://lm02.everyon.tv:1935/ptv2/phd18
韩002 rtmp://lm01.everyontv.net/ptv2/phd2
韩003 rtmp://lm01.everyontv.net/ptv2/phd3
韩004 rtmp://lm01.everyontv.net/ptv2/phd4
韩005 rtmp://lm01.everyontv.net/ptv2/phd5
韩006 rtmp://lm01.everyontv.net/ptv2/phd6
韩007 rtmp://lm01.everyontv.net/ptv2/phd7
韩008 rtmp://lm01.everyontv.net/ptv2/phd8 
韩013 rtmp://lm01.everyontv.net/ptv2/phd13
韩014 rtmp://lm01.everyontv.net/ptv2/phd14
韩015 rtmp://lm01.everyontv.net/ptv2/phd15
韩016 rtmp://lm01.everyontv.net/ptv2/phd16
韩018 rtmp://lm01.everyontv.net/ptv2/phd18
韩019 rtmp://lm01.everyontv.net/ptv2/phd19
韩021 rtmp://lm01.everyontv.net/ptv2/phd21
韩023 rtmp://lm01.everyontv.net/ptv2/phd23
韩024 rtmp://lm01.everyontv.net/ptv2/phd24
美国中文电视 rtmp://174.120.145.146/live/livestream
泰国国际中文 rtmp://61.90.153.168:1935/live/myStream
松视1台 rtsp://59.124.220.46:554/live/tv1.stream 
松视2台 rtsp://59.124.220.46:554/live/tv2.stream 
松视3台 rtsp://59.124.220.46:554/live/tv3.stream 
松视4台 rtsp://59.124.220.46:554/live/tv4.stream 
兔子CR台 rtmp://lm02.everyon.tv:1935/ptv/phd499 
花花公子CR台 rtmp://lm02.everyon.tv:1935/ptv2/phd497
韩国CR1台 rtsp://lm02.everyon.tv:1935/ptv2/phd501
韩国CR2台 rtsp://lm02.everyon.tv:1935/ptv/phd59
韩国CR3台 rtsp://lm02.everyon.tv:1935/ptv/phd60
韩国CR4台 rtsp://lm02.everyon.tv:1935/ptv/phd61
韩国CR5台 rtsp://lm02.everyon.tv:1935/ptv/phd62
韩国CR6台 rtsp://lm02.everyon.tv:1935/ptv/phd63
韩国CR7台 rtsp://lm01.everyon.tv:1935/ptv/phd64
欧美*CR1 rtmp://live190.la3.origin.filmon.com:1935/live/73.high.stream
欧美*CR2 rtmp://live190.la3.origin.filmon.com:1935/live/198.high.stream
欧美*CR3 rtmp://live190.la3.origin.filmon.com:1935/live/244.high.stream
欧美*CR4 rtmp://live190.la3.origin.filmon.com:1935/live/245.high.stream
欧美*CR5 rtmp://live190.la3.origin.filmon.com:1935/live/246.high.stream
欧美*CR6 rtmp://live190.la3.origin.filmon.com:1935/live/247.high.stream
亚洲CR电影1台 http://trailers.asiamoviepass.eu/amp/hi/bf-086.flv
亚洲CR电影2台 http://trailers.asiamoviepass.eu/amp/hi/avgl-027a.flv
亚洲CR电影3台 http://trailers.asiamoviepass.eu/amp/hi/day-018.flv
亚洲CR电影4台 http://trailers.asiamoviepass.eu/amp/hi/midd-576.flv
亚洲CR电影5台 http://trailers.asiamoviepass.eu/amp/hi/avgp-044.flv
亚洲CR电影6台 http://trailers.asiamoviepass.eu/amp/hi/avgl-030.flv
亚洲CR电影7台 http://trailers.asiamoviepass.eu/amp/hi/avgl-111a.flv
亚洲CR电影8台 http://trailers.asiamoviepass.eu/amp/hi/ezd262_Trimmed_1.flv
亚洲CR电影9台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/midd-573.flv
亚洲CR电影10台 http://trailers.asiamoviepass.eu/amp/hi/edd-092.flv
亚洲CR电影11台 http://trailers.asiamoviepass.eu/amp/hi/ifdva-034.flv
亚洲CR电影12台 http://trailers.asiamoviepass.eu/amp/hi/bbi-014.flv
亚洲CR电影13台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/avgl-128.flv
亚洲CR电影14台 http://trailers.asiamoviepass.eu/amp/hi/iesp-265.flv
亚洲CR电影15台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/kawd-264.flv
亚洲CR电影16台 http://trailers.asiamoviepass.eu/amp/hi/ddk-020.flv
亚洲CR电影17台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/juc-325.flv
亚洲CR电影18台 http://trailers.asiamoviepass.eu/amp/hi/avgl-004.flv
亚洲CR电影19台 http://trailers.asiamoviepass.eu/amp/hi/dandy-145.flv
亚洲CR电影20台 http://trailers.asiamoviepass.eu/amp/hi/dv1118.flv
亚洲CR电影21台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/bbi103.flv
亚洲CR电影22台 http://trailers.asiamoviepass.eu/amp/hi/jme-live/adz216.flv
亚洲CR电影23台 http://trailers.asiamoviepass.eu/amp/hi/iesp-263.flv
亚洲CR电影24台 http://trailers.asiamoviepass.eu/amp/hi/sdms736b.flv
亚洲CR电影25台 [http://trailers.asiamoviepass.eu/amp/hi/jme-live/avgl-149.flv](http://trailers.asiamoviepass.eu/amp/hi/jme-live/avgl-149.flv)
## [Vitamio 3.0 新手入门](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN)
- [hellojane96](http://vitamio.org/hellojane96?locale=zh-CN)
- 2013-1-15
- [Vitamio3.0](http://vitamio.org/pages/tag/vitamio3-dot-0?locale=zh-CN)
Vitamio项目发展迅猛，官方群突破400位开发者，基于Vitamio的项目突破1000个，使用Vitamio提供优质播放体验的用户超过5000万。也欢迎大家分享你的Vitamio使用经验，本文将进一步介绍Vitamio的简单使用方法。
### 下载 [](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN#toc-xia-zai)
目前Vitamio的项目托管在Github上面：[https://github.com/yixia](https://github.com/yixia) ，这里有很多公司的开源项目，其中：
1、VitamioBundle是Vitamio核心插件，（大家可以搜一下"Android Library"这个关键字，和jar差不多的用途），可以方便集成到项目中。（注意：本文使用tag里面的3.0版本）
2、VitamioDemo是Vitamio的官方例子。
### 简介 [](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN#toc-jian-jie)
Vitamio的中文名称为“维他蜜”
Vitamio 是一款 Android 平台上的全能多媒体开发框架。Vitamio 凭借其简洁易用的 API 接口赢得了全球众多开发者的青睐。到目前，全球已经有超过 1000 种应用在使用 Vitamio，覆盖用户超过 5000 万。
Vitamio 能够流畅播放720P甚至1080P高清MKV，FLV，MP4，MOV，TS，RMVB等常见格式的视频，还可以在 Android 上支持 MMS, RTSP, RTMP, HLS(m3u8) 等常见的多种视频流媒体协议，包括点播与直播。
支持 ARMv6 和 ARMv7 两种 ARM CPU，同时对 VFP, VFPv3, NEON 等指令集都做相应优化。
支持 Android 2.1+ 系统，支持超过 95% 的 Android 市场。同时 Android 2.1 之前的系统也基本支持，不过没做详细测试。
更多[Vitamio的介绍](http://vitamio.org/pages/whats-vitamio?locale=zh-CN)参照这里。
### 导入使用 [](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN#toc-dao-ru-shi-yong)
1、导入。下载回来后大家可能发现没有.project工程文件，可以通过File -> Import -> Android -> Existing Android Code Into Workspace来导入工程，然后改一下工程名称即可。
2、将VitamioBundle工程作为Android Library引入Demo工程使用即可。
### 关注Vitamio [](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN#toc-guan-zhu-vitamio)
官方微博：[http://weibo.com/vitamio](http://weibo.com/vitamio)
官方网站/论坛：vitamio.org
Vitamio QQ 3群:283274315
### 其他 [](http://vitamio.org/pages/vitamio-3-dot-0-novice-tutorial?locale=zh-CN#toc-qi-ta)
1、官方建议以Android Library方式使用Vitamio插件，以便后续方便升级。如果需要拷贝集成到一个工程，可能会报错找不到io.vov.vitamio.R.raw.libarm（硬编码导致的问题）
2、 Vitamio最新版本为3.0，极力推荐使用新版本。
3、 基于Vitamio仅支持ARMv6+以上的CPU，95%以上的视频格式支持，说明：
a). 无法播放的问题。使用VPlayer来测试链接，如果VPlayer没有问题那Vitamio肯定也没有问题。
b). 各种找不到so文件的情况只有两种情况：不支持设备、没有执行解压解码包。
4、Vitamio是免费的，没有商业版本，也没有Vitamio.apk。
5、Vitamio是[Cedric Fung](http://vec.io/)开发编写，同时他也是VPlayer创始人，Vitamio最终所有权为炫一下（北京）科技有限公司。
本文有三个意图：公布Vitamio使用情况、公布官方QQ群3、提供可直接运行的示例工程。继续努力的推动Vitamio事业，欢迎大家使用和支持！
1.1 Vitamio是什么？
Vitamio是Android平台视音频播放组件，支持播放几乎格式的视频以及主流网络视频流（http/rtsp/mms等）。
Vitamio官网：[http://vov.io/vitamio/](http://www.douban.com/link2?url=http%3A//vov.io/vitamio/)
Vitamio微博：[http://weibo.com/vitamio](http://www.douban.com/link2?url=http%3A//weibo.com/vitamio)
VPlayer官网：[http://vplayer.net](http://www.douban.com/link2?url=http%3A//vplayer.net)
Vitamio豆瓣：[http://site.douban.com/145815/](http://www.douban.com/link2?url=http%3A//site.douban.com/145815/)
1.2 关于Vitamio的名字
Vitamio 来源于英文单词Vitamin（维他命），希望Vitamio对于安卓多媒体开发者的重要性，就像Vitmain之于人体一样不可缺少。 
The name of Vitamio comes from Vitamin, We hope the importance of Vitamio for Android multimedia developers as indispensable as Vitamin for human.
1.3 关于Vitamio的由来
我们是一拨葱名勤劳的Coder，我们憎恶烦冗的Android多媒体程序开发，从版本升级到不同设 备，刚坑娘的测试完三星,htc,sony，操，又拿来了v880；我们认为这些讨厌的玩意注定会让我们的生活变得愚蠢，新技术神马的也是没有时间去研究 ，最令人发指的是不能专注于实现自己的创意APP。
经常沉溺于拯救世界的精神世界，平凡的Coder也能变奇葩。
作为最早研究Android媒体工具的团队，我们决定把我们的Vitamio框架免费出去，助力安卓多媒体开发者实现自己的创意梦想 :D
各位少年，我们是来自中国的高品质多媒体框架噢。 
第一版发布日期：2011年9月发布
1.4 Vitamio相关数据
从2011年9月发布至今,用户下载量超过2000W，使用Vitamio的开发者超过1500位，使用过Vitamio产品有
国内：搜狐视频，91熊猫影音（网龙），海底捞，歌华有限 等
国外：Pak TV Global,Madani Channel,Deen TV,myTRANS,FPlayer,AnimeFTW.tv,Radio Nepali,GNC LiveWell,MagicTV Streaming Player Free，Hispan TV Mobile，TV Italy等 
1.5 Vitamio有什么优点？
a). 强大。支持超多格式视频和网络视频播放。（不强大免费也没用，所以排在第一位）
b). 免费。个人、企业均免费使用，短时间内无任何收费计划。
（Google Play上已有多款基于Vitamio的收费播放器，大家可以在里面搜索Vitamio关键字）
c). 无缝集成。仅将Vitamio的Library工程引入即可使用，无需另外下载安装解码包（Vitamio的上一个版本是需要单独下载安装解码包）。
d). 使用简单。调用非常简单，方便使用。
e). 自由定制。播放界面的代码已完全开放，方便自定义播放界面、进度条等。
d). 持续更新。2012-7-9已发布新一版的内测版本，预计8月初能发布下一个版本。
e). 服务跟进。Vitamio官方QQ群（246969281），提供开发者交流和讨论。论坛还在开发中。
1.6 Vitamio背后的团队
Vitamio与VPlayer属同一个团队、同一个公司（yixia.com）。我们在北京，目前团队有5名成员，4名开发，1名设计师，以开发VPlayer为主，Vitamio为辅 :)
1.7 Vitamio与VPlayer什么关系？ 
VPlayer基于Vitamio开发，所以VPlayer能播放的Vitamio也能。 
1.8 Vitamio与ffmpeg
FFmpeg提供软件解码器和多路输出(demuxers)。Vitamio使用LGPLv2.1许可下FFmpeg的代码，代码可以从这里下载。
[https://bitbucket.org/ABitNo/ffmpegandroid](http://www.douban.com/link2?url=https%3A//bitbucket.org/ABitNo/ffmpegandroid)
二、Vitamio下载和运行
2.1 下载
[Vitamio 2.0](http://www.douban.com/link2?url=http%3A//files.cnblogs.com/over140/vitamio2.0.7z)下载。(2012-8-31更新)
2.2 运行例子
解压后两个项目：VitamioBundle和Test，其中VitamioBundle是Library（关于Android Library参见这里），运行Test例子即可。注意：
1). 例子使用API 16，没有安装升级最新Android 4.1 会报错，大家设置里降低至API 14(Android 4.0) 或API 15(Android 4.0.3) 即可。
2). 工程文件并没有gen文件夹，请大家自行加上。
3). 先编译VitamioBundle，后编译Test。如果仍然有错误，把工程Clean一下，依次再编译一下。
4). 修改VideoViewDemo视频路径为你要测试的视频路径。注意，本地视频用setVideoPath方法，播放网络视频用setVideoURI方法。
2.3 整合例子 
不希望以Library的方式引用Vitamio，可以直接把VitamioBundle里所有的类、资源拷贝至目标项目。注意src、libs、res目录下的都要拷贝过去，还有AndroidManifest.xml中的配置。
成功整合例子，大家可以看一下OPlayer的结构。
2.4 自定义InitActiviy，即正在解压解码包进度条界面。
         这里给出InitActivity代码： 
复制代码
public class InitActivity extends Activity {
    public static final String FROM_ME = "fromVitamioInitActivity";
    public static final String EXTRA_MSG = "EXTRA_MSG";
    public static final String EXTRA_FILE = "EXTRA_FILE";
    private ProgressDialog mPD;
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        new AsyncTask<Object, Object, Object>() {
            protected void onPreExecute() {
                mPD = new ProgressDialog(InitActivity.this);
                mPD.setCancelable(false);
                mPD.setMessage(getString(getIntent().getIntExtra(EXTRA_MSG, R.string.vitamio_init_decoders)));
                mPD.show();
            }
            @Override
            protected Object doInBackground(Object... params) {
                VitamioConnection.initNativeLibs(getApplicationContext(), getIntent().getIntExtra(EXTRA_FILE, R.raw.libarm), new VitamioConnection.OnNativeLibsInitedListener()
 {
                    @Override
                    public void onNativeLibsInitCompleted(String libPath) {
                        Log.d("Native libs inited at " + libPath);
                        uiHandler.sendEmptyMessage(0);
                    }
                });
                return null;
            }
        }.execute();
    }
    private Handler uiHandler = new Handler() {
        public void handleMessage(Message msg) {
            mPD.dismiss();
            Intent src = getIntent();
            Intent i = new Intent();
            i.setClassName(src.getStringExtra("package"), src.getStringExtra("className"));
            i.setData(src.getData());
            i.putExtras(src);
            i.putExtra(FROM_ME, true);
            startActivity(i);
            finish();
        }
    };
}
复制代码
         代码说明:
主要是VitamioConnection.initNativeLibs，大家自己写一个初始化的界面即可，线程里调用这个代码即可。 
三、Vitamio问题汇总
3.1 官网怎么打不开？
由于vplayer.net、vov.io等几个网站都是我们部门自己维护的，目前就一人再改，出点问题来不及弄，你懂的。 
3.2 case : 为什么一直停在正在解码的解码？
 case : Couldn't load vinit: findLibrary returned null 
 case : Not allowed to bind to service Intent (act=io.vov.vitmio.IVitamioService) 
 case : io.vov.vitmio.VitamioInstaller$VitamioNotFoundException 
 case : java.lang.UnsatisfiedLinkError: Library /data/data/null/libs/libvplaer.so not found 
 defalut : //各种运行不了的问题
return 基本就一个问题，2012-7-9发布的Vitamio测试版本无法共存的问题。无法与VPlayer共存、无法与其他Vitamio共存，需要卸载干净了。注意改一下包名(package)就是一个新的APK，需要把之前的卸载掉。
3.3 视频/视频流(rtp、rtsp等)播放不了。 
可能存在以下几种情况：
a). 视频本身就存在问题，你可以用其他播放器播放一下，是否能正常播放。
b). 本地网速不给力/设备本事配置过低，这个没办法
c). 视频源卡，本事带宽不够，直接访问也很慢，这个也没办法
d). 用VPlayer与Vitamio同样都有问题，而其他播放器没事，这个可能是我们的问题，我们还在持续改进，尤其是播在线视频这部分，这个可以等新版发布 :)
e). 等不及了，那就把视频测试地址和简要说明发给我们（vplayer@yixia.com），我们尽可能的抽时间来回复，感谢！ 
3.4 Vitamio最低支持的Android版本
目前我们在运行的项目最低支持Android 2.1，示例工程选的Android API Level 是4.1，但不代表只支持4.0以上，大家可从AndroidManifest.xml中看得出：
<uses-sdk android:minSdkVersion="7" android:targetSdkVersion="15" />
如果编译无法运行的话，把VitamioBundle项目下res/values-v11和values-v14删掉即可，其他部分稍作改动即可。 
3.5 为什么会弹出提示框要求安装Vitamio Plugin
因为你下载和使用的是旧版的Vitamio，请卸载干净后下载2012-7-9日发布的版本，请看本文 2.1 下载。 
3.6 是否支持硬解码？
目前支持硬解码这部分不是很成熟，也不稳定，暂时不提供。 
3.7 什么时候发布新版本？
诶，，，请关注我们官方微博（[http://weibo.com/vitamio](http://www.douban.com/link2?url=http%3A//weibo.com/vitamio) ）！！目前集中精力再弄VPlayer，新版顺利上线后应该能加快Vitamio的发布速度，所以大家可以先用VPlayer来测试。
3.8 E/Vitamio(1557): java.io.FileNotFoundException: No content provider: （2012-8-6）
这个不是错误，是正常的处理。现在播放视频是这样做的：先把 URL 当做一个 ContentProvider 来打开，如果打不开，就直接当做 URL 来打开
3.9 Vitamio支持哪些指令集的CPU？（2012-09-07）
ARMv6、ARMv6 VFP、ARMv7 VFPV3、ARMv7 NEON（注意：2012-8-31发布的版本不支持ARMv6的CPU，VPlayer已经不支持，但下次发版Vitamio会加上）
3.10 播放网络视频突然中断的问题
可能是视频网站做了限制，加入了一些不能播的地址，目前Vitamio的处理是播不了就直接关闭了，后续版本会改进，直接跳过。
四、已知问题和注意事项（新版指2012-8-31发布的版本）
4.1 混淆工程时注意，需要忽略vitamio里面的类库！（2012-8-1 update） 
-keep class io.vov.utils.** { *; }
-keep class io.vov.vitamio.** { *; }
4.2 不能在包含项目里面使用io.vov.vitamio.provider.MediaProvider
否则如果安装了VPlayer，会报冲突的错误无法安装。也就是说，可以直接使用VPlayer的MediaProvider，或者自己写扫描服务，但是名字不能相同。 
五、其他
5.1 调用VPlayer来播放视频的代码： 
复制代码
private void startPlayer(String url, String title) {
Intent i = new Intent();
i.setComponent(new ComponentName("me.abitno.vplayer.t", "me.abitno.vplayer.VideoActivity"));
i.setAction("me.abitno.vplayer.action.VIEW");
i.setData(Uri.parse(url));
i.putExtra("displayName", title);
startActivity(i);
}
复制代码
~~~~~~~~~~~~~~~~~~~~~~~~
关于Vitamio使用的问题，大家也可以往这里提问，这里汇总后持续更新。
# [Vitamio 3.0 新手教程](http://www.cnblogs.com/over140/archive/2013/01/15/2860653.html)
**前言**
Vitamio项目发展迅猛，官方群突破400位开发者，基于Vitamio的项目突破1000个，使用Vitamio提供优质播放体验的用户超过5000万。也欢迎大家分享你的Vitamio使用经验，本文将进一步介绍Vitamio的简单使用方法。
**声明**
　　欢迎转载，但请保留文章原始出处:) 
　　　　博客园：http://www.cnblogs.com
　　　　农民伯伯： http://over140.cnblogs.com   
正文
一、下载
目前Vitamio的项目托管在Github上面：[https://github.com/yixia](https://github.com/yixia) ，这里有很多公司的开源项目，其中：
1、VitamioBundle是Vitamio核心插件，（大家可以搜一下"Android Library"这个关键字，和jar差不多的用途），可以方便集成到项目中。（注意：本文使用tag里面的3.0版本）
2、VitamioDemo是Vitamio的官方例子。
提示：
如果没有安装Github，可以点击![](http://images.cnitblog.com/blog/25060/201301/15094830-0089e60393064b16ade8ed52742a650f.png)下载压缩包。
**如果无法访问Github，或者自行编译无法通过**（，这里也提供了重新打包直接下载：[Vitamio2013-01-15.zip](http://files.cnblogs.com/over140/Vitamio2013-01-15.zip)警告：建议使用官方例子，本博不保证更新到最新版本
 2013-01-15）
二、 简介
Vitamio 是一款 Android 平台上的全能多媒体开发框架。Vitamio 凭借其简洁易用的 API 接口赢得了全球众多开发者的青睐。到目前，全球已经有超过 1000 种应用在使用 Vitamio，覆盖用户超过 5000 万。 
Vitamio 能够流畅播放720P甚至1080P高清MKV，FLV，MP4，MOV，TS，RMVB等常见格式的视频，还可以在 Android 上支持 MMS, RTSP, RTMP, HLS(m3u8) 等常见的多种视频流媒体协议，包括点播与直播。
支持 ARMv6 和 ARMv7 两种 ARM CPU，同时对 VFP, VFPv3, NEON 等指令集都做相应优化。
支持 Android 2.1+ 系统，支持超过 95% 的 Android 市场。同时 Android 2.1 之前的系统也基本支持，不过没做详细测试。
其他Vitamio的介绍参照[这里。](http://vitamio.org/pages/whats-vitamio?locale=zh-CN)
三、使用
1、导入。下载回来后大家可能发现没有.project工程文件，可以通过File -> Import -> Android -> Existing Android Code Into Workspace来导入工程，然后改一下工程名称即可。
2、将VitamioBundle工程作为Android Library引入Demo工程使用即可。
新手无法解决这些问题建议直接使用上面的链接，导入工程即可使用。 
四、Vitamio
已建立Vitamio开发者联盟官方QQ群！
Vitamio官方群1：246969281（已满）
Vitamio官方群2：171570336（已满）
Vitamio官方群3：283274315
注意：目前仅接受已经开发基于Vitamio产品的开发者申请加入，申请理由请填写产品的名称和链接！
官方微博：[http://weibo.com/vitamio](http://weibo.com/vitamio)
官方网站/论坛：[vitamio.org](http://www.cnblogs.com/over140/admin/vitamio.org)
本博Vitamio相关文章：[http://www.cnblogs.com/over140/category/409230.html](http://www.cnblogs.com/over140/category/409230.html)（注意有部分文章是使用旧的版本）
五、OPlayer
OPlayer是本博基于Vitamio开发的一个开源项目，用于补充官方例子，以便大家更好的使用Vitamio项目。目前项目托管在淘宝的SVN（Taocode）： 
项目地址：[http://code.taobao.org/p/oplayer](http://code.taobao.org/p/oplayer)
SVN地址：[http://code.taobao.org/svn/oplayer/](http://code.taobao.org/svn/oplayer/)
注意：没有Taocode账户的请注册一个。 
六、其他
1、官方建议以Android Library方式使用Vitamio插件，以便后续方便升级。如果需要拷贝集成到一个工程，可能会报错找不到io.vov.vitamio.R.raw.libarm（硬编码导致的问题），请参照OPlayer方式集成（后续介绍）。
2、 Vitamio最新版本为3.0（2013-01-15更新），极力推荐使用新版本。
3、 基于Vitamio仅支持ARMv6+以上的CPU，95%以上的视频格式支持，说明：
a).无法播放的问题。使用[VPlayer](http://vplayer.net/)来测试链接，如果VPlayer没有问题那Vitamio肯定也没有问题。
b).各种找不到so文件的情况只有两种情况：不支持设备、没有执行解压解码包。
4、Vitamio是免费的，没有商业版本，也没有Vitamio.apk。
5、Vitamio是VPlayer创始人Cedric Fung ([http://vec.io](http://vec.io/))开发的，最终所有权是炫一下（北京）科技有限公司。
七、补充
有网友补了一个图文教材（2013-01-28）
[http://www.apkbus.com/android-85266-1-1.html](http://www.apkbus.com/android-85266-1-1.html)
**结束**
 本文有三个意图：公布Vitamio使用情况、公布官方QQ群3、提供可直接运行的示例工程。继续努力的推动Vitamio事业，欢迎大家使用和支持！

分类: [5、Vitamio](http://www.cnblogs.com/over140/category/409230.html)
标签: [android Vitamio](http://www.cnblogs.com/over140/tag/android%20Vitamio/), [vitamio](http://www.cnblogs.com/over140/tag/vitamio/), [Vitamio
 教材](http://www.cnblogs.com/over140/tag/Vitamio%20%E6%95%99%E6%9D%90/), [Vitamio开发指南](http://www.cnblogs.com/over140/tag/Vitamio%E5%BC%80%E5%8F%91%E6%8C%87%E5%8D%97/), [vitamio dev](http://www.cnblogs.com/over140/tag/vitamio%20dev/)
绿色通道： [好文要顶]()[关注我]()[收藏该文]()[与我联系](http://space.cnblogs.com/msg/send/%E5%86%9C%E6%B0%91%E4%BC%AF%E4%BC%AF)![](http://static.cnblogs.com/images/icon_weibo_24.png)
![](http://pic.cnitblog.com/face/u25060.png?id=26205926)
[农民伯伯](http://home.cnblogs.com/u/over140/)
[关注 - 6](http://home.cnblogs.com/u/over140/followees)
[粉丝 - 1591](http://home.cnblogs.com/u/over140/followers)
荣誉：[推荐博客](http://www.cnblogs.com/expert/)
[+加关注]()
0
0
(请您对文章做出评价)
[«](http://www.cnblogs.com/over140/archive/2012/12/17/2821874.html)博主上一篇：[【Andorid
 X 项目笔记】魅族照片剪切问题（4）](http://www.cnblogs.com/over140/archive/2012/12/17/2821874.html)
[»](http://www.cnblogs.com/over140/archive/2013/02/04/2501868.html)博主下一篇：[2012年总结](http://www.cnblogs.com/over140/archive/2013/02/04/2501868.html)
# [Flash(FLV)视频播放器开源代码大集合](http://www.cnblogs.com/top5/archive/2009/08/01/1536618.html)
下面的页面里收集了目前所有的Flash(FLV)视频播放器和开源代码 
[http://www.flashstreamworks.com/tutorials/flvplayerlinklist.php](http://www.flashstreamworks.com/tutorials/flvplayerlinklist.php)
Flv Player List
Proxus FLV player 
[http://www.proxus.com/components/index.php](http://www.proxus.com/components/index.php)
Check [Flashstreamworks' review](http://www.flashstreamworks.com/magazine/proxus.php)
FLV player by Martijn de Visser 
[http://www.martijndevisser.com/](http://www.martijndevisser.com/)
FLV player by Peldi
[http://www.peldi.com/blog/000103.html](http://www.peldi.com/blog/archives/2004/05/flvplayer_new_r.html)
FLV player by VideoSpark
[http://www.videospark.com/prog_flv8/player.php](http://www.videospark.com/prog_flv8/player.php)
FLV player by Wimpy
[http://www.gieson.com/Library/projects/wimpyAV.html](http://www.gieson.com/Library/projects/wimpyAV.html)
FLV player component by Meshybeats
[http://meshybeats.com/components/](http://meshybeats.com/components/)
[http://www.flashstreamworks.com/tutorials/flvplayersource.php](http://www.flashstreamworks.com/tutorials/flvplayersource.php)
Flash Video Player sources
These are simple source examples for Flash Video applications. Feel free to modify and edit them.
Playing video without components
[Download](http://www.flashstreamworks.com/tutorials/files/video_without_components.zip)
Playing video with components
[Download](http://www.flashstreamworks.com/tutorials/files/video_with_components.zip)
Using FVSS for Flash Video with HTML authoring
[Download](http://www.flashstreamworks.com/tutorials/files/fvss_html_templates.zip)
Live streaming without components
[Download](http://www.flashstreamworks.com/tutorials/files/live_streaming.zip)
Multiuser chat room with components
[Download](http://www.flashstreamworks.com/tutorials/files/multiuserchat.zip)
[FLVParser]. 
用法:
命令行>flvparse 你的视频.flv cue.txt -s
它另一个功能是根据cue-point文件分割FLV文件.
[点击下载](http://www.andrew.cmu.edu/user/jcwrubel/FLVParser.zip)
