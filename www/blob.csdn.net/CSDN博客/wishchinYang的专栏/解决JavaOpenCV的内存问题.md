# 解决JavaOpenCV的内存问题 - wishchinYang的专栏 - CSDN博客
2015年10月13日 18:46:31[wishchin](https://me.csdn.net/wishchin)阅读数：1405
         在使用OpenCV时，程序总是在某个时间墨明棋妙地终止，打开CygWin ，输入 adb logcat ，查看打印的信息，发现是内存问题。经过反复的查找，发现使用OpenCV的java类库时，一定要慎重使用New Mat()。
        内存溢出的原因： 在循环中使用Java层的OpenCV，本质都需要深入到Native层。在使用New Mat()，进行初始化时，默认使用了C++的new()函数，但并没有同时使用Delete()，并且也无法确定初始化对象的作用域，导致大量的New()没有Delete()，造成内存溢出。
       经过修改后的处理帧的函数是这样的：
```java
@SuppressLint("SdCardPath")
    public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		if(!inited)
			return inputFrame.rgba();//初始化OpenCV
		
        mRgba = inputFrame.rgba();
        mGray = inputFrame.gray();
        if (mTrack6Dof.mNativeObjCvT != 0 && mTrack6Dof.mCamParam!=null){
        	if(mRgba.rows()>0 ){
        		
            	//经过一次分辨率转换！
            	Imgproc.resize(mRgba, m_Mi, mRgba.size());
            	Imgproc.cvtColor( mRgba,m_Mi, Imgproc.COLOR_BGRA2RGB);
            	
            	//确认是否调用原始视频！
            	if(IsVideo == true){
                        //1.获得红色追踪面的重心
            		getSeqCentroid(m_Mi, mRgba);
            		
            	}
            	else{
            		
                	if(G_Counter <1000 ){
                		
                		String filename ="/sdcard/DevCv/CircleBall/";
                		filename +="Image" + G_Counter + ".png";
                		
                		Mat M =Highgui.imread(filename);
                		
                		//2.测试序列追踪
                		//代码无修改，测试追踪没有响应代码，暂时不可行
                		if( M.rows()>0 ){
                			
                			//3.测试球追踪
                			refineYSeq( M,  mRgba);
                    		
                		}
                		
                		G_Counter+=1;
                	}
                	else
                	{
                		IsVideo = false;
                	}
            	}
        	}
        }
        
        return mRgba;
    }
```
所使用的函数：
```java
public int refineYSeq(Mat M, Mat mRgba)
	{
		Point3 Centroid = new Point3();
		Centroid.x =0;
		
		Pattern.refineSeqJar(M,Centroid);
		
		if(Centroid.x !=0){
			Rect RCen = new Rect();
    		RCen.height = 4;
    		RCen.width  = 4;
    		RCen.x = (int) ( Centroid.x );
    		RCen.y = (int) ( Centroid.y );
    		
    		Point Center = new Point();
    		Center.x = (int) ( Centroid.x );
    		Center.y = (int) ( Centroid.y );
    		RectMyMat(M,  Center, (float)4.0, FACE_RECT_COLOR ,2);
		}
		
		//Mat MT= new Mat( mRgba.rows(),mRgba.cols(),CvType.CV_8UC3);
		Imgproc.resize(  M, m_MT, mRgba.size() );
		Imgproc.cvtColor(m_MT, mRgba, Imgproc.COLOR_RGB2BGRA);
		
		return 1;
	}
```
所使用的全局变量：
```java
//初始化OpenCV载入状态和循环中使用的mat，把循环内的mat转化为全局，不在局部new mat型
        boolean inited = false;
	
	Mat m_Mi    = null;
	Mat m_edges = null;
	Mat m_MT    = null;
```
初始化OpenCV和局部变量：
//初始化OpenCV的callback函数：
```java
private BaseLoaderCallback  mLoaderCallback = new BaseLoaderCallback(this) {
        @Override
        public void onManagerConnected(int status) {
            switch (status) {
                case LoaderCallbackInterface.SUCCESS:
                {
                    Log.i(TAG, "OpenCV loaded successfully");      	
                	//System.loadLibrary("feelJulyGpsMap");// Load native library after(!) OpenCV initialization
                    try {
                    	
                    	{
                    		m_Mi    = new Mat(540,960,CvType.CV_8UC3);
                    		m_edges = new Mat(540,960,CvType.CV_8UC1);
                    		m_MT    = new Mat(540,960,CvType.CV_8UC3);
                    		
                    		Pattern   = new CMovePattern();
                    		PatternRf = new CMovePatternRf();
                    	}
                    } catch (IOException e) {
                    	e.printStackTrace();
                    	Log.e(TAG, "Failed to load cascade. Exception thrown: " + e);
                    }
                    //尝试几种方法，暂时不改正摄像机角度问题了！
                    //camera.setDisplayOrientation(90); 
                    //mOpenCvCameraView.setRotationX(15);//立体旋转，旋转90度就成一条线了！wishchin！
                    //mOpenCvCameraView.surfaceCreated(surfaceHolder Holder);
                    mOpenCvCameraView.enableView();
                  
                } break;
                default:
                {
                	super.onManagerConnected(status);
                } break;
            }
        }
    };
```
总结：
       避免使用OpenCV造成内存溢出的方法是：
             把循环中需要初始化的变量移到全局，避免大量的New Mat() 出现；
             1. 初始化为null;
             2.初始化OpenCV后，再初始化Mat的大小；
             3.在循环中使用全局Mat型，避免new 或者初始化！
