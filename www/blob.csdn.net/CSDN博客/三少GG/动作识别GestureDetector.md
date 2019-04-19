# 动作识别GestureDetector - 三少GG - CSDN博客
2011年12月12日 16:52:36[三少GG](https://me.csdn.net/scut1135)阅读数：914标签：[float																[interface																[up](https://so.csdn.net/so/search/s.do?q=up&t=blog)](https://so.csdn.net/so/search/s.do?q=interface&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
# [浅析GestureDetector](http://www.cnblogs.com/xirihanlin/archive/2010/12/29/1920356.html)
最近在研究场景切换的动画效果，其中需要用到三连击的动作触发。三连击，即点三下屏幕，但意义上是双击效果。因此，我需要研究如何识别三连击的动作。
我们知道，一般的View只能响应点击（Click）和长按（LongPress）事件。这是因为View里只暴露了这些listener给我们使用。而实质上，View是在onTouchEvent(MotionEvent
 event)里对用户的动作做了一定的分析，从而通知我们是发生了点击还是长按等事件。
View里提供的回调在我描述的场景里，并不能满足要求。因此，GestureDetector出场了。我需要对其啃透才能写出自己的ActionDetector。
GestureDetector类可以帮助我们分析用户的动作，和View的onTouchEvent的处理方式差不多，但分析的动作类型更加细致，以下是它的回调接口：
public interface OnGestureListener {
                // Touch down时触发,
 e为down时的MotionEvent
                boolean onDown(MotionEvent e);
                //在Touch
 down之后一定时间（115ms）触发，e为down时的MotionEvent
                void onShowPress(MotionEvent e);
                // Touch up时触发，e为up时的MotionEvent
                boolean onSingleTapUp(MotionEvent e);
                //滑动时触发，e1为down时的MotionEvent，e2为move时的MotionEvent
                boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY);
                //在Touch
 down之后一定时间（500ms）触发，e为down时的MotionEvent
                void onLongPress(MotionEvent e);
                //滑动一段距离，up时触发，e1为down时的MotionEvent，e2为up时的MotionEvent
                boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY);
}
public interface OnDoubleTapListener {
                //完成一次单击，并确定没有二击事件后触发（300ms），e为down时的MotionEvent
                boolean onSingleTapConfirmed(MotionEvent e);
                //第二次单击down时触发，e为第一次down时的MotionEvent
                boolean onDoubleTap(MotionEvent e);
                //第二次单击down,move和up时都触发，e为不同时机下的MotionEvent
                boolean onDoubleTapEvent(MotionEvent e);
}
有了这么多的回调消息，我们就能更加方便的对用户的动作进行响应，那么，这个类如何使用呢？以下是使用该类的一个范例：
private GestureDetector mGestureDetector;
@Override
public void onCreate(Bundle savedInstanceState) {
  super.onCreate(savedInstanceState);
  mGestureDetector = new GestureDetector(this, newMyGestureListener());
}
@Override
public boolean onTouchEvent(MotionEvent event) {     //利用onTouchEvent方法作为入口检测，通过传递MotionEvent参数来监听操作手势。
returnmGestureDetector.onTouchEvent(event);
}
classMyGestureListener extends GestureDetector.SimpleOnGestureListener{
  @Override
  public boolean onSingleTapUp(MotionEvent ev) {
    Log.d("onSingleTapUp",ev.toString());
    return true;
  }
  @Override
  public void onShowPress(MotionEvent ev) {
    Log.d("onShowPress",ev.toString());
  }
  @Override
  public void onLongPress(MotionEvent ev) {
    Log.d("onLongPress",ev.toString());
  }
…
}
基本的内容就是创建一个GestureDetector的对象，传入listener对象，在自己接收到的onTouchEvent中将event传给GestureDetector进行分析，listener会回调给我们相应的动作。其中GestureDetector.SimpleOnGestureListener（Framework帮我们简化了）是实现了上面提到的OnGestureListener和OnDoubleTapListener两个接口的类，我们只需要继承它并重写其中我们关心的回调即可。
最后，再提一下双击和三击的识别过程：在第一次单击down时，给Hanlder发送了一个延时300ms的消息，如果300ms里，发生了第二次单击的down事件，那么，就认为是双击事件了，并移除之前发送的延时消息。如果300ms后仍没有第二次的down消息，那么就判定为SingleTapConfirmed事件（当然，此时用户的手指应已完成第一次点击的up过程）。三击的判定和双击的判定类似，只是多了一次发送延时消息的过程，有意思吧~~~嘿嘿~~~
