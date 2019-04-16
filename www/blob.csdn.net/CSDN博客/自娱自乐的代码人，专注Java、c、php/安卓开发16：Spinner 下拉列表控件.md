# 安卓开发16：Spinner 下拉列表控件 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月09日 14:22:17[initphp](https://me.csdn.net/initphp)阅读数：3049
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









Spinner 下拉列表控件

创建一个activity_main.xml文件：



```
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:paddingBottom="@dimen/activity_vertical_margin"
    android:paddingLeft="@dimen/activity_horizontal_margin"
    android:paddingRight="@dimen/activity_horizontal_margin"
    android:paddingTop="@dimen/activity_vertical_margin"
    tools:context=".MainActivity" >

    <!-- 下拉列表控件 -->
    <Spinner
        android:id="@+id/spinner1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_marginLeft="26dp"
        android:layout_marginTop="86dp"/>

</RelativeLayout>
```


创建一个数组字符串：string.xml



```
<string-array name="stringArray">
        <item >hello world</item>
        <item >yes i do!</item>
        <item>it\'s easy!</item>
        <item >I am javaer!</item>
        <item>Only a littly</item>
    </string-array>
```


MainActivity.java：



```java
package com.example.test.com;


import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.Toast;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        //获取下拉列表对象
        Spinner spinner = (Spinner) findViewById(R.id.spinner1);

        //获取字符串数组
        final String[] stringArray = getResources().getStringArray(R.array.stringArray);

        //通过适配器 填充数据
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, stringArray);
        spinner.setAdapter(adapter);

        //设置一个选中的监听事件
        spinner.setOnItemSelectedListener(new OnItemSelectedListener() {

            @Override
            public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2, long arg3) {
                int index = arg0.getSelectedItemPosition(); //选中的pos
                Toast.makeText(getBaseContext(), "SELECT: "  + stringArray[index], Toast.LENGTH_SHORT).show();
            }

            @Override
            public void onNothingSelected(AdapterView<?> arg0) {
            }
        });

    }

}
```


效果图：

![](https://img-blog.csdn.net/20130709142207218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




