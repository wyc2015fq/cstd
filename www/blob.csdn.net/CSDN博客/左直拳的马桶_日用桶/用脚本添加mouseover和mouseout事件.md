# 用脚本添加mouseover和mouseout事件 - 左直拳的马桶_日用桶 - CSDN博客
2012年08月25日 12:32:03[左直拳](https://me.csdn.net/leftfist)阅读数：1388标签：[脚本																[function																[javascript																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)
个人分类：[JavaScript																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94500)
通常列表采用表格显示，每一行有鼠标在上面时，置亮背景，鼠标移开，则恢复原来背景，这样才够有人性。
可是页面输出的时候，如果每行都写上 onmouseover,onmouseout，再加上函数名，参数，行数一多的话，页面就比较大了，看上去也很傻。
可以采用页面输出完毕后，才用脚本将这2个事件补上去。
脚本如下：
```java
$(document).ready(function() {
    $("#tbList tr").not("#trTop").not("#trHead").mouseover(function(){ $(this).css("background-color","#f1F8F9");});
    $("#tbList tr").not("#trTop").not("#trHead").mouseout(function(){ $(this).css("background-color","white");});
 })
```
tbList是table的ID
trTop,trHead是固定背景色的行，无须这种背景置亮效果。
```
<table id="tbList" width="100%" cellpadding="0" cellspacing="0">
<tr id="trTop">
<td colspan="5">
。。。
</td>
</tr>
<tr id="trHead">
<td id="gap"></td>
<td>标题</td>
<td style="width:35px;">私密</td>
<td style="width:45px;">日期</td>
<td style="width:65px;">操作</td>
</tr>
<tr>
<td></td>
<td><span><a href="mine_show.aspx?id=19" title="内衣宠爱自己是爱情魔咒" target="_blank">内衣宠爱自己是爱情魔咒</a></span></td>
<td>公开</td>
<td>10-13</td>
<td>
    <a href='/Add.aspx?id=19'>编辑</a>
    <a href="javascript:DelIt(19)">删除</a>
</td>
</tr>
</table>
```

