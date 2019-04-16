# 安卓开发08：布局-相对位置布局 RelativeLayout - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年06月25日 16:39:43[initphp](https://me.csdn.net/initphp)阅读数：2911
所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









**RelativeLayout：**

相对位置布局。根据相对的位置来进行布局排放。




看一个例子：



```
<!-- 相对位置布局总体框架 -->
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >

    <!-- Table布局，marginTop132dp 靠左边-->
    <TableLayout
        android:id="@+id/tableLayout1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_marginTop="132dp" >

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
                android:layout_width="200dp"
                android:layout_height="wrap_content" >

                <requestFocus />
            </EditText>
        </TableRow>

        <TableRow
            android:id="@+id/tableRow2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" >

            <TextView
                android:id="@+id/textView2"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Password:" />

            <EditText
                android:id="@+id/editText2"
                android:layout_width="200dp"
                android:layout_height="wrap_content" >

                <requestFocus />
            </EditText>
        </TableRow>
    </TableLayout>

    <!-- Button按钮 在table布局下方，并且和table布局marginTop距离30dp 靠左45dp-->
    <Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_below="@+id/tableLayout1"
        android:layout_marginLeft="45dp"
        android:layout_marginTop="30dp"
        android:text="Button" />

</RelativeLayout>
```


效果图：



![](https://img-blog.csdn.net/20130625163512218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




说明：

1. table布局。layout_marginTop是与布局的上边框距离132dp，layout_alignParentLeft靠左展示。




```
<TableLayout
        android:id="@+id/tableLayout1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_marginTop="132dp" >
```


2. Button layout_below="@+id/tableLayout1" 是在table布局下边，如果没有这个参数，则相对位置就是整个布局页面；layout_alignParentLeft 靠左布局。layout_marginLeft="45dp"左边45dp距离；layout_marginTop="30dp" 与上边距（Talbe布局）30dp;





```
<Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_below="@+id/tableLayout1"
        android:layout_marginLeft="45dp"
        android:layout_marginTop="30dp"
        android:text="Button" />
```


**总结：相对布局可以做总体布局，或者某一块小范围的相对位置布局**




更复杂点的布局:



```
<!-- 相对位置布局总体框架 -->
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="fill_parent"
    android:layout_height="fill_parent" >

    <!-- Table布局，marginTop132dp 靠左边 -->

    <TableLayout
        android:id="@+id/tableLayout1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_marginTop="132dp" >

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
                android:layout_width="200dp"
                android:layout_height="wrap_content" >

                <requestFocus />
            </EditText>
        </TableRow>

        <TableRow
            android:id="@+id/tableRow2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content" >

            <TextView
                android:id="@+id/textView2"
                android:layout_width="wrap_content"
                android:layout_height="wrap_content"
                android:text="Password:" />

            <EditText
                android:id="@+id/editText2"
                android:layout_width="200dp"
                android:layout_height="wrap_content" >

                <requestFocus />
            </EditText>
        </TableRow>
    </TableLayout>

    <!-- Button按钮 在table布局下方，并且和table布局marginTop距离30dp 靠左45dp -->

    <Button
        android:id="@+id/button1"
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_alignParentLeft="true"
        android:layout_below="@+id/tableLayout1"
        android:layout_marginLeft="45dp"
        android:layout_marginTop="30dp"
        android:text="Button" />

    <!-- 相对布局 -->

    <RelativeLayout
        android:layout_width="300dp"
        android:layout_height="100dp"
        android:layout_below="@+id/button1"
        android:layout_marginTop="10dp" >

        <Button
            android:id="@+id/button2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_centerHorizontal="true"
            android:layout_centerVertical="true"
            android:text="居中" />

        <Button
            android:id="@+id/button2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:text="靠左" />

        <Button
            android:id="@+id/button2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_alignParentRight="true"
            android:text="靠右" />

        <Button
            android:id="@+id/button2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_marginTop="60dp"
            android:text="靠左下" />

        <Button
            android:id="@+id/button2"
            android:layout_width="wrap_content"
            android:layout_height="wrap_content"
            android:layout_alignParentRight="true"
            android:layout_marginTop="60dp"
            android:text="靠右下" />
    </RelativeLayout>

</RelativeLayout>
```


效果图：






![](https://img-blog.csdn.net/20130625164851937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**常用相对布局属性表：**



 第一类:属性值为true或false 

    android:layout_centerHrizontal  水平居中 

    android:layout_centerVertical   垂直居中 

    android:layout_centerInparent    相对于父元素完全居中 

    android:layout_alignParentBottom 贴紧父元素的下边缘 

    android:layout_alignParentLeft   贴紧父元素的左边缘 

    android:layout_alignParentRight  贴紧父元素的右边缘 

    android:layout_alignParentTop    贴紧父元素的上边缘 

    android:layout_alignWithParentIfMissing  如果对应的兄弟元素找不到的话就以父元素做参照物 

第二类：属性值必须为id的引用名“@id/id-name” 

    android:layout_below      在某元素的下方 

    android:layout_above      在某元素的的上方 

    android:layout_toLeftOf   在某元素的左边 

    android:layout_toRightOf  在某元素的右边 

    android:layout_alignTop   本元素的上边缘和某元素的的上边缘对齐 

    android:layout_alignLeft  本元素的左边缘和某元素的的左边缘对齐 

    android:layout_alignBottom 本元素的下边缘和某元素的的下边缘对齐 

    android:layout_alignRight  本元素的右边缘和某元素的的右边缘对齐 

第三类：属性值为具体的像素值，如30dip，40px 

    android:layout_marginBottom              离某元素底边缘的距离 

    android:layout_marginLeft                   离某元素左边缘的距离 

    android:layout_marginRight                 离某元素右边缘的距离 

    android:layout_marginTop                   离某元素上边缘的距离 




