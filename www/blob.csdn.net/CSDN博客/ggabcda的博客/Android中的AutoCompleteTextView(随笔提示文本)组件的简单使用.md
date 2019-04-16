# Android中的AutoCompleteTextView(随笔提示文本)组件的简单使用 - ggabcda的博客 - CSDN博客





2017年09月01日 15:49:38[ggabcda](https://me.csdn.net/ggabcda)阅读数：158








Android中的随笔提示文本组件AutoCompleteTextView的使用，此组件用于输入文本，然后就会在所配置的适配器中的数据进行查找显示在组件下面。

　　这里值得注意的是AutoCompleteTextView使用时，你使用一个字符是没有反应的，你至少要输入两个字符以上才有反应。

本文只做记录作用，有差错的地方望指出**《黑幕下的人》**

**![](http://images2017.cnblogs.com/blog/1228744/201708/1228744-20170830213207874-1681251034.png)![](http://images2017.cnblogs.com/blog/1228744/201708/1228744-20170830213351187-1479204871.png)**

**java代码**

```
package cn.hmxin.autocompletetext;

import android.os.Bundle;
import android.support.v7.app.ActionBarActivity;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;

public class MainActivity extends ActionBarActivity {
    private AutoCompleteTextView autoText = null ;//声明AutoCompleteTextView对象，用于绑定布局文件中的组件
    private ArrayAdapter<String> adapter = null ;//声明ArrayAdapter对象《适配器》
    //声明并实例化一个String数组，放在适配器中
    String[] strs = new String[] {"C", "C++", "Java", "JavaWeb", "JavaScript", "Visual Basic", "汇编", "python", "Android"};
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        autoText = (AutoCompleteTextView)findViewById(R.id.autotext);//绑定组件
        //实例化适配器，并设置其数据
        adapter = new ArrayAdapter<String>(MainActivity.this, android.R.layout.simple_dropdown_item_1line, strs);
        autoText.setAdapter(adapter);//设置autoText对象中的适配器为adapter
    }
}
```

xml代码

```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    xmlns:tools="http://schemas.android.com/tools"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:orientation="vertical"
    tools:context="cn.hmxin.autocompletetext.MainActivity" >
    <AutoCompleteTextView
        android:id="@+id/autotext"
        android:layout_width="match_parent"
        android:layout_height="wrap_content"/>
</LinearLayout>
```





