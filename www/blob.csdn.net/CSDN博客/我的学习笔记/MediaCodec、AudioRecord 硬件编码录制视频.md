# MediaCodec、AudioRecord 硬件编码录制视频 - 我的学习笔记 - CSDN博客





2018年07月15日 15:45:24[zouzhiheng](https://me.csdn.net/u011330638)阅读数：146








关于 MediaCodec 和 AudioRecord 的使用方法介绍，可以看其它博客： 
[MediaCodec 实现硬件解码](https://blog.csdn.net/u011330638/article/details/81053472)， 
[使用 AudioRecord 录制 PCM、WAV 文件，AudioTrack 播放 PCM 文件](https://blog.csdn.net/u011330638/article/details/81053355)
基本步骤为： 

1) Camera、AudioRecord 获取原始图像、声音数据 

2) MediaCodec 编码 

3) MediaMuxer 合成 MP4 文件
# 获取原始图像、声音数据

获取图像数据使用 Camera 实现，首先打开 Camera：

```
mCamera = CameraHelper.openCamera();
    if (mCamera == null) {
       finish();
    }

    Camera.Parameters parameters = mCamera.getParameters();
    mPreviewSize = CameraHelper.chooseCameraSize(parameters.getSupportedPreviewSizes(), width, height);
    parameters.setPreviewSize(mPreviewSize.width, mPreviewSize.height);
    mPrevieweFormat = parameters.getPreviewFormat();
    mCamera.setParameters(parameters);
    mCamera.setDisplayOrientation(90);
    mCamera.setPreviewCallback(this);
    try {
       mCamera.setPreviewDisplay(holder);
       mCamera.startPreview();
    } catch (IOException e) {
       Log.e(TAG, "openCamera preview failed: " + e.getLocalizedMessage());
       ToastHelper.show("相机预览开启失败！");
       releaseCamera();
       finish();
    }
```

onPreviewFrame 就是获取原始图像数据的地方：

```java
@Override
    public void onPreviewFrame(byte[] data, Camera camera) {
        if (mIsRecording) {
            mRecorder.recordImage(data);
        }
    }
```

AudioRecord 录制音频：

```java
private class AudioRecordRunnable implements Runnable {

        private AudioRecord mAudioRecord;
        private int mBufferSize;

        private AudioRecordRunnable() {
            try {
                mChannels = 1;
                mBufferSize = AudioRecord.getMinBufferSize(mSampleRate, AudioFormat.CHANNEL_IN_MONO,
                        AudioFormat.ENCODING_PCM_16BIT);
                mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRate,
                        AudioFormat.CHANNEL_IN_MONO, AudioFormat.ENCODING_PCM_16BIT, mBufferSize);
            } catch (Exception e) {
                Log.e(TAG, "init AudioRecord exception: " + e.getLocalizedMessage());
            }

            if (mAudioRecord == null || mAudioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
                try {
                    mSampleRate = 16000;
                    mChannels = 2;
                    mBufferSize = AudioRecord.getMinBufferSize(mSampleRate, AudioFormat.CHANNEL_IN_STEREO,
                            AudioFormat.ENCODING_PCM_16BIT);
                    mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRate,
                            AudioFormat.CHANNEL_IN_STEREO, AudioFormat.ENCODING_PCM_16BIT, mBufferSize);
                } catch (Exception e) {
                    Log.e(TAG, "init AudioRecord exception: " + e.getLocalizedMessage());
                }
            }

            if (mAudioRecord == null || mAudioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
                Log.e(TAG, "cannot init AudioRecord");
            }
        }

        @Override
        public void run() {
            android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
            if (mAudioRecord == null || mAudioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
                return;
            }

            byte[] audioData = new byte[mBufferSize];
            mAudioRecord.startRecording();
            Log.d(TAG, "AudioRecord started");

            int actualSize;
            while (mIsRecording) {
                actualSize = mAudioRecord.read(audioData, 0, audioData.length);
                synchronized (mLockObject) {
                    if (actualSize > 0 && mIsRecording) {
                        mRecorder.recordSample(audioData);
                    }
                }
            }

            if (mAudioRecord != null) {
                mAudioRecord.stop();
                mAudioRecord.release();
                mAudioRecord = null;
                Log.d(TAG, "AudioRecord released");
            }
        }
    }
```

