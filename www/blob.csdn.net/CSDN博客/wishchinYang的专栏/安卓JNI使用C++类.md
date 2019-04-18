# 安卓JNI使用C++类 - wishchinYang的专栏 - CSDN博客
2015年10月12日 15:02:51[wishchin](https://me.csdn.net/wishchin)阅读数：1488
       安卓JNI使用C++类，同时可使用C++的类成员变量，这就必须保证程序持续保存Native状态，即长期维护C++对象的地址。完成初始化之后，需要使用对象成员的java层函数需要传入对象的地址。
**一、Native类-对象的初始化**
1. 一个Java类的数据成员
```java
private FeelJulyGpsMap  mNativeDetector;//分类器声明！用于标识使用C++对象地址
    //摄像机参数：用于缩放！
    CamParam                   mCamParam =null;
    //JNI2.用于识别的JNI代码！  
    private long mNativeObj = 0;
    //用于追踪测试！
    public long mNativeObjCvT = 0;
```
2. 使用的Native库
```java
//JNI1.使用JNI的C++库
	static {
		System.loadLibrary("feelJulyGpsMap");
	}
```
3.使用的JNI函数：
```java
//2.JNI函数！
	//四、追踪函数-OpenCV！wishchin！！！
	private static native long trackInitJni(String cascadeName);
```
4.Java初始化函数
```java
//1.主要处理函数！
	public int trackInit()
	{
		mCamParam = new CamParam();//初始化摄像机参数！wishchin!!!
		String cascadeName ="";
		mNativeObjCvT   = trackInitJni(cascadeName);//长期维持mNativeObjCvT 值，作为对象的地址
		return 1;
	}
```
5.java使用的函数
```java
mTrack6Dof = new CTrack6Dof();
       if(null!= mTrack6Dof){
              mTrack6Dof.trackInit();//初始化参数！参数对象全局变量已封装！
        }
```
上述过程完成native对象的初始化，java层长期保持 mNativeObjCvT 的值作为JNI层的传入地址。
**二、Native类-对象成员的使用**
1. 在java层，可以传入地址使用C++层的对象的数据和函数
```java
@SuppressLint("SdCardPath")
    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();
        //0.检测面部的原始代码！//Detect 面部的代码，使用C++！wishchin！
        //detectFace( mGray, mRgba);
        
        //JNI//wishchin !
        if (mTrack6Dof.mNativeObjCvT != 0 && mTrack6Dof.mCamParam!=null){
        	//1.测试追踪！
                 mTrack6Dof.trackOneFrame( M );//trackInit();
       }
```
2. 所使用的JNI接口函数的函数
```java
//主要处理函数，追踪一帧图像！
	@SuppressLint("SdCardPath")
	public  int trackOneFrame( Mat frame )
	{
		Mat frameRf= frame.clone();
		Mat rvec = new Mat();rvec =Mat.zeros(3,1,CvType.CV_64FC1);
		Mat tvec = new Mat();tvec =Mat.zeros(3,1,CvType.CV_64FC1);
		jniTrackProject(
				mNativeObjCvT,
				frame.getNativeObjAddr(),
				mCamParam.intrinsic_Mat.getNativeObjAddr(),
				mCamParam.disCoeff.getNativeObjAddr(),
				mCamParam.object_pointsM.getNativeObjAddr() 
				,(mCamParam.axisM).getNativeObjAddr()
				,frameRf.getNativeObjAddr() 
				,rvec.getNativeObjAddr()
				,tvec.getNativeObjAddr()
				);
      }
```
所使用的 mNativeObjCvT, 值为对象的C++层地址。
3. trackProject()的函数原型：
```java
//取代trackJniRf的输出，多输出几个参数！
	//把画图的投影点带出JNI层！
	private static native int jniTrackProject(
			long thiz,
			long frame
			,long intrinsic_Mat, long disCoeff, long object_pointsM , long axisM 
			,long frameRf
			,long rvec,long  tvec //The next Input! The Draw OutPut!
			);
```
使用 long thiz, 这个参数代表对象的地址参数输入....
上述过程完成对本地C++对象的数据成员进行更新，并取成员值，完成第本地对象的使用。
