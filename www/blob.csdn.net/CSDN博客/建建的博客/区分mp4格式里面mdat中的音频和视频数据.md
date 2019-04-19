# 区分mp4格式里面mdat中的音频和视频数据 - 建建的博客 - CSDN博客
2017年08月17日 18:05:40[纪建](https://me.csdn.net/u013898698)阅读数：466
首先在minf里面有个vmhd和smhd，那么vmhd代表视频，smhd代表音频
然后在stsz中
stsz Box
00 00 73 D8  size of stsz,20
73 74 73 7A:  stsz
00 00 00 00:  version
00 00 00 00:  sample-size
00 00 1C F1:  sample-count
00 00 86 24（从此开始，为当前chunk中每帧视频数据的字节大小，此大小与offset相对应，用offset找到偏移的绝对地址（指向mdat box中）后，从000001B6的第一个字节开始，加上对应的视频字节数，就是本帧的大小。一个视频chunk中首先是连续的视频帧，然后也包含其他数据，暂不清楚视频数据后面的数据类型）
00 00 08 F9   //第一个视频帧的位置
00 00 3B 04  //第二个视频帧的位置
00 00 1D F2
