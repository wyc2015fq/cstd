# Android——浅谈归纳Handler各种用法 - 右上角同学的专栏 - CSDN博客
2016年07月05日 13:58:32[右上角徐](https://me.csdn.net/u011032983)阅读数：954
 文章转自：http://www.360doc.com/content/14/0108/13/7673502_343568991.shtml#
我学习Handler是按照这样的思路，所以我也按照这个的思路来写：
1.Handler是什么？
2. Handler干了些什么？
3.我们应该怎样利用Handler针对不同的场景来实现不同的功能
  （一） Handler是什么？       
        Handler,顾名思义就是“处理者”，用来发送和接受信息并且按照信息的匹配来实现执行功能代码。它包含线程队列和消息队列，实现异步的消息处理机制在android里面被运用很巧妙。
        Androud启动时，首先会开启一个主线程 (也就是UI线程) , 主线程为管理界面中的UI控件，进行事件分发，来响应客户的操作。 如果此时需要一个耗时的操作，例如: 要联网去下载一个东西，我们是不能把这些操作放在主线程中，会很容易导致“ARN”异常。如果你放在主线程中的话，界面会出现假死现象, 如果5秒钟还没有完成的话，，会收到Android系统的一个错误提示 "强制关闭". 这个时候我们需要把这些耗时的操作，放在一个子线程中,因为子线程涉及到UI更新，Android主线程是线程不安全的，也就是说，更新UI只能在主线程中更新，子线程中操作是危险的.
 这个时候，Handler就出现了.,来解决这个复杂的问题 , 由于Handler运行在主线程中(UI线程中), 它与子线程可以通过Message对象来传递数据, 这个时候，Handler就承担着接受子线程传过来的(子线程用sendMessage()方法传弟)Message对象，(里面包含数据) , 把这些消息放入主线程队列中，配合主线程进行更新UI。
                                                                                       （二） Handler干了些什么？
    handler可以分发Message对象和Runnable对象到主线程中, 每个Handler实例,都会绑定到创建它的线程中(一般是位于主线程),
     它有两个作用: 
        (1)安排消息或Runnable 在某个主线程中某个地方执行；
        (2)安排一个动作在不同的线程中执行 
    那么，要实现上述的两个功能，我们得基于发送和接受信息的基础，先了解下怎么发送和处理。
    它分发消息的一些方法
       post(Runnable)
        postAtTime(Runnable,long)
        postDelayed(Runnable long)
        sendEmptyMessage(int)
        sendMessage(Message)  将消息发送到消息队列
        msg.sendToTarget()  将消息发送到消息队列
        sendMessageAtTime(Message,long) 定时将消息发送到消息队列
        sendMessageDelayed(Message,long) 延迟一定时间后，将消息发送到消息队列
        以上post类方法允许你排列一个Runnable对象到主线程队列中,
        sendMessage类方法, 允许你安排一个带数据的Message对象到队列中，等待更新.(Message里可以包含数据)
知识补充之Message怎么包含数据：
(1)Message对象
       Message对象携带数据，通常它用arg1,arg2来传递消息，当然它还可以有obj参数，可以携带Bundle数据。它的特点是系统性能消耗非常少。
       初始化： Message msg=handler.obtainMessage();
(2)Bundle对象
      Bundle是Android提供的类，可以把它看做是特殊的Map，即键值对的包。而它特殊在键和值都必须要是基本数据类型或是基本数据类型的数组（Map的键值要求都是对象），特别的，键要求都是String类型。用Message来携带Bundle数据：
           放入：msg.setData(Bundle bundle);
           取出：msg.getData();
     一般我都是借助Bundle类。
代码  
               Bundle bundle = new Bundle();  
               bundle.putInt("myage", 21);  
               bundle.putString("myname", "yummylau");  
               msg.setData(bundler);  
               msg.sendToTarget();  
代码  
             Bundle myData = msg.getData();  
            int myage = myData.getInt("myage");  
            String myname = myData.getString("myname");  
希望对大家有帮助。
        它处理消息的一般做法
    消息的具体处理过程，需要在new Handler对象时使用匿名内部类重写Handler的handleMessage(Message msg)方法，如下：
         Handler handler=new Handler(){
          @Override
          public void handleMessage(Message msg) 
                //写上自己想要执行的功能代码
           }
         };
                                                                     （三）我们应该怎样利用Handler针对不同的场景来实现不同的功能
        一般我们创建一个Handler对象，主要是想利用Handler来和UI线程交互，Handlre可以是UI线程的，也可以是新线程里面的。
   （1）通过handler的延时发送message，可以延时处理一些事务的处理
    比如我想设计某一个应用的启动时有一个欢迎界面，让这个界面停留几秒后再跳转到里一个Activity主页
             public class SplashActivity extends Activity{   
                 private Intent intent;   
                 @Override   
                protected void onCreate(Bundle savedInstanceState) {  
                             // TODO Auto-generated method stub   
                   super.onCreate(savedInstanceState);
                   setContentView(R.layout.splash);
                    startMainAvtivity();   
                  }
                private void startMainAvtivity(){
                    new Handler().postDelayed(new Runnable(){   
                          @Override   
                          public void run() {   
                           intent = new Intent(SplashActivity.this,MainActivity.class);
                           startActivity(intent);  
                          SplashActivity.this.finish();
                       }  
                 }, 3000);  
              }    
           }  
    }     
    上述红色代码的利用Handler来延迟启动另一个Activity。
    值得注意的是：有一些小伙伴会认为，我们在主线程里面启动了新线程，然后在新线程里面跳转了Activity。其实一开始我也是这样认为的，在学习的过程中看到了一位网上的朋友写的博文和测试，也反思了一下，请教了一些师兄。只要记住：java里面启动新线程有两种方法（个人知识所限），一种是利用一个类去继承Thread，然后实例化后执行start（）方法；另一种是实现Runable接口，把Runnable对象作为参数去实例化一个Thread对象再执行start（）方法。
    下面例子是参考网上朋友的：
