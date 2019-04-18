# FFMPEG学习【libavutil】：数据结构（三） - 一世豁然的专栏 - CSDN博客





2017年07月23日 10:25:39[一世豁然](https://me.csdn.net/Explorer_day)阅读数：261








AVOptions提供了一个通用的系统来声明任意结构（“对象”）上的选项。

一个选项可以有一个帮助文本，一个类型和一系列可能的值。 然后可以枚举，读取和写入选项。





**Implementing AVOptions**


本节介绍如何向结构体添加AVOptions功能。


所有AVOptions相关信息都存储在AVClass中。 因此，struct的第一个成员应该是一个指向描述它的AVClass的指针。 AVClass的选项字段必须设置为一个以NULL结尾的AVOptions的静态数组。 每个AVOption必须具有非空名称，类型，默认值和数字类型AVOptions也是允许值的范围。 它还必须从结构的开始处声明一个以字节为单位的偏移量，其中与此AVOption相关联的字段位于该位置。 AVOption结构中的其他字段也应在适用时进行设置，但不是必需的。


以下示例说明了支持AVOptions的结构：




```cpp
typedef struct test_struct {
    const AVClass *class;
    int      int_opt;
    char    *str_opt;
    uint8_t *bin_opt;
    int      bin_len;
} test_struct;
static const AVOption test_options[] = {
  { "test_int", "This is a test option of int type.", offsetof(test_struct, int_opt),
    AV_OPT_TYPE_INT, { .i64 = -1 }, INT_MIN, INT_MAX },
  { "test_str", "This is a test option of string type.", offsetof(test_struct, str_opt),
    AV_OPT_TYPE_STRING },
  { "test_bin", "This is a test option of binary type.", offsetof(test_struct, bin_opt),
    AV_OPT_TYPE_BINARY },
  { NULL },
};
static const AVClass test_class = {
    .class_name = "test class",
    .item_name  = av_default_item_name,
    .option     = test_options,
    .version    = LIBAVUTIL_VERSION_INT,
};
```


接下来，分配你的结构时，你必须确保AVClass指针被设置为正确的值。 然后，可以调用av_opt_set_defaults（）来初始化默认值。 之后，该结构就可以与AVOptions API一起使用了。

清理时，您可以使用av_opt_free（）函数自动释放所有分配的字符串和二进制选项。


继续以上示例：




```cpp
test_struct *alloc_test_struct(void)
{
    test_struct *ret = av_mallocz(sizeof(*ret));
    ret->class = &test_class;
    av_opt_set_defaults(ret);
    return ret;
}
void free_test_struct(test_struct **foo)
{
    av_opt_free(*foo);
    av_freep(foo);
}
```



**Nesting**


可能会发生一个AVOptions启用的结构包含另一个支持AVOptions的结构作为成员（例如，libavcodec中的AVCodecContext导出通用选项，而其priv_data字段导出编解码器特定的选项）。 在这种情况下，可以设置父结构以导出子选项。 要做到这一点，只需在父结构的AVClass中实现AVClass.child_next（）和AVClass.child_class_next（）。 假设来自上面的test_struct现在还包含一个child_struct字段：




```cpp
typedef struct child_struct {
    AVClass *class;
    int flags_opt;
} child_struct;
static const AVOption child_opts[] = {
    { "test_flags", "This is a test option of flags type.",
      offsetof(child_struct, flags_opt), AV_OPT_TYPE_FLAGS, { .i64 = 0 }, INT_MIN, INT_MAX },
    { NULL },
};
static const AVClass child_class = {
    .class_name = "child class",
    .item_name  = av_default_item_name,
    .option     = child_opts,
    .version    = LIBAVUTIL_VERSION_INT,
};
void *child_next(void *obj, void *prev)
{
    test_struct *t = obj;
    if (!prev && t->child_struct)
        return t->child_struct;
    return NULL
}
const AVClass child_class_next(const AVClass *prev)
{
    return prev ? NULL : &child_class;
```


将上面定义的child_next（）和child_class_next（）放在test_class中，现在可以通过test_struct访问child_struct的选项（再次，在创建之后，需要在child_struct上完成上述的正确设置）。

从上面的例子可能不清楚为什么需要child_next（）和child_class_next（）。 区别在于child_next（）迭代实际存在的对象，而child_class_next（）遍历所有可能的子类。 例如。 如果AVCodecContext被初始化为使用具有私有选项的编解码器，那么它的child_next（）将返回AVCodecContext.priv_data并完成迭代。 AVCodecContext.av_class上的OTOH child_class_next（）将使用私有选项来迭代所有可用的编解码器。





**Named constants**


可以为选项创建命名常量。 只需将选项的单位字段设置为常量应用于字符串，并将常量自身创建为类型为AV_OPT_TYPE_CONST的选项，其单位字段设置为相同的字符串。 它们的default_val字段应包含命名常量的值。 例如，要为上面的test_flags选项添加一些命名常量，请将以下内容放入child_opts数组中：




```cpp
{ "test_flags", "This is a test option of flags type.",
  offsetof(child_struct, flags_opt), AV_OPT_TYPE_FLAGS, { .i64 = 0 }, INT_MIN, INT_MAX, "test_unit" },
{ "flag1", "This is a flag with value 16", 0, AV_OPT_TYPE_CONST, { .i64 = 16 }, 0, 0, "test_unit" },
```

**Using AVOptions**

本节介绍在支持AVOptions的结构中访问选项。 FFmpeg中的这样的结构是例如。 libavcodec中的AVCodecContext或libavformat中的AVFormatContext。





**Examining AVOptions**


检查选项的基本功能是av_opt_next（），它遍历为一个对象定义的所有选项，av_opt_find（）将搜索给定名称的选项。


嵌套情况更复杂。 支持AVOptions的结构可能具有启用AVOptions的子项。 将AV_OPT_SEARCH_CHILDREN标志传递给av_opt_find（）将使函数递归地搜索子项。


列举基本上有两种情况。 第一个是当您想要获取结构体及其子代中可能存在的所有选项（例如构建文档时）。 在这种情况下，您应该在父结构的AVClass上递归调用av_opt_child_class_next（）。 第二种情况是当你有一个已经初始化的结构与所有的孩子，你想获得所有可以实际写入或读取的选项。 在这种情况下，您应该在每个结果上递归调用av_opt_child_next（）（和av_opt_next（））。





**Reading and writing AVOptions**


设置选项时，您通常会直接从用户读取字符串。 在这种情况下，只需将其传递给av_opt_set（）即可。 对于非字符串类型选项，av_opt_set（）将根据选项类型解析字符串。


类似地，av_opt_get（）将读取任何选项类型并将其转换为将被返回的字符串。 不要忘记字符串被分配，所以你必须用av_free（）释放它。


在某些情况下，将所有选项放入AVDictionary并调用av_opt_set_dict（）可能会更为方便。 一个具体的例子是lavf / lavc中的格式/编解码器打开功能，它使用填充了选项的字典作为参数。 这使得可以设置不能另外设置的一些选项，因为例如 在文件实际打开之前，输入文件格式是不知道的。








一、模块

一）、Evaluating option strings

这组函数可用于评估选项字符串并从中获取数字。

它们与av_opt_set（）执行相同的操作，除了将结果写入到调用者提供的指针中。


参数：obj：一个结构体，其第一个元素是指向AVClass的指针。

   o：要评估字符串的选项。

   val：要评估的字符串。

   *_val:字符串的值将写在这里。

返回：0成功，负数为失败。




**函数**


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_flags](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#gae245940b870e13b759354d570decf3dc) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *flags_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_int](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#ga8bcee89d4a51400092e2d653c842bfa8) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *int_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_int64](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#gae2285d602e2079f0df141826fc00c666) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), int64_t *int64_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_float](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#ga3c06cf2a2e7efc60d97adbeb7bbb6b44) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), float *float_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_double](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#gac65f3b9753c48616e8dd4dea0c47f9e6) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), double *double_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_eval_q](http://ffmpeg.org/doxygen/trunk/group__opt__eval__funcs.html#gabd95639131aac582a2f7c4bfc5adb4b0) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o, const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) *q_out)|
|----|----|



二）、Option setting functions

那些函数将给定名称的obj的字段设置为value。


参数：obj：一个结构，其第一个元素是指向AVClass的指针。

   name：要设置的字段的名称

   val：要设置的值。 如果av_opt_set（）如果字段不是字符串类型，则会解析给定的字符串。 支持SI后缀和一些命名标量。 如果字段是数字类型，则必须是数字或命名标量。 具有多个标量和+ - 中缀运算符的行为未定义。 如果该字段是一个标志类型，它必须是一个数字标量或命名标记的序列，以'+'或' - '分隔。 用'+'替代标志会导致设置不影响其他标志; 同样地，' - '取消标志。

   search_flags:标志传递给av_opt_find2。即 如果AV_OPT_SEARCH_CHILDREN被传递到这里，那么可以在obj的子对象上设置该选项。

返回：如果值已设置，则为0，如果出现错误，则为AVERROR代码：AVERROR_OPTION_NOT_FOUND（如果不存在匹配选项）AVERROR（ERANGE）如果值超出范围AVERROR（EINVAL），如果该值无效




**宏**


|#define|[av_opt_set_int_list](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gac06fc2b2e32f67f067ed7aaec163447f)(obj, [name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), term, [flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

将二进制选项设置为整数列表。

参数：obj：AVClass对象设置选项

   name：二进制选项的名称

   val：指向整数列表的指针（对于列表的内容，必须具有正确的类型）

   term：列表终止符（通常为0或-1）

   flags：搜索标志




**函数**


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga5fd4b92bdf4f392a2847f711676a7537) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_int](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga3adf7185c21cc080890a5ec02c2e56b2) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), int64_t [val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|




|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_double](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga11f22424bb01fd58f6b34f220a0fff6a) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), double [val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_q](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga4f4f0b06e931c638ee8b4074e3638cef) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|
|| |



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_bin](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga6c32155fd8a96bed2f8cf5ebc030dadf) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[size](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3d1e3edfcf61ca2d831883e1afbad89e), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_image_size](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#ga29c5723116dcc9be891ee2ca61f2df5e) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) w, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[h](http://ffmpeg.org/doxygen/trunk/vp9dsp__template_8c.html#a5e36941b3d856737e81516acd45edc50), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_pixel_fmt](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gae8992c7875fd69a0c0e8c010098d402d) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gad446f7a7b92442e464d43e400e661040) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5)[fmt](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ad9a5b33dafc33a26929ac177b964d4ab), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_video_rate](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gac4a81110dd4f5c717ec6ccf0a9e94b9f) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html)[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_channel_layout](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gaf25f11e11068f2c41197ba072f25d2a4) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), int64_t ch_layout, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_dict_val](http://ffmpeg.org/doxygen/trunk/group__opt__set__funcs.html#gad19358af12f0766fedec516e387b8196) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) *[val](http://ffmpeg.org/doxygen/trunk/aeval_8c.html#a16aca557f352132b55bbc3c4769daa8b), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|





三）、Option getting functions

那些函数从对象获取给定名称的选项的值。


参数：obj：一个结构体，其第一个元素是指向AVClass的指针。

   name：获取的选项的名称。

   search_flags:标志传递给av_opt_find2。即 如果AV_OPT_SEARCH_CHILDREN在这里被传递，那么该选项可能在obj的一个小孩中找到。

   out_val:该选项的值将在此处写入

返回：> = 0成功，否则为否定错误代码




**函数**


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#gaf31144e60f9ce89dbe8cbea57a0b232c) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [uint8_t](http://ffmpeg.org/doxygen/trunk/audio__convert_8c.html#ae1affc9ca37cfb624959c866a73f83c2) **out_val)|
|----|----|

注意：返回的字符串将被av_malloc（）编译，并且必须由调用者编写av_free（）

如果av_opt_get中的search_flags中设置了AV_OPT_ALLOW_NULL，并且该选项具有AV_OPT_TYPE_STRING或AV_OPT_TYPE_BINARY，并将其设置为NULL，则out_val将被设置为NULL，而不是分配的空字符串。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_int](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#gaab5b9c0969efe56420536a5d190e59bd) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, int64_t *out_val)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_double](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#ga4ee50245cd18a65ec1cd27bc8e0cab83) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, double *out_val)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_q](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#gabc553a1b95468f935eedfa9db86e2a15) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) *out_val)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_image_size](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#ga96d9c726c3feff308e7fe447633c9720) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *w_out, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) *h_out)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_pixel_fmt](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#ga41fdd93f0483e96548168f9cfaf5bf6b) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, enum [AVPixelFormat](http://ffmpeg.org/doxygen/trunk/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5) *out_fmt)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_sample_fmt](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#gade39dd8ce34b193f7012022072e36642) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, enum [AVSampleFormat](http://ffmpeg.org/doxygen/trunk/group__lavu__sampfmts.html#gaf9a51ca15301871723577c730b5865c5) *out_fmt)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_video_rate](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#gaf565dc77c53e8e56d08ee1a3c642f95f) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [AVRational](http://ffmpeg.org/doxygen/trunk/structAVRational.html) *out_val)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_channel_layout](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#ga35864ccde3246be3f56366fd5145fc12) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, int64_t *ch_layout)|
|----|----|



|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_dict_val](http://ffmpeg.org/doxygen/trunk/group__opt__get__funcs.html#ga6ed0b77def62321ff995843211405338) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **out_val)|
|----|----|








二、数据结构


|struct|[AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html){const char * name；const char * help；//  短英文帮助文本int offset;//相对于存储选项值的上下文结构的偏移量。 enum AVOptionType type;union {   int64_t   i64    double   dbl   const char *   str   AVRational   q} default_val//标量选项的默认值double min;//  选项的最小有效值double max;//  选项的最大有效值int flags;const char * unit;//选项所属的逻辑单位。}|
|----|----|



|struct|[AVOptionRange](http://ffmpeg.org/doxygen/trunk/structAVOptionRange.html){const char * str;double value_min;//价值范围。double value_max;double component_min;//价值的组成范围。double component_max;int is_range;//范围标志。}|
|----|----|



|struct|[AVOptionRanges](http://ffmpeg.org/doxygen/trunk/structAVOptionRanges.html){AVOptionRange ** range;//选项范围数组。int nb_ranges;//每个组件的范围数。int nb_components;//组件数量}|
|----|----|








三、宏


|#define|[AV_OPT_SEARCH_CHILDREN](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga25801ba4fc9b5313eb33ec84e082dd72)   (1 << 0)|
|----|----|

首先在给定对象的可能孩子中搜索。





|#define|[AV_OPT_SEARCH_FAKE_OBJ](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaa764998552a6f6f66a47ecd52b345caa)   (1 << 1)|
|----|----|

传递给av_opt_find（）的obj是假的 - 只有一个指向AVClass的双指针，而不是指向包含AVClass的结构的必需指针。





|#define|[AV_OPT_ALLOW_NULL](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga821eea4d28ffec3b26a9eef413ff9ed4)   (1 << 2)|
|----|----|

在av_opt_get中，如果该选项具有指针类型并返回NULL，而不是返回一个空字符串。





|#define|[AV_OPT_MULTI_COMPONENT_RANGE](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga1f10d07c79eec8bd1a8e1b91817e1b1a)   (1 << 12)|
|----|----|

允许av_opt_query_ranges和av_opt_query_ranges_default为某些选项类型返回多个组件。





|#define|[AV_OPT_SERIALIZE_SKIP_DEFAULTS](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga844ea4db9927936ea0cfc98e0113750f)   0x00000001|
|----|----|

序列化未设置为只默认值的选项。





|#define|[AV_OPT_SERIALIZE_OPT_FLAGS_EXACT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga49bfd169f5bcde70fb38fc036a524d3a)   0x00000002|
|----|----|

序列化与opt_flags完全匹配的选项。







四、枚举


|enum|[AVOptionType](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gabd75aa30eb8ad6387672df9a1fa79444) { [AV_OPT_TYPE_FLAGS](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a7aaeee142b88a3078ddc57c644b49a7e), [AV_OPT_TYPE_INT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444ac068b8f6afeeb293a13aeabb5d702411), [AV_OPT_TYPE_INT64](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a94d540a5b928da813ff94d9367a1078a), [AV_OPT_TYPE_DOUBLE](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444ae0b49ec51847a758ceb46976083da2e5), [AV_OPT_TYPE_FLOAT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444ae91d3f6cbac4a4de2a6e880ac2a52d1a), [AV_OPT_TYPE_STRING](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444afadddce95ad3b690dd38644b458b96c4), [AV_OPT_TYPE_RATIONAL](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a74ba464440564230a72b95b39593cbfc), [AV_OPT_TYPE_BINARY](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a2689966edbe494482dfdfaa4efdf95d3), [AV_OPT_TYPE_DICT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444aed6f0d8e5b8f3edcaea01c15ab43aefd), [AV_OPT_TYPE_UINT64](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444aa6d3f68edfa4b60e970e7422b7193dd0), [AV_OPT_TYPE_CONST](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a0fa9f07d428bdc32495fe55c365c5329) = 128, [AV_OPT_TYPE_IMAGE_SIZE](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a078f280b98b49a9f585c5d479b711d9a) = MKBETAG('S','I','Z','E'), [AV_OPT_TYPE_PIXEL_FMT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a114085ba9877df34de060124877d8274) = MKBETAG('P','F','M','T'), [AV_OPT_TYPE_SAMPLE_FMT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a711661923db91bbbd4abdec0efa5ff0a) = MKBETAG('S','F','M','T'), [AV_OPT_TYPE_VIDEO_RATE](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a9429811abc62d4a564ec56938df0a572) = MKBETAG('V','R','A','T'),[AV_OPT_TYPE_DURATION](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a2e3a2fc47ae688f28cc4abe59452a032) = MKBETAG('D','U','R',' '), [AV_OPT_TYPE_COLOR](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444afdcd13cbd184901b05e27595e647fadc) = MKBETAG('C','O','L','R'), [AV_OPT_TYPE_CHANNEL_LAYOUT](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444a5628c270688d2ec28643dfff825dbe26) = MKBETAG('C','H','L','A'), [AV_OPT_TYPE_BOOL](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ggabd75aa30eb8ad6387672df9a1fa79444af3898696f46dc79b74b708d8fa4b52a7) = MKBETAG('B','O','O','L') }|
|----|----|



|enum|{ [AV_OPT_FLAG_IMPLICIT_KEY](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gga103e76ed4bd42f776f7f260080b98b3faa836a939d74606db85ce1f09470d1605) = 1 }|
|----|----|





五、函数


|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_show2](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaf7394e27d34208b24af8bb4b4a11741b) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *av_log_obj, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) req_flags, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) rej_flags)|
|----|----|

