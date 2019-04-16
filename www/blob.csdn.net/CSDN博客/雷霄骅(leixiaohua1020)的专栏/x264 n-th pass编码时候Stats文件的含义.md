# x264 n-th pass编码时候Stats文件的含义 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年08月02日 21:46:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：118
个人分类：[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)










### 

### x264n-th pass（一般是2pass）编码时所用的文件包括下述x264参数生成.stats文件

```
options: 1280x816 fps=2997/125 timebase=125/2997 cabac=1 ref=4 deblock=1:0:0 analyse=0x3:0x113
me=umh subme=7 psy=1 psy_rd=0.00:0.00 mixed_ref=1 me_range=16 chroma_me=1 trellis=1 8x8dct=1 cqm=0
deadzone=21,11 fast_pskip=0 chroma_qp_offset=2 threads=1 sliced_threads=0 nr=0 decimate=1 interlaced=0
constrained_intra=0 bframes=5 b_pyramid=2 b_adapt=2 b_bias=0 direct=1 weightb=1 open_gop=0 weightp=2
keyint=300 keyint_min=25 scenecut=70 intra_refresh=0 rc_lookahead=40 rc=abr mbtree=1 bitrate=4719
ratetol=0.1 qcomp=0.58 qpmin=9 qpmax=51 qpstep=5 ip_ratio=1.38 aq=2:1.11
```

### 每帧信息

```
in:0 out:0 type:I dur:2 cpbdur:2 q:20.51 tex:686628 mv:49125 misc:5975 imb:4080 pmb:0 smb:0 d:- ref:;
in:4 out:2 type:P dur:2 cpbdur:2 q:30.05 tex:88336 mv:44264 misc:2128 imb:1654 pmb:1973 smb:453 d:- ref:4784 2665 443 ;
in:3 out:3 type:B dur:2 cpbdur:2 q:29.46 tex:13732 mv:16269 misc:3615 imb:106 pmb:1287 smb:2655 d:- ref:1130 98 ;
in:2 out:4 type:b dur:2 cpbdur:2 q:28.88 tex:15560 mv:23723 misc:4029 imb:104 pmb:1829 smb:2130 d:- ref:4839 92 ;
```

### 注:

```
in: 显示/输入帧号
out: 编码帧号
type: 帧类型
q: 帧量化值
tex: 用于residual的bits大小
mv: 用于mvs的bits大小
misc: 用于其它的bits大小
imb: 内部宏块(intra macroblocks)数
pmb: 帧间宏块(inter macroblocks)数
smb: 跳过宏块(skip macroblocks)数
d: 最适用于此帧的direct模式
ref: 列表中每个ref使用的次数
w: 此帧的最优权重(如果开启了权重)
```

### MB-Tree Stats文件开启MB-Tree选项后，产生的文件。

```
for each frame in the video except b-frames:{
     uint8_t header = frametype
     for each macroblock in the frame:{
       16-bit signed fixed point value ( 8.8 ) for delta-quant
     }
}
```

### 参考：[http://x264-settings.wikispaces.com/x264_Stats_File](http://x264-settings.wikispaces.com/x264_Stats_File)






