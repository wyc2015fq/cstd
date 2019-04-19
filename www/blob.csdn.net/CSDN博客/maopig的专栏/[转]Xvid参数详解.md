# [转]Xvid参数详解 - maopig的专栏 - CSDN博客
2011年09月01日 20:09:35[maopig](https://me.csdn.net/maopig)阅读数：3587
**![](http://www.3g365.com/attachments/day_050425/01_v1ujyEw3I5jT.jpg)众所周知，Mencoder以其极高的压缩速率和不错的画质赢得了很多朋友的认同！原来用Mencoder压缩Xvid的AVI都是使用Xvid编码器的默认设置，现在我来给大家冲冲电，讲解一下怎样使用Mencoder命令行高级参数制作Xvid编码格式AVI影片。经过测试，可以毫不夸张的说：在减小50%的文件体积同时能够保持原有画质！大家平时用WisMencoder压缩电影的时候也许会感觉到虽然它速度飞快，但是关于Xvid编码器的设置却少得可怜！并没有完全发挥出Xvid编码的优势！ ****这里仅仅讲解WisMncoder的手动参数，其他设置还需大家自己完成。压缩过程中的视频码率建议不要超过192Kbps（因为对于加入了手动参数的Xvid编码器，画质提升是相当明显的），强烈建议用2-Pass压缩！首先介绍一下这些高级参数的作用：cartoon——卡通模式vhq=4——VHQ MODE，有0-4五个值，越大画质越好，但是编码速度越慢chroma_me——动态检索max_key_interval——最大关键帧距离Quant_type=mpeg——量化矩阵，Mpeg模式，如果压缩时去掉这段就默认量化矩阵为H.263trellis——高质量量化模式GMC——全域动态补偿qpel——四分之一像素精确动态补偿（如果使用了该功能则可能降低解码速率，建议不用）bvhq=1——在B帧上启用VHQlumi_mask——加大画面明暗部分压缩率（一般不用，有时会导致画面失真）greyscale——使用灰度，但是便出来的画面是黑白的（一般不使用）interlacing——隔行处理，适用于交错片源（一般不使用）chroma_opt——色度优化Quantization量化选项（关键的关键就在这里！）量化器级别(quantizer level)即Q值越高画面细节越丰富，但是在低码率下越高的量化器级别，会带来越低的画面质量，Q 值越小，质量越好，码率越大；Q 值越大，质量越差，码率越小。（实际上，它的原始概念更复杂，我们干脆简单地认为1就是100%原质量，31就是1/31的原质量好了）。P-frame（即未来单项预测帧）建议设置在1--15之间，（它的作用是只储存与之前一个已解压画面的差值。即仅仅描述它与前一帧的区别）。I -frame（即关键帧）的Q值只用能作用于2pass-second压制上，在VirtualDub被标示为 [K]，关键帧是构成一个帧组（GOP， Group of Picture）的第一个帧。I-Frame 保留了一个场景的所有信息（Keyframe原意是指可以单独解码、用于同步的 frame，不过在MPEG系统中只有I-frame有这个特性，因为BF和PF都是预测帧，要靠I-Frame来还原）。B-frame（即双向预测帧）除了参考之前解压过了的画面外，亦会参考后面一帧中的画面信息。这三个设置的范围都是1-31，应该注意的是在场景昏暗的条件下不要让B-frame 过低。min_iquant=1——IF的量化最低值(取值范围在1-31之间)max_iquant=10——IF的量化最高值(取值范围在1-31之间)min_pquant=1——PF的量化最低值(取值范围在1-31之间)max_pquant=10——PF的量化最高值(取值范围在1-31之间)min_bquant=1——BF的量化最低值(取值范围在1-31之间)max_bquant=30——BF的量化最低值(取值范围在1-31之间)-sws 9 取值范围在(0-9)之间，不同值代表不同的Resize算法！建议用 -sws 90                   快速二次线性(默认)（速度较快）1                   二次线性2                   二次立方(质量很好)3                   实验中4                   最短距离 (bad quality)5                   区域6                   亮度二次立方 度二次线性7                   高斯8                   sincR9                   lanczos（速度较快，而且画面锐利）10                   双三次样条曲线**实际应用举例：把vhq=4:cartoon:chroma_me:min_iquant=1:chroma_opt:min_iquant=1:max_iquant=12:min_pquant=1:max_pquant=12:bvhq=1:min_bquant=1添加到WisMencoder的手动参数中的视频编码处，如本楼底部图片：然后再把 -sws 9 参数添加到视频滤镜处即可！记得-sws 9前一定加个空格，要不编码器不会启动！当然你也可以自己摸索适合的方案，做出效果更佳的方案！****
Xvid大概介绍：（个人认为XVID编码比DIVX编码的画质更好，故只讲XVID的参数。了解以下内容对转换影片有一定帮助，尤其是要放在硬盘上用PC观看的视频，当然对转换成手机观看的影片也是有好处的，使你不至于盲目的转换）
                             (此文是参照电脑杂志上的介绍而写，在此感谢写出那篇文章的作者。所涉及到的图片是自己抓的，文字也是一个一个字打出来的，并非从网上转抄，我无非是想让有兴趣于此道的朋友更多的了解xvid这个编码器）
如果在网页上看不太清楚的图片，请下载到个人电脑上查看
xvid有两种编码方式：single pass和twopass
single pass模式编码简单，速度也快，但最终效果不如twopass。
twopass就是视频压制需要经过两次编码，分别为twopass－1st pass（简称1pass）和twopass－2nd pass（简称2pass）
1pass时，编码器会用最高质量编码采集可供第2次运算参考的画面信息，而在2 pass时。编码器会根据第一次压缩获得的信息和用户指定的文件大小，自动分配比特率，使需要高流量的运动画面分配到更多的空间，更高的比特率来保证画面质量。相对的，对于那些不包含太多运动信息的静态画面则用较低的比特率。追求画质的朋友当然会选择这种方式，但运算比single pass更费时。
接下来介绍一些基本概念：
Q值——量化值，它被用来描述1帧的质量，每帧都有一个Q值，取值范围在1－31之间。Q值越小，画质越好，比特率越大
I－frame——关键帧，常被缩写为IF。关键帧是构成一个帧组的第一帧。IF保留了一个场景的所有信息
P－frame——未来单项预测帧，缩写为PF，只储存与之前一个已解压画面的差值
B－frame——双向预测帧，缩写为BF，除了参考之前解压的画面以外，也会参考后一帧的画面信息
![](http://www.3g365.com/attachments/day_050425/01_v1ujyEw3I5jT.jpg)
第一步：Profile@Level（框架级别）设置
选择Profile@Level旁的more按钮来进行框架设置。Profile@Level里方案的层级越低，编码的比特率及分辨率限制的越多。在制作 DVDrip时一般选择AS@L5或者unrestricted（不限定比特率，让编码器自行分配）。我在制作手机影片时通常选用AS@L5，因为可选择的方案更多，而且比特率可自己随意调节，只要手机能跑的动就行！！
（注意：在single pass模式下，Profile@Level的设定无效，由编码器自行分配比特率) 
![](http://www.3g365.com/attachments/day_050425/02_AMty0ZuaIzr8.jpg)
第二步：Encoding type（编码类型）设置           （这里是重中之重）
第1种方式：
single pass，求快可以用这种一次性编码来压缩视频。但因为每帧的压缩率相同，流量自行分配，所以压缩后的视频容量无法控制。 single pass有Target quantizer和Target bitrate两种模式，在主界面的Encoding type里先选择 single pass，再点击左下方的Target XXX按钮在两种模式中进行切换
名词解释：
Target bitrate——目标比特率模式。选择平均比特率编码，相对容易控制文件大小。
Target quantizer——目标Q值模式，采用动态比特率编码。设置为“1”是最高质量，31是最低质量。设置为2是容量与效果的最佳比，但文件体积很大，可以设置小一点的数值，例如2.5，如果不确定，可以按主界面上的“calc”按钮计算目标容量大小(详细的计算方法请看下一节）
![](http://www.3g365.com/attachments/day_050425/03_qcYsNT1cMGgp.jpg)
![](http://www.3g365.com/attachments/day_050425/04_m67N3N0BFlCB.jpg)
如何用calc计算容量：
Target size：目标容量的大小，举例来说，如果我们想将视频容量控制在1CD以内，由于1M等于1024KB，所以其值不能高于716800KB。
subtitles：选择此项能将外挂字幕的大小纳入计算
container：按照你想制作的最终视频格式选择
video：设定视频长度及fps
Audio：设定音频格式 
![](http://www.3g365.com/attachments/day_050425/vMbL48jdwb8=_6OrFPMz21T2X.jpg)
第2种方式：
为了保证视频的质量，很多时候我们会选择twopass模式来压缩视频
step 1：
在主窗口的“Encoding type”下拉菜单中选择1pass模式，再点击旁边的more按钮。在弹出的窗口中勾选 “Discard first pass”（不输出第一次运算结果）否则可能生成一个高达几GB的无用文件。 “full quality first pass”是指优质进行1pass，通常这是没必要的，这样反而会降低1pass的编码速度
![](http://www.3g365.com/attachments/day_050425/05_nKLEFk5229Nc.jpg)
![](http://www.3g365.com/attachments/day_050425/06_7UyyDo0MJIc7.jpg)
step 2：
再返回到主窗口，在“Encoding type”下拉菜单中选择2pass模式，点击旁边的more按钮进行设置。
首先点击“stats filename”这一栏的按钮，选择在1pass编码时生成的*.pass文件。
其余讲解请看图 
![](http://www.3g365.com/attachments/day_050425/07_BH2tzYHrUHM2.jpg)
![](http://www.3g365.com/attachments/day_050425/08_uXu5RxhzgTkh.jpg)
以上介绍了两种编码方式，现在讲一下视频的优化：
01.Advanced Options：
Motion search Precision（运动检索精确度）：设定运动检测的精密度，数值越高，文件转换时间越长，建议选用“6－ultra High”
VHQ mode（VHQ模式）：根据人类视觉特性进行建模，可以使肉眼可分辨的区域压缩的好一点，从而提升肉眼所见部分的视频质量。若想效率与质量兼顾，可以将VHQ设置为1，若想求得最佳画质则可以将VHQ设置为4
Use VHQ for bframes too：对BF也使用VHQ模式
Use chroma motion：使用chroma（色度）信息来动态计算，虽然会增加编码时间，但可以有效地防止色块和亮块，建议选择
Turbo：开启BF时可以提高编码速度，但会降低一点画质，不建议选取
Frame drop ratio（丢帧率）：保持为0，因为我们希望每一帧都被编码
Maximum I－frame interval：最大IF帧距，一般为帧率的10倍。例如一段视频的帧率为24，则该处就填写240，最好是维持为240，这样可确保每240帧就会有一个IF。如果场景变换侦测出需要插入IF的帧数小于这个值的话，它就会重新计算
![](http://www.3g365.com/attachments/day_050425/09_b1WeFMHFLjZd.jpg)
02.Quantization（量化）设置：
设定IF/PF/BF最小及最大Q值，从XVID1.1开始，它的流量控制做的很好，在比特率足够的情形下，不会出现过高的Q值，保持默认可防止目标容量超出或者小于设置的情况。但在single pass模式下设定Q值是无效的
Trellis quantization：在2pass模式下可以提升画质，但在single pass模式则会使画质有一些下降，容量也有所减少 
![](http://www.3g365.com/attachments/day_050425/10_TSoIQw9Imo1N.jpg)
03.Debug设置：
Performance optimizations：性能优化，可以选择特定的cpu指令集优化，建议选择第一项
Fourcc used：默认值是xvid，改变此设定可以让你使用divx等其他播放filter来播放，但有时会不兼容，建议不修改此项设置 
![](http://www.3g365.com/attachments/day_050425/11_HpWNCMBf5iH5.jpg)
04.Zone options设置：
zone其实就是将影片划分为几段，分别为这些片断定制一些设置（此项设置用于转换pc上观看的影片较多，在此不作细讲）
小提示：如果在片头之类非重要片段使用Quantizer模式，应该在1pass中也使用相同的设置。而若要用Weight模式则只需要在2pass中设置即可 
![](http://www.3g365.com/attachments/day_050425/xi85Mno1sM=_yMG4bc05NsOz.jpg)
至此，xvid的参数设置已讲解完了，有兴趣于此道的朋友可以多作尝试，找出适合你的具体参数。
我先献丑了，把自己的参数设置提供给大家
手机机型：QD
片源：一般都是网上下载的AVI文件
转换软件:TMPGEnc3XP
视频编码器:xvid
编码参数：帧率:15fps   ，码率:128－160kbps（动画片的码率有时更小） 编码方式：single pass（上传到论坛的视频都是用的 single pass，主要是为了节省时间，而且手机不能支持较高的码率，故个人感觉在此single pass和twopass差别不大。如果用于 pc上，还是建议用twopass） 其余的设置均是参照上文
音频编码：MPEG Layer－3         32000hz   ，48kbps   CBR   ，Mono
滤镜：只在加字幕文件的时候用到，一般是制作的SRT字幕，滤镜是Text Sub2.23
如果有朋友经常用DIVX编码，请发个帖子作一下详细的介绍，大家交流一下经验！！！
