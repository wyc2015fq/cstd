# Android第三课--ListView - Orisun - 博客园







# [Android第三课--ListView](https://www.cnblogs.com/zhangchaoyang/articles/1795256.html)





首届 Google 暑期大学生博客分享大赛——2010 Andriod 篇

由于手机屏幕的限制导致了我们做Android开发要经常用到ListView控件，先给大家看一下什么是ListView:

![](https://pic002.cnblogs.com/img/zcy/201008/2010080819335260.png)![](https://pic002.cnblogs.com/img/zcy/201008/2010080819343187.png)

先上一段小程序：

```
1 package ccmtc.summercamp.mobile;
 2 
 3  import android.app.ListActivity;
 4  import android.os.Bundle;
 5  import android.view.View;
 6  import android.widget.AdapterView;
 7  import android.widget.AdapterView.OnItemSelectedListener;
 8  import android.widget.ArrayAdapter;
 9  import android.widget.ListView;
10 
11  public class StudentHome extends ListActivity {
12 
13     @Override
14     public void onCreate(Bundle savedInstanceState) {
15       super.onCreate(savedInstanceState);
16 
17       String[] countries = getResources().getStringArray(R.array.functionS_array);
18       setListAdapter(new ArrayAdapter<String>(this, R.layout.list_item, countries));
19 
20       ListView lv = getListView();
21       lv.setTextFilterEnabled(true);
22 
23       lv.setOnItemSelectedListener(new OnItemSelectedListener() {
24 
25         public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,
26                 long arg3) {
27             // TODO Auto-generated method stub
28              
29         }
30 
31         public void onNothingSelected(AdapterView<?> arg0) {
32             // TODO Auto-generated method stub
33              
34         }
35         
36       });
37     }
38 
39 }
```


第17行从资源文件中获取数组的元素，看我们的values/strings.xml文件中有一个string-array：

```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string-array name="functionS_array">
        <item>查看新任务</item>
        <item>提交作业</item>
        <item>我的分数</item>
        <item>修改密码</item>
    </string-array>
</resources>
```


对于要显示ListView的页面来说我们都直接继承自ListActivity，这样会比你继承自Activity省去很多代码。

第18行出现一个setAdapter方法和一个ArrayAdapter类，先看一下Adapter是什么（它是Android.widget的一个子类）：

An Adapter object acts as a bridge between an `AdapterView` and the underlying data for that view. The Adapter provides access to the data items. The Adapter is also responsible for making a `View` for each item in the data set.

Android.widget.ArrayAdapter继承自Android.widget.BaseAdapter，BaseAdapter定义为：

Common base class of common implementation for an `[Adapter](file:///D:/AndndroidSDK_2.2/docs/reference/android/widget/Adapter.html)` that can be used in both `[ListView](file:///D:/AndndroidSDK_2.2/docs/reference/android/widget/ListView.html)` (by implementing the specialized  `ListAdapter`  interface }  and  `Spinner `(by implementing the specialized `SpinnerAdapter` interface.

ArrayAdapter的构造函数：

ArrayAdapter(Context context, int textViewResourceId)

ArrayAdapter(Context context, int resource, int textViewResourceId)

ArrayAdapter(Context context, int textViewResourceId, T[] objects)

ArrayAdapter(Context context, int resource, int textViewResourceId, T[] objects)

ArrayAdapter(Context context, int textViewResourceId, List<T> objects)

ArrayAdapter(Context context, int resource, int textViewResourceId, List<T> objects)

在我们的代码中使用的是第三个构造函数，参数countries是负责提供数据的，参数R.layout.list_item指明了ListView的每一项要显示什么东西，ListView的每一项自然是一个View(或其子类)的一个实例，这个View或许只是一个简单的TextView，也可能是一个ViewGroup(包含了很多控件)。

看看我们的layout.list_item.xml里放着什么：

```
<?xml version="1.0" encoding="utf-8"?>
<TextView xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:padding="10dp"
    android:textSize="16sp" >
</TextView>
```

但更多的时候我们会把第二个参数设为R.layout.simple_list_item_1，这是Android定义好的一种item的显示方式，我们不用自己去写xml文件了。


看一下25行中onItemSelected()方法中的各参数是什么含义：

public abstract void onItemSelected (AdapterView<?> parent, View view, int position, long id)

Callback method to be invoked when an item in this view has been selected. Impelmenters can call getItemAtPosition(position) if they need to access the data associated with the selected item.

Parameters

parent　　　The AdapterView where the selection happened

view　　　　The view within the AdapterView that was clicked　　对应的就是ArrayAdapter构造函数中的第二个参数R.layout.list_item

position　　The position of the view in the adapter　　　　第一项position=0，第二项position=1...

id　　　　　The row id of the item that is selected

看帮助文档中给的一个例子：

```
lv.setOnItemClickListener(new OnItemClickListener() {
    public void onItemClick(AdapterView<?> parent, View view,
        int position, long id) {
      // When clicked, show a toast with the TextView text
      Toast.makeText(getApplicationContext(), ((TextView) view).getText(),
          Toast.LENGTH_SHORT).show();
    }
  });
```

除了ArrayAdapter外，还可以用SimpleAdapter和SimpleCursorAdapter设置ListView中的数据，这三者的区别在于数据源不同：

String[]: ArrayAdapter 
List<Map<String,?>>: SimpleAdapter 
数据库Cursor: SimpleCursorAdapter 

下面重点讲一下SimpleAdapter ：

An easy adapter to map static data to views defined in an XML file. You can specify the data backing the list as an ArrayList of Maps. Each entry in the ArrayList corresponds to one row in the list. The Maps contain the data for each row. You also specify an XML file that defines the views used to display the row, and a mapping from keys in the Map to specific views. Binding data to views occurs in two phases.

看完下面这段程序再来看这段英文理解会好一些。

layout/main.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<ListView
    xmlns:android="http://schemas.android.com/apk/res/android"   
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:id="@+id/lv">
</ListView>
```

ShowList.java

```
package zcy.com;

import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

import android.app.Activity;
import android.os.Bundle;
import android.widget.SimpleAdapter;
import android.widget.ListView;

public class ShowList extends Activity {
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        ArrayList<HashMap<String,Object>> works=new ArrayList<HashMap<String,Object>>();
        for(int i=0;i<7;i++){
            HashMap<String,Object> work=new HashMap<String,Object>();
            work.put("img",R.drawable.rotateleft);
            work.put("name","第"+(i+1)+"章作业名称");
            work.put("time",new Date().toLocaleString());
            work.put("mark",(90-i)+"分");
            works.add(work);
        }
        SimpleAdapter simpleAdapter=new SimpleAdapter(this,//Context
                works,//数据源
                R.layout.works,//ListView中的每一项如何显示
                new String[]{"img","name","time","mark"},//对应数据源works中HashMap的每一个键值
                new int[]{R.id.img,R.id.workname,R.id.date,R.id.mark}//对应View中的id
                );
        ((ListView)findViewById(R.id.lv)).setAdapter(simpleAdapter);
    }
}
```

layout/works.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<LinearLayout
     xmlns:android="http://schemas.android.com/apk/res/android"    
     android:layout_width="fill_parent"
     android:layout_height="fill_parent"
     android:gravity="center_vertical">
     <ImageView    
        android:layout_width="wrap_content"    
        android:layout_height="wrap_content"   
        android:id="@+id/img"/>    
     <TextView 
         android:layout_width="wrap_content"
         android:layout_height="wrap_content"         
         android:textSize="20sp"
         android:id="@+id/workname"/>
     <LinearLayout
         android:layout_width="fill_parent"
         android:layout_height="wrap_content"
         android:gravity="right"
         android:orientation="vertical">
         <TextView 
             android:layout_width="wrap_content"
             android:layout_height="wrap_content"
             android:id="@+id/date"/>
         <TextView 
             android:layout_width="wrap_content"
             android:layout_height="wrap_content"
             android:id="@+id/mark"/>
     </LinearLayout>
</LinearLayout>
```

看一下SimpleAdapter的构造函数是怎么定义的：

#### publicSimpleAdapter([Context](http://www.cnblogs.com/reference/android/content/Context.html)context,[List](http://www.cnblogs.com/reference/java/util/List.html)<? extends [Map](http://www.cnblogs.com/reference/java/util/Map.html)<[String](http://www.cnblogs.com/reference/java/lang/String.html), ?>> data, int resource,[String[]](http://www.cnblogs.com/reference/java/lang/String.html)from, int[] to)

Since:[API Level 1](http://www.cnblogs.com/guide/appendix/api-levels.html#level1)



Constructor



##### Parameters
toThe views that should display column in the "from" parameter. These should all be TextViews. The first N views in this list are given the values of the first N columns in the from parameter.


运行效果图

![](https://pic002.cnblogs.com/img/zcy/201008/2010080913374279.png)

其实ArrayAdapter,SimpleAdapter,SimpleCursorAdapter都继承自BaseAdapter类，而BaseAdapter又继承自Adapter，当Android提供的这些XXAdapter不能满足需要时可以自已定义一个Adapter或BaseAdapter的子类。

以下内容转自 [http://www.j2mehome.com/System/Android/3605.html](http://www.j2mehome.com/System/Android/3605.html)

  Android中提供了大量的适配器Adapter，常见的有BaseAdapter、SimpleCursorAdapter、CursorAdapter等等，其中包含Cursor可以自动和数据库关联。一般在ListView中我们需要定义一些显示的内容，需要用到继承Adapter实现窗口的子类化。


  一般我们需要重写Adapter类的四个方法即可，分别是public int getCount() 、public Object getItem(int position)、public long getItemId(int position)和public View getView(int position, View convertView, ViewGroup parent) ，调用时仍然是在ListView或ListActivity中直接setAdapter(CustomAdapter);即可，比较典型的方法参考Android的自带控件Gallery的实现，详细的在API Demo中查找即可。

自定义BaseAdapter子类可参考 [http://qaohao.javaeye.com/blog/506421](http://qaohao.javaeye.com/blog/506421)

下面再给一个根据用户输入ListView的项目可以动态增加的例子


ListView myListView=(ListView)findViewById(R.id.myListView);
        final EditText myEditText=(EditText)findViewById(R.id.myEditText);
        
        final ArrayList<String> todoItems=new ArrayList<String>();
        int resID=R.layout.todolist_item;
        final ArrayAdapter<String> aa=new ArrayAdapter<String>(this,resID,todoItems);
        myListView.setAdapter(aa);
        
        myEditText.setOnKeyListener(new OnKeyListener(){
        	public boolean onKey(View v,int keyCode,KeyEvent event){
        		if(event.getAction()==KeyEvent.ACTION_DOWN)
        			if(keyCode==KeyEvent.KEYCODE_DPAD_CENTER){
        				todoItems.add(0,myEditText.getText().toString());
        				aa.notifyDataSetChanged();
        				myEditText.setText("");
        				return true;
        			}
        		return false;
        	}
        });














