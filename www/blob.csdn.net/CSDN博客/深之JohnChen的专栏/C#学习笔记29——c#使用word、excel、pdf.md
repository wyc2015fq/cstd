# C#学习笔记29——c#使用word、excel、pdf - 深之JohnChen的专栏 - CSDN博客

2011年09月04日 14:10:36[byxdaz](https://me.csdn.net/byxdaz)阅读数：4628标签：[c#																[excel																[object																[adobe																[application																[microsoft](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=adobe&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


一、C# Word操作
引入Word COM组件
菜单=》项目=》添加引用=》COM=》Microsoft Word 11.0 Object Library
using Word =  Microsoft.Office.Interop.Word;
1、功能：将数据以自制表格形式插入WORD中
2、主要程序代码如下：
创建新Word
object oMissing = System.Reflection.Missing.Value;
Word.Application oWord;
Word.Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
ref oMissing, ref oMissing);

打开文档:
object oMissing = System.Reflection.Missing.Value;
Word.Application oWord;
Word.Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
object fileName = @"E:CCCXCXXTestDoc.doc";
oDoc = oWord.Documents.Open(ref fileName,
ref oMissing, ref oMissing, ref oMissing, ref oMissing, ref oMissing,
ref oMissing, ref oMissing, ref oMissing, ref oMissing, ref oMissing,
ref oMissing, ref oMissing, ref oMissing, ref oMissing, ref oMissing);

导入模板
object oMissing = System.Reflection.Missing.Value;
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
object fileName = @"E:XXXCCXTest.doc";
oDoc = oWord.Documents.Add(ref fileName, ref oMissing,
ref oMissing, ref oMissing);

.添加新表
object oMissing = System.Reflection.Missing.Value;
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
ref oMissing, ref oMissing);

object start = 0;
object end = 0;
Word.Range tableLocation = oDoc.Range(ref start, ref end);
oDoc.Tables.Add(tableLocation, 3, 4, ref oMissing, ref oMissing);

.表插入行
object oMissing = System.Reflection.Missing.Value;
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
ref oMissing, ref oMissing);

object start = 0;
object end = 0;
Word.Range tableLocation = oDoc.Range(ref start, ref end);
oDoc.Tables.Add(tableLocation, 3, 4, ref oMissing, ref oMissing);

Word.Table newTable = oDoc.Tables[1];
object beforeRow = newTable.Rows[1];
newTable.Rows.Add(ref beforeRow);

.单元格合并
object oMissing = System.Reflection.Missing.Value;
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
ref oMissing, ref oMissing);

object start = 0;
object end = 0;
Word.Range tableLocation = oDoc.Range(ref start, ref end);
oDoc.Tables.Add(tableLocation, 3, 4, ref oMissing, ref oMissing);

Word.Table newTable = oDoc.Tables[1];
object beforeRow = newTable.Rows[1];
newTable.Rows.Add(ref beforeRow);

Word.Cell cell = newTable.Cell(1, 1);
cell.Merge(newTable.Cell(1, 2));

.单元格分离
object oMissing = System.Reflection.Missing.Value;
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add( oMissing,
ref oMissing, ref oMissing);

object start = 0;
object end = 0;
Word.Range tableLocation = oDoc.Range(ref start, ref end);
oDoc.Tables.Add(tableLocation, 3, 4, ref oMissing, ref oMissing);

Word.Table newTable = oDoc.Tables[1];
object beforeRow = newTable.Rows[1];
newTable.Rows.Add(ref beforeRow);

Word.Cell cell = newTable.Cell(1, 1);
cell.Merge(newTable.Cell(1, 2));

object Rownum = 2;
object Columnnum = 2;
cell.Split(ref Rownum, ref Columnnum);

通过段落控制插入
object oMissing = System.Reflection.Missing.Value;
object oEndOfDoc = "\endofdoc"; /**//* endofdoc is a predefinedbookmark */

