# android AMR-NB - 三少GG - CSDN博客
2012年06月07日 12:00:42[三少GG](https://me.csdn.net/scut1135)阅读数：3764
**常规MediaRecorder使用**
1.[http://blog.sina.com.cn/s/blog_51396f890102e0jg.html](http://blog.sina.com.cn/s/blog_51396f890102e0jg.html)
2.[http://www.cnblogs.com/skyseraph/archive/2012/03/31/2427593.html](http://www.cnblogs.com/skyseraph/archive/2012/03/31/2427593.html)
### MediaRecorder相关
参考：[【Android學習專題】多媒體篇：MediaRecoder 实现录像机和](http://www.cnblogs.com/skyseraph/archive/2012/03/09/2387417.html)[【Android學習專題】多媒體篇：MediaRecorder 实现录音机](http://www.cnblogs.com/skyseraph/archive/2012/03/08/2384359.html)
另附initiativeVideo核心模块
```
[](http://www.cnblogs.com/skyseraph/archive/2012/03/31/2427593.html)
private boolean initializeVideo()
    {
        try
        {
            Log.i(TAG,"##initializeVideo....");
            // 〇state: Initial 实例化MediaRecorder对象
            if (mSurfaceView == null)
            {
                Log.e(TAG,"mSurfaceView is null in initializeVideo");
                return false;
            }
            if(mMediaRecorder == null)
                mMediaRecorder = new MediaRecorder();
            else
                mMediaRecorder.reset();                
                    
            // 〇state: Initial=>Initialized
            // set audio source as Microphone, video source as camera
            // specified before settings Recording-parameters or encoders，called only before setOutputFormat
            mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA); 
            //mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);
            
            // 〇state: Initialized=>DataSourceConfigured
            // 设置錄製視頻输出格式
            //     THREE_GPP:    3gp格式，H263视频ARM音频编码
            //    MPEG-4:        MPEG4 media file format
            //    RAW_AMR:    只支持音频且音频编码要求为AMR_NB
            //    AMR_NB:        
            //    ARM_MB:        
            //    Default:    
            // 3gp or mp4
            //Android支持的音频编解码仅为AMR_NB；支持的视频编解码仅为H263，H264只支持解码；支持对JPEG编解码；输出格式仅支持.3gp和.mp4
            String lVideoFileFullPath;    
            lVideoFileFullPath = ".3gp"; //.mp4
            mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);    
            //mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);
            Log.i(TAG, "Video：Current container format: "+"3GP\n");
            
            // 设置視頻/音频文件的编码：AAC/AMR_NB/AMR_MB/Default
            //    video: H.263, MP4-SP, or H.264    
            mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
            //mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H263);
            //mMediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.MPEG_4_SP);
            Log.i(TAG, "Video：Current encoding format: "+"H264\n");
                        
            // audio: AMR-NB
            //mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
            
            // 设置视频录制的分辨率。必须放在设置编码和格式的后面，否则报错
            //mMediaRecorder.setVideoSize(176, 144);
            mMediaRecorder.setVideoSize(320, 240);
            //mMediaRecorder.setVideoSize(720, 480);    
            Log.i(TAG, "Video：Current Video Size: "+"320*240\n");
                        
            // 设置录制的视频帧率。必须放在设置编码和格式的后面，否则报错
            mMediaRecorder.setVideoFrameRate(FRAME_RATE);    
            
            // 预览
            mMediaRecorder.setPreviewDisplay(mSurfaceHolder.getSurface());    
            
            // 设置输出文件方式： 直接本地存储   or LocalSocket远程输出
            if(bIfNativeORRemote)    //Native
            {
                lVideoFileFullPath = strRecVideoFilePath + String.valueOf(System.currentTimeMillis()) + lVideoFileFullPath;
                mRecVideoFile = new File(lVideoFileFullPath);
                // mMediaRecorder.setOutputFile(mRecVideoFile.getAbsolutePath());
                mMediaRecorder.setOutputFile(mRecVideoFile.getPath());    //called after set**Source before prepare
                Log.i(TAG, "start write into file~");
            }
            else    //Remote
            {
                mMediaRecorder.setOutputFile(sender.getFileDescriptor()); //设置以流方式输出 
                Log.i(TAG, "start send into sender~");
            }
            
            // 
            mMediaRecorder.setMaxDuration(0);//called after setOutputFile before prepare,if zero or negation,disables the limit
            mMediaRecorder.setMaxFileSize(0);//called after setOutputFile before prepare,if zero or negation,disables the limit
            try
            {     
                mMediaRecorder.setOnInfoListener(this);
                mMediaRecorder.setOnErrorListener(this);
                // 〇state: DataSourceConfigured => prepared
                mMediaRecorder.prepare();
                // 〇state: prepared => recording
                mMediaRecorder.start();
                bIfRecInProcess = true;
                Log.i(TAG, "initializeVideo Start!");
            } catch (Exception e)
            {        
                releaseMediaRecorder();
                finish();
                e.printStackTrace();
            }
            return true;
        } catch (Exception e)
        {
            // TODO: handle exception
            e.printStackTrace();
            return false;
        }
    }
```
[ANDROID如何实时获取录音AMR流](http://www.eoeandroid.com/thread-101541-1-1.html)
如题，想要实时获取录音的AMR流，有两种思路:
一种是用AudioRecord获取PCM数据然后编码生成AMR流
还有就是用MediaRecord获取音频流但不保存至文件而是映射到SOCKET这样就能实时获取
针对第一钟，上网找没找着已经封装好了的JNI库
第二种不知道如何去实现
哪位大虾有做过这方面的能发个DEMO给在下么，不甚感激
[http://hi-android.info/src/android/media/AmrInputStream.java.html](http://hi-android.info/src/android/media/AmrInputStream.java.html)
|AmrInputStream.java||
/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */package android.media;
import[android.util.Config](http://hi-android.info/src/android/util/Config.java.html);
import[android.util.Log](http://hi-android.info/src/android/util/Log.java.html);
import java.io.InputStream;
import java.io.IOException;
/**
 * AmrInputStream
 * @hide
 */publicfinalclassAmrInputStream extends InputStream
{    
static {
        System.loadLibrary("media_jni");
    }
privatefinalstatic String TAG = "AmrInputStream";
// frame is 20 msec at 8.000 khz
privatefinalstaticint SAMPLES_PER_FRAME = 8000 * 20 / 1000;
// pcm input stream
private InputStream mInputStream;
// native handle
privateint mGae;
// result amr stream
privatebyte[] mBuf = newbyte[SAMPLES_PER_FRAME * 2];
privateint mBufIn = 0;
privateint mBufOut = 0;
// helper for bytewise read()
privatebyte[] mOneByte = newbyte[1];
/**
     * Create a new AmrInputStream, which converts 16 bit PCM to AMR     * @param inputStream InputStream containing 16 bit PCM.
     */publicAmrInputStream(InputStream inputStream) {
        mInputStream = inputStream;
        mGae = GsmAmrEncoderNew();
        GsmAmrEncoderInitialize(mGae);
    }
    @Override
publicint read() throws IOException {
int rtn = read(mOneByte, 0, 1);
return rtn == 1 ? (0xff & mOneByte[0]) : -1;
    }
    @Override
publicint read(byte[] b) throws IOException {
return read(b, 0, b.length);
    }
    @Override
publicint read(byte[] b, int offset, int length) throws IOException {
if (mGae == 0) thrownew IllegalStateException("not open");
// local buffer of amr encoded audio empty
if (mBufOut >= mBufIn) {
// reset the buffer
            mBufOut = 0;
            mBufIn = 0;
// fetch a 20 msec frame of pcm
for (int i = 0; i < SAMPLES_PER_FRAME * 2; ) {
int n = mInputStream.read(mBuf, i, SAMPLES_PER_FRAME * 2 - i);
if (n == -1) return -1;
                i += n;
            }
// encode it
            mBufIn = GsmAmrEncoderEncode(mGae, mBuf, 0, mBuf, 0);
        }
// return encoded audio to user
if (length > mBufIn - mBufOut) length = mBufIn - mBufOut;
        System.arraycopy(mBuf, mBufOut, b, offset, length);
        mBufOut += length;
return length;
    }
    @Override
publicvoid close() throws IOException {
try {
if (mInputStream != null) mInputStream.close();
        } finally {
            mInputStream = null;
try {
if (mGae != 0) GsmAmrEncoderCleanup(mGae);
            } finally {
try {
if (mGae != 0) GsmAmrEncoderDelete(mGae);
                } finally {
                    mGae = 0;
                }
            }
        }
    }
    @Override
protectedvoid finalize() throws Throwable {
if (mGae != 0) {
            close();
thrownew IllegalStateException("someone forgot to close AmrInputStream");
        }
    }
//
// AudioRecord JNI interface
//
privatestaticnativeint GsmAmrEncoderNew();
privatestaticnativevoid GsmAmrEncoderInitialize(int gae);
privatestaticnativeint GsmAmrEncoderEncode(int gae,
byte[] pcm, int pcmOffset, byte[] amr, intamrOffset) throws IOException;
privatestaticnativevoid GsmAmrEncoderCleanup(int gae);
privatestaticnativevoid GsmAmrEncoderDelete(int gae);
}
