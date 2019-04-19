# Qt控件父对象的确认 - xqhrs232的专栏 - CSDN博客
2016年10月14日 23:13:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：568
个人分类：[X11/Qt/Qt quick/Qxml界面技术](https://blog.csdn.net/xqhrs232/article/category/6455019)
原文地址::[http://blog.csdn.net/desert187/article/details/39671797](http://blog.csdn.net/desert187/article/details/39671797)
相关文章
1、[QT findChild() 及动态初始化窗体布局（原创）](http://zhaiyue-program.iteye.com/blog/1546394)----[http://zhaiyue-program.iteye.com/blog/1546394](http://zhaiyue-program.iteye.com/blog/1546394)
2、[在Qt中如何使用QtDesigner创建的UI文件（一）](http://blog.csdn.net/desert187/article/details/39672039)----[http://blog.csdn.net/desert187/article/details/39672039](http://blog.csdn.net/desert187/article/details/39672039)
3、[在Qt中如何使用QtDesigner创建的UI文件（二）](http://blog.csdn.net/desert187/article/details/39672061)----[http://blog.csdn.net/desert187/article/details/39672061](http://blog.csdn.net/desert187/article/details/39672061)
**Qt控件父对象的确认**
如果是是通过手动编写程序的方式来设计界面，则在编写代码时手动指定各控件的父对象；
如果是使用Qt设计师来设计界面，则各控件的最终父对象为该界面类的对象。
如有一个查找对话框界面，则Qt为其自动生成的界面类代码如下：
头文件：
#ifndefFINDDLG_H#defineFINDDLG_H#include<QDialog>namespaceUi{classFindDlg;}classFindDlg:publicQDialog{Q_OBJECTpublic:explicitFindDlg(QWidget*parent=0);~FindDlg();private:Ui::FindDlg*ui;};#endif//FINDDLG_H
这其实是使用单继承方式来使用Ui（关于使用Ui的更多细节请参阅《在Qt中如何使用QtDesigner创建的UI文件》）。
实现文件：
#include"FindDlg.h"#include"ui_FindDlg.h"FindDlg::FindDlg(QWidget*parent):QDialog(parent),ui(newUi::FindDlg){ui->setupUi(this);QObject*a=ui->ButFind_1->parent()->parent()->parent()->parent();//此处为手动添加a->children();//此处为手动添加}FindDlg::~FindDlg(){deleteui;}
以上代码除注释处为手动添加之外，其余均为Qt自动生成的代码。 从以上代码中可发现，在构造FindDlg的时候，先执行setupUi，并传入参数this。setupUi的部分代码如下：voidsetupUi(QDialog*FindDlg){if(FindDlg->objectName().isEmpty())FindDlg->setObjectName(QStringLiteral("FindDlg"));FindDlg->resize(302,152);gridLayout=newQGridLayout(FindDlg);gridLayout->setObjectName(QStringLiteral("gridLayout"));TabChnl=newQTabWidget(FindDlg);TabChnl->setObjectName(QStringLiteral("TabChnl"));...}
可见，在setupUi时，Qt为各控件设置了父对象，但所有控件的最终父对象都为setupUi时传入的this，也就是FindDlg的对象。
这一点可通过以上程序来验证：
由于Qt在删除对象时会自动删除其子对象，因此无需显式delete这些用Qt设计师添加的控件。