//Start Word and create a new document.
Word._Application oWord;
Word._Document oDoc;
oWord = new Word.Application();
oWord.Visible = true;
oDoc = oWord.Documents.Add(ref oMissing, ref oMissing,
ref oMissing, ref oMissing);

//Insert a paragraph at the beginning of the document.
Word.Paragraph oPara1;
oPara1 = oDoc.Content.Paragraphs.Add(ref oMissing);
oPara1.Range.Text = "Heading 1";
oPara1.Range.Font.Bold = 1;
oPara1.Format.SpaceAfter = 24; //24 ptspacing after paragraph.
oPara1.Range.InsertParagraphAfter();

**[C#word类库](http://www.cnblogs.com/yuxuetaoxp/archive/2010/01/14/1647873.html):**

[http://www.cnblogs.com/yuxuetaoxp/archive/2010/01/14/1647873.html](http://www.cnblogs.com/yuxuetaoxp/archive/2010/01/14/1647873.html)

二、C# Excel操作

引入Excel COM组件
菜单=》项目=》添加引用=》COM=》Microsoft Excel 11.0 Object Library
using Excel = Microsoft.Office.Interop.Excel;

/// <summary>
/// 执行完Excel操作方法后执行资源回收操作,必须与Excel操作的方法分开
/// </summary>
private void ExcelWrite()
{
try
{
ExcelOperation();
}
finally
{
System.GC.Collect();
} 
}

/// <summary>
/// Excel操作,只是例子
/// </summary>
private void ExcelOperation()
{
//
//定义一个缺少的object对象 
object oMis = System.Reflection.Missing.Value;

//
//定义一个Excel程序对象 
Excel.ApplicationClass excelApp = newMicrosoft.Office.Interop.Excel.ApplicationClass();

//
//由Execl程序创建一个工作薄对象 
Excel.Workbook excelWorkBook =excelApp.Workbooks.Add(Excel.XlWBATemplate.xlWBATWorksheet);

//
//由工作薄对象创建一个工作表 
Excel.Worksheet excelWorkSheet =(Excel.Worksheet)excelWorkBook.Worksheets.Add(oMis, oMis, 1, oMis);

//
//设置工作的表的名字 
excelWorkSheet.Name = "测试项目";

//
//定义一个Excel区域对象,用于保存选择的区域
Excel.Range selectRange;

//
//设置第1行第1列的值
((Excel.Range)excelWorkSheet.Cells[1, 1]).set_Item(1, 1, "日期");

//
//设置第1行第2列的值
((Excel.Range)excelWorkSheet.Cells[1, 1]).set_Item(1, 2, "时间");

//
//选择第2行第2列至第2行第2列,设置值与格式
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[2, 2],excelWorkSheet.Cells[2, 2]);
selectRange.set_Item(1, 1, "11:30");
selectRange.NumberFormatLocal = @"h:mm;@";

//
//选择第3行第2列至第2行第2列,设置值与格式
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[3, 2],excelWorkSheet.Cells[3, 2]);
selectRange.set_Item(1, 1, "12:30");
selectRange.NumberFormatLocal = @"h:mm;@";

//
//选择第4行第2列至第2行第2列,设置值与格式
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[4, 2],excelWorkSheet.Cells[4, 2]);
selectRange.set_Item(1, 1, "13:30");
selectRange.NumberFormatLocal = @"h:mm;@";

//
//选择第5行第2列至第2行第2列,设置值与格式
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[5, 2],excelWorkSheet.Cells[5, 2]);
selectRange.set_Item(1, 1, "14:30");
selectRange.NumberFormatLocal = @"h:mm;@";

//
//选择第6行第2列至第2行第2列,设置值与格式
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[6, 2], excelWorkSheet.Cells[6,2]);
selectRange.set_Item(1, 1, "15:30");
selectRange.NumberFormatLocal = @"h:mm;@";

