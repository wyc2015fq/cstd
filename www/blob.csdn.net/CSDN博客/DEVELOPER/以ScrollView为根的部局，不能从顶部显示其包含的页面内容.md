# 以ScrollView为根的部局，不能从顶部显示其包含的页面内容 - DEVELOPER - CSDN博客





2014年09月03日 14:12:31[学术袁](https://me.csdn.net/u012827205)阅读数：2517










以ScrollView为根的部局，不能从顶部显示其包含的页面内容；是我最头疼的问题了，很多冲突一个一个的解决。到最后遇到这么一个问题，在网上搜搜，却不知道使用什么关键字查！网上资源就查到那几条关于这方面的论坛谈话。这也算是一个给遇到该麻烦的小伙伴们出了道难题，现在我就来解释一下！

问题由来：

基本上就是这样一个布局：

```java
<?xml version="1.0" encoding="utf-8"?>
<ScrollView xmlns:android="http://schemas.android.com/apk/res/android"
    android:id="@+id/shortMovieScrollView"
    android:layout_width="fill_parent"
    android:layout_height="wrap_content"
    android:background="#fff" >

    <RelativeLayout
        android:layout_width="fill_parent"
        android:layout_height="wrap_content"
        android:background="#f8f8f8" >

         <include
        android:id="@+id/lay_shortMovie_titleBar"
        layout="@layout/title_public_item1" />
           
        <TextView
            android:id="@+id/tvLine_1"
            android:layout_width="fill_parent"
            android:layout_height="1dp"
            android:layout_alignBottom="@+id/lay_shortMovie_titleBar"
            android:background="#C8C7CC" />

        <RelativeLayout
            android:id="@+id/layout_filterBar"
            android:layout_width="fill_parent"
            android:layout_height="45dp"
            android:layout_below="@+id/lay_shortMovie_titleBar"
            android:background="#f8f8f8" >

            <LinearLayout
                android:layout_width="fill_parent"
                android:layout_height="45dp" >

                <TextView
                    android:id="@+id/tv_shortMovie_Selectleft"
                    android:layout_width="fill_parent"
                    android:layout_height="45dip"
                    android:layout_weight="1"
                    android:background="@drawable/btn_bg_color_selector"
                    android:drawableLeft="@drawable/icon_filter_teaching"
                    android:gravity="center_vertical"
                    android:paddingLeft="30dp"
                    android:text="筛选微电影"
                    android:textColor="#8f8f95"
                    android:textSize="16sp" />

                <ImageView
                    android:id="@+id/img_line_portrait"
                    android:layout_width="0.5dp"
                    android:layout_height="fill_parent"
                    android:background="#C8C7CC" />

                <TextView
                    android:id="@+id/tv_shortMovie_Selectright"
                    android:layout_width="fill_parent"
                    android:layout_height="45dip"
                    android:layout_weight="1"
                    android:background="@drawable/btn_bg_color_selector"
                    android:drawableLeft="@drawable/img_icon_random_filter"
                    android:gravity="center_vertical"
                    android:paddingLeft="30dip"
                    android:text="发现微电影"
                    android:textColor="#8f8f95"
                    android:textSize="16sp" />
            </LinearLayout>
        </RelativeLayout>

        <TextView
            android:id="@+id/tvLine_2"
            android:layout_width="fill_parent"
            android:layout_height="1dp"
            android:layout_below="@+id/layout_filterBar"
            android:background="#C8C7CC" />

        <android.support.v4.view.ViewPager
            android:id="@+id/shortMovie_viewpager"
            android:layout_marginTop="0.5dp"
            android:layout_width="fill_parent"
            android:layout_height="180dip"
            android:layout_below="@+id/tvLine_2" 
            android:background="#fff" />

        <LinearLayout
            android:id="@+id/shortMovie_viewGroup"
            android:layout_width="fill_parent"
            android:layout_height="35dip"
            android:layout_alignParentRight="true"
            android:layout_alignBottom="@+id/shortMovie_viewpager"
            android:background="@drawable/ic_transparent_content_title"
            android:gravity="center_vertical|right"
            android:orientation="horizontal"
            android:paddingRight="10dp" >
        </LinearLayout>

        <TextView
            android:id="@+id/filmName"
            android:layout_width="wrap_content"
            android:layout_height="35dp"
            android:layout_alignBottom="@+id/shortMovie_viewGroup"
            android:layout_alignParentLeft="true"
            android:gravity="center_vertical"
            android:text="一只狗的意愿"
            android:textColor="#fff"
            android:textSize="16sp" />

        <!-- 佳作推荐电影 -->

        <ImageView
            android:id="@+id/img_line_1"
            android:layout_width="3dip"
            android:layout_height="30dip"
            android:layout_below="@+id/shortMovie_viewGroup"
            android:layout_marginLeft="15dip"
            android:layout_marginTop="15dip"
            android:background="#f1a73c" />

        <TextView
            android:id="@+id/name_movieHeadLine_1"
            android:layout_width="wrap_content"
            android:layout_height="30dip"
            android:layout_below="@+id/shortMovie_viewGroup"
            android:layout_marginLeft="10dip"
            android:layout_marginTop="15dip"
            android:layout_toRightOf="@+id/img_line_1"
            android:gravity="center_vertical"
            android:text="佳作推荐"
            android:textSize="16sp" />

        <GridView
            android:id="@+id/item1_gridView_top"
            android:layout_width="fill_parent"
            android:layout_height="350dip"
            android:layout_below="@+id/img_line_1"
            android:layout_marginTop="5dip"
            android:cacheColorHint="#ffffff"
            android:listSelector="#ffffff"
            android:horizontalSpacing="5dip"
            android:transcriptMode="disabled"
            android:numColumns="3"
            android:scrollbars="none"
            android:verticalSpacing="20dip" />

        <!-- 国内电影 -->

        <ImageView
            android:id="@+id/img_line_2"
            android:layout_width="3dip"
            android:layout_height="30dip"
            android:layout_below="@+id/item1_gridView_top"
            android:layout_marginLeft="15dip"
            android:layout_marginTop="15dip"
            android:background="#f1a73c" />

        <TextView
            android:id="@+id/name_movieHeadLine_2"
            android:layout_width="wrap_content"
            android:layout_height="30dip"
            android:layout_below="@+id/item1_gridView_top"
            android:layout_marginLeft="10dip"
            android:layout_marginTop="15dip"
            android:layout_toRightOf="@+id/img_line_2"
            android:gravity="center_vertical"
            android:text="国内作品推荐"
            android:textSize="16sp" />

        <GridView
            android:id="@+id/item2_gridView_top"
            android:layout_width="fill_parent"
            android:layout_height="350dip"
            android:layout_below="@+id/img_line_2"
            android:layout_marginTop="5dip"
            android:cacheColorHint="#ffffff"
             android:listSelector="#ffffff"
            android:horizontalSpacing="5dip"
            android:transcriptMode="disabled"
            android:numColumns="3"
            android:scrollbars="none"
            android:verticalSpacing="20dip" />

        <!-- 国外电影 -->

        <ImageView
            android:id="@+id/img_line_3"
            android:layout_width="3dip"
            android:layout_height="30dip"
            android:layout_below="@+id/item2_gridView_top"
            android:layout_marginLeft="15dip"
            android:layout_marginTop="15dip"
            android:background="#f1a73c" />

        <TextView
            android:id="@+id/name_movieHeadLine_3"
            android:layout_width="wrap_content"
            android:layout_height="30dip"
            android:layout_below="@+id/item2_gridView_top"
            android:layout_marginLeft="10dip"
            android:layout_marginTop="15dip"
            android:layout_toRightOf="@+id/img_line_3"
            android:gravity="center_vertical"
            android:text="国外作品推荐"
            android:textSize="16sp" />

        <GridView
            android:id="@+id/item3_gridView_top"
            android:layout_width="fill_parent"
            android:layout_height="350dip"
            android:layout_below="@+id/img_line_3"
            android:layout_marginTop="5dip"
            android:cacheColorHint="#ffffff"
             android:listSelector="#ffffff"
            android:horizontalSpacing="5dip"
            android:transcriptMode="disabled"
            android:numColumns="3"
            android:scrollbars="none"
            android:verticalSpacing="20dip" />

    </RelativeLayout>

</ScrollView>
```


根部局是ScrollView，然后根部局中嵌套了一个TitleBar标题，其下面是一个ViewPager，然后再下面依次是三个GridView。



布局就是这样的，当我运行之后，页面显示结果不是整个页面从ScrollView的 y=0 位置显示。即：不是从顶部位置显示，而是冲ScrollView的中间部位展示该页面的所有内容；

解决办法：有人说使用方法：

```java
mScrollView.smoothScrollTo(0, 0);
```
或者
```java
mScrollView.scrollTo(0, 0);
```
但是经过我的测试使用，是不可以的。现有一个方法能解决：使用该布局最顶部的ScrollView中最顶部的那个控件View、layout都行，让后使用这几行代码使其获取焦点就行了；如下所示：
```java
view.setFocusable(true);
view.setFocusableInTouchMode(true);
view.requestFocus();
```









