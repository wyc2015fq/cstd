# GitHub上README写法暨GFM语法解读 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月26日 17:05:07[boonya](https://me.csdn.net/boonya)阅读数：316标签：[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)
个人分类：[Git/GitHub/SVN/TFS](https://blog.csdn.net/boonya/article/category/6068199)








*原文地址：*
[*http://blog.csdn.net/guodongxiaren/article/details/23690801*](http://blog.csdn.net/guodongxiaren/article/details/23690801)

*本文在GitHub的演示地址：*

*[https://github.com/guodongxiaren/README](https://github.com/guodongxiaren/README)*

--------------------------------------------------------------------


自从开始玩GitHub以来，就越来越感觉它有爱。最近对它的***README.md***文件颇为感兴趣。便写下这贴，帮助更多的还不会编写README文件的同学们。

README文件后缀名为md。md是markdown的缩写，markdown是一种编辑博客的语言。用惯了可视化的博客编辑器（比如CSDN博客，囧），这种编程式的博客编辑方案着实让人眼前一亮。不过GitHub支持的语法在标准markdown语法的基础上做了修改，称为Github Flavored Markdown，简称GFM。可不是GFW呀![偷笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/titter.gif)。

————————————————————————————

我在GitHub上为本文建的一个仓库“README”，供大家查看代码即具体效果：[README](https://github.com/guodongxiaren/README)[](https://github.com/guodongxiaren/test)

本仓库README文件持续更新，新的知识点可能不会更新到博文中。首先强烈建议一条，不要用360或搜狗浏览器访问GitHub网站，你会发现此时网站上很多按钮都不可用。。建议使用火狐或谷歌浏览器访问GitHub

————————————————————————————

# 开始编辑README

打开你的GitHub的某个项目，我们可以直接在线编辑你的README文件，如果你已经有了这个文件，则在文件目录中直接点击它，如果你还没有这个文件那么点击项目名称右边的一个按钮，来添加新文件：

![](https://img-blog.csdn.net/20140415150456937)


然后你就打开了编辑页面，编辑区的左上角有填写文件名的区域，注意加上后缀**.md**

![](https://img-blog.csdn.net/20140415150803687)

如果你本来就有这个文件要重新编辑它的话，那么在点击了文件目录中的该文件后，在上方有工具栏，选择**Edit**

![](https://img-blog.csdn.net/20140415151503093)


然后滚动屏幕到下面，如果是新文件会有一个**Commit new file**的按钮，若没有内容是不能点击的。如果是旧文件重修编辑，那么这个按钮显示的是** Commit changes**

**![](https://img-blog.csdn.net/20140415151653515)**

*//顺便吐槽一句如果是360或搜狗浏览器的话，这个按钮是永远都无法点击的，囧。。*

先随便写的东西把这个新文件提交，然后再点击 Edit 重新打开它。你会发现编辑区左上角有了变化。

![](https://img-blog.csdn.net/20140415152607656)

默认选中Code，即我们的编辑模式。若点击 Preview（预览）就能实时显示当前的显示效果了。


好了，下面正式开始编辑这个文件




# 关于标题

规范的README文件开头都写上一个标题，这被称为大标题。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- 大标题  
- ====  

在文本下面加上 等于号 = ，那么上方的文本就变成了大标题。等于号的个数无限制，但一定要大于0个哦。。
比大标题低一级的是中标题，也就是显示出来比大标题小点。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- 中标题  
- -------  


在文本下面加上 下划线 - ，那么上方的文本就变成了中标题，同样的 下划线个数无限制。

除此之外，你也会发现大，中标题下面都有一条横线，没错这就是 = 和 - 的显示结果。

如果你只输入了等于号=，但其上方无文字，那么就只会显示一条直线。如果上方有了文字，但你又只想显示一条横线，而不想把上方的文字转义成大标题的话，那么你就要在等于号=和文字直接补一个空行。

***补空行：是很常用的用法，当你不想上下两个不同的布局方式交错到一起的时候，就要在两种布局之间补一个空行。***


如果你只输入了短横线（减号）-，其上方无文字，那么要显示直线，必须要写三个减号以上。不过与等于号的显示效果不同，它显示出来时虚线而不是实线。同减号作用相同的还有星号*和下划线_，同样的这两者符号也要写三个以上才能显示一条虚横线。


除此以外，关于标题还有等级表示法，分为六个等级，显示的文本大小依次减小。不同等级之间是以井号  #  的个数来标识的。一级标题有一个 #，二级标题有两个# ，以此类推。




**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #一级标题  
- ##二级标题  
- ###三级标题  
- ####四级标题  
- #####五级标题  
- ######六级标题  

注意井号#和标题名称要并排写作一行，显示效果如图：
![](https://img-blog.csdn.net/20140415190616281)


实际上，前文所述的大标题和中标题是分别和一级标题和二级标题对应的。即大标题大小和一级标题相同，中标题大小和二级标题相同。


# 显示文本




## 普通文本

直接输入的文字就是普通文本。需要注意的是要换行的时候不能直接通过回车来换行，需要使用<br>(或者<br/>)。也就是html里面的标签。***事实上，markdown支持一些html标签，你可以试试。***当然如果你完全使用html来写的话，就丧失意义了，毕竟markdown并非专门做前端的，然而仅实现一般效果的话，它会比html写起来要简洁得多得多啦。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- 这是一段普通的文本，  
- 直接回车不能换行，<br>  
- 要使用\<br>  

注意第三行的<br>前加了反斜杠 \ 。目的就是像其他语言那样实现转义，也就是 <  的转义。

效果如图：

![](https://img-blog.csdn.net/20140415193641437)

此外，要显示一个***超链接***的话，就直接输入这个链接的URL就好了。显示出来会自动变成可链接的形式的。


## 显示空格的小Tip

默认的文本行首空格都会被忽略的，但是如果你想用空格来排一下版的话怎么办呢，有个小技巧，那就是把你的输入法由半角改成全角就OK啦。

## 单行文本

使用两个Tab符实现单行文本。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- Hello,大家好，我是果冻虾仁。  

注意前面有两个Tab。在GitHub上单行文本显示效果如图：
![](https://img-blog.csdn.net/20140415195517312)


## 多行文本

多行文本和单行文本异曲同工，只要在每行行首加两个Tab



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- 欢迎到访  
- 很高兴见到您  
- 祝您，早上好，中午好，下午好，晚安  



![](https://img-blog.csdn.net/20140415195443515)

## 部分文字的高亮

如果你想使一段话中部分文字高亮显示，来起到突出强调的作用，那么可以把它用 `  ` 包围起来。注意这不是单引号，而是Tab上方，数字1左边的按键（注意使用英文输入法）。

Thank `You` . Please `Call` Me `Coder`

![](https://img-blog.csdn.net/20140415195420468)


## 文字超链接

给一段文字加入超链接的格式是这样的 **[ ***要显示的文字***]( ***链接的地址***)。比如：**



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- [我的博客](http://blog.csdn.net/guodongxiaren)  

显示效果：
![](https://img-blog.csdn.net/20140415194927687)

你还可以给他加上一个鼠标悬停显示的文本。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- [我的博客](http://blog.csdn.net/guodongxiaren "悬停显示")  

即在URL之后 用双引号括起来一个字符串。同样要注意这里是英文双引号。




# 插入符号

## 圆点符
- 这是一个圆点符
- 这也是一个圆点符

上面这段的圆点是CSDN博客编辑器里面的符号列表。写文章在列出条目时经常用到。在GitHub的markdown语法里也支持使用圆点符。编辑的时候使用的是星号 *



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- * 昵称：果冻虾仁  
- * 别名：隔壁老王  
- * 英文名：Jelly  

要注意的是星号* 后面要有一个空格。否则显示为普通星号。上文的显示效果如图：

![](https://img-blog.csdn.net/20140415211024531)


此外还有二级圆点和三级圆点。就是多加一个Tab。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- * 编程语言  
-     * 脚本语言  
-         * Python  

第二行一个Tab，第三行两个Tab。这样用来表示层级结构就更清晰了吧，看效果：
![](https://img-blog.csdn.net/20140415210959703)


**如果你觉得三级的结构还不够表达清楚的话，我们可以试着换一种形式，请看*字符包围***




## 缩进

缩进的含义是很容易理解的。。



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- >数据结构  
- >>树  
- >>>二叉树  
- >>>>平衡二叉树  
- >>>>>满二叉树  

显示效果：
![](https://img-blog.csdn.net/20140415210938296)

当然比这个更一般的用法是这样。常常能在书籍里面看到的效果，比如引用别人的文章。直接看效果。

![](https://img-blog.csdn.net/20140418163033265)





具体这个“缩进”的用法。大家自己摸索吧。


# 插入图片




## 来源于网络的图片




网上有很多README插入图片的教程了，经我自己多次测试呢，发现可以使用的最简单，最基本的语法是：



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- ![](http://www.baidu.com/img/bdlogo.gif)  


即 叹号! + 方括号[ ] + 括号( ) 其中叹号里是图片的URL。

如果不加叹号! ,就会变成普通文本baidu了。


在方括号里可以加入一些 标识性的信息，比如



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- ![baidu](http://www.baidu.com/img/bdlogo.gif)  


这个方括号里的baidu并不会对图像显示造成任何改动，如果你想达到***鼠标悬停***显示提示信息，那么可以仿照前面介绍的文本中的方法，就是这样：



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- ![baidu](http://www.baidu.com/img/bdlogo.gif "百度logo")  

在URL后面，加一个双引号包围的字符串，显示效果如图：

![](https://img-blog.csdn.net/20140415084033031)




## GitHub仓库里的图片

有时候我们想显示一个GitHub仓库(或者说项目)里的图片而不是一张其他来源网络图片，因为其他来源的URL很可能会失效。那么如何显示一个GitHub项目里的图片呢？

其实与上面的格式基本一致的，所不同的就是括号里的URL该怎么写。

**    https://github.com*****/**你的用户名** /** 你的项目名** / raw /** 分支名** /** 存放图片的文件夹** /** 该文件夹下的图片*

这样一目了然了吧。比如：



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- ![](https://github.com/guodongxiaren/ImageCache/raw/master/Logo/foryou.gif)  

我在GitHub上的用户名guodongxiaren；有一个项目ImageCache；raw表示原数据的意思吧，不用管它；主分支master；项目里有一个文件夹Logo；Logo文件夹下有一张图片foryou.gif




## 给图片加上超链接

如果你想使图片带有超链接的功能，即点击一个图片进入一个指定的网页。那么可以这样写：



**[plain]**[view plain](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/guodongxiaren/article/details/23690801?utm_source=tuicool&utm_medium=referral#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- [![baidu]](http://baidu.com)  
- [baidu]:http://www.baidu.com/img/bdlogo.gif "百度Logo"  

这两句和前面的写法差异较大，但是也极易模仿着写出，就不过多介绍了。只需注意上下文中的 baidu 是你自己起的标识的名称，可以随意，但是一定要保证上下两行的 标识 是一致的。
这样就能实现 点击图片进入网页的功能了。



# 插入代码片段

我们需要在代码的上一行和下一行用` `` 标记。``` 不是三个单引号，而是数字1左边，Tab键上面的键。要实现语法高亮那么只要在 ``` 之后加上你的编程语言即可（忽略大小写）。c++语言可以写成c++也可以是cpp。看代码：

![](https://img-blog.csdn.net/20140414175605265)


实际显示效果

![](https://img-blog.csdn.net/20140414175637890)


# [题外话]在GitHub上用Gist写日记吧

看了这么多markdown的语法，你一定不满足于仅仅写一个README文件了，开始跃跃欲试想实际用markdown语法来编写博客或文章了吧。的确，网上也有依托或者支持markdown语法的博客。但是呢，更方便的是，你可以借助GitHub本身就有的一个功能——Gist。

Gist是以文件为单位的，不是以项目为单位的。而且与普通的GitHub上建的仓库不同，Gist是private的哦。普通的项目默认都是public的，要想弄成private貌似还要交钱的样子。既然是private那么用来写写日记，是极好的。

GitHub网页的顶部有：

![](https://img-blog.csdn.net/20140415210829250)


点进去:

![](https://img-blog.csdn.net/20140415210736578)


这就是你可以编辑的私有文件，它不仅支持Text文本，还支持各种编程语言呢！当然也包括markdown。输入文件名：

![](https://img-blog.csdn.net/20140415210247125)


最后保存，选中 Create Secret Gist 就是私有的喽。

![](https://img-blog.csdn.net/20140415210703281)

我在GitHub上为本文建的一个项目，供大家查看代码即具体效果：[*https://github.com/guodongxiaren/README*](https://github.com/guodongxiaren/README)



