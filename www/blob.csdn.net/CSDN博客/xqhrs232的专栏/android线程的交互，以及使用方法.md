# android线程的交互，以及使用方法 - xqhrs232的专栏 - CSDN博客
2010年12月24日 10:04:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3422
原文地址::[http://byandby.javaeye.com/blog/825071](http://byandby.javaeye.com/blog/825071)
  首先原谅我把文章的标题写的这么长。其实我还嫌弃它短了因为 写不下去了所以我就不写了。因为我实在不知道该怎么定义这篇文章的标题或许应该叫 “乱谈”比较合适。 这样可能还体现了 大道至简的 精髓 呵呵。
    希望和我一样正在学习android的新手可以通过这篇文章熟悉android线程的交互，以及使用方法。以便将来更好的在自己的程序中运用。也希望大家保持学习android的积极性，一起努力、交流、成长，因为文章比较长 大家可以分几次阅读。这篇文章包含了好几个例子，我之所把它们写在一起，是觉得它们有一定联系。
  好吧我们开始让 我们先从android进程说起吧。 
   当一个程序第一次启动的时候，Android会启动一个LINUX进程和一个主线程。默认的情况下，所有该程序的组件都将在该进程和线程中运行。同时，Android会为每个应用程序分配一个单独的LINUX用户。Android会劲量保留一个正在运行进程，只在内存资源出现不足时，Android会参试停止一些进程从而释放足够的资源给其他新的进程使用， 也能保证用户正在访问的当前进程有足够的资源去及时的响应用户的事件。Android会 根据进程中运行的组件类别以及组件的状态来判断该进程的重要性，Android会 首先停止那些不重要的进程。按照重要性从高到低一共有五个级别：
l  前台进程 
前台进程是用户当前正在使用的进程。只有一些前台进程可以在任何时候都存在。他们是最后一个被结束的，当内存低到根本连他们都不能运行的时候。一般来说， 在这种情况下，设备会进行内存调度，中止一些前台进程来保持对用户交互的响应。
l  可见进程 
可见进程不包含前台的组件但是会在屏幕上显示一个可见的进程是的重要程度很高，除非前台进程需要获取它的资源，不然不会被中止。 
l  服务进程 
运行着一个通过startService() 方法启动的service，这个service不属于上面提到的2种更高重要性的。service所在的进程虽然对用户不是直接可见的，但是他们执行了用户非常关注的任务（比如播放mp3，从网络下载数据）。只要前台进程和可见进程有足够的内存，系统不会 回收他们。
l  后台进程 
运行着一个对用户不可见的activity（调用过 onStop() 方法).这些进程对用户体验没有直接的影响，可以在服务进程、可见进程、前台进 程需要内存的时候回收。通常，系统中会有很多不可见进程在运行，他们被保存在LRU (least recently used) 列表中，以便内存不足的时候被第一时间回收。如果一个activity正 确的执行了它的生命周期，关闭这个进程对于用户体验没有太大的影响。
l  空进程 
未运行任何程序组件。运行这些进程的唯一原因是作为一个缓存，缩短下次程序需要重新使用的启动时间。系统经常中止这些进程，这样可以调节程序缓存和系统缓 存的平衡。 
Android 对进程的重要性评级的时候，选取它最高的级别。另外，当被另外的一个进程依赖的时候，某个进程的级别可能会增高。一个为其他进程服务的进程永远不会比被服 务的进程重要级低。因为服务进程比后台activity进程重 要级高，因此一个要进行耗时工作的activity最好启动一 个service来做这个工作，而不是开启一个子进程――特别 是这个操作需要的时间比activity存在的时间还要长的时 候。例如，在后台播放音乐，向网上上传摄像头拍到的图片，使用service可 以使进程最少获取到“服务进程”级别的重要级，而不用考虑activity目
 前是什么状态。broadcast receivers做费时的工作的时候，也应该启用一个服务而不是开一个线程。 
2单线程模型 
    当一个程序第一次启动时，Android会同时启动一个对应的 主线程（Main Thread），主线程主要负责处理与UI相关的事件，如：用户的按键事件，用户接触屏幕的事件以及屏幕绘图事 件，并把相关的事件分发到对应的组件进行处理。所以主线程通常又被叫做UI线 程。在开发Android应用时必须遵守单线程模型的原则： Android UI操作并不是线程安全的并且这些操作必须在UI线程中执行。
    单线程模型会在没有考虑到它的影响的情况下引起Android应用程序性能低下，因为 所有的任务都在同一个线程中执行，如果执行一些耗时的操作，如访问网络或查询数据库，会阻塞整个用户界面。当在执行一些耗时的操作的时候，不能及时地分发 事件，包括用户界面重绘事件。从用户的角度来看，应用程序看上去像挂掉了。更糟糕的是，如果阻塞应用程序的时间过长（5秒钟）Android会向用户提示一些信息，即打开一个“应用程序没有相应（application not responding）”ANR 的对话框。
其实单线程模型就是默认情况下android把所有操作都放在主线程也就是UI Thread线程中来执行 如果你想 O上边那段不是说它会阻塞用户界面嘛  那我可以另起一个线程来执行一些操作 没错你的想法非常good 。很给力。那么接下来 你就会尝试另起一个线程来 执行一些操作。OK 结果就有两种可能 一：你在另外开启的那个线程中执行了一些后台的操作 比如开启一个服务啊。神马的。那么恭喜你 你成功了。 二：第二种可能结果就是 你会收到一个华丽的异常 。这个例子很简单
下面我们就通过一个小例子来说明这个华丽的异常时怎么回事？ 因为本篇文章的例子比较多所以我为了大家好找 给例子起了名称 这个例子的名称是： 异常测试
布局文件
<?xml version="1.0" encoding="utf-8"?> 
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android" 
    android:orientation="vertical" 
    android:layout_width="fill_parent" 
    android:layout_height="fill_parent" 
    > 
<TextView 
android:id="@+id/textview01" 
    android:layout_width="fill_parent" 
    android:layout_height="wrap_content" 
    android:text="@string/hello" 
    /> 
