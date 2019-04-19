# Qt中UI::MainWindow与MainWindow - LC900730的博客 - CSDN博客
2017年09月01日 18:03:57[lc900730](https://me.csdn.net/LC900730)阅读数：6394
在创建Qt widget Application中，可以选择创建带ui的或者不带ui的应用程序。 
如果选择带.ui的应用，则 
MainWindow是Ui命名空间的MainWindow。
```
namespace Ui{
class MainWindow;
}
class MainWindow:public QMainWidow{
    Q_OBJECT
    public :
        explict MainWindow(QWidget *parent=0);
        ~MainWindow();
    }
    private:
    Ui::MainWindow *ui;
}
```
如果是不带ui文件的界面
```
class MainWindow:public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent=0);
    ~MainWindow();
}
```
### centralWidget()
//此方法是QMainWindow的，当创建MainWindow继承自QMianWindow时候，就具有此方法。 
centralWidget()->setLayout()； 
如
```
QVBoxLayout *_central=new QVBoxLayout();
_centralLayout->setContentMargins(0,0,0,0);
```
