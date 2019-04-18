# [ffmpeg] 多输入滤波同步方式（framesync） - TaigaComplex求职中 - 博客园







# [[ffmpeg] 多输入滤波同步方式（framesync）](https://www.cnblogs.com/TaigaCon/p/10193627.html)





滤波也不总是单一的输入，也存在对多个输入流进行滤波的需求，最常见的就是对视频添加可视水印，水印的组成通常为原视频以及作为水印的图片或者小动画，在ffmpeg中可以使用overlay滤波器进行水印添加。

对于多视频流输入的滤波器，ffmpeg提供了一个名为framesync的处理方案。framesync为滤波器分担了不同线路的输入的帧同步任务，并为滤波器提供同步过后的帧，使得滤波器专注于滤波处理。



# Extend Mode

由于各个视频流可能长短不一，可能起始或者结束时间也不同，为了应对由此产生的各种需求，framesync为每个输入流的起始以及结束都提供了3种可选的扩展方式
|Mode|before（流开始前）|after（流结束后）|
|----|----|----|
|EXT_STOP|在这个流开始前的这段时间不可以进行滤波处理。如果有多个流都指定了before=EXT_STOP，那么以时间线最后的流为准。|在这个流结束后滤波处理必须停止。如果有多个流都指定了after=EXT_STOP，那么以时间线最前的流为准。|
|EXT_NULL|其余的流可以在缺少了该流的情况下执行滤波处理。|其余的流可以在缺少了该流的情况下执行滤波处理。|
|EXT_INFINITY|在这个流开始前的这段时间，提供这一个流的第一帧给滤波器进行处理。|在这个流结束后的这段时间，提供这一个流的最后一帧给滤波器进行处理。|





# Sync

在framesync所提供的同步服务中，滤波器可以为输入流设置同步等级，同步等级最高的输入流会被当作同步基准。

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181229044129723-1248213237.png)

如上图所示，不同的输入流可能有不同的帧率，因此有必要对输入的流进行同步。上面的例子中，input stream 1的同步级别最高，因此以该流为同步基准，即每次得到input stream 1的帧时，可以进行滤波处理。滤波处理所提供的帧为各个流最近所获得的帧，在上面的例子中，当input stream 1获得序号为2的帧时，input stream 2刚刚所获得的帧序号为3，input stream 3刚刚所获得的帧序号为1，因此滤波时framesync所提供的帧分别为stream 1的2、stream 2的3、stream 3的1。





# Example

滤波器调用framesync需要执行如下代码：
typedef struct Context {
    FFFrameSync fs;           //Context involves FFFrameSync
} Context;

static int process_frame(FFFrameSync *fs)
{
    Context *s = fs->opaque;

    AVFrame *in1, *in2, *in3;
    int ret;

    //get frame before filtering
    if ((ret = ff_framesync_get_frame(&s->fs, 0, &in1, 0)) < 0 ||
        (ret = ff_framesync_get_frame(&s->fs, 1, &in2, 0)) < 0 ||
        (ret = ff_framesync_get_frame(&s->fs, 2, &in3, 0)) < 0)

    //filtering
}

//Before filtering, we can only get timebase in function config_output. //See avfilter_config_links 
static int config_output(AVFilterLink *outlink)
{
    FFFrameSyncIn *in;

    ret = ff_framesync_init(&s->fs, ctx, 3);          //init framesync
    if (ret < 0)
        return ret;

    //set inputs parameter: timebase, sync level, before mode, after mode
    in = s->fs.in;                                    
    in[0].time_base = srclink1->time_base;
    in[1].time_base = srclink2->time_base;
    in[2].time_base = srclink3->time_base;
    in[0].sync   = 2;
    in[0].before = EXT_STOP;
    in[0].after  = EXT_STOP;
    in[1].sync   = 1;
    in[1].before = EXT_NULL;
    in[1].after  = EXT_INFINITY;
    in[2].sync   = 1;
    in[2].before = EXT_NULL;
    in[2].after  = EXT_INFINITY;

    //save Context to fs.opaque which will be used on filtering
    s->fs.opaque   = s;
    
    //filtering function
    s->fs.on_event = process_frame;

    return ff_framesync_configure(&s->fs);       //framesync configure
}

