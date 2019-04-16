# FFmpeg(4)-使用avformat_find_stream_info()来探测获取封装格式的上下文信息 - 夜行过客 - 博客园







# [FFmpeg(4)-使用avformat_find_stream_info()来探测获取封装格式的上下文信息](https://www.cnblogs.com/yongdaimi/p/9797128.html)







```
/**
 * Read packets of a media file to get stream information. This
 * is useful for file formats with no headers such as MPEG. This
 * function also computes the real framerate in case of MPEG-2 repeat
 * frame mode.
 * The logical file position is not changed by this function;
 * examined packets may be buffered for later processing.
 *
 * @param ic media file handle
 * @param options  If non-NULL, an ic.nb_streams long array of pointers to
 *                 dictionaries, where i-th member contains options for
 *                 codec corresponding to i-th stream.
 *                 On return each dictionary will be filled with options that were not found.
 * @return >=0 if OK, AVERROR_xxx on error
 *
 * @note this function isn't guaranteed to open all the codecs, so
 *       options being non-empty at return is a perfectly normal behavior.
 *
 * @todo Let the user decide somehow what information is needed so that
 *       we do not waste time getting stuff the user does not need.
 */
int avformat_find_stream_info(AVFormatContext *ic, AVDictionary **options);
```

该函数主要用于获取视频流信息。

在一些格式当中没有头部信息，如flv格式，h264格式，这个时候调用avformat_open_input()在打开文件之后就没有参数，也就无法获取到里面的信息。这个时候就可以调用此函数，因为它会试着去探测文件的格式，但是如果格式当中没有头部信息，那么它只能获取到编码、宽高这些信息，还是无法获得总时长。如果总时长无法获取到，则仍需要把整个文件读一遍，计算一下它的总帧数。

例：

```
if (avformat_find_stream_info(ic, 0) >=0 ) {
        LOGI("duration is: %lld, nb_stream is: %d", ic->duration, ic->nb_streams);
}
```

其中的ic就是 AVFormatContext 的指针。














