# Android 音频 OpenSL ES 录音 采集 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年06月09日 13:28:48[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：645

1，创建引擎
2，创建AudioRecorder并开始录音
3，暂停录音
4，释放资源
5，数据是通过回调函数处理的。
好处：缓冲区不用通过AudioRecord.getMinBufferSize获取，设置很小也能正常工作。比如设置80字节bytes
前面的文章我们讲解的是OpenSL ES音频播放，OpenSL ES非常强大，有音频播放当然有录音
下面我们编写OpenSL PCM录音，完成的功能是录制麦克风的声音存储PCM到data私有目录。
二、布局XML 创建文件 /res/layout/open_sl_audio_record.xml
[html][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- **<?****xml**version="1.0"encoding="utf-8"**?>**
- **<****LinearLayout**xmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:orientation="vertical"**>**
- 
- 
- **<****Button**
- android:id="@+id/start"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:text="开始"**/>**
- 
- 
- **<****LinearLayout**
- android:layout_width="match_parent"
- android:layout_height="wrap_content"
- android:orientation="horizontal"**>**
- 
- **<****Button**
- android:id="@+id/stop"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:text="暂停"**/>**
- 
- **</****LinearLayout****>**
- 
- **</****LinearLayout****>**
布局文件：开始和暂停两个Button控制录音
三、Activity类  创建/src/com/example/testopensl/AudioRecordActivity.java
[java][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- **package** com.example.testopensl;  
- 
- **import** java.io.File;  
- 
- **import** android.app.Activity;  
- **import** android.os.Bundle;  
- **import** android.view.View;  
- **import** android.view.View.OnClickListener;  
- 
- **import** com.example.audio.R;  
- 
- **public****class** AudioRecordActivity **extends** Activity **implements** OnClickListener {  
- 
- **static** {  
-         System.loadLibrary("TestAudio");  
-     }  
- **private****boolean**mRecording;  
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {  
- **super**.onCreate(savedInstanceState);  
-         setContentView(R.layout.open_sl_audio_record);  
-         findViewById(R.id.start).setOnClickListener(**this**);  
-         findViewById(R.id.stop).setOnClickListener(**this**);  
- createEngine();  
-     }  
- 
- @Override
- **public****void** onClick(View v) {  
- **switch** (v.getId()) {  
- **case** R.id.start:  
-             File file = **new** File(getCacheDir(), "audio.pcm");  
- createAudioRecord(file.getAbsolutePath());  
- mRecording = **true**;  
- **break**;  
- **case** R.id.stop:  
- stop();  
- mRecording = **false**;  
- **break**;  
-         }  
-     }  
- 
- @Override
- **protected****void** onDestroy() {  
- **super**.onDestroy();  
- **if** (mRecording) {  
-             stop();  
-         }  
-         shutdown();  
-     }  
- 
- /** Native methods, implemented in jni folder */
- **public****native****void** createEngine();  
- 
- **public****native****void**createAudioRecord(String uri);  
- 
- **public****native****void** stop();  
- 
- **public****native****void**shutdown();  
- 
- }  
文件存储位置：/data/data/com.example.audio/cache/audio.pcm
四、编写日志头文件，用于日志输出， 创建/jni/log.h 文件
[cpp][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- #ifndef LOG_H_
- #define LOG_H_
- 
- #include <android/log.h>
- 
- #ifndef DGB
- #define DGB 0
- #endif
- 
- #ifndef LOG_TAG
- #define LOG_TAG __FILE__
- #endif
- 
- #ifndef ALOGD
- #if DGB
- #define ALOGD(...) \
-         __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)  
- #else
- #define ALOGD(...)   ((void)0)
- #endif
- #endif
- 
- #endif /* LOG_H_ */
六、JNI的实现 /jni/com_example_testopensl_AudioRecordActivity.cpp
[cpp][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- /** log */
- #define LOG_TAG "NATVIE-AudioRecordActivity"
- #define DGB 1
- 
- 
- #include <jni.h>
- #include "com_example_testopensl_AudioRecordActivity.h"
- 
- #include "log.h"
- #include <stdio.h>
- #include <assert.h>
- 
- #include <SLES/OpenSLES.h>
- #include <SLES/OpenSLES_Android.h>
- 
- /* Size of the recording buffer queue */
- #define NB_BUFFERS_IN_QUEUE 1
- 
- /* Explicitly requesting SL_IID_ANDROIDSIMPLEBUFFERQUEUE and SL_IID_ANDROIDCONFIGURATION
-  * on the AudioRecorder object */
- #define NUM_EXPLICIT_INTERFACES_FOR_RECORDER 2
- 
- /* Size of the recording buffer queue */
- #define NB_BUFFERS_IN_QUEUE 1
- /* Size of each buffer in the queue */
- #define BUFFER_SIZE_IN_SAMPLES 8192
- #define BUFFER_SIZE_IN_BYTES   (2 * BUFFER_SIZE_IN_SAMPLES)
- 
- /* Local storage for Audio data */
- int8_t pcmData[NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES];  
- 
- 
- // engine interfaces
- **static** SLObjectItf engineObject = NULL;  
- **static** SLEngineItf engineEngine = NULL;  
- 
- //audio record interfaces
- **static** SLObjectItf recorderObject = NULL;  
- **static** SLRecordItf recordItf = NULL;  
- **static** SLAndroidSimpleBufferQueueItf recBuffQueueItf = NULL;  
- **static** SLAndroidConfigurationItf configItf = NULL;  
- 
- **static****FILE** * gFile = NULL;  
- 
- //-----------------------------------------------------------------
- /* Structure for passing information to callback function */
- **typedef****struct** CallbackCntxt_ {  
-     SLPlayItf  playItf;  
-     SLuint32   size;  
-     SLint8*   pDataBase;    // Base address of local audio data storage
-     SLint8*   pData;        // Current address of local audio data storage
- } CallbackCntxt;  
- 
- **static** CallbackCntxt cntxt;  
- 
- /* Callback for recording buffer queue events */
- **void** recCallback(SLRecordItf caller, **void** *pContext, SLuint32 event) {  
- **if** (SL_RECORDEVENT_HEADATNEWPOS & event) {  
-         SLmillisecond pMsec = 0;  
-         (*caller)->GetPosition(caller, &pMsec);  
-         ALOGD("SL_RECORDEVENT_HEADATNEWPOS current position=%ums\n", pMsec);  
-     }  
- 
- **if** (SL_RECORDEVENT_HEADATMARKER & event) {  
-         SLmillisecond pMsec = 0;  
-         (*caller)->GetPosition(caller, &pMsec);  
-         ALOGD("SL_RECORDEVENT_HEADATMARKER current position=%ums\n", pMsec);  
-     }  
- }  
- 
- /* Callback for recording buffer queue events */
- **void** recBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, **void** *pContext) {  
- 
-     CallbackCntxt *pCntxt = (CallbackCntxt*) pContext;  
- 
- /* Save the recorded data  */
-     fwrite(pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES, 1, gFile);  
- 
- /* Increase data pointer by buffer size */
-     pCntxt->pData += BUFFER_SIZE_IN_BYTES;  
- 
- **if** (pCntxt->pData >= pCntxt->pDataBase + (NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES)) {  
-         pCntxt->pData = pCntxt->pDataBase;  
-     }  
- 
-     (*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES);  
- 
-     SLAndroidSimpleBufferQueueState recQueueState;  
-     (*queueItf)->GetState(queueItf, &recQueueState);  
- 
- }  
- 
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    createEngine
-  * Signature: ()V
-  */
- JNIEXPORT **void** JNICALL Java_com_example_testopensl_AudioRecordActivity_createEngine(JNIEnv *, jobject) {  
-     SLEngineOption EngineOption[] = {  
-                 {(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE}  
-         };  
-     SLresult result;  
-     result = slCreateEngine(&engineObject, 1, EngineOption, 0, NULL, NULL);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Realizing the SL Engine in synchronous mode. */
-     result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- // get the engine interface, which is needed in order to create other objects
-     result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);  
-     assert(SL_RESULT_SUCCESS == result);  
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    createAudioRecord
-  * Signature: (Ljava/lang/String;)V
-  */
- JNIEXPORT **void** JNICALL Java_com_example_testopensl_AudioRecordActivity_createAudioRecord(JNIEnv *env, jobject, jstring uri) {  
- 
- **if** (recorderObject != NULL) {  
- 
-         ALOGD(" already create auido record");  
- **return** ;  
-     }  
- 
- **const****char*** utf8 = env->GetStringUTFChars(uri, NULL);  
-     gFile = fopen(utf8, "w");  
-     env->ReleaseStringUTFChars(uri, utf8);  
- 
- **if** (gFile == NULL) {  
-         ALOGD(" open file fail ");  
- **return** ;  
-     }  
- 
-     SLresult result;  
- 
- /* setup the data source*/
-     SLDataLocator_IODevice ioDevice = {  
-             SL_DATALOCATOR_IODEVICE,  
-             SL_IODEVICE_AUDIOINPUT,  
-             SL_DEFAULTDEVICEID_AUDIOINPUT,  
-             NULL  
-     };  
- 
-     SLDataSource recSource = {&ioDevice, NULL};  
- 
-     SLDataLocator_AndroidSimpleBufferQueue recBufferQueue = {  
-             SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,  
-             NB_BUFFERS_IN_QUEUE  
-     };  
- 
-     SLDataFormat_PCM pcm = {  
-             SL_DATAFORMAT_PCM,  
-             2,  
-             SL_SAMPLINGRATE_44_1,  
-             SL_PCMSAMPLEFORMAT_FIXED_16,  
-             16,  
-             SL_SPEAKER_FRONT_LEFT| SL_SPEAKER_FRONT_RIGHT,  
-             SL_BYTEORDER_LITTLEENDIAN  
-     };  
- 
-     SLDataSink dataSink = { &recBufferQueue, &pcm };  
-     SLInterfaceID iids[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};  
-     SLboolean required[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};  
- 
- /* Create the audio recorder */
-     result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject , &recSource, &dataSink,  
-             NUM_EXPLICIT_INTERFACES_FOR_RECORDER, iids, required);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* get the android configuration interface*/
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDCONFIGURATION, &configItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Realize the recorder in synchronous mode. */
-     result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Get the buffer queue interface which was explicitly requested */
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (**void***) &recBuffQueueItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* get the record interface */
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recordItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* Set up the recorder callback to get events during the recording */
-     result = (*recordItf)->SetMarkerPosition(recordItf, 2000);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->SetPositionUpdatePeriod(recordItf, 500);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->SetCallbackEventsMask(recordItf,  
-                 SL_RECORDEVENT_HEADATMARKER | SL_RECORDEVENT_HEADATNEWPOS);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->RegisterCallback(recordItf, recCallback, NULL);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Initialize the callback and its context for the recording buffer queue */
- 
-     cntxt.pDataBase = (int8_t*) &pcmData;  
-     cntxt.pData = cntxt.pDataBase;  
-     cntxt.size = **sizeof**(pcmData);  
-     result = (*recBuffQueueItf)->RegisterCallback(recBuffQueueItf, recBufferQueueCallback, &cntxt);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Enqueue buffers to map the region of memory allocated to store the recorded data */
-     ALOGD("Enqueueing buffer ");  
- **for** (**int** i = 0; i < NB_BUFFERS_IN_QUEUE; i++) {  
-         ALOGD("%d ", i);  
-         result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, cntxt.pData, BUFFER_SIZE_IN_BYTES);  
-         assert(SL_RESULT_SUCCESS == result);  
-         cntxt.pData += BUFFER_SIZE_IN_BYTES;  
-     }  
-     cntxt.pData = cntxt.pDataBase;  
- 
- /* Start recording */
-     result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);  
-     assert(SL_RESULT_SUCCESS == result);  
-     ALOGD("Starting to record");  
- 
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    stop
-  * Signature: ()V
-  */
- JNIEXPORT **void** JNICALL Java_com_example_testopensl_AudioRecordActivity_stop(JNIEnv *, jobject) {  
- **if** (recordItf != NULL) {  
-         SLresult result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);  
-         assert(SL_RESULT_SUCCESS == result);  
-     }  
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    shutdown
-  * Signature: ()V
-  */
- JNIEXPORT **void** JNICALL Java_com_example_testopensl_AudioRecordActivity_shutdown(JNIEnv *, jobject) {  
- 
- //destroy recorder object , and invlidate all associated interfaces
- **if** (recorderObject != NULL) {  
-         (*recorderObject)->Destroy(recorderObject);  
-         recorderObject = NULL;  
-         recordItf = NULL;  
-         recBuffQueueItf = NULL;  
-         configItf = NULL;  
-     }  
- 
- // destroy engine object, and invalidate all associated interfaces
- **if** (engineObject != NULL) {  
-         (*engineObject)->Destroy(engineObject);  
-         engineObject = NULL;  
-         engineEngine = NULL;  
-     }  
- 
- //colse the file
- **if** (gFile != NULL) {  
-         fclose(gFile);  
-         gFile == NULL;  
-     }  
- }  
方法说明：
Java_com_example_testopensl_AudioRecordActivity_createEngine 创建引擎
        Java_com_example_testopensl_AudioRecordActivity_createAudioRecord创建AudioRecorder并开始录音
        Java_com_example_testopensl_AudioRecordActivity_stop 暂停录音
        Java_com_example_testopensl_AudioRecordActivity_shutdown 释放资源
