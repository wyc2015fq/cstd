# Tair LDB基于Prefixkey找到如何提取一系列性能优化项目key的prefix_size - weixin_33985507的博客 - CSDN博客
2015年07月17日 16:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6

眼下项目已快截止，编码任务也基本完毕。如今主要是性能測试。
项目是依照[“Tair LDB基于Prefixkey的范围查找性能优化项目提议方案”](http://blog.csdn.net/lanxuezaipiao/article/details/38362673)的步骤一步步完毕的，首先先介绍第一个关键问题是怎样解决的。
在提案中有以下描写叙述：
> 
因为getrange接口的数据是从prefixput/prefixincr接口进来的。那么prefix的长度信息就能够从它们的pkey參数得到。pkey的数据类型是dataentry，有属性prefixsize，那么我们在client将pkey和skey合并为mkey（已经设置mkey的prefixsize为pkey的size）后与value一起传送到server端。
在client与server端的连接过程中，将key的类型封装成LdbKey类。value的类型封装成LdbItem类，LdbItem里面含有key的prefixsize信息，然后两者都转化为Slice类型发送到leveldb底层进行存储操作。
注意此时value里面包括了prefixszie信息（序列化信息，不能直接提取），因此我们在生成filter block时能够从value中提取出prefix_size信息（按LdbItem的格式进行分析提取）以生成我们所须要的prefix bloomfilter。
提取的详细实现能够放在leveldb层的外面，在leveldb里面进行调用就可以（分离操作）。
这里面提到一个关键信息：key的prefix_size信息在db中是存储在value中而不是在key中.
那么value的格式是什么样的呢？
首先value的内容是由LdbItem的数据得到的，知道了LdbItem里数据的存储格式也就知道了value的数据存储格式。
LdbItem里data_的数据构成是由以下这个set函数完毕的：
// meta_ MUST already be set correctlyvoidset(constchar*value_data,constint32_tvalue_size){if(value_data!=NULL&&value_size>){char*metap=reinterpret_cast<char*>(&meta_);intreal_meta_size=LDB_ITEM_META_BASE_SIZE;LdbItemMetaBase*metabp=reinterpret_cast<LdbItemMetaBase*>(&meta_);free();if(metabp->flag_&TAIR_ITEM_FLAG_NEWMETA){if(META_VER_PREFIX==metabp->meta_version_)real_meta_size=LDB_ITEM_META_SIZE;elseif(META_VER_BASE==metabp->meta_version_)real_meta_size=LDB_ITEM_META_BASE_SIZE;}data_size_=value_size+real_meta_size;data_=newchar[data_size_];memcpy(data_,metap,real_meta_size);memcpy(data_+real_meta_size,value_data,value_size);alloc_=true;}}
能够知道data_的内容由两部分构成：
```
| LdbItemMeta数据 | 真实的value内容 |
```
或者
```
| LdbItemMetaBase数据 | 真实的value内容 |
```
两者的差别在于LdbItemMeta数据包括prefix_size信息而LdbItemMetaBase数据不包括。这通过这两个简单的数据结构组成部分就知道了。
structLdbItemMetaBase{LdbItemMetaBase():meta_version_(),flag_(),version_(),cdate_(),mdate_(),edate_(){}uint8_tmeta_version_;// meta data versionuint8_tflag_;// flaguint16_tversion_;// versionuint32_tcdate_;// create timeuint32_tmdate_;// modify timeuint32_tedate_;// expired time(for meta when get value. dummy with key)};structLdbItemMeta// change value() and set() ,if you want to add new metadata{LdbItemMeta():prefix_size_(){}structLdbItemMetaBasebase_;uint16_tprefix_size_;// prefix key size(for getRange conflict detect)uint16_treserved;//};
因此假设value的第一部分是LdbItemMeta数据，就说明它包括prefixsize信息，我们就能够将value内容按LdbItemMeta的格式进行解析。从而提取出当中的prefixsize信息。
以下是详细的解析提取程序：
// get prefix size from slice value contentintget_prefix_size(constleveldb::Slice&value){// parse LdbItemMetaBase from valuechar*val=const_cast<char*>(value.data());LdbItemMeta*metap=reinterpret_cast<LdbItemMeta*>(val);// check if prefix is setif(metap->base_.flag_&TAIR_ITEM_FLAG_NEWMETA){// if prefix is set, parse it from LdbItemMeta. if not, return 0if(META_VER_PREFIX==metap->base_.meta_version_){returnmetap->prefix_size_;}else{return;}}return;}
版权声明：本文博客原创文章。博客，未经同意，不得转载。
