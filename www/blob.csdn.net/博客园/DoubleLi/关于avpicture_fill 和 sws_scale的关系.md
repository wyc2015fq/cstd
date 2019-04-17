# 关于avpicture_fill 和 sws_scale的关系 - DoubleLi - 博客园





- avpicture_fill((AVPicture *) pFrameRGB, buffer, PIX_FMT_RGB565, pCodecCtx->width, pCodecCtx->height);

复制代码 这句调用时,pFrameRGB和buffer都是已经申请到的一段内存, 会将pFrameRGB的数据按RGB565格式自动"关联"到buffer。


- sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data, pFrameRGB->linesize)


复制代码


转换完成的数据保存到了pFrameRGB，也自动到了buffer里面。









