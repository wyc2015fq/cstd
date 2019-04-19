# Qt的Tab选项卡 - xqhrs232的专栏 - CSDN博客
2018年01月05日 13:49:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：348
原文地址::[http://blog.51cto.com/xcf007/106772](http://blog.51cto.com/xcf007/106772)
相关文章
1、Qt实现窗口tabwidget的拖出双击弹回功能----[http://blog.csdn.net/zmm19861210/article/details/9036779](http://blog.csdn.net/zmm19861210/article/details/9036779)
#ifndef __TABDLG_H__ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#define
 __TABDLG_H__ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QDialog> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class QWidget; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class QFileInfo; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class QTabWidget; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class QDialogButtonBox; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//常规选项卡 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class GeneralTab
 : public QWidget 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        Q_OBJECT 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)public: 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        GeneralTab(const QFileInfo
 &fileInfo, QWidget *parent = 0); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)}; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//许可权限 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class PermissionsTab
 : public QWidget 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        Q_OBJECT 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)public: 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        PermissionsTab(const QFileInfo
 &fileInfo, QWidget *parent = 0); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)}; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//程序 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class ApplicationsTab
 : public QWidget 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        Q_OBJECT 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)public: 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        ApplicationsTab(const QFileInfo
 &fileInfo, QWidget *parent = 0);    
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)}; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//自定义Tab对话框 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)class TabDialog:public QDialog 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)public: 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  TabDialog(const QString
 &fileName, QWidget *parent = 0);    
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)private: 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  QTabWidget
 *tabWidget; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  QDialogButtonBox
 *buttonBox; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)}; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#endif //
 __TABDLG_H__ 
