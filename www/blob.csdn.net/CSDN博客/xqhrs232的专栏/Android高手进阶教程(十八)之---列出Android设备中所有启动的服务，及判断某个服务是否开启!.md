# Android高手进阶教程(十八)之---列出Android设备中所有启动的服务，及判断某个服务是否开启! - xqhrs232的专栏 - CSDN博客
2012年05月08日 15:54:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：889标签：[android																[音乐																[service																[java																[手机](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=音乐&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/Android_Tutor/article/details/5824581](http://blog.csdn.net/Android_Tutor/article/details/5824581)
大家好，好久不见，今天在开发中遇到的一个问题给大家分享一下，我先前做了一个音乐播放器，因为播放音乐一般都是用服务来处理的。
当用户点击播放按钮时，音乐在服务中播放，然后用用户退出程序(服务进程还在，音乐还继续),所以妥用户点再次进入应用时，我们播放器要处于播放状态，这里我作了简单的处理，判断这个音乐服务是否开启来完成的。
今天给大家的小例子是列出Android设备中所有启动的服务，及判断某个服务是否开启，具体步骤如下了:
第一步:新建一个Android工程，命名为RunningService。
第二步:修改RunningService.java代码如下：
**[java]**[view
 plain](http://blog.csdn.net/Android_Tutor/article/details/5824581#)[copy](http://blog.csdn.net/Android_Tutor/article/details/5824581#)
- package com.tutor.runningservice;  
- import java.util.List;  
- import android.app.Activity;  
- import android.app.ActivityManager;  
- import android.os.Bundle;  
- import android.widget.TextView;  
- publicclass RunningService extends Activity {  
- /** Called when the activity is first created. */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
- //setContentView(R.layout.main);
-         TextView mTextView = new TextView(this);  
-         ActivityManager mActivityManager =   
-             (ActivityManager)getSystemService(ACTIVITY_SERVICE);  
- 
-         List<ActivityManager.RunningServiceInfo> mServiceList = mActivityManager.getRunningServices(30);  
- //我要判断的服务名字，我在launcher2里加了一个音乐服务   
- final String musicClassName = "com.android.launcher2.MusicService";  
- 
- boolean b = MusicServiceIsStart(mServiceList, musicClassName);  
- 
-         mTextView.setText("你要判断的服务状态为: " +b+"/n" + getServiceClassName(mServiceList));  
-         setContentView(mTextView);  
-     }  
- //通过Service的类名来判断是否启动某个服务
- privateboolean MusicServiceIsStart(List<ActivityManager.RunningServiceInfo> mServiceList,String className){  
- 
- for(int i = 0; i < mServiceList.size(); i ++){  
- if(className.equals(mServiceList.get(i).service.getClassName())){  
- returntrue;  
-             }  
-         }  
- returnfalse;  
-     }  
- //获取所有启动的服务的类名
- private String getServiceClassName(List<ActivityManager.RunningServiceInfo> mServiceList){  
-         String res = "";  
- for(int i = 0; i < mServiceList.size(); i ++){  
-             res+=mServiceList.get(i).service.getClassName()+ " /n";  
-         }  
- 
- return res;  
-     }  
- }  
第三步:运行上述工程，查看效果!
不看不知道，一看吓一跳，乖乖，开了这么多服务，难怪手机很快就没电了。
![](http://hi.csdn.net/attachment/201008/19/0_1282210037CXrJ.gif)
上面的一些服务好多是系统自带的，并且不可见的，我们可以到Settings->Applications->Running services查看
一共才看到十来个服务。
![](http://hi.csdn.net/attachment/201008/19/0_1282210192Q6LW.gif)
OK,今天就这么多，呵呵，继续上班中..............