static int activate(AVFilterContext *ctx)
{
    RemapContext *s = ctx->priv;
    return ff_framesync_activate(&s->fs);       //call filtering function if frame ready
}


static av_cold void uninit(AVFilterContext *ctx)
{
    RemapContext *s = ctx->priv;

    ff_framesync_uninit(&s->fs);
}

static const AVFilterPad remap_inputs[] = {
    {
        .name         = "source 1",
        .type         = AVMEDIA_TYPE_VIDEO,
        .config_props = config_input,
    },
    {
        .name         = "source 2",
        .type         = AVMEDIA_TYPE_VIDEO,
    },
    {
        .name         = "source 3",
        .type         = AVMEDIA_TYPE_VIDEO,
    },
    { NULL }
};

static const AVFilterPad remap_outputs[] = {
    {
        .name          = "default",
        .type          = AVMEDIA_TYPE_VIDEO,
        .config_props  = config_output,
    },
    { NULL }
};



可以发现使用framesync有如下要求：
- 在滤波器的参数结构体（Context）内包含FFFramesync结构体。
- 在进行滤波处理时，调用ff_framesync_get_frame来获得framesync同步后的帧。
- 在config_output时或之前调用ff_framesync_init来进行framesync初始化。
- 在config_output时设置各个输入的time base，extend mode，sync level，并调用ff_framesync_configure进行配置。
- 在config_output时或之前设置fs->opaque=context（参数结构体），用于后续滤波处理。
- 在config_output时或之前设置用于回调的滤波处理函数fs->on_event=process_frame。
- 在activate时调用ff_framesync_activate。在该函数内部如果frame ready，就会执行回调函数。





# framesync的同步实现

framesync的同步实现主要集中在ff_framesync_activate所调用的framesync_advance函数当中。
static int framesync_advance(FFFrameSync *fs)
{
    while (!(fs->frame_ready || fs->eof)) {
        ret = consume_from_fifos(fs);
        if (ret <= 0)
            return ret;
    }
    return 0;
}
framesync_advance内是一个循环，退出该循环需要满足任意如下一个条件：
- fs->frame_ready==1。代表接下来可以执行滤波处理。
- fs->eof==1。代表结束整个滤波处理。
- ret = consume_from_fifos(fs) <= 0。返回值小于0代表出错；返回值等于0代表目前无法都从所有的输入流中得到帧。



从consume_from_fifos开始分析，我们将会对framesync的同步机制有详细的了解。
static int consume_from_fifos(FFFrameSync *fs)
{
    AVFilterContext *ctx = fs->parent;
    AVFrame *frame = NULL;
    int64_t pts;
    unsigned i, nb_active, nb_miss;
    int ret, status;

    nb_active = nb_miss = 0;
    for (i = 0; i < fs->nb_in; i++) {
        if (fs->in[i].have_next || fs->in[i].state == STATE_EOF)
            continue;
        nb_active++;
        ret = ff_inlink_consume_frame(ctx->inputs[i], &frame);
        if (ret < 0)
            return ret;
        if (ret) {
            av_assert0(frame);
            framesync_inject_frame(fs, i, frame);
        } else {
            ret = ff_inlink_acknowledge_status(ctx->inputs[i], &status, &pts);
            if (ret > 0) {
                framesync_inject_status(fs, i, status, pts);
            } else if (!ret) {
                nb_miss++;
            }
        }
    }
    if (nb_miss) {
        if (nb_miss == nb_active && !ff_outlink_frame_wanted(ctx->outputs[0]))
            return FFERROR_NOT_READY;
        for (i = 0; i < fs->nb_in; i++)
            if (!fs->in[i].have_next && fs->in[i].state != STATE_EOF)
                ff_inlink_request_frame(ctx->inputs[i]);
        return 0;
    }
    return 1;
}
在consume_from_fifos返回1代表目前已经从所有的输入流中获得了帧。
- 如果已经从某个输入获得了帧，则不需要再次去获取。
- 如果某个输入流还未获得帧，则会调用ff_inlink_comsume_frame尝试从输入link中获取帧。
- 如果得到了帧，就会调用framesync_inject_frame把从输入流中获得的帧存放在fs->in[i].frame_next中，并用fs->in[i].have_next表示第i个输入流已经获得了帧。
- 如果没有获得帧，则调用ff_inlink_acknowledge_status检查是否出错或者EOF，是则表明该输入流结束，不是则表明前面的滤波器实例无法为我们提供帧。
- 由于无法获得我们所需要的帧，因此要调用ff_inlink_request_frame向前面的滤波器实例发出请求。
- 只有当从所有的输入流都得到帧后，consume_from_fifos才会返回1。



