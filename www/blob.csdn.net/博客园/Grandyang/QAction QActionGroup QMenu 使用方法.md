# QAction QActionGroup QMenu 使用方法 - Grandyang - 博客园







# [QAction QActionGroup QMenu 使用方法](https://www.cnblogs.com/grandyang/p/4663277.html)







在Qt中，QAction，QActionGroup 和 QMenu类用用来操作软件的菜单栏的，比如很多软件都有文件菜单，里面有打开，保存，另存为之类的选项，在Qt中就是通过这几个类来实现的。或者是在菜单中实现一些选项设置，多选或单选什么的。具体使用方法如下所示：



```
// Define QActionGroup
QActionGroup *group = new QActionGroup(this);
group->setExclusive(true);

// Add QAction
QAction *action1 = new QAction("Item1", group);
action1->setCheckable(true);
action1->setChecked(true);
ui->menuItem->addAction(action1);

QAction *action2 = new QAction("Item2", group);
action2->setCheckable(true);
ui->menuItem->addAction(action2);

// Connection
connect(group, SIGNAL(triggered(QAction*)), this, SLOT(updateItem(QAction*)));

// Respond function
void QtMainWindow::updateItem(QAction *action)
{
    if (action->text() == "Item1") {
        // TO DO
    }
    if (action->text() == "Item2") {
        // TO DO
    }
}

// Trigger function
void QtMainWindow::on_actionSave_triggered() {
    // TO DO
}
```














