
# HTML表格标签 - 李昆鹏的博客 - CSDN博客


2018年05月08日 16:04:24[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：250


---------------------------------HTML表格标签---------------------------------
HTML中的表格
表格标签：
Table
属性：
Border表格的边框
Width表格的宽度
既可以给像素，还可以给百分比
Height：表格的高度
Align水平位置  left、center、right
子标签：
Caption表格的标题
表格的行标签：
属性：
子标签：
th标题列标签，加粗并居中
td每一行的列标签，单元格，默认是居左
表格边框设置
Border:表格的外边框粗细
Cellspacing:表格内边框粗细，单元格之间的间距
Cellpadding：设置文字到单元格的距离
表格相关颜色的设置：
Bordercolor：边框的颜色
Bgcolor：背景色
表格的内容的位置设置
Align：文字的水平位置 left，center，right
Valign：文字的垂直位置 top，middle，bottom（一般不常用）
表格中单元格的合并
Colspan：列合并
Rowspan:行合并
代码示例：
html>
<head>
<title>www.likunpeng.comm</title>
</head>
<body>
<h1>HTML表格标签</h1>
<table  border="10"  width="500" height="30%" align="center"
cellspacing="0" cellpadding="30" bordercolor="red"
bgcolor="\#1979CA">
<caption>梁山好汉</caption>
<tr>
<th>姓名</th>
<th>地址</th>
<th>年龄</th>
</tr>
<tr  valign="middle">
<tdalign="center">宋江</td>
<td  align="center">山东郓城</td>
<td  align="center">32</td>
</tr>
<tr  align="center" valign="middle">
<td>晁盖</td>
<td>石洁村</td>
<td>35</td>
</tr>
<tr  align="center" valign="middle">
<td>武松</td>
<td>阳谷县</td>
<td>26</td>
</tr>
</table>
</body>
</html>

---------------------------------------------------
**HTML****表格标签**
![](https://img-blog.csdn.net/20180508160333510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MTU0NzQ4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



