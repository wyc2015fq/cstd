# 安卓多线程——AsyncTask - wishchinYang的专栏 - CSDN博客
2015年06月24日 15:41:27[wishchin](https://me.csdn.net/wishchin)阅读数：465
          在采集视频的同时需要对视频进行实时处理，因此要使用到多线程。
       AsyncTask是android提供的一个处理异步任务的框架，相当于Handler+Thread。相比而言，AsyncTask的优点是封装良好，代码简洁。
使用AsyncTask可以使你在后台执行耗时任务（doInBackground）并将结果反馈给UI线程（onPostExecute），方便UI线程更新界面，而不会阻塞UI线程。
         参考链接：http://www.netfoucs.com/article/RowandJJ/69121.html
使用的代码：
```java
//2. 开启一个线程进行实施处理帧
    /*自定义的IndentifyTask类，开启一个线程分析数据*/
    private class IndentifyTask extends AsyncTask<Void, Void, Void>{
        private byte[] mData;
        //构造函数
        IndentifyTask(byte[] data){
            this.mData = data;
        }
        
        @Override
        protected Void doInBackground(Void... params) {
            // TODO Auto-generated method stub
            //Size size = myCamera.getParameters().getPreviewSize(); //获取预览大小
            final int w = myCamera.getParameters().getPreviewSize().width;  //宽度
            final int h = myCamera.getParameters().getPreviewSize().height;
            final YuvImage image = new YuvImage(mData, ImageFormat.NV21, w, h, null);
            ByteArrayOutputStream os = new ByteArrayOutputStream(mData.length);
            if(!image.compressToJpeg(new Rect(0, 0, w, h), 100, os)){
                return null;
            }
            byte[] tmp = os.toByteArray();
            Bitmap bmp = BitmapFactory.decodeByteArray(tmp, 0,tmp.length); 
            //doSomethingNeeded(bmp);   //自己定义的实时分析预览帧视频的算法
            processBmp( bmp );//自己定义的实时分析预览帧视频的算法
          return null;
        }    
    }
```
```java
//1.建立一个全局变量
IndentifyTask m_IndentifyTask ;
```
//3.在处理帧函数里面添加处理逻辑
```java
@Override
public void onPreviewFrame(byte[] data, Camera arg1) {
	if(starttime  == 0) {
		starttime = System.currentTimeMillis();
			}
	long time = System.currentTimeMillis()-starttime;
	long current = time / 1000;
	Log.d("video", "video recorder =========================remains  "+current +"  "+(time % 1000));
	//在处理帧函数里面添加处理逻辑
	switch(m_IndentifyTask.getStatus()){
			            case RUNNING:
			                return;
			            case PENDING:
			            	m_IndentifyTask.cancel(false);
			                break;
						}
	m_IndentifyTask= new IndentifyTask(data);
	m_IndentifyTask.execute((Void)null);
						
}
```