显示对象选项。

参数：av_log_obj：用于显示选项的日志上下文

req_flags ：请求显示选项的标志。 只显示它的选项 - > flags＆req_flags。

rej_flags：拒绝标记的选项显示。 只显示它的选项！（opt-> flags＆req_flags）。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_opt_set_defaults](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gad4ca968e9058b39b2da630ad0a091bdb) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5))|
|----|----|

将所有AVOption字段的值设置为其默认值。

参数：s：一个支持Option的结构（它的第一个成员必须是A类的指针）





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_opt_set_defaults2](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaeef5479b31ac29a6a55eb9d108d6225f) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[s](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#ab87f55bd0280d90925050a4188c14ab5), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[mask](http://ffmpeg.org/doxygen/trunk/lzw_8c.html#a450ea4ae52aeb4ca6e2fabf02c695153), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

将所有AVOption字段的值设置为其默认值。



只有这些AVOption字段（opt-> flags＆mask）==标志将其默认值应用于s。


参数：s：一个支持Option的结构（它的第一个成员必须是A类的指针）

   mask：AV_OPT_FLAG_ *的组合

   flags：AV_OPT_FLAG_ *的组合






|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_set_options_string](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gab48669d1326f7d47ef39e5702e34abd2) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const char *[opts](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#af540b728145b614e34c4e10930354b93), const char *key_val_sep, const char *pairs_sep)|
|----|----|

解析opts中的键/值对列表。

对于找到的每个键/值对，将值存储在ctx中的字段中，名称类似于键。 ctx必须是AVClass上下文，使用AVOptions进行存储。


参数：opts：选项字符串进行解析，可能为NULL

key_val_sep：一个0个终止的字符列表，用于将值与值分离

pairs_sep：用于将两个对彼此分离的0个字符的字符列表

返回：成功设置密钥/值对的数量，或与错误：AVERROR（EINVAL）相对应的AVERROR代码，如果opts无法解析，则如果密钥/值对不能被解析，则由av_opt_set（）发出的错误代码。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_from_string](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaa936b1aa7caca67b21cd50819c2b9fb1) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[ctx](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#a93cc7296f13f507c1fd7abd0db36b898), const char *[opts](http://ffmpeg.org/doxygen/trunk/tests_2movenc_8c.html#af540b728145b614e34c4e10930354b93), const char *const *shorthand, const char *key_val_sep, const char *pairs_sep)|
|----|----|

解析opts中的键值对列表。

对于找到的每个键=值对，设置ctx中相应选项的值。


参数：ctx：AVClass对象设置选项

   opts：选项字符串，由分隔符分隔的键值对

shorthand：一个以NULL结尾的快捷方式符号选项名称数组：如果opts中的第一个字段没有关键部分，则该关键字取自速记的第一个元素; 然后再次为第二个等等，直到选择完成，缩写完成或找到命名选项; 之后，所有选项都必须命名

key_val_sep：用于将值与值分隔的0个终止的字符列表，例如'='

pairs_sep：用于将两个对彼此分离的0个字符的列表，例如'：'或'，'

返回：成功设置key = value对的数量，或与错误时的AVERROR代码相对应的负数值：AVERROR（EINVAL）如果opts无法解析，则如果键/值对不能被解析，则由av_set_string3（）发出的错误代码。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_opt_free](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gac14db824af52baf8181406e98d116097) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj)|
|----|----|