<Button 
android:id="@+id/myButton" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content" 
android:layout_below="@id/textview01" 
android:text="异常测试" 
/> 
<TextView 
android:id="@+id/myTextView" 
android:textSize="15pt" 
android:layout_toRightOf="@id/myButton" 
android:layout_alignTop="@id/myButton" 
android:textColor="#FF0000" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content"/> 
</RelativeLayout> 
Activity01 类
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- **import** android.app.Activity;   
- **import** android.os.Bundle;   
- **import** android.view.View;   
- **import** android.view.View.OnClickListener;   
- **import** android.widget.Button;   
- **import** android.widget.TextView;   
- 
- **public****class** Activity01 **extends** Activity {
   
- **private** Button myButton;   
- **private** TextView myTextView;   
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {   
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.main);   
-         myButton = (Button) findViewById(R.id.myButton);   
-         myTextView = (TextView) findViewById(R.id.myTextView);   
-         myButton.setOnClickListener(**new** MyButtonListener());   
-     }   
- 
- **class** MyButtonListener **implements** OnClickListener {   
- @Override
- **public****void** onClick(View v) {   
- **new** Thread() {   
- @Override
- **public****void** run() {   
- // 我们在这里更新了UI 设置了TextView的值
-                     myTextView.setText("张三");   
-                 }   
-             }.start();   
-         }   
-     }   
- }  
```java
package xiaohang.zhimeng;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
public class Activity01 extends Activity {
	private Button myButton;
	private TextView myTextView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		myButton = (Button) findViewById(R.id.myButton);
		myTextView = (TextView) findViewById(R.id.myTextView);
		myButton.setOnClickListener(new MyButtonListener());
	}
	class MyButtonListener implements OnClickListener {
		@Override
		public void onClick(View v) {
			new Thread() {
				@Override
				public void run() {
					// 我们在这里更新了UI 设置了TextView的值
					myTextView.setText("张三");
				}
			}.start();
		}
	}
}
```
接下来我们以看图片的方式 看看这是怎样的一个异常。请大家点击图片查看大图 
![](http://dl.javaeye.com/upload/attachment/356334/45de0dfa-835c-3790-86ec-3387953a06a3.jpg)
android.view.ViewRoot$CalledFromWrongThreadException: Only the original thread that created a view hierarchy can touch its views.
就是这样一个异常 这个异常告诉我们不可以再子线程中更新UI元素 比如我们上边那个例子设置了一个TextView的值。所有与UI相关的操作都不可以在子线程中执行都必须在UI线程中执行。这个异常大家以后可能会经常遇到多加注意就是了。
    我们继续，下面我们说说线程交互方面的东西。就比如我们想做点这些事情上边不是说了不能在子线程中更新UI 那比如我想在子线程中定义 一个字符串 然后通过发消息的方式 Message 把这个字符串 发送给主线程也就是UI线程 让UI线程来 设置这个TextView的值为我们刚刚在子线程中定义的字符串。 或者我想在子线程中开启音乐服务，或者把它停止该怎么做呢？  要知道怎么做先让我们了解 一下这几个对象吧！它们分别是
MessageQueue        Handler             Looper   下面对它们进行详细说明
Message Queue 
    在单线程模型下，为 了解决类似的问题，Android设 计了一个Message Queue(消息队列)， 线程间可以通过该Message Queue并结合Handler和Looper组 件进行信息交换。
l  Message Queue 
     Message Queue是一个消息队列，用来存放通过Handler发 布的消息。消息队列通常附属于某一个创建它的线程，可以通过Looper.myQueue()得 到当前线程的消息队列。如果没有消息队列对象则会抛出空指针异常 。Android在 第一次启动程序时会默认会为UI thread创建一个关联的消息队列，用来管理程序的一些上层组件，activities，broadcast receivers 等等。你可以在自己的子线程中创建Handler与UI thread通讯。也就是说我们程序一启动我们的UI线程也就是主线程就会有一个消息队列
 ，而如果我们自己另外开启的一个子线程就不会有MessageQueue(消息队列)对象。这一点大家务必知道。 
l  Handler 
    通过Handler你 可以发布或者处理一个消息或者是一个Runnable的 实例。每个Handler都 会与唯一的一个线程以及该线程的消息队列关联。当你创建一个新的Handler时候，默认情况下，它将关联到创建它的这个线程和该线程的消息队列。也就是说，如果你通过Handler发 布消息的话，消息将只会发送到与它关联的这个消息队列，当然也只能处理该消息队列中的消息。这里大家就得理解一下了 也就是说我们 一个Handler对应一个线程以及附属于该线程的消息队列。就比如 我们现在有一个Handler对象这个Handler在UI线程中创建
 叫xh_Handler 那么我们根据上边的说明 应该可以告诉我 这个xh_Handler是和那个线程关联的？  如果我用这个Handler发消息的话它将发给那个线程的消息队列？ 如果你知道了 我想一般情况下你肯定是知道的。说明你很懂。我们继续。这时我们用这个Handler对象发送消息
Handler对象也可以发送消息哦，它有发送消息的方法。下面就说到了 待会大家看示例程序也会发现 主要就是看它怎么使用。 
Handler的主要的方法有： 
1)   public final boolean sendMessage(Message msg) 
把消息放入该Handler所 关联的消息队列，放置在消息队列尾部。 
这里请允许我多说一句就是 我们把消息放进去的一端 消息不会阻塞但是处理消息的一端就有可能会阻塞。欢迎收看 看图频道。 
![](http://dl.javaeye.com/upload/attachment/356346/f7aabb39-1dd9-365b-8f9e-87eb537a86fa.jpg)
2)   public void handleMessage(Message msg) 
    关联该消息队列的线 程将通过调用Handler的handleMessage方 法来接收和处理消息，通常需要子类化Handler来 实现handleMessage。
l  Looper 
Looper扮演着一个Handler和 消息队列之间通讯桥梁的角色。程序组件首先通过Handler把 消息传递给Looper，Looper把 消息放入队列。Looper也 把消息队列里的消息广播给所有的Handler，Handler接 受到消息后调用handleMessage进 行处理。
1)   可以通过Looper类 的静态方法Looper.myLooper得 到当前线程的Looper实 例，如果当前线程未关联一个Looper实 例，该方法将返回空（null）它不户抛空指针。
2)   可以通过静态方法Looper. getMainLooper方法得到主线程的Looper实 例 这里需要注意一下 主线程默认是有Lo是oper对象的。但是我们自己定义的子线程没有的。那么我们怎么在子线程中得到Looper对象呢？如果一个线程中调用Looper.prepare()，那么系统就会自动的为该线程建立一个消息队列，然后调用 Looper.loop();之后就进入了消息循环，这个之后就可以发消息、取消息、和处理消息。这个如何发送消息和如何处理消息可以再其他的线程中通过 Handle来做，但前提是我们的Hanle知道这个子线程的Looper，但是你如果不是在子线程运行
 Looper.myLooper()，一般是得不到子线程的looper的。 
好了示例演示时间到了。示例名称：线程交互
  这个例子主要向大家说明线程之间是如何进行交互的。请大家静下心来好好看看。如果你也是android新手的话。先说一下我们具体要实现什么功能，一共有三个按钮 第一个按钮用来设置一个 TextView上显示的值。当我们第一次点击它的时候它就会把值显示在TextView当我们在一点一下 就会把这个值清空。 什么也不显示了。这个按钮的 的背景图片我做了点处理。就是默认情况下这个按钮一张图片，按下按钮又会换成另外一张图片，松开按钮又会换一张图片。这是怎么做到的说完这个例子会给大家解释。至于线程之间是怎么传递消息的
 大家就看代码吧。 另外2个按钮时用来 开启和关闭音乐服务的 ，注意当我们点击开始按钮的时候 就会开始播放音乐 并且把歌曲的名称显示在一个TextView上。当我们点击关闭按钮的时候就会停止音乐的播放，好了废话不多说了大家看代码吧。
  O 对了 差点忘记了我们应该先收看 看图频道之运行效果。 
