# 不去琢磨什么CSS后代选择器之类的鸟玩意了 - 左直拳的马桶_日用桶 - CSDN博客
2011年08月31日 11:58:28[左直拳](https://me.csdn.net/leftfist)阅读数：1131标签：[css																[border																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=border&t=blog)](https://so.csdn.net/so/search/s.do?q=css&t=blog)
个人分类：[Web](https://blog.csdn.net/leftfist/article/category/674421)
酱紫写了以后，外层表格有边框；里层没有。
不要去琢磨什么 #tbTese>td之类的鸟玩意了。
<style type="text/css">
    #tbNo1 td{border:solid 1px #E6E6E6;height:200px;}
    #tbNo1td td{border:none;}
</style>
<table id="tbNo1" style="width:100%;" cellpadding="0" cellspacing="5" border="0">
    <tr>
<td>
        <table>
            <tr><td><img src="/img/teseicon.jpg" alt="" /></td></tr>
        </table>
    </td>
    <td></td><td></td><td></td></tr>
    <tr><td></td><td></td><td></td><td></td></tr>
</table>
