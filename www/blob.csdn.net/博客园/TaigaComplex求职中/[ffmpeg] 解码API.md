# [ffmpeg] 解码API - TaigaComplex求职中 - 博客园







# [[ffmpeg] 解码API](https://www.cnblogs.com/TaigaCon/p/10041926.html)





# 版本迭代

ffmpeg解码API经过了好几个版本的迭代，上一个版本的API是
- 解码视频：[avcodec_decode_video2](https://ffmpeg.org/doxygen/3.3/group__lavc__decoding.html#ga3ac51525b7ad8bca4ced9f3446e96532)- 解码音频：[avcodec_decode_audio4](https://ffmpeg.org/doxygen/3.3/group__lavc__decoding.html#gaaa1fbe477c04455cdc7a994090100db4)


我们现在能看到的很多解码例子用的都是这两个，不过现在ffmpeg更推荐用新一代的API
- 向解码器输送数据包：[avcodec_send_packet](https://ffmpeg.org/doxygen/3.3/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3)- 从解码器获取帧：[avcodec_receive_frame](https://ffmpeg.org/doxygen/3.3/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c)




通常来说，一个packet会被解码出一个frame，不过也存在一个packet被解码出多个frame或者多个packet才能解码出一个frame的情况，甚至也有些解码器在输入以及输出端上可能会有延迟。因此原来的API在某种程度上存在对调用者误导的可能，使得调用者认为输入的一个或者多个Packet就对应着解码器所输出的一个frame，但实际上可能并非如此。

新的API完全隐藏了“解码”这一概念，只提供一个输入packet的接口以及输出frame的接口，如此一来调用者可以不必了解解码器的具体细节，只需要了解这两个接口的调用规则就能写出适用于所有解码器的代码。





# 状态机

新一代API是一个状态机。调用API是一种动作，API的返回值就是一种状态，通过动作可以进行状态的转换。正常情况下，状态机有6种状态：
- **send 0                **：send_packet返回值为0，正常状态，意味着输入的packet被解码器正常接收。  - **send EAGAIN    **：send_packet返回值为EAGAIN，输入的packet未被接收，需要输出一个或多个的frame后才能重新输入当前packet。  - **send EOF           **：send_packet返回值为EOF，当send_packet输入为NULL时才会触发该状态，用于通知解码器输入packet已结束。  - **receive 0            **：receive_frame返回值为0，正常状态，意味着已经输出一帧。  - **receive EAGAIN**：receive_frame返回值为EAGAIN，未能输出frame，需要输入更多的packet才能输出当前frame。  - **receive EOF       **：receive_frame返回值为EOF，当处于send EOF状态后，调用一次或者多次receive_frame后就能得到该状态，表示所有的帧已经被输出。 

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003949478-1646808234.png)

如上图所示，尽管状态转换稍微有些繁琐，但该状态转换图实际上包含了两种策略，对两种策略分别进行分析能对状态机有一个更为清晰的了解。



#### 以消耗packet为主的策略

虽然我们前面说过输入的packet并不一定对应于所输出的frame，不过在这里为了方便语言上的描述，在这里我们可以认为receive_frame是对输入的packet的一种消耗，当receive_frame返回EAGAIN时就认为所输入的packet被完全消耗。这里的策略就是对每次所输入的一个packet，都循环调用receive_frame对该packet进行消耗，直到所输入的packet消耗完成。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130204630084-1052501183.png)

在消耗完一个packet后输入下一个packet

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003951191-1914317772.png)

当所有的packet都消耗完成后，调用send_packet输入NULL，把状态转换为send EOF，最后调用receive_frame把状态转换为receive EOF即完成所有解码任务。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003952258-580589741.png)





#### 以获取frame为主的策略

本策略是先循环调用send_packet直到返回EAGAIN，此时肯定可以输出frame了

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003952977-2118770409.png)

然后调用receive_frame输出一帧

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003953615-719230522.png)

当所有的packet都输入完成后，调用send_packet输入NULL，把状态转换为send EOF，最后调用receive_frame把状态转换为receive EOF即完成所有解码任务。

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003954223-1276999992.png)





# API代码分析

#### avcodec_send_packet

