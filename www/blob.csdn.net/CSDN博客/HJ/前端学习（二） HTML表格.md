# 前端学习（二） HTML表格 - HJ - CSDN博客
2018年12月02日 16:45:34[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：106
### 一、表格的基本介绍
1、表格的基本元素：表格、行、单元格
<table> 表格
<tr> 行
<td> 普通单元格 
<th> 表头单元格(有加粗和居中效果)
2、基本部分：表头、表格主体、表格脚注
<thead>表格的头（放标题之类的内容）
<tbody>表格的主题（放数据主体）
<tfoot>表格的脚（放表格脚注）
样例如下： 
```
<table>
   <caption>...</caption>
   <thead>
    <tr>
     <th>表头</th>
    </tr>
   </thead>
   <tbody>
    <tr>
     <td>主体</td>
    </tr>
   </tbody>
   <tfoot>
    <tr>
     <td>脚注</td>
    </tr>
   </tfoot>
</table>
```
### 二、表格的属性
1、table属性：
width：规定表格的宽度(pixels、%)；
align：表格相对周围元素的对齐方式(left、center、ritht)；
border：规定表格边框的宽度(pixels)；
bgcolor：表格的背景颜色；
cellpadding：单元边沿与其内容之间的空白；
cellspacing：单元格之间的空白；
frame：规定外侧边框的哪个部分是可见的；
```
void：不显示外侧边框
    above：显示上部的外侧边框
    below：显示下部的外侧边框
    hsides：显示上部和下部的外侧边框
    vsides：显示左边和右边的外侧边框    
    lhs：显示左边的外侧边框
    rhs：显示右边的外侧边框
    box：在所有四个边上显示外侧边框
    border：在所有四个边上显示外侧边框
```
rules：规定内侧边框的哪个部分是可见的；
```
none：没有线条
    groups：位于行组合列组之间的线条
    rows：位于行之间的线条
    cols：位于列之间的线条
    all：位于行和列之间的线条
```
2、tr属性：
```
align：行内容的水平对齐(left、center、right、justify、char);
valign：行内容的垂直对齐(top、middle、bottom、baseline)；
bgcolor：行的背景颜色；
```
3、td和th属性：
`align、valign、bgcolor、width、height`
### 三、表格嵌套以及跨行跨列操作
跨列属性：colspan
跨行属性：rowspan
表格嵌套：注意，嵌入的表格必须是完整的表结构，并且只能放入td标签中
下面给出完整一个表格的样例应用供大家学习参考：
```
<table align="center" border="6" width="500px" bgcolor="#f2f2f2" cellspacing="0" cellpadding="5px" frame="box">   <!-- 表格开始 -->
   <caption>前端工程师工资</caption>
   <thead align="center" valign="middle">
    <tr bgcolor="#d8e4bc">  
     <th rowspan="2">城市</th>
     <th colspan="2">2016年</th>
     <th rowspan="2">2017年</th>
     <th rowspan="2">2018年</th>
    </tr>
    <tr bgcolor="#d8e4bc"> 
     <th>上半年</th>
     <th>下半年</th>
    </tr>
   </thead>
   <tbody align="center" valign="middle">
    <tr>
     <td bgcolor="#b8cce4">北京</td>
     <td>8000</td>
     <td>9000</td>
     <td>10000</td>
     <td>12000</td>
    </tr>
    <tr>
     <td bgcolor="#b8cce4">上海</td>
     <td>6000</td>
     <td>7000</td>
     <td>8000</td>
     <td>10000</td>
    </tr>
   </tbody>
   <tfoot align="center" valign="middle">
    <tr>
     <td bgcolor="#b8cce4">合计</td>
     <td>9000</td>
     <td>8000</td>
     <td>9000</td>
     <td>11000</td>
    </tr>
   </tfoot>
  </table>  <!-- 表格结束 -->
```
展示图：
![](https://img-blog.csdnimg.cn/20181202164458131.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==,size_16,color_FFFFFF,t_70)
