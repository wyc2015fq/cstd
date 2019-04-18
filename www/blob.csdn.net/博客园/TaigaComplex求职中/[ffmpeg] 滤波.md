# [ffmpeg] 滤波 - TaigaComplex求职中 - 博客园







# [[ffmpeg] 滤波](https://www.cnblogs.com/TaigaCon/p/10067871.html)





ffmpeg中有很多已经实现好的滤波器，这些滤波器的实现位于libavfilter目录之下，用户需要进行滤波时，就是是调用这些滤波器来实现的。ffmpeg对于调用滤波器有一整套的调用机制。



# 基本结构

我们把一整个滤波的流程称为滤波过程。下面是一个滤波过程的结构

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181204223756753-1114531886.png)

图中简要指示出了滤波所用到的各个结构体，各个结构体有如下作用：
|**AVFilterGraph**|用于统合这整个滤波过程的结构体。|
|----|----|
|**AVFilter**|滤波器，滤波器的实现是通过AVFilter以及位于其下的结构体/函数来维护的。|
|**AVFilterContext**|一个滤波器实例，即使是同一个滤波器，但是在进行实际的滤波时，也会由于输入的参数不同而有不同的滤波效果，AVFilterContext就是在实际进行滤波时用于维护滤波相关信息的实体。|
|**AVFilterLink**|滤波器链，作用主要是用于连接相邻的两个AVFilterContext。为了实现一个滤波过程，可能会需要多个滤波器协同完成，即一个滤波器的输出可能会是另一个滤波器的输入，AVFilterLink的作用是串联两个相邻的滤波器实例，形成两个滤波器之间的通道。|
|**AVFilterPad**|滤波器的输入输出端口，一个滤波器可以有多个输入以及多个输出端口，相邻滤波器之间是通过AVFilterLink来串联的，而位于AVFilterLink两端的分别就是前一个滤波器的输出端口以及后一个滤波器的输入端口。|
|**buffersrc**|一个特殊的滤波器，这个滤波器的作用就是充当整个滤波过程的入口，通过调用该滤波器提供的函数（如av_buffersrc_add_frame）可以把需要滤波的帧传输进入滤波过程。在创建该滤波器实例的时候需要提供一些关于所输入的帧的格式的必要参数（如：time_base、图像的宽高、图像像素格式等）。|
|**buffersink**|一个特殊的滤波器，这个滤波器的作用就是充当整个滤波过程的出口，通过调用该滤波器提供的函数（如av_buffersink_get_frame）可以提取出被滤波过程滤波完成后的帧。|





# 创建简单的滤波过程

创建整个滤波过程包含以下步骤：

首先需要得到整个滤波过程所需的滤波器（AVFilter），其中buffersrc以及buffersink是作为输入以及输出所必须的两个滤波器。
    const AVFilter *buffersrc  = avfilter_get_by_name("buffer");
    const AVFilter *buffersink = avfilter_get_by_name("buffersink");
    const AVFilter *myfilter   = avfilter_get_by_name("myfilter");
创建统合整个滤波过程的滤波图结构体（AVFilterGraph）
    filter_graph = avfilter_graph_alloc();
创建用于维护滤波相关信息的滤波器实例（AVFilterContext）
    AVFilterContext *in_video_filter = NULL;
    AVFilterContext *out_video_filter = NULL;
    AVFilterContext *my_video_filter = NULL;
    avfilter_graph_create_filter(&in_video_filter, buffersrc, "in", args, NULL, filter_graph);
    avfilter_graph_create_filter(&out_video_filter, buffersink, "out", NULL, NULL, filter_graph);
    avfilter_graph_create_filter(&my_video_filter, myfilter, "myfilter", NULL, NULL, filter_graph);

用AVFilterLink把相邻的两个滤波实例连接起来
    avfilter_link(in_video_filter, 0, my_video_filter, 0);
    avfilter_link(my_video_filter, 0, out_video_filter, 0);
提交整个滤波图
    avfilter_graph_config(filter_graph, NULL);




# 创建复杂的滤波过程

当滤波过程复杂到一定程度时，即需要多个滤波器进行复杂的连接来实现整个滤波过程，这时候对于调用者来说，继续采用上述方法来构建滤波图就显得不够效率。对于复杂的滤波过程，ffmpeg提供了一个更为方便的滤波过程创建方式。

