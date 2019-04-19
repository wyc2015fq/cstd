# Easyui Datagrid自动调整行号大小 - BlueSky - CSDN博客
2016年04月28日 14:57:54[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：4100
相信很多人在使用easyui的时候都遇到过这个问题,当我们设置成显示Rownumber的时候,你会发现,如果Rownumber越来越大,达到三位数或者四位数的时候,Rownumber就显示不全了.这个问题纠结了不知道多少人,在群里和论坛里面都的都被问到无数次了. 以前也一直没有太认真考虑过要解决这个问题,恰好最近有点时间,就研究了一下,其实要解决也很简单,扩展一个方法就行了.
首先,从datagrid生成的代码,我们可以发现,在rowNumber上都有特定的class标记,datagrid-cell-rownumber,datagrid-header-rownumber.既然有规则可循,那么接下来的扩展就好办了......
首先,从datagrid生成的代码,我们可以发现,在rowNumber上都有特定的class标记,datagrid-cell-rownumber,datagrid-header-rownumber.既然有规则可循,那么接下来的扩展就好办了......
下面直接贴出方法的扩展代码:
|`01`|```$.extend($.fn.datagrid.methods, {```|
|`02`|`    ````fixRownumber :````function````(jq) {```|
|`03`|`        ``return``jq.each(``function````() {```|
|`04`|`            ``var````panel = $(````this``).datagrid(``"getPanel"``);`|
|`05`|`            ``//获取最后一行的number容器,并拷贝一份`|
|`06`|`            ``var````clone = $(````".datagrid-cell-rownumber"````, panel).last().clone();```|
|`07`|`            ``//由于在某些浏览器里面,是不支持获取隐藏元素的宽度,所以取巧一下`|
|`08`|`            ``clone.css({`|
|`09`|`                ``"position"``: ``"absolute"``,`|
|`10`|`                ````left : -1000```|
|`11`|`            ``}).appendTo(``"body"``);`|
|`12`|`            ``var````width = clone.width(````"auto"``).width();`|
|`13`|`            ``//默认宽度是25,所以只有大于25的时候才进行fix`|
|`14`|`            ``if````(width > 25) {```|
|`15`|`                ``//多加5个像素,保持一点边距`|
|`16`|`                ``$(``".datagrid-header-rownumber,.datagrid-cell-rownumber"````, panel).width(width + 5);```|
|`17`|`                ``//修改了宽度之后,需要对容器进行重新计算,所以调用resize`|
|`18`|`                ``$(``this``).datagrid(``"resize"``);`|
|`19`|`                ``//一些清理工作`|
|`20`|`                ``clone.remove();`|
|`21`|`                ````clone =````null``;`|
|`22`|`            ``} ``else``{`|
|`23`|`                ``//还原成默认状态`|
|`24`|`                ``$(``".datagrid-header-rownumber,.datagrid-cell-rownumber"````, panel).removeAttr(````"style"``);`|
|`25`|`            ``}`|
|`26`|`        ``});`|
|`27`|`    ``}`|
|`28`|`});`|
有了方法,那要怎么使用,在什么时候使用呢?答案是:onLoadSuccess事件,所以就有如下,
|`1`|`$(``"#easyui-datagrid"``).datagrid({`|
|`2`|`    ````onLoadSuccess :````function````() {```|
|`3`|`        ``$(``this``).datagrid(``"fixRownumber"``);`|
|`4`|`    ``}`|
|`5`|`});`|
到此,就基本已经大功告成,你可以试下效果了.....
