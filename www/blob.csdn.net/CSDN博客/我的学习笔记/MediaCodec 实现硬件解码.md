# MediaCodec 实现硬件解码 - 我的学习笔记 - CSDN博客





2018年07月15日 15:32:18[zouzhiheng](https://me.csdn.net/u011330638)阅读数：589








# MediaCodec

MediaCodec 是 Android 提供的硬件编解码器，通常配合 MediaExtractor(用于解析媒体文件，获取音视频数据), MediaMuxer（用于封装已编码的音视频数据到文件中）, Image, Surface, AudioTrack（用于播放 PCM 数据） 等类一起使用。相关辅助类有 MediaFormat, MediaCodec.BufferInfo, MediaCodecInfo, MediaCodecList 等。

它使用了一系列 input buffer 和 output buffer，异步地处理数据。简单来说，你首先要请求（或接收）一个空的 input buffer，然后向里面填充数据并发送给编解码器进行处理，编解码器消耗（解码/编码）数据并把数据迁移到它其中一个 ouput buffer 中。

使用流程（以解码为例，编码基本类似）： 

1) MediaCodec.createDecoderByType()，创建解码器 

2) decoder.configure()，配置解码器 

3) decoder.start()，开始运行，MediaCodec 会在这里创建 inputBuffers、outputBuffers 两个 buffer 数组 

4) decoder.dequeueInputBuffer()，获取一个 InputBuffer，用于填充未解码的数据 

5) decoder.queueIntputBuffer()，填充数据完毕后，把这个 buffer 放进 MediaCodec 里面，让它进行解码操作 

6) decoder.dequeueOutputbuffer()，获取一个 OutputBuffer，此时已解码成功，可以使用解码后的数据了 

7) decoder.releaseOutputbuffer()，如果不再需要这个 OutputBuffer，就释放它，可以关联一个 Surface，调用这句代码的同时把解码后的数据渲染到 Surface 上面，Surface 使用完毕后会释放该 Buffer
流程图如下： 
![MediaCodec 运行流程图](https://img-blog.csdn.net/20180728132407752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
MediaCodec 的使用方式有三种： 

1) 使用 Buffer 数组同步处理（deprecated），本文使用的是这种方式 

2) 使用 Buffers 同步处理 