七、创建/jni/Android.mk 编译文件
[plain][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- LOCAL_PATH := $(call my-dir)  
- 
- include $(CLEAR_VARS)  
- 
- LOCAL_MODULE    := TestAudio  
- LOCAL_SRC_FILES := com_example_testopensl_AudioRecordActivity.cpp  
- LOCAL_LDLIBS += -llog -lOpenSLES -landroid  
- 
- include $(BUILD_SHARED_LIBRARY)  
LOCAL_LDLIBS 需要加llog、lOpenSLES、landroid 编译链接库
八、AndroidManifest 配置
录音时记得加上权限
[html][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- **<****uses-permission**android:name="android.permission.RECORD_AUDIO"**/>**
编写完Eclipse 结构图如下：
![](file:///D:/Program%20Files%20%28x86%29/Youdao/NoteData/elesos@163.com/0fa2ca82ed664bd194fe470d3d7ee942/20141213170625984.png)
运行之后界面图如下：
![](file:///D:/Program%20Files%20%28x86%29/Youdao/NoteData/elesos@163.com/ddf213b12d984099ae7f1942fb3448a1/20141213170929484.png)
代码编写完毕.点击开始就可以录音
在代码中需要注意的是录音的缓冲区大小，缓存区太小会导致录音失败（每个设备的最小缓冲区是不一样的），太大当音频实时传输会延迟比较大。这时候可以使用Java API获取最小的缓冲区，文中是写死的2*8192，在实际项目中建议使用API获取，下面是Java
 API：
[java][view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- **int** sampleRateInHz = 44100;  
- **int** channelConfig = AudioFormat.CHANNEL_OUT_STEREO;  
- **int** audioFormat = AudioFormat.ENCODING_PCM_16BIT;  
- **int** bufferSizeInBytes = AudioRecord.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat) * 2;  
这个可以通过jni访问java类AudioRecord实现，从而封装代码。
1，创建引擎
2，创建AudioRecorder并开始录音
3，暂停录音
4，释放资源
5，数据是通过回调函数处理的。
好处：缓冲区不用通过AudioRecord.getMinBufferSize获取，设置很小也能正常工作。比如设置80字节bytes
前面的文章我们讲解的是OpenSL ES音频播放，OpenSL ES非常强大，有音频播放当然有录音
下面我们编写OpenSL PCM录音，完成的功能是录制麦克风的声音存储PCM到data私有目录。
二、布局XML 创建文件 /res/layout/open_sl_audio_record.xml
**[html]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:orientation="vertical">
- 
- 
- <Button
- android:id="@+id/start"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:text="开始"/>
- 
- 
- <LinearLayout
- android:layout_width="match_parent"
- android:layout_height="wrap_content"
- android:orientation="horizontal">
- 
- <Button
- android:id="@+id/stop"
- android:layout_width="wrap_content"
- android:layout_height="wrap_content"
- android:text="暂停"/>
- 
- </LinearLayout>
- 
- </LinearLayout>
布局文件：开始和暂停两个Button控制录音
三、Activity类  创建/src/com/example/testopensl/AudioRecordActivity.java
**[java]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- package com.example.testopensl;  
- 
- import java.io.File;  
- 
- import android.app.Activity;  
- import android.os.Bundle;  
- import android.view.View;  
- import android.view.View.OnClickListener;  
- 
- import com.example.audio.R;  
- 
- publicclass AudioRecordActivity extends Activity implements OnClickListener {  
- 
- static {  
-         System.loadLibrary("TestAudio");  
-     }  
- privatebooleanmRecording;  
- 
- @Override
- protectedvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.open_sl_audio_record);  
-         findViewById(R.id.start).setOnClickListener(this);  
-         findViewById(R.id.stop).setOnClickListener(this);  
- createEngine();  
-     }  
- 
- @Override
- publicvoid onClick(View v) {  
- switch (v.getId()) {  
- case R.id.start:  
-             File file = new File(getCacheDir(), "audio.pcm");  
- createAudioRecord(file.getAbsolutePath());  
- mRecording = true;  
- break;  
- case R.id.stop:  
- stop();  
- mRecording = false;  
- break;  
-         }  
-     }  
- 
- @Override
- protectedvoid onDestroy() {  
- super.onDestroy();  
- if (mRecording) {  
-             stop();  
-         }  
-         shutdown();  
-     }  
- 
- /** Native methods, implemented in jni folder */
- publicnativevoid createEngine();  
- 
- publicnativevoidcreateAudioRecord(String uri);  
- 
- publicnativevoid stop();  
- 
- publicnativevoidshutdown();  
- 
- }  
文件存储位置：/data/data/com.example.audio/cache/audio.pcm
四、编写日志头文件，用于日志输出， 创建/jni/log.h 文件
**[cpp]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- #ifndef LOG_H_
- #define LOG_H_
- 
- #include <android/log.h>
- 
- #ifndef DGB
- #define DGB 0
- #endif
- 
- #ifndef LOG_TAG
- #define LOG_TAG __FILE__
- #endif
- 
- #ifndef ALOGD
- #if DGB
- #define ALOGD(...) \
-         __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)  
- #else
- #define ALOGD(...)   ((void)0)
- #endif
- #endif
- 
- #endif /* LOG_H_ */
六、JNI的实现 /jni/com_example_testopensl_AudioRecordActivity.cpp
**[cpp]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- /** log */
- #define LOG_TAG "NATVIE-AudioRecordActivity"
- #define DGB 1
- 
- 
- #include <jni.h>
- #include "com_example_testopensl_AudioRecordActivity.h"
- 
- #include "log.h"
- #include <stdio.h>
- #include <assert.h>
- 
- #include <SLES/OpenSLES.h>
- #include <SLES/OpenSLES_Android.h>
- 
- /* Size of the recording buffer queue */
- #define NB_BUFFERS_IN_QUEUE 1
- 
- /* Explicitly requesting SL_IID_ANDROIDSIMPLEBUFFERQUEUE and SL_IID_ANDROIDCONFIGURATION
-  * on the AudioRecorder object */
- #define NUM_EXPLICIT_INTERFACES_FOR_RECORDER 2
- 
- /* Size of the recording buffer queue */
- #define NB_BUFFERS_IN_QUEUE 1
- /* Size of each buffer in the queue */
- #define BUFFER_SIZE_IN_SAMPLES 8192
- #define BUFFER_SIZE_IN_BYTES   (2 * BUFFER_SIZE_IN_SAMPLES)
- 
- /* Local storage for Audio data */
- int8_t pcmData[NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES];  
- 
- 
- // engine interfaces
- static SLObjectItf engineObject = NULL;  
- static SLEngineItf engineEngine = NULL;  
- 
- //audio record interfaces
- static SLObjectItf recorderObject = NULL;  
- static SLRecordItf recordItf = NULL;  
- static SLAndroidSimpleBufferQueueItf recBuffQueueItf = NULL;  
- static SLAndroidConfigurationItf configItf = NULL;  
- 
- staticFILE * gFile = NULL;  
- 
- //-----------------------------------------------------------------
- /* Structure for passing information to callback function */
- typedefstruct CallbackCntxt_ {  
-     SLPlayItf  playItf;  
-     SLuint32   size;  
-     SLint8*   pDataBase;    // Base address of local audio data storage
-     SLint8*   pData;        // Current address of local audio data storage
- } CallbackCntxt;  
- 
- static CallbackCntxt cntxt;  
- 
- /* Callback for recording buffer queue events */
- void recCallback(SLRecordItf caller, void *pContext, SLuint32 event) {  
- if (SL_RECORDEVENT_HEADATNEWPOS & event) {  
-         SLmillisecond pMsec = 0;  
-         (*caller)->GetPosition(caller, &pMsec);  
-         ALOGD("SL_RECORDEVENT_HEADATNEWPOS current position=%ums\n", pMsec);  
-     }  
- 
- if (SL_RECORDEVENT_HEADATMARKER & event) {  
-         SLmillisecond pMsec = 0;  
-         (*caller)->GetPosition(caller, &pMsec);  
-         ALOGD("SL_RECORDEVENT_HEADATMARKER current position=%ums\n", pMsec);  
-     }  
- }  
- 
- /* Callback for recording buffer queue events */
- void recBufferQueueCallback(SLAndroidSimpleBufferQueueItf queueItf, void *pContext) {  
- 
-     CallbackCntxt *pCntxt = (CallbackCntxt*) pContext;  
- 
- /* Save the recorded data  */
-     fwrite(pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES, 1, gFile);  
- 
- /* Increase data pointer by buffer size */
-     pCntxt->pData += BUFFER_SIZE_IN_BYTES;  
- 
- if (pCntxt->pData >= pCntxt->pDataBase + (NB_BUFFERS_IN_QUEUE * BUFFER_SIZE_IN_BYTES)) {  
-         pCntxt->pData = pCntxt->pDataBase;  
-     }  
- 
-     (*queueItf)->Enqueue(queueItf, pCntxt->pDataBase, BUFFER_SIZE_IN_BYTES);  
- 
-     SLAndroidSimpleBufferQueueState recQueueState;  
-     (*queueItf)->GetState(queueItf, &recQueueState);  
- 
- }  
- 
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    createEngine
-  * Signature: ()V
-  */
- JNIEXPORT void JNICALL Java_com_example_testopensl_AudioRecordActivity_createEngine(JNIEnv *, jobject) {  
-     SLEngineOption EngineOption[] = {  
-                 {(SLuint32) SL_ENGINEOPTION_THREADSAFE, (SLuint32) SL_BOOLEAN_TRUE}  
-         };  
-     SLresult result;  
-     result = slCreateEngine(&engineObject, 1, EngineOption, 0, NULL, NULL);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Realizing the SL Engine in synchronous mode. */
-     result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- // get the engine interface, which is needed in order to create other objects
-     result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);  
-     assert(SL_RESULT_SUCCESS == result);  
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    createAudioRecord
-  * Signature: (Ljava/lang/String;)V
-  */
- JNIEXPORT void JNICALL Java_com_example_testopensl_AudioRecordActivity_createAudioRecord(JNIEnv *env, jobject, jstring uri) {  
- 
- if (recorderObject != NULL) {  
- 
-         ALOGD(" already create auido record");  
- return ;  
-     }  
- 
- constchar* utf8 = env->GetStringUTFChars(uri, NULL);  
-     gFile = fopen(utf8, "w");  
-     env->ReleaseStringUTFChars(uri, utf8);  
- 
- if (gFile == NULL) {  
-         ALOGD(" open file fail ");  
- return ;  
-     }  
- 
-     SLresult result;  
- 
- /* setup the data source*/
-     SLDataLocator_IODevice ioDevice = {  
-             SL_DATALOCATOR_IODEVICE,  
-             SL_IODEVICE_AUDIOINPUT,  
-             SL_DEFAULTDEVICEID_AUDIOINPUT,  
-             NULL  
-     };  
- 
-     SLDataSource recSource = {&ioDevice, NULL};  
- 
-     SLDataLocator_AndroidSimpleBufferQueue recBufferQueue = {  
-             SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,  
-             NB_BUFFERS_IN_QUEUE  
-     };  
- 
-     SLDataFormat_PCM pcm = {  
-             SL_DATAFORMAT_PCM,  
-             2,  
-             SL_SAMPLINGRATE_44_1,  
-             SL_PCMSAMPLEFORMAT_FIXED_16,  
-             16,  
-             SL_SPEAKER_FRONT_LEFT| SL_SPEAKER_FRONT_RIGHT,  
-             SL_BYTEORDER_LITTLEENDIAN  
-     };  
- 
-     SLDataSink dataSink = { &recBufferQueue, &pcm };  
-     SLInterfaceID iids[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_ANDROIDCONFIGURATION};  
-     SLboolean required[NUM_EXPLICIT_INTERFACES_FOR_RECORDER] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};  
- 
- /* Create the audio recorder */
-     result = (*engineEngine)->CreateAudioRecorder(engineEngine, &recorderObject , &recSource, &dataSink,  
-             NUM_EXPLICIT_INTERFACES_FOR_RECORDER, iids, required);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* get the android configuration interface*/
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDCONFIGURATION, &configItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Realize the recorder in synchronous mode. */
-     result = (*recorderObject)->Realize(recorderObject, SL_BOOLEAN_FALSE);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Get the buffer queue interface which was explicitly requested */
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, (void*) &recBuffQueueItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* get the record interface */
-     result = (*recorderObject)->GetInterface(recorderObject, SL_IID_RECORD, &recordItf);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- 
- /* Set up the recorder callback to get events during the recording */
-     result = (*recordItf)->SetMarkerPosition(recordItf, 2000);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->SetPositionUpdatePeriod(recordItf, 500);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->SetCallbackEventsMask(recordItf,  
-                 SL_RECORDEVENT_HEADATMARKER | SL_RECORDEVENT_HEADATNEWPOS);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
-     result = (*recordItf)->RegisterCallback(recordItf, recCallback, NULL);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Initialize the callback and its context for the recording buffer queue */
- 
-     cntxt.pDataBase = (int8_t*) &pcmData;  
-     cntxt.pData = cntxt.pDataBase;  
-     cntxt.size = sizeof(pcmData);  
-     result = (*recBuffQueueItf)->RegisterCallback(recBuffQueueItf, recBufferQueueCallback, &cntxt);  
-     assert(SL_RESULT_SUCCESS == result);  
- 
- /* Enqueue buffers to map the region of memory allocated to store the recorded data */
-     ALOGD("Enqueueing buffer ");  
- for (int i = 0; i < NB_BUFFERS_IN_QUEUE; i++) {  
-         ALOGD("%d ", i);  
-         result = (*recBuffQueueItf)->Enqueue(recBuffQueueItf, cntxt.pData, BUFFER_SIZE_IN_BYTES);  
-         assert(SL_RESULT_SUCCESS == result);  
-         cntxt.pData += BUFFER_SIZE_IN_BYTES;  
-     }  
-     cntxt.pData = cntxt.pDataBase;  
- 
- /* Start recording */
-     result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_RECORDING);  
-     assert(SL_RESULT_SUCCESS == result);  
-     ALOGD("Starting to record");  
- 
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    stop
-  * Signature: ()V
-  */
- JNIEXPORT void JNICALL Java_com_example_testopensl_AudioRecordActivity_stop(JNIEnv *, jobject) {  
- if (recordItf != NULL) {  
-         SLresult result = (*recordItf)->SetRecordState(recordItf, SL_RECORDSTATE_STOPPED);  
-         assert(SL_RESULT_SUCCESS == result);  
-     }  
- }  
- 
- /*
-  * Class:     com_example_testopensl_AudioRecordActivity
-  * Method:    shutdown
-  * Signature: ()V
-  */
- JNIEXPORT void JNICALL Java_com_example_testopensl_AudioRecordActivity_shutdown(JNIEnv *, jobject) {  
- 
- //destroy recorder object , and invlidate all associated interfaces
- if (recorderObject != NULL) {  
-         (*recorderObject)->Destroy(recorderObject);  
-         recorderObject = NULL;  
-         recordItf = NULL;  
-         recBuffQueueItf = NULL;  
-         configItf = NULL;  
-     }  
- 
- // destroy engine object, and invalidate all associated interfaces
- if (engineObject != NULL) {  
-         (*engineObject)->Destroy(engineObject);  
-         engineObject = NULL;  
-         engineEngine = NULL;  
-     }  
- 
- //colse the file
- if (gFile != NULL) {  
-         fclose(gFile);  
-         gFile == NULL;  
-     }  
- }  
方法说明：
Java_com_example_testopensl_AudioRecordActivity_createEngine 创建引擎
        Java_com_example_testopensl_AudioRecordActivity_createAudioRecord创建AudioRecorder并开始录音
        Java_com_example_testopensl_AudioRecordActivity_stop 暂停录音
        Java_com_example_testopensl_AudioRecordActivity_shutdown 释放资源
