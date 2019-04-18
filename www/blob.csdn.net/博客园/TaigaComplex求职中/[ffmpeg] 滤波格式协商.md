# [ffmpeg] 滤波格式协商 - TaigaComplex求职中 - 博客园







# [[ffmpeg] 滤波格式协商](https://www.cnblogs.com/TaigaCon/p/10111326.html)





ffmpeg的中滤波器是以帧为原料来进行滤波的，那么自然地就会对帧的格式有所要求，可以说如果滤波器不知道帧的格式，就无法对帧进行处理。在进行视频滤波时，滤波格式指的是视频的像素格式；在进行音频滤波时，滤波格式包括音频采样格式，采样率以及通道数目。



# 承担协商任务的结构体AVFilterLink

滤波器可能只支持某一种帧格式，也有可能对所有的帧格式都支持，因此在执行滤波操作之前，有必要对滤波过程中的各个滤波器所支持的格式进行协商。

相邻的两个滤波实例之间是由AVFilterLink来连接的，因此AVFilterLink也承担了协商相邻两个滤波实例的滤波格式的任务。AVFilterLink的结构体中与滤波格式相关如下的变量如下：
struct AVFilterLink {
    /*format negotiation valuables*/
    uint64_t channel_layout;
    int sample_rate;
    int format;

    AVFilterFormats *in_formats;
    AVFilterFormats *out_formats;
    AVFilterFormats  *in_samplerates;
    AVFilterFormats *out_samplerates;
    struct AVFilterChannelLayouts  *in_channel_layouts;
    struct AVFilterChannelLayouts *out_channel_layouts;
}
其中，in_formats/in_samplerates/in_channel_layouts、out_formats/out_samplerates/out_channel_layouts分别为当前Link输入端滤波器支持的输出滤波格式以及输出端滤波器支持的输入滤波格式，以Link的视点来说，分别就是可能的输入（所以变量被命名为in）格式以及可能的输出格式（所以变量被命名为out）。而format/sample_rate/channel_layout就是经过协商后得出唯一的格式。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220654510-954875747.png)





# 滤波格式协商

滤波格式的协商分为以下几个步骤：
- 设置所有AVFilterLink上的格式列表
- 如果AVFilterLink的输入输出格式列表中有相同的格式，则提取相同格式作为新的输入以及输出格式
- 否则表明需要进行格式转换
- 提取合并后的格式列表中的某个格式作为最终格式



#### 设置格式列表

滤波格式的协商是在AVFilterLink上执行的，因此滤波器需要向AVFilterLink表明自己所支持的输入以及输出格式，即由AVFilter来设置它周围的AVFilterLink上的格式列表。这个设置的操作是由AVFilter内的query_formats函数来实现的。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220655242-1736492670.png)

query_formats是AVFilter中的一个回调函数，如果某个滤波器对输入以及输出格式有要求或者限制，则需要通过query_formats来设置输入link的out_formats以及输出link的in_formats。如果滤波器不实现query_formats，则表明该滤波器的输入输出默认支持所有格式。
    for (i = 0; i < graph->nb_filters; i++) {
        AVFilterContext *f = graph->filters[i];
        if (formats_declared(f))
            continue;
        if (f->filter->query_formats)
            ret = filter_query_formats(f);
        else
            ret = ff_default_query_formats(f);
        if (ret < 0 && ret != AVERROR(EAGAIN))
            return ret;
        /* note: EAGAIN could indicate a partial success, not counted yet */
        count_queried += ret >= 0;
    }


#### AVFilterLink的格式合并

在设置完graph上所有link的in_formats以及out_formats后，就需要提取每个link上in_formats与out_formats的共同formats，我们称这一步骤为合并（merge）。

在讨论怎么merge formats之前，我们需要先了解AVFilterFormats这一结构体。AVFilterFormats就是我们前文所说的格式列表，定义如下：
struct AVFilterFormats {
    unsigned nb_formats;        ///< number of formats
    int *formats;               ///< list of media formats

    unsigned refcount;          ///< number of references to this list
    struct AVFilterFormats ***refs; ///< references to this list
};
nb_formats是列表中format的数目；formats是指向format列表的指针；refcount代表本列表被引用的次数；refs指向一个列表，该列表中存放的是引用了本列表的地方的地址。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220655882-1190086376.png)

这里的合并，是要从AVFilterLink的in_formats以及out_formats中挑出相同的format，然后组合成新的一个AVFilterFormats。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220656854-1544582005.png)

