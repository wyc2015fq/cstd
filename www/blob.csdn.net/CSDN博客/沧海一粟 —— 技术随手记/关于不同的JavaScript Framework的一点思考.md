# 关于不同的JavaScript Framework的一点思考 - 沧海一粟 —— 技术随手记 - CSDN博客





2015年01月12日 15:46:29[慢游](https://me.csdn.net/eengel)阅读数：608
个人分类：[JavaScript](https://blog.csdn.net/eengel/article/category/776806)









前几年一直用Dojo做应用开发，也经历了Dojo的一些大大小小的变化，比如1.6以后采用了AMD模式。应用所限，对于这些变化背后的动机，一直都没有很深入地去了解。最近半年有机会加入了一个大型应用开发项目，一下子接触了很多种不同的JavaScript Framework。个人除了Dojo之外，也上手了JQuery和d3。回首以前看过的一些文章对于这些框架优劣的争论，也有了一点自己的看法。

Dojo最大的特点就是大而全，而且从package大小来看，应该是一直在包容更多的东西进来，比如dijit, dojox。如果想从头开发一个web应用，那么用Dojo基本可以包圆了：从基本的页面元素操作、事件处理，到各种UI控件的使用、定制。而Dojo的缺点也在这里：package太大，一个项目其他代码加起来几M撑死，加入一个Dojo，少说得增加几十M。对项目的部署来说是一个不小的负担。当然也可以做build，但也有其缺陷所在。

JQuery小而强大，语法简洁、直观。用过$("#test")后，就再也不想用dom.byId("test")了，何况用dom还得require(["dojo/dom"])。更别说JQuery用起CSS Selector时的灵活了。除了页面元素操作等，一旦要做UI控件，JQuery也遇到了同样的问题：要渲染一个漂亮的对话框，没有十个八个js、css文件，实在做不出来。而JQuery没有AMD机制，就要乖乖得include这些文件了。这样比起来，Dojo只要在body中设置了相应的class，就可以让整个应用中的所有UI控件都统一风格，不太需要独立引入css，却是更好用了。

d3是专门用于画图的JavaScript Framework。倒是和Dojo，JQuery不在一个层面上。如果要比较的话，倒是可以把d3和Dojo Charting，IBM Rave，乃至五花八门的charting framework（Highcharts, Echarts, Flotchart, ...）来比。如果要画的图表比较规范，那最好就是选择Highcharts, Echarts, Flotchart等直接渲染指定图标类型的Framework。如果想做很多定制，或者要画的图表不是规范图表，那么d3就比较适合。这儿想说的是，为了更好的使用d3，用CJS模式对其进行了模块化设计。




这儿有一篇关于JavaScript模块化的文章，恰好谈到了这些不同的设计模式，可供参考：http://www.javaworld.com/article/2109734/html-css-js/code-in-javascript-the-smart-modular-way.html