七、创建/jni/Android.mk 编译文件
**[plain]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- LOCAL_PATH := $(call my-dir)  
- 
- include $(CLEAR_VARS)  
- 
- LOCAL_MODULE    := TestAudio  
- LOCAL_SRC_FILES := com_example_testopensl_AudioRecordActivity.cpp  
- LOCAL_LDLIBS += -llog -lOpenSLES -landroid  
- 
- include $(BUILD_SHARED_LIBRARY)  
LOCAL_LDLIBS 需要加llog、lOpenSLES、landroid 编译链接库
八、AndroidManifest 配置
录音时记得加上权限
**[html]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- <uses-permissionandroid:name="android.permission.RECORD_AUDIO"/>
编写完Eclipse 结构图如下：
![](file:///D:/Program%20Files%20%28x86%29/Youdao/NoteData/elesos@163.com/0fa2ca82ed664bd194fe470d3d7ee942/20141213170625984.png)
运行之后界面图如下：
![](file:///D:/Program%20Files%20%28x86%29/Youdao/NoteData/elesos@163.com/ddf213b12d984099ae7f1942fb3448a1/20141213170929484.png)
代码编写完毕.点击开始就可以录音
在代码中需要注意的是录音的缓冲区大小，缓存区太小会导致录音失败（每个设备的最小缓冲区是不一样的），太大当音频实时传输会延迟比较大。这时候可以使用Java API获取最小的缓冲区，文中是写死的2*8192，在实际项目中建议使用API获取，下面是Java
 API：
**[java]**[view
 plain](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)[copy](http://blog.csdn.net/liuhaitao_share/article/details/41911015#)
- int sampleRateInHz = 44100;  
- int channelConfig = AudioFormat.CHANNEL_OUT_STEREO;  
- int audioFormat = AudioFormat.ENCODING_PCM_16BIT;  
- int bufferSizeInBytes = AudioRecord.getMinBufferSize(sampleRateInHz, channelConfig, audioFormat) * 2;  
这个可以通过jni访问java类AudioRecord实现，从而封装代码。
