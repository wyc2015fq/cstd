# qt  tableb view  打印预览和打印的问题   个人解决办法 - baidu_33570760的博客 - CSDN博客
2016年05月12日 09:41:25[carman_风](https://me.csdn.net/baidu_33570760)阅读数：2948
                    好了长话短说  因为tableview 不提供print 函数 ，所以 要实现打印现在基本是基于 两种  一种是利用render 函数    一种是利用 html 来实现    我个人因为在用render方法的时候 打印预览是空白的  （同事使用相同的代码，在qt4.8+vs2008可以实现） 所以使用了html 来实现  。OK贴代码
// 打印预览  
QPrinterprinter(QPrinter::ScreenResolution);
QPrintPreviewDialogpreview(&printer);connect(&preview,SIGNAL(paintRequested(QPrinter*)),this,SLOT(print(QPrinter*)));preview.exec();
//打印
QStringstrStream,strTitle;QTextStreamout(&strStream);strTitle="carman——feng";constintrowCount=ui->tableView->model()->rowCount();constintcolumnCount=ui->tableView->model()->columnCount();out<<"<html>\n""<head>\n""<metaContent=\"Textml;charset=Windows-1251\">\n"<<QString("<title>%1</title>\n").arg(strTitle)<<"</head>\n""<bodybgcolor=#fffffflink=#5000A0>\n""<tableborder=1cellspacing=0cellpadding=2>\n";//headersout<<"<thead><trbgcolor=#f0f0f0>";for(intcolumn=0;column<columnCount;++column)if(!ui->tableView->isColumnHidden(column))out<<QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column,Qt::Horizontal).toString());out<<"</tr></thead>\n";//datatablefor(introw=0;row<rowCount;++row){out<<"<tr>";for(intcolumn=0;column<columnCount;++column){if(!ui->tableView->isColumnHidden(column)){QStringdata=ui->tableView->model()->data(ui->tableView->model()->index(row,column)).toString().simplified();out<<QString("<tdbkcolor=0>%1</td>").arg((!data.isEmpty())?data:QString(" "));}}out<<"</tr>\n";}out<<"</table>\n""</body>\n""<ml>\n";QTextDocument*document=newQTextDocument();document->setHtml(strStream);document->print(printer);
html的方法在qt 5.5 \5.6 版本的minGW 测试 可行  
