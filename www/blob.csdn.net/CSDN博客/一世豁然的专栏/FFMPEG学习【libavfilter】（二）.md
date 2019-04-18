# FFMPEG学习【libavfilter】（二） - 一世豁然的专栏 - CSDN博客





2017年06月13日 09:57:02[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1458








一、函数


|unsigned|[avfilter_version](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gab2e80a6bf5de47a81f64471e73524a6a) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回LIBAVFILTER_VERSION_INT常量。





|const char *|[avfilter_configuration](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gaeda0e16f48d137db0ef762259603cb06) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavfilter构建时配置。





|const char *|[avfilter_license](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gadc149473598907e3db669018abb11e6e) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回libavfilter许可证。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_pad_count](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae13acb5396b0204c3641502677db2f49) (const [AVFilterPad](http://ffmpeg.org/doxygen/trunk/structAVFilterPad.html) *pads)|
|----|----|

获取AVFilterPads的NULL终止数组中的元素数。






|const char *|[avfilter_pad_get_name](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga2d69631bb24a0a2b7ac0e00fe1dfab3b) (const [AVFilterPad](http://ffmpeg.org/doxygen/trunk/structAVFilterPad.html) *pads, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) pad_idx)|
|----|----|

获取AVFilterPad的名称。

参数：pads：一个AVFilterPads数组

    pad_idx：数组中的pad的索引; 调用者有责任确保索引有效。

返回：pad中pad_idx垫的名称





|enum [AVMediaType](http://ffmpeg.org/doxygen/trunk/group__lavu__misc.html#ga9a84bba4713dfced21a1a56163be1f48)|[avfilter_pad_get_type](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga2d241a0066fc3724ec3335e25bc3912e) (const [AVFilterPad](http://ffmpeg.org/doxygen/trunk/structAVFilterPad.html) *pads, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) pad_idx)|
|----|----|

获取AVFilterPad的类型。

参数：pads：一个AVFilterPads数组



    pad_idx：数组中的pad的索引; 调用者有责任确保索引有效。

返回：pad中pad_idx的垫的类型





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_link](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gabc6247ebae2c591e768c8555174402f1) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66), unsigned srcpad, [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *dst, unsigned dstpad)|
|----|----|

一起连接两个过滤器

参数：src：源过滤器

   srcpad：源滤波器上的输出焊盘的索引

   dst：目的过滤器

    dstpad：目标过滤器上输入焊盘的索引

返回：成功时返回0。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_link_free](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gacb88c44589fa57729637817f26826193) ([AVFilterLink](http://ffmpeg.org/doxygen/trunk/structAVFilterLink.html) **link)|
|----|----|

释放*link中的链接，并将其指针设置为NULL。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_link_get_channels](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga8c75ac0df01ef6fbab65b8a92d2af944) ([AVFilterLink](http://ffmpeg.org/doxygen/trunk/structAVFilterLink.html) *link)|
|----|----|

获取链接的频道数量。





|[attribute_deprecated](http://ffmpeg.org/doxygen/trunk/libswscale_2utils_8c.html#aa6d076561d3a9eea4729ee632652de02)[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_link_set_closed](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga0f20a579fb5853f55fe17443a2dfea41) ([AVFilterLink](http://ffmpeg.org/doxygen/trunk/structAVFilterLink.html) *link, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) closed)|
|----|----|

设置链接的封闭字段。

已废弃：应用程序不应该混淆链接，他们应该关闭接收器。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_config_links](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gab06aaeb012cc4fed64620c0e8064e002) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d))|
|----|----|

协商过滤器的所有输入的媒体格式，尺寸等。

参数：filter：过滤器来协商其输入的属性

返回：谈判成功返回零





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_process_command](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga7265908e69b4eaa69a4947a093743f26) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d), const char *cmd, const char *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), char *res, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) res_len, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

使过滤器实例处理一个命令。

建议使用avfilter_graph_send_command（）。






|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_register_all](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga2bf8f0a6e2599faf4bf97b04a5a25149) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

初始化过滤系统。

注册所有内置过滤器。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_register](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga3db814dea5d918f88af888d484f77fb8) ([AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d))|
|----|----|

注册过滤器

仅当您打算稍后使用avfilter_get_by_name以名称查找AVFilter结构时，才需要这样做。 即使没有注册，仍可以使用avfilter_graph_alloc_filter实例化一个过滤器。


参数：filter：用来进行注册过滤器

