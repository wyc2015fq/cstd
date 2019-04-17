# FFMPEG学习【libavutil】：数据结构（一） - 一世豁然的专栏 - CSDN博客





2017年07月20日 07:58:39[一世豁然](https://me.csdn.net/Explorer_day)阅读数：370








一、AvBuffer

AVBuffer是用于引用计数数据缓冲区的API。


这个API有两个核心对象 - AVBuffer和AVBufferRef。 AVBuffer表示数据缓冲区本身; 它是不透明的，并不意味着直接由调用者访问，而只能通过AVBufferRef。 然而，呼叫者可以例如 比较两个AVBuffer指针，以检查两个不同的引用是否描述相同的数据缓冲区。 AVBufferRef表示对AVBuffer的单一引用，它是可以由调用者直接操纵的对象。


提供了两个功能，用于创建一个具有单个引用的新AVBuffer - av_buffer_alloc（）来分配新的缓冲区，并使用av_buffer_create（）将现有的数组包装在AVBuffer中。 从现有参考中，可以使用av_buffer_ref（）创建其他引用。 使用av_buffer_unref（）来释放引用（一旦所有引用被释放，这将自动释放数据）。


整个API中的惯例和FFmpeg的其余部分使得如果只存在一个引用（并且没有被标记为只读），缓冲区被认为是可写的。 提供了av_buffer_is_writable（）函数来检查是否为true，并且必要时，av_buffer_make_writable（）将自动创建一个新的可写缓冲区。 当然没有什么可以阻止调用代码违反这个约定，但是只有当所有现有的引用都在其控制之下时才是安全的。


注意：引用和引用缓冲区是线程安全的，因此可以同时从多个线程完成，而不需要额外的锁定。

对相同缓冲区的两个不同引用可以指向缓冲区的不同部分（即它们的AVBufferRef.data将不相等）。




一）、数据结构

对数据缓冲区的引用。


此结构体的大小不是公共ABI的一部分，并不意味着直接分配。



|struct|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html){AVBuffer * buffer；uint8_t * data；//数据缓冲区。当且仅当这是缓冲区的引用时才被认为是可写的，在这种情况下，av_buffer_is_writable（）返回1。int size；//数据大小（以字节计）}|
|----|----|





二）、宏

#define AV_BUFFER_FLAG_READONLY（1 << 0）


始终将缓冲区视为只读，即使只有一个引用。








三）、函数


|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffer_alloc](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga5ae84ac902283dea1a660ede06c1cdac) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

使用av_malloc（）分配给定大小的AVBuffer。

返回：一个给定大小的AVBufferRef或当内存不足时为空





|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffer_allocz](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga27f37cc004f768b00442b6cb08091686) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

与av_buffer_alloc（）相同，但返回的缓冲区将被初始化为零。





