# 安卓开发13：ListView详细解读 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年07月03日 09:50:48[initphp](https://me.csdn.net/initphp)阅读数：1573
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









ListView一般用于展示列表元素。

先看一个ListView例子：

MainActivity.java:



```java
package com.example.test.com;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import android.app.Activity;
import android.os.Bundle;
import android.view.Window;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        setContentView(R.layout.activity_main);

        //获取ListView对象
        ListView listView = (ListView) findViewById(R.id.ListViewTest);
        //定义一个适配关系的数组
        String[] AdapterMap = new String[1];
        AdapterMap[0] = "title";
        //定义一个适配关系的ID数组
        int[] adapterId = new int[1];
        adapterId[0] = R.id.textView1;
        //定义一个HashMap
        ArrayList<Map<String, Object>> listMaps = new ArrayList<Map<String, Object>>();
        HashMap<String, Object> hashMap1 = new HashMap<String, Object>();
        hashMap1.put("title", "Hello World");
        HashMap<String, Object> hashMap2 = new HashMap<String, Object>();
        hashMap2.put("title", "Hello World2");
        listMaps.add(hashMap1);
        listMaps.add(hashMap2);
        //创建一个适配器
        SimpleAdapter myAdapter = (SimpleAdapter) new SimpleAdapter(this, listMaps, R.layout.listview, AdapterMap, adapterId);
        //ListView设置适配器
        listView.setAdapter(myAdapter);

    }

}
```



activity_main.xml:



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

    <!-- 一个ListView -->
    <ListView
        android:id="@+id/ListViewTest"
        android:layout_width="200dp"
        android:layout_height="fill_parent"
        android:scrollbars="none" >
    </ListView>

</RelativeLayout>
```


listview.xml：





```
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/listview"
    android:layout_width="200dp"
    android:layout_height="fill_parent" >

    <TextView
        android:id="@+id/textView1"
        android:layout_width="200dp"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_alignParentTop="true"
        android:layout_marginTop="10dp"
        android:text="TextView" />

</RelativeLayout>
```


效果：



![](https://img-blog.csdn.net/20130703111229312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**说明：**

ListView数据的渲染，可以使用适配器，例如SimpleAdapter：



```java
//获取ListView对象
        ListView listView = (ListView) findViewById(R.id.ListViewTest);
        //定义一个适配关系的数组
        String[] AdapterMap = new String[1];
        AdapterMap[0] = "title";
        //定义一个适配关系的ID数组
        int[] adapterId = new int[1];
        adapterId[0] = R.id.textView1;
        //定义一个HashMap
        ArrayList<Map<String, Object>> listMaps = new ArrayList<Map<String, Object>>();
        HashMap<String, Object> hashMap1 = new HashMap<String, Object>();
        hashMap1.put("title", "Hello World");
        HashMap<String, Object> hashMap2 = new HashMap<String, Object>();
        hashMap2.put("title", "Hello World2");
        listMaps.add(hashMap1);
        listMaps.add(hashMap2);
        //创建一个适配器 R.layout.listview 是listview模板
        SimpleAdapter myAdapter = (SimpleAdapter) new SimpleAdapter(this, listMaps, R.layout.listview, AdapterMap, adapterId);
        //ListView设置适配器
        listView.setAdapter(myAdapter);
```


**给ListView添加事件：**



```java
//添加一个Item点击后的事件
        listView.setOnItemClickListener(new OnItemClickListener() {

            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(getBaseContext(), "ItemClick", Toast.LENGTH_SHORT).show(); 
                //View view代表 listview.xml视图对象，可以通过findViewById来查找listview.xml中的视图对象
                TextView textView = (TextView) view.findViewById(R.id.textView1); 
                textView.setText("Hello World Chaneg"); //改变文案
            }
        });
```


点击后效果：

![](https://img-blog.csdn.net/20130703113313531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





**getChildAt方法：**

ListView 不能直接通过position来获取一个Item对象。但是我们有一个listView.getChildAt(index)的方法。

注意：getChildAt只能获取到视图中现在显示的Item，例如我有10个Item，页面上只显示5个，那么index值只能0-5。

如果我们需要根据从头到尾所有的Item来获取其中的Item，我们可以先通过getFirstVisiblePosition()获取到第一个Item的pos，然后再通过getChildAt来进行计算。

详细不作展开。





背景选择器：



```java
<?xml version="1.0" encoding="utf-8" ?>

<selector xmlns:android="http://schemas.android.com/apk/res/android">

    <!-- 默认时的背景图片-->

    <item android:drawable="@drawable/pic1" />

    <!-- 没有焦点时的背景图片-->

    <item android:state_window_focused="false" android:drawable="@drawable/pic1" />

    <!-- 非触摸模式下获得焦点并单击时的背景图片-->

    <item android:state_focused="true" android:state_pressed="true"

        android:drawable="@drawable/pic2" />

    <!-- 触摸模式下单击时的背景图片-->

    <item android:state_focused="false" android:state_pressed="true"

        android:drawable="@drawable/pic3" />

    <!--选中时的图片背景-->

    <item android:state_selected="true" android:drawable="@drawable/pic4" />

    <!--获得焦点时的图片背景-->

    <item android:state_focused="true" android:drawable="@drawable/pic5" />

</selector>
```