consume_from_fifos返回1的时候，所有输入流的帧缓存fs->in[i].frame_next都存储了一帧，该帧缓存标志fs->in[i].have_next的值都为1。然后进行下列同步处理：
static int framesync_advance(FFFrameSync *fs)
{
    unsigned i;
    int64_t pts;
    int ret;

    while (!(fs->frame_ready || fs->eof)) {
        ret = consume_from_fifos(fs);
        if (ret <= 0)
            return ret;

        pts = INT64_MAX;
        for (i = 0; i < fs->nb_in; i++)   //get the least pts frame
            if (fs->in[i].have_next && fs->in[i].pts_next < pts)
                pts = fs->in[i].pts_next;
        if (pts == INT64_MAX) {
            framesync_eof(fs);
            break;
        }
        for (i = 0; i < fs->nb_in; i++) {
            if (fs->in[i].pts_next == pts ||
                (fs->in[i].before == EXT_INFINITY &&
                 fs->in[i].state == STATE_BOF)) {
                av_frame_free(&fs->in[i].frame);
                fs->in[i].frame      = fs->in[i].frame_next; //move from frame_next to frame
                fs->in[i].pts        = fs->in[i].pts_next;
                fs->in[i].frame_next = NULL;
                fs->in[i].pts_next   = AV_NOPTS_VALUE;
                fs->in[i].have_next  = 0;
                fs->in[i].state      = fs->in[i].frame ? STATE_RUN : STATE_EOF;
                if (fs->in[i].sync == fs->sync_level && fs->in[i].frame)//the highest level frame
                    fs->frame_ready = 1;
                if (fs->in[i].state == STATE_EOF &&
                    fs->in[i].after == EXT_STOP)
                    framesync_eof(fs);
            }
        }
        if (fs->frame_ready)
            for (i = 0; i < fs->nb_in; i++)
                if ((fs->in[i].state == STATE_BOF &&
                     fs->in[i].before == EXT_STOP))
                    fs->frame_ready = 0;
        fs->pts = pts;
    }
    return 0;
}
这里我们把frame_next当作从上一滤波器实例中获取的帧缓存，frame当作接下来会用于进行滤波处理的帧缓存。
- 从所缓存的帧（frame_next）中提取pts最小的一帧。
- 存放到用于提供给滤波器的缓存中（frame = frame_next）。
- 把这一帧所在输入流帧缓存设置为空（frame_next = NULL）。
- 如果这一帧所在的输入流是同步级别最高的流，表明此时在frame中该同步级别最高的流所输入的帧的pts最大，符合我们前面的同步描述，因此设置frame_ready = 1，表明接下来可以进行滤波处理。
- 如果这一帧所在的输入流不是同步级别最高的流，则需要继续执行下一循环（执行consume_from_fifos）。

以我们前面所展示的图片为例

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181229044130559-1905682960.png)

每次都把frame_next中pts最小的一帧放入frame时，同时也表明在frame中新所放入的一帧永远是pts最大的一帧。当被放入到frame中的帧是属于最高同步等级的输入流的时候，可以执行滤波处理。如果我们把这一帧的pts定义为同步pts，此时其余的输入流中的帧的pts尽管比同步pts小，不过也是各自输入流中最大的，这与我们前面所说的同步处理是一致的。

framesync的实现总结来说就是循环执行:
- 从输入流中提取帧填补空缺的frame_next。
- 当所有输入流的frame_next都被写入帧后（即所有输入流的have_next都为1）consume_from_fifos才会返回1，然后进行各个流之间的pts比较。
- 接下来把pts最小的帧从frame_next存入frame，如此一来该frame_next又会出现空缺。

这种实现方式能保证所有的帧都是以pts从小到大由frame_next移入frame的，能防止帧被遗漏。












