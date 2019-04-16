# FFmpeg(6)-通过av_find_best_stream()来获取音视流的索引 - 夜行过客 - 博客园







# [FFmpeg(6)-通过av_find_best_stream()来获取音视流的索引](https://www.cnblogs.com/yongdaimi/p/9798249.html)





也可以通过av_find_best_stream()函数来获取流的索引：

例：

```
audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    LOGI("av_find_best_stream, audio index is: %d", audioStream);
```














