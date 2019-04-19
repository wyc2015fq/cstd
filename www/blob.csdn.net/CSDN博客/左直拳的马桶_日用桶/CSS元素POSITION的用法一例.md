# CSS元素POSITION的用法一例 - 左直拳的马桶_日用桶 - CSDN博客
2006年11月06日 14:24:00[左直拳](https://me.csdn.net/leftfist)阅读数：1745
CSS元素POSITION的用法一例
左直拳
页面上用到了Atlas的控件<atlas:UpdateProgress，以便在进行各种操作时显示一个等待图标，给用户解解闷，好让他们在漫长的等待结果的岁月中不那么烦躁，知道系统正在忙。
形如：
<atlas:UpdateProgressID="UpdateProgress1"runat="server">
<ProgressTemplate>
<imgsrc="../images/loading.gif"alt="请稍候..."/>
</ProgressTemplate>
</atlas:UpdateProgress>
可是这样写的话图标就歪在一边，不显眼。如果写上个绝对定位，比如
style="position:absolute;left:400px;top:300px;",可这仅仅适合800*600的屏幕，而用户的屏幕分辨率千差万别。
最后发觉可以用百分比，这样写：
<atlas:UpdateProgressID="UpdateProgress1"runat="server">
<ProgressTemplate>
<imgsrc="../images/loading.gif"alt="请稍候..."style="position:absolute;left:50%;top:50%;"/>
</ProgressTemplate>
</atlas:UpdateProgress>
这样不管什么情况下，图标都会居于页面之中。
