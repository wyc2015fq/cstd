# 【Android Debug系列】Conversion to Dalvik format failed with error - 三少GG - CSDN博客
2011年06月09日 19:19:00[三少GG](https://me.csdn.net/scut1135)阅读数：1547标签：[android																[eclipse																[properties																[jar																[include																[delete](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=include&t=blog)](https://so.csdn.net/so/search/s.do?q=jar&t=blog)](https://so.csdn.net/so/search/s.do?q=properties&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android 常规](https://blog.csdn.net/scut1135/article/category/1057214)
如何从已有android编译工程，得到一个新的eclipse工程
用ec**lipse导入import工程, bulid path---> add externel libraties-->sdk-android  包-**> 如果没错  在bin目录下自动生成 apk （若想重新生成，project -->clean）
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Conversion to Dalvik format failed with error 1 
### Android 的ADT还是有待改进，还是dex有问题，反正这个问题就是这么解决的：
Conversion to Dalvik format failed with error 1
解决办法是 **Project > Clean... > Clean project selected below > Ok**
另外当加入多个jar包也可能出现这个问题，可以将jar包放在assert中，然后用add lib加入jar包可以解决这个问题..
最后解决方案（Grant）：
**Go to Project » Properties » Java Build Path » Libraries and remove all except the "Android X.Y" (in my case Android 1.5). click OK. Go to Project » Clean » Clean projects selected below » select your project and click OK. That should work.**
**i **solved the problem.
i seems that is** have two jars on my buildpath that include the same package and classes.**
i used `smack.jar` and `android_maps_lib-1.0.2`
delete this package from one jar solved the problem.
someone got an idea for a better solution?
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
