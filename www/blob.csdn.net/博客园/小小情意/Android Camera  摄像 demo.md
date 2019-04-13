
# Android Camera  摄像 demo - 小小情意 - 博客园






# [Android Camera  摄像 demo](https://www.cnblogs.com/xiaoxiaoqingyi/p/7016312.html)
google 在Android 5.0推出 Camera2 这个类，用于替换 Camera，但是Camera2要求android sdk 最低版本为 minSdkVersion = 21 （5.0系统），所以Camera2 还不能完全替换 Camera，在兼容低版本的时候，还是需要两者一起协同开发。下面我来说一下 Camera 的拍摄例子：
首先需要在xml 上布局一个 SurfaceView 设置全屏
<SurfaceView
android:id="@+id/surfaceView"android:layout_width="match_parent"android:layout_height="match_parent"/>

同时也把状态栏和titleBar隐藏了：
requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏


然后获取SurfacView 实例以及其持有者SurfaceHolder，接入SurfaceHolder.Callback回调，
mSurfaceView =(SurfaceView)findViewById(R.id.surfaceView);
mSurfaceHolder= mSurfaceView.getHolder();//取得holdermSurfaceHolder.addCallback(this);//holder加入回调接口mSurfaceHolder.setKeepScreenOn(true);

SurfaceHolder.Callback会在页面Actvity 初始化完毕后调用，则在回调的surfaceChanged初始化Camera,也就是打开预览页面：
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170615104321493-1429594993.jpg)
@OverridepublicvoidsurfaceChanged(SurfaceHolder holder,intformat,intwidth,intheight) {if(mCamera !=null) {
freeCameraResource();
}try{
mCamera=Camera.open();if(mCamera ==null)return;
mCamera.setDisplayOrientation(90);
mCamera.setPreviewDisplay(mSurfaceHolder);
parameters= mCamera.getParameters();//获得相机参数List<Camera.Size> mSupportedPreviewSizes =parameters.getSupportedPreviewSizes();
List<Camera.Size> mSupportedVideoSizes =parameters.getSupportedVideoSizes();
optimalSize=CameraHelper.getOptimalVideoSize(mSupportedVideoSizes,
mSupportedPreviewSizes, height, width);
 
parameters.setPreviewSize(optimalSize.width, optimalSize.height);//设置预览图像大小parameters.set("orientation", "portrait");
List<String> focusModes =parameters.getSupportedFocusModes();if(focusModes.contains("continuous-video")) {
parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
}
mFpsRange=parameters.getSupportedPreviewFpsRange();
 
mCamera.setParameters(parameters);//设置相机参数mCamera.startPreview();//开始预览}catch(Exception io){
io.printStackTrace();
}
}

该方法返回了SurfaceView的宽与高，根据给出的尺寸与宽高比例，获取一个最适配的预览尺寸，你先看下面有两个参数：
List<Camera.Size> mSupportedPreviewSizes =parameters.getSupportedPreviewSizes();
List<Camera.Size> mSupportedVideoSizes = parameters.getSupportedVideoSizes();

这两个队列分别是 该相机支持的 预览大小（一般就是拍照时照片的大小），另外一个就是支持适配的大小，因为都是队列，说明相机支持很多组尺寸，而且，照片的尺寸与视频的尺寸是不一样的。我debug看了几款手机，通常摄像支持的尺寸少一点，照片会多一些。这样，我们就要通过刚刚方法给出的宽高，获取一个最佳匹配的预览尺寸：
publicstaticCamera.Size getOptimalVideoSize(List<Camera.Size>supportedVideoSizes,
List<Camera.Size> previewSizes,intw,inth) {//Use a very small tolerance because we want an exact match.finaldoubleASPECT_TOLERANCE = 0.1;doubletargetRatio = (double) w /h;//Supported video sizes list might be null, it means that we are allowed to use the preview//sizesList<Camera.Size>videoSizes;if(supportedVideoSizes !=null) {
videoSizes=supportedVideoSizes;
}else{
videoSizes=previewSizes;
}
Camera.Size optimalSize=null;//Start with max value and refine as we iterate over available video sizes. This is the//minimum difference between view and camera height.doubleminDiff =Double.MAX_VALUE;//Target view heightinttargetHeight =h;//Try to find a video size that matches aspect ratio and the target view size.//Iterate over all available sizes and pick the largest size that can fit in the view and//still maintain the aspect ratio.for(Camera.Size size : videoSizes) {doubleratio = (double) size.width /size.height;if(Math.abs(ratio - targetRatio) >ASPECT_TOLERANCE)continue;if(Math.abs(size.height - targetHeight) < minDiff &&previewSizes.contains(size)) {
optimalSize=size;
minDiff= Math.abs(size.height -targetHeight);
}
}//Cannot find video size that matches the aspect ratio, ignore the requirementif(optimalSize ==null) {
minDiff=Double.MAX_VALUE;for(Camera.Size size : videoSizes) {if(Math.abs(size.height - targetHeight) < minDiff &&previewSizes.contains(size)) {
optimalSize=size;
minDiff= Math.abs(size.height -targetHeight);
}
}
}returnoptimalSize;
}

