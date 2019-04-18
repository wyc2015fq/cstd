# [ffmpeg] 定制滤波器 - TaigaComplex求职中 - 博客园







# [[ffmpeg] 定制滤波器](https://www.cnblogs.com/TaigaCon/p/10171464.html)





如果有定制ffmpeg滤波器的需求，有两个结构体是必须要了解的：AVFilter、AVFilterPad，所定制的滤波器主要就是通过填充这两个结构体来实现的。我们下面将详细解析这两个结构体，并通过对滤波器的初始化流程以及滤波流程进行分析，进一步加深对ffmpeg滤波框架的了解。



# AVFilter

AVFilter就是一个滤波器的主体，其结构体定义如下：
typedef struct AVFilter {
    const char *name;
    const char *description;
    const AVFilterPad *inputs;
    const AVFilterPad *outputs;
    const AVClass *priv_class;
    int flags;
    int (*preinit)(AVFilterContext *ctx);
    int (*init)(AVFilterContext *ctx);
    int (*init_dict)(AVFilterContext *ctx, AVDictionary **options);
    void (*uninit)(AVFilterContext *ctx);
    int (*query_formats)(AVFilterContext *);
    int priv_size;      
    int flags_internal; 
    struct AVFilter *next;
    int (*process_command)(AVFilterContext *, const char *cmd, const char *arg, char *res, int res_len, int flags);
    int (*init_opaque)(AVFilterContext *ctx, void *opaque);
    int (*activate)(AVFilterContext *ctx);
} AVFilter;
其各个成员变量有如下含义：
|**name**|滤波器名字。|
|----|----|
|description|滤波器的简短介绍。|
|**inputs**|滤波器入口（AVFilterPad）列表。|
|**outputs**|滤波器出口（AVFilterPad）列表。|
|**priv_class**|主要用于维护用户传入的参数（AVOption）的结构体，一般来说用户向滤波器传入参数有两个手段：在创建滤波器实例的时候传入指定参数的字符串，或者在创建完成滤波器实例后通过[av_opt_set](https://ffmpeg.org/doxygen/3.3/group__opt__set__funcs.html#ga5fd4b92bdf4f392a2847f711676a7537)之类的接口传入字符串。|
|flags|滤波器标志。|
|preinit|滤波器预初始化函数。这个函数会在创建滤波器实例的开头被调用。|
|**init**|滤波器自身的特制初始化函数。初始化，即[avfilter_graph_create_filter](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gac0788a9ab6966dba9318b5d5c7524fea)，可以被分解成通用的初始化以及特制初始化。通用初始化通常包含三个步骤：  1. 创建用于存放滤波器实例的内存，进行一些初始化默认的赋值处理。  2.把传入的字符串解析进行解析得到字典的两要素：参数名称key,参数值val。  3.通过priv_class所维护的AVOption，可以找到名为key的参数对应的内存位置（即滤波器实例的私有结构体priv中名称为key的参数的位置），并把val写入该位置当中即可完成参数设置。私有结构体priv中的参数就通常就是滤波器的实际参数，在进行滤波时会根据其中的参数进行滤波处理。特制的初始化有很多不同的用途，比如检查参数，如果检查到所输入的参数中缺少一些重要的参数，则可以返回负值来表示初始化错误。|
|init_dict|与上方init功能相同，不太常用。|
|uninit|如果在init函数出现错误则会调用uninit来做一些后续处理。|
|**query_formats**|为了进行滤波器之间的[滤波格式协商](https://www.cnblogs.com/TaigaCon/p/10111326.html)，AVFilter的query_formats函数会去设置AVFilterLink上的in_formats/out_formats等，这是格式协商的第一步。|
|priv_size|滤波器实例的私有结构体（priv）的大小。我们前面也说了priv当中的参数就是滤波器的实际参数，而不同滤波器的参数不同，那么所占用的空间也不会一样，因此在创建滤波器实例的时候会根据priv_size来开辟用于存放参数的空间。|
|flags_internal|滤波器内部标志。|
|next|在新版本ffmpeg中不会使用到这个next参数。老版本的ffmpeg需要用[avfilter_rigister](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#ga3db814dea5d918f88af888d484f77fb8)来注册滤波器（AVFilter），注册的时候就会使用这个next参数，使得所有注册了的滤波器形成一个滤波器链表，如果需要某个滤波器则可以从该链表中获取。新版本的ffmpeg使用的是列表（filter_list）来列出所有的滤波器（AVFilter），一般来说，如果想获得滤波器，可以调用[avfilter_get_by_name](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gadd774ec49e50edf00158248e1bfe4ae6)来轮询列表获得。|
|process_command|一般来说，滤波参数的设置有两种方式：  1. 在初始化时（[avfilter_graph_create_filter](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gac0788a9ab6966dba9318b5d5c7524fea)），输入参数字符串。  2. 在初始化后，配置整个滤波图前（[avfilter_graph_config](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#ga1896c46b7bc6ff1bdb1a4815faa9ad07)），调用[av_opt_set](https://ffmpeg.org/doxygen/3.3/group__opt__set__funcs.html#ga5fd4b92bdf4f392a2847f711676a7537)之类的接口输入参数。为了保证滤波器正常运行，在滤波的过程中一般是不会对滤波参数进行修改的。当然，在滤波过程中调用[av_opt_set](https://ffmpeg.org/doxygen/3.3/group__opt__set__funcs.html#ga5fd4b92bdf4f392a2847f711676a7537)之类的函数是可以修改滤波参数，但是并不能保证滤波器会按照我们预想地那样运行。因为如果按照前面的两种方式设置滤波参数，后面可能还会执行AVFilterPad的config_props操作，而在滤波过程中通过av_opt_set之类的函数去设置滤波参数时是不会再回去继续执行这一步的。不过现实当中确实存在在滤波过程中修改滤波参数的需求，比如说播放音乐时可以调整EQ。此时就可以通过实现process_command这个函数来实现滤波过程中的各种变化。使用[avfilter_graph_send_command](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gaaad7850fb5fe26d35e5d371ca75b79e1)就能触发所指定的滤波器调用其process_command函数。|
|init_opaque|与init功能相同，不常用。|
|**activate**|滤波函数。滤波函数有两种实现方式，一种是通过activate来实现，另一种是后面会说到的AVFilterPad中的filter_frame以及request_frame函数。如果是采用activate的方式，就需要在activate内实现以下流程：  1. 获取前面的滤波器实例输出的帧。具体操作就是调用[ff_inlink_consume_frame](https://ffmpeg.org/doxygen/3.3/avfilter_8c.html#ac1daee38f363044775732a960441861c)来从inlink获取前面滤波器实例输出的帧。        如果所需要的帧未准备好，则需要通知相应的滤波器实例，表明当前滤波器需要帧。具体操作就是调用[ff_inlink_request_frame](https://ffmpeg.org/doxygen/3.3/avfilter_8c.html#aea1024da8862dda53d050c7619bdf9fc)来设置inlink上的frame_wanted_out，该变量就是用于表明inlink的目标滤波器实例，即当前滤波器实例需要前一个滤波器实例输出帧。        如果所需要的帧已准备好，就可以执行滤波操作。  2. 向后面的滤波器实例输出滤波完成的帧。具体操作就是调用[ff_filter_frame](https://ffmpeg.org/doxygen/3.3/avfilter_8c.html#a65d6e9755eadf69367843dfc738143b4)来向outlink输出帧。|





# AVFilterPad

AVFilterPad是滤波器的出口或者入口，其结构定义如下：
struct AVFilterPad {
    const char *name;
    enum AVMediaType type;
    AVFrame *(*get_video_buffer)(AVFilterLink *link, int w, int h);
    AVFrame *(*get_audio_buffer)(AVFilterLink *link, int nb_samples);
    int (*filter_frame)(AVFilterLink *link, AVFrame *frame);
    int (*poll_frame)(AVFilterLink *link);
    int (*request_frame)(AVFilterLink *link);
    int (*config_props)(AVFilterLink *link);
    int needs_fifo;
    int needs_writable;
};
各成员变量具有如下含义：
|name|出/入口（input/output pads）名字。|
|----|----|
|type|支持的帧类型：AVMEDIA_TYPE_VIDEO/AVMEDIA_TYPE_AUDIO。|
|get_video_buffer(input pads only)|提供用于写入视频图像的buffer，一般是向前一个滤波器实例提供。一个滤波器在滤波过程中，可能需要额外的buffer来进行滤波处理，比如scale或者aresample这种格式转换滤波器，在进行滤波处理时，有输入帧作为源材料，输入帧有确实存在的buffer，而为了进行输出，我们需要额外的buffer来存放格式转换后的帧。所需的buffer除了指定的宽与高之外，还有像素格式，这三点是影响buffer大小的因素，像素格式就是输出链上的格式（link->format）。如果一个滤波器实例需要buffer，可以通过[ff_get_video_buffer(outlink, w, h)](https://ffmpeg.org/doxygen/3.3/video_8c.html#a7f0f699d7689ef551622672dcd39a96a)来调用下一个滤波器对应AVFilterPad上的get_video_buffer函数。不过一般来说，是不需要AVFilterPad去实现get_video_buffer这个函数的，因为如果AVFilterPad不实现这个函数，则会调用默认的[ff_default_get_video_buffer](https://ffmpeg.org/doxygen/3.3/video_8c.html#aeac3e4fc069b44947d469b037a435092)，该函数会根据输入的w,h以及link的format来提供buffer。*※ffmpeg中仅有几个filter实现了get_video_buffer（vflip，swapuv等），不过其内部也是调用了*[*ff_default_get_video_buffer*](https://ffmpeg.org/doxygen/3.3/video_8c.html#aeac3e4fc069b44947d469b037a435092)*，并且其它部分的代码看起来并没有起到什么实际作用。*|
|get_audio_buffer(input pads only)|含义同上，提供给上一个滤波器实例调用。调用接口为[ff_get_audio_buffer(outlink, nb_samples)](https://ffmpeg.org/doxygen/3.3/audio_8c.html#a7954d515c464dafcefd68c2a344489a7)，如果没有实现该函数，则会默认调用到函数[ff_default_get_audio_buffer](https://ffmpeg.org/doxygen/3.3/audio_8c.html#a7f6ef4e5a27f396787e4642039d2ef3b)，buffer的大小受到输入参数的nb_samples以及link->channels，link->format的影响。一般来说不需要滤波器实现get_audio_buffer函数。※ffmpeg中并没有实现了get_audio_buffer的滤波器。|
|**filter_frame**(input pads only)|filter_frame是最常见的滤波实现函数。如果AVFilter没有实现activate函数，则会调用默认的activate函数[ff_filter_activate_default](https://ffmpeg.org/doxygen/3.3/avfilter_8c.html#abda1357b88f06c1554eae51fb881b82a)，该函数最终会调用到filter_frame来提供滤波的实现。filter_frame的输入参数中包括滤波实例的inlink以及从inlink上提取的frame，一般来说filter_frame会对该frame进行滤波处理，然后调用[ff_filter_frame](https://ffmpeg.org/doxygen/3.3/avfilter_8c.html#a65d6e9755eadf69367843dfc738143b4)向outlink输出滤波后的帧。|
|poll_frame(output pads only)|设定上poll_frame是用于查看前一个滤波器实例的request_frame能返回多少帧，不过实际上应该是没有用到这个函数的地方。|
|**request_frame **(output pads only)|request_frame其实也是一个用于产生帧的滤波函数，不过观察request_frame的参数可以发现该函数并没有frame作为输入参数，这表明了request_frame有特定的应用场景：1. 如果一个滤波器是源滤波器，仅需要输出帧，则可以在request_frame内生成帧，然后调用ff_filter_frame把帧输出到outlink。ffmpeg中源滤波器的源文件都带有src关键字，如buffersrc以及vsrc/asrc为开头的滤波器。2. 如果一个滤波器希望在EOF后继续输出帧，则可以用request_frame调用ff_filter_frame来进行输出。|
|**config_props**|config_props的调用发生在query_formats之后，此时滤波格式的协调已经完成，也就已经确定了滤波器实例的输入以及输出格式（inlink->format/outlink->format）。如果某些设置需要使用到这些输入输出格式，就可以在config_props中进行设置。如aresampe在config_props中就利用协调完成的format、channel_layout、sample_rate来进行重采样的参数设置。|
|needs_fifo(input pad only)|表明只有当滤波器实例主动请求帧（调用ff_inlink_request frame或者ff_request_frame）的时候，前一个滤波器实例才会向当前滤波器实例输出帧（ff_filter_frame）。如果needs_fifo为1，会自动在当前滤波器实例与前一个滤波器实例之间插入一个名为fifo的滤波器，该滤波器实现了上述功能。|
|needs_writable(input pad only)|表明滤波器需要对pad对应的link所输入的frame进行写入。如进行字幕渲染的ass滤波器就需要对输入的视频帧进行写入。|





# 初始化流程

首先是[avfilter_graph_create_filter](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gac0788a9ab6966dba9318b5d5c7524fea)，即创建滤波器实例。如前面所说，这个函数会在最开头调用滤波器的preinit函数，然后创建滤波器实例并做一些简单的初始化，解析输入的字符串，最后调用滤波器的init函数。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224528405-1308397682.png)

在构建好一整个AVFilterGraph后，就可以调用[avfilter_graph_config](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#ga1896c46b7bc6ff1bdb1a4815faa9ad07)来做graph最后的配置。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224529176-1004975923.png)

其中[graph_insert_fifos](https://ffmpeg.org/doxygen/3.3/avfiltergraph_8c.html#a9a3f1a9df56f7346848e658b1a99859f)中就会对设定了needs_fifo=1的input pad所在的link插入名为fifo的滤波器。
            if (!link->dstpad->needs_fifo)
                continue;

            fifo = f->inputs[j]->type == AVMEDIA_TYPE_VIDEO ?
                   avfilter_get_by_name("fifo") :
                   avfilter_get_by_name("afifo");

            snprintf(name, sizeof(name), "auto_fifo_%d", fifo_count++);

            ret = avfilter_graph_create_filter(&fifo_ctx, fifo, name, NULL,
                                               NULL, graph);

            ret = avfilter_insert_filter(link, fifo_ctx, 0, 0);
[graph_config_formats](https://ffmpeg.org/doxygen/3.3/avfiltergraph_8c.html#aef8b0d3746f278d0b81c1abdf12c7fd0)中如[前一篇文章](https://www.cnblogs.com/TaigaCon/p/10111326.html)所说，就是对整个graph中滤波格式进行协商，协商过后可以确定所有link上的格式。

[graph_config_links](https://ffmpeg.org/doxygen/3.3/avfiltergraph_8c.html#a364e5126c89a6b46ffc771bf368d4ac7)主要目的就是调用pad中的config_props，那么config_props就能根据前面协商得到的link格式做进一步的操作。





# 滤波流程

一般来说，用户会按照如下方式调用滤波API来进行滤波处理：
    ret = av_buffersrc_add_frame(in_filter, pFrame);

    while((ret = av_buffersink_get_frame(out_filter, pFrame))>=0){
        //TODO
    }
#### 向graph输入帧

滤波的流程都是首先调用[av_buffersrc_add_frame](https://ffmpeg.org/doxygen/3.3/group__lavfi__buffersrc.html#ga8fc71cb48c1ad1aa78b48f87daa4cf19)，从向buffersrc输入帧开始的

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224529926-135502033.png)

可以看到调用了buffersrc的request_frame函数，该函数最后用ff_filter_frame向outlink输出帧。、



#### 从graph提取帧

然后调用[av_buffersink_get_frame](https://ffmpeg.org/doxygen/3.3/group__lavfi__buffersink.html#ga653228f4cbca427c654d844a5fc59cfa)，尝试获得buffersink输出的帧，如果返回值大于0则表明得到了一帧，正常情况下如果无法获得帧通常会返回EAGAIN，这表明要求用户向buffersrc输入更多的帧。

av_buffersink_get_frame会向下调用到get_frame_internal，该函数主要作用就是调用滤波器进行滤波，并返回滤波完成的帧。函数实现如下：
static int get_frame_internal(AVFilterContext *ctx, AVFrame *frame, int flags, int samples)
{
    BufferSinkContext *buf = ctx->priv;
    AVFilterLink *inlink = ctx->inputs[0];
    int status, ret;
    AVFrame *cur_frame;
    int64_t pts;

    if (buf->peeked_frame)
        return return_or_keep_frame(buf, frame, buf->peeked_frame, flags);

    while (1) {
        ret = samples ? ff_inlink_consume_samples(inlink, samples, samples, &cur_frame) :
                        ff_inlink_consume_frame(inlink, &cur_frame);
        if (ret < 0) {
            return ret;
        } else if (ret) {
            /* TODO return the frame instead of copying it */
            return return_or_keep_frame(buf, frame, cur_frame, flags);
        } else if (ff_inlink_acknowledge_status(inlink, &status, &pts)) {
            return status;
        } else if ((flags & AV_BUFFERSINK_FLAG_NO_REQUEST)) {
            return AVERROR(EAGAIN);
        } else if (inlink->frame_wanted_out) {
            ret = ff_filter_graph_run_once(ctx->graph);
            if (ret < 0)
                return ret;
        } else {
            ff_inlink_request_frame(inlink);
        }
    }
}
该函数有如下实现逻辑：
- 调用ff_inlink_consume_frame，看是否可以获得滤波完成的帧，如果得到了滤波后的帧，则调用return_or_keep_frame进行返回。
- 调用ff_inlink_acknowledge_status，查看滤波过程中是否出了差错，或者是否到了EOF，是则返回错误。
- frame_wanted_out用于表明当前滤波器是否已经获得了前一个滤波器输出的一帧，等于1则表示未获得，那么需要调用ff_filter_graph_run_once；等于0则表示已获得一帧，或者是第一次调用该函数。

跳出get_frame_internal里面的while循环只有下面几种情况：
- ff_inlink_consume_frame返回值不为0，表明可以返回滤波后的帧。
- ff_inlink_acknowledge_status返回值不为0，表明滤波器运行过程中出错。
- ff_filter_graph_run_once返回值小于0，一般情况下为EAGAIN，表明滤波器需要输入更多的帧作为原料。

如果是第一次调用[av_buffersink_get_frame](https://ffmpeg.org/doxygen/3.3/group__lavfi__buffersink.html#ga653228f4cbca427c654d844a5fc59cfa)，当进入该函数时，正常情况下会按照如下步骤执行：
- 由于是第一次调用，因此frame_wanted_out为0，那么第一次循环会去执行ff_inlink_request_frame，这个函数会把frame_wanted_out设置为1，表明当前滤波器未获得前一个滤波器输出的帧。
- 然后下一次循环时由于frame_wanted_out为1，会去调用ff_filter_graph_run_once。
- 通常来说，经过几次循环调用ff_filter_graph_run_once后，会得到滤波后的帧，此时ff_inlink_consume_frame会返回1，因此就能调用return_or_keep_frame来向用户返回滤波后的帧了。
- 如果需要输入更多的帧才能继续进行滤波，那么会从ff_filter_graph_run_once返回EAGAIN。



#### 激活滤波器

我们前面提到的ff_filter_graph_run_once就是查找已经就绪（ready）的滤波器实例，并对该滤波器进行激活。所得到的滤波器实例会满足两个条件：
- 滤波器的ready值（优先级）更高的会被选上
- 在此基础上，因为查找顺序是按照用户调用[avfilter_graph_create_filter](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#gac0788a9ab6966dba9318b5d5c7524fea)的顺序，因此最先创建的滤波器实例会被选中。
AVFilterContext *avfilter_graph_alloc_filter(AVFilterGraph *graph,
                                             const AVFilter *filter,
                                             const char *name)
{
    s = ff_filter_alloc(filter, name);
    graph->filters[graph->nb_filters++] = s;
}

int ff_filter_graph_run_once(AVFilterGraph *graph)
{
    filter = graph->filters[0];
    for (i = 1; i < graph->nb_filters; i++)
        if (graph->filters[i]->ready > filter->ready)
            filter = graph->filters[i];
    if (!filter->ready)
        return AVERROR(EAGAIN);
    return ff_filter_activate(filter);
}


选出优先级最高的滤波器实例后，首先把该滤波器就绪状态清零，然后开始执行激活操作：如果该滤波器实现了activate函数，则调用该函数，否则调用默认的激活函数ff_filter_activate_default。
int ff_filter_activate(AVFilterContext *filter)
{
    filter->ready = 0;
    ret = filter->filter->activate ? filter->filter->activate(filter) :
          ff_filter_activate_default(filter);
}
如果调用的是默认的激活函数，则会按照以下优先级进行激活处理：
static int ff_filter_activate_default(AVFilterContext *filter)
{
    unsigned i;

    for (i = 0; i < filter->nb_inputs; i++) {
        if (samples_ready(filter->inputs[i], filter->inputs[i]->min_samples)) {
            return ff_filter_frame_to_filter(filter->inputs[i]);
        }
    }
    for (i = 0; i < filter->nb_inputs; i++) {
        if (filter->inputs[i]->status_in && !filter->inputs[i]->status_out) {
            av_assert1(!ff_framequeue_queued_frames(&filter->inputs[i]->fifo));
            return forward_status_change(filter, filter->inputs[i]);
        }
    }
    for (i = 0; i < filter->nb_outputs; i++) {
        if (filter->outputs[i]->frame_wanted_out &&
            !filter->outputs[i]->frame_blocked_in) {
            return ff_request_frame_to_filter(filter->outputs[i]);
        }
    }
    return FFERROR_NOT_READY;
}
可以发现共有三种激活处理方式，分别为：
- 从上往下传递滤波完成的帧。
- 从上往下传递错误代码。
- 从下往上传递帧的请求。

前面的方式优先级更高。下面我们会对这三种方式进行较为详细的分析

##### ff_filter_frame_to_filter

触发条件是：当前滤波器实例的input link上的samples_ready。这samples_ready表明了前面的滤波器实例已经执行ff_filter_frame向当前滤波器实例的input link输出了帧。

有了上述条件，那么为了完成当前滤波器实例的滤波操作，接下来会执行：
- 从input link上提取出帧。
- 调用当前滤波器的filter_frame函数来执行滤波操作。
- 一般来说滤波处理完成过后也会调用ff_filter_frame来把滤波完成的帧输出到output link。
- ff_filter_frame接下来会把下一个滤波器实例设为就绪状态。
- 如果在执行filter_frame时出错，没能完成滤波处理，则会把错误代码写入input link的status_out，表明该link无法顺利输出frame到当前filter。
- 如果执行filter_frame成功，考虑到前一个滤波器实例可能输出了不止一帧，因此再次把当前滤波器实例设为就绪状态，如果前一个滤波器确实输出了多个帧，这部操作会使得这多个帧都被当前滤波器实例滤波完成后才会执行下一个滤波器实例的滤波处理。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224530792-767251985.png)



##### foward_status_change

触发条件是：当前滤波器实例的input link上有status_in && !status_out。status_in不为0表明调用前一个滤波器实例的request_frame时出现了错误，status_in的值就是错误代码，我们需要向后面的滤波器实例传播这个错误代码。

forward_status_change是传播错误代码的入口，传播错误代码的很大一步分的实现都在ff_request_frame函数内。基于上述条件，即调用前一个滤波器实例的request_frame时出现了错误。
- forward_status_change会调用当前滤波器实例的request_frame来传输错误代码，request_frame一般内部都会调用到ff_request_farme函数。
- 错误代码status_in会导致ff_request_frame走入错误处理分支。
- 把status_out的值设置为status_in。
- 那么返回值也会是该错误代码，这用于表示当前request_frame也出现了错误。
- 因此在返回的时候会把下一个滤波器实例的input link（即当前滤波器实例的output link）上的status_in设置为错误代码。
- 把frame_wanted_out设置为0用于表示当前滤波器实例的错误处理已完成，避免再次调用当前滤波器实例的request_frame函数做重复的错误处理。
- 把下一个滤波器实例设置为就绪状态。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224532014-518819674.png)



##### ff_request_frame_to_filter

触发条件是：当前滤波器的output link上有frame_wanted_out以及!frame_blocked_in。其中frame_blocked_in用于防止重复对同一个link执行request frame操作，重要的是frame_wanted_out。我们前面也说过，如果一个link上的frame_wanted_out=1，表明该link的dst要求src输出帧，具体一点，就是这会导致执行src的request_frame函数。那么这里的output link上的frame_wanted_out=1就会导致：
- 当前的滤波器实例的request_frame被执行。
- request_frame内一般会调用ff_request_frame函数。一般情况下，该函数会把input link的frame_wanted_out设置为1。
- 然后把前一个滤波器实例设为就绪状态。
- 如果request_frame出错的话，就是request_frame没能处理完成，则会把错误代码写入outlink的status_in，表明该link无法顺序求得当前滤波器实例的帧。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181224224533165-758106272.png)



注意：上面所描述的三种处理方式只是比较常见方式。在实际应用中，有些滤波器在filter_frame内调用ff_request_frame，也有些滤波器在request_frame内调用ff_filter_frame的，而且实现了activate函数的滤波器并不会执行上述流程，如果全部都列出来就过于冗余了，具体问题具体分析是很重要的。



此外，我们可以看到在执行ff_filter_frame时会把ready值设置为300；在执行ff_request_frame时会吧ready值设置为100；在保存错误代码时会把ready值设置为200。这表明了优先进行滤波处理，其次是错误处理，最后才是帧请求。

滤波的激活处理可以按照如下进行总结：
- 优先执行滤波处理，如果前面的滤波器实例输出帧，当前的滤波器实例就可以执行滤波处理，并把滤波后得到的帧向后传递。
- 如果在滤波时缺乏帧作为原料，则向前要求前面的滤波器实例输出帧，这个要求会一直往前发送，直到有滤波器实例可以输出帧。
- 如果在滤波或者请求帧的过程中出错了，就把错误往后传递，典型的如EOF就是这种传递方式。