|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffer_create](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga3e7f4c92db85ccba0b2a16dd0069842a) ([uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)(*free)([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1)), [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](https://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

从现有阵列创建AVBuffer。

如果此函数成功，则数据由AVBuffer拥有。 调用者只能通过返回的AVBufferRef和从其派生的引用来访问数据。 如果此功能失败，数据将保持不变。


参数：data：数据数组

   size：数据大小（以字节为单位）

   free：一个释放缓冲区数据的回调

   opaque：要加工或传递给自由的参数

   flags：组合AV_BUFFER_FLAG_ *

返回：AVBufferRef参考成功的数据，失败时为NULL。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_buffer_default_free](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga37d620e8cfc9f2f3261c2b4c5d4adfe0) ([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [uint8_t](https://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[data](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a35090c370ccd7636420cba1acc908df1))|
|----|----|

默认的释放回调函数，它调用缓冲区数据上的av_free（）。

这个函数是传递给av_buffer_create（），不直接调用。






|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffer_ref](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#gaa40ce7d3ede946a89d03323bbd7268c1) ([AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|

创建对AVBuffer的新引用。

返回：一个新的AVBufferRef引用与buf相同的AVBuffer或失败时为NULL。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_buffer_unref](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga135e9e929b5033bb8f68322497b2effc) ([AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) **[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|

释放给定的引用，如果没有更多的引用，则自动释放缓冲区。

参数：buf：引用被释放。 指针在返回时设置为NULL。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffer_is_writable](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga060be34ace567ae378fd0a786e847053) (const [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|

返回：1如果调用者可以写入buf引用的数据（当且仅当buf是对基础AVBuffer的唯一引用时才是真实的）。 否则返回0。 一个肯定的回答是有效的，直到在buf上调用av_buffer_ref（）。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_buffer_get_opaque](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#gac1c38f469fc9933885cfffafd8cbf0d6) (const [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|

返回：由av_buffer_create设置的opaque参数。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffer_get_ref_count](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga7b16c3976dc555f71d0843bbd023dfcd) (const [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|



|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffer_make_writable](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#ga9c2a1be1b7bb80eec8613fdb62a19074) ([AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) **[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604))|
|----|----|

从给定的缓冲区引用创建可写参考，避免数据复制。

参数：buf：缓冲区引用使可写。 在成功时，buf或者保持不变，或者是未引用的，并且一个新的可写AVBufferRef写在它的位置。 失败时，buf保持不变。

返回：0成功，否定AVERROR失败。





|[int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_buffer_realloc](https://ffmpeg.org/doxygen/trunk/group__lavu__buffer.html#gac7c228f9ed54e246dcd270503bed7291) ([AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) **[buf](https://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a5bc5fa69bee375df074734a2c4858604), [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e))|
|----|----|

重新分配给定的缓冲区。

参数：buf：一个缓冲区引用重新分配。 在成功的情况下，buf将被未引用，并且具有所需大小的新参考将被写入其中。 失败buf将保持不变。 * buf可能为NULL，则分配一个新的缓冲区。

   size：需要新的缓冲区大小。

返回：0成功，否定AVERROR失败。

注意：如果最初通过av_buffer_realloc（NULL）分配缓冲区，并且只有一个引用（即传递给此函数的引用），则av_realloc（）才会实际使用av_realloc（）重新分配缓冲区。 在所有其他情况下，会分配一个新的缓冲区并复制数据。







二、AVBufferPool

AVBufferPool是AVBuffers无锁线程安全池的API。


经常分配和释放大型缓冲区可能很慢。 如果调用者需要一组相同大小的缓冲区（最明显的用例是原始视频或音频帧的缓冲区），则AVBufferPool旨在解决此问题。


开始时，用户必须调用av_buffer_pool_init（）来创建缓冲池。 然后每当需要缓冲区时，调用av_buffer_pool_get（）来获取对新缓冲区的引用，类似于av_buffer_alloc（）。 这个新引用与所有方面的工作方式与由av_buffer_alloc（）创建的引用相同。 但是，当对此缓冲区的最后一次引用未被引用时，它将返回到池而不是被释放，并将被重新用于后续的av_buffer_pool_get（）调用。


当调用者完成池并且不再需要分配任何新的缓冲区时，必须调用av_buffer_pool_uninit（）来将池标记为可以使用。 一旦释放所有缓冲区，它将自动释放。


只要使用默认的alloc回调，或用户提供的线程安全，就可以使用该API分配和释放缓冲区是线程安全的。





一）、函数


|[AVBufferPool](https://ffmpeg.org/doxygen/trunk/structAVBufferPool.html) *|[av_buffer_pool_init](https://ffmpeg.org/doxygen/trunk/group__lavu__bufferpool.html#ga26af9b07c08bc7fd2cfc62767e490b93) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *(*alloc)([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)))|
|----|----|

分配并初始化缓冲池。

参数：size：该池中每个缓冲区的大小

   alloc：当池为空时，将用于分配新缓冲区的函数。 可能为NULL，那么默认的分配器将被使用（av_buffer_alloc（））。

返回：新创建的缓冲池成功，NULL出错。





|[AVBufferPool](https://ffmpeg.org/doxygen/trunk/structAVBufferPool.html) *|[av_buffer_pool_init2](https://ffmpeg.org/doxygen/trunk/group__lavu__bufferpool.html#ga615151c8889d5be39d17bce4d8c65117) ([int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *(*alloc)([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [int](https://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e)), [void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)(*pool_free)([void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque))|
|----|----|

使用更复杂的分配器分配和初始化缓冲池。

参数：size：该池中每个缓冲区的大小

opaque：分配器使用的任意用户数据

   alloc： 当池为空时，将用于分配新缓冲区的函数。

pool_free：一个在池释放之前立即被调用的函数。即 之后，av_buffer_pool_uninit（）由调用者调用，所有的帧都返回到池中并被释放。 用于取消初始化用户不透明数据。

返回：新创建的缓冲池成功，NULL出错。





|[void](https://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_buffer_pool_uninit](https://ffmpeg.org/doxygen/trunk/group__lavu__bufferpool.html#ga7b76781c5944b3bb59b2e3b1a8994e4a) ([AVBufferPool](https://ffmpeg.org/doxygen/trunk/structAVBufferPool.html) **pool)|
|----|----|

将缓冲池标记为可用于释放。

只有释放与池相关联的所有分配的缓冲区，才能释放它。 因此，一些分配的缓冲区仍在使用中，可以安全地调用此函数。


参数：pool：指向要释放的池的指针。 它将被设置为NULL。





|[AVBufferRef](https://ffmpeg.org/doxygen/trunk/structAVBufferRef.html) *|[av_buffer_pool_get](https://ffmpeg.org/doxygen/trunk/group__lavu__bufferpool.html#ga2c745ecb4001452423e775b06b20fb7c) ([AVBufferPool](https://ffmpeg.org/doxygen/trunk/structAVBufferPool.html) *pool)|
|----|----|

分配一个新的AVBuffer，在可用时从池中重新使用一个旧的缓冲区。

该函数可以从多个线程同时调用。


返回：引用新的缓冲区成功，NULL出错。



