# 用POI导出excel时，较长的数字不想被自动变为科学计数法的解决方式 - z69183787的专栏 - CSDN博客
2015年08月31日 14:43:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15674
做过很多次导出excel了。都碰到一个问题，内容里如果包含一个比较长的数字，比如订单号“2546541656596”，excel会自动变成科学计数法。。。 
弄过好几次都没有解决，最近又要导出excel了，下决心一定要找到解决办法 
自己在excel里测试过，较长的数字，都会自动变成科学计数法，除非我们把单元格格式设置成“文本型” 
哈哈，似乎找到了思路：用poi先把单元格设置成“文本型”就可以了把。 
从文档里找到了这个方法HSSFCell.setCellType(int type)，怎么看这个方法都可以设置单元格格式。 
代码中加入cell.setCellType(HSSFCell.CELL_TYPE_STRING)，还是没有任何变化。。。 
又一次陷入无助状态 
从google搜索了半天，找到了这篇文章 
http://javacrazyer.iteye.com/blog/894758，博主写的非常详细，从原理到解决方法都有了 
思路还是那样，设置单元格格式为“文本型” 
下面直接阐述下真正的解决方法： 
1 //创建workBook 
2  HSSFWorkbook wb = new HSSFWorkbook(); 
3  //创建一个样式 
4  HSSFCellStyle cellStyle = wb.createCellStyle(); 
5 //创建一个DataFormat对象 
6 HSSFDataFormat format = wb.createDataFormat(); 
7 //这样才能真正的控制单元格格式，@就是指文本型，具体格式的定义还是参考上面的原文吧 
8 cellStyle.setDataFormat(format.getFormat("@")); 
9 
10 //具体如何创建cell就省略了，最后设置单元格的格式这样写 
11 cell.setCellStyle(cellStyle);
当我们把web页面上的数据导成excel形式时，有时候我们的数据需要以特定的格式呈现出来，这时候我们就需要给cell添加一些样式规格信息。
 首先，我们了解一下excel从web页面上导出的原理。当我们把这些数据发送到客户端时，我们想让客户端程序（浏览器）以excel的格式读取它，所以把mime类型设为：application/vnd.ms-excel，当excel读取文件时会以每个cell的格式呈现数据，如果cell没有规定的格式，则excel会以默认的格式去呈现该cell的数据。这样就给我们提供了自定义数据格式的空间，当然我们必须使用excel支持的格式。下面就列出常用的一些格式：
1）  文本：vnd.ms-excel.numberformat:@
2）  日期：vnd.ms-excel.numberformat:yyyy/mm/dd
3）  数字：vnd.ms-excel.numberformat:#,##0.00
4）  货币：vnd.ms-excel.numberformat:￥#,##0.00
5）  百分比：vnd.ms-excel.numberformat: #0.00%
这些格式你也可以自定义，比如年月你可以定义为：yy-mm等等。那么知道了这些格式，怎么去把这些格式添加到cell中呢？很简单，我们只需要把样式添加到对应的标签对（即闭合标签）即可。如<td></td>，给标签对<td></td>添加样式，如下：
view plaincopy to clipboardprint?
<td style="vnd.ms-excel.numberformat:@">410522198402161833</td>  
<td style="vnd.ms-excel.numberformat:@">410522198402161833</td>
同样，我们也可以给<div></div>添加样式，也可以给<tr></tr>，<table></table>添加样式，这样就会引入一个问题，你注意到了吗？先看如下的代码：
view plaincopy to clipboardprint?
<table style=’vnd.ms-excel.numberformat:#,##0.00’>  
   <tr>  
      <td>542</td>  
       <td style=’vnd.ms-excel.numberformat: #0.00%’>0.25</td>  
    </tr>  
</table>  
<table style=’vnd.ms-excel.numberformat:#,##0.00’>
    <tr>
        <td>542</td>
        <td style=’vnd.ms-excel.numberformat: #0.00%’>0.25</td>
    </tr>
</table>
