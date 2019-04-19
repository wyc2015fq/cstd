# MP4文件格式解析 - 建建的博客 - CSDN博客
2017年08月18日 11:08:54[纪建](https://me.csdn.net/u013898698)阅读数：199
一、概述
        MP4文件格式中，所有的内容存在一个称为**movie**的容器中。一个**movie**可以由多个**track**s组成。每个**track**就是一个随时间变化的媒体序列，例如，视频帧序列。**track**里的每个时间单位是一个**sample**，它可以是一帧视频，或者音频。**sample**按照时间顺序排列。注意，一帧音频可以分解成多个音频**sample**，所以音频一般用**sample**作为单位，而不用帧。MP4文件格式的定义里面，用**sample**这个单词表示一个时间帧或者数据单元。每个**track**会有一个或者多个**sample description**s。**track**里面的每个**sample**通过引用关联到一个**sample description**。这个**sampledescription**s定义了怎样解码这个**sample**，例如使用的压缩[算法](http://lib.csdn.net/base/datastructure)。
与其他的多媒体文件格式不同的是，MP4文件格式经常使用几个不同的概念，理解其不同是理解这个文件格式的关键。
       这个文件的物理格式没有限定媒体本身的格式。例如，许多文件格式将媒体数据分成帧，头部或者其他数据紧紧跟随每一帧视频，！！！TODO（例如MPEG2）。而MP4文件格式不是如此。
        文件的物理格式和媒体数据的排列都不受媒体的时间顺序的限制。视频帧不需要在文件按时间顺序排列。这就意味着如果文件中真的存在这样的一些帧，那么就有一些文件结构来描述媒体的排列和对应的时间信息。
        MP4文件中所有的数据都封装在一些**box**中（以前叫**atom**）。所有的metadata(媒体描述元数据)，包括定义媒体的排列和时间信息的数据都包含在这样的一些结构**box**中。MP4文件格式定义了这些这些**box**的格式。Metadata对媒体数据（例如，视频帧）引用说明。媒体数据可以包含在同一个的一个或多个**box**里，也可以在其他文件中，metadata允许使用URLs来引用其他的文件，而媒体数据在这些引用文件中的排列关系全部在第一个主文件中的metadata描述。其他的文件不一定是MP4文件格式，例如，可能就没有一个box。
        有很多种类的**track**，其中有三个最重要，**video track**包含了视频**sample**；**audio track**包含了audio**sample**；**hint track**稍有不同，它描述了一个流媒体服务器如何把文件中的媒体数据组成符合流媒体协议的数据包。 如果文件只是本地播放，可以忽略**hint track**，他们只与流媒体有关系。
二、 结构
    整体结构
![](https://img-blog.csdn.net/20130831133528875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
  1 ftype
![](https://img-blog.csdn.net/20130831133712296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   size = 0x  00 00 00 18            ftype = 66 74 79 70         data  为蓝色部分
 2   moov
![](https://img-blog.csdn.net/20130831133956156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    Movie atom定义了一部电影的数据信息。它的类型是'moov'，是一个容器atom，至少必须包含三种atom中的一种—movie header atom('mvhd'),compressed movie atom('cmov')和referencemovie atom ('rmra')。没有压缩的movie header atom必须至少包含movie header
 atom 和referencemovie atom中的一种。也可以包含其他的atom，例如一个clippingatom ('clip')，一个或几个trackatoms ('trak')，一个colortable atom ('ctab')，和一个userdata atom ('udta')。其中movieheader atom定义了整部电影的time scale，duration信息以及displaycharacteristics。track atom定义了电影中一个track的信息。Track就是电影中可以独立操作的媒体单位，例如一个声道就是一个track。
   2.1  mvhd
![](https://img-blog.csdn.net/20130831134854187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831135046390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130831134857703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130831135335968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
      电影的长度 = duration / time scale
     2.2 track
        一个Track atom定义了movie中的一个track。一部movie可以包含一个或多个tracks，它们之间相互独立，各自有各自的时间和空间信息。每个track atom 都有与之关联的media atom。
Track主要用于以下目的：
·        包含媒体数据引用和描述(media tracks)
·        包含modifier tracks (tweens等)
·        对于流媒体协议的打包信息(hint tracks)。Hint tracks可以引用或者复制对应的媒体sample data。
       Hinttracks和modifiertracks必须保证完整性，同时和至少一个media track一起存在。换句话说，即使hint tracks复制了对应的媒体sample data，media tracks也不能从一部hinted movie中删除。
     Track atoms 的atom类型是'trak'. Track atom要求必须有一个track header atom ('tkhd')和一个media atom ('mdia')。其他的track clipping atom ('clip')，track matte atom ('matt')，edit atom ('edts')，track reference atom ('tref')，track load settings atom
 ('load')，a track input map atom ('imap')以及user data atom ('udta')都是可选的。
![](https://img-blog.csdn.net/20130831135820437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        2.2.1   tkhd
           每个trak都包含了一个track header atom. Thetrack header atom 定义了一个track的特性，例如时间，空间和音量信息，它的类型是('tkhd').
![](https://img-blog.csdn.net/20130831140052421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831140134890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831140522078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130831140243796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                             ( video)       左边为video 解析出来的字段值   
                   可以获取图像的宽、 高等信息
![](https://img-blog.csdn.net/20130831140645812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                               (audio)  右边为audio解析出来的字段值
                 2.2. 2   mdia
             Mediaatoms定义了track的媒体类型和sample数据，例如音频或视频，描述sample数据的media handler component，media timescale and track duration以及media-and-track-specific 信息，例如音量和图形模式。它也可以包含一个引用，指明媒体数据存储在另一个文件中。也可以包含一个sample table atoms，指明sample description, duration, andbyte offset
 from the data reference for each media sample.
Mediaatom 的类型是'mdia'。它是一个容器atom，必须包含一个mediaheader atom ('mdhd')，一个handlerreference ('hdlr')，一个媒体信息引用('minf')和用户数据atom('udta').
![](https://img-blog.csdn.net/20130831141012687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
            1  mdhd  (Media head atom)
Mediaheader atom 定义了媒体的特性，例如time scale和duration。它的类型是'mdhd'.
![](https://img-blog.csdn.net/20130831141337656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831141528125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831141658406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
   video 的播放时长 = duration / time scale              audio 的播放时长也可计算。   video audio  可以不一样
                2  hdrl  
Handlerreference atom 定义了描述此媒体数据的mediahandler component，类型是'hdlr'。在过去，handler reference atom也可以用来数据引用，但是现在，已经不允许这样使用了。一个media atom内的handler atom解释了媒体流的播放过程。例如，一个视频handler处理一个video track.
![](https://img-blog.csdn.net/20130831142152281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831142241062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831142256234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20130831142534062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831142549875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        可知  音视频 采用的压缩方法
                3、  MINF
Mediainformation atoms的类型是'minf'，存储了解释该track的媒体数据的handler-specific的信息。media handler用这些信息将媒体时间映射到媒体数据，并进行处理。它是一个容器atom，包含其他的子atom。
这些信息是与媒体定义的数据类型特别对应的，而且media information atoms 的格式和内容也是与解释此媒体数据流的media handler 密切相关的。其他的media handler不知道如何解释这些信息。
![](https://img-blog.csdn.net/20130831142917281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831142924953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                     a  vmhd   smhd
                     Video  media information atoms是视频媒体的第一层atoms，包含其他的定义视频媒体数据的特性。  
                     Sound media information atoms是音频媒体的第一层atoms，包含其他的定义音频媒体数据的特性。
                      video  和 audio 分别为：            
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|vmhd|
|版本|1|这个atom的版本|
|标志|3|这里总是0x000001|
|图形模式|2|The transfer mode. The  transfer mode specifies which Booleanoperation  QuickDrawshould performwhen drawing or transferring an image fromone location  to another.|
|Opcolor|6|Three 16-bit values  that specify the red, green, and blue colors for the transfer mode operation  indicated in the graphics mode field.|
![](https://img-blog.csdn.net/20130831143741171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
|字段|长度(字节)|描述|
|----|----|----|
|尺寸|4|这个atom的字节数|
|类型|4|smhd|
|版本|1|这个atom的版本|
|标志|3|这里为0|
|均衡|2|音频的均衡是用来控制计算机的两个扬声器的声音混合效果，一般是0。一般值是0。|
|保留|2|保留字段，缺省为0|
![](https://img-blog.csdn.net/20130831143824453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
               b   dinf
handler reference定义data handler component如何获取媒体数据，data handler用这些数据信息来解释媒体数据。Data information atoms的类型是'dinf'。它是一个容器atom，包含其他的子atom。
![](https://img-blog.csdn.net/20130831144256234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831144303750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130831144310937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHhtbmV0MTIz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
