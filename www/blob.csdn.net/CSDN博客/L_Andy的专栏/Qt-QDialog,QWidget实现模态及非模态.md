# Qt - QDialog,QWidget实现模态及非模态 - L_Andy的专栏 - CSDN博客

2017年12月08日 23:23:17[卡哥](https://me.csdn.net/L_Andy)阅读数：320


对于 QDialog 的模态及非模态是直接可以实现的，很多课本中都会提到，此处总结下。

模态QDialog 

方式一：

```
QDialog dlg(this);
dlg.exec();
```

方式二：

```
QDialog *pDlg=new QDialog(this); 
pDlg->setModal(true);
pDlg->show();
```

非模态QDialog

```
QDialog *pDlg=new QDialog(this); 
pDlg->show();
```

QDialog实现模态非模态很简单，但是对于QWidget有点迷茫，QWidget中没有exec()，也没有setModal()方式，但是想想看，QWidget作为QDialog的基类，而且QWidget作为“窗口”使用也是在平常不过了，所以会意识到QWidget中是否存在一个相对exec()或setModal()更基本的操作来实现模态和非模态呢？就这样，我找到了setWindowModality()，此函数就是用来设置QWidget运行时的程序阻塞方式的，参数解释如下：

```
Qt::NonModal 不阻塞
Qt::WindowModal 阻塞父窗口，所有祖先窗口及其子窗口
Qt::ApplicationModal 阻塞整个应用程序
```

看来，setModal()也就是使用setWindowModality()设置Qt::ApplicationModal参数也实现的模态。

如此，要实现QWidget的模态和非模态，只要调用setWindowModality()设置阻塞类型就好了：

```
QWidget *pWid = new QWidget(this);
pWid->setWindowModality(Qt::ApplicationModal);
//pWid->setAttribute(Qt::WA_ShowModal, true);
pWid->show();
```

但是运行发现并未实现模态效果。这里需要注意，当希望使用setWindowModality()将QWidget设置为模态时应该保证QWidget父部件为0，这里修改QWidget *pWid = new QWidget(this);为QWidget *pWid = new QWidget(NULL);在运行就好了。

此外，通过setWindowModality()设置模态窗口并不是唯一方式，直接设置部件（或窗口）属性也可以：
`pWid->setAttribute(Qt::WA_ShowModal, true)`
还有很多地方需要注意，当创建QDialog后使用

```bash
setWindowFlags(Qt::FramelessWindowHint);
```

去掉标题栏时此对话框不再阻塞父窗口，如果需要实现阻塞效果可再次指定Qt::Dialog，即使用：
`setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint); //这样就会阻塞父窗口了！`
但是，这样会影响对话框的半透明（或透明）显示。使用Qt::Dialog之前半透明显示正常：![这里写图片描述](https://img-blog.csdn.net/20171208232029863?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Qt - QDialog,QWidget实现模态及非模态  

使用之后却死活不行了： 
![这里写图片描述](https://img-blog.csdn.net/20171208232106511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Qt - QDialog,QWidget实现模态及非模态  

不知道如何是好！ 

总而言之 

是否是模态和QDialog 和QWidget都可以模态和非模态.exec(), show() 等函数无直接关系，只和窗口属性有关,使用以下两种方式都行：

```
setAttribute(Qt::WA_ShowModal, true);//属性设置
setWindowModality(Qt::ApplicationModal);//设置阻塞类型
```

QDialog中的成员函数setModal(true)及exec()之所以是模态是因为他先设置了窗口属性:setAttribute()再show()的(具体看源码)！

1.如果使用exec（）默认为模态的。如果用show（）需要设置setModel（true）才是模态的。

2.当删除一个dialog对象时，exec（）返回QDialog：：Rejected，因此在删除dialog时要特别小心。举例说明一下：

A 是一个QWidget 。B是一个QDialog，B = new QDialog（A）。

```
int rec = B->exec();
    if (!thisDeleted)
    {
        if (rec == QDialog::Accepted)
            emit endSession();

    if (rec == QDialog::Rejected)
            emit endSession2();   

}
```

endSession（）和 endSession2（）都是A得成员函数。当删除A时，B也会删除（因为B得parent是A），这个时候exec（）会返回 QDialog：：Rejected，此时下面的 emit endSession2()还会继续执行，这个时候就会出现端错误了，因为A比B先删除，而此时还要调用A的成员，A都被删除了，还能调用其成员函数？要 是能调用就奇了怪了。因此在类A中多了个静态变量thisDeleted，记得在析构A的时候把这个值改变了，这样就不会在调用emit endSession2()，也就不会出现段错了。

3.exec（）是一个循环时间函数，哪它什么时候才能返回了？当调用 accept（）（返回QDialog::Accepted）， reject（）（返回QDialog::Rejected），done（int r）（返回r），close（）（返回QDialog::Rejected），hide（）（返回 QDialog::Rejected），destory（）（返回QDialog::Rejected）。还有就是delete 自己的时候也会返回 QDialog::Rejected（destory（）就会delete自己）。

4.close（）会销毁对象嘛。答案是只有当设置了 setAttribute（WA:closeOnDelelte）是才会删除，如果这个标志没有设置，其作用和hide（），setvisible（false）一样，只会隐藏改对象。

5.当dialog在exec（）时候。后台线程同样可以操作界面，如收到每个消息，还可以弹出QDialog

