# 深入浅出Android的多线程 - 2019跟着小虎玩着去软考 - CSDN博客
2011年04月22日 13:16:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3217标签：[android																[thread																[ui																[工作																[多线程																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=ui&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android																[手机开发](https://blog.csdn.net/littletigerat/article/category/649495)](https://blog.csdn.net/littletigerat/article/category/613910)
**深入浅出****Android****的多线程**
# 1.在Android中，线程的类型：
a．有消息循环的线程；
b．无消息循环的线程；
# 2.线程分为：
a.UI线程；
b.子线程；
# 3.主线程与子线程的区别：
只要是关于UI相关的东西，就不能放在子线程中处理，因为子线程是不能操作UI的，只能进行数据、系统等其他非UI的操作
# 4.有消息循环的线程一般都会有一个Looper
# 5.主线程（UI线程）就是一个消息循环的线程。
# 6.Handle如何初始化：
Handle的初始化函数（构造函数），
1．如果没有参数，那么他就默认使用的是当前的Looper；
2．如果有Looper参数，就是用对应的线程的Looper。
# 7.如何获得线程的Looper：
Looper.myLooper();//获得当前线程的Looper
Looper.getMainLooper () //获得UI线程的Lopper
# 8.消息处理过程：
a.发送消息；
b.获取消息；
c.处理消息；
# 9.Handler在应该在哪个线程中创建：
handler应该由处理消息的线程创建；
也就是说，如果handler的函数handleMessage里处理的是UI的消息，也就是更新界面的事情，那么该handler需要在主线程中创建；
# 10.Handler的作用：
1.是把消息加入特定的（Looper）消息队列中；
2.分发消息；
3.处理该消息队列中的消息；
# 11.handler与创建它的线程之间的关系：
handler与创建它的线程相关联，而且也只与创建它的线程相关联。
handler运行在创建它的线程中，所以，如果在handler中进行耗时的操作，会阻塞创建它的线程。
# 12.handle运行在哪个线程中（主线程还是子线程）：
handler运行在创建它的线程中，所以，如果在handler中进行耗时的操作，会阻塞创建它的线程。
# 13.结合具体代码，详解Handle的创建：
package com.android.tutor;  
import java.util.Timer;  
import java.util.TimerTask;  
import android.app.Activity;  
import android.os.Bundle;  
import android.os.Handler;  
import android.os.Message;  
public class HandlerDemo extends Activity {  
    //title为setTitle方法提供变量,这里为了方便我设置成了int型
    private int title = 0;  
    private Handler mHandler = new Handler(){  
        public void handleMessage(Message msg) {  
            switch (msg.what) {  
            case 1:  
                updateTitle();  
                break;  
            }  
        };  
    };  
    public void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);  
        setContentView(R.layout.main);  
        Timer timer = new Timer();  
        timer.scheduleAtFixedRate(new MyTask(), 1, 5000);  
    }  
    private class MyTask extends TimerTask{  
        @Override  
        public void run() {  
            Message message = new Message();  
            message.what = 1;  
            mHandler.sendMessage(message);  
        }     
    }  
    public void updateTitle(){  
        setTitle("Welcome to Mr Wei's blog " + title);  
        title ++;  
    }  
} 
上面的代码，直接在主线程中定义Handler成员。在子线程中通过主线程的handler向主线程发送消息。其使用步骤如下：
1、在主线程中定义handler，并为这个handler实现handleMessage方法。
2、在子线程中调用主线程的handler，通过其sendMessage方法发送消息。
Notes：
1.HandlerDemo对应一个主线程；
因为：当应用程序启动时，Android首先会开启一个主线程 (也就是UI线程)；
所以：
在HandlerDemo类中定义handler成员变量，就对应的是：在主线程中定义handler；
2.子线程就是MyTask，在MyTask的run成员函数中调用mHandler.sendMessage(message),
就对应的是：在子线程中调用主线程的handler，通过其sendMessage方法发送消息；
3.updateTitle()属于更新UI操作，只能在主线程中操作，而handler是在主线程中创建，所以符合：**UI更新只能在主线程中操作的原则**；
# 14.Android消息循环是针对线程的：
每个线程都可以有自己的消息队列和消息循环；
Android系统的消息队列和消息循环都是针对具体线程的；
一个线程可以存在（当然也可以不存在）一个消息队列和一个消息循环（Looper）；
特定线程的消息只能分发给本线程，不能进行跨线程，跨进程通讯。
# 15.创建的工作线程默认是没有消息循环和消息队列的：
# 16.如何让创建的工作线程具有：消息循环和消息队列呢？
# 17. Activity是一个UI线程，运行于主线程中
Activity是一个UI线程，运行于主线程中，Android系统在启动的时候会为Activity创建一个消息队列和消息循环（Looper）。
详细实现请参考ActivityThread.java文件。
# 18.子线程的代码示例：
class LooperThread extends Thread {
public Handler mHandler;
public void run() {
Looper.prepare();
mHandler = new Handler() {
public void handleMessage(Message msg) {
// process incoming messages here
}
};
Looper.loop();
}
}
# 19.一个子线程如何把消息放入主线程的消息队列中？
通过Handle对象，只要Handler对象以主线程的Looper创建，那么调用 Handler的sendMessage等接口，将会把消息放入队列都将是放入主线程的消息队列。
并且将会在Handler主线程中调用该handler 的handleMessage接口来处理消息。
# 20. handler对象的handleMessage接口：
handler对象的handleMessage接口将会在主线程中调用。
在这个函数可以放心的调用主线程中任何变量和函数，进而完成更新UI的任务
# 21.Handle用途：
如果通过工作线程刷新界面，推荐使用handler对象来实现。
# 22.handler对象使用要点：
1.handler对象在主线程中构造完成（并且启动工作线程之后不要再修改之，否则会出现数据不一致），然后在工作线程中可以放心的调用发送消息SendMessage等接口；
2.除了所述的hanlder对象之外的任何主线程的成员变量如果在工作线程中调用，仔细考虑线程同步问题。
如果有必要需要加入同步对象保护该变量。
# 22.子线程的代码示例之二：
package com.simon;
import android.app.Activity;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;
import android.os.Handler;
public class MyHandler extends Activity {
static final String TAG = “Handler”;
Handler h = new Handler(){
public void handleMessage (Message msg)
{
switch(msg.what)
{
case HANDLER_TEST:
Log.d(TAG, “The handler thread id = ” + Thread.currentThread().getId() + “/n”);
break;
}
}
};
static final int HANDLER_TEST = 1;
/** Called when the activity is first created. */
@Override
public void onCreate(Bundle savedInstanceState) {
super.onCreate(savedInstanceState);
Log.d(TAG, “The main thread id = ” + Thread.currentThread().getId() + “/n”);
new myThread().start();
setContentView(R.layout.main);
}
class myThread extends Thread
{
public void run()
{
Message msg = new Message();
msg.what = HANDLER_TEST;
h.sendMessage(msg);
Log.d(TAG, “The worker thread id = ” + Thread.currentThread().getId() + “/n”);
}
}
}
说明：
1.MyHandler是Activity的派生，是主线程；
2.myThread是子线程，工作线程；
3.在子线程的run函数中调用h.sendMessage(msg)发送消息；
4.Handler对象在主线程中创建；
Handler h = new Handler()
5.在onCreate中函数创建子线程；
new myThread().start()
