# Qt深入浅出（六）设计师界面

2018年02月24日 00:24:34 [吓人的猿](https://me.csdn.net/qq769651718) 阅读数：6149



# Qt Designer

​	使用代码布置窗口不仅耗时，而且也不够直观，Qt Designer设计师工具可以帮我们摆脱这些繁琐的工作，Qt  designer 是一款可视化的ui设计工具。

​	Qt Designer可以编辑好一个设计师界面，设计师界面一般是一个.ui后缀的文件，需要在.pro文件中添加变量FORMS，然后指定.ui文件的路径。

​	ui文件是通过xml标签语言来编写的。

## 1 使用设计师界面

- 如果想在设计师可以在创建项目的时候勾选创建界面，默认是勾选的。

![img](https://img-blog.csdn.net/2018022400214764)

- 也可以单独添加一个界面文件，右键工程，添加新文件，选择Qt，Qt设计师界面类



![img](https://img-blog.csdn.net/2018022400220890)

这里有多个界面模板可供选择，选择其中一种，一直点击下一步即可。 ![img](https://img-blog.csdn.net/20180224002217729)



- 在Qt Designer中你可以任意拖拽控件进行布局





![img](https://img-blog.csdn.net/20180224002331392)

## 2 ui文件的编译过程

1. 首先我们可以查看.pro文件, 检查ui文件是否添加到.pro文件中，例如



![img](https://img-blog.csdn.net/20180224002358411)	如果已经添加，那么在使用qmake去执行解析.pro文件的过程中，就会生成对应的将ui文件转换成代码的Makefile。





1. 查看编译过程，例如：



![img](https://img-blog.csdn.net/20180224002416885) 明显多了一行命令，`C:\Qt\Qt5.7.0\5.7\mingw53_32\bin\uic.exe ..\class1_1\form.ui -o ui_form.h`

这个命令就是通过uic.exe工具将form.ui这个文件转换成ui_form.h代码文件。



1. 那在哪里使用了这个头文件呢，我们可以到生成的设计师界面类中查看，例如：form.cpp

```cpp
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
}

Form::~Form()
{
    delete ui;
}
```



​	我们可以Ctrl+鼠标右键单击，`#include "ui_form.h"`查看具体的实现。