3) 使用 Buffers 异步处理
因为第二、第三种方式要求 API 21 以上，因此这篇文章用的是第一种，其它两种方式有兴趣的可以自己研究：[MediaCodec | Android Developers](https://developer.android.google.cn/reference/android/media/MediaCodec.html#start%28%29)

另外，我在使用 MediaCodec 的时候发现了很多问题，有些是兼容性问题，有些我也不懂是什么问题，感觉很不稳定，因此我用得更多的是 FFmpeg，MediaCodec 没有深入研究，这里仅作为一个代码示例供大家参考。

# 解码

```java
public class HWCodec {

    private static final String TAG = "HWCodec";

    private static final long DEFAULT_TIMEOUT = 10 * 1000;

    private static final int MEDIA_TYPE_VIDEO = 1;
    private static final int MEDIA_TYPE_AUDIO = 2;
    private static final int MEDIA_TYPE_UNKNOWN = 0;

    private static final String MIME_TYPE_AVC = "video/avc";
    private static final String MIME_TYPE_AAC = "audio/mp4a-latm";

    public static boolean decode(String srcFilePath, String yuvDst, String pcmDst) {
        boolean vsucceed = decodeVideo(srcFilePath, yuvDst);
        boolean asucceed = decodeAudio(srcFilePath, pcmDst);
        return vsucceed && asucceed;
    }

    public static boolean decodeVideo(String srcFilePath, String dstFilePath) {
        return doDecode(srcFilePath, dstFilePath, MEDIA_TYPE_VIDEO);
    }

    public static boolean decodeAudio(String srcFilePath, String dstFilePath) {
        return doDecode(srcFilePath, dstFilePath, MEDIA_TYPE_AUDIO);
    }

    private static boolean doDecode(String src, String dst, int mediaType) {
        MediaExtractor extractor = null;
        MediaCodec decoder = null;
        FileOutputStream fos = null;
        boolean decodeSucceed = false;
        boolean exceptionOccur = false;
        try {
            extractor = new MediaExtractor();
            extractor.setDataSource(src);
            fos = new FileOutputStream(dst);
            decoder = doDecode(extractor, fos, mediaType);
        } catch (IOException e) {
            Log.e(TAG, "doDecode exception: " + e);
            exceptionOccur = true;
        } finally {
            if (extractor != null) {
                extractor.release();
            }
            if (decoder != null) {
                decodeSucceed = true;
                decoder.stop();
                decoder.release();
            }
            if (fos != null) {
                try {
                    fos.flush();
                    fos.close();
                } catch (IOException e) {
                    Log.e(TAG, "doDecode close fos error: " + e.getLocalizedMessage());
                }
            }
        }
        return !exceptionOccur && decodeSucceed;
    }

    private static MediaCodec doDecode(MediaExtractor extractor, FileOutputStream fos,
                                       int mediaType) throws IOException {
        MediaFormat format = selectTrack(extractor, mediaType);
        if (format == null) {
            Log.e(TAG, "doDecode no " + mediaType + " track");
            return null;
        }
        Log.d(TAG, "docde format: " + format.toString());

        MediaCodec decoder = MediaCodec.createDecoderByType(format.getString(MediaFormat.KEY_MIME));
        decoder.configure(format, null, null, 0);
        decoder.start();

        ByteBuffer[] inputBuffers = decoder.getInputBuffers();
        ByteBuffer[] outputBuffers = decoder.getOutputBuffers();
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();

        boolean inputEof = false;
        boolean outputEof = false;
        while (!outputEof) {
            if (!inputEof) {
                int inIndex = decoder.dequeueInputBuffer(DEFAULT_TIMEOUT);
                if (inIndex >= 0) {
                    ByteBuffer inputBuffer = inputBuffers[inIndex];
                    inputBuffer.clear();
                    int size = extractor.readSampleData(inputBuffer, 0);
                    if (size < 0) {
                        inputEof = true;
                        decoder.queueInputBuffer(inIndex, 0, 0,
                                0, MediaCodec.BUFFER_FLAG_END_OF_STREAM);
                    } else {
                        decoder.queueInputBuffer(inIndex, 0, size, extractor.getSampleTime(), 0);
                        extractor.advance();
                    }
                }
            }

            int outIndex = decoder.dequeueOutputBuffer(bufferInfo, DEFAULT_TIMEOUT);
            if (outIndex >= 0) {
                if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                    decoder.releaseOutputBuffer(outIndex, false);
                    continue;
                }

                if (bufferInfo.size != 0) {
                    ByteBuffer outputBuffer = outputBuffers[outIndex];
                    outputBuffer.position(bufferInfo.offset);
                    outputBuffer.limit(bufferInfo.offset + bufferInfo.size);
                    byte[] data = new byte[bufferInfo.size];
                    outputBuffer.get(data);
                    fos.write(data);
                }

                decoder.releaseOutputBuffer(outIndex, false);

                if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_END_OF_STREAM) != 0) {
                    outputEof = true;
                }
            } else if (outIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                outputBuffers = decoder.getOutputBuffers();
                Log.d(TAG, "decoder output buffer have changed");
            } else if (outIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                MediaFormat tmp = decoder.getOutputFormat();
                Log.d(TAG, "decoder output format change to " + tmp.toString());
            }

        }

        return decoder;
    }

    private static MediaFormat selectTrack(MediaExtractor extractor, int mediaType) {
        MediaFormat format = null;
        int trackCount = extractor.getTrackCount();
        for (int i = 0; i < trackCount; ++i) {
            MediaFormat tmpFormat = extractor.getTrackFormat(i);
            if (getMediaType(tmpFormat) == mediaType) {
                format = tmpFormat;
                extractor.selectTrack(i);
                break;
            }
        }
        return format;
    }

    private static int getMediaType(MediaFormat format) {
        String mime = format.getString(MediaFormat.KEY_MIME);
        if (mime.startsWith("video/")) {
            return MEDIA_TYPE_VIDEO;
        } else if (mime.startsWith("audio/")) {
            return MEDIA_TYPE_AUDIO;
        }
        return MEDIA_TYPE_UNKNOWN;
    }
}
```

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)






