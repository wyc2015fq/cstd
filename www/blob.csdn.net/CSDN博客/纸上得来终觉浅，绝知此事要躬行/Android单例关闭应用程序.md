# Android单例关闭应用程序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月11日 16:55:39[boonya](https://me.csdn.net/boonya)阅读数：877








 其设计思路是采用单例记录所有的Activity然后退出时将所有的Activity关闭。

 这种东西一搜一大片，如果看过人家是怎么实现的写出来就不叫做原创，转自哪里不知道，直接看代码。



```java
package com.boonya.wtms.utils;

import java.util.ArrayList;
import java.util.List;
import android.app.Activity;
import android.app.Application;
/**
 * 
 * @author        boonya
 *
 * @having-line---------------------------------------------------------
 * @filename      MyApplication.java
 * @function      单例程序安全退出应用
 * @start-at      2014-9-11,下午4:29:14
 * @having-line---------------------------------------------------------
 */
public class MyApplication extends Application
{

	public MyApplication()
	{
	}

	private List<Activity> activities = new ArrayList<Activity>();

	private static MyApplication myApplication;

	public static MyApplication getInstance()
	{
		if (myApplication == null)
		{
			myApplication = new MyApplication();
		}
		return myApplication;
	}

	public void addActivity(Activity activity)
	{
		activities.add(activity);
	}

	public void exit()
	{
		for (Activity activity : activities)
		{
			activity.finish();
		}
		System.exit(0);
	}
}
```


在每一个Activity的oncreate方法中调用方法：



```java
MyApplication.getInstance().addActivity(this);
```


然后退出按钮注册事件调用方法：



```java
MyApplication.getInstance().exit();
```


下班收工！！！！！






