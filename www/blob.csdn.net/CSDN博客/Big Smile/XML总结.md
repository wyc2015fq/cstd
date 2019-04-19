# XML总结 - Big Smile - CSDN博客
2016年05月04日 14:20:47[王啸tr1912](https://me.csdn.net/tr1912)阅读数：653
        这两天学习了xml的知识，觉得比较琐碎，所以特来总结一番。
# 一、导图
![](https://img-blog.csdn.net/20160504113433048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        XML：可扩展标记语言，标准通用标记语言的子集，是一种用于标记电子文件使其具有结构性的标记语言。
        xml是一种比较自由的语言，由于是可扩展的，所以他的规范除了最基本的格式和符号限制之外没有其他的规范，一切的层次关系都是由人们自己定义的，从使用的角度上来讲，他是传递存储数据的一种标记格式，他和HTML最大的不同在于他的扩展性和无具体的定义性，html可以转化成好看的界面，主要是服务于 外观的，而xml独立的情况下是形成不了界面的，它专注于存储数据，所以这也是不同点之一。
# 二、语法&规范
xml的文档形成的时候需要在文件头写上xml的特性标记：
`<?xml version="1.0" encoding="utf-8"?>`它规定了使用的xml的版本和编码格式，为浏览器等程序的解析做好了准备。
        然后就是标签项，他所规定的必须由<xxx>和</xxx>来进行开始和结束，且最高一级的标签只允许有一个：
```
<?xml version="1.0" encoding="utf-8"?>
<book>
<书籍列表>
	<计算机书籍>
		<bookname isbn="xxx1234567"><< xml应用大全>></bookname>
		<price>39.0</price>
	</计算机书籍>
	<计算机书籍>
		<bookname>Java应用大全</bookname>
		<price>139.0</price>
	</计算机书籍>
	<计算机书籍>
		<bookname>Oracle应用大全</bookname>
		<price>59.0</price>
	</计算机书籍>
</书籍列表>
<!-- 注释 -->
<书籍列表2>
	<计算机书籍>
		<bookname> xml应用大全</bookname>
		<price>39.0</price>
	</计算机书籍>
	<计算机书籍>
		<bookname>Java应用大全</bookname>
		<price>139.0</price>
	</计算机书籍>
	<计算机书籍>
		<bookname> Oracle应用大全</bookname>
		<price>59.0</price>
	</计算机书籍>
</书籍列表2>
</book>
```
        如上述例子所示，标签<book>就是一个最高级的标签，它只允许出现一次，如果出现两次以上就需要在他的外面在加上一层标签，以此来符合规定。
        如果要显示的内容里面有“ <>/,''' ”等特殊的标记字符，那么要怎么办，方法有两种，一种是利用转义符，一种是用CDATA这个函数
CDATA显示转义：
`<![CDATE[ Oracle应用大全<><><><'''''''>>>>]]>``规范：<![CDATA[需要转换的内容]]>`
用转义符
![](https://img-blog.csdn.net/20160504115943542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而和CDATA相反的是PCDATA，这个函数是用来显示可以解析的字符的，如前面的标签，头命令等。一般默认都是用的PCDATA。
         由于xml的可扩展性，应运而生的自然就是他的人工定义的规范了，这个规范现在有两种，一种是DTD，一种是Schema，两者是发展的关系，Schema是从DTD发展而来的，所以又很多的相似之处，他们的功能主要就是定义一种规则，让引用了他们的xml符合这个规则存在。
![](https://img-blog.csdn.net/20160504141733984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 三、总结
        这四集视频讲到的东西基本上都是属于理解xml所必须的东西，知道了xml，知道了它的用途，知道了它的规范，以后的使用但愿不会太难，以上均属于个人观点，如有错误，欢迎指出。