返回：如果注册成功，则为0，否则为否





|const [AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *|[avfilter_get_by_name](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gadd774ec49e50edf00158248e1bfe4ae6) (const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

获取与给定名称匹配的过滤器定义。

参数：name：要查找过滤器的名称

返回：过滤器定义，如果有任何匹配的注册。 如果没有找到则为NULL。





|const [AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *|[avfilter_next](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga46ef237854ad481005e51bd13a0e1f87) (const [AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *prev)|
|----|----|

迭代所有注册过滤器。

返回：如果prev为非NULL，则在prev之后的下一个注册过滤器为NULL，如果prev为最后一个过滤器。 如果prev为NULL，则返回第一个注册过滤器。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_init_str](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga0d21eb00b9f24213bd7c5775393a09fd) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const char *[args](http://ffmpeg.org/doxygen/trunk/avisynth__c_8h.html#a68026e8546814670dd0cd181ed2308f9))|
|----|----|

使用提供的参数初始化一个过滤器。

参数：ctx：未初始化的过滤器上下文进行初始化

   args：用于初始化过滤器的选项。 这必须是'key = value'形式的'：'分隔的选项列表。 如果选项已经使用AVOptions API直接设置，或者没有需要设置的选项，则可能为NULL。

返回：0成功，否则返回AVERROR表示失败





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_init_dict](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga8c15af28902395399fe455f6f8236848) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

使用提供的选项字典初始化一个过滤器。

参数：ctx：未初始化的过滤器上下文进行初始化

   options：一个填充了此过滤器选项的AVDictionary。 返回时，此参数将被销毁，并替换为包含未找到的选项的dict。 该字典必须由调用者释放。 可能为NULL，那么这个函数等同于avfilter_init_str（），第二个参数设置为NULL。

返回：0成功，否则返回AVERROR表示失败


注意：这个函数和avfilter_init_str（）实质上是一样的，区别在于通过选项的方式。 电话代码取决于哪一个更适合。 当某些提供的选项未被声明为过滤器支持时，这两个函数的行为也不同。 在这种情况下，avfilter_init_str（）将失败，但是这个函数会在选项AVDictionary中留下额外的选项，照常继续。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_free](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga0ea7664a3ce6bb677a830698d358a179) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d))|
|----|----|

释放一个过滤器上下文。

这也将从过滤器的过滤器列表中删除过滤器。


