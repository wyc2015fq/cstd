# 连玉君 Markdown 笔记 - stata连享会 - CSDN博客





2018年09月21日 00:00:35[arlionn](https://me.csdn.net/arlionn)阅读数：642








# 连玉君 Markdown 笔记

> 
…… Stata连享会 - [精彩推文1](https://gitee.com/arlionn/stata_training/blob/master/README.md)  || [精彩推文2](https://github.com/arlionn/stata/blob/master/README.md) ……





### 文章目录
- [连玉君 Markdown 笔记](#_Markdown__0)
- [1. Markdown 基础](#1_Markdown__7)
- [1.1 标题](#11__21)
- [1.2 列表](#12__34)
- [无序列表](#_37)
- [有序列表](#_48)
- [列表的缩进和混排](#_59)
- [记事列表](#_76)
- [1.3 文字效果和分隔线](#13__115)
- [1.4 链接](#14__158)
- [文本链接](#_160)
- [插入图片](#_211)
- [脚注 / 尾注 / 参考文献](#_____232)
- [缩写名词 / 专有名词 / 索引条目](#_____257)
- [1.5 文字高亮和代码块](#15__282)
- [1.6 特殊字符](#16__339)
- [转义符](#_341)
- [脚注 (尚未完成)](#__398)
- [1.6 表格](#16__409)
- [2. Markdown 进阶功能](#2_Markdown__435)
- [2.1 数学公式](#21__437)
- [2.2 书签(锚链接)](#22__460)
- [2.3 目录](#23__470)
- [2.4 编辑器](#24__481)
- [2.5 CSS 样式](#25_CSS__492)
- [2.6 HTML 与 Markdown 的相互转换](#26_HTML__Markdown__496)
- [2.7 有道云笔记 Markdown 编辑器快捷键](#27__Markdown__506)
- [3. 应用专题](#3__516)
- [3.1 [在知乎使用 Markdown](https://zhuanlan.zhihu.com/p/23501187)](#31__Markdownhttpszhuanlanzhihucomp23501187_519)
- [3.2 [使用 markdown 写知乎专栏的终极解决方案](https://zhuanlan.zhihu.com/p/26983517)](#32__markdown_httpszhuanlanzhihucomp26983517_525)
- [3.3 [将 Markdown 文章搬运到微信公众号](http://blog.csdn.net/dyc87112/article/details/77417572)](#33__Markdown_httpblogcsdnnetdyc87112articledetails77417572_526)
- [3.4 使用 Markdown 制作简报(幻灯片)](#34__Markdown__529)
- [参考文献 (相关资料)](#__537)




## 1. Markdown 基础
- 
快速入门资料
- 
[为什么应该用 Markdown](http://jolestar.com/markdown-tools/)

- 
[有道 Markdown 指南](http://note.youdao.com/iyoudao/?p=2411&vendor=unsilent14)

- 
[极简 Markdown 指南](https://www.binarization.com/archive/2016/markdown-guide/#help)

- 
[Markdown 语法](https://www.binarization.com/archive/2016/markdown-syntax/)

- 
语法-预览对照范例：[范例1](https://www.zybuluo.com/mdeditor#872604), [范例2](http://pandao.github.io/editor.md/)



### 1.1 标题

只要一段文字前加 `#` 号即可定义标题， `#` 号的个数代表标题的级数，共有六级标题。书写时，建议在 `#` 号后加一个空格，这是最标准的 Markdown 语法。

```
# 一级标题

## 二级标题

### 三级标题
```

### 1.2 列表

Markdown 可以支持++有序列表++、++无序列表++、++记事列表++。

#### 无序列表

> 
无序列表使用星号 (`*`)、加号(`+`)或是减号(`-`) 作为列表标记 (三者均可，其后需要加一个空格)。因此，如下两种写法是等价的。**Note**: 三者可以混用，但会导致行间距不一致。


```
- 红色     
- 绿色
- 蓝色

* 红色     
* 绿色
* 蓝色
```

#### 有序列表
- 有序列表则使用数字接着一个英文句点：

```
1. 打开冰箱
2. 塞入大象
3. 关上冰箱
```
- **两点说明：**- 其一，将文字前的数字序号从`1. 2. 3.` 修改为 `1. 1. 1.` 并不影响最终的显示效果；
- 其二，若希望有序列表从`8.` 开始编号，则第一行改写为 `8. 打开冰箱` 即可。


#### 列表的缩进和混排

若需缩进，只需在 `-` 前加两个空格或四个空格 (敲一个 `Tab` 键相当于四个空格)。

例如：

```
- 一级列表
  - 二级列表
    - 三级列表
      1. 四级列表
      1. 三级列表
```

输出效果为：
- 一级列表
- 二级列表
- 三级列表
- 四级列表
- 三级列表




#### 记事列表

用 `- [x]` 开头的文本表示已完成的记事，`- [ ]` 开头的表示待办记事。上述标记后要加一个空格。如：

```
- [x] 已完成记事
- [ ] 待办记事
  - [ ] 吃啥？
    - [ ] 去吃个 subway 吧？
```

输出效果为：
-  已完成记事
-  待办记事
-  吃啥？
-  去吃个 subway 吧？


区块引用可以嵌套（例如：引用内的引用），只要根据层次加上不同数量的 > ：

```
> This is the first level of quoting.
>
> > This is nested blockquote.sdf
>
> Back to the first level.
```

> 
This is the first level of quoting.

> 
This is nested blockquote.sdf


Back to the first level.


### 1.3 文字效果和分隔线
- 文字效果的基本语法

```
*斜体*
**粗体**
++下划线++
~~删除线~~
==文本高亮==
**** (分隔线1)
---- (分隔线2)
```
- 
分隔线

在一行中用三个以上的星号 (`*`)、减号 (`-`) 或是下划线 (`_`) 来建立一个分隔线。需要独占一行，中间可以有空格，但行内不能包含其他字符。如下三种写法等价：


```
***
* * *
---
```
- 范例

```
---
    我用`Stata`已经有**十五年**了，
    它的++功能++很==庞大==。
    目前还没有~~发现~~明显的*缺点*。
    ---
```

输出效果：

> 
我用`Stata`已经有**十五年**了，它的 ++功能++ 很庞大。目前还没有发现明显的*缺点*。


### 1.4 链接

#### 文本链接

Markdown 支持两种形式的文本链接语法：++行内式++和++参考式++。
- 
**行内式**： `[文本](链接地址)`

也可以采用这种格式：`[文本](链接地址 "可选说明文字")`。

好处是当鼠标停留在`文本`上时，可以显示`可选文字说明`的内容。

> 
范例：


```
学习 [Stata](http:\\www.stata.com) 过程中遇到问题，
可以查看 [老连](http://www.lingnan.sysu.edu.cn/lnshizi/faculty_vch.asp?name=lianyj 
"连玉君主页") 
的 [知乎专栏](https://zhuanlan.zhihu.com/arlion "连玉君 Stata 知乎专栏")。
```

> 
学习 [Stata](http:%5Cwww.stata.com) 过程中遇到问题，

可以查看 [老连](http://www.lingnan.sysu.edu.cn/lnshizi/faculty_vch.asp?name=lianyj)

的 [知乎专栏](https://zhuanlan.zhihu.com/arlion)。- 
**参考式**：

> 
`[文本][标签]`

`[标签]: 链接地址 "可选文字说明"`

- 参考式比较适合定义那些将在文中多次出现的文本链接。
- 使用参考式的最大好处是代码的可读性比较高。
- 范例：

```
学习 [Stata][1] 时遇到问题，
可以查看 [老连][2]
的 [知乎专栏][Lzh]。
[老连][2] 用 [Stata][1] 很多年了，人很好。

[1]: http:\\www.stata.com
[2]: http://www.lingnan.sysu.edu.cn/lnshizi/faculty_vch.asp?name=lianyj "连玉君主页" 
[Lzh]: https://zhuanlan.zhihu.com/arlion "连玉君 Stata 知乎专栏"
```

> 
学习 [Stata](http:%5Cwww.stata.com) 时遇到问题，

可以查看 [老连](http://www.lingnan.sysu.edu.cn/lnshizi/faculty_vch.asp?name=lianyj)

的 [知乎专栏](https://zhuanlan.zhihu.com/arlion)。
[老连](http://www.lingnan.sysu.edu.cn/lnshizi/faculty_vch.asp?name=lianyj) 用 [Stata](http:%5Cwww.stata.com) 很多年了，人很好。
#### 插入图片

插入图片的方法与文字链接非常相似，也分为行内式和参考式两种类型。这里仅介绍行内式。
- 
语法格式： `![image](图片链接地址)`

或 `![image](图片链接地址 "说明文字")`

> 
范例：


```
这是老连讲课的样子
![](http://dwz.cn/lianyj45 "连玉君授课中")
```

> 
这是老连讲课的样子
![](http://dwz.cn/lianyj45)
> 
**说明：**

- 插入图片的语法和链接的语法很像，只是前面多了一个 `!`。将鼠标移动至图片区域内，光标下方会显示`连玉君授课中`字样。
- 本地图片没有 `http://` 开头的链接，可以先通过新浪微博、[极简图床](http://jiantuku.com/#/)、七牛图床([攻略](%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8%E4%B8%83%E7%89%9B%E4%BA%91%E5%81%9A%E4%B8%BA%E5%9B%BE%E5%BA%8A%EF%BC%9F))等生成图片链接网址。以新浪微博为例：在新浪微博发布一个微博（选择`仅自己可看`），然后`右击`图片，选择`复制图形地址(O)`即可。 也可以用[极简图床](http://yotuku.cn/?utm_source=doubangroup#/)在线生成图片的地址。它可以支持粘贴、本地上传、拖拽的方式生成图片的 markdown 链接，然后直接拷贝到 markdown 编辑器，就可以生成图片。
- 目前，Markdown 还无法指定图片的宽高，如果需要，可以使用普通的 

#### 脚注 / 尾注 / 参考文献

可以采用 **文本[#]** 模式添加尾注。这个尾注也可以用作学术论文的参考文献。
- 
定义方法：

```
Stata[^1] 可以很方便地估计各类面板数据[^xt]模型，连玉君(2008)[^连2008] 应用了动态面板模型。

[^1]: 一个很流行的数据处理和统计分析软件。
[^xt] 也称为追踪数据。
[^连2008]: 连玉君, 苏治, 2008, 现金持有动态调整机制研究. 《世界经济》，第 10 期。
```

Stata[1](#fn1) 可以很方便地估计各类面板数据[2](#fn2)模型，连玉君(2008)[3](#fn3) 应用了动态面板模型。

- 
说明：
- 正文中的添加尾注标签：`文字内容[^label]`。
- 定义尾注内容：`[^label]: 标签的具体内容` (冒号要采用英文半角录入)。
- `label` 可以是数字、英文字母、汉字，或者三者的组合。
- `[^label]: 标签的具体内容` 可以放在任何位置。


#### 缩写名词 / 专有名词 / 索引条目
- 
定义方式

`*[缩写名词]: 名词解释文字`
- 范例：

```
在 Stata 中，千万别把 GMM 当成 郭美美。
*[GMM]: Generalized Mothod of Moment (广义矩估计方法)
*[郭美美]: 一个网红美女
```

> 
在 Stata 中，千万别把 GMM 当成 郭美美。

*[GMM]: Generalized Mothod of Moment (广义矩估计方法)

*[郭美美]: 一个网红美女- 
说明
- 正文中引用缩写名词 (如上例中的 **GMM** 和 **郭美美**) 时，前必须有一个空格；
- 引用缩写名词时不能附加其他效果(如斜体、粗体等);
- 一次定义，全文有效：一旦在文中任何一处定义了缩写名词， 则无论在定义之前还是之后出现的缩写名词都会显示其释义。我通常会把所有缩写名词都定义在 `[toc]` 语句下方，便于查看和修改。


### 1.5 文字高亮和代码块
- 
**文本高亮**。需要高亮的文本，用 ``` 包围之。如，输入 ``stata``，显示为 `stata`。

- 
**引用代码块**。可以使用两组 `` `````(独占一行) 包围这段代码，或将代码缩进四个空格 (直接敲入 `Tab` 键即可产生四个空格)。
- 范例

```
```stata
sysuse auto, clear 
regress mpg price wei leng, robust
```
```

```
sysuse auto, clear 
regress mpg price wei leng, robust
```

> 
**Notes**:
- 上述代码引用效果是通过用 `````markdown` (首)

和 `` ``````(尾) 包围来实现的。二者都是独占一行。
- `` `````后面的 `stata` 表示此段代码为 `stata`  代码，Markdown 会自动使用 stata 代码颜色渲染。根据需要，可以替换成 `R`, `python`, `java` 等。平时也可以不写。


- 
**代码段中的反引号。** 如果要在代码区段内插入反引号 ```，可以用多个反引号来开启和结束代码区段。
- 范例：

```
你可以在正文中呈现 `` ` ``，`` `stata` ``，以及 `` ```stata `` 的原貌。
```

> 
你可以在正文中呈现 ```，``stata``，以及 ````stata` 的原貌。


- 
**列表中包含引用。** 如果要在列表中放入引用，那 `>` 就需要缩进 4 个空格或 1 个制表符：

```
* A list item with a blockquote:

    > This is a blockquote
    > inside a list item.
```
- A list item with a blockquote:

> 
This is a blockquote

inside a list item.



- 
**列表中放入代码块。** 如果要在列表中放代码区块的话，该区块就需要缩进两次，也就是 8 个空格或是 2 个制表符：

```
* 呈现相关系数矩阵

        pwcorr y x1 x2 x3
```
- 
呈现相关系数矩阵

```
pwcorr y x1 x2 x3
```



### 1.6 特殊字符

#### 转义符
- 
在如下字符前加反斜杠 `\` 可以呈现其原貌：

```
\   反斜线
`   反引号
*   星号
_   底线
{}  花括号
[]  方括号
()  括弧
.   英文句点
!   惊叹号
上角文字: 19^th^
下角文字: H~2~O
```


& sect  `$\rightarrow$` →  §
- 
范例：

```
\#陌上花开\#，可\缓\缓归\`矣\` !  
(我+你)\-她 = Sad^2^ + H~2~O
```

> 
#陌上花开#，可\缓\缓归`矣` !

(我+你)-她 = Sad2 + H2O


- 
**段首空两格**
- 在 Markdown 里直接打空格无法实现段首缩进。由于 Markdown 支持 HTML 语法，可以用 ` `，` ` 等 [HTML 标记符](http://blog.csdn.net/xcg8818/article/details/75006210) 产生空格效果。
- 范例：

```
普通空格| |    
半角空格| |
全角空格| |
  段落从此开始。
```

> 
普通空格| |

半角空格| |

全角空格| |

  段落从此开始。



另一种[方法](http://www.zhihu.com/question/21420126)：切换到全角模式下（中文输入法中按 `shift + space`）输入两个空格即可，宽度恰好是两个汉字。不过，此法在有些编辑器(如，有道云笔记的 Markdown) 中不适用。
- 
**其他特殊字符**

[在 Markdown 中使用 HTML 中的特殊符号](http://blog.csdn.net/vola9527/article/details/69948411)

[如何在markdown中打出上标、下标和一些特殊符号](http://www.jianshu.com/p/80ac23666a98)


### 脚注 (尚未完成)
- 
这个链接用 1 作为网址变量 [Google](http:%5Cwww.stata.com).

- 
这个链接用 yahoo 作为网址变量 [Yahoo!](http://www.yahoo.com/).

然后在文档的结尾为变量赋值（网址）

- 
[如何在markdown中打出上标、下标和一些特殊符号](http://www.jianshu.com/p/80ac23666a98)


### 1.6 表格

具体使用方式请看示例。
- `------:` 为右对齐。
- `:------` 为左对齐。
- `:------:` 为居中对齐。
- `-------` 为使用默认居中对齐。

```
Tables        | Are           | Cool  
 ------------- |:-------------:| -----:
 col 3 is      | right-aligned | $1600 
 col 2 is      | centered      |   $12 
 zebra stripes | are neat      |    $1 
 stata|is|good
```

输出为：
|Tables|Are|Cool|
|----|----|----|
|col 3 is|right-aligned|$1600|
|col 2 is|centered|$12|
|zebra stripes|are neat|$1|

## 2. Markdown 进阶功能

### 2.1 数学公式
- 单行数学公式可以用 `$$` 和 `$$` 包围 TEX  公式来实现
- 行内公式可以用 `$` 和 `$` 包围 TEX 公式来实现。注意：`$` 两侧不能有空格。
- 范例：


```
$$ y_i = \beta_0 + x_i\beta_1 + \epsilon_i$$
    其中，$\epsilon_i $ 表示干扰项，$ i=1, 2, \cdots, N $。
```

输出效果为：
$$    y_i = \beta_0 + x_i\beta_1 + \epsilon_i$$

其中，$\epsilon_i$ 表示干扰项，$i=1, 2, \cdots, N$。- 
相关链接
- [常用 LaTeX 数学公式](http://www.mohu.org/info/symbols/symbols.htm)
- [Markdown LaTeX 完美教程](http://xiang.leanote.com/post/introduction-to-mathjax-and-latex-expression)
- [Markdown 添加数学公式](http://www.cnblogs.com/peaceWang/p/Markdown-tian-jia-Latex-shu-xue-gong-shi.html)


### 2.2 书签(锚链接)

`锚链接 (anchor)` 就是 word 中的`书签`功能。

[锚链接定义](#ac01)

请参阅

跳转到这里：

### 2.3 目录

自动生成的目录有两类：顶端目录和侧边目录。

对于有道云笔记中的 Markdown 文档而言，可以在需要插入目录的地方输入 `[toc]` 即可自动生成目录。
- 目录的层级与你所定义的标题层级一致。
- 有些编辑器不支持 `[toc]` 目录功能

> 
相关链接

- [MarkDown 自动生成目录](http://www.jianshu.com/p/1b29172d4f7e)

### 2.4 编辑器

我目前主要使用 CSDN 的在线编辑器，支持 LATEX 数学公式，可以直接上传本地图片。简书的在线编辑器也不错，属于极简风格。但最近我开始远离简书了，因为她动不动就因为怀疑你的文章中有敏感词而自动封锁你的作品。

另一款目前使用起来相当舒服的编辑器是 Topyta，默认可以将 Markdown 文档转换成 PDF，对中文的支持非常友好。如果进一步配合 pandoc，可以快捷地将 Markdown 文档转换成 Word，HTML，幻灯片等格式，非常方便。

其他编辑器没怎么用，附上几篇网上的推介，供各位自行筛选。
- [7 款优秀 Markdown 编辑工具推荐](https://sspai.com/post/27792)
- [Sublime Text2 配置成 MarkdownPad2 效果](http://blog.csdn.net/humanking7/article/details/43516903)

### 2.5 CSS 样式
- [MarkDown CSS样式](http://blog.csdn.net/zhanlanmg/article/details/47947453)

### 2.6 HTML 与 Markdown 的相互转换

有时看到别人用 Markdown 写的文章不错，想要弄到自己的文章中，但又不想重新按照 Markdown 的语法敲一次，可以使用在线工具转换。

**方法：** 选中网页中的文字或链接，右击，在浏览器弹出窗空中选择`查看选中部分的源码`，然后把弹出的源码粘贴到如下在线工具中，转换成 Markdown 格式。
- HTML → Markdown :  [BeJSON](http://www.bejson.com/convert/html2markdown/)  | [aTool](http://www.atool.org/html2markdown.php)

### 2.7 有道云笔记 Markdown 编辑器快捷键
- 选中多行文本后，按下 `Tab` 键，可以整体右移一个 `Tab` 键位；
- `Ctrl+D`: 删除光标所在的段落
- `Ctrl+Shift+D`: 相当于回车键，但光标会停留在当前位置；
- `Ctrl+Cmd+K`: (选中部分文本时) 按此键向下查找并选中相同的文本；
- `Ctrl+Shift+L` 选中光标所在的段落，多次按下该组合键，可以向下选中多个段落；

### 3. 应用专题

#### 3.1 [在知乎使用 Markdown](https://zhuanlan.zhihu.com/p/23501187)

> 
**思路**: `Chrome 浏览器 + Markdown Here 插件`, 用后者将 Markdown 代码渲染为 html 格式代码。


> 
流程: 在插件市场下载一个叫 Markdown Here 的插件并安装 -> 在 chrome 上打开知乎编辑器开始使用markdown格式进行写作 -> 写作完毕，点击浏览器右上角 markdown here 插件图标进行渲染，发布文章。


#### 3.2 [使用 markdown 写知乎专栏的终极解决方案](https://zhuanlan.zhihu.com/p/26983517)

#### 3.3 [将 Markdown 文章搬运到微信公众号](http://blog.csdn.net/dyc87112/article/details/77417572)

#### 3.4 使用 Markdown 制作简报(幻灯片)
- 与编辑器有关
- 需求：一种输入，多种输出
- 尚未完成

#### 参考文献 (相关资料)
- 
[完整版 Markdown 手册](http://wowubuntu.com/markdown/)

- 
[Markdown 编辑器语法指南](https://segmentfault.com/markdown) 写的非常精炼，清晰

- 
[在知乎使用 Markdown](https://zhuanlan.zhihu.com/p/23501187)

- 
一个很流行的数据处理和统计分析软件。 [↩︎](#fnref1)

- 
也称为追踪数据。 [↩︎](#fnref2)

- 
连玉君, 苏治, 2008, 现金持有动态调整机制研究. 《世界经济》，第 10 期。 [↩︎](#fnref3)


