# 音视频数据编码并合成为 MP4 文件

首先初始化音视频编码器和 MediaMuxer：

```java
public static class Recorder {
        private static final int DEFAULT_FRAME_RATE = 30;
        private static final int DEFAULT_IFRAME_INTERVAL = 5;
        private static final int DEFAULT_BITRATE_AUDIO = 128 * 1000;

        private MediaMuxer mMuxer;
        private MediaCodec mVideoEncoder;
        private MediaCodec mAudioEncoder;
        private MediaCodec.BufferInfo mVBufferInfo;
        private MediaCodec.BufferInfo mABufferInfo;

        private boolean mIsInitialized = false;
        private long mVStartTime;
        private long mAStartTime;
        private int mVTrackIndex;
        private int mATrackIndex;
        private volatile boolean mMuxerStarted;

        public void init(int width, int height, int colorFormat, int bitRate, int sampleRate,
                         int channels, String dstFilePath) throws Exception {

            if (getCodecInfo(MIME_TYPE_AVC) == null || getCodecInfo(MIME_TYPE_AAC) == null) {
                throw new Exception("cannot find suitable codec");
            }

            MediaFormat videoFormat = MediaFormat.createVideoFormat(MIME_TYPE_AVC, width, height);
            videoFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
            videoFormat.setInteger(MediaFormat.KEY_FRAME_RATE, DEFAULT_FRAME_RATE);
            videoFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, DEFAULT_IFRAME_INTERVAL);
            videoFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT, colorFormat);

            mVideoEncoder = MediaCodec.createEncoderByType(MIME_TYPE_AVC);
            mVideoEncoder.configure(videoFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mVideoEncoder.start();

            MediaFormat audioFormat = MediaFormat.createAudioFormat(MIME_TYPE_AAC, sampleRate, channels);
            audioFormat.setInteger(MediaFormat.KEY_AAC_PROFILE, MediaCodecInfo.CodecProfileLevel.AACObjectLC);
            audioFormat.setInteger(MediaFormat.KEY_BIT_RATE, DEFAULT_BITRATE_AUDIO);

            mAudioEncoder = MediaCodec.createEncoderByType(MIME_TYPE_AAC);
            mAudioEncoder.configure(audioFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
            mAudioEncoder.start();

            File file = new File(dstFilePath);
            if (file.exists() && !file.delete()) {
                Log.w(TAG, "delete file failed");
            }

            mMuxer = new MediaMuxer(dstFilePath, MediaMuxer.OutputFormat.MUXER_OUTPUT_MPEG_4);
            mMuxerStarted = false;
            mVTrackIndex = -1;
            mATrackIndex = -1;
            mVStartTime = -1;
            mAStartTime = -1;

            mVBufferInfo = new MediaCodec.BufferInfo();
            mABufferInfo = new MediaCodec.BufferInfo();
            mIsInitialized = true;
            Log.i(TAG, "Recorder initialized");
        }

        private static MediaCodecInfo getCodecInfo(final String mimeType) {
            final int numCodecs = MediaCodecList.getCodecCount();
            for (int i = 0; i < numCodecs; i++) {
                final MediaCodecInfo codecInfo = MediaCodecList.getCodecInfoAt(i);
                if (!codecInfo.isEncoder()) {
                    continue;
                }
                final String[] types = codecInfo.getSupportedTypes();
                for (String type : types) {
                    if (type.equalsIgnoreCase(mimeType)) {
                        return codecInfo;
                    }
                }
            }
            return null;
        }

        ...

    }
```

接着接收音视频原始数据并编码，编码后合成至 MP4 文件，注意需要把音频轨道、视频轨道都添加到 MediaMuxer 之后才可以进行合成操作：

