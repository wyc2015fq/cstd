# Qt打印详解 - fanyun的博客 - CSDN博客
2017年01月25日 09:16:06[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：8985
QT作为一款轻量级的集成开发环境，其设计的目标是使开发人员利用QT这个应用程序框架更加快速及轻易的开发应用程序。要达到此目的，要求QT必须能够跨平台，QT能够在32位及64位的Linux，MAC OS X以及Windows上运行。
现在我们想在项目中加入一项打印功能，即使在不同的操作系统环境下，仍然能有相同的用户体验，在实现中发现QT对打印的支持不是那么友好。我们希望在跨平台的情况下，能打印各种文档，比如用户的简历，发现如果不调用对应操作系统的内核外壳打印函数，这项工作变得异常艰难。我们希望用QT自带的函数库，来实现简单的打印功能，这样就实现了一次编码，多环境使用的目的，在Qt中，打印与在QWidget，QPixmap或者QImage绘图很相似，一般步骤如下：
(1).创建绘图设备的QPrinter；
(2).弹出打印对话框，QPrintDialog，允许用户选择打印机，设置属性等；
(3).创建一个QPrinter的QPainter；
(4).用QPainter绘制一页；
(5).调用QPrinter::newPage()，然后绘制下一页；
(6).重复步骤4，5，直到打印完所有页。
目前操作系统打印支持两种方式：一.调用内核外壳打印函数
Linux下面相对比较容易，使用lpr –p [printer] [fileName]即可，即如果想把file.cpp文件送到打印机pl则使用lpr –p pl file.cpp即可。
Windows下，我们实现打印既定文件路径的文档时，我们调用windows内核命令执行函数，实现打印功能。具体代码如下：
```cpp
bool printFile(const QString & filePath)
{
    // create a printer
    QPrinter printer;
    QString printerName = printer.printerName();
    // no default printer or no file return false
    if( printerName.size() == 0 || filePath.size() == 0)
        return false;
    QFileInfo fi(filePath);
    if(!fi.exists())
        return false;
    int ret = 0;
    ret = (int)ShellExecuteW(NULL,
                  QString("print").toStdWString().c_str(),
                  filePath.toStdWString().c_str(),
                  NULL,
                  NULL,
                  SW_HIDE);
    // if return number bigger than 31 indicate succ
    if (ret > SE_ERR_NOASSOC)
        return true;
    return false;
}
```
2.使用QT自定义的类和函数实现打印文件的功能
![](https://img-blog.csdn.net/20170125090945562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmFueXVuXzAx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
QT中和打印文件相关的类如下所示，它们分别是：
抽象文档打印对话框类，提供配置打印机的打印对话框基本实现；
页面设置对话框类，和打印页面相关参数的配置对话框；
打印对话框类，指定打印机配置的对话框；
打印引擎类，定义了QPrinter类如何与打印子系统交互的接口；
打印预览对话框类，用来预览和配置页面布局的对话框；
打印预览控件类，预览页面布局的控件；
打印机类，指示打印机如何工作。
QT中实现打印的方法
在QT中如果想实现打印，首先，我们需要构造一个打印机类（QPrinter）对象， 其次，使用此对象构造一个打印对话框类（QprinterDialog）对象。如下所示：
```cpp
QPrinter printer;
QprinterDialog *dialog = new QprintDialog(&printer, this);
dialog->setWindowtitle(tr(“Print Document”));
if(editor->textCursor().hasSelection())
	dialog->addEnabledOption(QabstractPrintDialog::PrintSelection);
if (dialog->exec() != QDialog::Accepted)
	return;
```
最后附上在项目中，我们如何利用QT自身类，来实现这个跨平台的打印功能。
首先，在头文件中指定槽函数：
```cpp
private slots:
    void doPrint();
    void doPrintPreview();
    void printPreview(QPrinter *printer);
    void createPdf();
void setUpPage();
具体实现如下：
void MainWindow::doPrint()
{
    // 创建打印机对象
    QPrinter printer;
    // 创建打印对话框
    QString printerName = printer.printerName();
    if( printerName.size() == 0)
        return;
    QPrintDialog dlg(&printer, this);
    //如果编辑器中有选中区域，则打印选中区域
    if (ui->textEdit->textCursor().hasSelection())
        dlg.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    // 如果在对话框中按下了打印按钮，则执行打印操作
    if (dlg.exec() == QDialog::Accepted)
    {
       ui->textEdit->print(&printer);
       // print the existing document by absoult path
      //  printFile("D:/myRSM.doc");
    }
}
// 打印预览
void MainWindow::doPrintPreview()
{
    QPrinter printer;
    // 创建打印预览对话框
    QPrintPreviewDialog preview(&printer, this);
    // 当要生成预览页面时，发射paintRequested()信号
    connect(&preview, SIGNAL(paintRequested(QPrinter*)),
                  this,SLOT(printPreview(QPrinter*)));
    preview.exec();
}
void MainWindow::printPreview(QPrinter *printer)
{
    ui->textEdit->print(printer);
}
// 生成PDF文件
void MainWindow::createPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("导出PDF文件"), QString(), "*.pdf");
    if (!fileName.isEmpty()) {
        // 如果文件后缀为空，则默认使用.pdf
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        QPrinter printer;
        // 指定输出格式为pdf
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        ui->textEdit->print(&printer);
    }
}
// 页面设置
void MainWindow::setUpPage()
{
    QPrinter printer;
    QPageSetupDialog pageSetUpdlg(&printer, this);
    if (pageSetUpdlg.exec() == QDialog::Accepted)
    {
        printer.setOrientation(QPrinter::Landscape);
}
else
{
	printer.setOrientation(QPrinter::Portrait);
}
}
```
       在Windows和Mac OS X平台，QPrinter使用系统的打印驱动程序。在Unix上，QPrinter生成脚本并把脚本发送给lp或者lpr（或者发送给程序，打印程序有函 数QPrinter::setPrintProgram()）。调用QPrinter::setOutputFormat (QPrinter::PdfFormat)QPrinter也可以生成PDF文件。
Figure 8.12. Printing a QImage
一.例子：打印一个QImage到一页纸上。
```cpp
void PrintWindow::printImage(const QImage &image)
{
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size = image.size();
        size.scale(rect.size(), Qt::KeepAspectRatio);
        painter.setViewport(rect.x(), rect.y(),
                            size.width(), size.height());
        painter.setWindow(image.rect());
        painter.drawImage(0, 0, image);
    }
}
```
这里，我们假设了在PrintWindow类有一个QPrinter类型的成员变量printer。当然在printImage()函数的堆上我们也可以创建一个QPrinter，但是这样不能记录用户进行打印时的设置
创建QPrintDialog，调用exec()显示出来，如果用户点击了OK返回true，否则返回false。调用exec()后，QPrinter对象就可以使用了。（也可以不显示QPrintDialog，直接调用QPrinter的成员函数进行复制也可以）
然后，我们创建QPainter，绘图设备为QPrinter。设置窗口为所显示图形的矩形，视口也同样比例，然后在（0，0）绘制图像。
通常，QPainter的窗口自动进行了初始化，打印机和屏幕有着一致的分辨率（一英寸有72到100个点），使控件的打印代码能够重用。在上面的函数中，我们自己设置来QPainter的窗口。
在一页中进行打印很简单，但是，很多应用程序需要打印多页。这时我们一次打印一页，然后调用newPage()打印另一页。这里需要解决定一个问题是要确定一页打印多少内容。在Qt中有两种方法处理多页的打印文档：
(1).我们可以把数据转换为HTML格式，使用QTextDocument描述他们，QTextDocument是Qt的多文本引擎。
(2).手动进行分页
下面我们来分别看一下这两种方法。第一个例子，我们想打印一个花卉的指导：一列为花的名字，另一列为文本描述。每一条的文本格式存储为：“名称：描述”。 例如：Miltonopsis santanae: A most dangerous orchid species.
由于每一种花卉的数据都可以用一个字符串表示，我们可以用QStringList表示所有花卉的数据。下面的代码为使用Qt的多文本引擎进行打印的例子：
```cpp
void PrintWindow::printFlowerGuide(const QStringList &entries)
{
    QString html;
    foreach (QString entry, entries) {
        QStringList fields = entry.split(": ");
        QString title = Qt::escape(fields[0]);
        QString body = Qt::escape(fields[1]);
        html += "<table width=\"100%\" border=1 cellspacing=0>\n"
                "<tr><td bgcolor=\"lightgray\"><font size=\"+1\">"
                "<b><i>" + title + "</i></b></font>\n<tr><td>" + body
                + "\n</table>\n<br>\n";
    }
    printHtml(html);
}
```
首先把QStringList转换为HTML。每一种花卉为HTML表格中的一行，调用Qt::escapte()将特殊字符 ’&’，’>’，’<’等用相应的HTML字符表示（’amp’，’>’，’<’），然后调用 printHtml()打印文本：
```cpp
void PrintWindow::printHtml(const QString &html)
{
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        QTextDocument textDocument;
        textDocument.setHtml(html);
        textDocument.print(&printer);
    }
}
```
函数printHtml()弹出QPrintDialog对话框，负责打印一个HTML文档。这些代码可以在所有Qt的应用程序中打印任意HTML文档。
Figure 8.13. Printing a flower guide using QTextdocument
目前，把文本转换为HTML文档用QTextDocument打印是最方便的一个方法。如果需要更多的设置，就需要我们自己进行页面布局和绘制。下面的方法就是用人工干预的方式打印花卉指南。首先看一下printFlowerGuide()函数：
```cpp
void PrintWindow::printFlowerGuide(const QStringList &entries)
{
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        QList<QStringList> pages;
        paginate(&painter, &pages, entries);
        printPages(&painter, pages);
    }
}
```
在创建QPainter，设置好打印机以后，调用函数paginate()确定那些项目在那一页。执行这个函数的结果是得到一个QStringList的列表，每一个QStringList在一页里显示，把这个结果传递给printPages()进行打印。
例如：需要打印的花卉指南有6个条目：A,B,C,D,E,F。其中A和B在第一页，C,D,E打印在第二页，F在第三页打印。
```cpp
void PrintWindow::paginate(QPainter *painter, QList<QStringList> *pages,
                           const QStringList &entries)
{
    QStringList currentPage;
    int pageHeight = painter->window().height() - 2 * LargeGap;
    int y = 0;
    foreach (QString entry, entries) {
        int height = entryHeight(painter, entry);
        if (y + height > pageHeight && !currentPage.empty()) {
            pages->append(currentPage);
            currentPage.clear();
            y = 0;
        }
        currentPage.append(entry);
        y += height + MediumGap;
    }
    if (!currentPage.empty())
        pages->append(currentPage);
}
```
函数paginate()把花会指南条目分页。根据entryHeight()计算每一个条目的高度。同时考虑页面顶端和底端的垂直距离LargeGap。
遍历所有的条目，如果这个条目可以放在当前页，就把这个条目放到当前页的列表里面。当前页排满后，把当前页放到页的列表中，开始新的一页。
```cpp
int PrintWindow::entryHeight(QPainter *painter, const QString &entry)
{
    QStringList fields = entry.split(": ");
    QString title = fields[0];
    QString body = fields[1];
    int textWidth = painter->window().width() - 2 * SmallGap;
    int maxHeight = painter->window().height();
    painter->setFont(titleFont);
    QRect titleRect = painter->boundingRect(0, 0, textWidth, maxHeight,
                                            Qt::TextWordWrap, title);
    painter->setFont(bodyFont);
    QRect bodyRect = painter->boundingRect(0, 0, textWidth, maxHeight,
                                           Qt::TextWordWrap, body);
    return titleRect.height() + bodyRect.height() + 4 * SmallGap;
}
```
函数entryHeight()根据QPainter::boundingRect()计算每一个条目的垂直距离，图8.4表明了条目的布局和SmallGap还MediumGap的含义：
The enTRyHeight() function uses QPainter::boundingRect() to compute the vertical space needed by one entry. Figure 8.14 shows the layout of a flower entry and the meaning of the SmallGap and
 MediumGap constants.
Figure 8.14. A flower entry's layout
```cpp
void PrintWindow::printPages(QPainter *painter,
                             const QList<QStringList> &pages)
{
    int firstPage = printer.fromPage() - 1;
    if (firstPage >= pages.size())
        return;
    if (firstPage == -1)
        firstPage = 0;
    int lastPage = printer.toPage() - 1;
    if (lastPage == -1 || lastPage >= pages.size())
        lastPage = pages.size() - 1;
    int numPages = lastPage - firstPage + 1;
    for (int i = 0; i < printer.numCopies(); ++i) {
        for (int j = 0; j < numPages; ++j) {
            if (i != 0 || j != 0)
                printer.newPage();
            int index;
            if (printer.pageOrder() == QPrinter::FirstPageFirst) {
                index = firstPage + j;
            } else {
                index = lastPage - j;
            }
            printPage(painter, pages[index], index + 1);
        }
    }
}
```
函数printPages()的作用是调用printPage()按照顺序和打印份数打印每一页。通过QPrintDialog，用户可能需要打印多份，设置了打印范围，或者要求逆序打印。我们需要在程序中考虑这些需求
首先确定打印范围。QPrinter::fromPage()和toPage()返回用户选择的页面范围。如果没有选择，返回为0。我们进行了减1操作是 因为我们的页面索引是从0开始的。如果用户没有选定范围，则打印全部，firstPage和lastPage包含量所有的页面。
然后我们打印每一页。最外层循环为用户设定的打印的份数。对于那些支持多份打印的打印机，QPrinter::numCopies()总是返回1。如果打 印机驱动程序不支持多份打印，numCopies()返回到是用户指定的打印份数，有应用程序实现多份打印。（在这一节的QImage例子中，为了简单起 见，我们没有考虑多份打印。）
Figure 8.15. Printing a flower guide using QPainter
内层循环遍历打印的页数。如果页数不是第一页，调用newPage()清楚原来的页面开始填充新页面。调用printPage()打印每一页。
```cpp
void PrintWindow::printPage(QPainter *painter,
                            const QStringList &entries, int pageNumber)
{
    painter->save();
    painter->translate(0, LargeGap);
    foreach (QString entry, entries) {
        QStringList fields = entry.split(": ");
        QString title = fields[0];
        QString body = fields[1];
        printBox(painter, title, titleFont, Qt::lightGray);
        printBox(painter, body, bodyFont, Qt::white);
        painter->translate(0, MediumGap);
    }
    painter->restore();
    painter->setFont(footerFont);
    painter->drawText(painter->window(),
                      Qt::AlignHCenter | Qt::AlignBottom,
                      QString::number(pageNumber));
}
```
函数printPage()打印页面中的每一个条目。首先用printBox()打印标题，然后用printBox()打印描述。在每一页的底端打印页码。
Figure 8.16. The flower guide's page layout
```cpp
void PrintWindow::printBox(QPainter *painter, const QString &str,
                           const QFont &font, const QBrush &brush)
{
    painter->setFont(font);
    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * SmallGap;
    int maxHeight = painter->window().height();
    QRect textRect = painter->boundingRect(SmallGap, SmallGap,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, str);
    int boxHeight = textRect.height() + 2 * SmallGap;
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(brush);
    painter->drawRect(0, 0, boxWidth, boxHeight);
    painter->drawText(textRect, Qt::TextWordWrap, str);
    painter->translate(0, boxHeight);
}
```
printBox()首先绘制一个矩形框，然后在矩形框中绘制文本。
            
