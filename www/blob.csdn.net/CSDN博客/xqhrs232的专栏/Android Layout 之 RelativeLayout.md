# Android Layout 之 RelativeLayout - xqhrs232的专栏 - CSDN博客
2010年12月29日 18:48:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1147标签：[layout																[android																[button																[import																[2010																[class](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/lirushuokeyi/archive/2010/03/13/5375900.aspx](http://blog.csdn.net/lirushuokeyi/archive/2010/03/13/5375900.aspx)
Android Layout 之 RelativeLayout
  使用 AbsoluteLayout 可以直接指定其子 View  的绝对位置， 这种布局方式虽然简单，但是不够灵活。比如在一个程序中，按钮2 位于 按钮1 的下方且和 按钮1 左对齐，我们可以使用指定两个按钮的绝对位置的方式布局，但是当布局完成后，由于某些原因，这两个按钮需要相左平移一些距离以便在父 View 右边留出一些空白区域，那么我们就需要同时修改 按钮1 和 按钮2 的 layout params。如果布局更复杂一些呢？这样“牵一发而动全身”的布局模式恐怕不是那么友好吧？
  RelativeLayout，顾名思义，就是以“相对”位置/对齐 为基础的布局方式。android.widget.RelativeLayout 有个 继承自android.view.ViewGroup.LayoutParams 的内嵌类 LayoutParams，使用这个类的实例调用 RelativeLayout.addView 就可以实现“相对布局”。
  android.widget.RelativeLayout.LayoutParams 有一个构造函数：RelativeLayout.LayoutParams(int w, int h)，参数指定了子 View 的宽度和高度，这一点和其父类是一样的。而实现相对布局的关键在它的 两个 addRule 方法上。anchor 参数指定可以是 View 的 id(“相对于谁”)、RelativeLayout.TRUE（启用某种对齐方式） 或者 是-1（应用于某些不需要 anchor 的 verb）；AddRule
 方法的 verb 参数指定相对的“动作”（以下常量均定义于 android.widget.RelativeLayout中，为了简便不给出其全名）：
ALIGN_BOTTOM、ALIGN_LEFT、 ALIGN_RIGHT、 ALIGN_TOP： 本 View 的 底边/左边/右边/顶边 和 anchor 指定的 View 的 底边/左边/右边/顶边 对齐。
ALIGN_WITH_PARENT_BOTTOM 、ALIGN_WITH_PARENT_LEFT 、  ALIGN_WITH_PARENT_RIGHT 、 ALIGN_WITH_PARENT_TOP ： 和上面一组常量类似，只不过不需要再指定 anchor， 其 anchor 自动为 Parent View。
CENTER_HORIZONTAL、CENTER_IN_PARENT 、CENTER_VERTICAL ： 如果 anchor 为 TRUE，在 Parent 中 水平居中/水平和垂直均居中/垂直居中。
POSITION_ABOVE 、POSITION_BELOW 、 POSITION_TO_LEFT 、POSITION_TO_RIGHT  ： 本 View  位于 anchor 指定的 View 的 上边/下边/左边/右边。
看一个例子：
package com.farproc.RLTest;
import android.app.Activity;
import android.os.Bundle;
import android.widget.*;
import android.view.*;
public class RLTest extends Activity {
    private RelativeLayout rl;
    private Button btn1;
    private Button btn2;
    private Button btn3;
    private Button btn4;
    private static final int ID_BTN1 = 1;
    private static final int ID_BTN2 = 2;
    private static final int ID_BTN3 = 3;
    private static final int ID_BTN4 = 4;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle icicle) {
        super.onCreate(icicle);
        rl = new RelativeLayout(this);
        btn1 = new Button(this);
        btn1.setText("----------------------");
        btn1.setId(ID_BTN1);
        RelativeLayout.LayoutParams lp1 = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp1.addRule(RelativeLayout.ALIGN_WITH_PARENT_TOP);
        lp1.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE); 
        // btn1 位于父 View 的顶部，在父 View 中水平居中
        rl.addView(btn1, lp1 );
        btn2 = new Button(this);
        btn2.setText("|/n|/n|/n|/n|/n|");
        btn2.setId(ID_BTN2);
        RelativeLayout.LayoutParams lp2 = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp2.addRule(RelativeLayout.POSITION_BELOW, ID_BTN1);
        lp2.addRule(RelativeLayout.ALIGN_LEFT, ID_BTN1);
        // btn2 位于 btn1 的下方、其左边和 btn1 的左边对齐
        rl.addView(btn2, lp2);
        btn3 = new Button(this);
        btn3.setText("|/n|/n|/n|/n|/n|");
        btn3.setId(ID_BTN3);
        RelativeLayout.LayoutParams lp3 = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
       lp3.addRule(RelativeLayout.POSITION_BELOW, ID_BTN1);
        lp3.addRule(RelativeLayout.POSITION_TO_RIGHT, ID_BTN2);
        lp3.addRule(RelativeLayout.ALIGN_RIGHT, ID_BTN1);
        // btn3 位于 btn1 的下方、btn2 的右方且其右边和 btn1 的右边对齐（要扩充）
        rl.addView(btn3,lp3);
        btn4 = new Button(this);
        btn4.setText("--------------------------------------------");
        btn4.setId(ID_BTN4);
        RelativeLayout.LayoutParams lp4 = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp4.addRule(RelativeLayout.POSITION_BELOW, ID_BTN2);
        lp4.addRule(RelativeLayout.CENTER_HORIZONTAL, RelativeLayout.TRUE);
        // btn4 位于 btn2 的下方，在父 Veiw 中水平居中
        rl.addView(btn4,lp4);
        setContentView(rl);
    }
}
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/lirushuokeyi/archive/2010/03/13/5375900.aspx](http://blog.csdn.net/lirushuokeyi/archive/2010/03/13/5375900.aspx)

