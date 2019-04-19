# 【HTML＋CSS网页设计与布局 从入门到精通】第5章-表格 - Koma Hub - CSDN博客
2019年01月06日 20:31:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：114
**目录**
[基本表格](#%E5%9F%BA%E6%9C%AC%E8%A1%A8%E6%A0%BC)
[两个单元格合二为一（横向）](#%E4%B8%A4%E4%B8%AA%E5%8D%95%E5%85%83%E6%A0%BC%E5%90%88%E4%BA%8C%E4%B8%BA%E4%B8%80%EF%BC%88%E6%A8%AA%E5%90%91%EF%BC%89)
[两个单元格合二为一（纵向）](#%E4%B8%A4%E4%B8%AA%E5%8D%95%E5%85%83%E6%A0%BC%E5%90%88%E4%BA%8C%E4%B8%BA%E4%B8%80%EF%BC%88%E7%BA%B5%E5%90%91%EF%BC%89)
[四个单元格合并（横向+纵向）](#%E5%9B%9B%E4%B8%AA%E5%8D%95%E5%85%83%E6%A0%BC%E5%90%88%E5%B9%B6%EF%BC%88%E6%A8%AA%E5%90%91%2B%E7%BA%B5%E5%90%91%EF%BC%89)
[表格大小](#%E8%A1%A8%E6%A0%BC%E5%A4%A7%E5%B0%8F)
[单元格内居中对齐](#%E5%8D%95%E5%85%83%E6%A0%BC%E5%86%85%E5%B1%85%E4%B8%AD%E5%AF%B9%E9%BD%90)
[右对齐](#%E5%8F%B3%E5%AF%B9%E9%BD%90)
[底部对齐](#%E5%BA%95%E9%83%A8%E5%AF%B9%E9%BD%90)
[表格背景颜色](#%E8%A1%A8%E6%A0%BC%E8%83%8C%E6%99%AF%E9%A2%9C%E8%89%B2)
[表格单元格边缘距](#%E8%A1%A8%E6%A0%BC%E5%8D%95%E5%85%83%E6%A0%BC%E8%BE%B9%E7%BC%98%E8%B7%9D)
[例子](#%E4%BE%8B%E5%AD%90)
[例子一](#%E4%BE%8B%E5%AD%90%E4%B8%80)
[例子二](#%E4%BE%8B%E5%AD%90%E4%BA%8C)
[表格名称caption](#%E8%A1%A8%E6%A0%BC%E5%90%8D%E7%A7%B0caption)
# 基本表格
```
<html>
   <head>
      <title>基本表格</title>
   </head>
   <body>
      <center>
      <table border=1>
             <tr>
                <td> A1</td> <td>A2</td><td>A3</td> <td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B2</td><td>B3</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 两个单元格合二为一（横向）
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1">
             <tr>
                <td> A1</td> <td colspan="2">A2A3</td> <td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B2</td><td>B3</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 两个单元格合二为一（纵向）
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1">
             <tr>
                <td> A1</td> <td rowspan="2">A2<br>B2</td> <td>A3</td> <td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B3</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 四个单元格合并（横向+纵向）
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1">
             <tr>
                <td> A1</td> <td rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 表格大小
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1" height="150" width="200">
             <tr >
                <td> A1</td> <td rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 单元格内居中对齐
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1" height="150" width="200">
             <tr >
                <td> A1</td> <td  align="center" rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr>
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 右对齐
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1" height="150" width="200">
             <tr >
                <td> A1</td> <td  align="center" rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr>
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr   align="right">
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 底部对齐
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table border="1" height="150" width="200">
             <tr >
                <td> A1</td> <td  align="center" rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr  valign="bottom">
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr   align="right">
                <td>C1</td> <td>C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 表格背景颜色
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table bgcolor="#CCCCCC" border="1" height="150" width="200">
             <tr >
                <td> A1</td> <td  align="center" rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr  valign="bottom">
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr  bgcolor="#999999"  align="right">
                <td>C1</td> <td bgcolor="#555555">C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 表格单元格边缘距
```
<html>
   <head>
      <title>表格</title>
   </head>
   <body>
      <center>
      <table bgcolor="#CCCCCC" border="1" height="150" width="200" cellpadding="4" cellspacing="6">
             <tr >
                <td> A1</td> <td  align="center" rowspan="2" colspan="2">A2A3<br>B2B3</td><td>A4</td>
             </tr>
             <tr  valign="bottom">
                 <td>B1</td> <td>B4</td>
             </tr>
             <tr  bgcolor="#999999"  align="right">
                <td>C1</td> <td bgcolor="#555555">C2</td><td>C3</td> <td>C4</td>
             </tr>
         </table>
      </center>
   </body>
</html>
```
# 例子
## 例子一
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>表格演示</title>
</head>
<body>
<table width="400" border="1" align="center" bordercolor="#003399">
<thead>
  <tr>
    <th colspan="2">产品</td>
    <th colspan="2">描述信息</td>
  </tr>
  <tr align="center">
    <td>公司</td>
    <td>编号</td>
    <td>用途</td>
    <td>价格</td>
  </tr>
  </thead>
  <tbody>
  <tr>
    <th rowspan="2">大众</td>
    <td>DZ-1</td>
    <td>中端客户</td>
    <td>100.00</td>
  </tr>
  <tr>
    <td>DZ-2</td>
    <td>低端客户</td>
    <td>50.00</td>
  </tr>
  <tr>
    <th rowspan="2">前沿</td>
    <td>JY-1</td>
    <td>高端客户</td>
    <td>200.00</td>
  </tr>
  <tr>
    <td>JY-2</td>
    <td>中端客户</td>
    <td>100.00</td>
  </tr>
 </tbody>
 <tfoot>
  <tr>
    <td>2</td>
    <td>4</td>
    <td>3</td>
    <td>120.00</td>
  </tr>
   </tfoot>
</table>
</body>
</html>
```
## 例子二
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>表格演示</title>
<style type="text/css">
thead{
	background-color:#555;
    color:white; 
	}
tfoot{
	background:#BBB;
	}
</style>
</head>
<body>
<table width="400" border="1" align="center" bordercolor="#003399">
<thead>
  <tr>
    <th colspan="2">产品</td>
    <th colspan="2">描述信息</td>
  </tr>
  <tr align="center">
    <td>公司</td>
    <td>编号</td>
    <td>用途</td>
    <td>价格</td>
  </tr>
  </thead>
  <tbody>
  <tr>
    <th rowspan="2">大众</td>
    <td>DZ-1</td>
    <td>中端客户</td>
    <td>100.00</td>
  </tr>
  <tr>
    <td>DZ-2</td>
    <td>低端客户</td>
    <td>50.00</td>
  </tr>
  <tr>
    <th rowspan="2">前沿</td>
    <td>JY-1</td>
    <td>高端客户</td>
    <td>200.00</td>
  </tr>
  <tr>
    <td>JY-2</td>
    <td>中端客户</td>
    <td>100.00</td>
  </tr>
 </tbody>
 <tfoot>
  <tr>
    <td>2</td>
    <td>4</td>
    <td>3</td>
    <td>120.00</td>
  </tr>
   </tfoot>
</table>
</body>
</html>
```
# 表格名称caption
```
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>表格演示</title>
<style type="text/css">
thead {
	background-color:#555;
	color:white;
}
tfoot {
	background:#BBB;
}
</style>
</head>
<body>
<table width="400" border="1" align="center" bordercolor="#003399">
  <caption>
  产品介绍表
  </caption>
  <thead>
    <tr>
      <th colspan="2">产品
        </td>
      <th colspan="2">描述信息
        </td>
    </tr>
    <tr align="center">
      <td>公司</td>
      <td>编号</td>
      <td>用途</td>
      <td>价格</td>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th rowspan="2">大众
        </td>
      <td>DZ-1</td>
      <td>中端客户</td>
      <td>100.00</td>
    </tr>
    <tr>
      <td>DZ-2</td>
      <td>低端客户</td>
      <td>50.00</td>
    </tr>
    <tr>
      <th rowspan="2">前沿
        </td>
      <td>JY-1</td>
      <td>高端客户</td>
      <td>200.00</td>
    </tr>
    <tr>
      <td>JY-2</td>
      <td>中端客户</td>
      <td>100.00</td>
    </tr>
  </tbody>
  <tfoot>
    <tr>
      <td>2</td>
      <td>4</td>
      <td>3</td>
      <td>120.00</td>
    </tr>
  </tfoot>
</table>
</body>
</html>
```
