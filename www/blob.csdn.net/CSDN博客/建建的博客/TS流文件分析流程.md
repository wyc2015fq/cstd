# TS流文件分析流程 - 建建的博客 - CSDN博客
2017年11月02日 09:56:47[纪建](https://me.csdn.net/u013898698)阅读数：173
转载自：http://blog.csdn.net/a1875566250/article/details/40615473
1、查找PAT表，从PAT表里面找到所有的PMT表的map_id。
 - 注意1：PAT表并不一定在文件的起始位置，TS流这种对于电视直播的Live流需要保证在任何时间打开电视你都能看到画面，所以PAT表是被随机插到TS流的Packet中的，比如间隔10帧插一个PAT表和PMT表。所以TS流文件的第一个TS
 Packet可能是一个PES包，但是这个PES包更可能是续包，它没有解码器需要的Header，所以这种包可以在播放中被忽略，因为它可能是录制前一帧的I、P、B包的一个断包，根本解码不出数据；
 - 注意2：记得检测current_next_indicator这个flag，如果这个flag被置1，则忽略本次读到的这个PAT包，继续往下搜索PAT包；
 - 注意3：如果PAT包因为容纳的PMT的map_id很多，一个TS Packet的188个字节或许放不完，则last_section_number不是0了，你得根据当前的section_number（第一个是0），然后不断的搜索下去，把TS Packet去掉头后的数据组合成一个完整的PAT表；
2、查找PMT表，建立流id表。
 在通过PAT表找到所有的PMT表的id后，则需要开始继续跑文件，查找PMT表了，一般情况下，PMT表在TS文件中的位置跟在PAT表的后面，但是也有不同，所以我推荐在查找PAT表完成后，把指针Seek到文件的0位置，从头开始查找PMT表。这样可能能更快的找到PMT表也说不定，当然你用当前的位置继续向下找PMT表也是没问题的。
 - 注意1：PMT表也有跟PAT表一样的分段特性，一样检查last_section_number这个是不是有情况。也有current_next_indicator的特性，都得检查；
 - 注意2：当PAT表里提供了多张PMT表的id后，则表明文件是一个多视频、多音频流混合的文件；
3、通过流id，查找文件中的TS Packet。
 在搜索完所有PMT表后，保存其中的流类型和流id，此时我们有一张表，表里保存了所有的视频流id和音频流id，下面我们把文件指针Seek到0，我们开始一点点的查找TS Packet。在这之前有一些需要注意的地方：
 - 确定你要播放的视频和音频流：因为文件中可能有多个视频、音频流，并且这些流的编码也不同，比如日本的电视在播放时会用1080i的MPEG2和240P+360P的H264同时传输，这样录制下来的TS流则会有3个视频流（id），并且音频也是传输3条，也就是有6条流，但是我们在PC或者碟机中播放的时候，一般都是播放一条视频和一条音频，则我们必需根据用户选择播放那条视频和音频（如果你希望让用户选择的话），比如我们希望播放MPEG2的视频，所以在不断的跑读TS
 Packet的过程中，我们要忽略掉除了MPEG2流的视频id，那些全部Skip即可，音频同理。
 - 如何查找一个音频\视频帧的头，以及它的长度：这个问题也比较简单，在跑TS Packet的过程中，找到PES包，如果TS头表明payload_unit_start_indicator为1，则这个PES包此流id的某一帧起始包，去掉PES头后的ES流就是编码后的流的起始数据。而后面的针对这条流的PES包，只要没有payload_unit_start_indicator标志，都是这个包的续包，这些续包把头去掉后，跟上一个包的数据组合起来，就一个编码后的ES数据。
   这里有一个需要注意的，在找到一个包表明它是payload_unit_start_indicator后，往下查找可能会查找到其他流id的payload_unit_start_indicator的PES包。。。要分别组合。
4、如何计算TS流的PTS\DTS。
 PES\PCR中的PTS(DTS) / 90000即可。
5、如何计算TS文件的时间长度。
 （最后一个PCR的Time - 第一个PCR的Time） / 90000。
6、如何在TS文件中进行随机访问播放。
 计算出时间后，再计算rate，然后对Io进行Seek，Seek后，查找流的PES包，确定这个包是起始包（payload_unit_start_indicator），然后最重要的，确定这个包是I帧，不是的话继续查找下一个PES包。
7、如何得到TS文件中ES流的视频信息、音频信息。
 视频：解析H264的SPS，解析MPGE2的Header等。
 音频：解析AAC的ADTS，解析LPCM的HDMV Header等。
 也就是说，要想知道这些流的信息，最好的办法就是把他们的第一帧扔给解码器，但是这样是一个浪费时间的活，而我们只需要解析他们的头就能知道信息，所以自己加一个轻量的Parser即可，或者从解码器的代码里面抠。