tabdlg.cpp:
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QTabWidget> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QWidget> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QDialog> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QFileInfo> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QVBoxLayout> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QDialogButtonBox> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QLabel> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QLineEdit> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QDateTime> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QGroupBox> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QCheckBox> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QListWidget> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include "tabdlg.h"
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)TabDialog::TabDialog(const QString
 &fileName, QWidget *parent):QDialog(parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  QFileInfo
 fileInfo(fileName); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  tabWidget=new QTabWidget; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  tabWidget->addTab(new GeneralTab(fileName),trUtf8("常规")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  tabWidget->addTab(new PermissionsTab(fileName),trUtf8("权限")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  tabWidget->addTab(new ApplicationsTab(fileName),trUtf8("应用程序")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  buttonBox=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  connect(buttonBox,SIGNAL(accepted()),this,SLOT(accept()));//OK 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  connect(buttonBox,SIGNAL(rejected()),this,SLOT(reject()));//Cancel 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  QVBoxLayout
 *mainLayout=new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  mainLayout->addWidget(tabWidget); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  mainLayout->addWidget(buttonBox); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  setLayout(mainLayout); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  setWindowTitle(trUtf8("Tab对话框")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)} 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//常规面板类构造函数实现    
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)GeneralTab::GeneralTab(const QFileInfo
 &fileInfo, QWidget *parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  :QWidget(parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *fileNameLabel = new QLabel(trUtf8("文件名:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLineEdit *fileNameEdit = new QLineEdit(fileInfo.fileName());//去掉目录路径前缀后的 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *pathLabel = new QLabel(trUtf8("路径:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *pathValueLabel = new QLabel(fileInfo.absoluteFilePath());//完整的路径名 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 pathValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);//设置label的样式 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *sizeLabel = new QLabel(trUtf8("大小:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 qlonglong size = fileInfo.size()/1024;//size()返回字节大小 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *sizeValueLabel = new QLabel(trUtf8("%1
 K").arg(size)); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 sizeValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *lastReadLabel = new QLabel(trUtf8("上次读取时间:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *lastReadValueLabel = new QLabel(fileInfo.lastRead().toString()); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 lastReadValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *lastModLabel = new QLabel(trUtf8("上次修改时间:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *lastModValueLabel = new QLabel(fileInfo.lastModified().toString()); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 lastModValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QVBoxLayout *mainLayout = new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(fileNameLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(fileNameEdit); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(pathLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(pathValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(sizeLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(sizeValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(lastReadLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(lastReadValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(lastModLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(lastModValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//mainLayout->addStretch(1); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 setLayout(mainLayout);    
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)} 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//权限面板类 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)PermissionsTab::PermissionsTab(const QFileInfo
 &fileInfo, QWidget *parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  :QWidget(parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//群组框 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QGroupBox *permissionsGroup = new QGroupBox(trUtf8("权限")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QCheckBox *readable = new QCheckBox(trUtf8("可读")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)if (fileInfo.isReadable()) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 readable->setChecked(true);//勾选 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QCheckBox *writable = new QCheckBox(trUtf8("可写")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)if (
 fileInfo.isWritable() ) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 writable->setChecked(true); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QCheckBox *executable = new QCheckBox(trUtf8("可执行")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)if (
 fileInfo.isExecutable() ) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 executable->setChecked(true); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QGroupBox *ownerGroup = new QGroupBox(trUtf8("所有权")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *ownerLabel = new QLabel(trUtf8("所有者")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *ownerValueLabel = new QLabel(fileInfo.owner()); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *groupLabel = new QLabel(trUtf8("组")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *groupValueLabel = new QLabel(fileInfo.group()); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 groupValueLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QVBoxLayout *permissionsLayout = new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 permissionsLayout->addWidget(readable); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 permissionsLayout->addWidget(writable); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 permissionsLayout->addWidget(executable); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 permissionsGroup->setLayout(permissionsLayout);//权限组 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QVBoxLayout *ownerLayout = new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerLayout->addWidget(ownerLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerLayout->addWidget(ownerValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerLayout->addWidget(groupLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerLayout->addWidget(groupValueLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 ownerGroup->setLayout(ownerLayout);//所有权组 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QVBoxLayout *mainLayout = new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(permissionsGroup); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 mainLayout->addWidget(ownerGroup); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//mainLayout->addStretch(1); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 setLayout(mainLayout);    
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)} 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)//应用程序选项卡类构造函数实现 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)ApplicationsTab::ApplicationsTab(const QFileInfo
 &fileInfo, QWidget *parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  :QWidget(parent) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QLabel *topLabel = new QLabel(trUtf8("打开方式...:")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QListWidget *applicationsListBox = new QListWidget; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QStringList applications; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)for (int i
 = 1; i <= 30; ++i) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 applications.append(trUtf8("应用程序 %1").arg(i)); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 applicationsListBox->insertItems(0, applications); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QCheckBox *alwaysCheckBox; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)if (fileInfo.suffix().isEmpty()) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 alwaysCheckBox = new QCheckBox(trUtf8("始终使用该程序"
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)"打开该类型的文件")); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)else
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)                
 alwaysCheckBox = new QCheckBox(trUtf8("始终使用该程序"
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)"打开此扩展名文件
 '%1'").arg(fileInfo.suffix())); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 QVBoxLayout *layout = new QVBoxLayout; 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 layout->addWidget(topLabel); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 layout->addWidget(applicationsListBox); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 layout->addWidget(alwaysCheckBox); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)        
 setLayout(layout); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)} 
tabmain.cpp:
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include
 <QApplication> 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)#include "tabdlg.h"
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)int main(int argc,char*
 argv[]) 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif){ 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  QApplication
 app(argc,argv); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  TabDialog
 tabdlg("./src/tabmain.cpp"); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)  tabdlg.show(); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)return app.exec(); 
![](http://xcf007.blog.51cto.com/images/editer/InBlock.gif)} 
截图:
![](http://xcf007.blog.51cto.com/attachment/200810/200810201224475284546.gif)
![](http://xcf007.blog.51cto.com/attachment/200810/200810201224475296359.gif)