释放obj中所有分配的对象。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_flag_is_set](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga1436a3afc8dbc70ed9e70c8287dcc766) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *field_name, const char *flag_name)|
|----|----|

检查flags字段中是否设置了特定的标志。

参数：field_name：标志字段选项的名称

flag_name：要检查的标志的名称

返回：如果标志设置为非零，如果标志未设置，则为0，不是正确的类型，或者标志字段不存在。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_dict](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaf1a88e8f152d7835f111b1ef8813fb2e) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, struct [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6))|
|----|----|

设置对象上给定字典的所有选项。

参数：obj：一个结构体，其第一个元素是指向AVClass的指针

   options：选择处理。 这个字典将被释放，并被一个新的字体替换，其中包含obj中没有找到的所有选项。 当然这个新的字典需要被av_dict_free（）的调用者释放。

返回：0成功，如果在obj中发现某个选项，则为AVIDOR，但无法设置。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_set_dict2](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga1824a600b215f1807e77b5f6cfa58586) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, struct [AVDictionary](http://ffmpeg.org/doxygen/trunk/structAVDictionary.html) **[options](http://ffmpeg.org/doxygen/trunk/libswresample_2options_8c.html#a384ff94b5139f441c282c9b4c7b985e6), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|

设置对象上给定字典的所有选项。

参数：obj：一个结构体，其第一个元素是指向AVClass的指针

   options：选择处理。 这个字典将被释放，并被一个新的字体替换，其中包含obj中没有找到的所有选项。 当然这个新的字典需要被av_dict_free（）的调用者释放。

search_flags：AV_OPT_SEARCH_ *的组合。

返回：0成功，如果在obj中发现某个选项，则为AVIDOR，但无法设置。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_get_key_value](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga1e459af4e23415b8eb9b05aee81c933c) (const char **ropts, const char *key_val_sep, const char *pairs_sep, unsigned [flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4), char **rkey, char **rval)|
|----|----|

