
# [android] No resource found that matches the given name 'Theme.AppCompat.Light' - 杨秀璋的专栏 - CSDN博客

2014年12月06日 04:42:22[Eastmount](https://me.csdn.net/Eastmount)阅读数：11583所属专栏：[Android实例开发与学习](https://blog.csdn.net/column/details/eastmount-android.html)



在整合android过程中导入别人的项目出现下面的错误：
<style name="AppBaseTheme" parent="Theme.AppCompat.Light">
error: Error retrieving parent for item: No resource found that matches the given name 'Theme.AppCompat.Light'.
![](https://img-blog.csdn.net/20141206043024769)
通过stackoverflow查到其解决方法,如下：
1.File->Import (android-sdk\extras\android\support\v7). Choose "appcompat"
在"文件"中导入sdk路径extras\android\support\v7中的appcompat文件夹.如下图所示：
![](https://img-blog.csdn.net/20141206042456264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.Project->
 properties->Android.  In  the  section library "Add" and choose "appCompat"
![](https://img-blog.csdn.net/20141206042820016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时问题即可解决,能正确运行.
![](https://img-blog.csdn.net/20141206043934781)
问什么会出现这个问题呢？这是我整合“随时拍”项目其他人的代码,他的版本比我的低,新的eclipse默认模版主题UI需要使用比较高版本api,如果需要支持低版本,需要导入appCompact库来支持.希望文章能解决大家的bug.
(By:Eastmount 2014-12-3 晚上10点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))
参考资料：
[
stackoverflow答案](http://stackoverflow.com/questions/21059612/no-resource-found-that-matches-the-given-name-style-theme-appcompat-light)
[http://blog.csdn.net/huiguixian/article/details/41210895](http://blog.csdn.net/huiguixian/article/details/41210895)