Java代码  
package org.hualang.handlertest3;  
import android.app.Activity;  
import android.os.Bundle;  
import android.os.Handler;  
import android.util.Log;  
public class HandlerTest3 extends Activity {  
    private Handler handler = new Handler();  
    private String TAG = "System.out";  
    @Override  
    public void onCreate(Bundle savedInstanceState) {  
        super.onCreate(savedInstanceState);  
        handler.post(r);  
        setContentView(R.layout.main);  
        //Thread t = new Thread(r);  
        //t.start();  
        Log.d(TAG,"Activity id:"+Thread.currentThread().getId());  
        Log.d(TAG,"Activity name:"+Thread.currentThread().getName());  
    }  
    Runnable r = new Runnable()  
    {  
        public void run()  
        {  
            Log.d(TAG,"Handler id:"+Thread.currentThread().getId());  
            Log.d(TAG,"Handler name:"+Thread.currentThread().getName());  
            try {  
                Thread.sleep(5000);  
            } catch (InterruptedException e) {  
                // TODO Auto-generated catch block  
                e.printStackTrace();  
            }  
        }  
    };  
}  
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_1.gif)运行结果
证明是同一个线程的两个依据：
①Activity的id或name和Handler的id或name是同样的
②设置了
           handler.post(r);
           setContentView(R.layout.main);
也就是，如果执行后马上显示文本信息，那么可以证明它们不在同一个线程，但是实际情况是要先执行了handler后5秒，才显示文本信息，说明它们在同一线程
如果将代码改为
           handler.post(r);  
           setContentView(R.layout.main);  
           Thread t = new Thread(r);  
            t.start();  
再次执行，运行结果如下，通过start启动线程，它们不在同一个线程中
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_2.gif)
（2）利用handler发送message，然后不同线程接受和处理该消息。
显然，我们在Android里面利用Handler更多的是想在异步线程里面实现我们想要的东西
    我把Handler处理异步分解成两个方面：
△1在UI线程建立Handler，由非UI线程发送信息UI线程接受信息；（用于更新UI）
代码截图：
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_3.jpg)
运行结果：
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_4.jpg)
这个例子和上述的例子相似，不过在与Handler建立的线程不一样。
上述例子比较局限，因为我们在做项目的时候不可能都写在同一文件里面，如果我们在不同文件而要拿到同一个Handler对象，我们可以考虑利用“构造器”的方法，把Handler对象作为参数进行传递，就可以实现不同文件的公用；或者我们可以定义成static。
android中其实还给了我们一个类HandlerThread，该类封装好了Looper和MessageQueue（个人测试之后认为，如果不对请指出哦）。
例子是按照上面的例子改写的，非UI线程处理UI线程发送的消息。
直接上代码：
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_5.jpg)
运行结果：
![](http://image68.360doc.com/DownloadImg/2014/01/0813/38106667_6.jpg)
从例子上看 ，效果是和上一个例子基本是一样的，少了Looper.prepare（）和Looper.loop（）；本人对这个类不太熟悉，请大神指点一二。
    在使用Handler的时候只要时刻谨记Handler在哪一个线程建立post执行所在的线程；HandlerUI线程操作和非UI线程操作的区别和定时执行某些动作。就能基本掌握了。android异步线程还可以使用AsyncTask执行异步任务。其底层实现还是基于Handler。

