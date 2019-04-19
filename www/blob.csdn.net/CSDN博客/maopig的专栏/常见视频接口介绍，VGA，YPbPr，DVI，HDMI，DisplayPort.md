# 常见视频接口介绍，VGA，YPbPr，DVI，HDMI，DisplayPort - maopig的专栏 - CSDN博客
2012年04月21日 14:21:25[maopig](https://me.csdn.net/maopig)阅读数：5794

1，VGA(D-SUB)
这种是显示器最常见的，用了很多年，色域空间是RGB，也就是红绿蓝，模拟信号，无音频
![](http://www.consolegames.ro/forum/attachments/f9-microsoft-xbox-360/21857d1241344056-xbox-360-audio-video-mod-vga_pinout.jpg)
插头是15针的，实际所需的最小针数应该是5针，也就是RGB三色信号，水平同步信号，垂直同步信号（地线另外算）
2，YPbPr色差信号
这是电视机最常见的，三根线，红绿蓝，其中绿色的是亮度信号，红色和蓝色是色度信号
你只插一根绿色也是能显示的，但是是黑白，插了红色蓝色才是彩色的
YPbPr也是模拟信号，它和RGB是线性的转换关系，如下图，Pb信号是蓝色和亮度的差的某个倍数，Pr信号是红色和亮度的差的某个倍数
还有个区别就是，水平垂直同步信号，是加载在Y信号，也就是绿线上面的，所以一般它只需要三根线，不像VGA需要5根
![](http://img.tfd.com/cde/YUV.GIF)
顺带简单介绍一下S-Video和CVBS：
他们都是比较老的视频信号格式，S-video是两个信号（亮度Y和色度C信号，加个地），CVBS是一个信号（全调制信号，加个地），以后应该都不怎么会用的，反正你要记得的是，一般在传输同样的信号的情况下，线越多，效果越好，所以，常见模拟信号从理论上就效果来看
VGA（5根线）> YPbPr（三根线）> S-Video(两根线) > CVBS(一根线)
3, DVI接口
DVI接口是数字时代的产品，分成三种，DVI-D(Digital，数字)，DVI-A（Analog，模拟），DVI-I（I）
里面传播的是差分数字信号，无音频
如下图，左边的1～24个信号，是数字信号（8除外，8是模拟垂直同步），右边的C1~C5是模拟信号
![](http://techpubs.sgi.com/library/dynaweb_docs/hdwr/SGI_EndUser/books/SGI_PRISM_UG/sgi_html/figures/pinout.DVI-connector.gif)
DVI-D = 只有信号1～24 
DVI-A = 信号8 + 信号C1~C5（常见的DVI转VGA线，就是接的这些信号） 
DVI-I = 全部信号都有
还有要注意的就是LINK1和LINK2是可以独立使用的，只有一个也能工作，但是如果有两个，支持的分辨率就更好
![](http://bak2.beareyes.com.cn/2/lib/200708/15/376/image007.png)
4，HDMI接口
HDMI接口的出现，就是为了解决DVI的体积太大问题，信号是一样的，然后顺便把音频也加在数字信号里面传播了，就避免了还要另外接音频线的问题
也是差分全数字信号，但是包含音频
看，小吧
![](http://www.it.com.cn/f/diy/077/4/07622_diy_vx2435wm_5m.jpg)
5,DispayPort简单介绍
根据其他兄弟补充，顺便介绍下DisplayPort，一种比HDMI出道晚一些的数字传输技术，接口如下图左边（右边是HDMI）
它也是全数字信号，比HDMI的带宽还高，可以传输的数据更多，而且好像是免费的（HDMI是收费的，类似UL，需要认证费和标签费）
其他和HDMI区别不大，信号有一些差异但是较难将清楚，就不多说了，目前已经有不少高端机型具备了DisplayPort和HDMI双接口
未来到底谁是赢家，还不确定
![](http://diy.yesky.com/imagelist/2007/261/2azy1u89yy3y.jpg)
总结：
1，HDMI是未来的趋势，一根线包含了视频音频所有信号，而且是全数字的（音频也是数字的，无损），带宽高，抗干扰能力强，建议使用
2，对于信号转换的问题，以上几类信号是完全可以互相转换的，但是如果是数字转模拟，或者模拟转数字，就需要另外的IC来完成，就像下图一样的这种设备，里面其实就是一个模拟转数字的IC而已，（注意：模拟信号和数字信号不能直接用线连接，不可识别，需要有源IC进行转换才可以）
3，模拟信号由于起点低，带宽小，用来看高清的效果不会很好，有条件还是尽量往数字信号上面靠
4，模拟电视机（CRT）常见的接口是YPbPr，S-Video，CVBS，RF，液晶电视一般带有YPbPr，VGA，DVI，HDMI
5，显示器和液晶类似，不过有YPbPr的比较少，最多的是VGA，然后是DVI，高端机型才有HDMI
![](https://img-my.csdn.net/uploads/201204/21/1334991053_5935.jpg)
