# 谈ui文件的用法 - gauss的专栏 - CSDN博客
2013年01月03日 17:36:11[gauss](https://me.csdn.net/mathlmx)阅读数：250
                
相信用过Qt Designer的朋友，对Qt Project中的.ui文件并不陌生。这个文件在Qt Designer中并不能直接修改其源代码，而只能通过Qt Designer的图形工具对其进行操作。对于这一点，我不得不要赞一下设计Qt Designer的人，因为这大大可以避免.ui文件中出现语法错误的概率，同时使得程序员能省下大量的程序界面设计时间进而缩短整个程序项目的开发。
      从另一方面，我之所以想要谈谈这个.ui文件，是因为这个文件的使用方法不同于当今许多移动开发平台的类似文件的使用方法。从这一点上，我认为是一个相当好的创新。
      我相信大家都知道，每个Qt Project都是纯C++的，而如果我们用普通的文档编辑器打开.ui文件时，我们会发现.ui文件其实是个自定义标签的XML文件，那么这个文件对于整个Qt Project来说，怎么发挥它的作用呢？难道说Qt的C++编译器能把它直接转换成C++语言，然后进行编译吗？带着这个问题，我进行了探索，发现却是另一种结果。
      我们可以先用Qt Creator创建一个带有ui文件的GUI Project，创建过程中Qt Creator会让我们选择这个窗口类是基于QMainWindow、QWidget还是QDialog。我们随便选一个，在这我选的是QMainWindow。然后，我们可以得到5个文件，一个.pro文件，一个.ui文件，一个.h文件，两个.cpp文件，其中一个是main.cpp，其包含着Qt程序的入口函数main。
      而这时，如果我们打开.h文件的话，我们会看到如下一段声明：
           namespace Ui {
           class MainWindow;    }  
    这的意思是说，在命名空间Ui里面有一个类叫MainWindow，可是这个类的描述在哪呢，不清楚，但肯定不在当前这个.h文件中。但是我们可以在后面的类描述中发现，其中会有一个Ui::MainWindow的私有指针，既然是这样，估计在对应的.cpp文件中，应该能找到出现这个Ui::MainWindow的描述的.h文件，不出所料，在对应的.cpp中，就发现了这个文件：
     #include "ui_mainwindow.h"     
    但是，如果这时你还没有build过的话，你会发现这个文件是找不到的，那么我们就来build一下这个Project吧。结果一build之后，我们就能看到这个文件了。打开后，我们会看到Ui::MainWindow的描述：  
   namespace Ui {           
   class MainWindow: public Ui_MainWindow {};
     } // namespace Ui
      这就是这个类的描述，简短而精悍，而在它的上面就是Ui_MainWindow类的描述了，而这个Ui_MainWindow正是按着.ui文件的设计通过uic工具生成的。那么这个类该怎么用呢？
      我们知道在Qt Project中，窗口类的实现有三种途径，要么继承QMainWindow，或是继承QWidget，再或是继承QDialog。而对于类似于Ui::MainWindow类的使用则有两种方法：
      第一种方法：假设我们的真正的窗口类叫MainWindow，它继承于QMainWindow，那么它可以有一个Ui::MainWindow的私有成员，并在MainWindow的构造函数中，实例化这个私有的Ui::MainWindow，之后调用这个私有的Ui::MainWindow的setupUi方法，设置MainWindow的用户界面接口，即按.ui文件的设计初始化MainWindow的界面。这样，一个按照.ui文件设计的界面的窗口就建立起来了。
      第二种方法：仍然假设我们的真正的窗口类叫MainWindow，它仍然要选择QMainWindow，QWidget，QDialog中的一个类进行继承。而与前一种方法不同的是，这个MainWindow类不需要一个Ui::MainWindow类的私有成员，而是使用了C++中的多重继承，让MainWindow同时继承Ui::MainWindow。而这时只需在要使用MainWindow的时候实例化它，在它的构造函数中调用setupUi方法，即可。
      这两种方法，在正常情况下，虽然就最后窗口的显示效果来说没有什么区别，但在内存的管理机制上，却有不同。从内存管理的安全性方面来说，用惯了Symbian的我更倾向于使用后一种方法。因为第一种方法中先实例化Ui::MainWindow后，无法保证在接下来的构造函数中，有可能因为内存不足而引发构造函数异常退出，进而导致Ui::MainWindow成为内存泄漏。虽然说Qt有自己的内存垃圾站的处理机制来解决内存泄漏问题，但是就个人感觉而言，这种机制的效率终究赶不上人为释放内存的效率。
      另外还想说的一点，就是虽然现在很多移动开发平台都在使用类似.ui文件的XML格式的文件作为窗口的界面设计使用，但存在两点问题，第一，有很多平台需要程序员直接去写这个XML格式的文件，而没有类似Qt Designer的工具，这使得程序员写的很头痛，并且因此而流失了许多开发时间；第二，很少有平台想Qt这样先把这个XML格式的.ui文件转化为C++文件再进行编译的，很多都是直接丢给操作系统，在运行程序时才去解析，得到相应数据后才能产生出相应的程序界面，这无疑又降低了程序的运行效率。
      此上就是个人在研究Qt开发过程中的一些心得体会，在此分享给大家。有不足的地方还希望大家多提建议
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/luo_isaiah/archive/2010/08/08/5794973.aspx](http://blog.csdn.net/luo_isaiah/archive/2010/08/08/5794973.aspx)
