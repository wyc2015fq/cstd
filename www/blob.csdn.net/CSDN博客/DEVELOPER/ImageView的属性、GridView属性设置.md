# ImageView的属性、GridView属性设置 - DEVELOPER - CSDN博客





2014年03月20日 14:31:03[学术袁](https://me.csdn.net/u012827205)阅读数：1786








有些时候，ImageView直接是在xml中设置。这样很多属性都知道怎么做！但是换到使用代码去实现这些，那就有些不知所措了。

所以这样才能更明白：



```java
public View getView(int position, View convertView, ViewGroup parent){
     ImageView imageView = new ImageView(mContext);
     // 设置当前图像的图像（position为当前图像列表的位置）
     imageView.setImageResource(resIds[position]);
     imageView.setScaleType(ImageView.ScaleType.FIT_XY);
     imageView.setLayoutParams(new Gallery.LayoutParams(163, 106));
    // 设置Gallery组件的背景风格
    imageView.setBackgroundResource(mGalleryItemBackground);
    return imageView;
}
```


*GridView属性设置：*

```java
android:numColumns=”auto_fit”   //GridView的列数设置为自动

android:columnWidth=”90dp "       //每列的宽度，也就是Item的宽度

android:stretchMode=”columnWidth"//缩放与列宽大小同步

android:verticalSpacing=”10dp”          //两行之间的边距

android:horizontalSpacing=”10dp”      //两列之间的边距 

android:cacheColorHint="#00000000" //去除拖动时默认的黑色背景

android:listSelector="#00000000"        //去除选中时的黄色底色

android:scrollbars="none"                   //隐藏GridView的滚动条

android:fadeScrollbars="true"             //设置为true就可以实现滚动条的自动隐藏和显示

android:fastScrollEnabled="true"      //GridView出现快速滚动的按钮(至少滚动4页才会显示)

android:fadingEdge="none"                //GridView衰落(褪去)边缘颜色为空，缺省值是vertical。(可以理解为上下边缘的提示色)

android:fadingEdgeLength="10dip"   //定义的衰落(褪去)边缘的长度

android:stackFromBottom="true"       //设置为true时，你做好的列表就会显示你列表的最下面

android:transcriptMode="alwaysScroll" //当你动态添加数据时，列表将自动往下滚动最新的条目可以自动滚动到可视范围内

android:drawSelectorOnTop="false"  //点击某条记录不放，颜色会在记录的后面成为背景色,内容的文字可见(缺省为false)
```