![](http://dl.javaeye.com/upload/attachment/356336/f8768118-8593-3094-8588-aa2b9ebb7357.jpg)
![](http://dl.javaeye.com/upload/attachment/356338/34fbce75-f052-3cc9-b4ee-3a54485abbe9.jpg)
![](http://dl.javaeye.com/upload/attachment/356340/0fba9a37-d410-3698-905c-ecbe953cb598.jpg)
![](http://dl.javaeye.com/upload/attachment/356342/cc8407d9-792f-3a83-959c-bbfa11ca9c98.jpg)
  好了下边我们来看布局文件 一共有2个布局文件 一个是用来定义我们的布局我这里用的是相对布局 RelativeLayout 下边是main.xml 
<?xml version="1.0" encoding="utf-8"?> 
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android" 
    android:orientation="vertical" 
    android:layout_width="fill_parent" 
    android:background="@drawable/bg0" 
    android:layout_height="fill_parent"> 
<TextView 
android:id="@+id/textview01" 
android:textColor="#FF0000" 
    android:layout_width="fill_parent" 
    android:layout_height="wrap_content" 
    android:text="@string/hello" 
    /> 
<Button 
android:id="@+id/myButton" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content" 
android:layout_below="@id/textview01" 
android:background="@drawable/newbtn" 
/> 
<TextView 
android:id="@+id/myTextView" 
android:textSize="15pt" 
android:layout_toRightOf="@id/myButton" 
android:layout_alignTop="@id/myButton" 
android:textColor="#FF0000" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content"/> 
<Button 
android:id="@+id/myButton01" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content" 
android:layout_below="@id/myButton" 
android:text="开始" 
/> 
<Button 
android:id="@+id/myButton02" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content" 
android:layout_toRightOf="@id/myButton01" 
android:layout_marginLeft="10px" 
android:layout_alignTop="@id/myButton01" 
android:text="停止" 
/> 
<TextView 
android:id="@+id/textview2" 
android:textSize="15pt" 
android:layout_below="@id/myTextView" 
android:layout_toRightOf="@id/myButton02" 
android:layout_alignTop="@id/myButton02" 
android:textColor="#385E0F" 
android:layout_width="wrap_content" 
android:layout_height="wrap_content"/> 
</RelativeLayout> 
    那么另外一个xml文件就是用来定义程序的第一个按钮的图片转换效果。这个怎么实现等这个例子讲解完了，会给大家做一个简单的说明。这个文件的名字是 newbtn.xml
<?xml version="1.0" encoding="utf-8"?> 
<selector xmlns:android="http://schemas.android.com/apk/res/android"> 
<item android:state_pressed="true" 
android:drawable="@drawable/im1"/> 
<item android:state_focused="true" 
android:drawable="@drawable/im2"/> 
<item android:drawable="@drawable/im3"/> 
</selector> 
   下面就是这2个类了 先来看看我们的音乐服务类 MusicService这个类比较简单。
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- **import** android.app.Service;   
- **import** android.content.Intent;   
- **import** android.media.MediaPlayer;   
- **import** android.os.IBinder;   
- 
- **public****class** MusicService **extends** Service{
   
- 
- //MediaPlayer对象
- **private** MediaPlayer player;   
- 
- @Override
- **public** IBinder onBind(Intent intent) {   
- // TODO Auto-generated method stub
- **return****null**;   
-     }   
- 
- **public****void** onStart(Intent intent, **int** startId){
   
- **super**.onStart(intent, startId);   
- //这里可以理解为装载音乐文件
-         player = MediaPlayer.create(**this**, R.raw.test);   
- //开始播放
-         player.start();   
-     }   
- 
- **public****void** onDestroy(){   
- **super**.onDestroy();   
- //停止音乐-停止Service
-         player.stop();   
-     }   
- }  
```java
package xiaohang.zhimeng;
import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;
public class MusicService extends Service{
	
	//MediaPlayer对象
	private MediaPlayer player;
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public void onStart(Intent intent, int startId){
		super.onStart(intent, startId);
		//这里可以理解为装载音乐文件
		player = MediaPlayer.create(this, R.raw.test);
		//开始播放
		player.start();
	}
	
	public void onDestroy(){
		super.onDestroy();
		//停止音乐-停止Service
		player.stop();
	}
}
```
  在来看我们实现线程交互的这个Activity02类 看看它是怎么完成 子线程与UI线程的交互的
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- **import** android.app.Activity;   
- **import** android.content.Intent;   
- **import** android.os.Bundle;   
- **import** android.os.Handler;   
- **import** android.os.Looper;   
- **import** android.os.Message;   
- **import** android.view.View;   
- **import** android.view.View.OnClickListener;   
- **import** android.widget.Button;   
- **import** android.widget.TextView;   
- 
- **public****class** Activity02 **extends** Activity {
   
- **private** Button myButton;//这个按钮就是我们的第一个按钮就是实现了图片变换的那个按钮
- **private** Button myButton01;//这个按钮是用来开启音乐服务的
- **private** Button myButton02;//这个按钮用来停止音乐服务
- //myTextView 由myButton控制这个TextView用来显示"android小子"  textView由myButton01控制这个TextView用来显示音乐名称
- **private** TextView myTextView, textView;   
- 
- // 得到主线程的Looper对象
-     Looper looper = Looper.myLooper();   
- // 这个myHandler由主线程创建所以它与主线程关联
-     MyHandler myHandler = **new** MyHandler(looper);   
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {   
- **super**.onCreate(savedInstanceState);   
- //设置我们的布局文件
-         setContentView(R.layout.main);   
- //得到我们的Button 按钮对象 和 TextView对象
-         myButton02 = (Button) findViewById(R.id.myButton02);   
- 
-         myButton = (Button) findViewById(R.id.myButton);   
-         myTextView = (TextView) findViewById(R.id.myTextView);   
- 
-         myButton01 = (Button) findViewById(R.id.myButton01);   
-         textView = (TextView) findViewById(R.id.textview2);   
- 
-         myButton.setOnClickListener(**new** MyButtonListener());   
-         myButton02.setOnClickListener(**new** MyButton02Listener());   
- //给我们的myButton按钮设置监听器
-         myButton01.setOnClickListener(**new** Button.OnClickListener() {   
- @Override
- **public****void** onClick(View v) {   
- **new** Thread() {   
- @Override
- **public****void** run() {   
-                         System.out.println("the thread id is "
-                                 + Thread.currentThread().getId());   
-                         System.out.println("the thread name is"
-                                 + Thread.currentThread().getName());   
- //获取我们资源文件里边的音乐名称 因为连路径也获取了 所以这里截取了一下
-                         String tmusicName = getResources().getString(   
-                                 R.raw.music_name);   
- **int** b = tmusicName.lastIndexOf("/");   
- **int** e = tmusicName.lastIndexOf(".");   
-                         String musicName = tmusicName.substring(b + 1, e);   
- // String musicName =
- // getResources().getResourceName(R.raw.test);
- /** 定义消息对象 并且赋予内容 最后发送给UI线程
-                           *这里这个方法要说明一下 myHandler.obtainMessage()-->obtainMessage这个方法用来获得消息对象
-                           *但是这个消息对象会关联调用它的Handler对象。所以就会关联myHandler对象 而这个myHandler对象是在UI线程里边创建的
-                           * 所以很显然我们这消息会发给主线程的消息队列 我想这对大家来说根本没有任何难度
-                           */
-                         Message message = myHandler.obtainMessage();   
-                         message.obj = musicName;   
- //这里这个 arg1 是Message对象携带的参数我主要用它来区分消息对象(Message)
-                         message.arg1 = 2;   
- //把消息发送给目标对象，目标对象就是 myHandler 就是关联我们得到的那个消息对象的Handler
-                         message.sendToTarget();   
- //开启音乐服务
-                         startService(**new** Intent(   
- "xiaohang.zhimeng.Android.MUSIC"));   
-                     }   
- //启动线程
-                 }.start();   
-             }   
-         });   
-     }   
- 
- // myButton02按钮的监听器
- **class** MyButton02Listener **implements** OnClickListener {   
- @Override
- **public****void** onClick(View v) {   
- **new** Thread() {   
- @Override
- **public****void** run() {   
-                     System.out.println("this thread id is "
-                             + Thread.currentThread().getId());   
-                     System.out.println("this thread name is "
-                             + Thread.currentThread().getName());   
- // 停止Service
-                     stopService(**new** Intent("xiaohang.zhimeng.Android.MUSIC"));
   
-                 }   
-             }.start();   
-         }   
-     }   
- 
- // myButton监听器 这个按钮用来设置 myTextView 的值 显示 或 隐藏 有2种状态
- **class** MyButtonListener **implements** OnClickListener {   
- @Override
- **public****void** onClick(View v) {   
-             CharSequence test_View = myTextView.getText();   
-             String str = test_View.toString();   
- // 当myTextView上边显示的文本不等于空的时候
- **if** (!(str.trim().equals(""))) {   
-                 String str1 = "";   
-                 CharSequence charsq = str1;   
-                 myTextView.setText(charsq);   
-             } **else** {   
-                 Runnable r = **new** Runnable() {   
- **public****void** run() {   
-                         Message message = myHandler.obtainMessage();   
-                         message.arg1 = 1;   
-                         message.obj = "android小子";   
-                         message.sendToTarget();   
-                         System.out.println("name is "
-                                 + Thread.currentThread().getName());   
-                         System.out.println("id is "
-                                 + Thread.currentThread().getId());   
-                     }   
-                 };   
-                 Thread thread = **new** Thread(r);   
-                 thread.start();   
-             }   
-         }   
-     }   
- //MyHandler继承Handler类 用过覆写handlerMessage方法 来处理发给附属于UI线程的消息队列的消息
- **class** MyHandler **extends** Handler {   
- **public** MyHandler() {}   
- **public** MyHandler(Looper looper) {   
- **super**(looper);   
-         }   
- @Override
- **public****void** handleMessage(Message msg) {   
- **if** (msg.arg1 == 1) {   
-                 CharSequence xh_msg = (CharSequence) msg.obj;   
-                 myTextView.setText(xh_msg);   
-             } **else****if** (msg.arg1 == 2) {
   
-                 CharSequence xh2_msg = (CharSequence) msg.obj;   
-                 textView.setText(xh2_msg);   
-             }   
-         }   
-     }   
- }  
```java
package xiaohang.zhimeng;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
public class Activity02 extends Activity {
	private Button myButton;//这个按钮就是我们的第一个按钮就是实现了图片变换的那个按钮
	private Button myButton01;//这个按钮是用来开启音乐服务的
	private Button myButton02;//这个按钮用来停止音乐服务
	//myTextView 由myButton控制这个TextView用来显示"android小子"  textView由myButton01控制这个TextView用来显示音乐名称
	private TextView myTextView, textView;
	
	// 得到主线程的Looper对象
	Looper looper = Looper.myLooper();
	// 这个myHandler由主线程创建所以它与主线程关联
	MyHandler myHandler = new MyHandler(looper);
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		//设置我们的布局文件
		setContentView(R.layout.main);
		//得到我们的Button 按钮对象 和 TextView对象
		myButton02 = (Button) findViewById(R.id.myButton02);
		myButton = (Button) findViewById(R.id.myButton);
		myTextView = (TextView) findViewById(R.id.myTextView);
		myButton01 = (Button) findViewById(R.id.myButton01);
		textView = (TextView) findViewById(R.id.textview2);
		myButton.setOnClickListener(new MyButtonListener());
		myButton02.setOnClickListener(new MyButton02Listener());
		//给我们的myButton按钮设置监听器
		myButton01.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				new Thread() {
					@Override
					public void run() {
						System.out.println("the thread id is "
								+ Thread.currentThread().getId());
						System.out.println("the thread name is"
								+ Thread.currentThread().getName());
						//获取我们资源文件里边的音乐名称 因为连路径也获取了 所以这里截取了一下
						String tmusicName = getResources().getString(
								R.raw.music_name);
						int b = tmusicName.lastIndexOf("/");
						int e = tmusicName.lastIndexOf(".");
						String musicName = tmusicName.substring(b + 1, e);
						// String musicName =
						// getResources().getResourceName(R.raw.test);
					    /**	定义消息对象 并且赋予内容 最后发送给UI线程
						  *这里这个方法要说明一下 myHandler.obtainMessage()-->obtainMessage这个方法用来获得消息对象
						  *但是这个消息对象会关联调用它的Handler对象。所以就会关联myHandler对象 而这个myHandler对象是在UI线程里边创建的
						  * 所以很显然我们这消息会发给主线程的消息队列 我想这对大家来说根本没有任何难度
						  */
						Message message = myHandler.obtainMessage();
						message.obj = musicName;
						//这里这个 arg1 是Message对象携带的参数我主要用它来区分消息对象(Message)
						message.arg1 = 2;
						//把消息发送给目标对象，目标对象就是 myHandler 就是关联我们得到的那个消息对象的Handler
						message.sendToTarget();
						//开启音乐服务
						startService(new Intent(
								"xiaohang.zhimeng.Android.MUSIC"));
					}
					//启动线程
				}.start();
			}
		});
	}
	// myButton02按钮的监听器
	class MyButton02Listener implements OnClickListener {
		@Override
		public void onClick(View v) {
			new Thread() {
				@Override
				public void run() {
					System.out.println("this thread id is "
							+ Thread.currentThread().getId());
					System.out.println("this thread name is "
							+ Thread.currentThread().getName());
					// 停止Service
					stopService(new Intent("xiaohang.zhimeng.Android.MUSIC"));
				}
			}.start();
		}
	}
	// myButton监听器 这个按钮用来设置 myTextView 的值 显示 或 隐藏 有2种状态
	class MyButtonListener implements OnClickListener {
		@Override
		public void onClick(View v) {
			CharSequence test_View = myTextView.getText();
			String str = test_View.toString();
			// 当myTextView上边显示的文本不等于空的时候
			if (!(str.trim().equals(""))) {
				String str1 = "";
				CharSequence charsq = str1;
				myTextView.setText(charsq);
			} else {
				Runnable r = new Runnable() {
					public void run() {
						Message message = myHandler.obtainMessage();
						message.arg1 = 1;
						message.obj = "android小子";
						message.sendToTarget();
						System.out.println("name is "
								+ Thread.currentThread().getName());
						System.out.println("id is "
								+ Thread.currentThread().getId());
					}
				};
				Thread thread = new Thread(r);
				thread.start();
			}
		}
	}
	//MyHandler继承Handler类 用过覆写handlerMessage方法 来处理发给附属于UI线程的消息队列的消息
	class MyHandler extends Handler {
		public MyHandler() {}
		public MyHandler(Looper looper) {
			super(looper);
		}
		@Override
		public void handleMessage(Message msg) {
			if (msg.arg1 == 1) {
				CharSequence xh_msg = (CharSequence) msg.obj;
				myTextView.setText(xh_msg);
			} else if (msg.arg1 == 2) {
				CharSequence xh2_msg = (CharSequence) msg.obj;
				textView.setText(xh2_msg);
			}
		}
	}
}
```
  例子的运行效果大家就下载源码自行测试吧。 
  下面说说我们上边的那个按钮怎么设置那种图片变换的效果。其实很简单的，因为我们都知道按钮有 三种状态 分别是 当我们的应用程序启动的时候 有一种状态默认状态。 还有就是当我们按下按钮的时候一种状态， 最后一种状态就是 当我们松开这个按钮的时候也是一种状态。所有一共有三种状态。 我们要实现这种效果无非就是 当不同状态的时候去引用不同的图片资源。 来实现这种效果。
这里我找个一个文档来说明这种配置文件的一些属性 以及用法。E文的，小的英文灰常之差劲啊。看着太太给力了。基本不懂。
[http://idunnolol.com/android/drawables.html#selector](http://idunnolol.com/android/drawables.html#selector)
至于到底该怎么配置和设置大家看例子就是了 这里就不过多解释了。 
   累了吗？可以去喝喝茶。回来我们继续。下面我们 来看看人人皆知的AsyncTasks类 Async就是异步的意思，然后Task是任务的意思。翻译过来就是 异步任务了。首先我要告诉你它是用来捣鼓线程的。 这是你必须要一定要产生一个疑问就是 这个类的处理方式和我们上边写的那种 用 Hadler Looper MessageQueue 处理的方式有什么不同？？？ 好了这个类我们是必须会。必须会用。必须地。
       其实平常一般都是使用AsyncTask的，而并非Thread和Handler去更新UI，这里说下它们到底有什么区别，我们平时应该使用哪种解决方案。从Android 1.5开始系统将AsyncTask引入到android.os包中，过去在很早1.1和1.0 SDK时其实官方将其命名为UserTask，其内部是JDK 1.5开始新增的concurrent库，做过J2EE的网友可能明白并发库效率和强大性，比Java原始的Thread更灵活和强大，但对于轻量级的使用更为占用系统资源。Thread是Java早期为实现多线程而设计的，比较简单不支持concurrent中很多特性在同步和线程池类中需要自己去实现很多的东西，对于分布式应用来说更需要自己写调度代码，而为了Android
 UI的刷新Google引入了Handler和Looper机制，它们均基于消息实现，有时可能消息队列阻塞或其他原因无法准确的使用。 
   推荐大家使用AsyncTask代替Thread+Handler的方式，不仅调用上更为简单，经过实测更可靠一些，Google在Browser中大量使用了异步任务作为处理耗时的I/O操作，比如下载文件、读写数据库等等，它们在本质上都离不开消息，但是AsyncTask相比Thread加Handler更为可靠，更易于维护，但AsyncTask缺点也是有的比如一旦线程开启即dobackground方法执行后无法给线程发送消息，仅能通过预先设置好的标记来控制逻辑，当然可以通过线程的挂起等待标志位的改变来通讯，对于某些应用Thread和Handler以及Looper可能更灵活。
     那么先让我们看看要使用AsyncTask 类首先要做哪些工作？ 
      1)  子类化AsyncTask 
      2)  实现AsyncTask中定义的下面一个或几个方法 
         onPreExecute() 开始执行前的准备工作； 
         doInBackground(Params...) 开始执行后台处理，可以调用publishProgress方法来更新实时的任务进度； 
         onProgressUpdate(Progress...)  在publishProgress方法被调用后，UI thread将调用这个方法从而在界面上展示任务的进展情况，例如通过一个进度条进行展示。
         onPostExecute(Result) 执行完成后的操作，传送结果给UI 线程。 
         这4个方法都不能手动调用。而且除了doInBackground(Params...)方法，其余3个方法都是被UI线程所调用的，所以要求： 
        1) AsyncTask的实例必须在UI thread中创建； 
        2) AsyncTask.execute方法必须在UI thread中调用； 
    同时要注意：该task只能被执行一次，否则多次调用时将会出现异常。而且是不能手动停止的，这一点要注意，看是否符合你的需求！ 
    在使用过程中，发现AsyncTask的构造函数的参数设置需要看明白：AsyncTask<Params, Progress, Result> 
    Params对应doInBackground(Params...)的参数类型。而new AsyncTask().execute(Params... params)，就是传进来的Params数据，你可以execute(data)来传送一个数据，或者execute(data1, data2, data3)这样多个数据。
    Progress对应onProgressUpdate(Progress...)的参数类型； 
    Result对应onPostExecute(Result)的参数类型。 
    当以上的参数类型都不需要指明某个时，则使用Void，注意不是void。 
     说到这里就得多说几句 你看AsyncTask 你就会发现这个类是个泛型类 这个类是这样定义的。
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **public****abstract****class** AsyncTask<Params, Progress, Result> {}  
```java
public abstract class AsyncTask<Params, Progress, Result> {}
```
在把execute方法的源码 贴上来供大家参考 。
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **public****final** AsyncTask<Params, Progress, Result> execute(Params... params) {
   
- **if** (mStatus != Status.PENDING) {   
- **switch** (mStatus) {   
- **case** RUNNING:   
- **throw****new** IllegalStateException("Cannot execute task:"
-                            + " the task is already running.");   
- **case** FINISHED:   
- **throw****new** IllegalStateException("Cannot execute task:"
-                            + " the task has already been executed "
-                            + "(a task can be executed only once)");   
-            }   
-        }   
- 
-        mStatus = Status.RUNNING;   
- 
-        onPreExecute();   
- 
-        mWorker.mParams = params;   
-        sExecutor.execute(mFuture);   
- 
- **return****this**;   
-    }  
```java
public final AsyncTask<Params, Progress, Result> execute(Params... params) {
        if (mStatus != Status.PENDING) {
            switch (mStatus) {
                case RUNNING:
                    throw new IllegalStateException("Cannot execute task:"
                            + " the task is already running.");
                case FINISHED:
                    throw new IllegalStateException("Cannot execute task:"
                            + " the task has already been executed "
                            + "(a task can be executed only once)");
            }
        }
        mStatus = Status.RUNNING;
        onPreExecute();
        mWorker.mParams = params;
        sExecutor.execute(mFuture);
        return this;
    }
```
      这个类判断哪里我也不知道它在做什么没有继续往下深入的研究 内力不足。 显然这个类返回一个AsyncTask实例。 这里我要提醒大家的的是 onPreExecute()；方法  就是告知大家这个前置方法是什么时候执行的 当执行这个方法的时候 AsyncTask的工作流程就开始了。这里我没有用到这个方法就没有重写了。但是大家一定要知道有这么一个方法 ，它可以用来做一些准备和初始化的工作。刚才说到它的工作流程其实就是 这几个方法的执行顺序我就以图片的形式告知大家。 让大家明白它们相互之间的执行顺序，这里一定一定
 要搞清楚。又得点击大图查看图片了。 
![](http://dl.javaeye.com/upload/attachment/356348/f3159ac6-726e-321a-9cd5-54f403301d58.jpg)
     好了我们来看例子 我们这个例子 与上一个例子实现的效果是一样的所以这里就不贴运行效果了。其实 是稍微有些不同的 有2点不同 上边那个例子 我们的第一个按钮 第一个按钮就是最上边的那个按钮 刚开始是没有显示TextView 我们点击第一个按钮就会显示TextView并且会开启一个线程来执行 当我们再点击的时候 TextView没有值了 但是我们 这次点击并不会另外开启一个线程。 而我们这个例子会。第一个例子(线程交互) 就是显示TextView值的时候就会开启一个线程。当我们把TextView的值 置空的时候不会开启另外一个线程来执行。
第二点区别就是 我们这个例子 点击停止按钮之后 我们的音乐名也消失。其实也是把TextView显示的值 置空了。 下面我们就来看看这个例子 因为布局都是一样的 我在这里就只贴出了2个类。 大家可以下载源码进行测试。
示例名称 ：线程交互AsyncTask版
  MusicServie类 和 上一个例子一样的
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- **import** android.app.Service;   
- **import** android.content.Intent;   
- **import** android.media.MediaPlayer;   
- **import** android.os.IBinder;   
- 
- **public****class** MusicService **extends** Service{
   
- 
- //MediaPlayer对象
- **private** MediaPlayer player;   
- 
- @Override
- **public** IBinder onBind(Intent intent) {   
- // TODO Auto-generated method stub
- **return****null**;   
-     }   
- 
- **public****void** onStart(Intent intent, **int** startId){
   
- **super**.onStart(intent, startId);   
- //这里可以理解为装载音乐文件
-         player = MediaPlayer.create(**this**, R.raw.test);   
- //开始播放
-         player.start();   
-     }   
- 
- **public****void** onDestroy(){   
- **super**.onDestroy();   
- //停止音乐-停止Service
-         player.stop();   
-     }   
- }  
```java
package xiaohang.zhimeng;
import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.IBinder;
public class MusicService extends Service{
	
	//MediaPlayer对象
	private MediaPlayer player;
	
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	public void onStart(Intent intent, int startId){
		super.onStart(intent, startId);
		//这里可以理解为装载音乐文件
		player = MediaPlayer.create(this, R.raw.test);
		//开始播放
		player.start();
	}
	
	public void onDestroy(){
		super.onDestroy();
		//停止音乐-停止Service
		player.stop();
	}
}
```
Activity02类
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- 
- **import** android.app.Activity;   
- **import** android.content.Intent;   
- **import** android.os.AsyncTask;   
- **import** android.os.Bundle;   
- **import** android.view.View;   
- **import** android.view.View.OnClickListener;   
- **import** android.widget.Button;   
- **import** android.widget.TextView;   
- 
- **public****class** Activity02 **extends** Activity {
   
- **public****static**  String ARGS1;   
- **private** Button myButton;   
- **private** Button myButton01;   
- **private** Button myButton02;   
- **private** TextView myTextView, textView;   
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {   
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.main);   
- 
-         myTextView = (TextView) findViewById(R.id.myTextView);   
-         textView = (TextView) findViewById(R.id.textview2);   
- 
-         myButton01 = (Button) findViewById(R.id.myButton01);   
-         myButton02 = (Button) findViewById(R.id.myButton02);   
-         myButton = (Button) findViewById(R.id.myButton);   
-         myButton.setOnClickListener(**new** MyButtonListener());   
-         myButton02.setOnClickListener(**new** MyButton02Listener());   
-         myButton01.setOnClickListener(**new** Button.OnClickListener() {   
- @Override
- **public****void** onClick(View v) {   
-                 String arg2 = Activity02.ARGS1 = "2";   
- **new** task().execute(arg2);   
-             }   
-         });   
-     }   
- 
- // myButton02按钮的监听器
- **class** MyButton02Listener **implements** OnClickListener {   
- @Override
- **public****void** onClick(View v) {   
-                 String arg3 = Activity02.ARGS1 = "3";   
- **new** task().execute(arg3);   
-         }   
-     }   
- //此类继承AsyncTask
- **private****class** task **extends** AsyncTask<String,String, String>{
   
- //开启另外一个线程执行任务
- @Override
- **protected** String doInBackground(String...params) {   
-             System.out.println("the thread ID is " + Thread.currentThread().getId());   
-             System.out.println("the thread NAME is " + Thread.currentThread().getName());   
-             String test = params[0];   
-             System.out.println("test value is " + test);   
- **if** (params[0] == "1") {
   
-                 String arg1 = params[0];   
- **return** arg1;   
-             }**else****if**(params[0] == "2"){
   
- //开启音乐服务
-                 startService(**new** Intent(   
- "xiaohang.zhimeng.Android.MUSIC"));   
- //获取参数值
-                 String arg2 = params[0];   
- **return** arg2;   
-             }**else****if** (params[0] == "3") {
   
- //停止音乐服务
-                 stopService(**new** Intent("xiaohang.zhimeng.Android.MUSIC"));   
- //获取参数值
-                 String arg3 = params[0];   
- **return** arg3;   
-             }**else** {   
-                 System.out.println("param post error---->");                
-             }   
- **return****null**;   
-         }   
- 
- //执行完成后传送结果给UI线程 此方法最后执行
- **protected****void** onPostExecute(String result) {   
-             CharSequence test_View = myTextView.getText();   
-             String str = test_View.toString();   
- // 当不等于空的时候
- **if** (!(str.trim().equals(""))) {   
-                 String str1 = "";   
-                 CharSequence charsq = str1;   
-                 myTextView.setText(charsq);   
-             } **else** {   
- **if** (result == "1") {   
- //将 myTextView 赋值成 "android小子"
-                     String string = "android小子";   
-                     myTextView.setText(string);   
-                 }**else****if**(result == "2"){//result等于2说明是myButton01那个按钮也就是开启音乐服务的那个开始按钮
- //获取资源文件里边的音乐名称
-                     String tmusicName = getResources().getString(R.raw.test);   
- **int** b = tmusicName.lastIndexOf("/");   
- **int** e = tmusicName.lastIndexOf(".");   
- //截取歌曲名称
-                     String music_Name = tmusicName.substring(b+1, e);   
-                     System.out.println("music name is " + music_Name);   
- // 在textView上显示歌曲名称
-                     textView.setText(music_Name);   
-                 }**else****if**(result == "3"){
   
-                     String str_none = "";   
-                     CharSequence csq = str_none;   
-                     textView.setText(csq);   
-                 }**else** {   
-                     System.out.println("param post error");   
-                 }   
-             }   
-         }   
-     }   
- 
- // myButton监听器 这个按钮用来 显示和隐藏 值为"android小子"这个TextView
- **public****class** MyButtonListener **implements** OnClickListener {
   
- @Override
- **public****void** onClick(View v) {   
-             System.out.println("wo zhen de zhixing fou???");   
- //给静态变量赋值
-             String arg1 = Activity02.ARGS1 = "1";   
- **new** task().execute(arg1);   
-         }   
-     }   
- }  
```java
package xiaohang.zhimeng;
import android.app.Activity;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
public class Activity02 extends Activity {
	public static  String ARGS1;
	private Button myButton;
	private Button myButton01;
	private Button myButton02;
	private TextView myTextView, textView;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		myTextView = (TextView) findViewById(R.id.myTextView);
		textView = (TextView) findViewById(R.id.textview2);
		myButton01 = (Button) findViewById(R.id.myButton01);
		myButton02 = (Button) findViewById(R.id.myButton02);
		myButton = (Button) findViewById(R.id.myButton);
		myButton.setOnClickListener(new MyButtonListener());
		myButton02.setOnClickListener(new MyButton02Listener());
		myButton01.setOnClickListener(new Button.OnClickListener() {
			@Override
			public void onClick(View v) {
				String arg2 = Activity02.ARGS1 = "2";
				new task().execute(arg2);
			}
		});
	}
	// myButton02按钮的监听器
	class MyButton02Listener implements OnClickListener {
		@Override
		public void onClick(View v) {
				String arg3 = Activity02.ARGS1 = "3";
			    new task().execute(arg3);
		}
	}
	//此类继承AsyncTask
	private class task extends AsyncTask<String,String, String>{
		//开启另外一个线程执行任务
		@Override
		protected String doInBackground(String...params) {
			System.out.println("the thread ID is " + Thread.currentThread().getId());
			System.out.println("the thread NAME is " + Thread.currentThread().getName());
			String test = params[0];
			System.out.println("test value is " + test);
			if (params[0] == "1") {
				String arg1 = params[0];
				return arg1;
			}else if(params[0] == "2"){
				//开启音乐服务
				startService(new Intent(
				"xiaohang.zhimeng.Android.MUSIC"));
				//获取参数值
				String arg2 = params[0];
				return arg2;
			}else if (params[0] == "3") {
				//停止音乐服务
				stopService(new Intent("xiaohang.zhimeng.Android.MUSIC"));
				//获取参数值
				String arg3 = params[0];
				return arg3;
			}else {
				System.out.println("param post error---->");				
			}
			return null;
		}
		
		//执行完成后传送结果给UI线程 此方法最后执行
		protected void onPostExecute(String result) {
			CharSequence test_View = myTextView.getText();
			String str = test_View.toString();
			// 当不等于空的时候
			if (!(str.trim().equals(""))) {
				String str1 = "";
				CharSequence charsq = str1;
				myTextView.setText(charsq);
			} else {
				if (result == "1") {
					//将 myTextView 赋值成 "android小子"
					String string = "android小子";
					myTextView.setText(string);
				}else if(result == "2"){//result等于2说明是myButton01那个按钮也就是开启音乐服务的那个开始按钮
					//获取资源文件里边的音乐名称
					String tmusicName = getResources().getString(R.raw.test);
					int b = tmusicName.lastIndexOf("/");
					int e = tmusicName.lastIndexOf(".");
					//截取歌曲名称
					String music_Name = tmusicName.substring(b+1, e);
					System.out.println("music name is " + music_Name);
					// 在textView上显示歌曲名称
					textView.setText(music_Name);
				}else if(result == "3"){
					String str_none = "";
					CharSequence csq = str_none;
					textView.setText(csq);
				}else {
					System.out.println("param post error");
				}
			}
		}
	}
	
	// myButton监听器 这个按钮用来 显示和隐藏 值为"android小子"这个TextView
	public class MyButtonListener implements OnClickListener {
		@Override
		public void onClick(View v) {
			System.out.println("wo zhen de zhixing fou???");
			//给静态变量赋值
			String arg1 = Activity02.ARGS1 = "1";
		    new task().execute(arg1);
		}
	}
}
```
  运行效果大家就得下载源码运行了，程序有里边System.out 的输出我用来查看一些操作是在那些线程中执行的。大家可以自行过滤。我配置的System.out如下图所示
![](http://dl.javaeye.com/upload/attachment/356344/4d45244f-00e9-3558-b90e-1d4ed57a7254.jpg)
   下边也是一个AsyncTask的使用示例 来自eoe  示例名称：AsyncTask_eoe
   只有一个类，类名testAsync
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** xiaohang.zhimeng;   
- 
- **import** android.app.Activity;   
- **import** android.os.AsyncTask;   
- **import** android.os.Bundle;   
- **import** android.os.Handler;   
- **import** android.os.Message;   
- **import** android.os.SystemClock;   
- **import** android.widget.TextView;   
- **import** android.widget.Toast;   
- 
- /** 
-  * 一个使用异步任务的例子。一般来说一个异步任务只执行一次，这个例子有点非主流，任务结束后会触发下一次任务执行。
-  * 由任务task在屏幕上打印数字，第一次任务执行由主Activity的onCreate触发，每次任务结束后
-  * 设定下一次触发的时间，共执行5次。对于任务来说doInBackground()接收任务的参数params，并执行产生数字的动作，每一个数字
-  * 产生后调用一次publishProgress()来更新UI，这个函数本身也是异步的只是用来发个消息调用完成后立即返回，
-  * 而产生数字的动作在继续进行。更新界面的操作在onProgressUpdate()中设定。 所有的on函数都由系统调用，不能用户调用。
-  * 代码中使用Handler是为了能触发任务执行，android规定这种异步任务每次执行完就结束，若要重新执行需要new一个新的。
-  * 异步任务只能在UI线程里面创建和执行
-  */
- **public****class** testAsync **extends** Activity {
   
- **private****final****int** MSG_TIMER = 10000;
   
- **private** TextView vText = **null**;   
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {   
- // TODO Auto-generated method stub
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.test);   
-         vText = (TextView) findViewById(R.id.TextView01);   
-         vText.setText("Num...");   
- 
- **new** task().execute("->");   
- 
-     }   
- 
- // 接收任务task发来的消息，触发一个新的任务
- **private****final** Handler handler = **new** Handler() {
   
- 
- @Override
- **public****void** handleMessage(Message msg) {   
- // TODO Auto-generated method stub
- **super**.handleMessage(msg);   
-             System.out.println("Handler name -----------> " + Thread.currentThread().getName());   
-             System.out.println("Handler id ------------> " + Thread.currentThread().getId());   
- **switch** (msg.what) {   
- **case** MSG_TIMER:   
- **new** task().execute("->");   
- **break**;   
-             }   
-         }   
-     };   
- 
- // 任务执行次数 
- **private****static****int** times = 1;
   
- 
- // AsyncTask<>的参数类型由用户设定，这里设为三个String
- // 第一个String代表输入到任务的参数类型，也即是doInBackground()的参数类型
- // 第二个String代表处理过程中的参数类型，也就是doInBackground()执行过程中的产出参数类型，通过publishProgress()发消息
- // 传递给onProgressUpdate()一般用来更新界面
- // 第三个String代表任务结束的产出类型，也就是doInBackground()的返回值类型，和onPostExecute()的参数类型
- **private****class** task **extends** AsyncTask<String, String, String> {
   
- 
- // 后台执行的耗时任务，接收参数并返回结果
- // 当onPostExecute()执行完，在后台线程中被系统调用
- @Override
- **protected** String doInBackground(String... params) {   
-             System.out.println("doInBackground name -----> " + Thread.currentThread().getName());   
-             System.out.println("doInBackground  id -----> " + Thread.currentThread().getId());   
- // TODO Auto-generated method stub
- // 在这里产生数据，送给onProgressUpdate以更新界面
-             String pre = params[0];   
-             System.out.println("pre is ----->" + pre);   
- 
- **for** (**int** i = 0; i < 5; i++) {
   
-                 System.out.println("note i am begin sleep ");   
-                 publishProgress(pre + i);   
- 
- 
- // 这里是否需要停顿下
-                 System.out.println("hua li de bu zhuo " + pre + i);   
-                 SystemClock.sleep(1000);   
-             }   
- 
- **return**"任务结束";   
-         }   
- 
- // 任务执行结束后，在UI线程中被系统调用
- // 一般用来显示任务已经执行结束
- @Override
- **protected****void** onPostExecute(String result) {   
- // TODO Auto-generated method stub
-             System.out.println("onPostExecute name --------> " + Thread.currentThread().getName());   
-             System.out.println("onPostExecute id --------> " + Thread.currentThread().getName());   
- **super**.onPostExecute(result);   
- 
-             Toast.makeText(testAsync.**this**, result, Toast.LENGTH_SHORT).show();   
- 
- // 任务执行5次后推出
- **if** (times > 5) {   
- **return**;   
-             }   
- 
- // 设定下一次任务触发时间
-             Message msg = Message.obtain();   
-             msg.what = MSG_TIMER;    
-             handler.sendMessageDelayed(msg, 10000L);   
-         }   
- 
- // 最先执行，在UI线程中被系统调用
- // 一般用来在UI中产生一个进度条
- @Override
- **protected****void** onPreExecute() {   
- // TODO Auto-generated method stub
-             System.out.println("onPreExecute id -------> " + Thread.currentThread().getId());   
-             System.out.println("onPreExecute name -------> " + Thread.currentThread().getName() );   
- **super**.onPreExecute();   
-             Toast.makeText(testAsync.**this**, "开始执行第" + times + "次任务: " + **this**,
   
-                     Toast.LENGTH_SHORT).show();   
-             times++;   
-         }   
- 
- // 更新界面操作，在收到更新消息后，在UI线程中被系统调用
- @Override
- **protected****void** onProgressUpdate(String... values) {
   
- // TODO Auto-generated method stub
-             System.out.println("onProgressUpdate id ---------> " + Thread.currentThread().getId());   
-             System.out.println("onProgressUpdate name -------> " + Thread.currentThread().getName());   
- **super**.onProgressUpdate(values);   
-             vText.append(values[0]);   
-         }   
- 
-     }   
- 
- }  
```java
package xiaohang.zhimeng;
import android.app.Activity;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.widget.TextView;
import android.widget.Toast;
/**
 * 一个使用异步任务的例子。一般来说一个异步任务只执行一次，这个例子有点非主流，任务结束后会触发下一次任务执行。
 * 由任务task在屏幕上打印数字，第一次任务执行由主Activity的onCreate触发，每次任务结束后
 * 设定下一次触发的时间，共执行5次。对于任务来说doInBackground()接收任务的参数params，并执行产生数字的动作，每一个数字
 * 产生后调用一次publishProgress()来更新UI，这个函数本身也是异步的只是用来发个消息调用完成后立即返回，
 * 而产生数字的动作在继续进行。更新界面的操作在onProgressUpdate()中设定。 所有的on函数都由系统调用，不能用户调用。
 * 代码中使用Handler是为了能触发任务执行，android规定这种异步任务每次执行完就结束，若要重新执行需要new一个新的。
 * 异步任务只能在UI线程里面创建和执行
 */
public class testAsync extends Activity {
	private final int MSG_TIMER = 10000;
	private TextView vText = null;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.test);
		vText = (TextView) findViewById(R.id.TextView01);
		vText.setText("Num...");
		new task().execute("->");
	}
	// 接收任务task发来的消息，触发一个新的任务
	private final Handler handler = new Handler() {
		@Override
		public void handleMessage(Message msg) {
			// TODO Auto-generated method stub
			super.handleMessage(msg);
			System.out.println("Handler name -----------> " + Thread.currentThread().getName());
			System.out.println("Handler id ------------> " + Thread.currentThread().getId());
			switch (msg.what) {
			case MSG_TIMER:
				new task().execute("->");
				break;
			}
		}
	};
	// 任务执行次数
	private static int times = 1;
	// AsyncTask<>的参数类型由用户设定，这里设为三个String
	// 第一个String代表输入到任务的参数类型，也即是doInBackground()的参数类型
	// 第二个String代表处理过程中的参数类型，也就是doInBackground()执行过程中的产出参数类型，通过publishProgress()发消息
	// 传递给onProgressUpdate()一般用来更新界面
	// 第三个String代表任务结束的产出类型，也就是doInBackground()的返回值类型，和onPostExecute()的参数类型
	private class task extends AsyncTask<String, String, String> {
		
		// 后台执行的耗时任务，接收参数并返回结果
		// 当onPostExecute()执行完，在后台线程中被系统调用
		@Override
		protected String doInBackground(String... params) {
			System.out.println("doInBackground name -----> " + Thread.currentThread().getName());
			System.out.println("doInBackground  id -----> " + Thread.currentThread().getId());
			// TODO Auto-generated method stub
			// 在这里产生数据，送给onProgressUpdate以更新界面
			String pre = params[0];
			System.out.println("pre is ----->" + pre);
			for (int i = 0; i < 5; i++) {
				System.out.println("note i am begin sleep ");
				publishProgress(pre + i);
					
				
				// 这里是否需要停顿下
				System.out.println("hua li de bu zhuo " + pre + i);
				SystemClock.sleep(1000);
			}
			return "任务结束";
		}
		// 任务执行结束后，在UI线程中被系统调用
		// 一般用来显示任务已经执行结束
		@Override
		protected void onPostExecute(String result) {
			// TODO Auto-generated method stub
			System.out.println("onPostExecute name --------> " + Thread.currentThread().getName());
			System.out.println("onPostExecute id --------> " + Thread.currentThread().getName());
			super.onPostExecute(result);
			Toast.makeText(testAsync.this, result, Toast.LENGTH_SHORT).show();
			// 任务执行5次后推出
			if (times > 5) {
				return;
			}
			// 设定下一次任务触发时间
			Message msg = Message.obtain();
			msg.what = MSG_TIMER; 
			handler.sendMessageDelayed(msg, 10000L);
		}
		// 最先执行，在UI线程中被系统调用
		// 一般用来在UI中产生一个进度条
		@Override
		protected void onPreExecute() {
			// TODO Auto-generated method stub
			System.out.println("onPreExecute id -------> " + Thread.currentThread().getId());
			System.out.println("onPreExecute name -------> " + Thread.currentThread().getName() );
			super.onPreExecute();
			Toast.makeText(testAsync.this, "开始执行第" + times + "次任务: " + this,
					Toast.LENGTH_SHORT).show();
			times++;
		}
		// 更新界面操作，在收到更新消息后，在UI线程中被系统调用
		@Override
		protected void onProgressUpdate(String... values) {
			// TODO Auto-generated method stub
			System.out.println("onProgressUpdate id ---------> " + Thread.currentThread().getId());
			System.out.println("onProgressUpdate name -------> " + Thread.currentThread().getName());
			super.onProgressUpdate(values);
			vText.append(values[0]);
		}
	}
}
```
   这个例子来自eoe大家可以好好看看 每个回调方法它都用到了不错的。还有xh在这里建议大家去看看泛型的东西 我以前不会 也看了看。不然 自己用AsyncTask类的时候 那几个方法的参数 会有些晕。我现在还有些晕，有时间一定要好好研究一下。然后接下来的就是多多练习了。
  下面这例子也是线程交互的一个例子和上边的稍有不同大家看看吧，在这个例子中子线程和主线程交互 传送数据 是通过Bundle对象的方式。在这个例子中大家可以学会怎么用Bundle对象。
示例名称：线程交互_Bundle版
  只有一个类HandleTest2 
Java代码 ![复制代码](http://byandby.javaeye.com/images/icon_copy.gif)
- **package** mars.handler;   
- 
- **import** android.app.Activity;   
- **import** android.os.Bundle;   
- **import** android.os.Handler;   
- **import** android.os.HandlerThread;   
- **import** android.os.Looper;   
- **import** android.os.Message;   
- 
- **public****class** HandlerTest2 **extends** Activity {
   
- 
- @Override
- **protected****void** onCreate(Bundle savedInstanceState) {   
- // TODO Auto-generated method stub
- **super**.onCreate(savedInstanceState);   
-         setContentView(R.layout.main);   
- // 打印了当前线程的ID
-         System.out.println("Activity-->" + Thread.currentThread().getId());   
- // HandlerThread这个类继承了Thread
- // 生成一个HandlerThread对象，实现了使用Looper来处理消息队列的功能，这个类由Android应用程序框架提供
- // 这里这个参数指定的是线程的名字
-         HandlerThread handlerThread = **new** HandlerThread("handler_thread");   
- // 在使用HandlerThread的getLooper()方法之前，必须先调用该类的start();
-         handlerThread.start();// 启动这个线程
-         MyHandler myHandler = **new** MyHandler(handlerThread.getLooper());   
- // 这里这个obtainMessage()方法返回
-         Message msg = myHandler.obtainMessage();   
- // 将msg发送到目标对象，所谓的目标对象，就是生成该msg对象的handler对象
-         Bundle b = **new** Bundle();   
-         b.putInt("age", 20);   
-         b.putString("name", "Jhon");   
-         msg.setData(b);   
-         msg.sendToTarget();   
-     }   
- 
- **class** MyHandler **extends** Handler {   
- **public** MyHandler() {   
- 
-         }   
- 
- **public** MyHandler(Looper looper) {   
- **super**(looper);   
-         }   
- 
- @Override
- **public****void** handleMessage(Message msg) {   
- // 这里我们只做了简单的输出
-             Bundle b = msg.getData();   
- **int** age = b.getInt("age");   
-             String name = b.getString("name");   
-             System.out.println("age is " + age + ", name is" + name);   
-             System.out.println("Handler--->" + Thread.currentThread().getId());   
-             System.out.println("handlerMessage");   
-         }   
-     }   
- }  
```java
package mars.handler;
import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Looper;
import android.os.Message;
public class HandlerTest2 extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		// 打印了当前线程的ID
		System.out.println("Activity-->" + Thread.currentThread().getId());
		// HandlerThread这个类继承了Thread
		// 生成一个HandlerThread对象，实现了使用Looper来处理消息队列的功能，这个类由Android应用程序框架提供
		// 这里这个参数指定的是线程的名字
		HandlerThread handlerThread = new HandlerThread("handler_thread");
		// 在使用HandlerThread的getLooper()方法之前，必须先调用该类的start();
		handlerThread.start();// 启动这个线程
		MyHandler myHandler = new MyHandler(handlerThread.getLooper());
		// 这里这个obtainMessage()方法返回
		Message msg = myHandler.obtainMessage();
		// 将msg发送到目标对象，所谓的目标对象，就是生成该msg对象的handler对象
		Bundle b = new Bundle();
		b.putInt("age", 20);
		b.putString("name", "Jhon");
		msg.setData(b);
		msg.sendToTarget();
	}
	class MyHandler extends Handler {
		public MyHandler() {
		}
		public MyHandler(Looper looper) {
			super(looper);
		}
		@Override
		public void handleMessage(Message msg) {
			// 这里我们只做了简单的输出
			Bundle b = msg.getData();
			int age = b.getInt("age");
			String name = b.getString("name");
			System.out.println("age is " + age + ", name is" + name);
			System.out.println("Handler--->" + Thread.currentThread().getId());
			System.out.println("handlerMessage");
		}
	}
}
```
  布局文件 啥都没有就一个TextView 为了 维持完成也贴出来吧。 
<?xml version="1.0" encoding="utf-8"?> 
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android" 
    android:orientation="vertical" 
    android:layout_width="fill_parent" 
    android:layout_height="fill_parent" 
    > 
<TextView  
    android:layout_width="fill_parent" 
    android:layout_height="wrap_content" 
    android:text="@string/hello" 
    /> 
</LinearLayout> 
  这里有一些我学习的时候在网上参考的文章，大家也可以看看 ，重要的就是多练习了。 
  浅析Android线程模型[http://www.cppblog.com/fwxjj/archive/2010/05/31/116787.html](http://www.cppblog.com/fwxjj/archive/2010/05/31/116787.html)
   Android线程模型（Painless Threading）[http://www.cppblog.com/fwxjj/archive/2010/05/31/116788.html](http://www.cppblog.com/fwxjj/archive/2010/05/31/116788.html)
   有关Android线程的学习 [http://android.blog.51cto.com/268543/343823](http://android.blog.51cto.com/268543/343823)
   Android中Message机制的灵活应用[http://qaohao.javaeye.com/blog/509145](http://qaohao.javaeye.com/blog/509145)
    http://wghjay.javaeye.com/blog/427086[http://wghjay.javaeye.com/blog/427086](http://wghjay.javaeye.com/blog/427086)
    这些我认为写的还不错，大家 也可以看看 多多学习总是好事。 
下边罗列我们源码的下载地址 因为线程交互的那2个例子里边有一些资源文件所以会比较大eye传不上来 ，我传到趣盘了给大家下载。
   xh_Android_Test_1_2_线程交互     下载地址 趣盘下载 [http://hangvip.qupan.cc/6789636.html](http://hangvip.qupan.cc/6789636.html)
    XH_Android_Test_1_2_SyncTask_Demo_线程交互_AsyncTask版   [http://hangvip.qupan.cc/6789683.html](http://hangvip.qupan.cc/6789683.html)
    Android_Test_01_AsyncTask_eoe       附件下载 
    建议大家用IE下载直接把链接复制到IE下载 然后点迅雷下载 如果没安装迅雷 会弹出普通的下载方式，而火狐 貌似 只能用迅雷下了 还有趣盘有很多广告很恶心，大家忍住。
    所有例子的测试平台 均为 android2.0  APILEVEL 5  下载链接已测试可用 ，如果大家发现不可以下载可以留言给我。 
  到此这篇长长的文章就写完了。   睡觉去。![](http://writeblog.csdn.net/images/smiles/icon_lol.gif)
- [Android_Test_01_AsyncTask_eoe.rar](http://dl.javaeye.com/topics/download/9116ed1e-f03a-3187-b7a1-23edb3d8437e) (57.7 KB)
- 下载次数: 18 
- [HandlerTest2_线程交互_Bundle版.rar](http://dl.javaeye.com/topics/download/f466f05e-f207-3d78-8789-933d18b59ab9) (39.6 KB)
- 下载次数: 17 
- [xh_Android_Test_异常测试.rar](http://dl.javaeye.com/topics/download/0b372c61-02c3-3408-8cda-3914bee7c861) (42 KB)
- 下载次数: 15 
- ![](http://dl.javaeye.com/upload/attachment/356334/45de0dfa-835c-3790-86ec-3387953a06a3-thumb.jpg)
- 大小: 204 KB 
- ![](http://dl.javaeye.com/upload/attachment/356336/f8768118-8593-3094-8588-aa2b9ebb7357-thumb.jpg)
- 大小: 39.3 KB 
- ![](http://dl.javaeye.com/upload/attachment/356338/34fbce75-f052-3cc9-b4ee-3a54485abbe9-thumb.jpg)
- 大小: 39.8 KB 
- ![](http://dl.javaeye.com/upload/attachment/356340/0fba9a37-d410-3698-905c-ecbe953cb598-thumb.jpg)
- 大小: 43.8 KB 
- ![](http://dl.javaeye.com/upload/attachment/356342/cc8407d9-792f-3a83-959c-bbfa11ca9c98-thumb.jpg)
- 大小: 44.6 KB 
- ![](http://dl.javaeye.com/upload/attachment/356344/4d45244f-00e9-3558-b90e-1d4ed57a7254-thumb.jpg)
- 大小: 24.9 KB 
- ![](http://dl.javaeye.com/upload/attachment/356346/f7aabb39-1dd9-365b-8f9e-87eb537a86fa-thumb.jpg)
- 大小: 50.3 KB 
- ![](http://dl.javaeye.com/upload/attachment/356348/f3159ac6-726e-321a-9cd5-54f403301d58-thumb.jpg)
- 大小: 83.7 KB 
- [查看图片附件](http://writeblog.csdn.net/#)
