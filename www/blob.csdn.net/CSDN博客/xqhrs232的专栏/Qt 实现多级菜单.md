# Qt 实现多级菜单 - xqhrs232的专栏 - CSDN博客
2018年05月17日 15:38:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：316
原文地址::[https://blog.csdn.net/heaven_evil/article/details/78601478](https://blog.csdn.net/heaven_evil/article/details/78601478)
# 前言
在众多知名的软件UI中下拉菜单、按钮菜单、右键菜单、托盘菜单、多级菜单等等是很常见的，就连Windows和linux等系统级的软件也用到这些UI元素。
下图就是我们最常见的Windows系统多级菜单：
![](https://img-blog.csdn.net/20171122142111936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGVhdmVuX0V2aWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# Qt实现（多级）菜单
在Qt中通过QMenu和QAction就可以实现这些菜单，QMenu就是上图中我们看到的三个级联菜单，每个QMenu里面的小项目就是一个QAction，一个QAction由一个图标和一段文字描述组成。要先右键弹出菜单我们需要重写contextMenuEvent(QContextMenuEvent *event)事件，如果是按钮菜单等关联信号槽，在槽函数弹出菜单即可。
# 源码分析
[cpp][view plain](https://blog.csdn.net/heaven_evil/article/details/78601478#)[copy](https://blog.csdn.net/heaven_evil/article/details/78601478#)
- void Widget::createMenu()  
- {  
-     m_menu1 = new QMenu();  
-     m_menu1->addAction("1_1",this,SLOT(actionsSlot()));  
-     m_menu1->addAction("1_2",this,SLOT(actionsSlot()));  
-     m_menu1->addAction("1_3",this,SLOT(actionsSlot()));  
- 
-     QMenu* menu2 = m_menu1->addMenu("2_0");  
-     menu2->addAction("2_1",this,SLOT(actionsSlot()));  
- 
-     QMenu* menu3 = menu2->addMenu("3_0");  
-     menu2->addSeparator();  
-     menu3->addAction("3_1",this,SLOT(actionsSlot()));  
-     menu3->addAction("3_2",this,SLOT(actionsSlot()));  
-     menu3->addAction("3_3",this,SLOT(actionsSlot()));  
- 
-     menu2->addAction("2_2",this,SLOT(actionsSlot()));  
-     menu2->addAction("2_3",this,SLOT(actionsSlot()));  
- 
-     m_menu1->addSeparator();  
- 
-     m_menu1->addAction("1_4",this,SLOT(actionsSlot()));  
- }  
createMenu()函数中创建了三个菜单，通过addAction()函数添加小项目到每个菜单中,addAction()同时提供关联到对象槽函数的重载版本，这样就可以在添加的同时也关联好槽函数，addMenu()给菜单添加下级菜单，当鼠标滑到addMenu()添加的项目是会自动弹出下级菜单。addSeparator()在每个QAction之间添加分离器。通过addAction()和addMenu()的结合使用就可以构造看起来很强大的多级菜单，再通过实现它们的槽函数我们就可以实现每个QAction被点击时要响应的事件。

[cpp][view plain](https://blog.csdn.net/heaven_evil/article/details/78601478#)[copy](https://blog.csdn.net/heaven_evil/article/details/78601478#)
- void Widget::actionsSlot()  
- {  
-     QAction *action = (QAction*)sender();  
-     QString actionText = action->text();  
- 
-     qDebug()<<actionText<<"triggerred!"<<endl;  
- 
- if("1_1" == actionText) {  
- //...
-     } elseif("1_2" == actionText) {  
- //...
-     }  
- //...
- }  
该槽函数处理QAction点击的时候要处理的事情。通过sender()取得发送信号的对象，再取得QAction的属性就可以在代码逻辑中分别处理不同的QAction该处理的事情。
[cpp][view plain](https://blog.csdn.net/heaven_evil/article/details/78601478#)[copy](https://blog.csdn.net/heaven_evil/article/details/78601478#)
- void Widget::contextMenuEvent(QContextMenuEvent *event)  
- {  
-     m_menu1->exec(QCursor::pos());  
-     event->accept();  
- }  
重写contextMenuEvent()事件用于处理鼠标右键事件，但右击鼠标时，在鼠标点击的位置弹出菜单。
# 运行截图
![](https://img-blog.csdn.net/20171122145014050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGVhdmVuX0V2aWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
源码下载地址：[Qt多级菜单（右键菜单）](http://download.csdn.net/download/heaven_evil/10128619)
