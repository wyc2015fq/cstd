# android之人脸检测FaceDetector||人体？ - 三少GG - CSDN博客
2012年01月20日 00:36:29[三少GG](https://me.csdn.net/scut1135)阅读数：1422
1. [http://blog.csdn.net/shaojie519/article/details/6729333](http://blog.csdn.net/shaojie519/article/details/6729333)
FaceDetector检测到的并不是人的全脸，而只是双眼。
实际测试中，发现图片太小的话检测不到人脸，试验中使用小于100x100的图片检测不到人脸，但是由于Android内存有限，图片太大的话，会出现无法加载图片的异常。图片的格式必须为Bitmap RGB565格式。
文档  findFaces(Bitmap bitmap, Face[] faces)方法中有说明。
2.  [http://www.anddev.org/quick_and_easy_facedetector_demo-t3856.html](http://www.anddev.org/quick_and_easy_facedetector_demo-t3856.html)
Using
 Android's FaceDetector
 Tutorial: Simple implementation of the Android SDK's FaceDetector and FaceDetector.Face classes
**3.**[http://www.ways2u.com/?post=292](http://www.ways2u.com/?post=292)
如果你是做截图软件的话，估计利用人脸识别可以做得更人性化，如果你你是做客户端的话，上传头像截图也是可以好好利用的！！项目源码见附件！！
  if (faceBitmap != null) {
                      FaceDetector detector = new FaceDetector(faceBitmap.getWidth(),
                                         faceBitmap.getHeight(), mFaces.length); // 创建识别器
                      mNumFaces = detector.findFaces(faceBitmap, mFaces);    // 识别
                      if (mNumFaces > 0) {
                                for (int i = 0; i < mNumFaces; i++) {
                                         handleFace(mFaces[i]);        // 调用函数对人脸画面进行处理
                                }
                      }
             }
