# Android layout_margin 无效的解决办法 - =朝晖= - 博客园
# [Android layout_margin 无效的解决办法](https://www.cnblogs.com/dhcn/p/7121010.html)
http://www.aichengxu.com/view/31025
1、如果LinearLayout中使用[Android](http://lib.csdn.net/base/android):layout_marginRight不起作用，通过[测试](http://lib.csdn.net/base/softwaretest)原来在android2.x中，如果一个控件中有[android](http://lib.csdn.net/base/android):layout_gravity属性，就会出现android:layout_marginRight没有应有的效果，而是把左边距加到右边距上去，直接去掉android:layout_gravity这个属性就能解决
 2、如果是在RelativeLayout中不起作用，请在这个代码前加了一条android:layout_alignParentRight="true"，就行
3、如果是ScrollView中的LinearLayout 中设置margin 类的属性无效，解决方法 LinearLayout中加android:layout_gravity="top"属性就ok,
4、如果是LineaerLayout放到scrollview或者RelativeLayou里面中layout_margin失效不起作用，解决方法在属性里面加入android:layout_gravity="top"，大家注意跟1是不一样的，1是LinearLayout里面的控件，而这里是指LinearLayout在其他控件中的情况，请分清对待。
5、ListView里面子view layout_margin无效的解决办法，android:devider+android:deviderHeight属性

