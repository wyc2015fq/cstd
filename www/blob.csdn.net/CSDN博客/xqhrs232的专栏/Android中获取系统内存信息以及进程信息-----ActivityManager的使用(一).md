# Android中获取系统内存信息以及进程信息-----ActivityManager的使用(一) - xqhrs232的专栏 - CSDN博客
2012年05月24日 17:29:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：981标签：[android																[任务																[listview																[string																[service																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=listview&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/qinjuning/article/details/6978560](http://blog.csdn.net/qinjuning/article/details/6978560)
相关网帖
1.[Android 获取手机总内存和可用内存等信息](http://www.cnblogs.com/playing/archive/2011/05/14/2046396.html) ----[http://www.cnblogs.com/playing/archive/2011/05/14/2046396.html](http://www.cnblogs.com/playing/archive/2011/05/14/2046396.html)
2.[Android获取内存信息](http://blog.csdn.net/xpwang168/article/details/6982453)----[http://blog.csdn.net/xpwang168/article/details/6982453](http://blog.csdn.net/xpwang168/article/details/6982453)
3.Android中如何获得系统相关的信息（比如CPU使用率，内存的总量和已用的量）----[http://hi.baidu.com/lzlsha/blog/item/ac584e22c3b085bb4723e80d.html](http://hi.baidu.com/lzlsha/blog/item/ac584e22c3b085bb4723e80d.html)
4.linux下用java程序获取cpu和内存的使用率----[http://hi.baidu.com/qmiao128/blog/item/fa91b41f1b6c1f61f724e424.html](http://hi.baidu.com/qmiao128/blog/item/fa91b41f1b6c1f61f724e424.html)
**       转载请注明出处：[http://blog.csdn.net/qinjuning](http://blog.csdn.net/qinjuning)**
                  本节内容主要是讲解ActivityManager的使用，通过ActivityManager我们可以获得系统里正在运行的activities，包括
     进程(Process)等、应用程序/包、服务(Service)、任务(Task)信息。
              计划如下：
** 第一部分：获取系统可用内存以及所有的正在运行的进程信息 ;**
**                  第二部分：获取每个进程里运行的应用程序信息和所有正在运行的应用程序**
**                  第三部分：获取正在运行的服务和任务信息。**
                每部分都准备了相应的Demo，助您更深的理解.
        知识准备：Android 应用程序模块: **应用、任务、进程**的知识介绍：
            1  一个**android 包**(简称**.apk**) ：里面包含应用程序的代码以及资源。这是一个应用发布，用户能下载并安装他们
              设备上的文件。
            2  一个 **任务** ：通常用户能当它为一个“应用程序”来启动：通常在桌面上会有一个图标可以来启动任务，这是一个
              上层的应用，可以将你的任务切换到前台来。
            3  一个 **进程** ：是一个底层的代码运行级别的核心进程。通常.apk包里所有代码运行在一个进程里，一个进程对于
              一个.apk包；然而,[进程](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestApplication_process) 标签常用来改变代码运行的位置，可以是[全部的.apk包](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestApplication) 或者是独立的[活动](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestActivity),[接收器](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestReceiver),[服务](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestService),
 或者
[提供器](http://chaoqunz.blog.163.com/reference/android/R.styleable.html#AndroidManifestProvider)组件。
###   进程
         在Android中，进程是应用程序的完整实现，而不是用户通常了解的那样。他们主要用途很简单：
            1、提高稳定性和安全性，将不信任或者不稳定的代码移动到其他进程。
            2、可将多个.apk包运行在同一个进程里减少系统开销。
            3、帮助系统管理资源，将重要的代码放在一个单独的进程里，这样就可以单独销毁应用程序的其他部分。
         于是，为了完成某一任务或者减少系统资源的开销，**一个进程里可以运行一个或多个应用程序**。
    更多知识点获取，请访问：《[**Android 应用程序模块: 应用, 任务, 进程, 和线程**](http://chaoqunz.blog.163.com/blog/static/6154877720090495819383/)》
### ActivityManager 类：
             获取方法 ActivityManager mActivityManager (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);
          常用的静态内部类如下(下面具体介绍):
[**ActivityManager.MemoryInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.MemoryInfo.html)：
 系统可用内存信息
[**ActivityManager.RecentTaskInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RecentTaskInfo.html)：
 最近的任务信息
[**ActivityManager.RunningAppProcessInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningAppProcessInfo.html)：
 正在运行的进程信息
[**ActivityManager.RunningServiceInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningServiceInfo.html)：
 正在运行的服务信息
[**ActivityManager.RunningTaskInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningTaskInfo.html)：
 正在运行的任务信息
** 常用方法：**
> 
public void **getMemoryInfo**(ActivityManager.MemoryInfo outInfo)
                说明：获取系统可用内存信息，数据封装在outInfo对象上
> 
public Debug.MemoryInfo** getProcessMemoryInfo**(int[ ] pids
说明：获取每个进程ID(集合)占用的内存大小(集合), pid和MemoryInfo是一一对应的。
     参数： pids 进程ID的集合
PS :我们可以通过调用Debug.MemoryInfo 的dalvikPrivateDirty字段获取进程占用的内存大小(单位为KB)
 public List<ActivityManager.RunningAppProcessInfo>**getRunningAppProcess**()
    说明： 获取系统里正在运行的进程
> 
public [List](file:///F:/android-sdk-windows/docs/reference/java/util/List.html)<[ActivityManager.RunningServiceInfo](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningServiceInfo.html)>[**getRunningServices**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.html#getRunningServices(int))(int
 maxNum)
     说明： 获取系统里所有正在运行的服务
     参数：可能服务的最大值(赋予一常数即可，20、50都OK)
> 
public [List](file:///F:/android-sdk-windows/docs/reference/java/util/List.html)<[ActivityManager.RunningTaskInfoo](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningServiceInfo.html)>[**getRunningTasks**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.html#getRunningTasks(int))(int
 maxNum)
               说明：获取系统里正在运行的服务
               参数： 同上
> 
public [List](file:///F:/android-sdk-windows/docs/reference/java/util/List.html)<[ActivityManager.RecentTaskInfo](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RecentTaskInfo.html)>[**getRecentTasks**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.html#getRecentTasks(int, int))(int
 maxNum, int flags)
             说明：获取系统里最近启动的任务
             参数： 同上，flags一般为0即可
> 
public void**killBackgroundProcess**(String packageName)
            说明：立即杀掉给定包名的进程，释放进程占用的资源(内存等)，如果需要，系统会再次重新启动该进程。系统
            PS：系统进程是杀不死的，只能杀死用户进程。但我没有找到好的方法辨别系统进程还是用户进程。但可以肯定的是，
                   能够杀死的一定是用户进程。
          public void restartPackage(String packageName)
            说明：该方法已弃用，等同于killBackgroundProcess 。
###  ActivityManager.MemoryInfo类
**    常用字段：**
            long **availMem** 系统可用内存
            long [threshold](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.MemoryInfo.html#threshold)系统内存不足的阀值，即临界值
            boolean lowMemory 如果当前可用内存<=[threshold](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.MemoryInfo.html#threshold)，该值为真
### ActivityManager.RunningAppProcessInfo类
**   常用字段：**
          int pid                             进程ID
          int uid                             进程所在的用户ID
          String processName    进程名，默认是包名或者由android:process=””属性指定
          String [ ] pkgList           运行在该进程下的所有应用程序包名
 对[**ActivityManager.RecentTaskInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RecentTaskInfo.html)** 、**[**ActivityManager.RunningServiceInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningServiceInfo.html)** 、**[**ActivityManager.RunningTaskInfo**](file:///F:/android-sdk-windows/docs/reference/android/app/ActivityManager.RunningTaskInfo.html)
   类的介绍留在后文介绍。
**DEMO说明：**
          我们通过ActivityManager获取了系统的可用内存信息以及正在运行在系统里的进程具体信息，当然你也可以选择
       手动杀死这些进程，不过前提是用户进程，因为系统进程是杀不死的。
          模拟器上的截图如下：
![](http://hi.csdn.net/attachment/201111/16/0_13214464344xh9.gif)![](http://hi.csdn.net/attachment/201111/16/0_1321446494qfqf.gif)
资源文件什么的就不再贴代码了，直接列出逻辑文件。
** 获取系统可用内存的代码**：MainActivity.java
**[java]**[view
 plain](http://blog.csdn.net/qinjuning/article/details/6978560#)[copy](http://blog.csdn.net/qinjuning/article/details/6978560#)[print](http://blog.csdn.net/qinjuning/article/details/6978560#)[?](http://blog.csdn.net/qinjuning/article/details/6978560#)
- package com.qin.ammp;  
- 
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import android.app.Activity;  
- import android.app.ActivityManager;  
- import android.app.ActivityManager.MemoryInfo;  
- import android.content.Context;  
- import android.content.Intent;  
- import android.os.Bundle;  
- import android.os.Debug;  
- import android.text.format.Formatter;  
- import android.util.Log;  
- import android.view.View;  
- import android.widget.Button;  
- import android.widget.TextView;  
- 
- publicclass MainActivity extends Activity {  
- 
- privatestatic String TAG = "AM_MEMORYIPROCESS" ;  
- 
- private ActivityManager mActivityManager = null ;  
- 
- private TextView tvAvailMem  ;  
- private Button btProcessInfo ;  
- 
- /** Called when the activity is first created. */
- @Override
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
-         setContentView(R.layout.main);  
- 
-         tvAvailMem = (TextView)findViewById(R.id.tvAvailMemory) ;  
-         btProcessInfo =(Button)findViewById(R.id.btProcessInfo);  
- //跳转到显示进程信息界面
-         btProcessInfo.setOnClickListener(new View.OnClickListener() {  
- 
- @Override
- publicvoid onClick(View v) {  
- // TODO Auto-generated method stub
-                 Intent intent = new Intent(MainActivity.this,BrowseProcessInfoActivity.class);  
-                 startActivity(intent);  
-             }  
-         });                 
- 
- //获得ActivityManager服务的对象
-         mActivityManager = (ActivityManager)getSystemService(Context.ACTIVITY_SERVICE);  
- 
- //获得可用内存信息
-         String availMemStr = getSystemAvaialbeMemorySize();  
-         Log.i(TAG, "The Availabel Memory Size is"+availMemStr);   
- //显示
-         tvAvailMem.setText(availMemStr);   
- 
-     }  
- //获得系统可用内存信息
- private String getSystemAvaialbeMemorySize(){  
- //获得MemoryInfo对象
-         MemoryInfo memoryInfo = new MemoryInfo() ;  
- //获得系统可用内存，保存在MemoryInfo对象上
-         mActivityManager.getMemoryInfo(memoryInfo) ;  
- long memSize = memoryInfo.availMem ;  
- 
- //字符类型转换
-         String availMemStr = formateFileSize(memSize);  
- 
- return availMemStr ;  
-     }  
- 
- //调用系统函数，字符串转换 long -String KB/MB
- private String formateFileSize(long size){  
- return Formatter.formatFileSize(MainActivity.this, size);   
-     }  
- 
- }  
**获取系统进程信息的代码** ：BrowseProcessInfoActivity .java
**[java]**[view
 plain](http://blog.csdn.net/qinjuning/article/details/6978560#)[copy](http://blog.csdn.net/qinjuning/article/details/6978560#)[print](http://blog.csdn.net/qinjuning/article/details/6978560#)[?](http://blog.csdn.net/qinjuning/article/details/6978560#)
- package com.qin.ammp;  
- 
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import android.app.Activity;  
- import android.app.ActivityManager;  
- import android.app.AlertDialog;  
- import android.app.Dialog;  
- import android.content.Context;  
- import android.content.DialogInterface;  
- import android.os.Bundle;  
- import android.os.Debug;  
- import android.util.Log;  
- import android.view.ContextMenu;  
- import android.view.Menu;  
- import android.view.MenuItem;  
- import android.view.View;  
- import android.view.ContextMenu.ContextMenuInfo;  
- import android.widget.AdapterView;  
- import android.widget.ListView;  
- import android.widget.TextView;  
- import android.widget.AdapterView.OnItemClickListener;  
- 
- publicclass BrowseProcessInfoActivity extends Activity  implements OnItemClickListener{  
- 
- privatestatic String TAG = "ProcessInfo";  
- privatestaticfinalint KILL_PORCESS = 1;  
- privatestaticfinalint SEARCH_RUNNING_APP = 2;  
- 
- private ActivityManager mActivityManager = null;  
- // ProcessInfo Model类 用来保存所有进程信息
- private List<ProcessInfo> processInfoList = null;  
- 
- private ListView listviewProcess;  
- private TextView tvTotalProcessNo ;   
- 
- publicvoid onCreate(Bundle savedInstanceState) {  
- super.onCreate(savedInstanceState);  
- 
-         setContentView(R.layout.browse_process_list);  
- 
-         listviewProcess = (ListView) findViewById(R.id.listviewProcess);  
-         listviewProcess.setOnItemClickListener(this);  
- 
-         tvTotalProcessNo =(TextView)findViewById(R.id.tvTotalProcessNo);  
- 
- this.registerForContextMenu(listviewProcess);  
- // 获得ActivityManager服务的对象
-         mActivityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);  
- // 获得系统进程信息
-         getRunningAppProcessInfo();  
- // 为ListView构建适配器对象
-         BrowseProcessInfoAdapter mprocessInfoAdapter = new BrowseProcessInfoAdapter(  
- this, processInfoList);  
-         listviewProcess.setAdapter(mprocessInfoAdapter);  
- 
-         tvTotalProcessNo.setText("当前系统进程共有："+processInfoList.size());  
-     }  
- //杀死该进程，并且刷新
- @Override
- publicvoid onItemClick(AdapterView<?> arg0, View arg1,  finalint position, long arg3) {  
- // TODO Auto-generated method stub
- new AlertDialog.Builder(this).setMessage("是否杀死该进程")  
-         .setPositiveButton("确定", new DialogInterface.OnClickListener() {  
- 
- @Override
- publicvoid onClick(DialogInterface dialog, int which) {  
- // TODO Auto-generated method stub
- //杀死该进程，释放进程占用的空间
-                 mActivityManager.killBackgroundProcesses(processInfoList.get(position).getProcessName());  
- //刷新界面
-                 getRunningAppProcessInfo() ;  
-                 BrowseProcessInfoAdapter mprocessInfoAdapter = new BrowseProcessInfoAdapter(  
-                         BrowseProcessInfoActivity.this, processInfoList);  
-                 listviewProcess.setAdapter(mprocessInfoAdapter);  
-                 tvTotalProcessNo.setText("当前系统进程共有："+processInfoList.size());  
- 
-             }  
-         }).setNegativeButton("取消", new DialogInterface.OnClickListener() {  
- 
- @Override
- publicvoid onClick(DialogInterface dialog, int which) {  
- // TODO Auto-generated method stub
-                 dialog.cancel() ;  
-             }  
-         }).create().show() ;  
-     }  
- // 获得系统进程信息
- privatevoid getRunningAppProcessInfo() {  
- // ProcessInfo Model类   用来保存所有进程信息
-         processInfoList = new ArrayList<ProcessInfo>();  
- 
- // 通过调用ActivityManager的getRunningAppProcesses()方法获得系统里所有正在运行的进程
-         List<ActivityManager.RunningAppProcessInfo> appProcessList = mActivityManager  
-                 .getRunningAppProcesses();  
- 
- for (ActivityManager.RunningAppProcessInfo appProcessInfo : appProcessList) {  
- // 进程ID号
- int pid = appProcessInfo.pid;  
- // 用户ID 类似于Linux的权限不同，ID也就不同 比如 root等
- int uid = appProcessInfo.uid;  
- // 进程名，默认是包名或者由属性android：process=""指定
-             String processName = appProcessInfo.processName;  
- // 获得该进程占用的内存
- int[] myMempid = newint[] { pid };  
- // 此MemoryInfo位于android.os.Debug.MemoryInfo包中，用来统计进程的内存信息
-             Debug.MemoryInfo[] memoryInfo = mActivityManager  
-                     .getProcessMemoryInfo(myMempid);  
- // 获取进程占内存用信息 kb单位
- int memSize = memoryInfo[0].dalvikPrivateDirty;  
- 
-             Log.i(TAG, "processName: " + processName + "  pid: " + pid  
-                     + " uid:" + uid + " memorySize is -->" + memSize + "kb");  
- // 构造一个ProcessInfo对象
-             ProcessInfo processInfo = new ProcessInfo();  
-             processInfo.setPid(pid);  
-             processInfo.setUid(uid);  
-             processInfo.setMemSize(memSize);  
-             processInfo.setPocessName(processName);  
-             processInfoList.add(processInfo);  
- 
- // 获得每个进程里运行的应用程序(包),即每个应用程序的包名
-             String[] packageList = appProcessInfo.pkgList;  
-             Log.i(TAG, "process id is " + pid + "has " + packageList.length);  
- for (String pkg : packageList) {  
-                 Log.i(TAG, "packageName " + pkg + " in process id is -->"+ pid);  
-             }  
-         }  
-     }  
- 
- publicvoid onCreateContextMenu(ContextMenu menu, View v,  
-             ContextMenuInfo menuInfo) {  
-         menu.add(0, 0, KILL_PORCESS, "杀死该进程");  
-         menu.add(0, 0, SEARCH_RUNNING_APP, "运行在该进程的应用程序");  
- super.onCreateContextMenu(menu, v, menuInfo);  
- 
-     }  
- 
- publicboolean onContextItemSelected(MenuItem item) {  
- switch (item.getItemId()) {  
- case KILL_PORCESS: // 杀死该进程 ， 重新加载界面
- new AlertDialog.Builder(this).setMessage("是否杀死该进程")  
-             .setPositiveButton("确定", new DialogInterface.OnClickListener() {  
- 
- @Override
- publicvoid onClick(DialogInterface dialog, int which) {  
- // TODO Auto-generated method stub
- 
-                 }  
-             }).setNegativeButton("取消", new DialogInterface.OnClickListener() {  
- 
- @Override
- publicvoid onClick(DialogInterface dialog, int which) {  
- // TODO Auto-generated method stub
-                     dialog.cancel() ;  
-                 }  
-             }).create().show() ;  
- break;  
- case SEARCH_RUNNING_APP: // 查看运行在该进程的应用程序信息
- break;  
- default:  
- break;  
-         }  
- returnsuper.onContextItemSelected(item);  
-     }  
- 
- }  
   我们可以通过进程占用内存大小来进而获取占用cpu大小，直接换算还是很简单的。
**   源代码地址：****[http://download.csdn.net/detail/qinjuning/3803314](http://download.csdn.net/detail/qinjuning/3803314)**