从字符串的开头提取键值对。

参数：ropts：指向选项字符串的指针，将被更新为指向其余的字符串（对之一或最后一个NUL）

key_val_sep：用于将值与值分隔的0个终止的字符列表，例如'='

pairs_sep：用于将两个对彼此分离的0个字符的列表，例如'：'或'，'

   flags：标志; 请参阅下面的AV_OPT_FLAG_ *值

rkey：解密键，必须使用av_free（）

rval：解析值，必须使用av_free（）

返回：> = 0表示成功，否则为错误对应于错误代码的负值;
 特别是：AVERROR（EINVAL）如果没有键存在





|const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *|[av_opt_find](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gae31ae7fb20113b00108d0ecf53f25664) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *unit, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) opt_flags, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|

在对象中查找一个选项。

只考虑设置了所有指定标志的选项。


参数：obj：指向结构体的指针，其第一个元素是指向AVClass的指针。 或者，如果设置了AV_OPT_SEARCH_FAKE_OBJ搜索标志，则指向AVClass的双指针。

   name：要查找的选项的名称。

   uint：当搜索命名常量时，它所属的单位的名称。

opt_flags：仅查找所有指定的标志设置的选项（AV_OPT_FLAG）。

search_flags：AV_OPT_SEARCH_ *的组合。

