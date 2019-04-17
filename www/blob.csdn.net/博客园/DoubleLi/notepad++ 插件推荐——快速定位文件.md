# notepad++ 插件推荐——快速定位文件 - DoubleLi - 博客园






介绍Notepad++的文章很多，我这里就不赘述了。简言之，是一个轻便的文本编辑工具。结合丰富的插件，可以实现很多功能，可以成为一个轻量级的IDE，用来做脚本开发非常合适。

这里介绍几个插件可以用来进行文件定位。（所有插件都可以在Notepad++的菜单栏 插件->Plugin Manager->Show Plugin Manager里面找到）



**第一个是Explorer**

这个插件很多用Notepad++的人应该很熟悉了，可以通过展开文件夹找到特定文件。当然很难说是快速定位，左下角的Filter框并不能像xcode那样实现对所有文件的快速过滤，只能过滤当前文件夹这一层的内容。这个插件的一个重要作用是能够像浏览文件夹那样直接在Notepad++中操作，不用切换到其他窗口。右击还能找到标准的鼠标右键菜单，比较方便的是能找到tortoise svn菜单和winRAR的菜单。一般直接在notepad++里面做了修改就可以直接在notepad++里面提交svn了（如果机器上装了svn——非tortoiseSVN，并且Notepad++装了NppExec插件的话，也可以在NppExec的console窗口中直接用svn命令行操作）。

![Explorer](http://img.blog.csdn.net/20150701225127355?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![Explorer_standard_menu](http://img.blog.csdn.net/20150701225206584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**第二个定位文件的插件是File Switcher**

![FileSwitcher](http://img.blog.csdn.net/20150701225255889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

相比于前面的Explorer要逐层展开文件夹来找寻文件，这个插件并不需要用鼠标来操作。设定好快捷键后，快捷键打开File Switcher，然后通过关键字查找就能定位到已打开的文件了。注意，这里是已打开的文件。所以事先还是要通过手动的文件查找打开该文件，后面就不需要重新手动查找了。当然可以一直不关闭打开的文件，Notepad++关闭后重新打开的话上次打开的文件也都是打开的，只不过打开Notepad++时间会稍微长一点因为要加载所有要打开的文件。但是这样后面用File Switcher定位文件就很方便，只要查找的文件是已打开的，那就不需要记住它的路径逐层查找了。

另一个类似的功能可以在Notepad++的首选项中开启，如下图所示：

![preference_DocSwitcher](http://img.blog.csdn.net/20150701225331817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后会出现一个已打开文件列表的窗口，这里面也可以定位到已打开的文件，还可以对已打开文件进行后缀名排序，选择性的批量关闭文件。

![doc_switcher](http://img.blog.csdn.net/20150701225416823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

只不过用Doc Switcher定位文件不如File Switcher快速和方便。



**第三个要介绍的快速定位文件的插件是Open File In Solution**

要用这个插件，还要装另外两个插件Solution Hub和Solution Hub UI。这三个插件一起装反正也就是多点两下鼠标的事，很方便。

然后打开SolutionHubUI（没有定义快捷键的话点击Notepad++菜单栏插件->SolutionHubUI->SolutionHubUI Show）：

![solutionHubUI_new](http://img.blog.csdn.net/20150701225454230?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



执行如下步骤：直接查看第5步

1.点击New solution按钮

2.在Name里面填入solution的名称，这里我以cocos2d-x-3.1为例，就取名cocos2d_3.1

3.点击Path下面的文件夹图标，指定文件夹并勾选Recursive和Monitored。这里可以指定多个文件夹，只需要点击中间的 "Add new field"  图标即可。"Remove field" 可以删除文件夹。

4.点击Save solution...按钮

5.然后Solution Name下面就会出现刚才命名的cocos2d_3.1。这时候在Connections里面填入ofis（这是Open File In Solution的首字母，说明这个Solution是给Open File In Solution这个插件用的），然后点击Save connections...

![solutionHubUI_new1](http://img.blog.csdn.net/20150701225711824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

最后结果如下图所示：



![solutionHubUI_new_2](http://img.blog.csdn.net/20150701225746409?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后关闭SolutionHubUI的窗口。

6.接下来就可以用Open File In Solution快速定位文件了：Notepad++菜单栏 插件->Open File In Solution->OFIS Show（我指定了快捷键ctrl+shift+r——这是eclipse常用的快捷键打开Open File In Solution）

![Open ofis](http://img.blog.csdn.net/20150701225817735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

怎么样，还是很酷的吧：

![ofis](http://img.blog.csdn.net/20150701225839109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb19veG9fbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

相比于前面两种插件的定位文件的方式，Open File In Solution是要方便不少吧，不需要记住文件路径，而且只通过键盘操作就可以了。不过有必要提出的是，虽然这样看File Switcher这个插件似乎多余了，但我还是会用到File Switcher的，因为如果某个文件不在指定的Solution里面，但是又是打开过的，那么File Switcher还是比Open File In Solution有用的。











