# VS2010+QT5编程之为QTableView添加右键菜单 - xqhrs232的专栏 - CSDN博客
2018年05月16日 17:48:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：254
原文地址::[https://blog.csdn.net/godmaycry/article/details/46415581](https://blog.csdn.net/godmaycry/article/details/46415581)
相关文章
1、QT 鼠标右键菜单----[https://blog.csdn.net/TJU355/article/details/7652327](https://blog.csdn.net/TJU355/article/details/7652327)
QTableView是一个比较实用的类，下面教给大家如何在QTableView中添加右键菜单。
[cpp][view plain](https://blog.csdn.net/godmaycry/article/details/46415581#)[copy](https://blog.csdn.net/godmaycry/article/details/46415581#)
- #include <QMenu>
- #include <QAction>
- 
- QTableView *tableview;  
- QMenu *rightMenu;  //右键菜单
- QAction *cutAction;  //剪切
- QAction *copyAction;  //复制
- QAction *pasteAction;  //粘贴
- QAction *deleteAction;  //删除
- 
- private slots:  
- void clicked_rightMenu(const QPoint &pos);  //右键信号槽函数
[cpp][view plain](https://blog.csdn.net/godmaycry/article/details/46415581#)[copy](https://blog.csdn.net/godmaycry/article/details/46415581#)
- tableview->setContextMenuPolicy(Qt::CustomContextMenu);  //少这句，右键没有任何反应的。
- createRightMenu();  //创建一个右键菜单
- connect(tableview,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(clicked_rightMenu(QPoint)));  
其中createRightMenu函数：
[cpp][view plain](https://blog.csdn.net/godmaycry/article/details/46415581#)[copy](https://blog.csdn.net/godmaycry/article/details/46415581#)
- rightMenu = new QMenu；  
- cutAction = new QAction("剪切",this);  
- copyAction = new QAction("复制",this);  
- pasteAction = new QAction("粘贴",this);  
- deleteAction = new QAction("删除",this);  
- 
- rightMenu->addAction(cutAction);  
- rightMenu->addAction(copyAction);  
- rightMenu->addAction(pasteAction);  
- rightMenu->addAction(deleteAction);  
clicked_rightMenu槽函数：
[cpp][view plain](https://blog.csdn.net/godmaycry/article/details/46415581#)[copy](https://blog.csdn.net/godmaycry/article/details/46415581#)
- rightMenu->exec(QCursor::pos());  
- 