返回：指向找到的选项的指针，如果没有找到选项，则返回NULL。

注意：使用AV_OPT_SEARCH_CHILDREN标志找到的选项可能无法通过av_opt_set（）直接设置。 使用AVDictionary（例如avformat_open_input（））选项的特殊调用来设置使用此标志找到的选项。





|const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *|[av_opt_find2](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga3e121fd0b44b34e31756f4fe848d29a7) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), const char *unit, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) opt_flags, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) **target_obj)|
|----|----|

在对象中查找一个选项。

只考虑设置了所有指定标志的选项。


参数：obj：指向结构体的指针，其第一个元素是指向AVClass的指针。 或者，如果设置了AV_OPT_SEARCH_FAKE_OBJ搜索标志，则指向AVClass的双指针。

   name：要查找的选项的名称。

   uint：当搜索命名常量时，它所属的单位的名称。

opt_flags：仅查找所有指定的标志设置的选项（AV_OPT_FLAG）。

search_flags：AV_OPT_SEARCH_ *的组合。

target_obj：如果非NULL，则该选项所属的对象将在此处写入。 如果AV_OPT_SEARCH_CHILDREN存在于search_flags中，它可能与obj不同。
 如果search_flags包含AV_OPT_SEARCH_FAKE_OBJ，则忽略此参数。

