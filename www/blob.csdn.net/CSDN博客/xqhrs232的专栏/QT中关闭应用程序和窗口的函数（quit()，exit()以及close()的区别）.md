# QT中关闭应用程序和窗口的函数（quit()，exit()以及close()的区别） - xqhrs232的专栏 - CSDN博客
2017年10月12日 10:16:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1277
原文地址::[http://www.cnblogs.com/findumars/p/5830422.html](http://www.cnblogs.com/findumars/p/5830422.html)
使用QT编辑界面，其中带来很大方便的一点就是Qt中自带丰富的、种类齐全的类及其功能函数，程序员可以在编辑程序的过程中简单地直接调用。关于窗口关闭的操作，在这里指出常用的三个槽，即quit()，exit()以及close()。
    首先说明窗口退出时，系统提示对话框的代码编辑。对主程序的退出，可以调用成员函数exit(),同时也可以调用槽quit()，二者此时都能起到关闭应用程序的作用。只是应注意二者调用的方式不同。如下程序示例：
        {
         QApplication* app;
         app->exit(0);
         }
         或者：
       {
         QApplication* app;
         app->quit();
         }
    此时二者是等价的，即void QApplication::quit ()等价于函数调用 QApplication::exit( 0 )。此时，若需要给出用户提示，则只需要在程序当中添加QMessageBox的消息判断语句，以提示用户是否确定退出应用程序。另外，quit()作为槽，也可以连接信号和槽的形式，响应某个信号后关闭应用程序。如：
          QPushButton *quitButton = new QPushButton( "Quit" );
          connect( quitButton, SIGNAL(clicked()), qApp, SLOT(quit()) );
    如果关闭的不是应用程序，而是关闭窗口等部件的形式，则必须调用close()函数，已关闭部件。如下：
          if (!(QMessageBox::information(this,tr("CT Control View"),tr("Do you really want to log out CT Control View?"),tr("Yes"),tr("No"))))
         {
              this->close();
          }
    其中通过一条if语句，判断条件为一个对话框，根据用户的选择做出是否关闭该部件，this在这里代表当前窗口部件对象的地址。当用户选择“Yes”时，该窗口部件退出；反之则取消退出操作。
    至此，我们已经简单了解了应用程序和窗口等部件的退出或关闭的代码编辑。然而，如果用户点击窗口右上角的X按键时，你会发现窗口仍然会在不给出任何提示的情况下直接退出了，即便你在程序当中已添加了提示的对话框语句。那为什么此时的关闭操作没有按我们希望的那样先给出提示对话框呢？原因是此时的关闭操作并没有引起调用带有提示消息的关闭或退出语句。当用户点击X关闭窗口时，系统会自动将这个事件告之某个特定的函数，即void QWidget::closeEvent ( QCloseEvent * e )，因此，这种情况应特别注意。在关闭窗口前，可先定义一条消息语句，以提示用户是否确定关闭窗口。如：
  void mainWindow::closeEvent( QCloseEvent * event )
{
    switch( QMessageBox::information( this, tr("CT Control View"),
  tr("Do you really want to log out CT Control View?"),
  tr("Yes"), tr("No"),
  0, 1 ) )
 {
    case 0:
  event->accept();
        break;
    case 1:
 default:
        event->ignore();
        break;
 }
}
编译程序后，系统会在用户响应close操作时，自动调用该语句。也许这个时候你还会问，上面的那个功能函数究竟是把它定义成槽呢，还是定义成成员函数。答案是二者都可以。一方面槽本身就是一个特殊的成员函数，除了能与信号连接之外，其功能与函数无异。另一方面，此时不需要额外设置连接信号和槽，添加该功能函数之后，窗口一旦有了close操作，机会立即响应该。
尽管添加closeEvent( QCloseEvent * event )这一功能函数之后，窗口会在关闭时给出提示对话框。但有时候还会发生这样一种情况：在点击确认关闭后，还会再会出现一个对话框，即提示对话框会出现两次。那这又是什么原因呢？其实很简单，那是因为closeEvent( QCloseEvent * event )只会响应close的操作，出现两次对话框无疑是在自定义的关闭函数中又添加一条提示对话框语句，确认关闭后，响应close操作，这时系统又会马上调用closeEvent( QCloseEvent
 * event )这个函数。因此，解决的办法是只定义一个带提示对话框的关闭函数，对不同的关闭操作都相应closeEvent( QCloseEvent * event )这个函数即可。
最后，如果想要在关闭主窗口或主程序时，所有打开的独立的子窗口都能同时被关闭。那么这个时候一般是在main.cpp文件中连接信号void QApplication::lastWindowClosed ()；关于lastWindowClosed()的详细介绍，这里不作说明，需要使用时可以使用Qt Assistant查看其使用发法。如：
int main( int argc, char ** argv )
{
QApplication a( argc, argv );
ABMainWindow *mw = new ABMainWindow();
mw->setCaption( "Qt Example - Addressbook" );
a.setMainWidget( mw );
mw->show();
a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
int result = a.exec();
delete mw;
return result;
}
这样，系统会在关闭主窗口部件时，同时关闭打开的其它子窗口程序。比较值得说明的是，并不是使用了a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) )这条语句后就一定能实现该功能。在此之前，必须特别注意是否设定了主窗口部件，即调用setMainWidget这一功能函数，这样，当关闭这一主窗口部件时，才会响应关闭这一时刻已打开的其它子窗口部件。 
---
转载请注明本文标题和链接：《[QT中关闭应用程序和窗口的函数](http://www.iybin.com/?post=289)》