```java
public void recordImage(byte[] image) throws Exception {
            long pts;
            if (mVStartTime == -1) {
                mVStartTime = System.nanoTime();
                pts = 0;
            } else {
                pts = (System.nanoTime() - mVStartTime) / 1000;
            }
            doRecord(mVideoEncoder, mVBufferInfo, image, pts);
        }

        public void recordSample(byte[] sample) throws Exception {
            long pts;
            if (mAStartTime == -1) {
                mAStartTime = System.nanoTime();
                pts = 0;
            } else {
                pts = (System.nanoTime() - mAStartTime) / 1000;
            }
            doRecord(mAudioEncoder, mABufferInfo, sample, pts);
        }

        private void doRecord(MediaCodec encoder, MediaCodec.BufferInfo bufferInfo, byte[] data,
                              long pts) throws Exception {
            if (!mIsInitialized) {
                Log.e(TAG, "Recorder must be initialized!");
                return;
            }
            int index = encoder.dequeueInputBuffer(DEFAULT_TIMEOUT);
            ByteBuffer[] inputBuffers = encoder.getInputBuffers();
            ByteBuffer buffer = inputBuffers[index];
            if (index >= 0) {
                buffer.clear();
                buffer.put(data);
                encoder.queueInputBuffer(index, 0, data.length, pts, 0);
            }
            drainEncoder(encoder, bufferInfo);
        }

        private void drainEncoder(MediaCodec encoder, MediaCodec.BufferInfo bufferInfo) throws Exception {
            int trackIndex = encoder == mVideoEncoder ? mVTrackIndex : mATrackIndex;
            ByteBuffer[] outputBuffers = encoder.getOutputBuffers();
            while (true) {
                int index = encoder.dequeueOutputBuffer(bufferInfo, DEFAULT_TIMEOUT);
                if (index == MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED) {
                    outputBuffers = encoder.getOutputBuffers();
                } else if (index == MediaCodec.INFO_OUTPUT_FORMAT_CHANGED) {
                    trackIndex = addTrackIndex(encoder);
                } else if (index == MediaCodec.INFO_TRY_AGAIN_LATER) {
                    break;
                } else if (index < 0) {
                    Log.w(TAG, "drainEncoder unexpected result: " + index);
                } else {
                    if ((bufferInfo.flags & MediaCodec.BUFFER_FLAG_CODEC_CONFIG) != 0) {
                        continue;
                    }

                    if (bufferInfo.size != 0) {
                        ByteBuffer outputBuffer = outputBuffers[index];

                        if (outputBuffer == null) {
                            throw new RuntimeException("drainEncoder get outputBuffer " + index + " was null");
                        }

                        synchronized (this) {
                            if (!mMuxerStarted) {
                                wait();
                            }
                        }

                        outputBuffer.position(bufferInfo.offset);
                        outputBuffer.limit(bufferInfo.offset + bufferInfo.size);
                        mMuxer.writeSampleData(trackIndex, outputBuffer, bufferInfo);
                    }

                    encoder.releaseOutputBuffer(index, false);
                }
            }
        }

        private int addTrackIndex(MediaCodec encoder) {
            int trackIndex;
            synchronized (this) {
                MediaFormat format = encoder.getOutputFormat();
                if (getMediaType(format) == MEDIA_TYPE_VIDEO) {
                    mVTrackIndex = mMuxer.addTrack(format);
                    trackIndex = mVTrackIndex;
                } else {
                    mATrackIndex = mMuxer.addTrack(format);
                    trackIndex = mATrackIndex;
                }

                if (mVTrackIndex != -1 && mATrackIndex != -1) {
                    mMuxer.start();
                    mMuxerStarted = true;
                    notifyAll();
                    Log.i(TAG, "MediaMuxer has added all track, notifyAll");
                }
            }
            return trackIndex;
        }
```

录制结束后释放资源：

```
public void release() {
            if (mVideoEncoder != null) {
                mVideoEncoder.stop();
                mVideoEncoder.release();
                mVideoEncoder = null;
            }

            if (mAudioEncoder != null) {
                mAudioEncoder.stop();
                mAudioEncoder.release();
                mAudioEncoder = null;
            }

            if (mMuxer != null) {
                mMuxer.stop();
                mMuxer.release();
                mMuxer = null;
            }

            if (mIsInitialized) {
                Log.i(TAG, "Recorder released");
            }
            mIsInitialized = false;
        }
```

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)