在实际滤波过程中，我们需要通过名为buffersrc的滤波器输入帧，而buffersrc在进行初始化的时候需要指定唯一的帧格式，即位于整个滤波图头部的是一个format个数为1的AVFilterFormats。并且，对于很多滤波器来说，所支持的输入输出格式是一样的，因此位于滤波器输入端link的out_formats与输出端link的in_formats会指向同一个AVFilterFormats。出于这两个原因，在循环地对graph上的AVFilterLink进行merge的时候，很容易地就能使得整个graph上所有的link中的in_formats与out_formats都指向同一个AVFilterFormats，并且其中含有唯一一个format就是输入帧的format。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220657776-1580006482.png)

如上图就是filter的输入与输出端支持同样格式，因此会把输入link的out_formats以及输出link的in_formats指向同一AVFilterFormats。不过如果filter两端支持不同的格式，则表示该filter内可能对帧的格式进行了转换。



#### 自动格式转换

如果AVFilterLink的in_formats与out_formats中不含有相同format，就表明需要进行格式转换。出现这种情况的时候ffmpeg会在该link上插入一个用于格式转换的滤波器，进行视频像素格式转换的滤波器名为scale，进行音频采样格式转换的滤波器名为aresample。
if (convert_needed) {
    switch (link->type) {
    case AVMEDIA_TYPE_VIDEO:
        filter = avfilter_get_by_name("scale");
        avfilter_graph_create_filter(&convert, filter,
                                        inst_name, graph->scale_sws_opts, NULL,
                                        graph);
        break;
    case AVMEDIA_TYPE_AUDIO:
        filter = avfilter_get_by_name("aresample");
        avfilter_graph_create_filter(&convert, filter,
                                            inst_name, graph->aresample_swr_opts,
                                            NULL, graph);
        break;
    default:
        return AVERROR(EINVAL);
    }

    if ((ret = avfilter_insert_filter(link, convert, 0, 0)) < 0)
        return ret;

    if ((ret = filter_query_formats(convert)) < 0)
        return ret;
    if (!ff_merge_formats( inlink->in_formats,  inlink->out_formats,  inlink->type) ||
        !ff_merge_formats(outlink->in_formats, outlink->out_formats, outlink->type))
        ret = AVERROR(ENOSYS);
插入格式转换滤波器包含以下步骤：
- 插入格式转换滤波器的函数avfilter_insert_filter会把原来的link的出口连接到格式转换滤波器的入口上，然后用新的link连接格式转换滤波器的出口以及源link原来的目标端口。最后还会把源link的out_formats移给新link的out_formats。
- filter_query_formats则会调用格式转换滤波器的query_formats函数来设置其两端的out_formats以及in_formats，由于此时我们一般不会为其设置任何参数，因此此时格式转换滤波器两端的out_formats以及in_formats会支持所有格式。
- 最后的ff_merge_formats把涉及到的这两个link上的格式进行merge，这样就使得格式转换滤波器两端的out_formats以及in_formats设定完毕。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220658827-1465176215.png)



在得到格式转换滤波器两边的两个link的最终格式后，会通过调用格式转换滤波器的config_props函数来进行转换的初始化，初始化时的输入参数就是这两个link的最终格式。
static int config_output(AVFilterLink *outlink)
{
    AVFilterContext *ctx = outlink->src;
    AVFilterLink *inlink = ctx->inputs[0];

    aresample->swr = swr_alloc_set_opts(aresample->swr,
                                        outlink->channel_layout, outlink->format, outlink->sample_rate,
                                        inlink->channel_layout, inlink->format, inlink->sample_rate,
                                        0, ctx);
}




#### 提取最终格式

经过前面的流程，已经能保证link上的in_formats与out_formats是merge过的了，下面处理同一个滤波器的输入以及输出link之间的格式问题。

尽管我们前面说过，对于内部不会进行格式转换的滤波器，通常其输入输出会支持相同的格式，因此输入link的out_formats与输出link的in_formats一般来说都是指向同一个AVFilterFormats，不过滤波器多种多样，也有可能会出现明明可以不用格式转换，输入link的out_formats却与输出link的in_formats却指向不同AVFilterFormats。为了防止滤波器做不必要的格式转换，有以下处理方式：一旦发现输入link的out_formats当中只有唯一一个格式，并且输出link的in_formats当中包含该格式，则会把该格式移动到in_formats[0]，并把格式数目设置为1。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220659634-324384534.png)



在选取link的最终格式时，一般来说link的格式列表中只有一个格式，因此会直接选则这一个格式。
static int pick_format(AVFilterLink *link, AVFilterLink *ref)
{
    link->in_formats->nb_formats = 1;
    link->format = link->in_formats->formats[0];
}


不过也存在特殊情况：如果滤波器支持不同于源格式的多种输出格式，但是用户并没有指定具体的输出格式，那么应该根据源格式从当前列表中选择最优的输出格式。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181212220700343-1333511776.png)