参数：filter：用来释放的过滤器





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_insert_filter](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga0d2740a8679ff5086f680547eb0a08e0) ([AVFilterLink](http://ffmpeg.org/doxygen/trunk/structAVFilterLink.html) *link, [AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *[filt](http://ffmpeg.org/doxygen/trunk/af__earwax_8c.html#af168ba1108ec9a2931e0f8335202c228), unsigned filt_srcpad_idx, unsigned filt_dstpad_idx)|
|----|----|

在现有链接的中间插入一个过滤器。

参数：link：插入过滤器的链接

   filt：要插入的过滤器

filt_srcpad_idx：滤波器上的输入板连接

filt_dstpad_idx：滤波器上的输出板连接

返回：成功时返回0。





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[avfilter_get_class](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga47ab3c56888ecc8ced403cbf39c35a4d) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

返回：AVClass for AVFilterContext.





|[AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *|[avfilter_graph_alloc](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga6c778454b86f845805ffd814b4ce51d4) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配过滤图。

返回：分配的过滤器图表成功或为空。





|[AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *|[avfilter_graph_alloc_filter](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gaa9af17ecf4c5c87307b57cf08411088b) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const [AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *[filter](http://ffmpeg.org/doxygen/trunk/vf__yadif_8c.html#a4aac1797de0c2fcdc5b2b2b3191d252d), const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

在过滤器图中创建一个新的过滤器实例。

参数：graph：将使用新的过滤器的图形

   filter：该过滤器创建一个实例

   name：名称给新的实例（将被复制到AVFilterContext.name）。 调用者可以使用这种方法来识别不同的过滤器，libavfilter本身不会为此参数分配任何语义。 可能为NULL。

返回：新创建的过滤器实例的上下文（注意它也可以通过AVFilterGraph.filters或avfilter_graph_get_filter（）直接检索）成功，或者在失败时为NULL。





|[AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) *|[avfilter_graph_get_filter](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gafdc2bad47c8b477a4baed6a37c61edfc) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

从图中获取由实例名称标识的过滤器实例。

参数：graph：过滤器图表进行搜索。

   name：过滤器实例名称（在图中应该是唯一的）。

参数：指向找到的过滤器实例的指针，如果找不到则返回NULL。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_create_filter](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gac0788a9ab6966dba9318b5d5c7524fea) ([AVFilterContext](http://ffmpeg.org/doxygen/trunk/structAVFilterContext.html) **filt_ctx, const [AVFilter](http://ffmpeg.org/doxygen/trunk/structAVFilter.html) *[filt](http://ffmpeg.org/doxygen/trunk/af__earwax_8c.html#af168ba1108ec9a2931e0f8335202c228), const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *[args](http://ffmpeg.org/doxygen/trunk/avisynth__c_8h.html#a68026e8546814670dd0cd181ed2308f9), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *opaque, [AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph_ctx)|
|----|----|

创建一个过滤器实例并添加到现有的图表中。

过滤器实例从过滤器过滤器创建，并与参数args和opaque进行联合。


在成功的情况下放入* filt_ctx指向创建的过滤器实例的指针，否则将* filt_ctx设置为NULL。


参数：name：要创建的过滤器实例的实例名称

graph_ctx：过滤图

返回：出现错误时为负的AVERROR错误代码，否则为非负值





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_graph_set_auto_convert](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gae7e75e3df70de53fae2cf7950c1247a8) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, unsigned [flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

在图表中启用或禁用自动格式转换。

请注意，格式转换仍然可以在显式插入的比例和样本过滤器中进行。


桉树：flags：any of the AVFILTER_AUTO_CONVERT_* constants





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_config](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga1896c46b7bc6ff1bdb1a4815faa9ad07) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graphctx, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *log_ctx)|
|----|----|

检查有效性并配置图中的所有链接和格式。

参数：graphctx：过滤图

log_ctx：用于记录的上下文

返回：> = 0表示成功，否则一个负数AVERROR错误码。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_graph_free](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga871684449dac05050df238a18d0d493b) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) **graph)|
|----|----|

释放一个图形，破坏它的链接，并将*图形设置为NULL

如果* graph为NULL，则不执行任何操作。






|[AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) *|[avfilter_inout_alloc](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga6e1c2931e15eb4283c59c6ccc8b83919) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac))|
|----|----|

分配单个AVFilterInOut条目。

必须使用avfilter_inout_free（）来释放。


返回：分配AVFilterInOut成功，NULL失败。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[avfilter_inout_free](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga294500a9856260eb1552354fd9d9a6c4) ([AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) **inout)|
|----|----|

释放AVFilterInOut提供的列表，并将*输入设置为NULL。

如果* input为NULL，则不执行任何操作。






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_parse](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gab4718e1716c0a0d9343d49ff10d542cb) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *[filters](http://ffmpeg.org/doxygen/trunk/noise__shaping__data_8c.html#a967dc603e672f94e43935d15e1b46386), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) *[inputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a6e04c7ed634034f9b8c1fec9d362ce44), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) *[outputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a7ddb0b5ac76101c5bee71e9840bf1698), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *log_ctx)|
|----|----|

将一个字符串描述的图形添加到图形中。

注意：调用者必须提供输入和输出的列表，因此在调用该函数之前必须知道这些列表。

输入参数描述图的已有部分的输入; 即从新创建的部分的观点来看，它们是输出。 类似地，输出参数描述已经存在的滤波器的输出，其作为输入被提供给经解析的滤波器。

参数：graph：过滤图在哪里链接解析图上下文

   inputs：要解析的字符串

   outputs：链接列表到图形的输入

log_ctx：链接列表到图形的输出

返回：零表示成功，错误返回负ERROR代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_parse_ptr](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga34f4ff420bd58da6747a3ff1fbedd001) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *[filters](http://ffmpeg.org/doxygen/trunk/noise__shaping__data_8c.html#a967dc603e672f94e43935d15e1b46386), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) **[inputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a6e04c7ed634034f9b8c1fec9d362ce44), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) **[outputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a7ddb0b5ac76101c5bee71e9840bf1698), [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *log_ctx)|
|----|----|

将一个字符串描述的图形添加到图形中。

在图形过滤器描述中，如果没有指定第一个过滤器的输入标签，则假定为“in”; 如果未指定最后一个过滤器的输出标签，则假定为“out”。


参数：graph：过滤图在哪里链接解析图上下文

   filters：要解析的字符串

   inputs：指向链表到图形输入的指针可能为NULL。 如果非NULL，*输入更新为包含解析后的开放输入列表，应该使用avfilter_inout_free（）来释放。

   outputs：指向链表到图形输出的指针可能为NULL。 如果非NULL，*输出更新为包含解析后的打开输出列表，则应该使用avfilter_inout_free（）来释放。

返回：成功时为非负，错误为负的ERROR码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_parse2](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga6c3c39e0861653c71a23f90d1397239d) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *[filters](http://ffmpeg.org/doxygen/trunk/noise__shaping__data_8c.html#a967dc603e672f94e43935d15e1b46386), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) **[inputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a6e04c7ed634034f9b8c1fec9d362ce44), [AVFilterInOut](http://ffmpeg.org/doxygen/trunk/structAVFilterInOut.html) **[outputs](http://ffmpeg.org/doxygen/trunk/vf__zoompan_8c.html#a7ddb0b5ac76101c5bee71e9840bf1698))|
|----|----|

将一个字符串描述的图形添加到图形中。

参数：graph：过滤图在哪里链接解析图上下文

   filters：要解析的字符串

   inputs：此处将返回解析图的所有空闲（未链接）输入的链接列表。 使用avfilter_inout_free（）的方式可以释放它。

   outputs：此处将返回解析图的所有空闲（未链接）输出的链接列表。 使用avfilter_inout_free（）的方式可以释放它。

返回：返回0表示成功，失败返回负的错误码

注意：该函数返回在解析图之后未链接的输入和输出，然后调用者处理它们。

该函数对图形的已有部分没有引用，输入参数将返回包含图解新分析部分的输入。 类似地，outputs参数将包含新创建的过滤器的输出。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_send_command](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gaaad7850fb5fe26d35e5d371ca75b79e1) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *target, const char *cmd, const char *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), char *res, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) res_len, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

