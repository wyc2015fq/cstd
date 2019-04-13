
# 常用HTML标签2：表格和列表 - 尹成的技术博客 - CSDN博客

2018年11月14日 16:14:27[尹成](https://me.csdn.net/yincheng01)阅读数：71


\#html列表
\#\#有序列表
在网页上定义一个有编号的内容列表可以用`<ol>`、`<li>`配合使用来实现，代码如下：
`<ol>
    <li>列表文字一</li>
    <li>列表文字二</li>
    <li>列表文字三</li>
</ol>`在网页上生成的列表，每条项目上会按1、2、3编号，有序列表在实际开发中较少使用。
\#\#无序列表
在网页上定义一个无编号的内容列表可以用`<ul>`、`<li>`配合使用来实现，代码如下：
`<ul>
    <li>列表文字一</li>
    <li>列表文字二</li>
    <li>列表文字三</li>
</ul>`在网页上生成的列表，每条项目上会有一个小图标，这个小图标在不同浏览器上显示效果不同，所以一般会用样式去掉默认的小图标，如果需要图标，可以用样式自定义图标，从而达到在不同浏览器上显示的效果相同,实际开发中一般用这种列表。
\#\#定义列表
定义列表通常用于术语的定义。`<dl>`标签表示列表的整体。`<dt>`标签定义术语的题目。`<dd>`标签是术语的解释。一个`<dl>`中可以有多个题目和解释，代码如下：
`<h3>前端三大块</h3>
<dl>
    <dt>html</dt>
    <dd>负责页面的结构</dd>
    <dt>css</dt>
    <dd>负责页面的表现</dd>
    <dt>javascript</dt>
    <dd>负责页面的行为</dd>
</dl>`\#html表格
\#\#table常用标签
1、table标签：声明一个表格
2、tr标签：定义表格中的一行
3、td和th标签：定义一行中的一个单元格，td代表普通单元格，th表示表头单元格
\#\#table常用属性：
1、border 定义表格的边框
2、cellpadding 定义单元格内内容与边框的距离
3、cellspacing 定义单元格与单元格之间的距离
4、align 设置单元格中内容的水平对齐方式,设置值有：left | center | right
5、valign 设置单元格中内容的垂直对齐方式 top | middle | bottom
6、colspan 设置单元格水平合并
7、rowspan 设置单元格垂直合并
\#\#传统布局：
传统的布局方式就是使用table来做整体页面的布局，布局的技巧归纳为如下几点：
1、定义表格宽高，将border、cellpadding、cellspacing全部设置为0
2、单元格里面嵌套表格
3、单元格中的元素和嵌套的表格用align和valign设置对齐方式
4、通过属性或者css样式设置单元格中元素的样式
\#\#传统布局目前应用：
1、快速制作用于演示的html页面
2、商业推广EDM制作(广告邮件)
\#\#table布局实例（个人简历页面布局实例）：
![这里写图片描述](https://img-blog.csdn.net/20180911231614626?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MjI4NjAyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
\#\#表格常用样式属性
border-collapse:collapse 设置边框合并，制作一像素宽的边线的表格
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

