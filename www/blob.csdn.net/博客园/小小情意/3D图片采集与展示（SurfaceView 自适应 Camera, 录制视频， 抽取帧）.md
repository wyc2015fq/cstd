
# 3D图片采集与展示（SurfaceView 自适应 Camera, 录制视频， 抽取帧） - 小小情意 - 博客园






# [3D图片采集与展示（SurfaceView 自适应 Camera, 录制视频， 抽取帧）](https://www.cnblogs.com/xiaoxiaoqingyi/p/5025579.html)
![](https://images2015.cnblogs.com/blog/804587/201512/804587-20151217152345740-461155476.jpg)![](https://images2015.cnblogs.com/blog/804587/201512/804587-20151217152355771-2039140740.jpg)![](https://images2015.cnblogs.com/blog/804587/201512/804587-20151217152414037-1648655745.gif)
最近在做一个3D图片采集与展示。
主要功能为：自定义Camera（google 已经摈弃了Camera, 推荐使用Camera2，后续篇幅，我将会用Camera2取代Camera），围绕一个物体360度录制一个视频，然后在该视频抽取一定数量的帧，保存为图片存放。最后在一个Activity页面展示第一张图片，通过滑动或点击切换下一张图片，从而形成用图片展示的3D效果。该项目主要的目的是采集3D图片素材，然后上传到服务器处理，最终在用户客户端或网页端展示是通过OpenGL ES处理而来。
技术要点：
## 1.在SurfaceView 展示Camera的时候，如果不按照camera支持的尺寸比例，那么预览会出现拉伸。
mCamera.getParameters().getSupportedPreviewSizes();
mCamera.getParameters().getSupportedPictureSizes();
可以通过debug查看该摄像头支持哪些预览框大小、图片大小。如果要做适配，需要通过轮询获取自己所需要范围大小。通常情况下，是支持全屏大小，也就是该手机的像素尺寸。
如果你想把预览框设置成正方形，原则上是不行的（本人目前没有找到相应的方法，求大牛指示），那我们可以先用全屏的摄像框尺寸，然后通过在层叠隐藏部分区域形成正方形，
再获取图像时根据隐藏的区域做相应的截图，就能得到你想要的任何效果了。（如上图示例）
在预览中也有些小问题，比如说Camera默认是横向取景，你需要mCamera.setDisplayOrientation(90);同时图片保存的时候需要再旋转90度，才能达到预览的效果。
## 2.在录像时，也会遇到一些问题图片会有拉伸，跟预览的时候不一致。这时，应该获取设备所支持的参数。
CamcorderProfile mProfile = null;
if(CamcorderProfile.hasProfile(CamcorderProfile.QUALITY_1080P)) {
mProfile = CamcorderProfile.get(CamcorderProfile.QUALITY_1080P);
}else if(CamcorderProfile.hasProfile(CamcorderProfile.QUALITY_720P)){
mProfile = CamcorderProfile.get(CamcorderProfile.QUALITY_720P);
}else {
mProfile = CamcorderProfile.get(CamcorderProfile.QUALITY_480P);
}
主要还是要根据自己的实际情况去获取，然后再设置参数：
mMediaRecorder.setProfile(mProfile);//不知道为什么，直接这么设置，不起作用，要下面set 才行
//mMediaRecorder.setProfile(mProfile);//不知道为什么，直接这么设置，不起作用，要下面set 才行
mMediaRecorder.setOutputFormat(mProfile.fileFormat);// 视频输出格式
mMediaRecorder.setVideoFrameRate(mProfile.fileFormat);// 设置录制的视频帧率
mMediaRecorder.setVideoSize(mHeight, mWidth);;// 设置分辨率：
mMediaRecorder.setVideoEncodingBitRate(mProfile.videoBitRate);// 设置帧频率，然后就清晰了
mMediaRecorder.setVideoEncoder(mProfile.videoCodec);// 视频录制格式
## 3. 从视频抽帧， 每一帧是bitmap形式返回。从Java接口提供的抽帧方法，
MediaMetadataRetriever 类
getFrameAtTime(long timeUs)
通过时间去抽帧，而每一帧的时间也未必相隔是均匀的，这导致抽出来的帧会有重复，这需要再深入研究(后续再研究)。
具体方法为：
/**
 * 获取视频关键帧
 */
public void getFrameFromVideo(String filePath, String dirName){
    mFile = new File(filePath);
    retriever = new MediaMetadataRetriever();
    retriever.setDataSource(filePath);
    fileLength = retriever.extractMetadata(MediaMetadataRetriever.METADATA_KEY_DURATION);
    long lengthLong = Long.parseLong(fileLength) *1000/ (Constants.Num_Frame - 1);
    String prefixName = Utils.randomCapital(5);//图片名前缀
    int k = 0;
    for (long i = 0; i< Long.parseLong(fileLength)*1000+lengthLong; i=i+lengthLong){
        k++;
        Bitmap bitmap = retriever.getFrameAtTime(i);
        if(bitmap != null){
            Matrix matrix = new Matrix();
            matrix.reset();
            matrix.setRotate(90);//图片默认是横盘的，转90度变竖屏
            if(bitmap.getWidth() >bitmap.getHeight()){
                bitmap = Bitmap.createBitmap(bitmap,(bitmap.getWidth() - bitmap.getHeight())/2,0, bitmap.getHeight(), bitmap.getHeight(),matrix, true);
            }else{
                bitmap = Bitmap.createBitmap(bitmap,0,(bitmap.getHeight()-bitmap.getWidth())/2, bitmap.getWidth(), bitmap.getWidth(),matrix, true);
            }
            BitmapUtils.saveBitmap(bitmap, dirName, prefixName + k + ".jpg");
        }
        if(k < 50){
            sendMessageForProgress(dirName,k,true);
        }else {
            sendMessageForProgress(dirName,k,false);
        }
    }
    //删除视频文件
    mFile.delete();
}
最后呈现一下，预览与录制视频的代码吧。RecordVideoActivity
publicclassRecordVideoActivityextendsAppCompatActivityimplementsView.OnClickListener
                             , SurfaceHolder.Callback, MediaRecorder.OnErrorListener{privateProgressBar mProgressbar;privateSurfaceView mSurfaceView;privateMediaRecorder mMediaRecorder;//录制视频的类privateSurfaceHolder mSurfaceHolder;privateCamera mCamera;privateTimer mTimer;//计时器privatebooleanisOpenCamera =true;//是否一开始就打开摄像头privatefinalstaticintmRecordMaxTime = 20;//一次拍摄最长时间privateOnRecordFinishListener mOnRecordFinishListener;//录制完成回调接口privateintmTimeCount;//时间计数privateFile mVecordFile =null;//文件privateintmWidth = 0;//视频分辨率宽度privateintmHeight = 0;//视频分辨率高度privatebooleanisStarting =false;
    @OverrideprotectedvoidonCreate(Bundle savedInstanceState) {
        requestWindowFeature(Window.FEATURE_NO_TITLE);//去掉标题栏getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);//设置全屏super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_record_video);
        initView();
    }privatevoidinitView() {
        mProgressbar=(ProgressBar) findViewById(R.id.progressBar);
        mProgressbar.setMax(mRecordMaxTime);
        findViewById(R.id.btn_start).setOnClickListener(this);
        mSurfaceView=(SurfaceView)findViewById(R.id.surfaceView);
        mSurfaceHolder= mSurfaceView.getHolder();//取得holdermSurfaceHolder.addCallback(this);//holder加入回调接口mSurfaceHolder.setKeepScreenOn(true);
    }/*** 初始化摄像头*/privatevoidinitCamera(intwidth,intheight) {if(mCamera !=null) {
            freeCameraResource();
        }try{
            mCamera=Camera.open();if(mCamera ==null)return;
            mCamera.setDisplayOrientation(90);//摄像头默认是横向，需要调整角度变成竖直mCamera.setPreviewDisplay(mSurfaceHolder);
            Camera.Parameters parameters= mCamera.getParameters();//获得相机参数mWidth =width;
            mHeight=height;
            parameters.setPreviewSize(height, width);//设置预览图像大小parameters.set("orientation", "portrait");
            List<String> focusModes =parameters.getSupportedFocusModes();if(focusModes.contains("continuous-video")) {
                parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
            }
            mCamera.getParameters().getSupportedPreviewSizes();
            mCamera.getParameters().getSupportedPictureSizes();
            mCamera.setParameters(parameters);//设置相机参数mCamera.startPreview();//开始预览mCamera.unlock();//解锁，赋予录像权限}catch(Exception e) {
            e.printStackTrace();
            freeCameraResource();
        }
    }
    @OverrideprotectedvoidonDestroy() {super.onDestroy();
        stop();
    }/*** 释放摄像头资源*/privatevoidfreeCameraResource() {if(mCamera !=null) {
            mCamera.setPreviewCallback(null);
            mCamera.stopPreview();
            mCamera.lock();
            mCamera.release();
            mCamera=null;
        }
    }/*** 录制前，初始化*/privatevoidinitRecord() {try{
            mMediaRecorder=newMediaRecorder();
            mMediaRecorder.reset();if(mCamera !=null)
                mMediaRecorder.setCamera(mCamera);
            mMediaRecorder.setOnErrorListener(this);
            mMediaRecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);//视频源CamcorderProfile mProfile=null;if(CamcorderProfile.hasProfile(CamcorderProfile.QUALITY_1080P)) {
                mProfile=CamcorderProfile.get(CamcorderProfile.QUALITY_1080P);
            }elseif(CamcorderProfile.hasProfile(CamcorderProfile.QUALITY_720P)){
                mProfile=CamcorderProfile.get(CamcorderProfile.QUALITY_720P);
            }else{
                mProfile=CamcorderProfile.get(CamcorderProfile.QUALITY_480P);
            }//mMediaRecorder.setProfile(mProfile);//不知道为什么，直接这么设置，不起作用，要下面set 才行mMediaRecorder.setOutputFormat(mProfile.fileFormat);//视频输出格式mMediaRecorder.setVideoFrameRate(mProfile.fileFormat);//设置录制的视频帧率mMediaRecorder.setVideoSize(mHeight, mWidth);;//设置分辨率：mMediaRecorder.setVideoEncodingBitRate(mProfile.videoBitRate);//设置帧频率，然后就清晰了mMediaRecorder.setVideoEncoder(mProfile.videoCodec);//视频录制格式//}mMediaRecorder.setOutputFile(mVecordFile.getAbsolutePath());
            mMediaRecorder.prepare();
            mMediaRecorder.start();
        }catch(Exception e) {
            e.printStackTrace();
        }
    }/*** 开始录制视频*/publicvoidstartRecord(finalOnRecordFinishListener onRecordFinishListener) {
        isStarting=true;this.mOnRecordFinishListener =onRecordFinishListener;
        createRecordDir();try{
            initRecord();
            mTimeCount= 0;//时间计数器重新赋值mTimer =newTimer();
            mTimer.schedule(newTimerTask() {
                @Overridepublicvoidrun() {//TODO Auto-generated method stubmTimeCount++;
                    mProgressbar.setProgress(mTimeCount);//设置进度条if(mTimeCount == mRecordMaxTime) {//达到指定时间，停止拍摄stop();if(mOnRecordFinishListener !=null)
                            mOnRecordFinishListener.onRecordFinish();
                    }
                }
            },0, 1000);
        }catch(Exception e) {
            e.printStackTrace();
        }
    }/*** 停止拍摄*/publicvoidstop() {
        stopRecord();
        releaseRecord();
        freeCameraResource();
    }/*** 停止录制*/publicvoidstopRecord() {
        mProgressbar.setProgress(0);if(mTimer !=null)
            mTimer.cancel();if(mMediaRecorder !=null) {//设置后不会崩mMediaRecorder.setOnErrorListener(null);
            mMediaRecorder.setPreviewDisplay(null);try{
                mMediaRecorder.stop();
            }catch(IllegalStateException e) {
                e.printStackTrace();
            }catch(RuntimeException e) {
                e.printStackTrace();
            }catch(Exception e) {
                e.printStackTrace();
            }
        }
    }/*** 释放资源*/privatevoidreleaseRecord() {if(mMediaRecorder !=null) {
            mMediaRecorder.setOnErrorListener(null);try{
                mMediaRecorder.release();
            }catch(IllegalStateException e) {
                e.printStackTrace();
            }catch(Exception e) {
                e.printStackTrace();
            }
        }
        mMediaRecorder=null;
    }/*** 创建目录与文件*/privatevoidcreateRecordDir() {
        File FileDir=newFile(Environment.getExternalStorageDirectory() + File.separator+"RecordVideo/");if(!FileDir.exists()) {
            FileDir.mkdirs();
        }//创建文件try{
            mVecordFile=newFile(FileDir.getAbsolutePath()+"/test.mp4");
            Log.d("Path:", mVecordFile.getAbsolutePath());
        }catch(Exception e) {
            e.printStackTrace();
        }
    }
    OnRecordFinishListener recordFinishListener=newOnRecordFinishListener() {
        @OverridepublicvoidonRecordFinish() {
            Intent intent=newIntent(RecordVideoActivity.this, MainActivity.class);
            intent.putExtra("filePath", mVecordFile.getAbsolutePath());
            setResult(Activity.RESULT_OK, intent);
            finish();
        }
    };
    @OverridepublicvoidonBackPressed() {if(!isStarting){
           finish();
       }
    }
    @OverridepublicvoidonClick(View v) {switch(v.getId()){caseR.id.btn_start:if(!isStarting)
                   startRecord(recordFinishListener);break;
        }
    }
    @OverridepublicvoidsurfaceCreated(SurfaceHolder holder) {
    }
    @OverridepublicvoidsurfaceChanged(SurfaceHolder holder,intformat,intwidth,intheight) {
        initCamera(width,height);
    }
    @OverridepublicvoidsurfaceDestroyed(SurfaceHolder holder) {
        freeCameraResource();
    }
    @OverridepublicvoidonError(MediaRecorder mr,intwhat,intextra) {try{if(mr !=null)
                mr.reset();
        }catch(IllegalStateException e) {
            e.printStackTrace();
        }catch(Exception e) {
            e.printStackTrace();
        }
    }/*** 录制完成回调接口*/publicinterfaceOnRecordFinishListener {voidonRecordFinish();
    }
}
最后提供源代码：[https://github.com/xiaoxiaoqingyi/3DShow](https://github.com/xiaoxiaoqingyi/3DShow)






