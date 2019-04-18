# import package怎么用 - Orisun - 博客园







# [import package怎么用](https://www.cnblogs.com/zhangchaoyang/articles/1796530.html)





看这么一个文件结构：


```
1 package zcy.android
 2 {
 3     class TextView
 4     {
 5         //...
 6      }
 7     class Button
 8     {
 9         //...
10      }
11     
12     package widget
13     {
14         class View
15         {
16             //...
17         }
18         class TextView extends View
19         {
20             //...
21         }
22     }
23 }
```



上面出现的4个类都包含在包zcy.andrid中，但我们把3、7行的TextView类Button类称之为“直属”于包zcy.android，而14、18行的View和TextView类“直属”于zcy.android.widget包。

如果我们另外新建了一个project，代码开头没有import任何包，而在代码中突然出现了TextView，那就会报错，因为编译环境不认识TextView是个啥玩意儿。有人会问：我编程的时候什么包也没有引入，但是当我使用String类时为什么没有报错呢？这是因为任何Java程序都默认地引入了java.object.lang（这是java中所有类的祖先），而像String,Integer,Float这些类都"直属“于lang。

当你import zcy.android.*后，出现TextView编译环境就知道你用的是第3行的那个TextView类，而当出现View时还是不认识它，因为编译环境只会从zcy.android的直属类中去搜索View类是否存在。

当你import zcy.android.Button后还是不认识TextView。




同理，当你import zcy.android.widget.*后，出现TextView编译环境就知道你用的是第18行的那个TextView类。

当你同时import zcy.android.*;import zcy.android.widget.*;后，代码中出现TextView编译环境就困惑了，同样会报错。但是我们又必须同时引入这两个包，因为我们想使用Button和View这两个类。解决办法是在类名前加上包名作为前缀:android.TextView tv1=new android.TextView();widget.TextView tv2=new widget.TextView();当然你也可以写android.Button b=new android.Button();但对于Button和View没有必要加前缀。












