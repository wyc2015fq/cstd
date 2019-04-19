# Android下实现程序关闭后，监听取消 - xqhrs232的专栏 - CSDN博客
2011年03月15日 10:42:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：968
原文地址::[http://www.cnblogs.com/tt_mc/archive/2010/03/03/1677474.html](http://www.cnblogs.com/tt_mc/archive/2010/03/03/1677474.html)
在先前的文章中，接收短信的监听器会一直活在后台，哪怕程序被关闭，监听器会一直存在并在收到消息时激活主程序。在一般的设计中，都不会这么做，因为这是浪费资源且得不到良好用户体验的。因此需要在自己的程序并闭后，将监听器取消。前文链接（[点击进入](http://hi.baidu.com/rarnu/blog/item/61ac91ca9afa978cc917688b.html)）
一、建立一个后台服务，该服务用于接收消息并将消息广播给BroadcastReceiver。由于服务可以被终止，终止时不再转发消息，后台的BroadcastReceiver也因此不再生效，会自动被GC回收。程序的目标也达到了
package com.demo;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.IBinder;
import android.telephony.gsm.SmsMessage;
public class ListenService 
extends Service {
public static final String SvrId = 
"RARNU.SERVICE.DEMO";
public static final String strACT = 
"android.provider.Telephony.SMS_RECEIVED";
private SMSMsgReceiver recv;
@Override
public void onStart(Intent intent, 
int startId) {
super.onStart(intent, startId);
}
@Override
public void onCreate() {
IntentFilter filter = new IntentFilter(strACT);
recv = new SMSMsgReceiver();
registerReceiver(recv, filter);
super.onCreate();
}
@Override
public IBinder onBind(Intent intent) {
return null;
}
@Override
public void onDestroy() {
unregisterReceiver(recv);
super.onDestroy();
}
public class SMSMsgReceiver 
extends BroadcastReceiver {
@Override
public void onReceive(Context context, Intent intent) {
if (intent.getAction().equals(strACT)) {
StringBuilder sb = new StringBuilder();
Bundle bundle = intent.getExtras();
if (bundle != null) {
Object[] pdus = (Object[]) bundle.get("pdus");
SmsMessage[] msg = new SmsMessage[pdus.length];
for (int i = 0; i < pdus.length; i++) {
msg[i] = SmsMessage.createFromPdu((byte[]) pdus[i]);
}
for (SmsMessage currMsg : msg) {
sb.append("From:");
sb.append(currMsg.getDisplayOriginatingAddress());
sb.append("/nMessage:");
sb.append(currMsg.getDisplayMessageBody());
}
}
Intent i = new Intent(SvrId);
i.putExtra("PARAM", sb.toString());
sendBroadcast(i);
}
}
}
}
可以看到，前文中接收短信的代码被原封不动的复制到服务的代码内，唯一不同的就是服务中多了sendBroadcast()
二、修改BroadcastReceiver
package com.demo;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Toast;
public class MessageReceiver 
extends BroadcastReceiver {
public static final String SvrId = 
"RARNU.SERVICE.DEMO";
@Override
public void onReceive(Context context, Intent intent) {
if (intent.getAction().toString().equals(SvrId)) {
Bundle bund = intent.getExtras();
String strParam = "";
if (bund != null) {
strParam = bund.getString("PARAM");
}
Intent i = new Intent(context, MsgListen.class);
i.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
Toast.makeText(context, strParam, Toast.LENGTH_SHORT).show();
context.startActivity(i);
}
}
}
收短信的代码被移到服务中了，那么BroadcastReceive自然也不再需要那段代码，直接从Extras中取得服务传来的字符串即可。
三、启动和终止
package com.demo;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
public class MsgListen 
extends Activity {
@Override
public void onCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
setContentView(R.layout.main);
}
@Override
protected void onResume() {
Intent i = new Intent(MsgListen.this, ListenService.class );
i.setFlags( Intent.FLAG_ACTIVITY_NEW_TASK ); 
startService(i);
super.onResume();
}
@Override
protected void onPause() {
Intent i = new Intent(MsgListen.this, ListenService.class);
stopService(i);
super.onPause();
}
}
在程序的onResume()事件中启动服务，在onPause()事件中停止服务
然后就可以尝试测试一下程序了，看看是不是程序关闭后，收到短信不会再弹提示了呢？
