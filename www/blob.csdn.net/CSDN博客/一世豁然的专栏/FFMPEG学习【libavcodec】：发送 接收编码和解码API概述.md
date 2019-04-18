# FFMPEG学习【libavcodec】：发送/接收编码和解码API概述 - 一世豁然的专栏 - CSDN博客





2017年05月15日 14:23:05[一世豁然](https://me.csdn.net/Explorer_day)阅读数：10686








avcodec_send_packet（）/ avcodec_receive_frame（）/ avcodec_send_frame（）/ avcodec_receive_packet（）函数提供了一个输入和输出的编码/解码API。




API与编码/解码和音频/视频非常相似，工作原理如下：


1、像往常一样设置和打开AVCodecContext。




2、发送有效输入：

 1）、对于解码，请调用avcodec_send_packet（）以在AVPacket中给出解码器原始的压缩数据。

 2）、对于编码，请调用avcodec_send_frame（）为编码器提供包含未压缩音频或视频的AVFrame。 在这两种情况下，建议对AVPackets和AVFrames进行重新计数，否则libavcodec可能必须复制输入数据。 （libavformat总是返回引用计数的AVPackets，av_frame_get_buffer（）分配引用计数的AVFrames）




3、在循环中接收输出。 定期调用avcodec_receive _ *（）函数并处理其输出：

 1）、对于解码，请调用avcodec_receive_frame（）。 成功后，它将返回一个包含未压缩音频或视频数据的 AVFrame。

 2）、对于编码，请调用avcodec_receive_packet（）。 一旦成功，它将返回带有压缩帧的AVPacket。 重复此呼叫，直到它返回AVERROR（EAGAIN）或错误。 AVERROR（EAGAIN）返回值意味着需要新的输入数据才能返回新的输出。 在这种情况下，继续发送输入。 对于每个输入帧/包，编解码器通常将返回1个输出帧/包，但也可以是0或大于1。




在解码或编码开始时，编解码器可能会接收多个输入帧/数据包而不返回帧，直到其内部缓冲区被填充为止。 如果您遵循上述步骤，这种情况是透明的。





理论上来说，发送输入可能会导致EAGAIN - 只有在没有收到所有输出的情况下才会发生。 您可以使用它来构建除上述建议之外的其他解码或编码循环。 例如，如果返回EAGAIN，您可以尝试在每次迭代中发送新的输入，并尝试接收输出。





结束流情况。 这些需要“刷新”（也称为排水）编解码器，因为编解码器可能在内部缓冲多个帧或数据包以实现性能或不必要（考虑B帧）。 处理如下：


1、发送NULL到avcodec_send_packet（）（解码）或avcodec_send_frame（）（编码）函数，而不是有效的输入。 这将进入排水模式。




2、在循环中调用avcodec_receive_frame（）（解码）或avcodec_receive_packet（）（编码），直到返回AVERROR_EOF。 除非您忘记进入排水模式，否则这些功能将不会返回AVERROR（EAGAIN）。




3、在再次解码之前，必须使用avcodec_flush_buffers（）重新编码。




强烈建议您使用上述API。 但是也可以调用这个刚性模式之外的函数。 例如，您可以重复调用avcodec_send_packet（），而无需调用avcodec_receive_frame（）。 在这种情况下，avcodec_send_packet（）将成功，直到编解码器的内部缓冲区已经被填满（通常在初始输入后每个输出帧大小为1），然后使用AVERROR（EAGAIN）拒绝输入。 一旦它开始拒绝输入，你别无选择，只能读取至少一些输出。





并不是所有的编解码器都将遵循刚性和可预测的数据流; 唯一的保证是在一端的发送/接收呼叫上的AVERROR（EAGAIN）返回值意味着另一端的接收/发送呼叫将成功，或至少不会因AVERROR（EAGAIN）而失败。 一般来说，没有编解码器将允许输入或输出的无限制缓冲。





此API替代以下遗留功能：


1、avcodec_decode_video2（）和avcodec_decode_audio4（）：使用avcodec_send_packet（）将输入提供给解码器，然后使用avcodec_receive_frame（）在每个数据包之后接收解码的帧。 与旧的视频解码API不同，多个帧可能来自一个数据包。 对于音频，通过部分解码数据包将输入数据包分割成帧变得对API用户是透明的。 您不需要将AVPacket馈送到API两次（除非它被AVERROR（EAGAIN）拒绝 - 然后没有从数据包中读取数据）。 另外，只需要发送一个刷新/排出数据包一次。




2、avcodec_encode_video2（）/ avcodec_encode_audio2（）：使用avcodec_send_frame（）将输入馈送到编码器，然后使用avcodec_receive_packet（）接收编码数据包。 为avcodec_receive_packet（）提供用户分配的缓冲区是不可能的。




3、新的API尚未处理字幕。




在同一AVCodecContext上混合新旧函数调用是不允许的，这将导致未定义的行为。





一些编解码器可能需要使用新的API; 使用旧的API会在调用它时返回一个错误。 所有编解码器都支持新的API。





编码解码器不允许返回发送和接收的AVERROR（EAGAIN）。这将是一个无效的状态，这可能会使编解码器用户进入无限循环。 API没有时间的概念：不可能试图做avcodec_send_packet（）导致AVERROR（EAGAIN），但是1秒后重复的调用接受数据包（没有其他接收/刷新API调用涉及）。 API是一个严格的状态机，时间的流逝不应该影响它。某些与时间相关的行为在某些情况下仍然被认为是可接受的。但是绝对不能在任何时候同时发送/接收EAGAIN。还必须绝对避免当前状态“不稳定”，并且可以在发送/接收API之间“触发”允许进行。例如，在avcodec_send_packet（）调用之后，它只是在AVAROR（EAGAIN）上返回AVERROR（EAGAIN）之后，编码解码器随机地决定它实际上想要消耗数据包而不是返回一个帧。