这种复杂的滤波器过程创建方式要求用户以字符串的方式描述各个滤波器之间的关系。如下是一个描述复杂滤波过程的字符串的例子：
    [0]trim=start_frame=10:end_frame=20[v0];\
    [0]trim=start_frame=30:end_frame=40[v1];\
    [v0][v1]concat=n=2[v2];\
    [1]hflip[v3];\
    [v2][v3]overlay=eof_action=repeat[v4];\
    [v4]drawbox=50:50:120:120:red:t=5[v5]
以上是一个连续的字符串，为了方便分析我们把该字符串进行了划分，每一行都是一个滤波器实例，对于一行：
- 开头是一对中括号，中括号内的是输入的标识名0。
- 中括号后面接着的是滤波器名称trim。
- 名称后的第一个等号后面是滤波器参数start_frame=10:end_frame=20，这里有两组参数，两组参数用冒号分开。
- 第一组参数名称为start_frame，参数值为10，中间用等号分开。
- 第二组参数名称为end_frame，参数值为20，中间用等号分开。
- 最后也有一对中括号，中括号内的是输出的标识名v0。
- 如果一个滤波实例的输入标识名与另一个滤波实例的输出标识名相同，则表示这两个滤波实例构成滤波链。
- 如果一个滤波实例的输入标识名或者输出标识名一直没有与其它滤波实例的输出标识名或者输入标识名相同，则表明这些为外部的输入输出，通常我们会为其接上buffersrc以及buffersink。

按照这种规则，上面的滤波过程可以被描绘成以下滤波图：

![image](https://img2018.cnblogs.com/blog/421096/201812/421096-20181204223757791-1107112009.png)

ffmpeg提供一个函数用于解析这种字符串：[avfilter_graph_parse2](https://ffmpeg.org/doxygen/3.3/group__lavfi.html#ga6c3c39e0861653c71a23f90d1397239d)。这个函数会把输入的字符串生成如上面的滤波图，不过我们需要自行生成buffersrc以及buffersink的实例，并通过该函数提供的输入以及输出接口把buffersrc、buffersink与该滤波图连接起来。整个流程包含以下步骤：

创建统合整个滤波过程的滤波图结构体（AVFilterGraph）
    filter_graph = avfilter_graph_alloc();
解析字符串，并构建该字符串所描述的滤波图
    avfilter_graph_parse2(filter_graph, graph_desc, &inputs, &outputs);
其中inputs与outputs分别为输入与输出的接口集合，我们需要为这些接口接上输入以及输出。
    for (cur = inputs, i = 0; cur; cur = cur->next, i++) {
        const AVFilter *buffersrc = avfilter_get_by_name("buffer");
        avfilter_graph_create_filter(&filter, buffersrc, name, args, NULL, filter_graph);
        avfilter_link(filter, 0, cur->filter_ctx, cur->pad_idx);
    }
    avfilter_inout_free(&inputs);

    for (cur = outputs, i = 0; cur; cur = cur->next, i++) {
        const AVFilter *buffersink = avfilter_get_by_name("buffersink");
        avfilter_graph_create_filter(&filter, buffersink, name, NULL, NULL, filter_graph);
        avfilter_link(cur->filter_ctx, cur->pad_idx, filter, 0);
    }
    avfilter_inout_free(&outputs);
提交整个滤波图
    avfilter_graph_config(filter_graph, NULL);




# 滤波API

上面主要讨论了如何创建滤波过程，不过要进行滤波还需要把帧传输进入该过程，并在滤波完成后从该过程中提取出滤波完成的帧。

buffersrc提供了向滤波过程输入帧的API：[av_buffersrc_add_frame](https://ffmpeg.org/doxygen/3.3/group__lavfi__buffersrc.html#ga8fc71cb48c1ad1aa78b48f87daa4cf19)。向指定的buffersrc实例输入想要进行滤波的帧就可以把帧传入滤波过程。
    av_buffersrc_add_frame(c->in_filter, pFrame);
buffersink提供了从滤波过程提取帧的API：[av_buffersink_get_frame](https://ffmpeg.org/doxygen/3.3/group__lavfi__buffersink.html#ga653228f4cbca427c654d844a5fc59cfa)。可以从指定的buffersink实例提取滤波完成的帧。
    av_buffersink_get_frame(c->out_filter, pFrame);
当av_buffersink_get_frame返回值大于0则表示提取成功。












