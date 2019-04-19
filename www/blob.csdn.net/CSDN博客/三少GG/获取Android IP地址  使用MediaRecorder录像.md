# 获取Android IP地址 || 使用MediaRecorder录像 - 三少GG - CSDN博客
2012年03月21日 18:14:43[三少GG](https://me.csdn.net/scut1135)阅读数：1549标签：[android																[layout																[button																[listview																[file																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
参考前人研究成果，非个人创作
1.使用WIFI
首先设置用户权限
<uses-permission android:name="android.permission.ACCESS_WIFI_STATE"></uses-permission>
<uses-permission android:name="android.permission.CHANGE_WIFI_STATE"></uses-permission>
<uses-permission android:name="android.permission.WAKE_LOCK"></uses-permission>
其次，代码如下
```java
public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        //获取wifi服务
        WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
        //判断wifi是否开启
        if (!wifiManager.isWifiEnabled()) { 
        wifiManager.setWifiEnabled(true);   
        } 
        WifiInfo wifiInfo = wifiManager.getConnectionInfo();      
        int ipAddress = wifiInfo.getIpAddress();  
        String ip = intToIp(ipAddress);  
        EditText et = (EditText)findViewById(R.id.EditText01);
        et.setText(ip);
    }    
    private String intToIp(int i) {      
        
          return (i & 0xFF ) + "." +      
        ((i >> 8 ) & 0xFF) + "." +      
        ((i >> 16 ) & 0xFF) + "." +      
        ( i >> 24 & 0xFF) ;
     }
```
2.使用GPRS
首先，设置用户上网权限
<uses-permission android:name="android.permission.INTERNET"></uses-permission>
其次，代码如下
```java
public String getLocalIpAddress() 
    { 
        try 
        { 
            for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) 
            { 
               NetworkInterface intf = en.nextElement(); 
               for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) 
               { 
                   InetAddress inetAddress = enumIpAddr.nextElement(); 
                   if (!inetAddress.isLoopbackAddress()) 
                   { 
                       return inetAddress.getHostAddress().toString(); 
                   } 
               } 
           } 
        } 
        catch (SocketException ex) 
        { 
            Log.e("WifiPreference IpAddress", ex.toString()); 
        } 
        return null; 
    }
```
[Android 中使用MediaRecorder进行录像详解（视频录制）](http://www.it619.net/?p=711)
2012年01月04日⁄ [安卓之JAVA](http://www.it619.net/category/android/%e5%ae%89%e5%8d%93%e4%b9%8bjava), [安卓之系统开发](http://www.it619.net/category/android/%e5%ae%89%e5%8d%93%e4%b9%8b%e7%b3%bb%e7%bb%9f%e5%bc%80%e5%8f%91), [安卓精华汇总](http://www.it619.net/category/android/%e5%ae%89%e5%8d%93%e7%b2%be%e5%8d%8e%e6%b1%87%e6%80%bb)⁄ [暂无评论](http://www.it619.net/711.html#respond) ⁄
 被围观 185 次+
在这里给出自己的一个测试DEMO，里面注释很详细。简单的视频录制功能.
- package com.video;
- 
- import java.io.IOException;
- 
- import android.app.Activity;
- import android.content.pm.ActivityInfo;
- import android.graphics.PixelFormat;
- import android.media.MediaRecorder;
- import android.os.Bundle;
- import android.view.SurfaceHolder;
- import android.view.SurfaceView;
- import android.view.View;
- import android.view.View.OnClickListener;
- import android.view.Window;
- import android.view.WindowManager;
- import android.widget.Button;
- 
- /**
- *
 class name：TestBasicVideo<BR>
- *
 class description：一个简单的录制视频例子<BR>
- *
 PS：实现基本的录制保存文件 <BR>
- *
- *
 @version 1.00 2011/09/21
- *
 @author CODYY)peijiangping
- */
- publicclass TestBasicVideo extends Activity implements SurfaceHolder.Callback
 {
- private Button
 start;//
 开始录制按钮
- private Button
 stop;//
 停止录制按钮
- private MediaRecorder
 mediarecorder;//
 录制视频的类
- private SurfaceView
 surfaceview;//
 显示视频的控件
- //
 用来显示视频的一个接口，我靠不用还不行，也就是说用mediarecorder录制视频还得给个界面看
- //
 想偷偷录视频的同学可以考虑别的办法。。嗯需要实现这个接口的Callback接口
- private SurfaceHolder
 surfaceHolder;
- 
- publicvoid onCreate(Bundle
 savedInstanceState) {
- super.onCreate(savedInstanceState);
- requestWindowFeature(Window.FEATURE_NO_TITLE);//
 去掉标题栏
- getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
- WindowManager.LayoutParams.FLAG_FULLSCREEN);//
 设置全屏
- //
 设置横屏显示
- setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
- //
 选择支持半透明模式,在有surfaceview的activity中使用。
- getWindow().setFormat(PixelFormat.TRANSLUCENT);
- setContentView(R.layout.main);
- init();
- }
- 
- privatevoid init()
 {
- start
 = (Button) this.findViewById(R.id.start);
- stop
 = (Button) this.findViewById(R.id.stop);
- start.setOnClickListener(new TestVideoListener());
- stop.setOnClickListener(new TestVideoListener());
- surfaceview
 = (SurfaceView) this.findViewById(R.id.surfaceview);
- SurfaceHolder
 holder = surfaceview.getHolder();//
 取得holder
- holder.addCallback(this); //
 holder加入回调接口
- //
 setType必须设置，要不出错.
- holder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
- }
- 
- class TestVideoListener implements OnClickListener
 {
- 
- @Override
- publicvoid onClick(View
 v) {
- if (v
 == start) {
- mediarecorder
 = new MediaRecorder();//
 创建mediarecorder对象
- //
 设置录制视频源为Camera(相机)
- mediarecorder.setVideoSource(MediaRecorder.VideoSource.CAMERA);
- //
 设置录制完成后视频的封装格式THREE_GPP为3gp.MPEG_4为mp4
- mediarecorder
- .setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
- //
 设置录制的视频编码h263 h264
- mediarecorder.setVideoEncoder(MediaRecorder.VideoEncoder.H264);
- //
 设置视频录制的分辨率。必须放在设置编码和格式的后面，否则报错
- mediarecorder.setVideoSize(176, 144);
- //
 设置录制的视频帧率。必须放在设置编码和格式的后面，否则报错
- mediarecorder.setVideoFrameRate(20);
- mediarecorder.setPreviewDisplay(surfaceHolder.getSurface());
- //
 设置视频文件输出的路径
- mediarecorder.setOutputFile("/sdcard/love.3gp");
- try {
- //
 准备录制
- mediarecorder.prepare();
- //
 开始录制
- mediarecorder.start();
- } catch (IllegalStateException
 e) {
- //
 TODO Auto-generated catch block
- e.printStackTrace();
- } catch (IOException
 e) {
- //
 TODO Auto-generated catch block
- e.printStackTrace();
- }
- }
- if (v
 == stop) {
- if (mediarecorder
 != null)
 {
- //
 停止录制
- mediarecorder.stop();
- //
 释放资源
- mediarecorder.release();
- mediarecorder
 = null;
- }
- }
- 
- }
- 
- }
- 
- @Override
- publicvoid surfaceChanged(SurfaceHolder
 holder, int format, int width,
- int height)
 {
- //
 将holder，这个holder为开始在oncreat里面取得的holder，将它赋给surfaceHolder
- surfaceHolder
 = holder;
- }
- 
- @Override
- publicvoid surfaceCreated(SurfaceHolder
 holder) {
- //
 将holder，这个holder为开始在oncreat里面取得的holder，将它赋给surfaceHolder
- surfaceHolder
 = holder;
- }
- 
- @Override
- publicvoid surfaceDestroyed(SurfaceHolder
 holder) {
- //
 surfaceDestroyed的时候同时对象设置为null
- surfaceview
 = null;
- surfaceHolder
 = null;
- mediarecorder
 = null;
- }
- }
main.xml
- <LinearLayoutxmlns:android="http://schemas.android.com/apk/res/android"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:orientation="horizontal">
- 
- <LinearLayout
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:layout_weight="1">
- 
- <SurfaceView
- android:id="@+id/surfaceview"
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"/>
- </LinearLayout>
- 
- <LinearLayout
- android:layout_width="fill_parent"
- android:layout_height="fill_parent"
- android:layout_weight="4"
- android:gravity="center"
- android:orientation="vertical">
- 
- <Button
- android:id="@+id/start"
- android:layout_width="fill_parent"
- android:layout_height="wrap_content"
- android:layout_weight="1"
- android:text="Start"/>
- 
- <Button
- android:id="@+id/stop"
- android:layout_width="fill_parent"
- android:layout_height="wrap_content"
- android:layout_weight="1"
- android:text="Stop"/>
- </LinearLayout>
- 
- </LinearLayout>
AndroidManifest.xml
- <?xmlversion="1.0"encoding="utf-8"?>
- <manifestxmlns:android="http://schemas.android.com/apk/res/android"
- package="com.video"
- android:versionCode="1"
- android:versionName="1.0">
- 
- <application
- android:icon="@drawable/icon"
- android:label="@string/app_name">
- <activity
- android:label="@string/app_name"
- android:name=".TestBasicVideo">
- <intent-filter>
- <actionandroid:name="android.intent.action.MAIN"/>
- 
- <categoryandroid:name="android.intent.category.LAUNCHER"/>
- </intent-filter>
- </activity>
- </application>
- 
- <uses-sdkandroid:minSdkVersion="8"/>
- 
- <uses-permissionandroid:name="android.permission.CAMERA">
- </uses-permission>
- <uses-permissionandroid:name="android.permission.RECORD_AUDIO">
- </uses-permission>
- <uses-permissionandroid:name="android.permission.WRITE_EXTERNAL_STORAGE">
- </uses-permission>
- 
- </manifest>
# [【Android學習專題】多媒體篇：MediaRecorder 实现录音机](http://www.cnblogs.com/skyseraph/archive/2012/03/08/2384359.html)
## 【mediarecorder】
mediarecorder_state_diagram： Initial
 》Initialed 》DataSourceConfiged 》Prepared 》Recording 》Released 》 Error
![](http://pic002.cnblogs.com/images/2012/137896/2012030812201559.gif)
## 录音界面
![](http://pic002.cnblogs.com/images/2012/137896/2012030812183317.png)
## 播放界面
![](http://pic002.cnblogs.com/images/2012/137896/2012030812184845.png)
## 开始录音（步骤）
![](http://pic002.cnblogs.com/images/2012/137896/2012030812213379.png)
## 停止录音（步骤）
![](http://pic002.cnblogs.com/images/2012/137896/2012030812214322.png)
## 播放实现（步骤）
1 检测SD卡存在，并更新SD卡中已存在的文件.
![](http://pic002.cnblogs.com/images/2012/137896/2012030812244789.png)
![](http://pic002.cnblogs.com/images/2012/137896/2012030812260790.png)
2 点击列表时，播放被点击的文件
![](http://pic002.cnblogs.com/images/2012/137896/2012030812261933.png)
3 播放录音文件
![](http://pic002.cnblogs.com/images/2012/137896/2012030812262915.png)
## 完整源码
JAVA源码
```
1 public class mediarecorder1 extends ListActivity
  2 {
  3     private Button mAudioStartBtn;
  4     private Button mAudioStopBtn;
  5     private File mRecAudioFile;        // 录制的音频文件
  6     private File mRecAudioPath;        // 录制的音频文件路徑
  7     private MediaRecorder mMediaRecorder;// MediaRecorder对象
  8     private List<String> mMusicList = new ArrayList<String>();// 录音文件列表
  9     private String strTempFile = "recaudio_";// 零时文件的前缀
 10 
 11     @Override
 12     protected void onCreate(Bundle savedInstanceState)
 13     {
 14         // TODO Auto-generated method stub
 15         super.onCreate(savedInstanceState);
 16         setContentView(R.layout.mymultimedia_mediarecorder1);
 17 
 18         mAudioStartBtn = (Button) findViewById(R.id.mediarecorder1_AudioStartBtn);
 19         mAudioStopBtn = (Button) findViewById(R.id.mediarecorder1_AudioStopBtn);
 20         
 21         /*按钮状态*/
 22         mAudioStartBtn.setEnabled(true);
 23         mAudioStopBtn.setEnabled(false);
 24         
 25         /* 检测是否存在SD卡 */
 26         if (Environment.getExternalStorageState().equals(android.os.Environment.MEDIA_MOUNTED))
 27         {
 28             mRecAudioPath = Environment.getExternalStorageDirectory();// 得到SD卡得路径
 29             musicList();// 更新所有录音文件到List中
 30         } else
 31         {
 32             Toast.makeText(mediarecorder1.this, "没有SD卡", Toast.LENGTH_LONG).show();
 33         }
 34         
 35         /* 开始按钮事件监听 */
 36         mAudioStartBtn.setOnClickListener(new Button.OnClickListener()    
 37         {
 38             @Override
 39             public void onClick(View arg0)
 40             {
 41                 try
 42                 {                    
 43                     /* ①Initial：实例化MediaRecorder对象 */
 44                     mMediaRecorder = new MediaRecorder();
 45                     /* ②setAudioSource/setVedioSource*/
 46                     mMediaRecorder.setAudioSource(MediaRecorder.AudioSource.MIC);//设置麦克风
 47                     /* ②设置输出文件的格式：THREE_GPP/MPEG-4/RAW_AMR/Default
 48                      * THREE_GPP(3gp格式，H263视频/ARM音频编码)、MPEG-4、RAW_AMR(只支持音频且音频编码要求为AMR_NB)
 49                      * */
 50                     mMediaRecorder.setOutputFormat(MediaRecorder.OutputFormat.DEFAULT);
 51                     /* ②设置音频文件的编码：AAC/AMR_NB/AMR_MB/Default */
 52                     mMediaRecorder.setAudioEncoder(MediaRecorder.AudioEncoder.DEFAULT);
 53                     /* ②设置输出文件的路径 */                    
 54                     try
 55                     {
 56                         mRecAudioFile = File.createTempFile(strTempFile, ".amr", mRecAudioPath);
 57                         
 58                     } catch (Exception e)
 59                     {
 60                         e.printStackTrace(); 
 61                     }
 62                     mMediaRecorder.setOutputFile(mRecAudioFile.getAbsolutePath());
 63                     /* ③准备 */
 64                     mMediaRecorder.prepare();
 65                     /* ④开始 */
 66                     mMediaRecorder.start();
 67                     /*按钮状态*/
 68                     mAudioStartBtn.setEnabled(false);
 69                     mAudioStopBtn.setEnabled(true);
 70                 } catch (IOException e)
 71                 {
 72                     e.printStackTrace();
 73                 }
 74             }
 75         });
 76         /* 停止按钮事件监听 */
 77         mAudioStopBtn.setOnClickListener(new Button.OnClickListener()
 78         {
 79             @Override
 80             public void onClick(View arg0)
 81             {
 82                 // TODO Auto-generated method stub
 83                 if (mRecAudioFile != null)
 84                 {
 85                     /* ⑤停止录音 */
 86                     mMediaRecorder.stop();
 87                     /* 将录音文件添加到List中 */
 88                     mMusicList.add(mRecAudioFile.getName());
 89                     ArrayAdapter<String> musicList = new ArrayAdapter<String>(mediarecorder1.this,
 90                             R.layout.list, mMusicList);
 91                     setListAdapter(musicList);
 92                     /* ⑥释放MediaRecorder */
 93                     mMediaRecorder.release();
 94                     mMediaRecorder = null;
 95                     /* 按钮状态 */
 96                     mAudioStartBtn.setEnabled(true);
 97                     mAudioStopBtn.setEnabled(false);
 98                 }
 99             }
100         });
101     }
102 
103     /* 播放录音文件 */
104     private void playMusic(File file)
105     {
106         Intent intent = new Intent();
107         intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
108         intent.setAction(android.content.Intent.ACTION_VIEW);
109         /* 设置文件类型 */
110         intent.setDataAndType(Uri.fromFile(file), "audio");
111         startActivity(intent);
112     }
113 
114     @Override
115     /* 当我们点击列表时，播放被点击的音乐 */
116     protected void onListItemClick(ListView l, View v, int position, long id)
117     {
118         /* 得到被点击的文件 */
119         File playfile = new File(mRecAudioPath.getAbsolutePath() + File.separator
120                 + mMusicList.get(position));
121         /* 播放 */
122         playMusic(playfile);
123     }
124 
125     /* 播放列表 */
126     public void musicList()
127     {
128         // 取得指定位置的文件设置显示到播放列表
129         File home = mRecAudioPath;
130         if (home.listFiles(new MusicFilter()).length > 0)
131         {
132             for (File file : home.listFiles(new MusicFilter()))
133             {
134                 mMusicList.add(file.getName());
135             }
136             ArrayAdapter<String> musicList = new ArrayAdapter<String>(mediarecorder1.this,
137                     R.layout.list, mMusicList);
138             setListAdapter(musicList);
139         }
140     }
141 }
142 
143 /* 过滤文件类型 */
144 class MusicFilter implements FilenameFilter
145 {
146     public boolean accept(File dir, String name)
147     {
148         return (name.endsWith(".amr"));
149     }
150 }
```
XML布局文件
```
1 <?xml version="1.0" encoding="utf-8"?>
 2 <AbsoluteLayout xmlns:android="http://schemas.android.com/apk/res/android"
 3     android:layout_width="fill_parent"
 4     android:layout_height="fill_parent"
 5     android:orientation="vertical" >    
 6      
 7     <ListView
 8 android:id="@id/android:list"
 9         android:layout_width="fill_parent"
10         android:layout_height="fill_parent"
11         android:layout_weight="1"
12         android:drawSelectorOnTop="false" />
13 
14     <Button
15 android:id="@+id/mediarecorder1_AudioStartBtn"
16         android:layout_width="170px"
17         android:layout_height="wrap_content"
18         android:layout_x="30px"
19         android:layout_y="100px"
20         android:text="开始录制音频" />
21 
22     <Button
23 android:id="@+id/mediarecorder1_AudioStopBtn"
24         android:layout_width="170px"
25         android:layout_height="wrap_content"
26         android:layout_x="210px"
27         android:layout_y="100px"
28         android:text="停止音频录制" /> 
29     
30 </AbsoluteLayout>
```

