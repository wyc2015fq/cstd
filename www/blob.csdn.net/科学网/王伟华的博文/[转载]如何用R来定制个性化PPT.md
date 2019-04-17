# 科学网—[转载]如何用R来定制个性化PPT - 王伟华的博文




# [转载]如何用R来定制个性化PPT                           

已有 2541 次阅读2016-1-28 00:16|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|文章来源:转载



ReporteRs包可以创建word,ppt,html文档。它可以格式化R的输出：如可编辑的矢量图，复杂的表格报告功能，企业模板文档的重用（*.docx**和*.pptx）。它是一个很好的自动化报告工具，并且不需要你安装微软的任何产品。需要注意的是，电脑需安装了java（>=1.6）本文演示的是如何用它来制作PPT文档。    

**入门**

**创建一个****pptx****对象**

通过pptx函数创建的一个R对象可以表示一个PPT文档。它包含两个参数：标题和模板文件。如果未指定，模板文件在包目录下是一个空文档。

每当创建一个pptx对象的时候，你就可以使用一个模板文件。这个文件是从内存中复制过来的，并且副本可以通过R输出为一个文档。模板文件提供了可用的格式和幻灯片版式。

**将****R****的输出结果发送到一个对象**

接下来，创建pptx文件的组成部分。

在将R的输出发送到一个文档（或幻灯片）之前，必须**添加一个幻灯片**。通过函数addSlide可以实现。

在添加幻灯片的时候，需要选定一个布局。然后添加文本，表格，图形和其它组成部分。

**将对象写入到一个文件中**

最后，使用函数writeDoc将对象写入到后缀为.pptx的文件中。

**示例**

下面我们通过一个带有注解的R脚本来做演示：

library( ReporteRs )



# 创建一个pptx对象的文档（默认模板）

mydoc = pptx( )



# 检测布局名称

slide.layouts(mydoc)



mydoc = addSlide( mydoc, "TwoContent" )

# 将iris数据集的前10行添加到mydoc

mydoc = addTitle( mydoc, "First 10lines of iris" )

mydoc = addFlexTable( mydoc,FlexTable(iris[1:10,] ) )



# 在mydoc中添加文本（从addParagraph函数的value参数可以看出第一行为空行）

# 属性值由正在使用的布局格式给定

mydoc = addParagraph( mydoc, value =c("", "Hello World!") )



mydoc = addSlide( mydoc, "Title andContent" )

# 在mydoc中添加图形

mydoc = addPlot( mydoc, function() barplot(1:8, col = 1:8 ) )



# 将mydoc写入文档中（在当前工作目录下会生成一个ppt）

writeDoc( mydoc, "pp_simple_example.pptx")

**模板，版式和样式**

pptx函数可以通过"pptx"模板文件创建一个文档。

如果没有提供，可以使用一个空文档（在包目录下的templates文件中可以找到）

# 使用D:/docs/template/my_corporate_template.pptx作为模板（需在上述路径先创建一个名称为my_corporate_template.pptx的ppt，否则提示文件无效）

   doc = pptx(template = 'D:/docs/template/my_corporate_template.pptx')

# 使用默认模板

   doc = pptx()

ReporteRs使用PPT文件作为模板。这个模板是一个最原始的PPT文档，所有的幻灯片布局，外形（占位符）和样式来自于：

·模板中可用的幻灯片版式

·模板中自定义好的设计，模板格式和外形（模板中的占位符）

·ppt模板的内容不会被删除，以便在现有的演示文稿中添加内容，而不用再复制粘贴。

