# Qt事件过滤器详解 - fanyun的博客 - CSDN博客
2016年11月11日 15:50:51[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：826

**Qt的事件模型一个强大的功能是:一个QObject对象能够监视发送其他QObject对象的事件，在事件到达之前对其进行处理。**
**假设我们有一个CustomerInfoDialog控件，由一些QLineEdit控件组成。我们希望使用Space键得到下一个QLineEdit的输入焦点。**
**一个最直接的方法是继承QLineEdit重写keyPressEvent()函数，当点击了Space键时，调用focusNextChild()：**
**[cpp]**[view
 plain](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[copy](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)
[print](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[?](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2025035/fork)
- void MyLineEdit::keyPressEvent(QKeyEvent *event)  
- {  
- if (event->key() == Qt::Key_Space)   
-     {  
-         focusNextChild();  
-     }   
- else
-     {  
-         QLineEdit::keyPressEvent(event);  
-     }  
- }  
**这个方法有一个最大的缺点：如果我们在窗体中使用了很多不同类型的控件（QComboBox，QSpinBox等等），**
**我们也要继承这些控件，重写它们的keyPressEvent()。一个更好的解决方法是让CustomerInfoDialog监视其子控件的键盘事件，**
**在监视代码处实现以上功能。这就是事件过滤的方法。实现一个事件过滤包括两个步骤：**
**1.      在目标对象上调用installEventFilter()，注册监视对象。**
**2.      在监视对象的eventFilter()函数中处理目标对象的事件。**
**注册监视对象的位置是在CustomerInfoDialog的构造函数中：**
**[cpp]**[view
 plain](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[copy](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)
[print](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[?](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2025035/fork)
- CustomerInfoDialog::CustomerInfoDialog(QWidget *parent) : QDialog(parent)  
- {  
-     ...  
-     firstNameEdit->installEventFilter(this);  
-     lastNameEdit->installEventFilter(this);  
-     cityEdit->installEventFilter(this);  
-     phoneNumberEdit->installEventFilter(this);  
- }  
**事件过滤器注册后，发送到firstNameEdit，lastNameEdit，cityEdit，phoneNumberEdit控件的事件首先到达CustomerInfoDialog::eventFilter()函数，然后在到达最终的目的地。**
**下面是eventFilter()函数的代码：**
**[cpp]**[view
 plain](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[copy](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)
[print](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)[?](http://blog.csdn.net/rl529014/article/details/53446455?ref=myread#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/2025035/fork)
- bool CustomerInfoDialog::eventFilter(QObject *target, QEvent *event)  
- {  
- if (target == firstNameEdit || target == lastNameEdit  
-             || target == cityEdit || target == phoneNumberEdit)  
-     {  
- if (event->type() == QEvent::KeyPress)  
-         {  
-             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);  
- if (keyEvent->key() == Qt::Key_Space)   
-             {  
-                 focusNextChild();  
- returntrue;  
-             }  
-         }  
-     }  
- return QDialog::eventFilter(target, event);  
- }  
**首先，我们看是目标控件是否为QLineEdit，如果事件为键盘事件，把QEvent转换为QKeyEvent，确定被敲击的键。如果为Space键，调用focusNextChild()，把焦点交给下一个控件，返回true通知Qt已经处理了这个事件，如果返回false，Qt将会把事件传递给目标控件，把一个空格字符插入到QLineEdit中。**
**如果目标控件不是QLineEdit，或者事件不是Space敲击事件，把控制权交给基类QDialog的eventFilter()。目标控件也可以是基类QDialog正在监视的控件。（在Qt4.1中，QDialog没有监视的控件，但是Qt的其他控件类，如QScrollArea，监视一些它们的子控件）**
**Qt的事件处理有5中级别：**
**1. 重写控件的事件处理函数：如重写keyPressEvent()，mousePressEvent()和paintEvent()，这是最常用的事件处理方法，我们已经看到过很多这样的例子了。**
**2.  重写QObject::event()，在事件到达事件处理函数时处理它。在需要改变Tab键的惯用法时这样做。也可以处理那些没有特定事件处理函数的比较少见的事件类型（例如，QEvent::HoverEnter）。我们重写event()时，必须要调用基类的event()，由基类处理我们不需要处理的那些情况。**
**3.  给QObject对象安装事件过滤器：对象用installEventFilter()后，所有达到目标控件的事件都首先到达监视对象的eventFilter()函数。如果一个对象有多个事件过滤器，过滤器按顺序激活，先到达最近安装的监视对象，最后到达最先安装的监视对象。**
**4. 给QApplication安装事件过滤器，如果qApp（唯一的QApplication对象）安装了事件过滤器，程序中所有对象的事件都要送到eventFilter()函数中。这个方法在调试的时候非常有用，在处理非活动状态控件的鼠标事件时这个方法也很常用。**
**5.  继承QApplication，重写notify()。Qt调用QApplication::nofity()来发送事件。重写这个函数是在其他事件过滤器处理事件前得到所有事件的唯一方法。通常事件过滤器是最有用的，因为在同一时间，可以有任意数量的事件过滤器，但是notify()函数只有一个。**
**许多事件类型，包括鼠标，键盘事件，是能够传播的。如果事件在到达目标对象的途中或者由目标对象处理掉，事件处理的过程会重新开始，不同的是这时的目标对象是原目标对象的父控件。这样从父控件再到父控件，知道有控件处理这个事件或者到达了最顶级的那个控件。**
下图显示了一个键盘事件在一个对话框中从子控件到父控件的传播过程。当用户敲击一个键盘，时间首先发送到有焦点的控件上（这个例子中是QCheckBox）。如果QCheckBox没有处理这个事件，Qt把事件发送到QGroupBox中，如果仍然没有处理，则最后发送到QDialog中。
![](https://img-blog.csdn.net/20161203180233810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
