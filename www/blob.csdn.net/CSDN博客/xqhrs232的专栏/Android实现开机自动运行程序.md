# Android实现开机自动运行程序 - xqhrs232的专栏 - CSDN博客
2010年12月22日 10:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：543标签：[android																[import																[service																[action																[class																[框架](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址:: [http://news.e800.com.cn/articles/2009/1231/1262242877505_1.html](http://news.e800.com.cn/articles/2009/1231/1262242877505_1.html)
有些时候，应用需要在开机时就自动运行，例如某个自动从网上更新内容的后台service。怎样实现开机自动运行的应用？在撰写本文时，联想到高焕堂先生以“Don''t call me, I''ll call you back!”总结Android框架，真是说到点子上了。
理解这句话的含义，许多有关Android平台上实现某种功能的问题，都能迎刃而解。 使用场景：手机开机后，自动运行程序，在屏幕上显示"Hello. I started!"字样。
背景知识：当Android启动时，会发出一个系统广播，内容为ACTION_BOOT_COMPLETED，它的字符串常量表示为android.intent.action.BOOT_COMPLETED。只要在程序中“捕捉”到这个消息，再启动之即可。记住，Android框架说：Don''t call me, I''ll call you back。我们要做的是做好接收这个消息的准备，而实现的手段就是实现一个BroadcastReceiver。
代码解析： 
1、界面Activity：SayHello.java package com.ghstudio.BootStartDemo; import android.app.Activity; import android.os.Bundle; import android.widget.TextView; public class SayHello extends Activity { @Override public void onCreate(Bundle savedInstanceState) { super.onCreate(savedInstanceState);
 TextView tv = new TextView(this); tv.setText("Hello. I started!"); setContentView(tv); } } 这段代码很简单，当Activity启动时，创建一个TextView，用它显示"Hello. I started!"字样。
2、接收广播消息：BootBroadcastReceiver.java package com.ghstudio.BootStartDemo; import android.content.BroadcastReceiver; import android.content.Context; import android.content.Intent; public class BootBroadcastReceiver extends BroadcastReceiver { static final String
 ACTION = "android.intent.action.BOOT_COMPLETED"; @Override public void onReceive(Context context, Intent intent) { if (intent.getAction().equals(ACTION)){ Intent sayHelloIntent=new Intent(context,SayHello.class); sayHelloIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
 context.startActivity(sayHelloIntent); } } } 该类派生自BroadcastReceiver，覆载方法onReceive中，检测接收到的Intent是否符合BOOT_COMPLETED，如果符合，则启动SayHello那个Activity。
3、配置文件：AndroidManifest.xml 
注意其中粗体字那一部分，该节点向系统注册了一个receiver，子节点intent-filter表示接收android.intent.action.BOOT_COMPLETED消息。不要忘记配置android.permission.RECEIVE_BOOT_COMPLETED权限。
 完成后，编译出apk包，安装到模拟器或手机中。关机，重新开机。 
运行截图： 
延伸思考：在多数情况下，要自动运行的不是有界面的程序，而是在后台运行的service。此时，就要用startService来启动相应的service了。