如果对这些PPT版式的术语不太了解，可以参考这两篇文章：[自定义版式](https://support.office.com/en-us/article/Create-a-new-custom-layout-48664ad4-7fc5-4197-9aec-5942e65397a1?CorrelationId=2fa9ef24-3668-4f3f-885b-79df2a2d750e&ui=en-US&rs=en-US&ad=US)-[修改占位符](http://office.microsoft.com/en-us/powerpoint-help/change-a-placeholder-HA010064940.aspx)

**如果需要除了****PPT****内置的幻灯片母版版式**，可以在你的PPT演示文稿中添加版式并做自定义。要添加自己的版式，只需在PPT中按如下操作即可：

1.切换到幻灯片母版视图

2.插入版式

3.插入你想要增加到新版式中的占位符。根据需要包含的内容移动并调整它们的大小。

4.可以对新的占位符做些设置（如：修改背景颜色，字体等）

5.保存并关闭模板

6.通过pptx函数中参数template所指定的模板文件名创建一个pptx对象。

关于模板的更多了解，参考[这里](http://office.microsoft.com/en-gb/powerpoint-help/what-is-a-slide-layout-HA010338412.aspx)

**添加内容**

**添加幻灯片**

利用addSlide函数可以在**pptx**对象中添加一个幻灯片。可用的形状数根据所选的版式通过R输出来填充。例如，版式"Title and Content"只能接收一个R输出，"Two Content"可以接收两个R输出。

如果你需要一个新的版式，在PPT中创建它。

当添加一个R输出的时候，如果没有指定位置和大小，R输出会采用被定义的模板版式中的位置和尺寸。如果你不喜欢模板中的模型位置属性，可以强制改变大小和位置。

**将****R****的输出结果发送到幻灯片**

接下来，创建幻灯片的组成部分。

在幻灯片中添加文本，表格，图形和其它组成部分。可用的函数见如下**函数列表**。

如果现在的幻灯片没有剩余空间或者是你想添加其它内容，可以强制设定模型的大小和位置。具体可参考**几个关键的函数和参数**

**函数列表**

下面这些函数可以用于输出格式为pptx的文档：

·添加标题：[addTitle](http://davidgohel.github.io/ReporteRs/addTitle.html)

·添加表格：addFlexTable-见[FlexTable](http://davidgohel.github.io/ReporteRs/FlexTable.html)和[addFlexTable](http://davidgohel.github.io/ReporteRs/addFlexTable.html)

·添加图形：见[addPlot](http://davidgohel.github.io/ReporteRs/addPlot.html)

·添加外部图像：见[addImage](http://davidgohel.github.io/ReporteRs/addImage.html)

·添加文本段落：[addPagraph](http://davidgohel.github.io/ReporteRs/addParagraph.html)

·添加语法高亮的R代码：见[addRScript](http://davidgohel.github.io/ReporteRs/addRScript.html)

·添加幻灯片：addSlide（见"**几个关键的函数和参数**"）

·将pptx对象写入PPT文档：见[writeDoc](http://davidgohel.github.io/ReporteRs/writeDoc.html)

·添加日期：addDate（见下述"**几个特殊函数**"）

·添加脚注：addFooter（见下述"**几个特殊函数**"）

·添加页数：addPageNumber（见下述"**几个特殊函数**"）

**几个关键的函数和参数**

**addSlide****函数**

函数addSlide可以添加一个幻灯片到pptx对象。参数slide.layout指定新建幻灯片的版式设计。

oc = pptx()

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addSlide( doc, slide.layout = 'TwoContent' )

**slide.layouts****函数**

模板中包含了可用的版式。幻灯片的版式概念在ReporteRs中非常重要。

内容取决于所选择的版式。当添加的幻灯片版式为"Title and Content"时，幻灯片只包含两个部分：标题和内容。

外形（或占位符）的大部分格式都已经设置好了：图形的大小由版式中的外形尺寸指定好了，默认字体和段落样式也由外形的属性值指定。

slide.layouts返回幻灯片版式名称。用addSlide函数添加一个幻灯片的时候可能需要核对下有哪些版式可用。

doc = pptx()

slide.layouts(doc)

**检查幻灯片版式**

通过slide.layouts函数的可选参数layout获取版式的设计图。

 slide.layouts(doc, 'Comparison') 

![](http://image.sciencenet.cn/album/201601/26/140933qxu0p6xxsxfw4of9.gif)

这里，幻灯片可用接收如下形状：标题，四个内容（图形，表格，段落），日期和幻灯片数。

**设置大小和位置**

宽度，高度和输出位置在PPT模板给定的时候就通过外形属性值设定好了。**当我们在幻灯片中添加内容的时候，****ReporteRs****会在当前幻灯片中读取下一个可用外形的属性值。你可以指定一些特定输出（表格，文本，图形和外部图像）的外形位置和尺寸**如果没有设定offx,offy,width,heigh这些参数。位置的尺寸会通过幻灯片的下一个可用形状中的width和heigh默认设定好。如果指定这些参数，可以得到新的外形位置和尺寸。这样做可以在幻灯片没有更多剩余的空间时添加新的内容。

以下R脚本可以生成

library( ReporteRs )

library( ggplot2 )



mydoc = pptx( title = "title" )



mydoc = addSlide( mydoc, slide.layout ="Title and Content" )



myplot = qplot(Sepal.Length, Petal.Length,data = iris,

 color = Species, size = Petal.Width, alpha = I(0.7) )



mydoc = addPlot( doc = mydoc, fun = print,x = myplot,

 offx = 1, offy = 1, width = 6, height = 5 )



mydoc = addFlexTable( doc = mydoc,FlexTable( head( iris ) ),

 offx = 8, offy = 2, width = 4.5, height = 3 )



writeDoc( mydoc, file ="examples/pp_force_pos.pptx" )

**几个特殊函数**

**日期**

利用addDate函数可以在幻灯片中增加日期。它的形状通过模板文档中的日期形状属性值设定。

doc = pptx()

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addDate( doc )

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addDate( doc, 'Dummy date' )

**页数**

利用函数addPageNumber可以在幻灯片中添加幻灯片的页数。同样，它的形状通过模板文档中的页数形状属性值设定。

doc = pptx()

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addPageNumber( doc )

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addPageNumber( doc, 'Dummy text' )

**页脚**

函数addFooter可以实现在幻灯片的页脚处添加注解。同样，通过模板文件默认设定。

doc = pptx()

doc = addSlide( doc, slide.layout = 'Titleand Content' )

doc = addFooter( doc, 'Dummy text' )

**副标题**

函数addSubtitle实现在幻灯片中添加副标题。外形由模板默认设定。

副标题的外形只存在类型为"Title Slide"的幻灯片。

doc = pptx()

doc = addSlide( doc, slide.layout = 'TitleSlide' )

doc = addTitle( doc, 'Presentation title' )

doc = addSubtitle( doc , 'This document isgenerated with ReporteRs.')

**更换幻灯片**

通过设定addSlide中的参数bookmark可以实现幻灯片的替换。

library( ReporteRs )

library( ggplot2 )



#需提前在你的工作目录下新建一个名为"pp_example.pptx"的文档

mydoc = pptx( title = 'title', template ='pp_example.pptx' )



myplot = qplot(Sepal.Length, Petal.Length,data = iris, color = Species, size = Petal.Width, alpha = I(0.7))



mydoc = addSlide( mydoc, slide.layout ='Title and Content', bookmark = 3 )



mydoc = addTitle( mydoc, 'my new graph')



mydoc = addPlot( mydoc, print, x = myplot )



writeDoc( mydoc, 'pp_replacement.pptx' )

**注：**在本文中遇到的坑跟大家分享下。

1.前面已经提到创建一个模板有两种方法。方法一，mydoc =pptx()通过调用包目录下templates文件夹中的EMPTY_DOC.pptx作为模板。方法二，自己在当前工作目录下新建一个ppt文档作为模板。如：我在工作目录下新建了一个名为"PP_example.pptx"文档，通过mydoc = pptx( title = 'title',template = 'pp_example.pptx' )调用这个模板。这两种方法会有些区别：i)版式

采用方法一

slide.layouts(mydoc)

 [1]"Blank"                  

 [2]"Title Only"             

 [3]"Title and Content"      

 [4]"Section Header"         

 [5]"Title and Vertical Text"

 [6]"Comparison"             

 [7]"Content with Caption"   

 [8]"Title Slide"            

 [9]"Two Content"            

[10] "Vertical Title and Text"

采用方法二

slide.layouts(mydoc)

 [1]"比较"              "仅标题"             "图片与标题"        

 [4]"垂直排列标题与文本" "节标题"             "标题和竖排文字"    

 [7]"两栏内容"           "标题和内容"         "空白"              

[10] "内容与标题"         "标题幻灯片" 

所以，如果用的是方法二调用的模板，在做幻灯片替换的时候做下中文设置即可：mydoc =addSlide( mydoc, slide.layout = '标题和内容', bookmark = 3 )。但这里有一个问题就是无法插入图形，出现如下错误（如果有小伙伴解决了这个问题欢迎在下面评论中留言）：

mydoc = addPlot( mydoc, print, x = myplot )



Error in .jcall(doc$obj,paste0("L", class.pptx4r.LayoutDescription, ";"),  : 

 java.lang.NullPointerException

但文本，表格等可以插入。但通过方法一得到一个ppt文档，再把它作为模板后就可以插入图形了。

2.在做幻灯片替换的过程中，将pptx对象writeDoc()到ppt文档的时候ppt需先关闭。

3.用方法二作为模板的时候，需先在工作目录下新建一个ppt文档，否则提示文件不存在。

**完整案例**

下面的代码展示了可以用在pptx对象上的一些最有用的函数。

library( ReporteRs )

require( ggplot2 )



mydoc = pptx( title = "title" )



# 显示版式名称

slide.layouts( mydoc )



# 添加一个 Title slide -------------------------------------------------------

mydoc = addSlide( mydoc, slide.layout ="Title Slide" )



mydoc = addTitle( mydoc, "Presentationtitle" ) #设置主标题

mydoc = addSubtitle( mydoc , "Thisdocument is generated with ReporteRs.")#设置副标题





# 图形演示 ---------------------------------------------------------------

mydoc = addSlide( mydoc, slide.layout ="Title and Content" )

mydoc = addTitle( mydoc, "Plotexamples" )



myplot = qplot(Sepal.Length, Petal.Length

                , data = iris, color = Species

                , size = Petal.Width, alpha =I(0.7)

)

# 将图形添加到ppt

mydoc = addPlot( mydoc, function( ) print(myplot ) )



# 添加页数，日期，脚注

mydoc = addPageNumber(mydoc)

mydoc = addDate(mydoc)

mydoc = addFooter(mydoc, "Modify thegraph within PowerPoint")



# 表格演示----------------------------------------------------------

mydoc = addSlide( mydoc, slide.layout ="Title and Content" )

mydoc = addTitle( mydoc, "FlexTableexample" )



options( "ReporteRs-fontsize" =12 )



# 将mtcars数据框对象创建为一个可伸缩的表格并显示行名

# 将表头和表体设置为不同的格式属性

MyFTable = FlexTable( data = mtcars[1:15,],add.rownames = TRUE

                , body.cell.props =cellProperties( border.color = "#EDBD3E")

                , header.cell.props =cellProperties( background.color = "#5B7778" )

)

# 斑马线 - 表行的颜色背景是交替出现的

MyFTable = setZebraStyle( MyFTable, odd ="#DDDDDD", even = "#FFFFFF" )

MyFTable = setFlexTableWidths( MyFTable,widths = c(2,rep(.7,12)))



# 设置表格边框线

MyFTable = setFlexTableBorders(MyFTable

       , inner.vertical = borderProperties( color="#EDBD3E",style="dotted" )

       , inner.horizontal = borderProperties( color = "#EDBD3E",style = "none" )

       , outer.vertical = borderProperties( color = "#EDBD3E", style= "solid" )

       , outer.horizontal = borderProperties( color = "#EDBD3E",style = "solid" )

)



# 添加可伸缩表格

mydoc = addFlexTable( mydoc, MyFTable )



# 文本演示----------------------------------------------------------------

# 设置默认字体大小为26

options( "ReporteRs-fontsize" =26 )



# 添加一个版式为"Two Content"的幻灯片

mydoc = addSlide( mydoc, slide.layout ="Two Content" )



# 添加标题

mydoc = addTitle( mydoc, "Textsdemo" )

texts = c( "Lorem ipsum dolor sitamet, consectetur adipiscing elit."

  ,"In sit amet ipsum tellus. Vivamus dignissim arcu sit ametfaucibus auctor."

, "Quisque dictum tristiqueligula."

)



# 添加文本

mydoc = addParagraph( mydoc, value =texts  )



# 添加内容 "My tailor isrich" 和 "Cats and Dogs"

# 设置文本中某些片段的格式

pot1 = pot("My tailor" ,textProperties(color="red" ) ) + " is " +pot("rich", textProperties(font.weight="bold") )

pot2 = pot("Cats",textProperties(color="red" ) ) + " and " +pot("Dogs", textProperties(color="blue" ) )



mydoc = addParagraph(mydoc, set_of_paragraphs(pot1, pot2 ) )



writeDoc( mydoc, file ="pp_long_demo.pptx" )

关于ReporteRs的更多信息请参考[http://davidgohel.github.io/ReporteRs/index.html](http://davidgohel.github.io/ReporteRs/index.html)

本文由雪晴数据网负责翻译整理，原文请参考[CreatePowerPoint documents from R](http://davidgohel.github.io/ReporteRs/powerpoint.html)作者David Gohel。转载请注明本文链接[http://www.xueqing.cc/cms/article/118](http://www.xueqing.cc/cms/article/118)





转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-952595.html](http://blog.sciencenet.cn/blog-81613-952595.html)

上一篇：[[转载]数学有多美？看看这位法国奇男子怎么说](blog-81613-952583.html)
下一篇：[[转载]丘成桐谈几何：从黎曼、爱因斯坦到弦论](blog-81613-952603.html)


