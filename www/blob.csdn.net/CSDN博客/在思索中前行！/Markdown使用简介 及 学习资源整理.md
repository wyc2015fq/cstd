# Markdown使用简介 及 学习资源整理 - 在思索中前行！ - CSDN博客





2015年04月16日 09:54:31[_Tham](https://me.csdn.net/txl16211)阅读数：1168








[Markdown资源整理](http://www.jianshu.com/p/5651e24bc2e0)




## 官网
- [http://daringfireball.net/projects/markdown/](http://daringfireball.net/projects/markdown/)
- [http://jgm.github.io/stmd/spec.html](http://jgm.github.io/stmd/spec.html)
- [http://commonmark.org/](http://commonmark.org/)

## 教程
- [Markdown 语法说明 (简体中文版)](http://wowubuntu.com/markdown/)
- [GitHub Flavored Markdown](https://help.github.com/articles/github-flavored-markdown/)
- [献给写作者的 Markdown 新手指南](http://www.jianshu.com/p/q81RER)

## 在线
- [Cmd Markdown作业部落](https://www.zybuluo.com)
- [MaHua](http://mahua.jser.me/)
- [Dillinger](http://dillinger.io/)
- [SlickEdit](https://stackedit.io/)
- [Lepture's Editor](http://lab.lepture.com/editor/)
- [Online Markdown Editor](http://www.ctrlshift.net/project/markdowneditor/)
- [Markdown Live Editor](http://jrmoran.com/playground/markdown-live-editor/)
- [Sribble](http://www.tryscribble.com/)
- [马克飞象](http://maxiang.info/)
- [Markable](http://markable.in/)
- [HTMLtoMarkdown](http://domchristie.github.io/to-markdown/)
- [MarkdowntoPDF](http://www.markdowntopdf.com/)
- [简书](http://jianshu.io/)
- [小书匠](http://markdown.xiaoshujiang.com/)

## 软件
- **Windows**- [MarkdownPad](http://www.markdownpad.com/)
- [MdCharm](http://www.mdcharm.com/)
- [Texts](http://www.texts.io/)
- [Haroopad](http://pad.haroopress.com/user.html)
- [CuteMarkEd](http://cloose.github.io/CuteMarkEd/)
- [Miu](https://github.com/0x142857/Miu)

- **Linux**- [Haroopad](http://pad.haroopress.com/user.html)
- [ReText](http://sourceforge.net/p/retext/home/ReText/)
- [UberWrite](http://uberwriter.wolfvollprecht.de/)
- [CuteMarkEd](http://cloose.github.io/CuteMarkEd/)

- **Mac**- [Mou](http://mouapp.com/)
- [Kobito](http://kobito.qiita.com/)
- [nvALT](http://brettterpstra.com/projects/nvalt/)
- [Haroopad](http://pad.haroopress.com/user.html)
- [iA Writer](https://ia.net/writer)
- [Byword](http://bywordapp.com/)
- [Ulysses III](http://www.ulyssesapp.com/)


## Chrome 插件
- [Markdown Here](https://chrome.google.com/webstore/detail/minimalist-markdown-edito/pghodfjepegmciihfhdipmimghiakcjf)
- [马克飞象](https://chrome.google.com/webstore/detail/minimalist-markdown-edito/pghodfjepegmciihfhdipmimghiakcjf)
- [Minimalist Markdown Editor](https://chrome.google.com/webstore/detail/minimalist-markdown-edito/pghodfjepegmciihfhdipmimghiakcjf)

## Sublime 插件
- [Markdown Editting](https://sublime.wbond.net/packages/MarkdownEditingdfjepegmciihfhdipmimghiakcjf)
- [Markdown Preview](https://sublime.wbond.net/packages/Markdown%20Preview)
- [OmniMarkupPreviewer](https://sublime.wbond.net/packages/OmniMarkupPreviewer)






# **Markdown公式及表格的生成**


如何在 markdown 编辑器中生成公式或表格呢？下面是我个人常使用的方法。

## 1. 公式的生成


在 word 中编辑复杂公式时，常用 MathType 就可以达到预期效果。那么在 markdown 中想要编辑公式时，有没有类似 word 中的 MathType 工具呢？

LaTex 公式编辑器 就可以达到这个效果。通常我使用 在线 LaTex 公式编辑器来编辑公式。

[在线 LaTex 公式编辑器](http://latex.codecogs.com/eqneditor/editor.php).

在 LaTex 中编辑好公式，将编辑框中的内容复制到 markdown 中，并将其放在“$”中。示例如下：


1） 首先在 LaTex 中变价公式  ：

![这里写图片描述](https://img-blog.csdn.net/20170404164733188?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU3Vuc2hpbmVfbG92ZTUyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2） 复制编辑框中的内容（即红色标注部分），将其粘贴到 markdown 中，在这部分内容的左右两侧加上 “$” 符号，就可以显示你的公式了。

## 2. 表格的生成


使用 Tables Generator 生成表格并编辑内容。 [Tables Generator](http://www.tablesgenerator.com/markdown_tables)


可以选择 LaTex Tables、HTML Tables、Text Tables、Markdown Tables等，这里选择 Markdown Tables。


1）在 Table 下选择 “set size”，设置自己要生成的表格大小，这里选择4行5列的表格。

![这里写图片描述](https://img-blog.csdn.net/20170404170056216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU3Vuc2hpbmVfbG92ZTUyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2）在表格中编辑内容，编辑完后，点击“Generate”按钮，生成表格内容对应的结果如下图。

![这里写图片描述](https://img-blog.csdn.net/20170404170858495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvU3Vuc2hpbmVfbG92ZTUyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


3） 点击右边的“Copy to Clipboard”按钮，复制内容到 markdown 编辑器中即可显示表格内容如下。
|学号|姓名|性别|年龄|电话|
|----|----|----|----|----|
|001|小红|女|20|83145678|
|002|小明|男|21|83145679|
|003|小白|女|22|83145623|


# [**Markdown简介**](http://blog.csdn.net/youmingyu/article/details/53101627)



摘要：Markdown是一种轻量级的「标记语言」，它的优点很多，目前也被越来越多的写作爱好者，撰稿者广泛使用。看到这里请不要被「标记」、「语言」所迷惑，Markdown 的语法十分简单。常用的标记符号也不超过十个，这种相对于更为复杂的HTML 标记语言来说，Markdown 可谓是十分轻量的，学习成本也不需要太多，且一旦熟悉这种语法规则，会有一劳永逸的效果。

主要参考文章：
- [认识与入门 Markdown](http://sspai.com/25137)
- [CSDN Markdown在线编辑](http://blog.csdn.net/column/details/csdnmarkdown.html)

# 认识 Markdown

在刚才的导语里提到，Markdown 是一种用来写作的轻量级「标记语言」，它用简洁的语法代替排版，而不像一般我们用的字处理软件 Word 或 Pages 有大量的排版、字体设置。它使我们专心于码字，用「标记」语法，来代替常见的排版格式。例如此文从内容到格式，甚至插图，键盘就可以通通搞定了。目前来看，支持 Markdown 语法的编辑器有很多，包括很多网站（例如[简书](http://www.jianshu.com/)）也支持了
 Markdown 的文字录入。Markdown 从写作到完成，导出格式随心所欲，你可以导出 HTML 格式的文件用来网站发布，也可以十分方便的导出 PDF 格式，这种格式写出的简历更能得到 HR 的好感。甚至可以利用[CloudApp](https://www.getcloudapp.com/)这种云服务工具直接上传至网页用来分享你的文章，全球最大的轻博客平台Tumblr，也支持使用
 Mou 这类 Markdown 工具进行编辑并直接上传。

## Markdown 官方文档

下面可以看到官方的 Markdown 语法规则文档，当然，后文我也会用自己的方式，阐述这些语法在实际使用中的用法：
- [创始人 John Gruber 的 Markdown语法说明](http://daringfireball.net/projects/markdown/syntax)
- [Markdown 中文版语法说明](http://wowubuntu.com/markdown/#list)

## 使用 Markdown 的优点
- 专注你的文字内容而不是排版样式。
- 轻松的导出 HTML、PDF 和本身的 .md 文件。
- 纯文本内容，兼容所有的文本编辑器与字处理软件。
- 可读，直观。适合所有人的写作语言。

# 我该用什么工具？

## Mac 平台

在Mac OS X 上，我强烈建议你用[Mou](http://25.io/mou/)这款免费且十分好用的 Markdown 编辑器，它支持实时预览，既左边是你编辑 Markdown 语言，右边会实时的生成预览效果，笔者文章就是 Mou 这款应用写出来的。 
![Mou](https://img-blog.csdn.net/20161109202400925)

其次还有很多同类选择。如果你是个编辑作者，我强烈建议你购买[Ulysses Ⅲ](https://ulyssesapp.com/)，这款应用入围了苹果去年Mac App Store的The Best of 2013，相比Mou它支持更多的写作格式、多文档的支持。Mou、iA Writer这些应用都是基于单文档的管理方式，而Ulysses Ⅲ支持Folder、Filter的管理，一个Folder里面可以创建多个Sheet，Sheet之间还可以进行Combine处理。 
![Ulysses Ⅲ](https://img-blog.csdn.net/20161109202606302)

## Windows、iOS、Web 平台
- 笔者并未使用过 Windows 下的 Markdown 工具，但经朋友介绍，有两款还算不错，一款叫[MarkdownPad](http://www.markdownpad.com/)，另一款叫[MarkPad](http://code52.org/DownmarkerWPF/)。
- iOS 端已有相当多的 app 支持 Markdown 语法编辑，例如 Drafts、Day One、iA Writer 等。
- Web 端上，我强烈推荐简书这款产品，上面有无数热爱文字的人在不停的创造、分享。在 Web 端使用 Markdown 没有比简书更舒服的地方了，它同样支持左右两栏的实时预览，字体优雅、简洁。 
![这里写图片描述](https://img-blog.csdn.net/20161109203106325)
- 同样是Web端，[Draftin](https://draftin.com/)这款在线MD编辑器也近乎完美。

# Markdown语法的简要规则

## 标题

标题是每篇文章都需要也是最常用的格式，在 Markdown 中，如果一段文字被定义为标题，只要在这段文字前加 # 号即可。 
![这里写图片描述](https://img-blog.csdn.net/20161109203858288)

以此类推，总共六级标题，建议在井号后加一个空格，这是最标准的 Markdown 语法。

## 字号

![这里写图片描述](https://img-blog.csdn.net/20161109205941363)
效果： 
这是放大一个字号
这是缩小一个字号

## 列表

熟悉 HTML 的同学肯定知道有序列表与无序列表的区别，在 Markdown 下，列表的显示只需要在文字前加上 - 或 * 即可变为无序列表，有序列表则直接在文字前加 1. 2. 3. 符号要和文字之间加上一个字符的空格。 
![这里写图片描述](https://img-blog.csdn.net/20161109204003056)

## 引用

如果你需要引用一小段别处的句子，那么就要用引用的格式。 
![这里写图片描述](https://img-blog.csdn.net/20161109204052416)

只需要在文本前加入 > 这种尖括号（大于号）即可

## 图片与链接

插入链接与插入图片的语法很像，区别在一个 ! 号 

插入图片的地址需要图床，这里推荐[CloudApp](https://www.getcloudapp.com/)的服务，生成URL地址即可。 
![这里写图片描述](https://img-blog.csdn.net/20161109204209496)

## 粗体与斜体

Markdown 的粗体和斜体也非常简单，用两个 * 包含一段文本就是粗体的语法，用一个 * 包含一段文本就是斜体的语法。 
![这里写图片描述](https://img-blog.csdn.net/20161109204354733)

## 表格

表格是我觉得 Markdown 比较累人的地方，例子如下：

```
|Tables         |      Are      |   Cool|
|:------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5

这种语法生成的表格如下：
|Tables|Are|Cool|
|----|----|----|
|col 3 is|right-aligned|$1600|
|col 2 is|centered|$12|
|zebra stripes|are neat|$1|

从上面的例子，我们可以看出来，Markdown生成的表格是使用冒号来选择左 :—、右 —: 或者居中 :—: 对齐的。其中代码中第二行中的短线的数量可以根据美观来定。

## 代码框

如果你是个程序猿，需要在文章里优雅的引用代码框，在 Markdown 下实现也非常简单，只需要用多个 ` 把中间的代码包裹起来，后面可以注明语言类型，使用 tab 键即可缩进。。

例如： 
![这里写图片描述](https://img-blog.csdn.net/20161110101757786)

效果：

```cpp
include <iostream>
using namespace std;
int main(){
  cout << "hello markdown" << endl;
  return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6

## 分割线

分割线的语法只需要另起一行，连续输入三个星号 * 即可(CSDN上很不明显)。

## 公式

Markdown支持LaTeX写公式，LaTeX 公式有两种，一种是用在正文中的，一种是单独显示的。正文中的公式用 `$...$`来定义，单独显示的用 `$$...$$`来定义，其中
 … 表示的是LaTeX 的公式命令。 

例如：

```matlab
我们定义$f(x) = \sum_{i=0}^{N}\int_{a}^{b} g(t,i) \text{ d}t$. (行内公式)
或者定义$f(x)$如下（行间公式）: 
$$f(x) = \sum_{i=0}^{N}\int_{a}^{b} g(t,i) \text{ d}t{6}\tag{1}$$
```
- 1
- 2
- 3
- 1
- 2
- 3

得到的结果是：


我们定义0.
 (行内公式) 

或者定义如下（行间公式）: 




LaTeX的具体用法本文不再赘述，百度上很多，可以参考下面(**多个 ∑ 连在一块的时候 ，\sum_{}^{} 要进行转义 \sum\_{}^{} **)。   

- [使用LaTeX基本数学公式](http://blog.csdn.net/bendanban/article/details/44196101)
- [使用LaTeX写矩阵](http://blog.csdn.net/bendanban/article/details/44221279)

## 分段

「Markdown 语言」不负责实现段首缩进，事实上大多web语言都没有首行缩进，我们在区分段落时一般使用段间距，如果没有，可以强行空一行，在Markdown中推荐使用空一行这种方法。

如果必需输入空格，有以下两种方法可以使用。
- 使用` `，对于中文不是很不推荐这种方法。
- 全角空格，切换到全角模式下（一般的中文输入法都是按 shift + space）输入两个空格，宽度是整整两个汉字，整齐干净。

# 小结

到这里，Markdown 的基本语法在日常的使用中基本就没什么大问题了，只要多加练习，配合好用的工具，写起东西来肯定会行云流水。更多的语法规则，其实Mou的 Help文档例子很好，当你第一次使用Mou时，就会显示该文档，其次，你也可在撰写过程中，使用CMD+R快捷键来快速打开文档，以随时查阅和学习语法。 
![这里写图片描述](https://img-blog.csdn.net/20161109204641071)

# 与 Markdown 相关的一些推荐

## 可配套使用的工具
- [Droplr](http://droplr.com/)
- [Cloudapp](https://www.getcloudapp.com/)
- [ezShare for Mac](https://itunes.apple.com/cn/app/yi-xiang/id672522335?mt=12&ign-mpt=uo=4)
- [围脖图床修复计划](http://weibotuchuang.sinaapp.com/)
- [马克飞象](https://maxiang.io/)，专为印象笔记打造的 Markdown 编辑器，非常推荐。

## 相关文章阅读
- [为什么作家应该用 Markdown 保存自己的文稿](http://apple4us.com/2012/02/why-writers-should-use-markdown/)
- [Markdown 写作浅谈](http://www.yangzhiping.com/tech/r-markdown-knitr.html)
- [Markdown 工具补完](http://www.appinn.com/markdown-tools/)
- [Drafts + Scriptogr.am + Dropbox 打造移动端 Markdown 风格博客](http://www.jianshu.com/p/63HYZ6)
- [图灵社区 - 怎样使用 Markdown](http://www.ituring.com.cn/article/23)
- [为什么我们要学习 Markdown 的三个理由](https://news.cnblogs.com/n/139649/)
- [Markdown 语法写作入门指南 by ibuick](http://ibuick.me/?p=4093)

# PS：这篇文章的Markdown代码

```
<big>**摘要**：Markdown是一种轻量级的「标记语言」，它的优点很多，目前也被越来越多的写作爱好者，撰稿者广泛使用。看到这里请不要被「标记」、「语言」所迷惑，Markdown 的语法十分简单。常用的标记符号也不超过十个，这种相对于更为复杂的HTML 标记语言来说，Markdown 可谓是十分轻量的，学习成本也不需要太多，且一旦熟悉这种语法规则，会有一劳永逸的效果。

<big>**主要参考文章**：

- <big>[认识与入门 Markdown](http://sspai.com/25137)
- <big>[CSDN Markdown在线编辑](http://blog.csdn.net/column/details/csdnmarkdown.html)

#**认识 Markdown**
<big>在刚才的导语里提到，Markdown 是一种用来写作的轻量级「标记语言」，它用简洁的语法代替排版，而不像一般我们用的字处理软件 Word 或 Pages 有大量的排版、字体设置。它使我们专心于码字，用「标记」语法，来代替常见的排版格式。例如此文从内容到格式，甚至插图，键盘就可以通通搞定了。目前来看，支持 Markdown 语法的编辑器有很多，包括很多网站（例如[简书](http://www.jianshu.com/)）也支持了 Markdown 的文字录入。Markdown 从写作到完成，导出格式随心所欲，你可以导出 HTML 格式的文件用来网站发布，也可以十分方便的导出 PDF 格式，这种格式写出的简历更能得到 HR 的好感。甚至可以利用[CloudApp](https://www.getcloudapp.com/)这种云服务工具直接上传至网页用来分享你的文章，全球最大的轻博客平台Tumblr，也支持使用 Mou 这类 Markdown 工具进行编辑并直接上传。
##**Markdown 官方文档**
<big>下面可以看到官方的 Markdown 语法规则文档，当然，后文我也会用自己的方式，阐述这些语法在实际使用中的用法：

- <big>[创始人 John Gruber 的 Markdown语法说明](http://daringfireball.net/projects/markdown/syntax)
- <big>[Markdown    中文版语法说明](http://wowubuntu.com/markdown/#list)
##**使用 Markdown 的优点**
- <big>专注你的文字内容而不是排版样式。
- <big>轻松的导出 HTML、PDF 和本身的 .md 文件。
- <big>纯文本内容，兼容所有的文本编辑器与字处理软件。
- <big>可读，直观。适合所有人的写作语言。
#**我该用什么工具？**
##**Mac 平台**
<big>在Mac OS X 上，我强烈建议你用[Mou](http://25.io/mou/)这款免费且十分好用的 Markdown 编辑器，它支持实时预览，既左边是你编辑 Markdown 语言，右边会实时的生成预览效果，笔者文章就是 Mou 这款应用写出来的。
![Mou](https://img-blog.csdn.net/20161109202400925)
其次还有很多同类选择。如果你是个编辑作者，我强烈建议你购买[Ulysses Ⅲ](https://ulyssesapp.com/)，这款应用入围了苹果去年Mac App Store的The Best of 2013，相比Mou它支持更多的写作格式、多文档的支持。Mou、iA Writer这些应用都是基于单文档的管理方式，而Ulysses Ⅲ支持Folder、Filter的管理，一个Folder里面可以创建多个Sheet，Sheet之间还可以进行Combine处理。
![Ulysses Ⅲ](https://img-blog.csdn.net/20161109202606302)
##**Windows、iOS、Web 平台**
- <big>笔者并未使用过 Windows 下的 Markdown 工具，但经朋友介绍，有两款还算不错，一款叫[MarkdownPad](http://www.markdownpad.com/)，另一款叫[MarkPad](http://code52.org/DownmarkerWPF/)。
- <big>iOS 端已有相当多的 app 支持 Markdown 语法编辑，例如 Drafts、Day One、iA Writer 等。
- <big>Web 端上，我强烈推荐简书这款产品，上面有无数热爱文字的人在不停的创造、分享。在 Web 端使用 Markdown 没有比简书更舒服的地方了，它同样支持左右两栏的实时预览，字体优雅、简洁。
![这里写图片描述](https://img-blog.csdn.net/20161109203106325)
- <big>同样是Web端，[Draftin](https://draftin.com/)这款在线MD编辑器也近乎完美。
#**Markdown语法的简要规则**
##**标题**
<big>标题是每篇文章都需要也是最常用的格式，在 Markdown 中，如果一段文字被定义为标题，只要在这段文字前加 # 号即可。
![这里写图片描述](https://img-blog.csdn.net/20161109203858288)
以此类推，总共六级标题，建议在井号后加一个空格，这是最标准的 Markdown 语法。
##**字号**
![这里写图片描述](https://img-blog.csdn.net/20161109205941363)
<big>效果：
<big>这是放大一个字号</big>
<small>这是缩小一个字号</small>
##**列表**
<big>熟悉 HTML 的同学肯定知道有序列表与无序列表的区别，在 Markdown 下，列表的显示只需要在文字前加上 - 或 * 即可变为无序列表，有序列表则直接在文字前加 1. 2. 3. 符号要和文字之间加上一个字符的空格。
![这里写图片描述](https://img-blog.csdn.net/20161109204003056)
##**引用**
<big>如果你需要引用一小段别处的句子，那么就要用引用的格式。
![这里写图片描述](https://img-blog.csdn.net/20161109204052416)
只需要在文本前加入 > 这种尖括号（大于号）即可
##**图片与链接**
<big>插入链接与插入图片的语法很像，区别在一个 ! 号
插入图片的地址需要图床，这里推荐[CloudApp](https://www.getcloudapp.com/)的服务，生成URL地址即可。
![这里写图片描述](https://img-blog.csdn.net/20161109204209496)
##**粗体与斜体**
<big>Markdown 的粗体和斜体也非常简单，用两个 * 包含一段文本就是粗体的语法，用一个 * 包含一段文本就是斜体的语法。
![这里写图片描述](https://img-blog.csdn.net/20161109204354733)
##**表格**
<big>表格是我觉得 Markdown 比较累人的地方，例子如下：
|Tables         |      Are      |   Cool|
|:------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |

<big>这种语法生成的表格如下：略
<big>从上面的例子，我们可以看出来，Markdown生成的表格是使用冒号来选择左 :---、右 ---: 或者居中 :---: 对齐的。其中代码中第二行中的短线的数量可以根据美观来定。
##**代码框**
<big>如果你是个程序猿，需要在文章里优雅的引用代码框，在 Markdown 下实现也非常简单，只需要用多个 ` 把中间的代码包裹起来，后面可以注明语言类型，使用 tab 键即可缩进。。

<big>例如：
![这里写图片描述](https://img-blog.csdn.net/20161110101757786)
效果：略
##**分割线**
<big>分割线的语法只需要另起一行，连续输入三个星号 ***  即可(CSDN上很不明显)。
##**公式**
<big>Markdown支持LaTeX写公式，LaTeX 公式有两种，一种是用在正文中的，一种是单独显示的。正文中的公式用 ```$...$ ```来定义，单独显示的用 ```$$...$$ ```来定义，其中 ... 表示的是LaTeX 的公式命令。
例如：
我们定义$f(x) = \sum_{i=0}^{N}\int_{a}^{b} g(t,i) \text{ d}t$. (行内公式)
或者定义$f(x)$如下（行间公式）: 
$$f(x) = \sum_{i=0}^{N}\int_{a}^{b} g(t,i) \text{ d}t{6}\tag{1}$$
<big>得到的结果是：略
- <big>[使用LaTeX基本数学公式](http://blog.csdn.net/bendanban/article/details/44196101)
- <big>[使用LaTeX写矩阵](http://blog.csdn.net/bendanban/article/details/44221279)
##**分段**
<big>「Markdown 语言」不负责实现段首缩进，事实上大多web语言都没有首行缩进，我们在区分段落时一般使用段间距，如果没有，可以强行空一行，在Markdown中推荐使用空一行这种方法。

<big>如果必需输入空格，有以下两种方法可以使用。

- <big>使用```  ```，对于中文不是很不推荐这种方法。
- <big>全角空格，切换到全角模式下（一般的中文输入法都是按 shift + space）输入两个空格，宽度是整整两个汉字，整齐干净。
#**小结**
<big>到这里，Markdown 的基本语法在日常的使用中基本就没什么大问题了，只要多加练习，配合好用的工具，写起东西来肯定会行云流水。更多的语法规则，其实Mou的 Help文档例子很好，当你第一次使用Mou时，就会显示该文档，其次，你也可在撰写过程中，使用CMD+R快捷键来快速打开文档，以随时查阅和学习语法。
![这里写图片描述](https://img-blog.csdn.net/20161109204641071)
#**与 Markdown 相关的一些推荐**
##**可配套使用的工具**
- <big>[Droplr](http://droplr.com/)
- <big>[Cloudapp](https://www.getcloudapp.com/)
- <big>[ezShare for Mac](https://itunes.apple.com/cn/app/yi-xiang/id672522335?mt=12&ign-mpt=uo=4)
- <big>[围脖图床修复计划](http://weibotuchuang.sinaapp.com/)
- <big>[马克飞象](https://maxiang.io/)，专为印象笔记打造的 Markdown 编辑器，非常推荐。
##**相关文章阅读**
- <big>[为什么作家应该用 Markdown 保存自己的文稿](http://apple4us.com/2012/02/why-writers-should-use-markdown/)
- <big>[Markdown 写作浅谈](http://www.yangzhiping.com/tech/r-markdown-knitr.html)
- <big>[Markdown 工具补完](http://www.appinn.com/markdown-tools/)
- <big>[Drafts + Scriptogr.am + Dropbox 打造移动端 Markdown 风格博客](http://www.jianshu.com/p/63HYZ6)
- <big>[图灵社区 - 怎样使用 Markdown](http://www.ituring.com.cn/article/23)
- <big>[为什么我们要学习 Markdown 的三个理由](https://news.cnblogs.com/n/139649/)
- <big>[Markdown 语法写作入门指南 by ibuick](http://ibuick.me/?p=4093)
```











