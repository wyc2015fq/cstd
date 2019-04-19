# FFMPEG解码流程 - maopig的专栏 - CSDN博客
2011年09月07日 22:36:27[maopig](https://me.csdn.net/maopig)阅读数：2978标签：[codec																[input																[video																[stream																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/maopig/article/category/847674)
1. 注册所有容器格式和CODEC: av_register_all()
2. 打开文件: av_open_input_file()
3. 从文件中提取流信息: av_find_stream_info()
4. 穷举所有的流，查找其中种类为CODEC_TYPE_VIDEO
5. 查找对应的解码器: avcodec_find_decoder()
6. 打开编解码器: avcodec_open()
7. 为解码帧分配内存: avcodec_alloc_frame()
8. 不停地从码流中提取出帧数据: av_read_frame()
9. 判断帧的类型，对于视频帧调用: avcodec_decode_video()
10. 解码完后，释放解码器: avcodec_close()
11. 关闭输入文件:av_close_input_file()
