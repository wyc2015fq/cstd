# Broadcast实现Activity之间刷新或值传递 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年02月10日 11:58:13[boonya](https://me.csdn.net/boonya)阅读数：2499








Activity之间UI刷新，是个一个很常见的应用场景（Scene）。虽然可以定义一个静态变量和方法去搞定，但不推荐采用静态方法去操作。Android的广播机制是信心传递的一种方式，Android广播的特点是：支持消息发送并可以允许开发者对消息做出响应处理，很类似于系统消息监听。广播在一定程度上很类似于推送，但它不是推送，推送就是现实里面的广播，消息播出去了就不管了，错过了就是错过了。

**需求：**选择任务的执行人，但是好友列表是从另一个Activity来选的，选择后在原来打开的Activity中刷新选择的结果。

#### 第一个Activity内部广播



```java
public class DraftEditActivity extends Activity
{

        private BroadcastReceiver broadcastReceiver;

        @Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
                ..............
                // 初始化广播接收
		initBroadcastReceiver();
        }

        public void initBroadcastReceiver()
	{
		broadcastReceiver = new BroadcastReceiver()
		{

			@Override
			public void onReceive(Context arg0, Intent arg1)
			{
				
				setPeopleToTags();
			}
		};
		IntentFilter filter = new IntentFilter(AddPersonActivity.action);
		registerReceiver(broadcastReceiver, filter);
	}
 
        // 设置显示选择的人
        public void setPeopleToTags(){}

        @Override
        public void onDestroy()
	{
		unregisterReceiver(broadcastReceiver);
                super.onDestroy();
	}

}
```




#### 第二个Activity发送广播



```java
public class AddPersonActivity extends Activity
{

        public static final String action = "com.scengine.escheduler.editdraft.broadcast.action"; 

        @Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
                ..............
        }

       	@Override
	public void finish()
	{
		// TODO Auto-generated method stub
		Intent intent = new Intent(action); 
		sendBroadcast(intent);
		super.finish();
	}

}
```


在窗口关闭的时候发送广播，然后更新上一个Activity。

#### 测试效果图

图一：

![](https://img-blog.csdn.net/20150210120225332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图二：

![](https://img-blog.csdn.net/20150210120243069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


图三：

![](https://img-blog.csdn.net/20150210120251759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### Activity之间传值

  Activity之间传值可以使用Intent来传递。



```java
Intent intent = new Intent(action); 
 intent.putExtra("data", "hello,boonya");  
sendBroadcast(intent);
```

注：广播注册还可以在AndroidManifest.xml中注册receiver即可，上面没有采用此（静态注册）方式，而是在代码中注册（动态注册）的，以上就是全部内容了，欢迎指正！

#### 其他方式实现Activity刷新

  有人用Application来操作一个Handler异步线程处理器，实现UI刷新，在Activity中调用getHandler和setHandler来处理。

Application由于是生存周期与整个应用程序存活周期是一致的，而且它是应用实例的上层架构，所以这个是方案也是可取的。



```java
public class MyApplication extends Application { 
// 共享变量 
private MyHandler handler = null; 
   
// set方法 
public void setHandler(MyHandler handler) { 
   this.handler = handler; 
} 
   
// get方法 
public MyHandler getHandler() { 
   return handler; 
} 
   
}
```