该方法是获取最佳的预览与摄像尺寸。然后设置预览图像大小：
parameters.setPreviewSize(optimalSize.width, optimalSize.height);

可以通过获取相机的参数实例，设置里面各种效果，包括刚刚的预览图，前置摄像头，闪光灯等。
parameters = mCamera.getParameters();//获得相机参数

设置好预览已经相机参数，则打开:
mCamera.setParameters(parameters);//设置相机参数mCamera.startPreview();//开始预览

那么就进入一个预览的拍摄页面了，该页面其实也可以用来做拍照。要想做拍摄，还要实例化MediaRecorder，然后传入camera并初始化相应的参数：
mMediaRecorder.setCamera(mCamera);
mMediaRecorder.setOnErrorListener(this);
mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.DEFAULT );
mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);//视频源//Use the same size for recording profile.CamcorderProfile mProfile =CamcorderProfile.get(CamcorderProfile.QUALITY_HIGH);
mProfile.videoFrameWidth=optimalSize.width;
mProfile.videoFrameHeight=optimalSize.height;
 
mMediaRecorder.setProfile(mProfile);//该设置是为了抽取视频的某些帧，真正录视频的时候，不要设置该参数//mMediaRecorder.setCaptureRate(mFpsRange.get(0)[0]);//获取最小的每一秒录制的帧数mMediaRecorder.setOutputFile(mVecordFile.getAbsolutePath());
 
mMediaRecorder.prepare();
mMediaRecorder.start();

录完的时候停止，需要重置才能再次使用。
try{
mMediaRecorder.stop();
mMediaRecorder.reset();
}catch(Exception e) {
e.printStackTrace();
}

当页面destroy的时候，要记得释放它们：
privatevoidreleaseRecord() {if(mMediaRecorder !=null) {
mMediaRecorder.setPreviewDisplay(null);
mMediaRecorder.setOnErrorListener(null);try{
mMediaRecorder.release();
}catch(IllegalStateException e) {
e.printStackTrace();
}catch(Exception e) {
e.printStackTrace();
}
}
mMediaRecorder=null;
}

闪光灯关闭与开启：
privatevoidflashLightToggle(){try{if(isFlashLightOn){
parameters.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
mCamera.setParameters(parameters);
isFlashLightOn=false;
}else{
parameters.setFlashMode(Camera.Parameters.FLASH_MODE_TORCH);
mCamera.setParameters(parameters);
isFlashLightOn=true;
}
}catch(Exception e) {
e.printStackTrace();
}
}

前后摄像头切换，就要重新初始化 camera实例：
privatevoidswitchCamera(){
Camera.CameraInfo cameraInfo=newCamera.CameraInfo();intcameraCount = Camera.getNumberOfCameras();//得到摄像头的个数for(inti = 0; i < cameraCount; i++) {
Camera.getCameraInfo(i, cameraInfo);//得到每一个摄像头的信息if(cameraPosition == 1) {//现在是后置，变更为前置if(cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {//代表摄像头的方位，CAMERA_FACING_FRONT前置 CAMERA_FACING_BACK后置mCamera.stopPreview();//停掉原来摄像头的预览mCamera.release();//释放资源mCamera =null;//取消原来摄像头mCamera = Camera.open(i);//打开当前选中的摄像头try{
mCamera.setDisplayOrientation(90);
mCamera.setPreviewDisplay(mSurfaceHolder);//通过surfaceview显示取景画面}catch(IOException e) {//TODO Auto-generated catch blocke.printStackTrace();
}
mCamera.setParameters(parameters);//设置相机参数mCamera.startPreview();//开始预览cameraPosition = 0;break;
}
}else{//现在是前置， 变更为后置if(cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {//代表摄像头的方位，CAMERA_FACING_FRONT前置 CAMERA_FACING_BACK后置mCamera.stopPreview();//停掉原来摄像头的预览mCamera.release();//释放资源mCamera =null;//取消原来摄像头mCamera = Camera.open(i);//打开当前选中的摄像头try{
mCamera.setDisplayOrientation(90);
mCamera.setPreviewDisplay(mSurfaceHolder);//通过surfaceview显示取景画面}catch(IOException e) {//TODO Auto-generated catch blocke.printStackTrace();
}
mCamera.setParameters(parameters);//设置相机参数mCamera.startPreview();//开始预览cameraPosition = 1;break;
}
}
 
}
}

录制时的页面：
![](https://images2015.cnblogs.com/blog/804587/201706/804587-20170615104412196-656783742.jpg)
这就是使用 camera 来摄像的主要步骤，demo:[https://github.com/xiaoxiaoqingyi/android-CameraVideo](https://github.com/xiaoxiaoqingyi/android-CameraVideo)
如果你想了解 Camera2，你也可以看看google 的 Camera2 官方例子：
[https://github.com/googlesamples/android-Camera2Basic](https://github.com/googlesamples/android-Camera2Basic)






