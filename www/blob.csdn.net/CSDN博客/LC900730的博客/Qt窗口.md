# Qt窗口 - LC900730的博客 - CSDN博客
2017年08月30日 10:25:00[lc900730](https://me.csdn.net/LC900730)阅读数：157
## 窗口类型
QWidget的构造函数有2个参数 
QWidget *parent=0和Qt::WindowFlags f=0 
parent指的是父窗口控件，默认是0，表示没有父窗口； 
而f参数是Qt::WindowFlags类型的，是一个枚举类型，分为窗口类型(WindowType)和窗口标志(WindowFlags)。前者定义窗口的类型，如f=0表明使用了Qt::widget一项，这是QWidget的默认类型，这种类型的部件如果没有父窗口，那么它就是子部件，否则就是独立窗口。
```
QWidget * widget=new QWidget(0,Qt::Dialog);
QLabel *label=new QLabel(0,Qt::SplashScreen);
```
### 窗口的几何布局
widget默认的大小就是它所包含的子部件label2的大小，而widget和label出现时在窗口上的位置也是不确定的。
### QDialog
QDialog类是所有对话框窗口类的基类。对话框窗口是一个经常用来完成一个短小任务或者和用户进行简单交互的顶层窗口。
```
MyWidget::MyWidget(QWidget * parent):QWidget(parent),ui(new Ui::MyWidget){
    ui->setupUi(this);
    QDialog dialog(this);
    dialog.show();
}
```
#### 构造函数中定义的局部对象
MyWidget类中定义了一个QDialog类对象，还指定了dialog的父窗口为MyWidget类对象，就是这个this参数，最后调用show()函数让其显示。 
窗口一闪而过的原因是在这个构造函数中定义的变量，等函数执行完成后，就自动释放了。也就是这个dialog对象只在构造函数中有用
#### new开辟空间的指针对象
更改如下： 
QDialog *dialog=new QDialog(this); 
dialog->show(); 
使用QDialog对象的指针，并且使用new开辟内存空间。 
定义一个对象：QDialog dialog，但是QDialog *dialog不能称为定义对象，而是定义了一个指向QDialog类对象的指针变量。
#### 模态与非模态
要想使得一个对话框成为模态对话框，只需要调用它的exec()函数，而要使其成为非模态对话框，可以使用new操作来创建，然后使用show()函数来显示。其实使用show()函数也可以建立模态对话框，只需要在其前面加入setModal()函数即可。 
QDialog *dialog=new QDialog(this); 
dialog->setModal(true); 
dialog->show(); 
与exec()函数不同的是：父窗口MyWidget对象窗口也显示出来了。因为调用完show()函数后会立即把控制权交给调用者，那么程序会继续往下执行。而调用exec()函数却不同，只有当对话框被关闭时才会返回。
## 多窗口切换
信号与槽： 
槽必须声明为slots,这里使用了public slots，表明这个槽可以在类外被调用。
connect()函数将按钮的clicked()与新建的槽进行关联。
```
//@param1:发送信号的对象
//@param2:发送的信号
//@param3:接收信号的对象
//@param4:要执行的槽
connect(ui->showChildButton,SIGNAL(clicked()),this,SLOT(showChildDialog()))；
//connect()是QObject类中的函数，因为类继承QObject，所以可以直接使用。
```
### 标准对话框
- 颜色对话框类QColorDialog
```
//@param1:设置初始颜色
//@param2:父窗口
//@param3:对话框标题
QColor    color=
 QColorDialog::getColor(Qt::red,this,tr("颜色对话框"))；
```
- 文件对话框QFileDialog
```
//@param1:指定父窗口
//@param2:设置对话框标题
//@param3:指定默认打开的目录路径
QString fileName= QFileDialog::getOpenFileName(this,tr("文件对话框")，“F:”,tr("图片文件(*png  *jpg)"));
//多个文件
tr("图片文件(*png  *jpg);;文本文件( *txt)"));
```
