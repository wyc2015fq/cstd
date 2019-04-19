# Android中TextVIew一些属性 - xqhrs232的专栏 - CSDN博客
2012年04月16日 18:08:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：542标签：[android																[layout																[integer																[null																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=integer&t=blog)](https://so.csdn.net/so/search/s.do?q=layout&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/weichaohnu/article/details/5628184](http://blog.csdn.net/weichaohnu/article/details/5628184)
android:layout_gravity="center_vertical"
设置控件显示的位置：默认top，这里居中显示，还有bottom
android:hint="请输入数字！"
设置显示在空间上的提示信息
android:numeric="integer"
设置只能输入整数，如果是小数则是：decimal
android:singleLine="true"
设置单行输入，一旦设置为true，则文字不会自动换行。
android:password="true"
设置只能输入密码
android:gravity="top"
EditText设置，这一行就可以让光标处于第一行了，若不设置默认就居中
TextView则在最顶上
android:textColor = "#ff8c00"
字体颜色
android:textStyle="bold"
字体，bold, italic, bolditalic
android:textSize="20dip"
大小
android:capitalize = "characters"
以大写字母写
android:textAlign="center"
EditText没有这个属性，但TextView有
android:autoText：自动拼写帮助
android:editable：是否可编辑
android:textColorHighlight="#cccccc"
被选中文字的底色，默认为蓝色
android:textColorHint="#ffff00"
设置提示信息文字的颜色，默认为灰色
android:textScaleX="1.5"
控制字与字之间的间距
android:typeface="monospace"
字型，normal, sans, serif, monospace
android:background="@null"
空间背景，这里没有，指透明，将EditText自定义的背景去掉
android:imeOptions="actionDone"
设置软键盘的Enter键
android:layout_weight="1"
权重，控制控件之间的地位,在控制控件显示的大小时蛮有用的。
android:textAppearance="?android:attr/textAppearanceLargeInverse"
文字外观，这里引用的是系统自带的一个外观，？表示系统是否有这种外观，否则使用默认的外观。不知道这样理解对不对？
