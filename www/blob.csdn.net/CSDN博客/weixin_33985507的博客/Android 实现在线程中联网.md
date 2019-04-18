# Android 实现在线程中联网 - weixin_33985507的博客 - CSDN博客
2015年10月31日 10:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
其实我们要牢记的是，对数据流的操作都是阻塞的，在一般情况下，我们是不需要考虑这个问题的，但是在Android 实现联网的时候，我们必须考虑到这个问题。比如：从网络上下载一张图片：
Java代码：
- 
public Bitmap returnBitmap(String url)
- 
{
- 
URL myFileUrl = null;
- 
Bitmap bitmap = null;
- 
try{
- 
myFileUrl = new URL(url);
- 
}catch(MalformedURLException e){
- 
e.printStackTrace();
- 
return null;
- 
};
- 
try{
- 
HttpURLConnection conn = (HttpURLConnection)myFileUrl.openConnection();
- 
conn.setDoInput(true);
- 
conn.connect();
- 
InputStream is = conn.getInputStream();
- 
bitmap = BitmapFactroy.decodeStream(is);
- 
is.close();
- 
}catch(IOException e){
- 
e.printStackTrace();
- 
}
- 
return bitmap;
- 
}
复制代码
 
             
   由于网络连接需要很长的时间，需要3-5秒，甚至更长的时间才能返回页面的内容。如果此连接动作直接在主线程，也就是UI线程中处理，会发生什么情
况呢？ 整个程序处于等待状态，界面似乎是“死”掉了。为了解决这个问题，必须把这个任务放置到单独线程中运行，避免阻塞UI线程，这样就不会对主线程有任何影
 响。举个例子如下：
Java代码：
- 
private void connect(String strURL){
- 
new Thread() {
- 
public void run() {
- 
try {
- 
HttpClient client = new DefaultHttpClient();
- 
// params[0]代表连接的url
- 
HttpGet get = new HttpGet(url.getText().toString());
- 
HttpResponse response = client.execute(get);
- 
HttpEntity entity = response.getEntity();
- 
long length = entity.getContentLength();
- 
InputStream is = entity.getContent();
- 
String s = null;
- 
if (is != null) {
- 
ByteArrayOutputStream baos = new ByteArrayOutputStream();
- 
byte[] buf = new byte[128];
- 
int ch = -1;
- 
int count = 0;
- 
while ((ch = is.read(buf)) != -1) {
- 
baos.write(buf, 0, ch);
- 
count += ch;
- 
}
- 
s = new String(baos.toByteArray());
- 
Log.V(“moandroid sample”,s);
- 
}
- 
} catch (Exception e) {
- 
e.printStackTrace();
- 
}
- 
}
- 
}.start();
- 
}
复制代码
使用Handler更新界面
                 如何将下载的信息显示在界面上了，比如说下载的进度。Android
 SDK平台只允许在主线程中调用相关View的方法来更新界面。如果返回结果在新线程中获得，那么必须借助Handler来更新界面。为此，在界面 Activity中创建一个Handler对象，并在handleMessage()中更新UI。
Java代码：
- 
//Task在另外的线程执行，不能直接在Task中更新UI，因此创建了Handler
- 
private Handler handler = new Handler() {
- 
@Override
- 
public void handleMessage(Message msg) {
- 
String m = (String) msg.obj;
- 
message.setText(m);
- 
}
- 
}; 
- 
- 
//只需要将上面的
- 
Log.V(“moandroid sample”,s); 
- 
- 
//替换为：
- 
s = new String(baos.toByteArray());
- 
Message mg = Message.obtain();
- 
mg.obj = s;
- 
handler.sendMessage(mg);
