# 利用 Qt 读取 XML 文件的方法 - xqhrs232的专栏 - CSDN博客
2018年09月06日 16:35:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：195
原文地址::[https://blog.csdn.net/liyuanbhu/article/details/44062479](https://blog.csdn.net/liyuanbhu/article/details/44062479)
相关文章
1、QT读取xml例子----[https://blog.csdn.net/zouhao619/article/details/6891736](https://blog.csdn.net/zouhao619/article/details/6891736)
XML 是可扩展标记语言（Extensible Markup Language）的缩写。XML 文件由内容和标记组成，通过以标记包围内容的方式将大部分内容包含在元素中。
Qt 中提供了多种读取XML文件的方法，这里简单的记录一下用 QDomDocument 读取的步骤。为什么使用QDomDocument 呢，因为XML 本身就是一以树状结构组织数据的，而DOM 也是将数据组织为树状结构，最适合直观地展示XML数据。
下面的代码是Qt 帮助文件中自带的例子代码：
- 
`QDomDocument doc("mydocument");`
- 
`QFile file("mydocument.xml");`
- 
`if (!file.open(QIODevice::ReadOnly))`
- 
`return;`
- 
`if (!doc.setContent(&file))`
- 
`{`
- 
`file.close();`
- 
`return;`
- 
`}`
- 
`file.close();`
- 
- 
`// print out the element names of all elements that are direct children`
- 
`// of the outermost element.`
- 
`QDomElement docElem = doc.documentElement();`
- 
- 
`QDomNode n = docElem.firstChild();`
- 
`while (!n.isNull())`
- 
`{`
- 
`QDomElement e = n.toElement(); // try to convert the node to an element.`
- 
`if (!e.isNull()) `
- 
`{`
- 
`cout << qPrintable(e.tagName()) << endl; // the node really is an element.`
- 
`}`
- 
`n = n.nextSibling();`
- 
- 
`}`
如果xml有多层，那么可以递归的去读取。我写了小程序将xml 的的数据读入到一个树型列表控件中。下面是核心的代码：
- 
`#ifndef DIALOG_H`
- 
`#define DIALOG_H`
- 
`#include <QtXml>`
- 
`#include <QDialog>`
- 
- 
`namespace Ui {`
- 
`class Dialog;`
- 
`}`
- 
`class QTreeWidgetItem;`
- 
`class Dialog : public QDialog`
- 
`{`
- 
`Q_OBJECT`
- 
- 
`public:`
- 
`explicit Dialog(QWidget *parent = 0);`
- 
`void listDom(QDomElement& docElem, QTreeWidgetItem* pItem);`
- 
`void openXML(QString fileName);`
- 
`~Dialog();`
- 
- 
`private:`
- 
`Ui::Dialog *ui;`
- 
`private slots:`
- 
`void openFile();`
- 
`};`
- 
- 
`#endif // DIALOG_H`
- 
`#include "dialog.h"`
- 
`#include "ui_dialog.h"`
- 
`#include <QFileDialog>`
- 
`Dialog::Dialog(QWidget *parent) :`
- 
`QDialog(parent),`
- 
`ui(new Ui::Dialog)`
- 
`{`
- 
`ui->setupUi(this);`
- 
`connect(ui->pushButtonOpen, SIGNAL(clicked()), this, SLOT(openFile()));`
- 
`ui->treeWidget->setColumnCount(2);`
- 
`ui->treeWidget->setColumnWidth(0, 400);`
- 
`setWindowFlags(Qt::Dialog | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);`
- 
`showMaximized();`
- 
`}`
- 
`void Dialog::openXML(QString fileName)`
- 
`{`
- 
`QFile file(fileName);`
- 
`if(file.open(QIODevice::ReadOnly))`
- 
`{`
- 
`QDomDocument dom("WCM");`
- 
`if (dom.setContent(&file))`
- 
`{`
- 
`ui->treeWidget->clear();`
- 
`QDomElement docElem = dom.documentElement();`
- 
`listDom(docElem, NULL);`
- 
`}`
- 
`}`
- 
`file.close();`
- 
`}`
- 
- 
`void Dialog::openFile()`
- 
`{`
- 
`QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML File"), "c:/", tr("XML Files (*.xml)"));`
- 
`if(!fileName.isEmpty())`
- 
`{`
- 
`openXML( fileName );`
- 
`}`
- 
`}`
- 
- 
`Dialog::~Dialog()`
- 
`{`
- 
`delete ui;`
- 
`}`
- 
- 
`void Dialog::listDom(QDomElement& docElem, QTreeWidgetItem * pItem)`
- 
`{`
- 
`QDomNode node = docElem.firstChild();`
- 
`if(node.toElement().isNull())`
- 
`{`
- 
`pItem->setText (1, docElem.text());`
- 
`}`
- 
`while(!node.isNull())`
- 
`{`
- 
`QDomElement element = node.toElement(); // try to convert the node to an element.`
- 
`if( !element.isNull() )`
- 
`{`
- 
`QTreeWidgetItem *item;`
- 
`if( pItem )`
- 
`{`
- 
`item = new QTreeWidgetItem(pItem);`
- 
`}`
- 
`else`
- 
`{`
- 
`item = new QTreeWidgetItem(ui->treeWidget);`
- 
`}`
- 
`item->setText(0, element.tagName());`
- 
`listDom(element, item);`
- 
`if( pItem )`
- 
`{`
- 
`pItem->addChild(item);`
- 
`}`
- 
`else`
- 
`{`
- 
`ui->treeWidget->addTopLevelItem(item);`
- 
`}`
- 
`}`
- 
- 
`node = node.nextSibling();`
- 
`}`
- 
`return;`
- 
`}`
下面是个测试 xml 文件：
- 
`<?xml version="1.0" encoding="UTF-8"?>`
- 
`<recipe type="dessert">`
- 
`<recipename cuisine="american" servings="1">Ice Cream Sundae</recipename>`
- 
`<ingredlist>`
- 
`<listitem>`
- 
`<quantity units="cups">0.5</quantity>`
- 
`<itemdescription>vanilla ice cream</itemdescription>`
- 
`</listitem>`
- 
`<listitem>`
- 
`<quantity units="tablespoons">3</quantity>`
- 
`<itemdescription>chocolate syrup or chocolate fudge</itemdescription>`
- 
`</listitem>`
- 
`<listitem>`
- 
`<quantity units="tablespoons">1</quantity>`
- 
`<itemdescription>nuts</itemdescription>`
- 
`</listitem>`
- 
`<listitem>`
- 
`<quantity units="each">1</quantity>`
- 
`<itemdescription>cherry</itemdescription>`
- 
`</listitem>`
- 
`</ingredlist>`
- 
`<utensils>`
- 
`<listitem>`
- 
`<quantity units="each">1</quantity>`
- 
`<utensilname>bowl</utensilname>`
- 
`</listitem>`
- 
`<listitem>`
- 
`<quantity units="each">1</quantity>`
- 
`<utensilname>spoons</utensilname>`
- 
`</listitem>`
- 
`<listitem>`
- 
`<quantity units="each">1</quantity>`
- 
`<utensilname>ice cream scoop</utensilname>`
- 
`</listitem>`
- 
`</utensils>`
- 
`<directions>`
- 
`<step>Using ice cream scoop, place vanilla ice cream into bowl.</step>`
- 
`<step>Drizzle chocolate syrup or chocolate fudge over the ice cream.</step>`
- 
`<step>Sprinkle nuts over the mound of chocolate and ice cream.</step>`
- 
`<step>Place cherry on top of mound with stem pointing upward.</step>`
- 
`<step>Serve.</step>`
- 
`</directions>`
- 
`<variations>`
- 
`<option>Replace nuts with raisins.</option>`
- 
`<option>Use chocolate ice cream instead of vanilla ice cream.</option>`
- 
`</variations>`
- 
`<preptime>5 minutes</preptime>`
- 
`</recipe>`
下面是软件界面：
![](https://img-blog.csdn.net/20150304170028654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl5dWFuYmh1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
