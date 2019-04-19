# Qt QTableView另类打印解决方案 - baidu_33570760的博客 - CSDN博客
2016年03月28日 11:14:50[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1183
文章转载自  http://blog.sina.com.cn/s/blog_728505cb01017k8n.html  
Qt QTableView另类打印解决方案
上回书说道Qt的model/view，我就做了个demo用于显示数据库中的内容。没想到tableview的打印竟然成了问题。我困惑了，难道Qt不应该提供一个print函数给tableview吗？这是最最常用的功能啊。
Google了半天，也没什么有用的结果。看到Qt labs有一篇blog，叫“All You Need is a little Polish”，里面给出了最新的spreadsheet demo中的tableview print代码。还挺高兴，原来已经可以解决了。后来试了一下，发现根本不是那么回事，这个例子只能打印tableview当前显示的区域，对于分页、页眉、页脚、等等都不支持。原来写这个demo的并不是真正的开发人员，只是support
 team的。
作为10年的老MFCer，我有着厚脸皮的DIY精神（没办法，被逼的太多次了，每次只能自己来）,[点胶针头](http://www.hunheguan.com/)。相信以我在MFC中写Doc/View
 printing的经验，搞定Qt的打印还是没问题的。但是在跳进代码海之前，我花了几个小时仔细研究了一下QTableView的代码，发现我可以选下面几个实现方案。
1. 鸵鸟方案
学过操作系统的Tx都知道这意味着什么样的方案。上面提的那篇Blog里的方法可以算一种喽。如果你打印的table每次都是在1页内的话，可以用这个方案。但是作为10年英明神武的老MFCer，实在没脸这样做。
2. model方案
这个方案是从model中取得所有table的数据，然后使用2个for循环将这些数据以行和列打印出来。在qt-app网站上有人给出了这个解决方案（http://qt-apps.org/content/show.php/TableView+Printer?content=76616）。我看了代码，作者在打印每页时从model中取数据，然后在内存中生成一个graphicview，然后用graphicview打印。也实现分页等功能。但打印的效果不好,[混合管](http://www.hunheguan.com/)，增加的边框感觉也很别扭。
还有人建议将model中的数据导入至QTextDocument中，然后利用QTextDocument稍微强大一点打印功能打印。
总之，所有这些方案实现起来比较麻烦，而且只从model中取数据，忽略了view(delegate)中显示相关的因素（如文字render的格式，是否被选中等等）。还有一点就是分页总是成问题。即使纵向分页对了，横向的分页还是没看到相应的实现。（大家都注意到在Excel中一页横向打不下时会将未打印的内容打印至下一页吧）
3. delegate方案
在QTableView代码中，真正render每个item时，还是使用delegate的。所以有人建议使用delegate在2个for循环中将这些数据以行和列打印出来。这个貌似是最好的解决方案了。如果我是Qt开发人员的话，我会首选这个方案的。
4. 我的方案
可惜我不是Qt的开发人员（不知Nokia这种企业氛围如何）。个人感觉Qt的风格是沉稳，外加一丝灵动。所以我的方案可能不是Qt开发人员喜欢的。即使我个人也觉得这个方案难登大雅之堂。只是在时间和精力都有限的情况下，借助了Qt的一点Q，想出来的一个非常tricky的办法。
其实我的方案就是：鸵鸟方案的升级版。当当当。。。
我的灵感来源于上面Bolg中鸵鸟方案的4句代码：
setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
resize(printer->width(), printer->height());
render(printer);
前面2具隐藏scrollbars,[AB胶枪](http://www.hunheguan.com/)，第3句，让view与print的纸张大小相同，最关键的第4句，直接将view
 render到printer中。
作为一个10年的老MFCer，我坚信Qt在代码复用性方面应该更胜一筹。既然view可以直接render到printer中（感谢Qt中painter和printer的设计），那么为什么还需要去写delegate的paint呢？**所以，我的方案出发点就是从怎么复用render函数去解决print问题。**
方案成功了吗？心急的Tx请先看下面的图片。
打印预览、横向分页、纵向分页、背景色、选中标记、页眉、页脚。。。。。。该有的都有了。从单页到上百页我都测试过。那么我的鸵鸟升级方案是怎么实现的呢？
其实非常简单,[点胶针筒](http://www.hunheguan.com/)，还是基于view的render函数，既然view当前显示的内容可以直接render到printer中，那么我就模拟给它翻页，然后render每页不就行了。最初的办法是使用QScrollBar的setValue对横向和纵向进行翻页。就是打完第1页之后，翻到第2页，再打印，以此类推。但最后一页很难处理，如果最后一页的内容只有几行的话，是没法继续向下翻得，造成的结果就是倒数第2页很多打印过的行会再次被打印出来。难道没办法了。
一筹莫展之际，只能继续翻Qt的文档，当看到tableview中setRowHidden和setColumnHidden这2个函数时，我一刹那被Carmack大仙（我的偶像）灵魂附体。这不就解决了吗？
什么？你还不明白？
打印完第1页时，将这页打印过的row隐藏，然后打印下一页。打印完纵向分页后，将横向打印过的colum隐藏。再次打印纵向分页（从而实现横向分页）。
剩下的就是数学了。。。
大部分问题都解决了。我写了个QTableView的派生类MyTableView，里面一些打印相关的代码用到了David Johnson david@usermode.org的textprinter中某些代码，感谢开源社区。我的MyTableView类可以实现下面的功能：
1. WYSIWYG打印/打印预览，这意味着每个item的背景颜色、文字颜色、显示属性、选择状态、边框、表头。。。。。。所有你看到tableview显示的，都可以原封不动直接打印出来；
2. 纵向分页，当在一页末尾某行没打印全时，会自动在下一页再次打印；
3. 横向分页，当在一页右侧某列没打印全时，会自动在下一次横向分页时再次打印；
4. 打印页眉
5. 打印页脚
6. 打印边距
7. 打印成PDF文件
