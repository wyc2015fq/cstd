# [转载]使用EasyRTMP的Android版采集AAC音频流进行RTMP推送 - xcyl的口袋 - CSDN博客
2016年10月29日 14:10:48[xcyl](https://me.csdn.net/cai6811376)阅读数：853
本文转自EasyDarwin团队kim的博客：[http://blog.csdn.net/jinlong0603/article/details/52963378](http://blog.csdn.net/jinlong0603/article/details/52963378)
EasyRTMP Android版demo中，通过使用AudioRecord录音，然后将每一帧数据通过MediaCodec编码成LATM格式音频，然后就发送给服务器就OK了。
AudioRecord的使用
```java
public void startRecord() {
        try {
            init();
            mAudioRecord.startRecording();
            mMediaCodec.start();
            final ByteBuffer[] inputBuffers = mMediaCodec.getInputBuffers();
            mThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    Process.setThreadPriority(Process.THREAD_PRIORITY_AUDIO);
                    int len = 0, bufferIndex = 0;
                    try {
                        while (!Thread.interrupted() && !stoped) {
                            bufferIndex = mMediaCodec.dequeueInputBuffer(50000);
                            if (bufferIndex >= 0) {
                                inputBuffers[bufferIndex].clear();
                                len = mAudioRecord.read(inputBuffers[bufferIndex], BUFFER_SIZE);
                                if (len == AudioRecord.ERROR_INVALID_OPERATION || len == AudioRecord.ERROR_BAD_VALUE) {
                                    mMediaCodec.queueInputBuffer(bufferIndex, 0, 0, 0, 0);
                                } else {
                                    mMediaCodec.queueInputBuffer(bufferIndex, 0, len, 0, 0);
                                }
                            }
                        }
                    } catch (RuntimeException e) {
                        Log.i(TAG, "record" + e.getMessage());
                    }
                }
            }, "AACRecoder");
            mThread.start();
            startEncode();
        } catch (Exception e) {
            Log.e(TAG, "Record___Error!!!!!");
        }
    }
```
在循环中通过mAudioRecord.read(inputBuffers[bufferIndex], BUFFER_SIZE)不断的读取音频，然后放入编码队列中去。 新开一个线程用来获取队列中的数据：
```
private void startEncode() {
        mBuffers = mMediaCodec.getOutputBuffers();
        mBuffer=null;
        encodeThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while (!Thread.interrupted() && !stoped) {
                    try {
                        if (mBuffer == null) {
                            mBuffer = ByteBuffer.allocate(10240);
                            while (!Thread.currentThread().isInterrupted() && !stoped) {
                                mIndex = mMediaCodec.dequeueOutputBuffer(mBufferInfo, 50000);
                                if (mIndex >= 0) {
                                    if (mBufferInfo.flags == MediaCodec.BUFFER_FLAG_CODEC_CONFIG) {
                                        continue;
                                    }
                                    mBuffer.clear();
                                    mBuffer.position(0);
                                    mBuffers[mIndex].get(mBuffer.array(), 0, mBufferInfo.size);
                                    mBuffers[mIndex].clear();
                                    mBuffer.position(mBuffer.position() + mBufferInfo.size);
                                    mBuffer.flip();
                                    break;
                                } else if (mIndex == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                                    mBuffers = mMediaCodec.getOutputBuffers();
                                } else if (mIndex == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                                    Log.v(TAG, "output format changed...");
                                } else if (mIndex == MediaCodec.INFO_TRY_AGAIN_LATER) {
                                    Log.v(TAG, "No buffer available...");
                                } else {
                                    Log.e(TAG, "Message: " + mIndex);
                                }
                            }
                        }
                        int size = mBufferInfo.size;
                        byte[] buffer = new byte[size];
                        mBuffer.get(buffer);
                        easyPusher.push(buffer,System.currentTimeMillis(), 0);
                        if (mBuffer.position() >= size) { // read complete
                            mMediaCodec.releaseOutputBuffer(mIndex, false);
                            mBuffer = null;
                        }
                    } catch (RuntimeException e) {
                                                Log.i(TAG, "record" + e.getMessage());
                    }
                }
            }
        }, "AACEncoder");
        encodeThread.start();
    }
```
EasyRTMP推送RTMP流时不需要添加ADTS头部。而EasyPusher推送RTSP流需要添加ADTS。
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
