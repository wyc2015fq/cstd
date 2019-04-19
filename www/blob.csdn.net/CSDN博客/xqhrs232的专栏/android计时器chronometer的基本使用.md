# android计时器chronometer的基本使用 - xqhrs232的专栏 - CSDN博客
2011年01月06日 11:31:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1222标签：[android																[button																[resources																[string																[class																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=resources&t=blog)](https://so.csdn.net/so/search/s.do?q=button&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://my.oschina.net/kevin008/blog/2209](http://my.oschina.net/kevin008/blog/2209)
在Android中，可以使用计时器来实现对时间的监测，这个类所实现的功能有开始计时，停止计时，重新计时，设置计
时模式，
下面列出计时器方法的原型：
long getBase();//返回基地的时间，由setBase(long)设置的
String getFormat();//返回当前字符串格式，此格式是通过setFormat()实现的
void setBase(long base);//设置时间，计数定时器指定的值
void setFormat(String format);//设置显示的内容，计时器将会显示这个参数所对应的值得，如果字符串的值
//为null，那么返回的值为MM:SS格式的
下面就来介绍一个实例：
package com.example.android.apis.view;
// Need the following import to get access to the app resources, since this
// class is in a sub-package.
import com.example.android.apis.R;
import android.app.Activity;
import android.os.Bundle;
import android.os.SystemClock;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Chronometer;
public class ChronometerDemo extends Activity {
    Chronometer mChronometer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.chronometer);
        Button button;
        mChronometer = (Chronometer) findViewById(R.id.chronometer);
        // Watch for button clicks.
        button = (Button) findViewById(R.id.start);
        button.setOnClickListener(mStartListener);//对应的是开始计时
        button = (Button) findViewById(R.id.stop);
        button.setOnClickListener(mStopListener); //对应的是停止计时
        button = (Button) findViewById(R.id.reset);
        button.setOnClickListener(mResetListener);//对应的是重新置数
        button = (Button) findViewById(R.id.set_format);
        button.setOnClickListener(mSetFormatListener);//对应的是设置时间的显示格式
        button = (Button) findViewById(R.id.clear_format);
        button.setOnClickListener(mClearFormatListener);//对应的是使用非格式的计时显示功能
    }
    View.OnClickListener mStartListener = new OnClickListener() {
        public void onClick(View v) {
            mChronometer.start();
        }
    };
    View.OnClickListener mStopListener = new OnClickListener() {
        public void onClick(View v) {
            mChronometer.stop();  
        }
    };
    View.OnClickListener mResetListener = new OnClickListener() {
        public void onClick(View v) {
            mChronometer.setBase(SystemClock.elapsedRealtime());
        }
    };
    View.OnClickListener mSetFormatListener = new OnClickListener() {
        public void onClick(View v) {
            mChronometer.setFormat("Formatted time (%s)");
        }
    };
    View.OnClickListener mClearFormatListener = new OnClickListener() {
        public void onClick(View v) {
            mChronometer.setFormat(null);
        }
    };
}
//==================================================
相关网址
1》Android中使用定时器TimerTask类介绍
[http://www.dnbcw.com/biancheng/shouji/onrc234834.html](http://www.dnbcw.com/biancheng/shouji/onrc234834.html)

