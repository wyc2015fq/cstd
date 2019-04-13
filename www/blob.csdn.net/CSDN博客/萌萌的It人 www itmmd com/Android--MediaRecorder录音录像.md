
# Android--MediaRecorder录音录像 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年12月02日 15:10:23[Jlins](https://me.csdn.net/dyllove98)阅读数：4975


**前言**
Android除了支持播放多媒体文件之外，还可以从对应的硬件中捕获多媒体，比如从麦克风录音、从摄像头录像等。本篇博客讲解一下Android下如何通过MediaRecorder进行录音以及录像的步骤，最后将以简单的Demo演示。
本篇博客的主要内容：
[MediaRecorder](http://www.cnblogs.com/plokmju/p/android_MediaRecorder.html#mr)
[MediaRecorder录制音频](http://www.cnblogs.com/plokmju/p/android_MediaRecorder.html#amr)
[MediaRecorder录制视频](http://www.cnblogs.com/plokmju/p/android_MediaRecorder.html#mp4)
[MediaRecorder的内部类](http://www.cnblogs.com/plokmju/p/android_MediaRecorder.html#NestedClass)
**MediaRecorder**
MediaRecorder主要用于记录音频与视频，它位于"android.media.MediaRecorder"下。它为开发人员提供了详尽的AIP以完成记录音频与视频的工作，下面简单介绍一些常用的。
void prepar()：准备录制。
void start()：开始录制。
void stop()：停止录制。
void reset()：重置MediaRecorder。
void release()：释放MediaRecorder占用的资源。
void setAudioEncoder(int)：设置音频记录的编码格式。
void setAudoSource(int)：设置音频记录的音频源。
void setOutoutFormat(int)：设置记录的媒体文件的输出转换格式。
void setPreviewDisplay(Surface)：设置视频的预览界面。
void setVideoEncoder(int)：设置视频记录的编码格式。
void setVideoSource(int)：设置视频记录的视频源。
void setOutputFile(String)：媒体文件输出路径。
void setMaxDuration(int)：设置最大记录时长，单位为毫秒。
上面仅仅列出了一些常用的方法，但是通过这个方法可以看出MediaRecorder是需要在多个状态之间切换的，准备、开始、停止、重置等等。MediaRecorder也为我们提供了一套状态切换的规则，只有在既定的状态下，才能完成对应的操作，下图是官方文档提供的状态图。
![](http://images.cnitblog.com/blog/234895/201311/24234720-7b799f27675941dab22ea49a643dab69.gif)
上图非常直观的展示了MediaRecorder的状态间的转换，这里只说几点需要注意的地方：
当为MediaRecorder设定媒体录入源之后即初始化完成，这个时候才可以为其设定其他的参数。
在开始录制前必须先调用prepare()准备录制。
只有在Initial初始化状态，才可以对MediaRecorder调用release()释放资源，其他状态必须先stop()或者reset()。
错误状态是游离在所有状态之外的，当发生错误的时候，只有进行reset()才会进入Initial初始状态。
MediaRecorder录制视频必不可少的步骤：
调用new MediaRecorder()构造函数得到MediaRecorder的实例。
调用setOutputFormat()设定媒体文件的输出格式，必须在设定音频与视频的编码格式之前设定。
如果需要记录音频，则调用setAudioSource()设定音频的录入源以及调用setAudioEncoder()设定音频的编码方式。
如果需要记录视频，则调用setVideoSource()设定视频的录入员以及调用setVideoEncoder()设定视频的编码方式。
调用setOutputFile()设定记录的媒体文件保存的路径。
先调用prepare()准备录制，准备完成之后调用start()开始录制。
记录完成后，调用stop()停止录制。
**MediaRecorder录制音频**
上面介绍了使用MediaRecorder录制视频必不可少的步骤，因为录制音频无需捕获图像，所以更简单一些。下面直接通过一个简单的示例，演示如何录制音频，录制的音频文件保存在sd根目录下，注释很详细，这里不在累述了。布局很简单，只有两个Button，开始录音、结束录音，就不提供布局代码了。
实现代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1packagecn.bgxt.mediarecorderdemo;23importjava.io.File;4importandroid.app.Activity;5importandroid.media.MediaRecorder;6importandroid.media.MediaRecorder.OnErrorListener;7importandroid.os.Bundle;8importandroid.view.View;9importandroid.view.View.OnClickListener;10importandroid.widget.Button;11importandroid.widget.Toast;1213publicclassRecordActivityextendsActivity {14privateButton btn_RecordStart, btn_RecordStop;15privateMediaRecorder mediaRecorder;16privatebooleanisRecording;17@Override18protectedvoidonCreate(Bundle savedInstanceState) {19//TODO Auto-generated method stub20super.onCreate(savedInstanceState);21setContentView(R.layout.activity_record);2223btn_RecordStart =(Button) findViewById(R.id.btn_RecordStart);24btn_RecordStop =(Button) findViewById(R.id.btn_RecordStop);2526btn_RecordStop.setEnabled(false);2728btn_RecordStart.setOnClickListener(click);29btn_RecordStop.setOnClickListener(click);30}3132privateView.OnClickListener click =newOnClickListener() {3334@Override35publicvoidonClick(View v) {36switch(v.getId()) {37caseR.id.btn_RecordStart:38start();39break;40caseR.id.btn_RecordStop:41stop();42break;43default:44break;45}46}47};4849/**50* 开始录音51*/52protectedvoidstart() {53try{54File file =newFile("/sdcard/mediarecorder.amr");55if(file.exists()) {56//如果文件存在，删除它，演示代码保证设备上只有一个录音文件57file.delete();58}59mediaRecorder =newMediaRecorder();60//设置音频录入源61mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);62//设置录制音频的输出格式63mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);64//设置音频的编码格式65mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);66//设置录制音频文件输出文件路径67mediaRecorder.setOutputFile(file.getAbsolutePath());6869mediaRecorder.setOnErrorListener(newOnErrorListener() {7071@Override72publicvoidonError(MediaRecorder mr,intwhat,intextra) {73//发生错误，停止录制74mediaRecorder.stop();75mediaRecorder.release();76mediaRecorder =null;77isRecording=false;78btn_RecordStart.setEnabled(true);79btn_RecordStop.setEnabled(false);80Toast.makeText(RecordActivity.this, "录音发生错误", 0).show();81}82});8384//准备、开始85mediaRecorder.prepare();86mediaRecorder.start();8788isRecording=true;89btn_RecordStart.setEnabled(false);90btn_RecordStop.setEnabled(true);91Toast.makeText(RecordActivity.this, "开始录音", 0).show();92}catch(Exception e) {93e.printStackTrace();94}95}9697/**98* 录音结束99*/100protectedvoidstop() {101if(isRecording) {102//如果正在录音，停止并释放资源103mediaRecorder.stop();104mediaRecorder.release();105mediaRecorder =null;106isRecording=false;107btn_RecordStart.setEnabled(true);108btn_RecordStop.setEnabled(false);109Toast.makeText(RecordActivity.this, "录音结束", 0).show();110}111}112113@Override114protectedvoidonDestroy() {115if(isRecording) {116//如果正在录音，停止并释放资源117mediaRecorder.stop();118mediaRecorder.release();119mediaRecorder =null;120}121super.onDestroy();122}123124}![复制代码](http://common.cnblogs.com/images/copycode.gif)
录制音频需要访问麦克风设备，访问设备需要添加权限：
<uses-permissionandroid:name="android.permission.RECORD_AUDIO"/>
效果展示：
![](http://images.cnitblog.com/blog/234895/201311/26231722-81f3acc98343406abbf59acecd5517d2.x-png)![](http://images.cnitblog.com/blog/234895/201311/26231749-879d856b40034eb08e4583b3354ed263.x-png)
**MediaRecorder录制视频**
上面已经介绍了使用MediaRecorder录制视频的步骤，这里依照步骤一步一步写代码就可以了。下面通过一个简单的Demo演示如何使用MediaRecorder录制一段从摄像头捕获到的视频，录制的视频文件将保持在SD卡上。布局很简单，两个Button，开始、停止，然后还有一个SurfaceView显示预览效果，就不给出布局代码了。
实现代码：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1packagecn.bgxt.mediarecorderdemo;23importjava.io.File;4importjava.io.IOException;56importandroid.app.Activity;7importandroid.media.MediaRecorder;8importandroid.media.MediaRecorder.OnErrorListener;9importandroid.os.Bundle;10importandroid.view.SurfaceHolder;11importandroid.view.SurfaceView;12importandroid.view.View;13importandroid.view.View.OnClickListener;14importandroid.widget.Button;15importandroid.widget.Toast;1617publicclassVideoActivityextendsActivity {18privateButton btn_VideoStart, btn_VideoStop;19privateSurfaceView sv_view;20privatebooleanisRecording;21privateMediaRecorder mediaRecorder;2223@Override24protectedvoidonCreate(Bundle savedInstanceState) {25super.onCreate(savedInstanceState);26setContentView(R.layout.activity_video);2728btn_VideoStart =(Button) findViewById(R.id.btn_VideoStart);29btn_VideoStop =(Button) findViewById(R.id.btn_VideoStop);30sv_view =(SurfaceView) findViewById(R.id.sv_view);3132btn_VideoStop.setEnabled(false);3334btn_VideoStart.setOnClickListener(click);35btn_VideoStop.setOnClickListener(click);3637//声明Surface不维护自己的缓冲区，针对Android3.0以下设备支持38sv_view.getHolder().setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);39}4041privateView.OnClickListener click =newOnClickListener() {4243@Override44publicvoidonClick(View v) {45switch(v.getId()) {46caseR.id.btn_VideoStart:47start();48break;49caseR.id.btn_VideoStop:50stop();51break;52default:53break;54}55}56};5758protectedvoidstart() {59try{60File file =newFile("/sdcard/video.mp4");61if(file.exists()) {62//如果文件存在，删除它，演示代码保证设备上只有一个录音文件63file.delete();64}6566mediaRecorder =newMediaRecorder();67mediaRecorder.reset();68//设置音频录入源69mediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);70//设置视频图像的录入源71mediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);72//设置录入媒体的输出格式73mediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.MPEG_4);74//设置音频的编码格式75mediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);76//设置视频的编码格式77mediaRecorder.setVideoEncoder(MediaRecorder.VideoEncoder.MPEG_4_SP);78//设置视频的采样率，每秒4帧79mediaRecorder.setVideoFrameRate(4);80//设置录制视频文件的输出路径81mediaRecorder.setOutputFile(file.getAbsolutePath());82//设置捕获视频图像的预览界面83mediaRecorder.setPreviewDisplay(sv_view.getHolder().getSurface());8485mediaRecorder.setOnErrorListener(newOnErrorListener() {8687@Override88publicvoidonError(MediaRecorder mr,intwhat,intextra) {89//发生错误，停止录制90mediaRecorder.stop();91mediaRecorder.release();92mediaRecorder =null;93isRecording=false;94btn_VideoStart.setEnabled(true);95btn_VideoStop.setEnabled(false);96Toast.makeText(VideoActivity.this, "录制出错", 0).show();97}98});99100//准备、开始101mediaRecorder.prepare();102mediaRecorder.start();103104btn_VideoStart.setEnabled(false);105btn_VideoStop.setEnabled(true);106isRecording =true;107Toast.makeText(VideoActivity.this, "开始录像", 0).show();108}catch(Exception e) {109e.printStackTrace();110}111112}113114protectedvoidstop() {115if(isRecording) {116//如果正在录制，停止并释放资源117mediaRecorder.stop();118mediaRecorder.release();119mediaRecorder =null;120isRecording=false;121btn_VideoStart.setEnabled(true);122btn_VideoStop.setEnabled(false);123Toast.makeText(VideoActivity.this, "停止录像，并保存文件", 0).show();124}125}126127@Override128protectedvoidonDestroy() {129if(isRecording) {130mediaRecorder.stop();131mediaRecorder.release();132mediaRecorder =null;133}134super.onDestroy();135}136137}![复制代码](http://common.cnblogs.com/images/copycode.gif)
录制视频需要用到麦克风和摄像头硬件，还需要赋予一些相应的权限。
1<uses-permissionandroid:name="android.permission.RECORD_AUDIO"/>2<uses-permissionandroid:name="android.permission.WRITE_EXTERNAL_STORAGE"/>3<uses-permissionandroid:name="android.permission.CAMERA"/>4<uses-permissionandroid:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS"/>
效果展示：
![](http://images.cnitblog.com/blog/234895/201311/26232419-8a381dbd656d4e30af680e47d8d65d95.x-png)![](http://images.cnitblog.com/blog/234895/201311/26232437-f3f21985c1fd45ae808a8a75ba603ff7.x-png)
**MediaRecorder的内部类**
通过上面录制视频的Demo可以看出，使用MediaRecorder录制视频，需要设置一些录入源、编码格式、输出格式等。这些方法都是以int类型的参数设定的，它们其实都是在MediaRecorder的内部类中已经以常量的形式定义好了，直接适用场景进行选择即可。
MediaRecorder.AudioEncoder：音频的编码格式。
MediaRecorder.AudioSource：录制的音频源。
MediaRecorder.OutputFormat：媒体文件的输出编码格式。
MediaRecorder.VideoEncoder：视频的编码格式。
MediaRecorder.VideoSource：录制的视频源。

