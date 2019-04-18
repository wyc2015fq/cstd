# 多媒体开发之--- rtsp 中的H264 编码+打包+解码相关知识es、pes、ts... - weixin_33985507的博客 - CSDN博客
2014年12月11日 18:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
1)ES流(Elementary Stream): 也叫基本码流,包含视频、音频或数据的连续码流.
2)PES流(Packet Elementary Stream): 也叫打包的基本码流, 是将基本的码流ES流根据需要分成长度不等的数据包, 并加上包头就形成了打包的基本码流PES流.
3)TS流(Transport Stream): 也叫传输流, 是由固定长度为188字节的包组成, 含有独立时基的一个或多个program, 一个program又可以包含多个视频、音频、和文字信息的ES流; 每个ES流会有不同的PID标示. 而又为了可以分析这些ES流, TS有一些固定的PID用来间隔发送program和ES流信息的表格: PAT和PMT表，(在MPEG-2系统中,由视频, 音频的ES流和辅助数据复接生成的用于实际传输的标准信息流称为MPEG-2传送流)
封装 : 就是捆绑打包, 将画面视频文件和音轨文件打包在一起, 并按照一定规则建立排序和索引, 便于播放器或播放软件来索引播放. 包括AVI / PS(Program Stream)/ TS（Transport Stream）/ MKV（Matroska）等。
4)I frame ：帧内编码帧又称intra picture，I帧通常是每个
GOP（MPEG所使用的一种视频压缩技术）的第一个帧，经过适度地压缩，做为随机访问的参考点，可以当成图象。I帧可以看成是一个图像经过压缩后的产物。
5)P frame:前向预测编码帧又称predictive-*frame*，通过充分将低于图像序列中前面已编码帧的时间冗余信息来压缩传输数据量的编码图像，也叫预测帧；
6)B frame:双向预测内插编码帧又称bi-directional interpolated predictionframe，既考虑与源图像序列前面已编码帧，也顾及源图像序列后面已编码帧之间的时间冗余信息来压缩传输数据量的编码图像，也叫双向预测帧；
7)PTS：PresentationTime Stamp。PTS主要用于度量解码后的视频帧什么时候被显示出来;
8)DTS：Decode TimeStamp。DTS主要是标识读入内存中的ｂｉｔ流在什么时候开始送入解码器中进行解码。
在没有B帧存在的情况下DTS的顺序和PTS的顺序应该是一样的。
9)
I frame:自身可以通过视频解压[算法](http://www.xuebuyuan.com/category/%E7%AE%97%E6%B3%95)解压成一张单独的完整的图片。
P frame：需要参考其前面的一个I frame或者B frame来生成一张完整的图片。
B frame:则要参考其前一个I或者P帧及其后面的一个P帧来生成一张完整的图片。
两个I frame之间形成一个GOP，在x264中同时可以通过参数来设定bf的大小，即：I和p或者两个P之间B的数量。
通过上述基本可以说明如果有B frame存在的情况下，一个GOP的最后一个frame一定是P.
**(10)DTS和PTS的不同：**
DTS主要用于视频的解码,在解码阶段使用.PTS主要用于视频的同步和输出.在display的时候使用.在没有B
frame的情况下.DTS和PTS的输出顺序是一样的.
下面给出一个GOP为15的例子,其解码的参照frame及其解码的顺序都在里面:
![](https://img-my.csdn.net/uploads/201301/01/1357024065_4568.JPG)
如上图：I frame 
的解码不依赖于任何的其它的帧.而p frame的解码则依赖于其前面的I frame或者P
frame.B frame的解码则依赖于其前的最近的一个I frame或者P frame及其后的最近的一个P
frame.
(11)视频解码大致流程如下：
![](https://img-my.csdn.net/uploads/201301/01/1357024324_6040.JPG)
(12)PTS的计算
方法一、根据前后帧的IPB类型，可以得知帧的实际显示顺序，使用前面获取的sps信息中的帧率，以及帧计数frame_count即可计算出PTS。此方法需要做几帧缓存（一般缓存一个group的长度）。
I P  B  B  I  P  B  B  I P  B  ...帧类型
1  2  3  4  5  6  7  8 9  10 11 ...第几帧
1  4 2  3  5  8  6  7  9  12 10...帧显示顺序
一个I帧与下一个I帧之间，是一个group。
从上图可见，P类型的帧的显示顺序，是排在后面最后一个B帧之后。
所以要获取第7帧的pts，起码要知道他下一帧的类型，才能得知他的显示顺序。
第8帧的pts=1000（毫秒）*7（帧显示顺序）*帧率
方法二、每一个slice的信息里面，都记录有pic_order_cnt_lsb，当前帧在这个group中的显示顺序。通过这个pic_order_cnt_lsb，可以直接计算出当前帧的PTS。此方法不需要做帧缓存。
计算公式：
pts=1000*(i_frame_counter+ pic_order_cnt_lsb)*(time_scale/num_units_in_tick)
i_frame_counter是最近一次I帧位置的帧序，通过I帧计数+当前group中的帧序，得到帧实际显示序列位置，乘上帧率，再乘上1000（毫秒）的base_clock（基本时钟频率），得到PTS。
I P  B  B  I  P  B  B  I  P B 
...帧类型
1  2  3  4  5  6  7  8 9  10 11 ...第几帧
1  4 2  3  5  8  6  7  9  12 10 ...帧显示顺序
0  6  2  4 0  6  2  4  0  6 2 
... pic_order_cnt_lsb
细心一点可以注意到，在上图，slice里面的pic_order_cnt_lsb是以2进行递增。
通常H264里面的sps中记录的帧率，也是实际帧率的2倍time_scale/num_units_in_tick=fps*2
因此，实际的计算公式应该是这样
pts=1000*(i_frame_counter*2+pic_order_cnt_lsb)*(time_scale/num_units_in_tick)
或者是
pts=1000*(i_frame_counter+pic_order_cnt_lsb/2)*(time_scale/num_units_in_tick/2)
所以，第11帧的pts应该是这么计算
1000*(9*2+2)*(time_scale/num_units_in_tick)
RTP打包H264的时间戳，由于H264标准说明是90000/帧率,这里pts的base_clock都是按照1000（毫秒）计算，如果复用到ts里，base_clock是90k，所以还应该再乘以90。
pic_order_cnt_lsb：
lsb：least significant bits POC低位
poc：picture oder counter
pic_order_cnt_type：指明了POC（Picture Order Count）的编码方法、POC标识图像的播放顺序。
http://www.cppblog.com/dvb-dvb/archive/2009/08/12/99268.html
http://blog.csdn.net/ljh081231/article/details/5828997
http://www.chinavideo.org/archiver/?tid-5256.html 264码流的帧率求解
http://bbs.csdn.net/topics/390368092 rtp 帧率、时间戳
http://70565912.blog.51cto.com/1358202/533736/  pts 获取代码实现
http://zhongcong386.blog.163.com/blog/static/1347278042012113033745404/
http://www.xuebuyuan.com/660091.html
[http://video.vomont.com/client.html](http://video.vomont.com/client.html)
[http://www.phpv.net/html/122.html](http://www.phpv.net/html/122.html) 破解电视下载
[http://wxinpeng.iteye.com/blog/2106068](http://wxinpeng.iteye.com/blog/2106068) rtsp 公网地址
[http://www.foko.cn/admin/upload/201323123111.pdf](http://www.foko.cn/admin/upload/201323123111.pdf) 美赞ipc
[http://www.fanli7.net/a/bianchengyuyan/_NET/20130415/297725.html](http://www.fanli7.net/a/bianchengyuyan/_NET/20130415/297725.html)
http://blog.csdn.net/ljh081231/article/details/8124535 ccd之殇
