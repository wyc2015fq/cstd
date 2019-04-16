# 安卓开发02：Activity的生命周期 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月04日 15:59:38[initphp](https://me.csdn.net/initphp)阅读数：1831
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









**基础代码：**


创建安卓最基础版本的代码之后，会看到一个HelloWorld的例子。同时也会看到一个MainActivity.java的文件

![](https://img-blog.csdn.net/20130604155834593)




打开看代码：



```java
package com.example.androidtest;


import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.Window;

public class MainActivity extends Activity {
    
    private String tag = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main); //设置一个View模板
        Log.d(tag, "In the onStart() event");  
 }
    
    @Override
    public void onStart() {  
        super.onStart();  
        Log.d(tag, "In the onStart() event");  
    }  
    
    public void onRestart() {  
        super.onRestart();  
        Log.d(tag, "In the onRestart() event");  
    }  
  
    public void onResume() {  
        super.onResume();  
        Log.d(tag, "In the onResume() event");  
    }  
  
    public void onPause() {  
        super.onPause();  
        Log.d(tag, "In the onPause() event");  
    }  
  
    public void onStop() {  
        super.onStop();  
        Log.d(tag, "In the onStop() event");  
    }  
  
    public void onDestroy() {  
        super.onDestroy();  
        Log.d(tag, "In the onDestroy() event");  
    }  

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    
}
```




**相关概念：**


一个应用程序通常由多个activities组成，他们通常是松耦合关系。通常，一个应用程序中的activity被指定为"main"activity，当第一次启动应用程序的时候呈现给用户的那个activity。每一个activity然后可以启动另一个activity为了完成不同的动作。每一次一个activity启动，前一个activity就停止了，但是系统保留activity在一个栈上（“back stack”）。当一个新activity启动，它被推送到栈顶，取得用户焦点。Back Stack符合简单“后进先出”原则，所以，当用户完成当前activity然后点击back按钮，它被弹出栈（并且被摧毁），然后之前的activity恢复。

　　当一个activity因新的activity启动而停止，它被通知这种状态转变通过activity的生命周期回调函数。有许多回调函数一个activity可能会收到，源于它自己的状态变化－无论系统创建它、停止它、恢复它、摧毁它－并且每个回调提供你完成适合这个状态的指定工作的机会。例如，当停止的时候，你的activity应该释放任何大的对象，例如网络数据库连接。当activity恢复，你可以重新获得必要的资源和恢复被中断的动作。这些状态转换都是activity的生命周期的部分。




**我们测试一把：**


我们启动这个Hello World程序，观察日志：我们可以观察到，启动的时候，首先onCreate() 创建一个页面布局，然后当用户可见的时候，调用onStart() ，当可以和用户交互的时候，调用onResume()


![](https://img-blog.csdn.net/20130604160632218)




当我退出这个程序，日志打印的顺序是：onPause() ，onStop() ，onDestroy()。也就说，先暂停，然后停止，然后销毁

![](https://img-blog.csdn.net/20130604161013484)





**总结：**


- onCreate()  ——  当Activity第一次创建的时候被调用。一般每个Activity都需要有一个onCreate()函数
- onStart()  ——  当Activity对用户可见的时候被调用。
- onResume()  ——  当Activity开始和用户交互的时候被调用。
- onPause()  ——  正在运行的Activity马上要被暂停的时候被调用，此时，在这之前的Activity被重新获取。
- onStop()  ——  当Activity不在对用户可见的时候被调用。
- onDestroy() —— 当被销毁的时候调用




**生命周期图：**


下面是一副Activity的生命周期图：

![](https://img-blog.csdn.net/20130604161147453)




