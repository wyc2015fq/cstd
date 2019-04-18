# Word公式编排技巧 - 硬曲奇小屋 - CSDN博客





2017年09月21日 16:01:22[HardCookies](https://me.csdn.net/james_616)阅读数：329








虽说会LaTeX能给我们写文章编排公式带来很大方便，但是在平时的工作中还是要用Word，毕竟不是所有人都能直接上来就用LaTeX的。我就简单介绍一下在Word中编排公式的小技巧，我这里用的是Word for Mac 2016，其他版本的也同样适用。 

一般用Word的插入公式工具可用直接插入，插入后公式会自动居中。但是我们常常需要对公式进行编号，编号靠右对齐，而公式居中。 

 1. 首先打开word的标尺，因为之后要确定字符位置。 

 2. 新建样式，输入样式名称。 
![这里写图片描述](https://img-blog.csdn.net/20170921152029727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 3. 设置样式属性，选择格式-制表位。这里要要设置两个制表位，一个是公式，一个是标号。从标尺中可以读出位置，一般总共34个字符。所以输入17，居中对齐，无前导符，点击+添加。再输入34，靠右对齐，无前导符，点击+添加。最后，点击确定。 

 4. 设置格式段落，选择格式-段落。 

缩进和间距中，行距为单倍行距，中文版式中，文本对齐方式设置为居中。 
![这里写图片描述](https://img-blog.csdn.net/20170921153036432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设置好样式就可以使用了，我们来看看。 

输入公式和标号后，选择我们刚刚新建的样式。 
![这里写图片描述](https://img-blog.csdn.net/20170921153454730?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然而并没有什么特别效果，别怕！试试在公式和标号前分别键入一个**tab**，那么神奇的事情发生了。 
![这里写图片描述](https://img-blog.csdn.net/20170921153718307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然而，有时候我们需要插入一个两行的公式，其中两个等号需要对齐，那么该怎么办呢？ 

1. 首先插入一个2x3的矩阵，在中间一列输入等号，两边输入内容。 
![这里写图片描述](https://img-blog.csdn.net/20170921154545597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2. 对这个矩阵进行设置，隐藏占位符，列对齐方式-左，矩阵间距-最小列间距-固定3磅。 
![这里写图片描述](https://img-blog.csdn.net/20170921155317769?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3. 然后再设置成我们刚刚的样式，就OK啦！ 
![这里写图片描述](https://img-blog.csdn.net/20170921155620653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们最后看看整体的效果 
![这里写图片描述](https://img-blog.csdn.net/20170921155721682?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamFtZXNfNjE2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



