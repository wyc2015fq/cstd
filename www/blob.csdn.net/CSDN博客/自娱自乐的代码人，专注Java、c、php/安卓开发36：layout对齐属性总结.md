# 安卓开发36：layout对齐属性总结 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年09月12日 13:43:26[initphp](https://me.csdn.net/initphp)阅读数：1457
个人分类：[安卓开发笔记系列](https://blog.csdn.net/initphp/article/category/1439903)

所属专栏：[安卓开发](https://blog.csdn.net/column/details/androidgo.html)









android:layout_above    将该控件的底部至于给定ID的控件之上


android:layout_below    将该控件的顶部至于给定ID的控件之下


android:layout_toLeftOf    将该控件的右边缘和给定ID的控件的左边缘对齐


android:layout_toRightOf    将该控件的左边缘和给定ID的控件的右边缘对齐


android:layout_alignBaseline    该控件的baseline和给定ID的控件的baseline对齐


android:layout_alignBottom    将该控件的底部边缘与给定ID控件的底部边缘


android:layout_alignLeft     将该控件的左边缘与给定ID控件的左边缘对齐


android:layout_alignRight     将该控件的右边缘与给定ID控件的右边缘对齐


android:layout_alignTop     将给定控件的顶部边缘与给定ID控件的顶部对齐


android:alignParentBottom     如果该值为true，则将该控件的底部和父控件的底部对齐


android:layout_alignParentLeft    如果该值为true，则将该控件的左边与父控件的左边对齐


android:layout_alignParentRight    如果该值为true，则将该控件的右边与父控件的右边对齐


android:layout_alignParentTop    如果该值为true，则将空间的顶部与父控件的顶部对齐


android:layout_centerHorizontal    如果值为真，该控件将被至于水平方向的中央


android:layout_centerInParent    如果值为真，该控件将被至于父控件水平方向和垂直方向的中央


android:layout_centerVertical    如果值为真，该控件将被至于垂直方向的中央




第一类:属性值为true或false 

android:layout_centerHrizontal 水平居中

android:layout_centerVertical  垂直居中

android:layout_centerInparent   相对于父元素完全居中

android:layout_alignParentBottom贴紧父元素的下边缘

android:layout_alignParentLeft  贴紧父元素的左边缘

android:layout_alignParentRight 贴紧父元素的右边缘

android:layout_alignParentTop   贴紧父元素的上边缘

android:layout_alignWithParentIfMissing 如果对应的兄弟元素找不到的话就以父元素做参照物

第二类：属性值必须为id的引用名“@id/id-name” 

    android:layout_below     在某元素的下方

    android:layout_above     在某元素的的上方

    android:layout_toLeftOf  在某元素的左边

    android:layout_toRightOf 在某元素的右边

    android:layout_alignTop  本元素的上边缘和某元素的的上边缘对齐

    android:layout_alignLeft 本元素的左边缘和某元素的的左边缘对齐

    android:layout_alignBottom本元素的下边缘和某元素的的下边缘对齐

    android:layout_alignRight 本元素的右边缘和某元素的的右边缘对齐

第三类：属性值为具体的像素值，如30dip，40px 

    android:layout_marginBottom             离某元素底边缘的距离

    android:layout_marginLeft                  离某元素左边缘的距离

    android:layout_marginRight                离某元素右边缘的距离

    android:layout_marginTop                  离某元素上边缘的距离


**EditText****的android:hint**

设置EditText为空时输入框内的提示信息。

**android:gravity**
android:gravity属性是对该view内容的限定．比如一个button上面的text. 你可以设置该text在view的靠左，靠右等位置．以button为例，android:gravity="right"则button上面的文字靠右

**android:layout_gravity**
android:layout_gravity是用来设置该view相对与起父view的位置．比如一个button在linearlayout里，你想把该button放在靠左、靠右等位置就可以通过该属性设置．以button为例，android:layout_gravity="right"则button靠右
****android:layout_alignParentRight****
使当前控件的右端和父控件的右端对齐。这里属性值只能为true或false，默认false。

**android:scaleType：**
android:scaleType是控制图片如何resized/moved来匹对ImageView的size。ImageView.ScaleType
 / android:scaleType值的意义区别：

CENTER /center 按图片的原来size居中显示，当图片长/宽超过View的长/宽，则截取图片的居中部分显示

CENTER_CROP / centerCrop 按比例扩大图片的size居中显示，使得图片长(宽)等于或大于View的长(宽) 

CENTER_INSIDE / centerInside 将图片的内容完整居中显示，通过按比例缩小或原来的size使得图片长/宽等于或小于View的长/宽

FIT_CENTER / fitCenter 把图片按比例扩大/缩小到View的宽度，居中显示

FIT_END / fitEnd  把图片按比例扩大/缩小到View的宽度，显示在View的下部分位置

FIT_START / fitStart 把图片按比例扩大/缩小到View的宽度，显示在View的上部分位置

FIT_XY / fitXY 把图片不按比例扩大/缩小到View的大小显示
MATRIX / matrix用矩阵来绘制，动态缩小放大图片来显示。要注意一点，Drawable文件夹里面的图片命名是不能大写的。




