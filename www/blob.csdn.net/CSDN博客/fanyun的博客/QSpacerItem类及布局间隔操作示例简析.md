# QSpacerItem类及布局间隔操作示例简析 - fanyun的博客 - CSDN博客
2018年03月25日 22:16:40[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：154

# QSpacerItem类是Qt的一个布局操作类。它提供了布局中的空白空间。通常情况下，你不需要直接使用这个类。Qt内置的布局管理器提供以下函数来处理布局中的空白：
     如QHBoxLayout中提供：addSpacing(), addStretch(), insertSpacing(), insertStretch()四种操作。QGridLayout布局中提供：setRowMinimumHeight(), setRowStretch(), setColumnMinimumWidth(), setColumnStretch()四种操作。
     下面举个例子：
首先，默认比例显示：
```cpp
QStretch::QStretch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QStretch)
{
    ui->setupUi(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    btu1 = new QPushButton("one");
    btu2 = new QPushButton("Two");
    btu3 = new QPushButton("Three");
    qline = new QLineEdit(this);
    qcom = new QComboBox(this);
    spin1 = new QSpinBox(this);
    buttonLayout->addWidget(btu1);
    buttonLayout->addWidget(btu2);
    buttonLayout->addWidget(btu3);
    buttonLayout->addWidget(qline);
    buttonLayout->addWidget(qcom);
    buttonLayout->addWidget(spin1);
    buttonLayout->setContentsMargins(0,0,0,0);
    ui->centralWidget->setLayout(buttonLayout);
}
```
显示效果如下：
![](https://img-blog.csdn.net/20180321143239136)
下面我们按照1:1:1:1:1:1:20的间隔大小来调整组件的空白显示间隔比例：
```cpp
QStretch::QStretch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QStretch)
{
    ui->setupUi(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    btu1 = new QPushButton("one");
    btu2 = new QPushButton("Two");
    btu3 = new QPushButton("Three");
    qline = new QLineEdit(this);
    qcom = new QComboBox(this);
    spin1 = new QSpinBox(this);
    buttonLayout->addStretch(1);//增加伸缩量
    buttonLayout->addWidget(btu1);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(btu2);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(btu3);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(qline);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(qcom);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(spin1);
    buttonLayout->addStretch(20);
    buttonLayout->setContentsMargins(0,0,0,0);
    ui->centralWidget->setLayout(buttonLayout);
}
```
显示效果如下：
![](https://img-blog.csdn.net/20180321144326702)
下面我们按照比例：1:2:3:4:5:6的组件间隔比例去调整组件的位置
```cpp
QStretch::QStretch(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QStretch)
{
    ui->setupUi(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(this);
    btu1 = new QPushButton("one");
    btu2 = new QPushButton("Two");
    btu3 = new QPushButton("Three");
    qline = new QLineEdit(this);
    qcom = new QComboBox(this);
    spin1 = new QSpinBox(this);
    buttonLayout->addStretch(1);//增加伸缩量
    buttonLayout->addWidget(btu1);
    buttonLayout->addStretch(2);
    buttonLayout->addWidget(btu2);
    buttonLayout->addStretch(3);
    buttonLayout->addWidget(btu3);
    buttonLayout->addStretch(4);
    buttonLayout->addWidget(qline);
    buttonLayout->addStretch(5);
    buttonLayout->addWidget(qcom);
    buttonLayout->addStretch(6);
    buttonLayout->addWidget(spin1);
    buttonLayout->addStretch(1);
    buttonLayout->setContentsMargins(0,0,0,0);
    ui->centralWidget->setLayout(buttonLayout);
}
```
![](https://img-blog.csdn.net/20180321142858234)