avcodec_send_packet有如下结构：

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003954860-590982655.png)

首先粗略了解一下bsf，即bitstream filter。音频与视频编码后数据会以一定的语法结构进行构建，除了编码后的数据之外还有一些并非解码所必须的语法元素，这些语法元素通常只是在解码、显示等过程起到辅助作用，这些语法元素很少使用到，它们的位置一般是位于在编码后的数据之前，如h264中的SEI。bitstream filter就是对这些语法元素进行调整。

av_bsf_send_packet会把packet输送到bitstream filter中，在av_bsf_send_packet当中，会判断用于暂存输入packet的buffer_pkt是否为有效packet，如果是有效packet，则表明上次传入的packet仍未被解码器消耗，因此无法接收这次传入的packet，返回EAGAIN。
    if (ctx->internal->buffer_pkt->data ||
        ctx->internal->buffer_pkt->side_data_elems)
        return AVERROR(EAGAIN);
否则就把当前packet移动到用于暂存的buffer_pkt
    av_packet_move_ref(ctx->internal->buffer_pkt, pkt);


decode_receive_frame_internal是实际的解码入口，它有如下结构

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003955587-545005494.png)

decode_receive_frame_internal需要先从用于暂存的buffer_pkt中取出输入的packet，这是调用bsfs_poll来实现的。bsfs_poll会执行所有的bitstream filter，最终会调用到ff_bsf_get_packet_ref，在该函数内，会先判断用于暂存packet的buffer_pkt是否为有效packet，不是则返回EAGAIN
    if (!ctx->internal->buffer_pkt->data &&
        !ctx->internal->buffer_pkt->side_data_elems)
        return AVERROR(EAGAIN);
有效则取出该packet
    av_packet_move_ref(pkt, ctx->internal->buffer_pkt);
取出该packet后就可以调用codec的decode函数来进行解码。

总体来看avcodec_send_packet经历了如下流程。

![image](https://img2018.cnblogs.com/blog/421096/201901/421096-20190112202315201-845073117.png)



#### avcodec_receive_frame

avcodec_receive_frame有如下结构：

![image](https://img2018.cnblogs.com/blog/421096/201811/421096-20181130003956450-658485396.png)

avcodec_receive_frame会先进行判断，如果解码器解码出了一帧，则会调用av_frame_move_ref输出这一帧，否则继续调用decode_receive_frame_internal继续进行解码。
    if (avci->buffer_frame->buf[0]) {
        av_frame_move_ref(frame, avci->buffer_frame);
    } else {
        ret = decode_receive_frame_internal(avctx, frame);
        if (ret < 0)
            return ret;
    }
总体来说avcodec_receive_frame经历了如下流程。

![image](https://img2018.cnblogs.com/blog/421096/201901/421096-20190112202318313-1330889828.png)



#### 关于EAGAIN

我们前面讨论过EAGAIN状态：
- avcodec_send_packet返回EAGAIN表明无法输入当前packet，需要调用avcodec_receive_frame进行消耗上一个packet。 
- avcodec_receive_packet返回EAGAIN表明无法获取当前frame，需要调用avcodec_send_packet输入更多的packet。 




一般来说，在实际的实现中，EAGAIN是由bsf相关的函数返回的。
- 调用avcodec_send_packet时，会先调用av_bsf_send_packet，此时如果用于暂存packet的buffer_pkt中含有有效packet时，av_bsf_send_packet会返回EAGAIN，这会导致avcodec_send_packet也返回EAGAIN。 
- 调用avcodec_receive_frame时，如果没有可输出的frame，则会进入decode_receive_frame_internal分支。此时如果用于暂存packet的buffer_pkt中不含有效packet时，ff_bsf_get_packet_ref会返回EAGAIN，这会导致decode_receive_frame_internal返回EAGAIN，从而也使得avcodec_receive_frame也返回EAGAIN。 


不过我们注意到avcodec_send_packet中也调用了decode_receive_frame_internal，不过avcodec_send_packet会忽视decode_receive_frame_internal所返回的EAGAIN。
        ret = decode_receive_frame_internal(avctx, avci->buffer_frame);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            return ret;
















