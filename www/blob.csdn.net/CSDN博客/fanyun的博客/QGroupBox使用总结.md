# QGroupBox使用总结 - fanyun的博客 - CSDN博客
2017年12月30日 21:30:14[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4611
       QGroupBox为构建分组框提供了支持。分组框通常带有一个边框和一个标题栏，作为容器部件来使用，在其中可以布置各种窗口部件。布局时可用作一组控件的容器，但是需要注意的是，内部必须使用布局控件（如QBoxLayout）进行布局。
       QGroupBox继承关系：
![](https://img-blog.csdn.net/20171230212538931)
使用示例：
```cpp
groupBox = QGroupBox("Exclusive Radio Buttons")
radio1 = QRadioButton("&Radio button 1")
radio2 = QRadioButton("R&adio button 2")
radio3 = QRadioButton("Ra&dio button 3")
radio1.setChecked(True)
vbox = QVBoxLayout()
vbox.addWidget(radio1)
vbox.addWidget(radio2)
vbox.addWidget(radio3)
vbox.addStretch(1)
groupBox.setLayout(vbox)
```
![](https://img-blog.csdn.net/20171230212645410)
示例2：
```cpp
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QGroupBox *createFirstExclusiveGroup();
    QGroupBox *createSecondExclusiveGroup();
    QGroupBox *createNonExclusiveGroup();
    QGroupBox *createPushButtonGroup();
};
#endif // MAINWINDOW_H
```
```cpp
#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(createFirstExclusiveGroup(),0,0);
    grid->addWidget(createSecondExclusiveGroup(),0,1);
    grid->addWidget(createPushButtonGroup(),1,0);
    grid->addWidget(createNonExclusiveGroup(),1,1);
    ui->centralWidget->setLayout(grid);
    this->setWindowTitle(tr("Group Boxes"));
}
MainWindow::~MainWindow()
{
    delete ui;
}
QGroupBox *MainWindow::createFirstExclusiveGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"));
    QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));
    radio1->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    return groupBox;
}
QGroupBox *MainWindow::createSecondExclusiveGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("E&xclusive Radio Buttons"));
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    QRadioButton *radio1 = new QRadioButton(tr("Rad&io button1"));
    QRadioButton *radio2 = new QRadioButton(tr("Radi&o buttoon2"));
    QRadioButton *radio3 = new QRadioButton(tr("Radio &button3"));
    radio1->setChecked(true);
    QCheckBox *checkBox = new QCheckBox(tr("Ind&ependent checkbox"));
    checkBox->setChecked(true);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);
    vbox->addWidget(checkBox);
    groupBox->setLayout(vbox);
    return groupBox;
}
QGroupBox *MainWindow::createNonExclusiveGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("No-Exclusive Checkboxes"));
    groupBox->setFlat(true);
    QCheckBox *checkBox1 = new QCheckBox(tr("&Checkbox 1"));
    QCheckBox *checkBox2 = new QCheckBox(tr("C&heckbox 2"));
    checkBox2->setChecked(true);
    QCheckBox *tristateBox = new QCheckBox(tr("Tri-&state buttton"));
    tristateBox->setTristate(true);//是否启用三种状态切换
    tristateBox->setCheckState(Qt::PartiallyChecked);//介于Qt::Unchecked和Qt::Checked之间
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(checkBox1);
    vbox->addWidget(checkBox2);
    vbox->addWidget(tristateBox);
    vbox->addStretch(1);
    groupBox->setLayout(vbox);
    return groupBox;
}
QGroupBox *MainWindow::createPushButtonGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("&Push Button"));
    groupBox->setCheckable(true);
    groupBox->setChecked(true);
    QPushButton *pushButton = new QPushButton(tr("&Normal Button"));
    pushButton->setStyleSheet("background-color:rgb(35,65,90)");
    QPushButton *toggleButton = new QPushButton(tr("&Toggle Button"));
    toggleButton->setCheckable(true);
    toggleButton->setChecked(true);
    QPushButton *flatButton = new QPushButton(tr("&Flat Button"));
    flatButton->setFlat(true);
    flatButton->setStyleSheet("background-color:rgb(35,65,90)");
    QPushButton *popupButton = new QPushButton(tr("Pop&up Button"));
    QMenu *menu = new QMenu(this);
    menu->addAction((tr("&First Item")));
    menu->addAction((tr("&Second Item")));
    menu->addAction((tr("Third Item")));
    menu->addAction(tr("F&ourth Item"));
    popupButton->setMenu(menu);
    popupButton->setStyleSheet("background-color:rgb(35,65,90)");
    QAction *newAction = menu->addAction(tr("Submenu"));
    QMenu *subMenu = new QMenu(tr("Popup Submenu"));
    subMenu->addAction(tr("Item 1"));
    subMenu->addAction(tr("Item 2"));
    subMenu->addAction(tr("Item 3"));
    newAction->setMenu(subMenu);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(pushButton);
    vbox->addWidget(toggleButton);
    vbox->addWidget(flatButton);
    vbox->addWidget(popupButton);
    groupBox->setLayout(vbox);
    return groupBox;
}
```
![](https://img-blog.csdn.net/20171230212910249)
