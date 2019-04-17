# Markdown指南 - 在思索中前行！ - CSDN博客





2015年04月15日 14:46:44[_Tham](https://me.csdn.net/txl16211)阅读数：1181








## Markdown 指南


前几天学习了markdown，然后一直在用，很喜欢这种`格式即内容`的写作形式。在[简书](http://jianshu.io/)写了几篇，对固定链接的格式很不满意，于是寻找能够支持markdown、固定链接显格式、支持自定义域名的博客平台。Github是个很好的选择，在折腾完Jekyll和Octopress几近筋疲力尽的时候，一束光芒照过来，我看到了`hexo`。

说远了…这篇文章是想介绍一下markdown这种写作形式。


本质上，类似于html，markdown也是一种标记型语言，ie，用某些字符（不像html用字符串）标示某种格式，只是标记格式不同罢了。

比如：`###Hello Word`，标示出helloword是三级标题，markdown进行渲染后就成了对应大小、加粗的格式。

介绍markdown的教程太多了，我不打算做无用的重复，给出我认为必读的几个供大家参考。

- [鲁塔弗：markdown 简明语法](http://lutaf.com/markdown-simple-usage.htm)
- [图灵社区：怎样使用Markdown](http://www.ituring.com.cn/article/23)
- [简书：献给写作者的 Markdown 新手指南](http://jianshu.io/p/q81RER)
- [官方文档(中文版)：Markdown 语法说明](http://wowubuntu.com/markdown/#p)
- [用Markdown来书写你的博客](http://upwith.me/?p=503)


说明一下，上述1-3是并列关系，不同人总结的教程，建议都看。4比较繁琐，可以暂时不看，以后需要时再查询即可。

还有，markdown就这么点东西，学会了就一劳永逸了，性价比很高！


下面给出几个编辑器，大家边学边练。
- [简书](http://jianshu.io/) 用微博登录，开始记笔记吧！
- [MaDe (Chrome插件)](https://chrome.google.com/webstore/detail/made/oknndfeeopgpibecfjljjfanledpbkog)
- [dillinger](http://dillinger.io/)
- [StackEdit](http://benweet.github.io/stackedit/) 由[TangPro](http://www.tangpro.com/)推荐。
- [Cmd - 在线 Markdown 编辑阅读器](https://www.zybuluo.com/mdeditor)


好了，开始你的markdown之旅吧！



## markdown 番外篇




开博后写过一篇[Introduction to markdown](http://zipperary.com/2013/05/22/introduction-to-markdown/)，给出了几个介绍markdown的教程。这几个教程或是简明易懂，或是全面规范，可以作为很好的入门教材或查阅手册。


在实际的使用当中，我发现有些用法不是这些教程里能够找到的，或者教程里没有进行应有的强调导致使用时容易出错。本文就对这样的问题进行汇总，并会不定期更新。
- 

删除线。


方法：在要划删除线的文本前后添加`~~`


示例：`~~看《冰与火之歌》~~`


解析为： ~~看冰与火之歌~~

- 

代码块中html代码显示不正常，`<、>`被解析为
```
>
 <
```



原因：markdown是一个语法规范，具体的解析可以使用不同的程序，而不同程序对规范的完成度也未必相同。hexo 所用的 marked 解析程序，无法对代码块中`<、>`做正确解析，需要人工矫正。


方法：标明语言类型html


示例：

![](http://zipperary.com/img/kk.png)


解析为：

![](http://zipperary.com/img/kkk.png)

- 

有序列表。


问题：有序列表每项中包含若干段落时，这几个列表项得不到正确解析。


方法：在每个列表项中各段落之前加一个空格。


示例：


```
1. foo

 foo
 
2. bar

 bar
```


解析为：
- 

foo


foo

- 

bar


bar


## 轻松搞定Markdown


之前写过[《Markdown指南》](http://localhost:4000/2013/05/22/introduction-to-markdown/)和[《markdown
 番外篇》](http://localhost:4000/2013/06/06/markdown-extras/)用来介绍Markdown，前者是入门资料索引，后者是几个稍微高级的用法，都只是介绍了片面，不太好。


使用Markdown写博客已经三个月了，对这个标记语言很是喜欢。现在流行的教程，要么就是太复杂太难以理解，要么就是太简单太肤浅，稍微高级点的用法没有提及。本文的初衷就是用最容易理解的方式，把Markdown入门到高级用法一一介绍给读者。看完之后，你不仅可以开始使用Markdown写东西，在日常使用中也随时可以打开这篇博客参考各种用法。

为什么学习Markdown？别问为什么，尽管学习就好了。这么简单的东西，一会就能学会，受益可能终生，太值了。

怎么学习Markdown？我认为最好的方法是通过示例，本文正式采用这样的方法。

在哪可以使用Markdown？ 你可以在[简书](http://jianshu.io/)、[Logdown](http://logdown.com/)这样的免费平台用Markdown写博客。如果你愿意折腾，也可以像我一样，使用hexo搭建自己的博客，并用Markdown书写博客，请参考拙作[《hexo系列教程》](http://zipperary.com/categories/hexo/)。另外，现在写论文也流行使用Markdown，不学你就out了！

怎样开始？打开[这个网址](http://ghosertblog.github.io/mdeditor/)，把下面学到的东西在这个里面练习。左侧是Markdown，右侧是实时的显示。

还要注意什么？一个效果，有时候可以通过不同的方式生成，比如无序列表可以用
```
-
 + *
```
。本文从实践出发，只列举最好用的一种。读者也只需要学习一种。


以下是Markdown代码：

```
#这里是一级标题
###这里是三级标题
######这里是六级标题，就到这里了，木有七级！



段落前面没有缩进，看到了吧。

　　要想使用缩进，在中文输入法中改成「全角」输入，再按两个空格

行尾一个回车换行，显示效果依然是是一行。
我说的对吧~

想要分成两行，需要在行尾输入两个空格。  
我说的对吧~

想要开始一个新的段落，乖乖的按两下回车吧

我说的对吧~




**两个星号包围的是粗体**

*一个星号包围的是斜体*

> 引用内容，多行的话，用两个空格结尾吧  
是吧~



`反引号包围的是行内代码`，这个按键在「Tab」上边

``嵌套的话，可以使用`两个双引号`嵌套在外面``

```python
#三个反引号包围的是代码块。
#会显示行号。
#也有高亮显示
#我上面加了python，你也可以不加。如果不能高亮，那么还是加上吧。。另外，如果你的代码块包围的是html，那你应该写html而不是python
import os
print os.sep
```
嵌套的话，可以使用六个反引号嵌套在外面。




1. 有序列表1，注意点之后的空格。
2. 有序列表2.
1. 我这里写成1，在显示时还是3，厉害不。

1. 有序列表1
 详细内容，注意前面有一个空格
2. 有序列表2
 详细内容

- 无序列表1
- 无序列表2
- 无序列表3
 - 小1，注意前面有一个空格
 - 小2



超级链接：[我的博客](http://zipperary.com)

插入图片：![好看不](http://ww2.sinaimg.cn/large/5e8cb366jw1e62o63tkv3j20dh078q5a.jpg)

另外，markdown完全兼容html，你可以用html插入这些东西，例如：

<iframe height=498 width=510 src="http://player.youku.com/embed/XMjI2MjU3MDMy" frameborder=0 allowfullscreen></iframe>


~~删除线~~


分割线用`---`，注意单独一样，上面必须是一个空行

---

更多高级的格式，请果断插入html标签
```


以下到结尾处是效果：

# 这里是一级标题

### 这里是三级标题

###### 这里是六级标题，就到这里了，木有七级！


段落前面没有缩进，看到了吧。


　　要想使用缩进，在中文输入法中改成「全角」输入，再按两个空格


行尾一个回车换行，显示效果依然是是一行。

我说的对吧~


想要分成两行，需要在行尾输入两个空格。

我说的对吧~


想要开始一个新的段落，乖乖的按两下回车吧


我说的对吧~

两个星号包围的是粗体

一个星号包围的是斜体

> 

引用内容，多行的话，用两个空格结尾吧

是吧~


`反引号包围的是行内代码`，这个按键在「Tab」上边

`嵌套的话，可以使用`两个双引号`嵌套在外面`

```
#三个反引号包围的是代码块。
#会显示行号。
#也有高亮显示
#我上面加了python，你也可以不加。如果不能高亮，那么还是加上吧。。另外，如果你的代码块包围的是html，那你应该写html而不是python
import os
print os.sep
```


嵌套的话，可以使用六个反引号嵌套在外面。
- 有序列表1，注意点之后的空格。
- 有序列表2。
- 我这里写成1，在显示时还是3，厉害不。
- 

有序列表1


详细内容，注意该行和上面一行，前面有一个空格

- 

有序列表2


详细内容

- 无序列表1
- 无序列表2
- 无序列表3
- 小1，注意前面有一个空格
- 小2



超级链接：[我的博客](http://zipperary.com/)


插入图片：

![好看不](http://ww2.sinaimg.cn/large/5e8cb366jw1e62o63tkv3j20dh078q5a.jpg)好看不


另外，markdown完全兼容html，你可以用html插入这些东西，例如：

~~删除线~~


分割线用`---`，注意单独一行，上面必须是一个空行


更多高级的格式，请果断插入html标签







