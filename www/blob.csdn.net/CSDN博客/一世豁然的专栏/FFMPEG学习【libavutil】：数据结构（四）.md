# FFMPEG学习【libavutil】：数据结构（四） - 一世豁然的专栏 - CSDN博客





2017年07月23日 17:11:48[一世豁然](https://me.csdn.net/Explorer_day)阅读数：419








一、AVDictionary

简单键：值存储。


字典用于存储键：值对。 要创建一个AVDictionary，只需将一个NULL指针的地址传递给av_dict_set（）。 NULL可以用作空字典，无论何处需要指向AVDictionary的指针。 使用av_dict_get（）来检索一个条目或迭代所有条目，最后使用av_dict_free（）来释放字典及其所有内容。




```cpp
AVDictionary *d = NULL;           // "create" an empty dictionary
AVDictionaryEntry *t = NULL;
av_dict_set(&d, "foo", "bar", 0); // add an entry
char *k = av_strdup("key");       // if your strings are already allocated,
char *v = av_strdup("value");     // you can avoid copying them like this
av_dict_set(&d, k, v, AV_DICT_DONT_STRDUP_KEY | AV_DICT_DONT_STRDUP_VAL);
while (t = av_dict_get(d, "", t, AV_DICT_IGNORE_SUFFIX)) {
    <....>                             // iterate over all entries in d
}
av_dict_free(&d);
```






一）、数据结构


|struct|[AVDictionaryEntry](http://ffmpeg.org/doxygen/trunk/structAVDictionaryEntry.html){char * key；char * value；}|
|----|----|





二）、宏


|#define|[AV_DICT_MATCH_CASE](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gad9cbc53cec515b72ae7caa2e194c6bc0)   1|
|----|----|

只能获得一个具有完全匹配关键字的条目。





|#define|[AV_DICT_IGNORE_SUFFIX](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga341bbd1ea599dfb0510f419e5272fecc)   2|
|----|----|

返回字典中的第一个条目，其第一部分对应于搜索关键字，忽略找到的关键字串的后缀。





|#define|[AV_DICT_DONT_STRDUP_KEY](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gacb0ce4f20c15bf174f93590ace0fc8fd)   4|
|----|----|

获取已分配给av_malloc（）或另一个内存分配函数的密钥的所有权。





|#define|[AV_DICT_DONT_STRDUP_VAL](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga7a9a991cb7837b10ffd44725263de425)   8|
|----|----|

获取已经使用av_malloc（）或另一个内存分配函数分配的值的所有权。





|#define|[AV_DICT_DONT_OVERWRITE](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga8ed4237acfc3d68484301a5d1859936c)   16|
|----|----|
不要覆盖现有条目。





|#define|[AV_DICT_APPEND](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga64018c01d1e8c44d1d8c03dfb099f2c2)   32|
|----|----|
如果条目已经存在，请附加到该条目。





|#define|[AV_DICT_MULTIKEY](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga14e9f9a502fbd2990028fc47b494dc34)   64|
|----|----|
允许在字典中存储几个相等的键。




三）、函数


|[AVDictionaryEntry](http://ffmpeg.org/doxygen/trunk/structAVDictionaryEntry.html) *|[av_dict_get](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gae67f143237b2cb2936c9b147aa6dfde3) (const [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *m, const char *key, const [AVDictionaryEntry](http://ffmpeg.org/doxygen/trunk/structAVDictionaryEntry.html) *prev, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

获取具有匹配键的字典条目。

返回的条目键或值不能更改，否则将导致未定义的行为。


要遍历所有字典条目，您可以将匹配键设置为空字符串“”，并设置AV_DICT_IGNORE_SUFFIX标志。


参数：prev：设置到先前匹配的元素以找到下一个。 如果设置为NULL，则返回第一个匹配元素。

  key：匹配键

  flags：控制如何检索条目的AV_DICT_ *标志的集合

返回：找到条目或NULL，以防在字典中找不到匹配条目





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_count](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gad21456d6601aa096c7c78dce8848d50a) (const [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *m)|
|----|----|

获取字典中的条目数。

参数：m：字典

返回：字典中的条目数





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_set](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga8d9c2de72b310cef8e6a28c9cd3acbbe) ([AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **pm, const char *key, const char *[value](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a1b2be7bfa84f129a2690a62f3ad82d62), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

设置* pm中的给定条目，覆盖现有条目。

注意：如果设置了AV_DICT_DONT_STRDUP_KEY或AV_DICT_DONT_STRDUP_VAL，这些参数将被错误地释放。


警告：将新条目添加到字典会使先前使用av_dict_get返回的所有现有条目无效。


参数：pm：指向字典结构体的指针。 如果* pm为NULL，则会分配一个字典结构并放入* pm。

  key：添加到* pm的条目键（将根据标志为av_strduped或添加为新的键）

  value：条目值添加到* pm（将被av_strduped或添加为一个新的密钥取决于标志）。 传递NULL值将导致现有条目被删除。

返回：> = 0，否则为错误代码<0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_set_int](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gafd013a88620b2da7d497b083f6ea7d29) ([AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **pm, const char *key, int64_t [value](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a1b2be7bfa84f129a2690a62f3ad82d62), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

av_dict_set的便利包装器将值转换为字符串并存储。

注意：如果设置了AV_DICT_DONT_STRDUP_KEY，则键将被释放。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_parse_string](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#gaca5ff7c251e60bd13164d13c82f21b79) ([AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **pm, const char *str, const char *key_val_sep, const char *pairs_sep, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

解析键/值对列表，并将解析的条目添加到字典中。

万一发生故障，所有成功设置的条目存储在* pm中。 您可能需要手动释放创建的字典。


参数：key_val_sep：一个0个终止的字符列表，用于将值与值分离

pairs_sep：用于将两个对彼此分离的0个字符的字符列表

  flags：添加到字典时要使用的标志。
 AV_DICT_DONT_STRDUP_KEY和AV_DICT_DONT_STRDUP_VAL将被忽略，因为键/值令牌将始终被复制。

返回：0成功，否则ERROR代码失败





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_copy](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga59a6372b124b306e3a2233723c5cdc78) ([AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **dst, const [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

将条目从一个AVDictionary结构复制到另一个。

参数：dst：指向AVDictionary结构体的指针。 如果* dst为NULL，则此函数将为您分配一个结构，并将其放在* dst中

  src：指向源AVDictionary结构的指针

  flags：在* dst中设置条目时要使用的标志

注意：使用AV_DICT_IGNORE_SUFFIX标志读取元数据

返回：0成功，否则ERROR代码失败。 如果dst由此功能分配，则呼叫者应释放相关联的内存。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_dict_free](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga1bafd682b1fbb90e48a4cc3814b820f7) ([AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **m)|
|----|----|

释放分配给AVDictionary结构体的所有内存以及所有键和值。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_dict_get_string](http://ffmpeg.org/doxygen/trunk/group__lavu__dict.html#ga480ab3fb3ac6cae8e60e97f58ec30b66) (const [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *m, char **[buffer](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3667f558219c90437106b544a3ca00b8), const char key_val_sep, const char pairs_sep)|
|----|----|

以字符串形式获取字典条目。

创建一个包含字典条目的字符串。 这样的字符串可以传回给av_dict_parse_string（）。


注意：字符串使用反斜杠（'\'）进行转义。

参数：m：字典

  buffer：将分配给字符串的缓冲区指针包含条目。 当不再需要时，调用者必须释放缓冲区。

key_val_sep：用于将键与值分离的字符

pairs_sep：用于将两个对彼此分开的字符

返回：> = 0成功，否定错误

警告：分隔符不能既不是“\”也不是“\ 0”。 他们也不一样。







二、AVTree

低复杂度树容器。


插入，移除，找到相等的，最大的，小于和小于大于所有都具有O（log n）最坏情况的复杂性。





一）、函数


|struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) *|[av_tree_node_alloc](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#gaa22713575bd8846e61128660b6b2c952) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配AVTreeNode。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_tree_find](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#gaf577181dd95c6c922c1e530c4f0ef66b) (const struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) *root, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[cmp](http://ffmpeg.org/doxygen/trunk/tests_2swresample_8c.html#a6f91b4cd1af5dc277444c465e2ca3ab6))(const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *key, const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072)), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *next[2])|
|----|----|

找一个元素

参数：root：指向树的根节点的指针

  next：如果next不为NULL，则next [0]将包含上一个元素，next [1]将包含下一个元素。 如果不存在，则下一个对应的条目不变。

  cmp：比较功能用于比较树中的元素，API与标准C的qsort相同。保证第一个也只有第一个参数为cmp（）将成为av_tree_find（）的关键参数，因此如果用户想要 ，是不同的类型（如不透明的上下文）。

返回：如果树中没有这样的元素，则具有cmp（key，elem）== 0或NULL的元素。

参数：





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_tree_insert](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#ga40aae331288a02ddcbb963b0cad95f66) (struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) **rootp, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[cmp](http://ffmpeg.org/doxygen/trunk/tests_2swresample_8c.html#a6f91b4cd1af5dc277444c465e2ca3ab6))(const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *key, const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[b](http://ffmpeg.org/doxygen/trunk/vf__curves_8c.html#a0a667482238bcca3ac201c1cd09c5072)), struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) **next)|
|----|----|

插入或移除元素。

如果* next为NULL，则提供的元素将被删除（如果存在）。 如果* next为非NULL，则提供的元素将被插入，除非它已经存在于树中。


参数：rootp：指向树的根节点的指针的指针; 注意根节点在插入过程中可以改变，这是保持树平衡所必需的。

  key：指向要插入树中的元素键的指针

           next：用于分配和释放AVTreeNodes。 对于插入，用户必须将其设置为至少为av_tree_node_size字节大小的已分配和归零的对象。 如果av_tree_insert（）已被使用，将会将其设置为NULL。 对于删除元素* next由用户设置为NULL，av_tree_insert（）将其设置为用于已删除元素的AVTreeNode。 这允许使用扁平阵列，与许多已发现的元素相比，它们具有较低的开销。 您可能需要定义如下功能：



```cpp
void *tree_insert(struct AVTreeNode **rootp, void *key,
                  int (*cmp)(void *key, const void *b),
                  AVTreeNode **next)
{
    if (!*next)
        *next = av_mallocz(av_tree_node_size);
    return av_tree_insert(rootp, key, cmp, next);
}
void *tree_remove(struct AVTreeNode **rootp, void *key,
                  int (*cmp)(void *key, const void *b, AVTreeNode **next))
{
    av_freep(next);
    return av_tree_insert(rootp, key, cmp, next);
```

   cmp：比较功能用于比较树中的元素，API与标准C的qsort相同

返回：如果没有插入，发现元素; 如果发生插入或删除，则返回任一键或NULL。 哪一个取决于树状态和实现。 你不应该假设它是代码中的一个或另一个。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_tree_destroy](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#gae2f16a7b3895fd776696cd9eccd6c26a) (struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) *t)|
|----|----|



|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_tree_enumerate](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#ga1c7cce17e3ab55eb07c6ad8739ff58d2) (struct [AVTreeNode](http://ffmpeg.org/doxygen/trunk/structAVTreeNode.html) *t, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*[cmp](http://ffmpeg.org/doxygen/trunk/tests_2swresample_8c.html#a6f91b4cd1af5dc277444c465e2ca3ab6))([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *elem), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)(*enu)([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *elem))|
|----|----|

对给定范围内的树中的所有元素应用enu（opaque，＆elem）。

参数：cmp：对于范围以下的元素返回<0的比较函数，范围以上的元素返回> 0，范围内的元素返回== 0

注意：cmp函数应该使用与构造树相同的顺序。




二）、变量


|const [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_tree_node_size](http://ffmpeg.org/doxygen/trunk/group__lavu__tree.html#gabad004dcf19a9d481793c03cabda5984)|
|----|----|




