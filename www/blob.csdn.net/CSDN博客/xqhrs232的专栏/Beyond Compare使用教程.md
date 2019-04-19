# Beyond Compare使用教程 - xqhrs232的专栏 - CSDN博客
2014年08月14日 23:24:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1128
个人分类：[SI/Notepad++/EditPlus](https://blog.csdn.net/xqhrs232/article/category/906927)
原文地址::[http://blog.csdn.net/dclchj/article/details/6789449](http://blog.csdn.net/dclchj/article/details/6789449)
相关文章
1、Beyond Compare软件使用教程----[http://jingyan.baidu.com/article/066074d68f847ec3c31cb05a.html](http://jingyan.baidu.com/article/066074d68f847ec3c31cb05a.html)
2、Beyond Compare使用说明----[http://wenku.baidu.com/link?url=jfttfo_hSMPJajD82diBGnMWp5opfVZnvUEAl0x-QfnQXzWl0mAUz_iMH8bmrq9B_Z_1vT1NUD8RNlTpgjD_6nCTxc6Wh8mmYlUc9ug04qS](http://wenku.baidu.com/link?url=jfttfo_hSMPJajD82diBGnMWp5opfVZnvUEAl0x-QfnQXzWl0mAUz_iMH8bmrq9B_Z_1vT1NUD8RNlTpgjD_6nCTxc6Wh8mmYlUc9ug04qS)
文章转自：[http://www.heibai.net/book/html/shiyonggongju/qitaruanjian/2009/0617/583.html](http://www.heibai.net/book/html/shiyonggongju/qitaruanjian/2009/0617/583.html)
　　今天和大家介绍的是一款很好用的工具——Beyond Compare！这是一款用于文件及文件夹比较软件，不仅可以快速比较出两个文件夹的不同之处，还可以详细的比较文件之间的内容差异。　　
　　安装好Beyond Compare后，我们运行这个软件，会弹出引导窗口，让我们选择要进行对比的文件。BC支持在线对FTP的文件进行对比和更新，这使得我们更新网站程序文件非常方便。我们选择左侧为SF1.1.0.1的更新包的website目录，右侧选择为网站空间里的SF程序目录。
![](http://hi.csdn.net/attachment/201109/19/0_1316402974D6Oz.gif)![](http://hi.csdn.net/attachment/201109/19/0_1316403039dc8a.gif)
　　点击确定后，程序进入主界面。我们还需要设置比较标准：使用二进制比较。二进制比较是为了比较文件的实际内容，如果使用普通比较的话，如果文件时间不同，内容相同，也会被视为差异文件。
![](http://hi.csdn.net/attachment/201109/19/0_13164030679ukq.gif)
　　完成后软件便开始文件对比。几秒钟后比较完成，左右两侧列出文件夹的所有文件，有不同的文件则图标显示红色。
![](http://hi.csdn.net/attachment/201109/19/0_1316403116ioO9.gif)
　　这时候，界面里显示出的文件列表太多了，不便于我们查看要更新的部分，我们可以通过点击下图所示的这个按钮，只显示左边较新的有差异的或孤立的文件，也就是更新包里的新增文件与更新修改过的文件。
![](http://hi.csdn.net/attachment/201109/19/0_1316403145p96G.gif)
　　接着就是进行网站更新操作了。对于Admin、Controls、Install、Bin等文件夹，我们可执行复制命令，将这几个文件夹里的文件直接覆盖ftp里的相应文件。执行办法是，在左侧的文件夹点右键，弹出菜单中选择复制。复制完成后，左侧的红色文件夹图标变为灰色，表示已更新。
![](http://hi.csdn.net/attachment/201109/19/0_131640317850fW.gif)
　　对于config、template文件夹，我们需要逐个文件进行对比覆盖。操作方法为（以config文件夹为例）：
　　首先双击文件夹展开文件。
　　1、对蓝色的左侧的孤立文件AjaxLabel.config，使用右键复制到ftp中去。
![](http://hi.csdn.net/attachment/201109/19/0_1316403205wbA0.gif)
　　2、对红色的差异文件QueryStrings.config，双击打开文件查看器，进行逐行对比覆盖或编辑。点击“红色的不等号”的按钮可以只显示有差异的代码行。如果我们曾经编辑过ftp里的QueryStrings.config文件，并且想保留我们增加或修改的代码，那么我们可不对该代码进行覆盖修改。
![](http://hi.csdn.net/attachment/201109/19/0_1316403248Uqra.gif)
![](http://hi.csdn.net/attachment/201109/19/0_1316403255YTy4.gif)
　　Ok，到此为止，这款强大的工具的基本使用就介绍完毕了，大家所需要做的就是有选择地反复执行上述操作步骤，就可以很快速地完成对自己的网站的更新升级啦！
