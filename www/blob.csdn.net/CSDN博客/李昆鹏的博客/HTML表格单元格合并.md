
# HTML表格单元格合并 - 李昆鹏的博客 - CSDN博客


2018年05月08日 16:08:41[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：512


-------------------------------------HTML表格单元格合并-------------------------------
**表格中单元格的合并**
**Colspan****：列合并**
**代码示例：**
**<html>**
**<head>**
**<title>www.likunpeng.com</title>**
**</head>**
**<body>**
**<h1>单元格合并</h1>**
**<hr/>**
**<table border="1"align="center" width="60%" cellspacing="0">**
**<tralign="center">**
**<tdcolspan="2">惊悚类</td>**
**<tdcolspan="2">剧情类</td>**
**</tr>**
**<tr>**
**<td>电锯惊魂</td>**
**<td>德州电锯杀人狂</td>**
**<td>放牛班的春天</td>**
**<td>奔腾时代</td>**
**</tr>**
**<tr>**
**<td>午夜食人列车</td>**
**<td>隔山有眼</td>**
**<td>最快的摩托车手</td>**
**<td>鲁迪传奇</td>**
**</tr>**
**</table>**
**</body>**
**-------------------------------**
**单元格合并**
---

|惊悚类
|剧情类
|
|电锯惊魂
|德州电锯杀人狂
|放牛班的春天
|奔腾时代
|
|午夜食人列车
|隔山有眼
|最快的摩托车手
|鲁迪传奇
|

**Rowspan:****行合并**
代码示例：
<html>
<head>
<title>www.likunpeng.com</title>
</head>
<body>
<h1>单元格合并行</h1>
<hr/>
<table  border="1" align="center" width="60%"cellspacing="0">
<tr>
<td  rowspan="4" width="100" height="200"align="center">惊悚类</td>
<td>电锯惊魂</td>
</tr>
<tr>
<td>德州电锯杀人狂</td>
</tr>
<tr>
<td>午夜食人列车</td>
</tr>
<tr>
<td>隔山有眼</td>
</tr>
<tr>
<tdrowspan="4">剧情类</td>
<td>最快的摩托车手</td>
</tr>
<tr>
<td>鲁迪传奇</td>
</tr>
<tr>
<td>放牛班的春天</td>
</tr>
<tr>
<td>奔腾时代</td>
</tr>
</table>
</body>
</html>
-----------------------------------------------------------
**单元格合并行**
---

|惊悚类
|电锯惊魂
|
|德州电锯杀人狂
|
|午夜食人列车
|
|隔山有眼
|
|剧情类
|最快的摩托车手
|
|鲁迪传奇
|
|放牛班的春天
|
|奔腾时代
|



