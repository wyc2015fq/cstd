# HTML5微数据初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月03日 16:57:25[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2613








                
1、概念

   微数据是在如 span、div 的标签内添加属性，让机器（如搜索引擎）识别其意义，一些特定类型的信息，例如评论、人物信息或事件都有相应的属性，用来描述其意义。

   HTML5 微数据规范是一种标记内容以描述特定类型的信息，例如评论、人物信息或事件。每种信息都描述特定类型的项，例如人物、事件或评论。例如，事件可以包含 venue、starting time、name 和 category 属性。

   微数据使用 HTML 标记（常为 span 或 div）中的简单属性为项和属性指定简要的描述性名称。



2、格式

   1）itemscope：声明你所要添加的属性的作用域，加在最外层的元素。

　 2）itemtype：声明所使用的词汇表，加在最外层的元素中。

　 3）itemprop：声明属性名，具体的内容要参加所引用的词汇表。



3、示例

   1）简单的HTML 

```
<div>
    <span>Kayo</span>
    <span>2012年2月6日 22:30</span>
    <span>评论内容</span>
</div>
```



   2)微数据标记的HTML

```
<div itemscope itemtype="http://data-vocabulary.org/Review">
    <span itemprop="reviewer">Kayo</span>
    <span itemprop="dtreviewed" datatime="2012-02-06T22:30:30.50+08:00">2012年2月6日 22:30</span>
    <span itemprop="description">评论内容</span>
</div>
```


   说明：itemscop 表示该 div 中描述了某一项微数据，itemtype="http://data-vocabulary.org/Review" 表示该微数据项类型是一条评论；

         具体的数据项都用 itemprop 属性标识，如 "reviewer" 表示评论者， "dtreviewed" 表示评论时间，其中 datatime 属性标识了该评论的具体评论时间，属性值必须使用 ISO 日期格式描述一个日期，另外为了使用户易于理解，网页上可以用常见的时间格式标识时间。



4、语法

   在一项微数据的最外层 div ，需要用 itemscop 标识，并用 itemtype 指定微数据类型， itemtype 的值是一个 url ，用于引入一个微数据词汇表（即各种微数据的类型定义），可以使用 http://schema.org 或是 http://data-vocabulary.org ，具体的微数据类型可以参考上面两套微数据词汇表。

   在用 itemscop 标识了的元素内使用 itemprop 属性添加微数据，itemprop 的值为 HTML 标签包含的内容（如例子中的 reviewer 和 description ），又或是标签的属性值（如例子中的 dtreviewed，取 datatime 的值为属性值）。



5、深入：

   参考:http://wiki.jikexueyuan.com/project/html5/microdata.html 
            


