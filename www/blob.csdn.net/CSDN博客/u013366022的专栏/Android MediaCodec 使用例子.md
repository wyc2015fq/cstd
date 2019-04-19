# Android MediaCodec 使用例子 - u013366022的专栏 - CSDN博客
2014年05月14日 15:20:04[slitaz](https://me.csdn.net/u013366022)阅读数：1082
下面的例子是使用MediaCodec 录制到文件的例子。
```
public class AvcEncoder {
private MediaCodec mediaCodec;
private BufferedOutputStream outputStream;
public AvcEncoder() { 
    File f = new File(Environment.getExternalStorageDirectory(), "Download/video_encoded.264");
    touch (f);
    try {
        outputStream = new BufferedOutputStream(new FileOutputStream(f));
        Log.i("AvcEncoder", "outputStream initialized");
    } catch (Exception e){ 
        e.printStackTrace();
    }
    mediaCodec = MediaCodec.createEncoderByType("video/avc");
    MediaFormat mediaFormat = MediaFormat.createVideoFormat("video/avc", 320, 240);
    mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, 125000);
    mediaFormat.setInteger(MediaFormat.KEY_FRAME_RATE, 15);
    mediaFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
    mediaFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 5);
    mediaCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
    mediaCodec.start();
}
public void close() {
    try {
        mediaCodec.stop();
        mediaCodec.release();
        outputStream.flush();
        outputStream.close();
    } catch (Exception e){ 
        e.printStackTrace();
    }
}
// called from Camera.setPreviewCallbackWithBuffer(...) in other class
public void offerEncoder(byte[] input) {
    try {
        ByteBuffer[] inputBuffers = mediaCodec.getInputBuffers();
        ByteBuffer[] outputBuffers = mediaCodec.getOutputBuffers();
        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = inputBuffers[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(input);
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, input.length, 0, 0);
        }
        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo,0);
        while (outputBufferIndex >= 0) {
            ByteBuffer outputBuffer = outputBuffers[outputBufferIndex];
            byte[] outData = new byte[bufferInfo.size];
            outputBuffer.get(outData);
            outputStream.write(outData, 0, outData.length);
            Log.i("AvcEncoder", outData.length + " bytes written");
            mediaCodec.releaseOutputBuffer(outputBufferIndex, false);
            outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        }
    } catch (Throwable t) {
        t.printStackTrace();
    }
}
```
这个例子配置了编码格式、视频大小、比特率、帧率等参数，然后在camera 预览的回调函数里把yuv420数据推送给mediacodec，编码后取出nalu，写入到文件。
