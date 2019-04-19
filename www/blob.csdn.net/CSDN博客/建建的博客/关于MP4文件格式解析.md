# 关于MP4文件格式解析 - 建建的博客 - CSDN博客
2017年08月10日 14:12:25[纪建](https://me.csdn.net/u013898698)阅读数：359
**MP4 **文件格式
综述
关键概念
        MP4 文件格式中，所有的内容存在一个称为**movie**的容器中。一个**movie **可以由多个**track**s 组成。每个**track**就是一个随时间变化的媒体序列，例如，视频帧序列。**track **里的每个时间单位是一个**sample**，它可以是一帧视频，或者音频。**sample**按照时间顺序排列。注意，一帧音频可以分解成多个音频**sample**，所以音频一般用**sample **作为单位，而不用帧。MP4
 文件格式的定义里面，用**sample **这个单词表示一个时间帧或者数据单元。每个**track**会有一个或者多个**sample description**s。**track **里面的每个**sample**通过引用关联到一个**sample description**。这个**sample description**s
 定义了怎样解码这个**sample**，例如使用的压缩[算法](http://lib.csdn.net/base/datastructure)。与其他的多媒体文件格式不同的是，MP4 文件格式经常使用几个不同的概念，理解其不同是理解这个文件格式的关键。
        这个文件的物理格式没有限定媒体本身的格式。例如，许多文件格式将媒体数据分成帧，头部或者其他数据紧紧跟随每一帧视频，！！！TODO（例如MPEG2）。而MP4 文件格式不是如此。文件的物理格式和媒体数据的排列都不受媒体的时间顺序的限制。视频帧不需要在文件按时间顺序排列。这就意味着如果文件中真的存在这样的一些帧，那么就有一些文件结构来描述媒体的排列和对应的时间信息。
        MP4 文件中所有的数据都封装在一些**box **中（以前叫**atom**）。所有的metadata(媒体描述元数据)，包括定义媒体的排列和时间信息的数据都包含在这样的一些结构**box**中。MP4 文件格式定义了这些这些**box **的格式。Metadata
 对媒体数据（例如，视频帧）引用说明。媒体数据可以包含在同一个的一个或多个**box**里，也可以在其他文件中，metadata 允许使用URLs 来引用其他的文件，而媒体数据在这些引用文件中的排列关系全部在第一个主文件中的metadata 描述。其他的文件不一定是MP4 文件格式，例如，可能就没有一个box。
        有很多种类的**track**，其中有三个最重要，**video track**包含了视频**sample**；**audio track**包含了audio **sample**；**hint track **稍有不同，它描述了一个流媒体服务器如何把文件中的媒体数据组成符合流媒体协议的数据包。如果文件只是本地播放，可以忽略**hint track**，他们只与流媒体有关系。
**媒体文件的物理结构**
**        Box**定义了如何在sample table 中找到媒体数据的排列。这包括data reference(数据引用), the sample size table, the sample to chunk table, and the chunkoffset table. 这些表就可以找到track 中每个sample在文件中的位置和大小。
**        data reference**允许在第二个媒体文件中找到媒体的位置。这样，一部电影就可以由一个媒体[数据库](http://lib.csdn.net/base/mysql)中的多个不同文件组成，而且不用把它们全部拷贝到另一个新文件中。例如，对视频编辑就很有帮助。为了节约空间，这些表都很紧凑。另外，interleave不是sample
 by sample，而是把单个track 的几个samples 组合到一起，然后另外几个sample 又进行新的组合，等等。一个track 的连续几个sample 组成的单元就被称为**chunk**。每个chunk 在文件中有一个偏移量，这个偏移量是从文件开头算起的，在这个chunk 内，sample 是连续存储的。这样，如果一个chunk 包含两个sample，第二个sample 的位置就是chunk 的偏移量加上第一个sample 的大小。chunk offset table
 说明了每个chunk 的偏移量，sample to chunk table说明了sample 序号和chunk 序号的映射关系。
注意chunk 之间可能会有死区，没有任何媒体数据引用到这部分区域，但是chunk 内部不会有这样的死区。这样，如果在节目编辑的时候，不需要一些媒体数据，就可以简单的留在那里，而不用引用，这样就不用删除它们了。类似的，如果媒体存放在第二个文件中，但是格式不同于MP4 文件格式，这个陌生文件的头部或者其他文件格式都可以简单忽略掉。
**Temporal structure of the media**
文件中的时间可以理解为一些结构。电影以及每个track 都有一个**timescale**。它定义了一个时间轴来说明每秒钟有多少个ticks。合理的选择这个数目，就可以实现准确的计时。一般来说，对于audiotrack，就是audio 的sampling rate。对于video track，情况稍微复杂，需要合理选择。例如，如果一个media TimeScale是30000，media sample
 durations 是1001，就准确的定义了NTSC video 的时间格式（虽然不准确，但一般就是29.97），and provide 19.9 hours of time in 32bits.
Track 的时间结构受一个**edit list **影响，有两个用途：全部电影中的一个track 的一部分时间片断变化（有可能是重用）；空白时间的插入，也就是空的edits。特别注意的是如果一个track不是从节目开头部分开始，edit list 的第一个edit 就一定是空的edit。每个track的全部duration 定义在文件头部，这就是对track 的总结，每个sample 有一个规定的**duration**。一个sample
 的准确描述时间，也就是他的时间戳(time-stamp)就是以前的sample 的duration 之和。
**Interleave**
文件的时间和物理结构可以是对齐的，这表明媒体数据在容器中的物理顺序就是时间顺序。另外，如果多个track 的媒体数据包含在同一个文件中，这个媒体数据可以是interleaved。一般来说，为了方便读取一个track 的媒体数据，同时保证每个表紧凑，以一个合适的时间间隔（例如1 秒）做一次interleave，而不是sample by sample。这样就可以减少chunk 的数据，减小chunk offset table 的大小。
**Composition**
如果多个audio track 包含在同一个文件中，他们有可能被混合在一起进行播放，并且由一个总track**volume **和左/右**balance**控制。
类似的，video track 也可以根据各自的层次序列号（从后向前）和合成模式进行混合。另外，每个track 可以用一个matrix 进行变换，也可以全部电影用一个matrix 进行变换。这样既可以进行简单操作（例如放大图像，校正90º旋转），也可以做更复杂的操作（例如shearing,arbitrary rotation）。这个混合方法只是非常简单，是一个缺省的方法，MPEG4 的另一份文档会定义更强有力的方法（例如MPEG-4
 BIFS）。
MP4文件的格式是一层一层嵌套的。
**1、最上一层是Movie atom。**
        Movie atom 定义了一部电影的数据信息。它的类型是'moov'，是一个容器atom，至少必须包含三种atom 中的一种—movie header atom('mvhd'), compressed movie atom('cmov')和reference movie atom ('rmra')。没有压缩的movieheader atom 必须至少包含movie header atom 和reference movie atom中的一种。也可以包含其他的atom，例如一个clipping
 atom ('clip')，一个或几个track atoms ('trak')，一个color table atom ('ctab')，和一个user data atom('udta')。其中movie header atom 定义了整部电影的time scale，duration 信息以及display characteristics。track atom 定义了电影中一个track 的信息。Track 就是电影中可以独立操作的媒体单位，例如一个声道就是一个track。Compressed movie atoms
 和reference movie atoms 不太使用，不在本文讨论范围内。本文主要讨论uncompressedmovie atoms。
**1-1、Movie Header Atoms - MVHD。是Movie atom的子层**
       Movie headeratom 定义了整个movie 的特性，例如time scale 和duration，它的atom 类型是'mvhd'。一般这一层没有太多有用的信息，基本不用解析。直接去解析trak。
1-2、**Track Atoms - TRAK,****是Movie atom的子层**
       一个Track atom 定义了movie 中的一个track。一部movie可以包含一个或多个tracks，它们之间相互独立，各自有各自的时间和空间信息。每个track atom 都有与之关联的media atom。Track 主要用于以下目的：
       •包含媒体数据引用和描述(media tracks)
       •包含modifier tracks (tweens 等)
       •对于流媒体协议的打包信息(hint tracks)。Hint tracks 可以引用或者复制对应的媒体sample data。
       Hint tracks 和modifier tracks 必须保证完整性，同时和至少一个mediatrack 一起存在。换句话说，即使hint tracks 复制了对应的媒体sample data，media tracks 也不能从一部hinted movie 中删除。
       Track atoms 的atom 类型是'trak'. Track atom 要求必须有一个track headeratom ('tkhd') 和一个media atom ('mdia')。其他的track clipping atom ('clip')，track matteatom ('matt')，edit atom ('edts')，track reference atom ('tref')，track loadsettings atom ('load')，a track input map
 atom ('imap')以及user data atom ('udta')都是可选的。
       Track atoms 是一个容器atom，本身没有特别的字段，需要子atom 来进一步说明有效的内容。每个trak 都包含了一个track header atom.
**1-2-1、Track Header Atoms - TKHD，trak的子层。**
       每个trak 都包含了一个trackheader atom. The track header atom 定义了一个track 的特性，例如时间，空间和音量信息，它的类型是('tkhd').
**1-2-2、Edit Atoms - EDTS，trak的子层**
       Edit atoms 定义了创建movie 中一个track 的一部分媒体。所有的edit 都在一个表里面，包括每一部分的时间偏移量和长度。Edit atoms 的类型是'edts'。如果没有该表，则此track 会被立即播放。一个空的edit用来偏移track 的起始时间。
       如果没有edit atom 或editlist atom，则此track 使用全部媒体。Editatoms 是一个容器atom，本身没有特别的字段，需要子atom来进一步说明有效的内容。
Ps：这一层atom基本没什么用，跳过不用解析。
**1-2-2-1、Edit List Atoms - ELST，edts的子层**
       Edit list atom 用来映射movie 的时间到此track media 的时间。所有信息在一个edit list 表中，见上图。Edit list atoms 的类型是'elst'.
**1-2-3、Track Reference Atoms - TREF，TRAK的子层**
       没有太多用处，不做讨论.类型是'tref'.
**1-2-4、Media Atoms - MDIA，trak的子层**
       Media atoms 定义了track 的媒体类型和sample 数据，例如音频或视频，描述sample 数据的media handler component，media timescale and track duration 以及media-and-track-specific信息，例如音量和图形模式。它也可以包含一个引用，指明媒体数据存储在另一个文件中。也可以包含一个sampletable atoms，指明sample description,duration, and byteoffset
 from the data reference for each media sample.
       Media atom 的类型是'mdia'。它是一个容器atom，必须包含一个media header atom ('mdhd')，一个handlerreference ('hdlr')，一个媒体信息引用('minf')和用户数据atom('udta').
**1-2-4-1、Media Header Atoms - MDHD,MDIA子层**
       Media header atom 定义了媒体的特性，例如time scale 和duration。它的类型是'mdhd'.一般没什么用，不解析。
**1-2-4-2、Handler Reference Atoms - HDLR，mdia子层**
       Handler reference atom 定义了描述此媒体数据的media handler component，类型是'hdlr'。在过去，handler reference atom 也可以用来数据引用，但是现在，已经不允许这样使用了。一个mediaatom 内的handler atom 解释了媒体流的播放过程。例如，一个视频handler 处理一个video track。
**1-2-4-3、Media Information Atoms - MINF，mdia子层**
       Media information atoms 的类型是'minf'，存储了解释该track 的媒体数据的handler-specific 的信息。media handler 用这些信息将媒体时间映射到媒体数据，并进行处理。它是一个容器atom，包含其他的子atom。
       这些信息是与媒体定义的数据类型特别对应的，而且mediainformation atoms 的格式和内容也是与解释此媒体数据流的media handler 密切相关的。其他的media handler 不知道如何解释这些信息。
**1-2-4-3、Video Media Information Atoms，即minf层**
       Video media information atoms 是视频媒体的第一层atoms，包含其他的定义视频媒体数据的特性。
**1-2-4-3、Sound Media Information Atoms，即minf层**
       Sound media information atoms 是音频媒体的第一层atoms，包含其他的定义音频媒体数据的特性。
**1-2-4-3-1、Video Media Information Header Atoms,minf子层**
       Video media information header atoms 定义颜色和图形模式信息。
**1-2-4-3-2、Sound Media Information Header Atoms,minf子层**
       The sound media information header atom 定义了声音媒体的控制信息，例如均衡。
**1-2-4-3-3、Data Information Atoms - DINF，minf子层**
       handler reference 定义data handler component 如何获取媒体数据，data handler 用这些数据信息来解释媒体数据。Data informationatoms 的类型是'dinf'。它是一个容器atom，包含其他的子atom。
**1-2-4-3-3-1、Data Reference Atoms--DREF，dinf子层**
       Data reference atoms 包含列表数据，data handler component 可以用这些数据获取媒体数据。
**1-2-4-3-4、Sample Atoms介绍**
       存储媒体数据的单位是samples。一个sample 是一系列按时间顺序排列的数据的一个element。Samples 存储在media 中的chunk内，可以有不同的durations。Chunk 存储一个或者多个samples，是数据存取的基本单位，可以有不同的长度，一个chunk 内的每个sample 也可以有不同的长度。例如如下图，chunk 2 和3 不同的长度，chunk 2 内的sample5 和6 的长度一样，但是sample 4 和5，6的长度不同。
       sample table atom 描述sample 的所有信息以及一些不同类型的atoms，media handler 可以用这些信息正确的按顺序解析所有的samples，而不需要强迫这些数据按movie 的时间顺序存放到实际数据流中。
       • 如何得到chunk 的数目和每个chunk 包含多少个sample，每个chunk的description 是如何解析stsc -Sample-to-Chunk table。这个表类似于行程编码，第一个first chunk 减去第二个first chunk就是一共有多少个trunk 包含相同的sample 数目，这样通过不断的叠加，就可以得到一共有280个trunk，每个trunk 包含多少个sample，以及每个trunk 对应的description。
       • 如何计算每个sample 的duration
       • 如何计算每个sample 的长度
       解析stsz - Sample Size table。这个表包含了每个sample 的长度，找到sample 的序号，就可以找到对应sample 的长度了。
**1-2-4-3-4、Sample Table Atoms--STBL，minf子层**
       sample table atom 包含转化媒体时间到实际的sample 的信息，他也说明了解释sample 的信息，例如，视频数据是否需要解压缩，解压缩算法是什么？它的类型是'stbl'，是一个容器atom，包含sample description atom,time-to-sample atom, sync sample atom, sample-to-chunk atom, sample sizeatom,chunk offset atom 和shadow sync atom.sample
 tableatom 包含track 中media sample 的所有时间和数据索引，利用这个表，就可以定位sample 到媒体时间，决定其类型，大小，以及如何在其他容器中找到紧邻的sample。
       如果sample table atom 所在的track 没有引用任何数据，那么它就不是一个有用的media track，不需要包含任何子atom。
       如果sample table atom 所在的track 引用了数据，那么必须包含以下的子atom：sample description, sample size, sample to chunk 和chunk offset。所有的子表有相同的sample数目。
       sample description atom 是必不可少的一个atom，而且必须包含至少一个条目，因为它包含了数据引用atom 检索media sample 的目录信息。没有sample description，就不可能计算出mediasample 存储的位置。sync sample atom 是可选的，如果没有，表明所有的samples 都是sync samples。
**1-2-4-3-4-1、Sample Description Atoms - STSD，stbl子层**
       利用sample description atom 存储的信息可以正确的解码media sample。不同的媒体类型存储不同的sample description，例如，视频媒体，sample description 就是图像的结构。第四章解释了不同媒体类型对应的sampledescription 信息。
       sample description atom 的类型是'stsd'，包含了一个sample description 表。根据不同的编码方案和存储数据的文件数目，每个media可以有一个到多个sample description。sample-to-chunkatom 通过这个索引表，找到合适medai 中每个sample的description。
**1-2-4-3-4-2、Time-to-Sample Atoms - STTS，stbl子层**
       Time-to-sample atoms 存储了media sample 的duration 信息，提供了时间对具体data sample的映射方法，通过这个atom，你可以找到任何时间的sample，类型是'stts'。
       这个atom 可以包含一个压缩的表来映射时间和sample 序号，用其他的表来提供每个sample 的长度和指针。表中每个条目提供了在同一个时间偏移量里面连续的sample 序号，以及samples 的偏移量。递增这些偏移量，就可以建立一个完整的time-to-sample 表，计算公式如下
DT(n+1) = DT(n) + STTS(n)
       其中STTS(n)是没有压缩的STTS第n 项信息，DT 是第n 个sample 的显示时间。Sample的排列是按照时间戳的顺序，这样偏移量永远是非负的。DT 一般以0开始，如果不为0，edit list atom 设定初始的DT值。DT 计算公式如下
DT(i) = SUM (for j=0 to i-1 of delta(j))
       所有偏移量的和就是track 中media 的长度，这个长度不包括media 的time scale，也不包括任何edit list。
       如果多个sample 有相同的duration，可以只用一项描述所有这些samples，数量字段说明sample 的个数。例如，如果一个视频媒体的帧率保持不变，整个表可以只有一项，数量就是全部的帧数。
**1-2-4-3-4-3、Sync Sample Atoms - STSS，stbl子层**
       sync sample atom 确定media 中的关键帧。对于压缩的媒体，关键帧是一系列压缩序列的开始帧，它的解压缩是不依赖于以前的帧。后续帧的解压缩依赖于这个关键帧。
       sync sample atom 可以非常紧凑的标记媒体内的随机存取点。它包含一个sample 序号表，表内的每一项严格按照sample 的序号排列，说明了媒体中的哪一个sample 是关键帧。如果此表不存在，说明每一个sample 都是一个关键帧，是一个随机存取点。
       Sync sample atoms 的类型是'stss'。
**1-2-4-3-4-4、Sample-to-Chunk Atoms - STSC，stbl子层**
       当添加samples 到media 时，用chunks 组织这些sample，这样可以方便优化数据获取。一个trunk 包含一个或多个sample，chunk的长度可以不同，chunk 内的sample 的长度也可以不同。sample-to-chunk atom 存储sample 与chunk 的映射关系。
       Sample-to-chunk atoms 的类型是'stsc'。它也有一个表来映射sample 和trunk 之间的关系，查看这张表，就可以找到包含指定sample 的trunk，从而找到这个sample。
1-2-4-3-4-5、Sample Size Atoms - STSZ，stbl子层
       sample size atoms 定义了每个sample的大小，它的类型是'stsz'，包含了媒体中全部sample的数目和一张给出每个sample大小的表。这样，媒体数据自身就可以没有边框的限制。      
**1-2-4-3-4-6、Chunk Offset Atoms - STCO，stbl子层**
        Chunk offset atoms 定义了每个trunk 在媒体流中的位置，它的类型是'stco'。位置有两种可能，32 位的和64 位的，后者对非常大的电影很有用。在一个表中只会有一种可能，这个位置是在整个文件中的，而不是在任何atom 中的，这样做就可以直接在文件中找到媒体数据，而不用解释atom。需要注意的是一旦前面的atom 有了任何改变，这张表都要重新建立，因为位置信息已经改变了。
**获取mp4 文件信息**
**2-1.计算电影图像宽度和高度**
        tkhd – track header atom中找到宽度和高度即是。
**2-2. 计算电影声音采样频率**
        从tkhd – track header atom 中找出audio track 的time scale 即是声音的采样频率。
**2-3. 计算视频帧率**
        首先计算出整部电影的duration，和帧的数目然后   帧率=整部电影的duration / 帧的数目
**2-4. 计算电影的比特率**
        整部电影的尺寸除以长度，即是比特率，此电影的比特率为846623/70= 12094 bps
**2-5. 查找sample**
        当播放一部电影或者一个track的时候，对应的media handler 必须能够正确的解析数据流，对一定的时间获取对应的媒体数据。如果是视频媒体，media handler 可能会解析多个atom，才能找到给定时间的sample 的大小和位置。具体步骤如下：
        1．确定时间，相对于媒体时间坐标系统
        2．检查time-to-sample atom来确定给定时间的sample 序号。
        3．检查sample-to-chunk atom来发现对应该sample 的chunk。
        4．从chunk offset atom中提取该trunk 的偏移量。
        5．利用sample size atom找到sample在trunk 内的偏移量和sample 的大小。例如，如果要找第1 秒的视频数据，过程如下：
        5-1． 第1秒的视频数据相对于此电影的时间为600
        5-2． 检查time-to-sample atom，得出每个sample 的duration 是40，从而得出需要寻找第600/40= 15 + 1 = 16 个sample
        5-3． 检查sample-to-chunk atom，得到该sample 属于第5 个chunk 的第一个sample，该chunk 共有4 个sample
        5-4． 检查chunk offset atom找到第5 个trunk 的偏移量是20472
        5-5． 由于第16个sample 是第5 个trunk 的第一个sample，所以不用检查sample size atom，trunk 的偏移量即是该sample 的偏移量20472。如果是这个trunk 的第二个sample，则从samplesize atom 中找到该trunk 的前一个sample 的大小，然后加上偏移量即可得到实际位置。
        5-6． 得到位置后，即可取出相应数据进行解码，播放
**2.6. 查找关键帧**
        查找过程与查找sample的过程非常类似，只是需要利用sync sample atom 来确定key frame 的sample 序号
        1. 确定给定时间的sample序号
        2. 检查sync sample atom来发现这个sample序号之后的key frame
        3. 检查sample-to-chunk atom来发现对应该sample 的chunk
        4. 从chunk offset atom中提取该trunk的偏移量
        5. 利用sample size atom找到sample在trunk 内的偏移量和sample 的大小
**2.7. Random access**
        Seeking 主要是利用sample table box 里面包含的子box 来实现的，还需要考虑edit list 的影响。可以按照以下步骤seek 某一个track 到某个时间T，注意这个T 是以movie header box 里定义的time scale 为单位的：
        1.如果track有一个edit list，遍历所有的edit，找到T 落在哪个edit 里面。将Edit开始时间变换为以movie time scale 为单位，得到EST，T 减去EST，得到T'，就是在这个edit里面的duration，注意此时T'是以movie 的timescale 为单位的。然后将T'转化成track媒体的time scale，得到T''。T''与Edit
 的开始时间相加得到以track 媒体的time scale 为单位的时间点T'''。
2.这个track 的time-to-sample 表说明了该track 中每个sample 对应的时间信息，利用这个表就可以得到T'''对应的sample NT。
3.sample NT 可能不是一个random access point，这样就需要其他表的帮助来找到最近的random access point。一个表是sync sample 表，定义哪些sample 是random access point。使用这个表就可以找到指定时间点最近的sync sample。如果没有这个表，就说明所有的sample 都是synchronization points，问题就变得更容易了。另一个shadowsync
 box 可以帮助内容作者定义一些特殊的samples，它们不用在网络中传输，但是可以作为额外的random access point。这就改进了random access，同时不会影响正常的传输比特率。这个表指出了非random access point 和random access point 之间的关系。如果要寻找指定sample 之前最近的shadow sync sample，就需要查询这个表。总之，利用sync sample 和shadow sync 表，就可以seek 到NT 之前的最近的accesspoint
 sample Nap。
4.找到用于access point 的sample Nap 之后，利用sample-to-chunk 表来确定sample 位于哪个chunk 内。
5. 找到chunk 后，使用chunk offset 找到这个chunk 的开始位置。
6. 使用sample-to-chunk 表和sample size 表中的数据，找到Nap 在此chunk 内的位置，再加上此chunk 的开始位置，就找到了Nap 在文件中的位置。
