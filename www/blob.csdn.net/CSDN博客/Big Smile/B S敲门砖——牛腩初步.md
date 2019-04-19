# B/S敲门砖——牛腩初步 - Big Smile - CSDN博客
2016年03月27日 16:49:18[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1039
        这几天的学习一直都是在牛腩老师的声音中度过的，他的讲课很是有特点，有的时候很是激动，有的时候很是缓慢，有的时候充满了迷茫，在这样的气氛中，牛腩老师带领着我走进了B/S开发的大门。
# 一、后台的搭建
## 1、sqlHelper
提到sqlHelper我们肯定都已经不再陌生了，尤其是合作和重构都完成了同志们，但是对于sqlHelper的原理可谓是一知半解不明其理啊，牛腩老师就是从sqlHelper开始教我们一个B/S项目的开发的。
        首先就是数据库的访问对象，在Vs中使用的ADO.NET，其中的数据对象有SqlCommand，SqlConnection，SqlDataReader三个基本的对象，其中SqlCommand是用来设置数据库的查询方法，查询语句还有执行查询用的，sqlConnection是用来建立数据库连接的，而SqlDataReader是用来读取数据库中返回的数据用的他们在查询中都扮演了不可或缺的角色。其中SqlCommand的查询数据库的方法有两种，一种是ExecuteNonQuery，一种是ExecuteReader，前者返回的是执行受影响的行数，后者返回的是执行查询的结果也就是数据行。最后就是SqlParamater，这个是执行查询的参数集，用来存放要查询的参数的。
        明白了这些，sqlHelper的编写也就简单了，按参数分有带参数的和不带参数的，按结果分就是结果为数据行的和结果为受影响行的，所以sqlHelper中的方法也就是这2X2的排列组合了。
        由于每个查询都要用到SqlCommand，SqlConnection，所以把他们卸载最前面来使用，避免了语句的编写重复，也就是sqlHelper的存在的意义。
##     2、存储过程和触发器的使用
        在牛腩老师编写的系统中，查询新闻的模块用的全都是存储过程，使用过程中sqlHelper又进行了一次升级，就是写入了commandType的变量，来分别让sqlHelper为一般查询和存储过程更好的服务，在存储过程的调用之前还提到了触发器的使用，其实感觉上和存储过程差不多的样子但是需要的逻辑思维更加的多了一些，在删除类别的时候使用触发器，要把这个类别下的所有新闻都删除，这时候就要先删除新闻后删除类，否则会有约束的警告：
```
USE [newssystem]
GO
/****** Object:  Trigger [dbo].[trigCategoryDelete]    Script Date: 2016/3/27 15:54:15 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
-- =============================================
-- Author:		王啸
-- Create date: 2016-03-23
-- Description: 删除类别触发器
-- =============================================
ALTER TRIGGER [dbo].[trigCategoryDelete]
   ON  [dbo].[category]
   instead of DELETE
AS 
BEGIN
	declare @id int         
	select @id=id from deleted           <span style="font-family: Arial, Helvetica, sans-serif;">--获得要删除记录的ID</span>
	delete comment where newsId in (select newsId from news where caId=@id)   --删除新闻评论
	delete news where caId=@id           --删除新闻
	delete category where id=@id         --删除类别
END
```
之后的D层代码和B层代码基本上都是一样的，学习到的一点就是写B层的时候可以直接return 调的D层方法：
`return cdao.SelectAll();         //返回执行D层的结果`
# 二、前台的入门
##     1、div+css
        前台就是用的web窗体建立的界面，开始的时候不知道怎么建立项目的同学们要了解一下，牛腩老师用的是老版本的vs所以他的建立的网站项目都是用的.netframework3.5标准的，建立的项目才是那么少的文件，配置文件也就是web.config，如果是用的默认.netframework4.5的话，会出现一堆文件，并且没有和以前一致的配置文件，在后面的编码中也会出现问题，要切记：
![](https://img-blog.csdn.net/20160327160542764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        下面就是div和css的说明，div指的就是HTML中的项目标签，而css（Cascading Style Sheets）是一种样式列表，用来统一的设置样式来用的，也就是说Css是HTML网页的一种可编辑的配置文件，里面包含了各种窗体上元素的颜色，位置，边框等属性的信息，在高级应用中也用来编辑实时的网页动画。
![](https://img-blog.csdn.net/20160327161302664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在使用的时候要把css文件和网页链接起来，在vs中直接拖入便可（生成的这段代码）：
 <link href="CSS/Common.css" rel="stylesheet" />
然后再从HTML网页中的标记后面写入Css用到的标记码：
```
<div class="test">
         this is a example
</div>
```
然后就可以在css中使用这个标记码了：
```
.test{
      color:#ff0;
      border:1px solid #00f;
}
```
        这里标记码分为两种，一种就是上面的class=“”，另外一种就是id="" ,他们的区别就是在用class的时候可以有多个标记的class中的值是同一个标记，而用id来标记的时候一个id只能对应一种标记。
```
<div id="tsf" class="test">
         this is a example
</div>
```
##  对应的 css的代码要这样写：
```
#tsf{
     color:#ff0;
      border:1px solid #00f;
}
.test{
      color:#ff0;
      border:1px solid #00f;
}
```
显然id在css中要用#号来标记，而class要用"**.**"来标记.
##     2、界面的建立
建立界面就少不了要了解一下网页中的盒子模型了，在网页中，网页的布局都形如一个一个的盒子在其中，他们都有独立的盒子属性，设置这些属性就是css要干的活，盒子分为内边距和外边距，浮动的盒子和随流的盒子，其中的盒子的成型和包含内容是由HTML 来决定的，而盒子的边距，位置，是否浮动、边框等属性是由css来定的。
        在这个里面理解盒子模型和标准/非标准流是很重要的。
# 三、自己的思考
##     1、这个阶段的学习
这个阶段的学习就自己的感受而言其实并没有多少的收获，之后问了师傅，知道了这个阶段的学习是对于B/S的入门，也是对于B/S开发流程和内容的一个认识，第一个牛腩的项目就是要让我们了解都有什么东西，而具体的学习是安排到后面才深入的一个材料一个材料的讲解，所以学过就好，也不用了解的很深入，就像牛腩老师提到过的，他所说的知识点像是JQurey了，js的学习了，去看个入门就好，于是乎我就找到了一些资源，和大家分享一下。
##     2、学习总结和资料分享
这段时间的学习总结就是知识的广度的提升和B/S开发的初步认识,自己跟着牛腩老师也敲成功了几个网页了，但是还没有深入的了解，为此专门准备了入门资料来学习。
JS的入门书籍：[http://download.csdn.net/detail/llqkl0214/4898626](http://download.csdn.net/detail/llqkl0214/4898626)
JS的视频资料：链接: [http://pan.baidu.com/s/1kUX6LZh](http://pan.baidu.com/s/1kUX6LZh) 密码: h6pe
JQurey的学习资料：链接:[http://pan.baidu.com/s/1c1SvwXE](http://pan.baidu.com/s/1c1SvwXE) 密码: an4q
        我还发现了一个有意思的网站：[http://codepen.io/](http://codepen.io/)        这个网站可以在线编写网页的代码，html、css、js三种代码的同时编写然后可以即时的在下面显示代码的编写界面，还可以查看别人分享的网页动画的源码，是一个很不错的学习实践的网站，有兴趣的同学可以尝试一下，初次进入直接点NewPen就可以编写了，注册了之后才可以看到别人编写的代码。
