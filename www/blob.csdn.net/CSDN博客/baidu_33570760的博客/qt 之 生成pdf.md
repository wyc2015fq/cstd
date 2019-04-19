# qt 之 生成pdf - baidu_33570760的博客 - CSDN博客
2017年05月04日 12:00:05[carman_风](https://me.csdn.net/baidu_33570760)阅读数：183
 转载自：http://blog.sina.com.cn/s/blog_a6fb6cc90101gvnx.html
Qt中如何让图片、文本、HTML或者其他形式的内容生成pdf呢？主要利用QPrinter来实现，QPrinter不止可以操作打印机来打印纸张文件，并且可以将文件保存至磁盘，存储为pdf格式的文件。
**图片生成pdf**
代码如下：
QPrinter printer_pixmap(QPrinter::HighResolution);
printer_pixmap.setPageSize(QPrinter::A4); //设置纸张大小为A4
printer_pixmap.setOutputFormat(QPrinter::PdfFormat); //设置输出格式为pdf
printer_pixmap.setOutputFileName("E:\\test_pixmap.pdf");  //设置输出路径
QPixmap pixmap = QPixmap::grabWidget(main_widget,main_widget->rect());  //获取界面的图片
QPainter painter_pixmap;
painter_pixmap.begin(&printer_pixmap);
QRect rect = painter_pixmap.viewport();
int multiple = rect.width()/pixmap.width();
painter_pixmap.scale(multiple, multiple);//将图像(所有要画的东西)在pdf上放大multiple-1倍
painter_pixmap.drawPixmap(0, 0, pixmap); //画图
painter_pixmap.end();
效果如下：![](https://img-blog.csdn.net/20170515140331495?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**文本生成pdf**
代码如下：
QPrinter printer_text;
printer_text.setOutputFormat(QPrinter::PdfFormat);
printer_text.setOutputFileName("E:\\test_text.pdf");
QPainter painter_text;
painter_text.begin(&printer_text);
QDateTime current_date_time =QDateTime::currentDateTime();
QString current_date = current_date_time.toString("yyyy-MM-ddhh:mm:ss ddd");
QPoint point(10, 10);
for(int i=1; i<=5; i++)
{
QString message = QString("%1        %2        %3").arg(QString::number(i)).arg(current_date).arg(QStringLiteral("北京"));
int y = point.y();
point.setY(y+20);
painter_text.drawText(point, message);
message.clear();
}
//printer_text.newPage(); //生成新的一页，可继续插入
painter_text.end();
效果如下：![](https://img-blog.csdn.net/20170515140349462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**HTML生成pdf**
QStringList title;
title.push_back(QStringLiteral("名称"));
title.push_back(QStringLiteral("修改日期"));
title.push_back(QStringLiteral("类型"));
title.push_back(QStringLiteral("大小"));
![](https://img-blog.csdn.net/20170515140408825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170515140425322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
QPrinter printer_html;
printer_html.setPageSize(QPrinter::A4);
printer_html.setOutputFormat(QPrinter::PdfFormat);
printer_html.setOutputFileName("E:\\test_html.pdf");
QTextDocument text_document;
text_document.setHtml(html);
text_document.print(&printer_html);
text_document.end();

效果如下![](https://img-blog.csdn.net/20170515135950007?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzM1NzA3NjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**注：**
   技术在于交流、沟通，转载请注明出处并保持作品的完整性。
