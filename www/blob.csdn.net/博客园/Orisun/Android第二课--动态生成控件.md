# Android第二课--动态生成控件 - Orisun - 博客园







# [Android第二课--动态生成控件](https://www.cnblogs.com/zhangchaoyang/articles/1795119.html)





首届 Google 暑期大学生博客分享大赛——2010 Andriod 篇

上一篇是一个layout xml文件和一个java文件配合展现了一个页面，我们知道的ASP.NET中可以在后台动态生成控件，那么在Android中也可以，都是面向对象的架构嘛。我们在上一次代码的基础上添加一些新内容：



```
package zcy.com;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;


public class Show extends Activity {
    private Button btNext;
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.show1);
        btNext=(Button)findViewById(R.id.next);
        btNext.setOnClickListener(new Button.OnClickListener(){
            public void onClick(View view){

　　　　　　　　　　//创建一个布局管理器控件
                LinearLayout linearLayout=new LinearLayout(Show.this);
                setContentView(linearLayout);

　　　　　　　　　　//设置orientation为horizontal,1表示为vertical
                linearLayout.setOrientation(0);
                TextView tvSecond=new TextView(Show.this);
                tvSecond.setText("这是Android奉献给你的第二个页面");
                Button btNext2=new Button(Show.this);
                btNext2.setText("下一步");
                btNext2.setId(2);
                LinearLayout.LayoutParams lp1 = new LinearLayout.LayoutParams(

　　　　　　　　　　　　　　ViewGroup.LayoutParams.WRAP_CONTENT,

　　　　　　　　　　　　   ViewGroup.LayoutParams.WRAP_CONTENT);
                linearLayout.addView(tvSecond,lp1);
                linearLayout.addView(btNext2,lp1);
                btNext2.setOnClickListener(new OnClickListener(){
                    public void onClick(View view){

　　　　　　　　　　　　　　 //这次只放置一个TextView不需要布局管理器
                        TextView tv3=new TextView(Show.this);
                        tv3.setText("恭喜你已来到最后一个页面！");

　　　　　　　　　　　　　　　//直接让页面显示一个TextView
                        setContentView(tv3);
                    }
                });
            }
        });
    }
}
```

这里涉及到几个类的构造函数和方法，我把Android SDK文档中的内容直接引用过来：



Public Constructors



LinearLayout(Context context)

LinearLayout(Context context, AttributeSet attrs)



public void setOrientation (int orientation)

Should the layout be a column or a row.

Related XML Attributes

android:orientation

Parameters  orientation  Pass HORIZONTAL or VERTICAL. Default value is HORIZONTAL.



Constant 　　Value 　　Description

horizontal　　0 　　　　Defines an horizontal widget.

vertical　　　　1　　　　 Defines a vertical widget.

This corresponds to the global attribute resource symbol orientation.



Public Constructors

LinearLayout.LayoutParams(Context c, AttributeSet attrs)

LinearLayout.LayoutParams(int width, int height)



addView方法的定义在ViewGroup类中

//Adds a child view with the specified layout parameters.

void　　addView(View child, ViewGroup.LayoutParams params)

//Adds a child view with the specified layout parameters.

void 　 addView(View child, int index, ViewGroup.LayoutParams params)

看一下这里的继承关系：
   ↳android.widget.LinearLayout





另外注意，为什么在传递Context参数时必须用Show.this而不能直接用this呢？因为此时我们已经进入了一个匿名类new Button.OnClickListener()中，虽然这个匿名类还是在Show类，但是我们的代码已经不再“直属”于Show类了。



运行效果图：



![](https://pic002.cnblogs.com/img/zcy/201008/2010080817490471.png)

![](https://pic002.cnblogs.com/img/zcy/201008/2010080817493470.png)

![](https://pic002.cnblogs.com/img/zcy/201008/2010080817501856.png)