返回：指向找到的选项的指针，如果没有找到选项，则返回NULL。





|const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *|[av_opt_next](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gabc75970cd87d1bf47a4ff449470e9225) (const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *prev)|
|----|----|

迭代所有属于obj的AVOptions。

参数：obj：一个AVOptions启用的结构体或一个描述它的AVClass的双指针。

   prev：先前调用此对象上的av_opt_next（）的结果或NULL

返回：下一个AVOption或NULL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_opt_child_next](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaacce30022d2e99c7b9309d3ad3345f69) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *prev)|
|----|----|

迭代支持obo的启用AVOptions的子对象。

参数：prev：以前调用此函数或NULL的结果

返回：下一个启用AVOptions的子项或NULL





|const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *|[av_opt_child_class_next](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gadee1f425013a0f861a40a72d80f189b1) (const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *parent, const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *prev)|
|----|----|

迭代潜在的启用AVOptions的父母子女。

参数：prev：以前调用此函数或NULL的结果

返回：AVClass对应于下一个潜在的小孩或NULL





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *|[av_opt_ptr](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gab1d208d09e487741a28016919225bef6) (const [AVClass](http://ffmpeg.org/doxygen/trunk/structAVClass.html) *avclass, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c))|
|----|----|

获取一个指向struct中请求的字段的指针。

这个函数允许访问一个结构体，即使它的字段被移动或重新命名，因为进行访问的应用程序已被编译，


返回：一个指向该字段的指针，可以将其转换为正确的类型并读取或写入。





|[void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac)|[av_opt_freep_ranges](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga880756f24644ff66c806ed2d9ab3d790) ([AVOptionRanges](http://ffmpeg.org/doxygen/trunk/structAVOptionRanges.html) **ranges)|
|----|----|

释放一个AVOptionRanges结构并将其设置为NULL。





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_query_ranges](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga9751db4da3e0facd522f5b345f4001ef) ([AVOptionRanges](http://ffmpeg.org/doxygen/trunk/structAVOptionRanges.html) **, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

获取给定选项的允许范围列表。

返回的列表可能取决于obj中的其他字段，例如配置文件。


参数：flags：是一个位掩码的标志，未定义的标志不应该被设置，应该被忽略AV_OPT_SEARCH_FAKE_OBJ表示obj是一个双指针到AVClass而不是一个完整的实例AV_OPT_MULTI_COMPONENT_RANGE表示该函数可能返回多个组件，结果必须使用av_opt_freep_ranges来释放。

返回：成功返回的数量，否则为否定错误代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_copy](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga0bcbbef33c677ddbf37d423dcb9c7833) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *dest, const [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *[src](http://ffmpeg.org/doxygen/trunk/avxsynth__c_8h.html#a67caf60f8ec76e0adc7b693bfb651b66))|
|----|----|

将选项从src对象复制到dest对象中。

需要内存分配的选项（例如字符串或二进制）在目标对象中被错误地配置。 分配给这些选项的原始内存将被释放，除非src和dest选项都指向同一个内存。


参数：dest：要复制的对象

   src：复制的对象

返回：0成功，否定错误





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_query_ranges_default](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga4e50bee09b6a374cc54237314892c001) ([AVOptionRanges](http://ffmpeg.org/doxygen/trunk/structAVOptionRanges.html) **, [void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *key, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4))|
|----|----|

获取给定选项的允许范围的默认列表。

此列表是在不使用AVClass.query_ranges（）回调的情况下构建的，可用作回调中的回退。


参数：flags：是一个位掩码的标志，未定义的标志不应该被设置，应该被忽略AV_OPT_SEARCH_FAKE_OBJ表示obj是一个双指针到AVClass而不是一个完整的实例AV_OPT_MULTI_COMPONENT_RANGE表示该函数可能返回多个组件，结果必须使用av_opt_free_ranges来释放。

返回：成功返回的数量，否则为否定错误代码





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_is_set_to_default](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga3ca54258a4c5ac18999dd19b4444c15a) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const [AVOption](http://ffmpeg.org/doxygen/trunk/structAVOption.html) *o)|
|----|----|

检查给定选项是否设置为其默认值。

选项o必须属于对象。 不能调用此函数来检查子项的选项状态。


参数：obj：AVClass对象检查选项

   o：被检查的选项

返回：> 0，当选项设置为默认值时，0选项未设置为默认值时，错误为<0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_is_set_to_default_by_name](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#gaa37a02c9d3e6d9f049f2e7174811f5fb) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, const char *[name](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a09d548d751fcf0b9b67ea230712ae06c), [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) search_flags)|
|----|----|

检查给定选项是否设置为其默认值。

参数：obj：AVClass对象检查选项

   name：选项名称

search_flags：组合AV_OPT_SEARCH_ *

返回：> 0，当选项设置为默认值时，0选项未设置为默认值时，错误为<0





|[int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)|[av_opt_serialize](http://ffmpeg.org/doxygen/trunk/group__avoptions.html#ga523122ff19a79f2867a9da29bbafa33b) ([void](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#ac5958db46ec541224d0b0c2f10a941ac) *obj, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11) opt_flags, [int](http://ffmpeg.org/doxygen/trunk/ffmpeg__filter_8c.html#a61569f2965b7a369eb10b6d75d410d11)[flags](http://ffmpeg.org/doxygen/trunk/log_8c.html#ac8bf36fe0577cba66bccda3a6f7e80a4), char **[buffer](http://ffmpeg.org/doxygen/trunk/opengl__enc_8c.html#a3667f558219c90437106b544a3ca00b8), const char key_val_sep, const char pairs_sep)|
|----|----|

序列化对象的选项。

创建一个包含对象的序列化选项的字符串。 这样的字符串可以传回给av_opt_set_from_string（），以恢复选项值。 序列化值或名称字符串中出现的键/值或对分隔符通过av_escape（）函数进行转义。


参数：obj：AVClass对象进行序列化

opt_flags；将所有指定的标志设置为序列化选项（AV_OPT_FLAG）

   flags：组合AV_OPT_SERIALIZE_ *标志

   buffer：指向缓冲区的指针将分配给包含序列化选项的字符串。 当不再需要时，调用者必须释放缓冲区。

key_val_sep：用于将键与值分离的字符

pairs_sep：用于将两个对彼此分开的字符

返回：> = 0成功，否定错误



