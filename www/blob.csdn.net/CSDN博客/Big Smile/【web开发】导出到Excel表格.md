# 【web开发】导出到Excel表格 - Big Smile - CSDN博客
2016年08月07日 23:25:01[王啸tr1912](https://me.csdn.net/tr1912)阅读数：2590
本文所需开发文件及其例子下载地址：[http://download.csdn.net/detail/tr1912/9597549](http://download.csdn.net/detail/tr1912/9597549)
        众所周知的是我们在日常的开发中少不了的是和数据打交道，这个时候就需要对于数据进行汇总整理，或者是用工具导出方便统计，这个时候就用到了导出到Excel这个功能。
# 一、插件的选择
        导出到Excel有两种方式，一种是用asp.net自带的方法，一种是用第三方的控件来导出，由于浏览器的限制和本地Excel的是否安装的限制，所以，我选择了网上推荐的Nopi控件，这个控件的最大的优点就是可以在任何系统上导出Office 97-2013文件格式，至少能够支持Word, PowerPoint, Excel, Visio的文件，实现了随意导出的结果。
# 二、导出Excel的实现
        首先来看一下不用控件的导出方法代码：
```
public void CreateExcel(DataSet ds,string typeid,string FileName)  
  { 
   HttpResponse resp; 
   resp = Page.Response; 
   resp.ContentEncoding = System.Text.Encoding.GetEncoding("GB2312"); 
   resp.AppendHeader("Content-Disposition", "attachment;filename=" + FileName);    
   string colHeaders= "", ls_item=""; 
   int i=0; 
 
   //定义表对象与行对像，同时用DataSet对其值进行初始化 
   DataTable dt=ds.Tables[0]; 
   DataRow[] myRow=dt.Select("");  
   // typeid=="1"时导出为EXCEL格式文件；typeid=="2"时导出为XML格式文件 
   if(typeid=="1") 
   { 
    //取得数据表各列标题，各标题之间以\t分割，最后一个列标题后加回车符 
    for(i=0;i     colHeaders+=dt.Columns[i].Caption.ToString()+"\t"; 
    colHeaders +=dt.Columns[i].Caption.ToString() +"\n";    
    //向HTTP输出流中写入取得的数据信息 
    resp.Write(colHeaders);  
    //逐行处理数据   
    foreach(DataRow row in myRow) 
    { 
     //在当前行中，逐列获得数据，数据之间以\t分割，结束时加回车符\n 
     for(i=0;i      ls_item +=row[i].ToString() + "\t";      
     ls_item += row[i].ToString() +"\n"; 
     //当前行数据写入HTTP输出流，并且置空ls_item以便下行数据     
     resp.Write(ls_item); 
     ls_item=""; 
    } 
   } 
   else 
   { 
    if(typeid=="2") 
    {  
     //从DataSet中直接导出XML数据并且写到HTTP输出流中 
     resp.Write(ds.GetXml()); 
    }     
   } 
   //写缓冲区中的数据到HTTP头文件中 
   resp.End(); 
  }
```
        大家可以看出来，他们用的都是web上面的对象和方法来临时创建一个字符串的拼接类型的方法，然后用response响应方法接收，最后用.net自带的write方法写出文件。这样做是不是十分的麻烦？
用Nopi之后的代码：
```
public void NpoiExcel(DataTable dt, string title)
        {
            NPOI.HSSF.UserModel.HSSFWorkbook book = new NPOI.HSSF.UserModel.HSSFWorkbook();
            NPOI.SS.UserModel.ISheet sheet = book.CreateSheet("Sheet1");    //创建表
            NPOI.SS.UserModel.IRow headerrow = sheet.CreateRow(0);    //创建行
            ICellStyle style = book.CreateCellStyle();        //设置样式
            style.Alignment = HorizontalAlignment.CENTER;
            style.VerticalAlignment = VerticalAlignment.CENTER;
            //写表头
            for (int i = 0; i < dt.Columns.Count; i++)
            {
                ICell cell = headerrow.CreateCell(i);     //在行上添加格
                cell.CellStyle = style;
                if (dt.Columns[i].ColumnName=="cardId")
                {
                    cell.SetCellValue("卡号");
                }
                else if (dt.Columns[i].ColumnName == "ownerName")
                {
                    cell.SetCellValue("持卡人");
                }
                else if (dt.Columns[i].ColumnName == "orderid")
                {
                    cell.SetCellValue("订单编号");
                }
                else if (dt.Columns[i].ColumnName == "totalcost")
                {
                    cell.SetCellValue("总金额");
                }
                else if (dt.Columns[i].ColumnName == "orderTime")
                {
                    cell.SetCellValue("订餐时间");
                }
            }
            //写数据
            for (int i = 0; i < dt.Rows.Count; i++)
            {
                IRow row = sheet.CreateRow(i+1);       //创建行
                for (int j = 0; j < dt.Columns.Count; j++)
                {
                    ICell cell = row.CreateCell(j);
                    cell.SetCellValue(dt.Rows[i][j].ToString());
                }
            }
            MemoryStream ms = new MemoryStream();
            book.Write(ms);
            Response.AddHeader("Content-Disposition", string.Format("attachment; filename={0}.xls", HttpUtility.UrlEncode(title + "_" + DateTime.Now.ToString("yyyy-MM-dd"), System.Text.Encoding.UTF8)));
            Response.BinaryWrite(ms.ToArray());
            Response.End();
            book = null;
            ms.Close();
            ms.Dispose();
        }
```
        是不是感觉用专用对象进行操作比用web对象操作明白多了，下面我来介绍一下如何用Nopi的控件。
# 三、Nopi的使用
        首先就是对于Nopi的文件支持，如图：
![](https://img-blog.csdn.net/20160807223630743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)文件可以从最上方下载。
        然后就是命名空间的使用：
```
using System.Data;
using NPOI;
using NPOI.HPSF;
using NPOI.HSSF;
using NPOI.HSSF.UserModel;
using NPOI.SS.UserModel;
using System.IO;
```
中间的六个就是Nopi自带的引用，然后要加上IO引用用来输出。
        1、创建一个Excel表：
```
HSSFWorkbook wk = new HSSFWorkbook();  
//创建一个Sheet  
ISheet sheet = wk.CreateSheet("例子");
```
        2、创建表格
```
/在第一行创建行  
IRow row = sheet.CreateRow(0);  
//在第一行的第一列创建单元格  
ICell cell = row.CreateCell(0);
```
        3、对表格赋值
`cell.SetCellValue("测试");  `
值得注意的是，SetCellValue的重载中支持
    bool
    DateTime
    double
    string
    IRichTextString
        4、可以选择保存到硬盘上
```
//打开一个xls文件，如果没有则自行创建，如果存在myxls.xls文件则在创建时不要打开该文件  
using (FileStream fs = File.OpenWrite("d:\\excel.xls"))  
{  
    wk.Write(fs);//向打开的这个xls文件中写入并保存。  
}
```
        5、可以选择网页下载
```
MemoryStream ms = new MemoryStream();    //存储池
            book.Write(ms);             //生成文件
            Response.AddHeader("Content-Disposition", string.Format("attachment; filename={0}.xls", HttpUtility.UrlEncode(title + "_" + DateTime.Now.ToString("yyyy-MM-dd"), System.Text.Encoding.UTF8)));
            Response.BinaryWrite(ms.ToArray());      //把文件写入，写入文件名，然后让用户下载
            Response.End();               //一系列释放资源
            book = null;
            ms.Close();
            ms.Dispose();
```
          注：亲们注意到了，在两种输出方式中除了调用的系统资源外，系统没有释放Nopi的资源，这个是因为Nopi自带资源释放的功能，能够有效的提高使用的效果。
# 四、总结
         经过这段时间的学习，发现了Excel的导出和C/S版的程序差别还是很大的，在某些方面也强大了不少，而大体的思路确实不变的，我们实现功能的时候一定要理清楚思路在行动。
