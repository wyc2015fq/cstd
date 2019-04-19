# 如何利用Handler更新android的UI - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月22日 16:53:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：2136
如何利用Handler更新android的UI
# 提出问题
新手刚开始接触android线程编程的时候，会想到如下使用如下代码：
new Thread( new Runnable() {
public void run() {
myView.invalidate();
}
}).start(); 
但是：该实现方法是不可行的。
# 分析问题：
因为：**Android UI****操作并不是线程安全的,****这些操作必须在UI****线程中执行**。
1.myView.invalidate()是UI操作；
2.new Thread创建了一个新的线程，是一个子线程，而不是UI线程，在这个新的线程里面对view进行刷新，这样是不安全的。
# 解决问题：
那么如何解决这个问题呢？
答案：使用Handler来实现UI线程的更新的。
# 实现步骤：
1.在view类里面定义一个Handler变量，这并没有开启一个新的线程，因此在handler里面更新本view是安全的；
2.然后我们创建一个线程，通过这个线程来给activity的handler变量发送消息，同时通过这个线程进行延时。
## 意即：
1．Handler接收消息和处理消息；
2．创建一个线程，来给Handler发送消息；
# 消息处理三部曲：
1．发送消息；
2．接收消息；
3．处理消息；
# 代码参考
首先创建简单的View,代码如下：
package com.ray.handler;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.drawable.Drawable;
import android.view.View;
public class BounceView extends View {
float x = 40;
public BounceView(Context context) { 
super(context); 
} 
@Override 
protected void onDraw(Canvas canvas) { 
x+=10;
Paint mPaint = new Paint();
mPaint.setAntiAlias(true);
mPaint.setColor(Color.GREEN);
canvas.drawCircle(x, 40, 40, mPaint);
}
}
创建Activity类，代码如下：
package com.ray.handler;
import android.app.Activity;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.Window; 
public class TestHandler extends Activity {
protected static final int GUIUPDATEIDENTIFIER = 0x101; 
Thread myRefreshThread = null; 
BounceView myBounceView = null; 
**Handler myHandler = new Handler() {//****在主线程中创建Handler****对象**
public void handleMessage(Message msg) {**//****处理消息**
switch (msg.what) { 
case TestHandler.GUIUPDATEIDENTIFIER:// 
**myBounceView.invalidate();//UI****界面更新**
break; 
} 
super.handleMessage(msg); 
} 
};
public void onCreate(Bundle savedInstanceState) { 
super.onCreate(savedInstanceState); 
this.requestWindowFeature(Window.FEATURE_NO_TITLE); 
this.myBounceView = new BounceView(this);
this.setContentView(this.myBounceView); 
new Thread(new myThread()).start();**//****创建一个子线程**
} 
//线程类，这个线程只是发送消息，然后延时而已。
class myThread implements Runnable { 
public void run() {
while (!Thread.currentThread().isInterrupted()) {
Message message = new Message(); 
message.what = TestHandler.GUIUPDATEIDENTIFIER; 
TestHandler.this.myHandler.sendMessage(message);** //****发送一次消息，自动调用上面handler****类的handleMessage****函数，从而更新view****类。**
try { 
Thread.sleep(100);**//****延时**
} catch (InterruptedException e) { 
Thread.currentThread().interrupt(); 
} 
} 
} 
} 
}
