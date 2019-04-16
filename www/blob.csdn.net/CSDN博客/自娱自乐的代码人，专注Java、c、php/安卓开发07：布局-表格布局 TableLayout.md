# 安卓开发07：布局-表格布局 TableLayout - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月25日 10:26:40[initphp](https://me.csdn.net/initphp)阅读数：1553
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)








** TableLayout：**
TableLayout可以把视图views组织成“行”或“列”。可以使用<TableRow>元素指定表格中的一行。每一行又可以包含一个或多个视图。每行中的每个视图组成了表格的一个元素。每列的宽度，取决于这一列中宽度最大的视图view。




看一个例子：



```
<LinearLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent"
    android:orientation="vertical" >

    <!-- Table布局 -->

    <TableLayout
        android:layout_width="wrap_content"
        android:layout_height="wrap_content" >

        <!-- 第一行 -->

        <TableRow
            android:id="@+id/tableRow1"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" >

            <TextView
                android:id="@+id/textView1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="UserName:" />

            <EditText
                android:id="@+id/editText1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:ems="10"
                android:text="" >

                <requestFocus />
            </EditText>
        </TableRow>

        <!-- 第二行 -->

        <TableRow
            android:id="@+id/tableRow1"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" >

            <TextView
                android:id="@+id/textView1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="UserPassword:" />

            <EditText
                android:id="@+id/editText1"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:ems="10"
                android:text="" >

                <requestFocus />
            </EditText>
        </TableRow>
    </TableLayout>

</LinearLayout>
```


效果图：



![](https://img-blog.csdn.net/20130625155553515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**总结：Table简单易用，适用于表单等一些根据每一列自动对齐的场景使用。**







