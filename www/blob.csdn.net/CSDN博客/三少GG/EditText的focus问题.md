# EditText的focus问题 - 三少GG - CSDN博客
2012年03月19日 14:24:31[三少GG](https://me.csdn.net/scut1135)阅读数：2774标签：[android																[layout](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)

### [Android EditText remove focus](http://blog.kenyang.net/2012/01/android-edittext-remove-focus.html)
只要頁面中有用到EditText 的，
只要跑到那個頁面，一定會被自動focus，也就是說鍵盤會自動彈開!
要取消focus，只要在你layout中的root element加上下面兩個屬性，
(root element就是最外圍的那個element，預設都是LinearLayout）
android:focusable="true"
android:focusableInTouchMode="true"
**一个Acvitivy Start了之后，会把Focus设置到一个Item上。有时候正好是EditText，而这个时候又不希望EditText处于选中状态。**++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
失败办法有：
EditText.setSelected(false);
EditText.ClearFocus();
解决方法：增加一个可以focus的宽度为0的linearLayout
<LinearLayout
    android:focusable="true" android:focusableInTouchMode="true"
    android:layout_width="0px" android:layout_height="0px"/>
具体做法是，建一个linearlayout,长宽都为0，而且设置focusable和touchfocusable在TextView旁边。
这样，对TextView或者EditText使用ClearFocus就会把Focus更改到LinearLayout上。TextView 就失去焦点了。
