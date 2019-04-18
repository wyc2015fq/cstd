# 安卓使用ImageView显示OpenCV-Mat - wishchinYang的专栏 - CSDN博客
2015年07月30日 09:38:11[wishchin](https://me.csdn.net/wishchin)阅读数：3796
         Android 的Application—native调试暂时还只能进行主线程调试，在GDB里面，运行于子线程的OpenCv数据处理过程不能直接调试，OPenCV-Native函数    public Mat onCameraFrame(CvCameraViewFrame inputframe){} 是运行于子线程的，暂时还找不到调试的方法。并且，自己编写的子函数若是嵌入onCameraFrame()函数，也不能进行Native调试，因此是一个令人忧桑的开发过程。
        对OpenCvMat 型进行测试，C++的Highgui显示函数是无效的，因为窗口不能直接从Android界面弹出。需要使用imageview进行显示以便查看图像处理的结果，因此我借鉴了少许代码：
```java
@SuppressLint("SdCardPath")
private int trackTestCv()
{
        String filename ="/sdcard/DevCv/Picture/";
	 filename += 1 + ".jpg";
    	Mat frame =Highgui.imread(filename);
    	Mat frameRf = new Mat(frame.rows(),frame.cols(),CvType.CV_8UC3);
        Bitmap bmp= null;
        bmp =Bitmap.createBitmap( frame.width(),  frame.height(),  Bitmap.Config.ARGB_8888);
        covMat2bm(frameRf,bmp);
        
        mImageView.setImageBitmap( bmp );
        mImageView.invalidate();
        mImageView.setVisibility(View.VISIBLE);
}
private int covMat2bm(Mat mat,Bitmap bm)
{
   Utils.matToBitmap(mat, bm);
   return 1;
}
```
函数解释：
```java
mImageView
```
为一个imageview控件，使用前先获取ID
注意事项：事件的更新是遵循事件响应进行的，在一个写死的循环里对imageview进行更新是不改变显示效果的