向一个或多个过滤器实例发送命令。

参数：graph：过滤图

   target：命令应发送到的过滤器“all”发送到所有过滤器，否则它可以是一个过滤器或过滤器实例名称，它将命令发送到所有匹配的过滤器。

   cmd：要发送的命令，为了处理简单，所有命令只能是字母数字

   arg：命令的论据

   res：一个大小为res_size的缓冲区，其中过滤器可以返回响应。

返回：> = 0，否则出现错误代码。 对于不支持的命令，AVERROR（ENOSYS）





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_queue_command](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#ga3140b3cd7bf9a80c2dfa868dcf7ac541) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *target, const char *cmd, const char *[arg](http://ffmpeg.org/doxygen/trunk/libavcodec_2jacosubdec_8c.html#ac3a73711fd7f8a50db1c4b62e41cf852), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4), double ts)|
|----|----|

为一个或多个过滤器实例排队命令。

参数：graph：过滤图

   target：命令应发送到的过滤器“all”发送到所有过滤器，否则它可以是一个过滤器或过滤器实例名称，它将命令发送到所有匹配的过滤器。

   cmd：发送的命令，为了处理简单，所有命令只能是字母数字

   arg：命令的论据

   ts：命令应发送到过滤器的时间

注意：由于此函数返回后执行命令，因此不提供来自过滤器的返回代码，也不支持AVFILTER_CMD_FLAG_ONE。





|char *|[avfilter_graph_dump](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gadb442aca4e5a8c3ba740f6049f0a288b) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph, const char *[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

将图形转储为人类可读的字符串表示形式。

参数：graph：转储图

   options：格式选项; 目前被忽略

返回：一个字符串，或者在内存分配失败的情况下为NULL; 该字符串必须使用av_free来释放





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[avfilter_graph_request_oldest](http://ffmpeg.org/doxygen/trunk/group__lavfi.html#gab20535e0685fb5f9b4f02e436412c3f0) ([AVFilterGraph](http://ffmpeg.org/doxygen/trunk/structAVFilterGraph.html) *graph)|
|----|----|

请求在最旧的接收器链接上的帧。

如果请求返回AVERROR_EOF，请尝试下一个。


请注意，此功能并不意味着是滤镜的唯一调度机制，只是在正常情况下帮助排除滤光片的方便功能。


另请注意，AVERROR_EOF并不意味着帧在过程中没有到达某些接收器。 当存在多个宿链路时，如果所请求的链路返回EOF，则这可能导致过滤器冲突发送到另一个宿链路的等待帧，尽管未经请求。


返回：ff_request_frame（）的返回值，如果所有链接都返回AVERROR_EOF，则为AVERROR_EOF



