# Qt开发：Qt Widgets模块——QApplication类 - 一世豁然的专栏 - CSDN博客





2018年05月18日 20:15:31[一世豁然](https://me.csdn.net/Explorer_day)阅读数：1364








QApplication类管理GUI应用程序的控制流和主要设置。



头文件：#include <QApplication> 

qmake：QT += widgets

继承：QGuiApplication

QApplication专门为QGuiApplication提供基于QWidget的应用程序所需的一些功能。 它处理小部件特定的初始化，最终化。

对于任何使用Qt的GUI应用程序，只需一个QApplication对象，无论应用程序在任何给定时间是否有0,1,2或更多窗口。 对于基于非QWidget的Qt应用程序，请改用QGuiApplication，因为它不依赖于QtWidgets库。


某些GUI应用程序提供特殊的批处理模式，即。 为执行任务提供命令行参数而无需手动干预。 在这种非GUI模式下，实例化简单QCoreApplication通常就足以避免不必要地初始化图形用户界面所需的资源。 以下示例显示如何动态创建适当类型的应用程序实例：


```cpp
QCoreApplication* createApplication(int &argc, char *argv[])
  {
      for (int i = 1; i < argc; ++i)
          if (!qstrcmp(argv[i], "-no-gui"))
              return new QCoreApplication(argc, argv);
      return new QApplication(argc, argv);
  }

  int main(int argc, char* argv[])
  {
      QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

      if (qobject_cast<QApplication *>(app.data())) {
         // start GUI version...
      } else {
         // start non-GUI version...
      }

      return app->exec();
  }
```

QApplication对象可以通过instance（）函数访问，该函数返回一个相当于全局qApp指针的指针。

QApplication的主要责任领域是：


1）、它使用用户的桌面设置（如palette（），font（）和doubleClickInterval（））初始化应用程序。 它会跟踪这些属性，以防用户在全局范围内更改桌面，例如通过某种控制面板。

2）、它执行事件处理，这意味着它从底层窗口系统接收事件并将它们分派到相关的小部件。 通过使用sendEvent（）和postEvent（），您可以将自己的事件发送到小部件。

3）、它解析常用命令行参数并相应地设置其内部状态。 有关更多详细信息，请参阅下面的构造器文档

4）、它定义了应用程序的外观和感觉，它被封装在一个QStyle对象中。 这可以在运行时使用setStyle（）进行更改。

5）、它指定应用程序如何分配颜色。 有关详细信息，请参阅setColorSpec（）。

6）、它通过translate（）提供用户可见的字符串本地化。

7）、它提供了一些神奇的对象，如桌面（）和剪贴板（）。

8）、它知道应用程序的窗口。 您可以使用widgetAt（）来查询哪个小部件位于特定位置，并获取topLevelWidgets（）和closeAllWindows（）等的列表

9）、它管理应用程序的鼠标光标处理，请参阅setOverrideCursor（）

由于QApplication对象执行了很多初始化，因此必须在创建与用户界面相关的任何其他对象之前创建它。 QApplication还处理常见的命令行参数。 因此，在应用程序本身对argv进行任何解释或修改之前创建它通常是一个好主意。


函数组；


|系统设置|desktopSettingsAware(), setDesktopSettingsAware(), cursorFlashTime(), setCursorFlashTime(), doubleClickInterval(), setDoubleClickInterval(), setKeyboardInputInterval(), wheelScrollLines(), setWheelScrollLines(), palette(), setPalette(), font(), setFont(), fontMetrics().|
|----|----|
|事件处理|exec(), processEvents(), exit(), quit(). sendEvent(), postEvent(), sendPostedEvents(), removePostedEvents(), hasPendingEvents(), notify().|
|GUI样式|style(), setStyle().|
|颜色用法|colorSpec(), setColorSpec().|
|文本处理|installTranslator(), removeTranslator() translate().|
|小部件|allWidgets(), topLevelWidgets(), desktop(), activePopupWidget(), activeModalWidget(), clipboard(), focusWidget(), activeWindow(), widgetAt().|
|高级光标处理|overrideCursor(), setOverrideCursor(), restoreOverrideCursor().|
|杂项|closeAllWindows(), startingUp(), closingDown().|







一、成员类型

1、enum QApplication::ColorSpec


|内容|值|描述|
|----|----|----|
|QApplication::NormalColor|0|默认的颜色分配策略|
|QApplication::CustomColor|1|与X11的NormalColor相同; 在Windows下按需分配颜色到调色板|
|QApplication::ManyColor|2|正确选择使用数千种颜色的应用程序|




二、属性

1、autoMaximizeThreshold : int

该属性为自动最大化小部件定义了一个阈值。


自动最大化阈值仅作为Windows CE的Qt的一部分提供。


此属性为窗口大小定义了一个阈值，以屏幕大小的百分比表示。 如果窗口的最小大小提示超过了阈值，调用show（）将使窗口自动最大化。


将阈值设置为100或更高意味着小部件将始终最大化。 或者，将阈值设置为50意味着只有垂直最小尺寸提示至少为垂直屏幕尺寸的50％时，才会最大化控件。


将阈值设置为-1会禁用该功能。


在Windows CE上，默认值为-1（即禁用）。 在Windows Mobile上，默认值是40。


这个属性是在Qt 4.4中引入的。


访问函数；

```cpp
bool autoSipEnabled() const
void setAutoSipEnabled(const bool enabled)
```


```cpp
int autoMaximizeThreshold() const
void setAutoMaximizeThreshold(const int threshold)
```




2、autoSipEnabled : bool



该属性保持切换自动SIP（软件输入面板）可见性。

将此属性设置为true以在输入接受键盘输入的小部件时自动显示SIP。 此属性仅影响设置了WA_InputMethodEnabled属性的小部件，并且通常用于在具有非常少或没有密钥的设备上启动虚拟键盘。

该属性仅对使用软件输入面板（如Windows CE）的平台有影响。

默认值取决于平台。

这个属性是在Qt 4.5中引入的。

访问函数；

```cpp
bool autoSipEnabled() const
void setAutoSipEnabled(const bool enabled)
```




3、cursorFlashTime : int



该属性保存文本光标的闪烁（闪烁）时间，以毫秒为单位。

闪光时间是显示，反转和恢复插入符号显示所需的时间。 通常情况下，文本光标显示为光标闪烁时间的一半，然后隐藏相同的时间，但这可能会有所不同。

X11的默认值是1000毫秒。 在Windows上，使用控制面板值并设置此属性为所有应用程序设置光标闪烁时间。

我们建议小部件不要缓存该值，因为如果用户更改全局桌面设置，该值可能随时发生变化。

注意：例如，如果光标闪烁被禁用，该属性可能会保持负值。

访问函数；

```cpp
int cursorFlashTime()
void setCursorFlashTime(int)
```




4、doubleClickInterval : int

此属性保留以毫秒为单位的时间限制，以区分双击和两次连续鼠标单击。

X11的默认值是400毫秒。 在Windows和Mac OS上，使用操作系统的值。


访问函数；

```cpp
int doubleClickInterval()
void setDoubleClickInterval(int)
```



5、globalStrut : QSize

该属性拥有用户可以与之交互的任何GUI元素应具有的最小大小。

例如，不应将按钮的大小调整为小于全局支柱大小。 重新实现可能用于触摸屏或类似I / O设备的GUI控件时，应考虑支柱尺寸。


例如；

```cpp
QSize MyWidget::sizeHint() const
  {
      return QSize(80, 25).expandedTo(QApplication::globalStrut());
  }
```

默认情况下，此属性包含宽度和高度均为零的QSize对象。

访问函数；

```cpp
QSize globalStrut()
void setGlobalStrut(const QSize &)
```




6、keyboardInputInterval : int



此属性保持以毫秒为单位的时间限制，以区分一次按键和两次连续按键。

X11的默认值是400毫秒。 在Windows和Mac OS上，使用操作系统的值。

该属性在Qt 4.2中引入。

访问函数；

```cpp
int keyboardInputInterval()
void setKeyboardInputInterval(int)
```




7、startDragDistance : int

如果您支持拖放应用程序，并且想要在用户按住某个按钮将光标移动一定距离后开始拖放操作，则应该使用此属性的值作为所需的最小距离。

例如，如果点击的鼠标位置存储在startPos中，并且当前位置（例如，在鼠标移动事件中）是currentPos，则可以找出是否应使用如下代码开始拖动：


```cpp
int main(int argc, char *argv[])
  {
      QApplication::setDesktopSettingsAware(false);
      QApplication app(argc, argv);
      ...
      return app.exec();
  }
```

Qt在内部使用这个值，例如 在QFileDialog中。

默认值（如果平台不提供不同的默认值）是10个像素。

访问函数；

```cpp
int startDragDistance()
void setStartDragDistance(int l)
```




8、startDragTime : int



此属性保留以毫秒为单位的时间，即在开始拖放操作之前必须按住鼠标按钮。

如果您支持在应用程序中拖放，并且想要在用户按住鼠标按钮一定时间后开始拖放操作，则应该使用此属性的值作为延迟。

Qt也在内部使用这个延迟，例如 在QTextEdit和QLineEdit中，用于开始拖动。

默认值是500毫秒。

访问函数；

```cpp
int startDragTime()
void setStartDragTime(int ms)
```




9、styleSheet : QString



该属性包含应用程序样式表。

默认情况下，除非用户在运行应用程序时在命令行上指定-stylesheet选项，否则此属性将返回空字符串。

该属性在Qt 4.2中引入。

访问函数；

```cpp
QString styleSheet() const
void setStyleSheet(const QString &sheet)
```




10、wheelScrollLines : int



此属性包含滚动鼠标滚轮时滚动窗口小部件的行数。

如果该值超过小部件的可见行数，则小部件应该将滚动操作解释为单页向上或向下翻页。 如果小部件是一个项目视图类，那么滚动一行的结果取决于小部件滚动模式的设置。 滚动一行可以表示滚动一个项目或滚动一个像素。

默认情况下，此属性的值为3。

访问函数；

```cpp
int wheelScrollLines()
void setWheelScrollLines(int)
```



11、windowIcon : QIcon

该属性保存默认的窗口图标。


访问函数；

```cpp
QIcon windowIcon()
void setWindowIcon(const QIcon &icon)
```







三、成员函数

1、QApplication::QApplication(int &argc, char **argv)



初始化窗口系统并使用argv中的argc命令行参数构造一个应用程序对象。

警告：由argc和argv引用的数据必须在QApplication对象的整个生命周期内保持有效。 另外，argc必须大于零，并且argv必须包含至少一个有效的字符串。

全局qApp指针指向这个应用程序对象。 应该只创建一个应用程序对象。

此应用程序对象必须在任何绘画设备（包括小部件，像素图，位图等）之前构造。

注意：argc和argv可能会因Qt删除它识别的命令行参数而发生更改。

所有Qt程序都自动支持以下命令行选项：


1）、-style = style，设置应用程序的GUI风格。 可能的值取决于您的系统配置。 如果您使用其他样式编译Qt或将其他样式作为插件进行编译，这些将可用于-style命令行选项。 您还可以通过设置QT_STYLE_OVERRIDE环境变量来设置所有Qt应用程序的样式。

2）、-style style，与上面列出的一样。

3）、-stylesheet = stylesheet，设置应用程序样式表。 该值必须是包含样式表的文件的路径。

注意：样式表文件中的相对URL与样式表文件的路径有关。


4）、-stylesheet stylesheet, 与上面列出的一样。

5）、-widgetcount，在最后打印调试消息，其中包含未保存的小部件数量以及同时存在的最大小部件数量

6）、-reverse，将应用程序的布局方向设置为Qt :: RightToLeft

7）、-qmljsdebugger =，用指定的端口激活QML / JS调试器。 该值必须是格式port：1234 [，block]，其中block是可选的，并会使应用程序等待，直到调试器连接到该端口。




2、[virtual] QApplication::~QApplication()

清理此应用程序分配的任何窗口系统资源。 将全局变量qApp设置为0。





3、[static slot] void QApplication::aboutQt()



显示一个关于Qt的简单消息框。 该消息包含应用程序正在使用的Qt的版本号。

这对于包含在应用程序的“帮助”菜单中非常有用，如菜单示例中所示。

此函数是QMessageBox :: aboutQt（）的便捷插槽。




4、[static] QWidget *QApplication::activeModalWidget()



返回活动的模式小部件。

模式小部件是一个特殊的顶层小部件，它是QDialog的子类，它将构造函数的模态参数指定为true。 在用户可以继续使用该程序的其他部分之前，必须先关闭模式窗口小部件。

模态小部件被组织在一个堆栈中。 该函数返回堆栈顶部的活动模态小部件。




5、[static] QWidget *QApplication::activePopupWidget()



返回活动的弹出窗口部件。

弹出窗口小部件是设置Qt :: WType_Popup窗口小部件标志的特殊顶级窗口小部件，例如， QMenu小部件。 当应用程序打开一个弹出窗口小部件时，所有事件都被发送到弹出窗口。 在关闭弹出窗口小部件之前，不能访问普通窗口小部件和模式窗口小部件。

显示弹出窗口小部件时，只有其他弹出窗口小部件可能会打开。 弹出窗口小部件被组织在一个堆栈中。 该函数返回堆栈顶部的活动弹出窗口小部件。




6、[static] QWidget *QApplication::activeWindow()

返回具有键盘输入焦点的应用程序顶层窗口，如果没有应用程序窗口具有焦点，则返回0。 即使没有focusWidget（），也可能有activeWindow（），例如，如果该窗口中没有小部件接受关键事件。





7、[static] void QApplication::alert(QWidget *widget, int msec = 0)



如果窗口不是活动窗口，则会为窗口小部件显示警报。 该警报显示为毫秒毫秒。 如果msec为零（默认值），则警报将无限期显示，直到窗口再次变为活动状态。

目前这个功能在Qt for Embedded Linux上不做任何事情。
在OS X上，这在应用程序级更适用，并会导致应用程序图标在底座中反弹。

在Windows上，这会导致窗口的任务栏条目闪烁一段时间。 如果毫秒为零，闪烁将停止，任务栏条目将变成不同的颜色（当前为橙色）。

在X11上，这会导致窗口被标记为“需要关注”，窗口不能被隐藏（即没有对其调用hide（），而是以某种方式显示），以使其工作。




8、[static] QWidgetList QApplication::allWidgets()



返回应用程序中所有小部件的列表。

如果没有小部件，列表是空的（QList :: isEmpty（））。

注意：一些小部件可能被隐藏。

例；

```cpp
void updateAllWidgets()
  {
      foreach (QWidget *widget, QApplication::allWidgets())
          widget->update();
  }
```




9、[static] void QApplication::beep()

使用默认的音量和声音听起来很响。 该功能在Qt for Embedded Linux中不可用。





10、[static slot] void QApplication::closeAllWindows()

关闭所有顶级窗口。

此功能对于具有许多顶级窗口的应用程序特别有用。 例如，它可以连接到文件菜单中的退出条目：


```cpp
const QIcon exitIcon = QIcon::fromTheme("application-exit");
      QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), qApp, &QApplication::closeAllWindows);
      exitAct->setShortcuts(QKeySequence::Quit);
      exitAct->setStatusTip(tr("Exit the application"));
      fileMenu->addAction(exitAct);
```

窗口以随机顺序关闭，直到一个窗口不接受关闭事件。 应用程序在最后一个窗口成功关闭时退出; 这可以通过将quitOnLastWindowClosed设置为false来关闭。




11、[static] int QApplication::colorSpec()

返回颜色规格。





12、[static] QDesktopWidget *QApplication::desktop()

返回桌面小部件（也称为根窗口）。

桌面可能由多个屏幕组成，所以它会不正确，例如，尝试将某个小部件居中放置在桌面的几何图形中。 QDesktopWidget具有各种功能，用于在桌面上获取有用的几何图形，例如QDesktopWidget :: screenGeometry（）和QDesktopWidget :: availableGeometry（）。





13、[protected] bool QApplication::event(QEvent *e)




14、[static] int QApplication::exec()



进入主事件循环并等待，直到调用exit（），然后返回设置为exit（）的值（如果通过quit（）调用exit（），则返回0。

有必要调用这个函数来启动事件处理。主事件循环从窗口系统接收事件并将它们分派给应用程序小部件。

通常，在调用exec（）之前，用户交互不会发生。作为一种特殊情况，可以在调用exec（）之前使用类似QMessageBox的模式小部件，因为模式小部件调用exec（）来启动本地事件循环。

为了让您的应用程序执行空闲处理，即在没有未决事件时执行特殊功能，请使用具有0超时的QTimer。使用processEvents（）可以实现更高级的空闲处理方案。

我们建议您将清理代码连接到aboutToQuit（）信号，而不是将其放入应用程序的main（）函数中。这是因为，在某些平台上，QApplication :: exec（）调用可能不会返回。例如，在Windows平台上，当用户注销时，系统在Qt关闭所有顶级窗口后终止进程。因此，不能保证应用程序将有时间在QApplication :: exec（）调用之后退出其事件循环并在main（）函数结束时执行代码。




15、[signal] void QApplication::focusChanged(QWidget *old, QWidget *now)



当具有键盘焦点的小部件从旧改变为现在时，即由于用户按下Tab键，点击进入小部件或改变活动窗口时，发出该信号。 旧的和现在都可以是空指针。

两个小部件都通过QFocusEvent通知了更改后，发出信号。

这个函数是在Qt 4.1中引入的。




16、[static] QWidget *QApplication::focusWidget()

返回具有键盘输入焦点的应用程序小部件，如果此应用程序中没有小部件具有焦点，则返回0。





17、[static] QFont QApplication::font()

返回默认的应用程序字体。





18、[static] QFont QApplication::font(const QWidget *widget)

这是一个重载函数。

返回小部件的默认字体。





19、[static] QFont QApplication::font(const char *className)

这是一个重载函数。

返回给定className的小部件的字体。





20、[static] QFontMetrics QApplication::fontMetrics()

返回应用程序字体的显示（屏幕）字体指标。





21、[static] bool QApplication::isEffectEnabled(Qt::UIEffect effect)



如果启用了效果，则返回true; 否则返回false。

默认情况下，Qt会尝试使用桌面设置。 为了防止这种情况，请调用setDesktopSettingsAware（false）。

注意：在低于16位颜色深度的屏幕上，所有效果均被禁用。




22、[static] Qt::NavigationMode QApplication::navigationMode()



返回Qt正在使用哪种焦点导航。

此函数仅适用于嵌入式Linux Qt和Windows CE。

注意：在Windows CE上，此功能在默认情况下对于触摸设备mkspecs处于禁用状态。 要启用键盘导航，请使用QT_KEYPAD_NAVIGATION定义构建Qt。

这个函数是在Qt 4.6中引入的。




23、bool QApplication::notify(QObject *receiver, QEvent *e)




24、[static] QPalette QApplication::palette(const QWidget *widget)

这是一个重载函数。

如果传递了一个小部件，则返回该小部件的类的默认调色板。 这可能是也可能不是应用程序调色板。 在大多数情况下，某些类型的窗口小部件没有特殊的调色板，但是一个明显的例外是Windows下的弹出式菜单，如果用户在显示设置中为菜单定义了特殊的背景颜色。





25、[static] QPalette QApplication::palette(const char *className)

这是一个重载函数。

返回给定className的小部件的调色板。





26、[static] void QApplication::setActiveWindow(QWidget *active)



响应系统事件，将活动窗口设置为活动窗口小部件。 该函数是从特定于平台的事件处理程序中调用的。

警告：此功能不会将键盘焦点设置为活动小部件。 改为调用QWidget :: activateWindow（）。

它设置activeWindow（）和focusWidget（）属性，并将适当的WindowActivate / WindowDeactivate和FocusIn / FocusOut事件发送到所有适当的小部件。 窗口将被激活状态（例如，线条编辑中的光标将闪烁），并且它将启用工具提示。




27、[static] void QApplication::setColorSpec(int spec)



将应用程序的颜色规范设置为规范。

颜色规范控制应用程序在具有有限颜色的显示器上运行时如何分配颜色，例如， 8位/ 256色显示器。

颜色规格必须在创建QApplication对象之前设置。

选项是：

1、QApplication:: NORMALCOLOR。 这是默认的颜色分配策略。 如果您的应用程序使用少量颜色的按钮，菜单，文本和像素图，请使用此选项。 使用此选项，应用程序使用系统全局颜色。 这适用于X11下的大多数应用程序，但在Windows平台上，它可能会导致非标准颜色的抖动。

2、QApplication:: CustomColor。 如果您的应用程序需要少量自定义颜色，请使用此选项。 在X11上，此选项与NormalColor相同。 在Windows上，Qt创建一个Windows调色板，并根据需要为其分配颜色。

3、QApplication:: ManyColor。 如果您的应用程序颜色很饿，例如需要数千种颜色，请使用此选项。在X11下的效果是：

1）、对于最多具有256色真彩色视觉的256色显示器，使用默认视觉效果，并从彩色立方体分配颜色。 颜色立方体是6x6x6（216颜色）的“Web调色板”（红色，绿色和蓝色组件总是具有以下值之一：0x00,0x33,0x66,0x99,0xCC或0xFF），但颜色数量 可以通过-ncols选项进行更改。 用户可以强制应用程序使用-visual选项使用真彩色视觉效果。

2）、对于具有超过256种颜色的真彩视觉的256色显示器，请使用该视觉效果。 例如，Silicon Graphics X服务器提供此功能。 它们默认提供8位视觉效果，但当被问到时可以提供真实的颜色。

在Windows上，Qt创建一个Windows调色板，并用一个彩色立方体填充它。


请注意，CustomColor和ManyColor选项可能导致颜色映射闪烁：前景应用程序获取（大部分）可用颜色，而背景窗口看起来不那么吸引人。


例；

```cpp
int main(int argc, char *argv[])
  {
      QApplication::setColorSpec(QApplication::ManyColor);
      QApplication app(argc, argv);
      ...
      return app.exec();
  }
```



28、[static] void QApplication::setEffectEnabled(Qt::UIEffect effect, bool enable = true)

如果启用为true，则启用UI效果效果，否则不会使用该效果。

注意：在低于16位颜色深度的屏幕上，所有效果均被禁用。





29、[static] void QApplication::setFont(const QFont &font, const char *className = Q_NULLPTR)



将默认应用程序字体更改为字体。 如果传递了className，则该更改仅适用于继承className的类（由QObject :: inherits（）报告）。

在应用程序启动时，默认字体取决于窗口系统。 它可以根据窗口系统版本和区域设置而变化。 该功能可以让您覆盖默认字体; 但重写可能是一个坏主意，因为，例如，某些语言环境需要额外的大字体来支持其特殊字符。

警告：请勿将此功能与Qt样式表一起使用。 应用程序的字体可以使用“字体”样式表属性进行自定义。 要为所有QPushButtons设置粗体字体，请将应用程序styleSheet（）设置为“QPushButton {font：bold}”




30、[static] void QApplication::setNavigationMode(Qt::NavigationMode mode)



设置Qt应该用于模式的焦点导航类型。

此功能仅适用于嵌入式Linux Qt和Windows CE。

注意：在Windows CE上，此功能在默认情况下对于触摸设备mkspecs处于禁用状态。 要启用键盘导航，请使用QT_KEYPAD_NAVIGATION定义构建Qt。

这个函数是在Qt 4.6中引入的。




31、[static] void QApplication::setPalette(const QPalette &palette, const char *className = Q_NULLPTR)



将默认应用程序调色板更改为调色板。

如果传递了className，则更改仅适用于继承className的小部件（由QObject :: inherits（）报告）。 如果className为0，则该更改会影响所有小部件，从而覆盖以前设置的任何类特定的调色板。
可以根据QStyle :: polish（）中的当前GUI样式更改调色板。

警告：请勿将此功能与Qt样式表一起使用。 使用样式表时，可以使用“颜色”，“背景颜色”，“选择颜色”，“选择背景颜色”和“备用背景颜色”来自定义控件的调色板。

注意：某些样式不会为所有绘图使用调色板，例如，如果他们使用本机主题引擎。 这是Windows XP，Windows Vista和OS X样式的情况。




32、[static] void QApplication::setStyle(QStyle *style)

将应用程序的GUI样式设置为样式。 样式对象的所有权转移到QApplication，所以QApplication将删除应用程序退出时的样式对象，或者当设置新样式并且旧样式仍然是应用程序对象的父类时。


例；

```cpp
QApplication::setStyle(QStyleFactory::create("Fusion"));
```

切换应用程序样式时，调色板将设置回初始颜色或系统默认值。 这是必要的，因为某些样式必须调整调色板以完全符合样式指南。

在调色板设置之前设置样式，即在创建QApplication之前，将导致应用程序使用QStyle :: standardPalette（）作为调色板。

警告：自定义QStyle子类目前不支持Qt样式表。 我们计划在未来的某个版本中解决这个问题。




33、[static] QStyle *QApplication::setStyle(const QString &style)



这是一个重载函数。

从QStyleFactory中为样式请求一个QStyle对象。

该字符串必须是QStyleFactory :: keys（）之一，通常是“windows”，“fusion”，“windowsxp”或“macintosh”之一。 样式名称不区分大小写。

如果传递未知样式，则返回0，否则将返回的QStyle对象设置为应用程序的GUI样式。

警告：为确保应用程序的样式设置正确，最好在QApplication构造函数之前调用此函数（如果可能）。




34、[static] QStyle *QApplication::style()

返回应用程序的样式对象。





35、[static] QWidget *QApplication::topLevelAt(const QPoint &point)

这是一个重载函数。

返回点（x，y）处的顶级小部件; 如果没有这样的小部件，则返回0。





36、[static] QWidgetList QApplication::topLevelWidgets()

返回应用程序中顶级窗口小部件（窗口）的列表。

注意：某些顶级窗口小部件可能会隐藏，例如，如果当前没有显示任何工具提示，则为工具提示。


例；

```cpp
void showAllHiddenTopLevelWidgets()
  {
      foreach (QWidget *widget, QApplication::topLevelWidgets()) {
          if (widget->isHidden())
              widget->show();
      }
  }
```




37、[static] QWidget *QApplication::widgetAt(const QPoint &point)

在全局屏幕位置点返回小部件，如果没有Qt小部件，则返回0。

这个函数可能很慢。





38、[static] QWidget *QApplication::widgetAt(int x, int y)

这是一个重载函数。

在全局屏幕位置（x，y）返回小部件，如果没有Qt小部件，则返回0。







四、宏

1、qApp

引用唯一应用程序对象的全局指针。 它等同于QCoreApplication :: instance（）函数返回的指针，但在GUI应用程序中，它是指向QApplication实例的指针。

只能创建一个应用程序对象。