//
//选择第2行第1列至第2行第1列,
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[2, 1],excelWorkSheet.Cells[2, 1]);
//
//选择是必须的,因设置了选择区域后并未选中此区域 
selectRange.Select();

//
//冻结窗体
excelApp.ActiveWindow.FreezePanes = true;

//
//选择第2行第1列至第6行第1列,
selectRange = excelWorkSheet.get_Range(excelWorkSheet.Cells[2, 1],excelWorkSheet.Cells[6, 1]); 

//
//合并单元格
selectRange.Merge(oMis);

selectRange.EntireColumn.AutoFit(); //全部列自适应宽度
selectRange.EntireRow.AutoFit(); //全部行自适应高度

selectRange.HorizontalAlignment =Excel.XlHAlign.xlHAlignCenter;//水平居中
selectRange.VerticalAlignment = Excel.XlVAlign.xlVAlignCenter; //垂直居中

//
//设置单元格的格式
selectRange.NumberFormatLocal = @"yyyy-m-d;@";
/*
NumberFormatLocal = "￥#,##0.00_);[红色](￥#,##0.00)"//货币 
NumberFormatLocal = "0.00%" //百分比
NumberFormatLocal = "# ??/??" //分数
* */

//
//设置单元格的值
selectRange.set_Item(1, 1, "2007-02-27");

//selectRange.ColumnWidth = 50; //固定宽度
selectRange.Columns.AutoFit(); //自适应宽度

selectRange.Borders.LineStyle =BorderStyle.FixedSingle; //设置边框样式
selectRange.Borders.Weight = Excel.XlBorderWeight.xlThin; //边框粗细
selectRange.Borders.ColorIndex = Excel.XlColorIndex.xlColorIndexAutomatic; //颜色 

excelWorkBook.Saved = true; //将工作薄的保存标志置为True

//捕捉异常,以防止文件为只读或已打开,保存时会出错
try
{
excelWorkBook.SaveCopyAs(@"d:/aa.xls");
}
catch(Exception exp)
{
MessageBox.Show(exp.Message);
}

//Excel程序退出,注意:要在外面释放内存资源
excelApp.Quit();
} 

三、C# pdf操作

Adobe 可移植文档格式 (PDF) 由 Adobe Systems 创立, 并在超过 15 年的时间里日甄完善。使用 Adobe 可移植文档格式, 可以捕获和查看来自任何应用程序、任何计算机系统的丰富强大的信息, 并将这些信息与全球各地的任何人共享。 世界各地的个人、企业和政府机构都很信赖并依靠 Adobe® PDF 来交流思想和视野。

将信息解放, 思想自由交流

开放格式 — 进行更安全可靠的电子信息交流的事实标准, 已为世界各地的各个行业部门和政府机构所认可。 符合行业标准 (包括 PDF/A、PDF/X 和 PDF/E)。

多平台 — 可在任何平台 (Macintosh、Microsoft® Windows®、UNIX® 和许多移动平台) 上进行查看和打印。

可扩展性 — 全世界有超过 1,800 家供应商提供基于 PDF 的解决方案, 包括生成工具、插件、咨询、培训和支持工具。

可信可靠 — 现今网络上的超过 2 亿份 PDF 文档, 证明依靠 Adobe PDF 来捕获信息的组织机构数量可观。

维护信息完整性 — 无论通过什么应用程序创建, Adobe PDF 文件在外观上都与原始文件完全一致, 并且会保留源文件 (文本、绘图、3D、全色图像、照片, 甚至业务逻辑) 的信息。

