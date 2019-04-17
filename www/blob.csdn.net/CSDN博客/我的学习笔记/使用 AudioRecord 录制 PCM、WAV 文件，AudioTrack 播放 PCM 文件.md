# 使用 AudioRecord 录制 PCM、WAV 文件，AudioTrack 播放 PCM 文件 - 我的学习笔记 - CSDN博客





2018年07月15日 15:22:46[zouzhiheng](https://me.csdn.net/u011330638)阅读数：245








# 概述

AudioRecord、AudioTrack 的使用都比较简单，基本流程：

AudioRecord： 

1) AudioRecord.getMinBufferSize() 

2) new AudioRecord() 

3) mAudioRecord.read()
如此，即可得到录制好的音频裸数据（PCM）。

AudioTrack 有两种播放方式： 

1) static：把音频数据一次性写入 AudioTrack，再进行播放 

2) stream：边写边播
播放流程基本都是： 

1) AudioTrack.getMinBufferSize() 

2) new AudioTrack() 

3) mAudioTrack.write() // static 方式必须先 write，再 play 

4) mAudioTrack.play()
其中 getMinBufferSize()、new AudioRecord()、new AudioTrack() 需要传入 sampleRate、channel、sampleFormat 等参数，这些参数涉及到音频开发的一些基础知识，可参考我的另一篇博客：[音视频开发基础概述 - PCM、YUV、H264、常用软件介绍](https://blog.csdn.net/u011330638/article/details/81107312)

# AudioRecord 录制 PCM 数据

```java
public class AudioRecorder {

    private static final String TAG = "AudioRecorder";

    private static final int DEFAULT_SAMPLE_RATE = 48000;
    private static final int DEFAULT_PCM_DATA_FORMAT = AudioFormat.ENCODING_PCM_16BIT;

    private ExecutorService mExecutor = Executors.newCachedThreadPool();
    private AudioRecord mAudioRecord;
    private int mBufferSize;
    private int mSampleRate = DEFAULT_SAMPLE_RATE;
    private int mPcmFormat = DEFAULT_PCM_DATA_FORMAT;

    private AudioRecordCallback mRecordCallback;
    private Handler mHandler;
    private boolean mIsRecording = false;

    public void setSampleRate(int sampleRate) {
        mSampleRate = sampleRate;
    }

    public void setPcmFormat(int pcmFormat) {
        mPcmFormat = pcmFormat;
    }

    public void setRecordCallback(AudioRecordCallback recordCallback) {
        mRecordCallback = recordCallback;
    }

    public int getChannels() {
        return 1;
    }

    public boolean start() {
        try {
            mBufferSize = AudioRecord.getMinBufferSize(mSampleRate, AudioFormat.CHANNEL_IN_MONO,
                    mPcmFormat);
            mAudioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, mSampleRate,
                    AudioFormat.CHANNEL_IN_MONO, mPcmFormat, mBufferSize);
        } catch (Exception e) {
            Log.e(TAG, "init AudioRecord exception: " + e.getLocalizedMessage());
            return false;
        }

        if (mAudioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
            Log.e(TAG, "cannot init AudioRecord");
            return false;
        }
        mIsRecording = true;
        mExecutor.execute(this::record);
        mHandler = new Handler(Looper.myLooper());

        return true;
    }

    private void record() {
        android.os.Process.setThreadPriority(android.os.Process.THREAD_PRIORITY_URGENT_AUDIO);
        if (mAudioRecord == null || mAudioRecord.getState() != AudioRecord.STATE_INITIALIZED) {
            return;
        }

        ByteBuffer audioBuffer = ByteBuffer.allocate(mBufferSize);
        mAudioRecord.startRecording();
        Log.d(TAG, "AudioRecorder started");

        int readResult;
        while (mIsRecording) {
            readResult = mAudioRecord.read(audioBuffer.array(), 0, mBufferSize);
            if (readResult > 0 && mRecordCallback != null) {
                byte[] data = new byte[readResult];
                audioBuffer.position(0);
                audioBuffer.limit(readResult);
                audioBuffer.get(data, 0, readResult);
                mHandler.post(() -> mRecordCallback.onRecordSample(data));
            }
        }

        release();
        Log.d(TAG, "AudioRecorder finished");
    }

    public void stop() {
        mIsRecording = false;
    }

    private void release() {
        if (mAudioRecord != null) {
            mAudioRecord.stop();
            mAudioRecord.release();
            mAudioRecord = null;
        }
    }

    public interface AudioRecordCallback {
        // start 在哪个线程调用，就运行在哪个线程
        void onRecordSample(byte[] data);
    }

}
```

# PCM 转 WAV 文件

wav 是一种音频编码算法，它不会压缩音频，其中一种实现是在 PCM 数据前面加上 44 字节，分别描述 PCM 的采样率、声道数、数据格式等信息。

根据 wiki 的描述，wav 文件头如下：

![WAV 文件头](https://img-blog.csdn.net/20180728123820475?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTEzMzA2Mzg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码实现：

```java
private void pcmToWav(File pcmFile, File wavFile) {
        FileInputStream fis = null;
        FileOutputStream fos = null;
        try {
            fis = new FileInputStream(pcmFile);
            fos = new FileOutputStream(wavFile);

            int sampleFormat = mPcmDataFormat == AudioFormat.ENCODING_PCM_16BIT ? 16 : 8;
            writeWavHeader(fos, fis.getChannel().size(), sampleFormat, mSampleRate, mChannels);

            int channelConfig =mChannels == 1 ? AudioFormat.CHANNEL_IN_MONO : AudioFormat.CHANNEL_IN_STEREO;
            int bufferSize = AudioRecord.getMinBufferSize(mSampleRate, channelConfig, mPcmDataFormat);

            byte[] data = new byte[bufferSize];
            while (fis.read(data) != -1) {
                fos.write(data);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                if (fis != null) {
                    fis.close();
                }

                if (fos != null) {
                    fos.flush();
                    fos.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void writeWavHeader(@NonNull FileOutputStream fos, long pcmDataLength, int sampleFormat,
                                int sampleRate, int channels) throws IOException {
        long audioDataLength = pcmDataLength + 36;
        long bitRate = sampleRate * channels * sampleFormat / 8;
        byte[] header = new byte[44];
        // RIFF
        header[0] = 'R';
        header[1] = 'I';
        header[2] = 'F';
        header[3] = 'F';
        // pcm data length
        header[4] = (byte) (pcmDataLength & 0xff);
        header[5] = (byte) ((pcmDataLength >> 8) & 0xff);
        header[6] = (byte) ((pcmDataLength >> 16) & 0xff);
        header[7] = (byte) ((pcmDataLength >> 24) & 0xff);
        // WAVE
        header[8] = 'W';
        header[9] = 'A';
        header[10] = 'V';
        header[11] = 'E';
        // 'fmt '
        header[12] = 'f';
        header[13] = 'm';
        header[14] = 't';
        header[15] = ' ';
        header[16] = 16;
        header[17] = 0;
        header[18] = 0;
        header[19] = 0;
        // 1(PCM)
        header[20] = 1;
        header[21] = 0;
        // channels
        header[22] = (byte) channels;
        header[23] = 0;
        // sample rate
        header[24] = (byte) (sampleRate & 0xff);
        header[25] = (byte) ((sampleRate >> 8) & 0xff);
        header[26] = (byte) ((sampleRate >> 16) & 0xff);
        header[27] = (byte) ((sampleRate >> 24) & 0xff);
        // bit rate
        header[28] = (byte) (bitRate & 0xff);
        header[29] = (byte) ((bitRate >> 8) & 0xff);
        header[30] = (byte) ((bitRate >> 16) & 0xff);
        header[31] = (byte) ((bitRate >> 24) & 0xff);
        header[32] = 4;
        header[33] = 0;
        // 采样精度
        header[34] = (byte) sampleFormat;
        header[35] = 0;
        // data
        header[36] = 'd';
        header[37] = 'a';
        header[38] = 't';
        header[39] = 'a';
        // data length
        header[40] = (byte) (audioDataLength & 0xff);
        header[41] = (byte) ((audioDataLength >> 8) & 0xff);
        header[42] = (byte) ((audioDataLength >> 16) & 0xff);
        header[43] = (byte) ((audioDataLength >> 24) & 0xff);
        fos.write(header);
    }
```

# AudioTrack 播放 PCM 数据

```
private class AudioPlayer {

        private AudioTrack mAudioTrack;
        private volatile boolean mIsPlaying = false;
        private int mBufferSize;
        private ExecutorService mExecutor = Executors.newSingleThreadExecutor();

        private void start() {
            if (!mPcmFile.exists()) {
                ToastHelper.show("无音频文件，请先进行录制");
                return;
            }
            if (mIsPlaying) {
                ToastHelper.show("当前正在播放中");
                return;
            }
            releaseAudioTrack();
            mBufferSize = AudioTrack.getMinBufferSize(mSampleRate, AudioFormat.CHANNEL_OUT_MONO,
                    mPcmDataFormat);
            mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, mSampleRate,
                    AudioFormat.CHANNEL_OUT_MONO, mPcmDataFormat, mBufferSize, AudioTrack.MODE_STREAM);
            mIsPlaying = true;
            mExecutor.execute(this::play);
        }

        private void play() {
            Log.d(TAG, "play started");
            try {
                byte[] buffer = new byte[mBufferSize];
                int readCount;
                DataInputStream dis = new DataInputStream(new FileInputStream(mPcmFile));
                while (dis.available() > 0 && mIsPlaying) {
                    readCount = dis.read(buffer);
                    if (readCount == AudioTrack.ERROR_INVALID_OPERATION || readCount == AudioTrack.ERROR_BAD_VALUE) {
                        continue;
                    }
                    if (readCount > 0) {
                        mAudioTrack.play();
                        mAudioTrack.write(buffer, 0, readCount);
                    }
                }
                mIsPlaying = false;
            } catch (IOException e) {
                mIsPlaying = false;
                Log.e(TAG, "read audio data fail: " + e.getMessage());
            }
            Log.d(TAG, "play stopped");
        }

        private void stop() {
            mIsPlaying = false;
            try {
                mExecutor.shutdown();
                mExecutor.awaitTermination(500, TimeUnit.MILLISECONDS);
                releaseAudioTrack();
            } catch (InterruptedException e) {
                Log.e(TAG, "stop play faild");
            }
        }

        private void releaseAudioTrack() {
            if (mAudioTrack != null) {
                mAudioTrack.stop();
                mAudioTrack.release();
                mAudioTrack = null;
            }
        }

    }
```

源码已上传到 [GitHub](https://github.com/zouzhiheng/AVGraphics)






