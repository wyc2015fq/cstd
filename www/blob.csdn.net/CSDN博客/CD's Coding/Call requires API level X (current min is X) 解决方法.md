# Call requires API level X (current min is X) 解决方法 - CD's Coding - CSDN博客





2014年10月03日 11:47:12[糖果天王](https://me.csdn.net/okcd00)阅读数：764









`Call requires API level 9 (current min is 8):`


安卓开发中，经常用了一个函数就立马出现这个问题，调半天调不出，实际上是权限不够的问题

在AndroidManifest.xml中的 

package="com.xxx.bt" android:versionCode="1" android:versionName="1.0"> 这一行下面添加上（如果有的话直接修改即可）


<uses-sdk android:minSdkVersion="9" /> （此处的数字是所需要的level，按情况而定）





注：bin\和 res\layout\ 下都有，都要改哦