保持信息安全性 — 通过 [Adobe Acrobat® 7.0](http://www.adobe.com/cn/products/acrobatpro/) 或 [Adobe LiveCycle™](http://www.adobe.com/cn/products/server/) 软件创建的 Adobe PDF 文件可以进行数字签名或使用密码保护。

可搜索性 — 利用全文本搜索功能查找文档中的词语、书签和数据域。

可访问性 — AdobePDF 文档支持辅助技术, 可帮助残障人士访问信息。

在实际开发中, PDF文档创建我们一般会采用开源的 iText# (iTextSharp), 当然也有一些商用专门组件可以选择.

iTextSharp

iText# (iTextSharp) is a port of the iTextopen source java library written entirely in C# for the .NET platform. iText#is a library that allows you to generate PDF files on the fly. It isimplemented as an assembly. 

[iText inAction 第一章及第三章免费下载地址](http://www.cnblogs.com/hardrock/archive/2006/11/28/575020.html)

[iText 实践的目录(thecontent of iText in Action)](http://www.cnblogs.com/hardrock/archive/2006/08/16/478565.html) 摘要: 该书是iText的作者Bruno Lowagie创作，将由Manning Publications Co. 于2006-11-01出版。in Action系列图书一向口碑不错，虽然还没有看到该书的内容（已经有部分读者有幸看到其中的部分章节），但有理由相信这是一本值得期待的好书。下面简要翻译了下该书的目录。 
正在争取该书的中文翻译事宜，由于国内PDF类的书出的不多，出版社似乎比较犹豫，希望看到中文版本的“iText in Action”（《iText实践》）的朋友留言支持下，必要的话，可以考虑把有关内容翻译成C#版本，来个iTextSharp实践，如何？ [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/08/16/478565.html)

[苟安廷的专栏](http://blog.csdn.net/gatr/)[用C#制作PDF文件全攻略](http://blog.csdn.net/gatr/archive/2005/02/02/278173.aspx)( 推荐, 是一份比较全面讲述iTextSharp组件使用的文档 )

PDF文件是目前比较流行的电子文档格式，在办公自动化（OA）等软件的开发中，经常要用到该格式，但介绍如何制作PDF格式文件的资料非常少，在网上搜来搜去，都转贴的是同一段“暴力”破解的方法,本文介绍了通过一个控件实现PDF文件制作，功能强大，制作简单。[阅读全文>](http://blog.csdn.net/gatr/archive/2005/02/02/278173.aspx)

[Cure](http://www.cnblogs.com/dahuzizyd/)[报表开发最后一计-使用iTextSharp来开发报表](http://www.cnblogs.com/dahuzizyd/archive/2006/10/05/Report_iTextSharp.html)

[RubyPdf的中文博客](http://www.cnblogs.com/hardrock/)(专注PDF技术)

[[置顶]利用iTextSharp填写中文(中日韩)PDF表单（完整解决方案）](http://www.cnblogs.com/hardrock/archive/2006/09/23/512605.html)

摘要: FillChinese（CJK） PDF Form with iTextSharp [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/09/23/512605.html)

[iTextSharp生成的PDF显示“PageX of Y”的效果，IPdfPageEvent的应用一例](http://www.cnblogs.com/hardrock/archive/2005/11/16/277417.html)摘要: 一个网友的来信： 
你好，我刚开始尝试使用iTextSharp，遇到一个问题： 
我生成的pdf要求页脚显示“Page x of y”，看范例里面的写法 
HeaderFooter f = new HeaderFooter( 
new Phrase("Page "), 
true); 
doc.Footer = f; 
这样显示可以看到当前页码，但是没办法得到PageCount。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2005/11/16/277417.html)

[实现用C#和VB.NET调用Ghostscript的API，把Postscript文件转为PDF文件。](http://www.cnblogs.com/hardrock/archive/2006/03/01/340580.html)摘要: 因为一直有想法做.NET版本的PDFCreator，配合iTextSharp，那样这个PDF 虚拟打印机将会多么强大，梦想中…… [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/03/01/340580.html)

[利用iTextSharp对PDF进行签名（E-signingPDF documents with iTextSharp）－－推荐](http://www.cnblogs.com/hardrock/archive/2006/06/28/438163.html)摘要: an exampledemonstrating how to sign PDF documents with iTextSharp library [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/06/28/438163.html)

[PDFToTextwith ITextSharp--Extract text from PDF in C# (100% .NET)(推荐）](http://www.cnblogs.com/hardrock/archive/2006/06/16/427112.html) 摘要: A simple class to extract plaintext from PDF documents with ITextSharp. 
这是个好消息，但还仅仅是个雏形。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/06/16/427112.html)

[iTextSharp问题系列：在PDFPtable里加入图像，如何控制图像大小](http://www.cnblogs.com/hardrock/archive/2006/04/02/364828.html) 摘要: 加入的图像总是充满了整个Cell，需要控制大小，该如何处理 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/04/02/364828.html)

[一个利用ChapterAutoNumber（iTextSharp的一个类）自动创建书签的话题](http://www.cnblogs.com/hardrock/archive/2006/03/21/354570.html)

[iTextSharp应用中关于“Cannotaccess a closed Stream”问题的解决办法（附带提供如何在页面中显示PDF的流）](http://www.cnblogs.com/hardrock/archive/2006/02/23/336397.html)摘要: 因为不少朋友提到过这个问题，就索性整理下方便大家也方便我自己了。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/02/23/336397.html)

[iTextSharp将亚洲字体支持（CIDFonts）和itext-hyph-xml发布为独立的library](http://www.cnblogs.com/hardrock/archive/2005/11/05/269313.html)

其实使用单独的project把那些资源文件以embeded resource的方式添加到project中，然后生成新的library，这样的好处你不用修改原来的iTextSharp来实现了（我之前介绍的CIDFont字体支持解决方案，当然效果是一样的）。作者前几天跟我说“Thefonts can be loaded not only from resources but also from a directory
path. I'll try to setip some documentation this weekend.”，他说的可能应该不是这个方案。
[http://sourceforge.net/project/showfiles.php?group_id=72954&package_id=168416](http://sourceforge.net/project/showfiles.php?group_id=72954&package_id=168416)

[iTextSharp增加CIDFont支持的另类实现方法（也许灵活性更大些）](http://www.cnblogs.com/hardrock/archive/2005/09/24/243127.html)

在《[为iTextSharp增加CID-keyed 字体（简称CIDFont）支持，让你在没有中文(CJK)TrueType字体（.TTF,.TTC）环境下一样完美显示中文(CJK)](http://www.cnblogs.com/archive/2005/09/24/243118.html)》一文中介绍的方法是把所有（或者想要的）CIDFont和Cmap嵌入到iTextSharp中，这时只要留心就会发现，iText的体积翻倍了，而且如果有了新的CIDFont，你还要重新编译iTextSharp，有没有别的解决办法呢？

下面介绍的就是不使用iTextSharp的嵌入资源的方式，而是根据需要从文件中直接读取（当然你也可以把所需资源嵌入到你的程序中，不是iTextSharp中），解决办法当然还是从上文提到的三个方法（函数）动手了，那就是修改CJKFont.cs类，下面就把我修改好的贴在下面，有兴趣的自己研究吧，有什么好的建议也望不吝赐教。

[利用iTextSharp填写中文(中日韩)PDF表单（完整解决方案）](http://www.cnblogs.com/hardrock/archive/2006/09/23/512605.html)

越来越多的人来信或者留言（甚至msn上找我），询问关于PDFForm填写的问题，尤其是中文（或者说中日韩文）表单填写的问题，本不想回答这类问题，因为相关的注意事项都已经在我的博客里说了，但现在看来还是有必要再啰唆下了，如果再有问题的话，希望带着Money来问，拜托了。 

下面这段代码根据iText的java教程修改而成，主要有一下用途：
1.填写表单；
2.利用CID字体填写中文（CJK：中日韩）表单；
3.利用TrueType字体填写中日韩表单（因为字体嵌入，生成的PDF会非常大）；
4.支持服务端PDF的填写；

[我的两份iTextSharp的教程源码（分别为C#和VB.NET）已被iTextsharp.sf.net收录](http://www.cnblogs.com/hardrock/archive/2005/10/14/254368.html) 摘要: 看来我要继续努力更新和完善它们才是。另外根据iText 1.54 版本的教程也几经port完毕，不日也将提供下载，这封教程相比已经提交的在iTextSharp的应用深度上更有进步。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2005/10/14/254368.html)

[iTextSharp(iText#) tutorial update version,VB.NET version](http://www.cnblogs.com/hardrock/archive/2005/09/28/245556.html) 摘要: 这个版本根据C#的版本通过SharpDevelop转换，然后修改而成 [阅读全文](http://www.cnblogs.com/hardrock/archive/2005/09/28/245556.html)

[一个简单的iTextSharp的ASP.NET的例子（根据iText的jsp版本转换而来）](http://www.cnblogs.com/hardrock/archive/2005/09/28/245548.html)摘要: 这个例子本也属于iTextSharp Tutorial的一部份，但因为是ASP.NET的，所以当时在提交教程的时候就把它忽略了，不知道是否有放上来的必要 [阅读全文](http://www.cnblogs.com/hardrock/archive/2005/09/28/245548.html)

[iTextSharp(iText#) tutorial update version(36773 times download from cnblogs)](http://www.cnblogs.com/hardrock/archive/2005/08/28/224200.html) 摘要: iTextSharp从0.4.0以后，API做了很大修改，典型的是把方法名的首字母由小写改为了大写，另外很多java下痕迹也被.NET的风格所替代，因此http://iTextSharp.sf.net上提供教程都无法编译了，这对初学者不是什么好事情，我花了近一个星期的早晨时间把它们修改调试，并补充了一些遗漏的教程，上传上来供初学者参考。[阅读全文](http://www.cnblogs.com/hardrock/archive/2005/08/28/224200.html)

[一些关于iText和iTextSharp的旧闻(someold news about iText and iTextSharp)](http://www.cnblogs.com/hardrock/archive/2006/07/02/440864.html) 摘要: Google Calendar使用iText来输出PDF(iTextused in Google Calendar) 
New release iTextSharp 3.1.2 and iText 1.4.2 (2006-06-23)[阅读全文](http://www.cnblogs.com/hardrock/archive/2006/07/02/440864.html)

[一些和Acrobat有关的书籍（仅提供链接地址，不断更新中）Update:2006-05-16](http://www.cnblogs.com/hardrock/archive/2006/05/12/398412.html)摘要: 慢慢把从网上搜集到的关于Acrobat的书籍资料介绍给大家，如果你有好的资料，并且这里没有列出的也可以告诉我，谢谢。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/05/12/398412.html)

PDF Tools

[PDF N-UpMaker:一个把PDF转成小册子或者把多个页面放到一个页面上的工具（免费，免Acrobat，命令行模式）](http://www.cnblogs.com/hardrock/archive/2006/04/23/382767.html) 摘要: N-Up Printing allows you toshrink several pages onto one page(N-Up打印输出允许你缩小若干个页面到一个页面上) Booklet就是小册子 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/04/23/382767.html)

[pdfrotate:rotateseach page in a pdf file by a given angle(now support 90,180,270)](http://www.cnblogs.com/hardrock/archive/2006/03/24/357819.html) 摘要: pdfrotate可以方便地把PDF中的每一页按一定的角度（90,180,270)旋转，生成新的PDF。 [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/03/24/357819.html)

[pdfselect:随意抽取，排序你的PDF（免费，免Acrobat，Console）](http://www.cnblogs.com/hardrock/archive/2006/04/30/389445.html) 摘要: 想从PDF中抽取奇数页（偶数页）为一个新的PDF？ 
想把PDF逆向排序？ 
想把PDF按照你希望的顺序从新排放？ 
那就试试PDFSelect吧，免费，免Acrobat，Console [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/04/30/389445.html)

[Releasepdf cropper0.1(pdfcrop)](http://www.cnblogs.com/hardrock/archive/2006/06/28/437425.html) 摘要: Remove white space around PDF's(need ghostscript 8.x) 
切除PDF每一页的白边 
支持批处理的命令行程序 
[阅读全文](http://www.cnblogs.com/hardrock/archive/2006/06/28/437425.html)

[PDFCreator：一款免费，开源的PDF（Tiff，pcx，png，jpeg，bmp，PS，EPS）打印机（VB,GPL)，并提供了COM接口，方便使用各种编程语言调用](http://www.cnblogs.com/hardrock/archive/2006/02/21/334532.html)摘要: 1.免费，开源（GPL），同时提供VB源代码和多种预编译版本； 
2.多种格式输出支持，不仅仅支持PDF，而且支持Tiff，pcx，png，jpeg，bmp，PS，EPS； 
3.多语言支持（中文翻译的很烂）； 
4.增加AMD64 芯片的支持； 
5.提供COM接口，支持多种语言编程调用（提供了多种语言的demo）； 
6.方便与PDF-T-Maker集成； 
[阅读全文](http://www.cnblogs.com/hardrock/archive/2006/02/21/334532.html)

[PDFtoDXFor other Vector Graphics （PDF转Autocad 的DXF及其他格式矢量图形）:pstoedit+Ghostscript](http://www.cnblogs.com/hardrock/archive/2006/07/13/449841.html)摘要: thisarticle shows how to Convert PDF to DXF, so autocad can easily edit it, the applicationof pstoedit , ghostscript and gsview [阅读全文](http://www.cnblogs.com/hardrock/archive/2006/07/13/449841.html)

其它

[向PDF中添加图片和圆角文本框等元素](http://www.msproject.cn/Article/PDFAddImageTextbox.html)

[翻译]
pmpdesign著[Add Imagesand Textboxes to PDF](http://www.codeproject.com/cs/library/imagestotextboxes.asp)

向PDF中添加图片和圆角文本框等元素

本文中的代码用以向PDF文件中添加图片和圆角文本框，然后将PDF安全地嵌在一个Web页面中。

[介绍]

我需要在程序中创建PDF文件，显示诸如发票等商业财务文档。

当然，现在网上也有这类的控件，不过都是有商业限制的，于是，我准备开发一个没有任何限制、没有license费用的控件。

另一个想法是，限制代码的大小，我觉得如果能用5%的代码完成的时候，就不该加上3MB代码量。

最后，通过仔细研究Zainu的精彩的文章[PDF Library forcreating PDF with tables and text, in C#](http://www.codeproject.com/dotnet/PdfLibrary.asp) ，我开始了本文工作。

[在ASP.NET页面中创建PDF文档](http://www.msproject.cn/Article/CreatePDFInAspx.html)

翻译
[fstrahberger](http://www.codeproject.com/script/Articles/list_articles.asp?userid=2378724).著[CreatingPDF documents in ASP.NET](http://www.codeproject.com/useritems/Creating_PDF_documents_in.asp)

简介
这篇很短的文章介绍如何在ASP.NET页面中创建PDF文档，本文使用了免费的开源库[http://sourceforge.net/projects/itextsharp/](http://sourceforge.net/projects/itextsharp/).

代码
首先，我会创建一个最简单的“HelloPDF”，接下来再使用table创建一个更复杂些的PDF文档。为了达到这些，你需要首先下载开源的itextsharp库（），并在项目中引用它。

PDF文档有页面ShowPDF.aspx创建，同时通过“Response.Redirect”传递PDF给用户。

网络资源

http://sourceforge.net/projects/itextsharp/
http://www.lowagie.com/iText/
http://www.microsoft.com/china/
[CodeProject上PDF相关组件](http://www.codeproject.com/info/search.aspx?artkw=pdf&sbo=kw)


