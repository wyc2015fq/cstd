# Qt开发：Qt Widgets模块——QAction类 - 一世豁然的专栏 - CSDN博客





2018年05月18日 07:52:42[一世豁然](https://me.csdn.net/Explorer_day)阅读数：3417








QAction类提供了可以插入到窗口小部件中的抽象用户界面操作。



头文件：#include <QAction> 

qmake：QT += widgets

继承：QObject

被继承：QWidgetAction

在应用程序中，可以通过菜单，工具栏按钮和键盘快捷键调用许多常用命令。 由于用户期望每个命令都以相同的方式执行，无论使用的用户界面如何，将每个命令表示为一个操作都很有用。


操作可以添加到菜单和工具栏，并会自动保持同步。 例如，在文字处理器中，如果用户按下粗体工具栏按钮，则会自动检查粗体菜单项。


操作可以创建为独立的对象，但也可以在菜单构建过程中创建; QMenu类包含便利功能，用于创建适合用作菜单项的动作。


QAction可能包含图标，菜单文本，快捷方式，状态文本“这是什么？” 文字和工具提示。 其中大部分可以在构造函数中设置。 它们也可以用setIcon（），setText（），setIconText（），setShortcut（），setStatusTip（），setWhatsThis（）和setToolTip（）独立设置。 对于菜单项，可以使用setFont（）设置单个字体。


使用QWidget :: addAction（）或QGraphicsWidget :: addAction（）将操作添加到窗口小部件。 请注意，必须先将某个操作添加到小部件才能使用; 当快捷方式应该是全局的时候（即，Qt :: ApplicationShortcut作为Qt :: ShortcutContext），也是如此。


一旦QAction被创建，它应该被添加到相关的菜单和工具栏中，然后连接到将执行操作的插槽。 例如：


```cpp
const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
openAct->setShortcuts(QKeySequence::Open);
openAct->setStatusTip(tr("Open an existing file"));
connect(openAct, &QAction::triggered, this, &MainWindow::open);
fileMenu->addAction(openAct);
fileToolBar->addAction(openAct);
```

```cpp
fileMenu->addAction(openAct);
```

我们建议将操作创建为其所用窗口的子项。在大多数情况下，操作将是应用程序主窗口的子项。







一、成员类型

1、enum QAction::ActionEvent

这个枚举类型在调用QAction :: activate（）时使用。



|内容|值|描述|
|----|----|----|
|QAction::Trigger|0|这会导致发出QAction :: triggered（）信号。|
|QAction::Hover|1|这会导致QAction :: hovered（）信号被发射|




2、enum QAction::MenuRole

此枚举描述了如何将操作移动到OS X上的应用程序菜单中。


|内容|值|描述|
|----|----|----|
|QAction::NoRole|0|此操作不应放入应用程序菜单中|
|QAction::TextHeuristicRole|1|这个动作应该根据QMenuBar文档中描述的动作文本放在应用程序菜单中。|
|QAction::ApplicationSpecificRole|2|此操作应该放在具有应用程序特定角色的应用程序菜单中|
|QAction::AboutQtRole|3|这个动作处理“关于Qt”菜单项。|
|QAction::AboutRole|4|此操作应放置在“关于”菜单项位于应用程序菜单中的位置。 菜单项的文本将被设置为“关于<应用程序名称>”。 应用程序名称是从应用程序包中的Info.plist文件中提取的（请参阅Qt for OS X - 部署）。|
|QAction::PreferencesRole|5|此操作应放置在应用程序菜单中“Preferences ...”菜单项的位置。|
|QAction::QuitRole|6|此操作应放置在应用程序菜单中退出菜单项目的位置。|
设置此值仅对菜单栏的直接菜单中的项目有效，而不影响菜单的子菜单。 例如，如果菜单栏中有“文件”菜单，而“文件”菜单中有一个子菜单，则为该子菜单中的操作设置MenuRole不起作用。 他们永远不会被移动。



3、enum QAction::Priority

此枚举为用户界面中的操作定义优先级。



|内容|值|描述|
|----|----|----|
|QAction::LowPriority|0|该操作不应该在用户界面中排列优先级。|
|QAction::NormalPriority|128||
|QAction::HighPriority|256|该操作应该在用户界面中排列优先级。|
这个枚举是在Qt 4.6中引入或修改的。






二、属性

1、autoRepeat : bool

该属性可以确定操作是否可以自动重复。


如果设置为true，则在按住键盘快捷键组合的情况下，该操作会自动重复，前提是系统上启用了键盘自动重复功能。 默认值是true。


该属性在Qt 4.2中引入。


访问函数；

```cpp
bool autoRepeat() const
void setAutoRepeat(bool)
```

通知信号；

```cpp
void changed()
```




2、checkable : bool

该属性保持该操作是否为可检查操作。


可检查的动作是具有开/关状态的动作。 例如，在文字处理器中，粗体工具栏按钮可以打开或关闭。 不是切换操作的操作是命令操作; 一个命令动作就简单地执行，例如， 文件保存。 默认情况下，此属性为false。


在某些情况下，一个切换动作的状态应该取决于其他状态。 例如，“左对齐”，“中心”和“右对齐”切换操作是互斥的。 要实现独占切换，请将QActionGroup :: exclusive属性设置为true的相关切换操作添加到QActionGroup。


访问函数；

```cpp
bool isCheckable() const
void setCheckable(bool)
```

通知信号；

```cpp
void changed()
```




3、checked : bool

此属性保持是否检查操作。

只有可检查的动作可以被检查。 默认情况下，这是错误的（该操作未被选中）。


访问函数；

```cpp
bool isChecked() const
void setChecked(bool)
```

通知信号；

```cpp
void toggled(bool checked)
```




4、enabled : bool



该属性保持是否启用该操作。

禁用的操作不能由用户选择。 它们不会从菜单或工具栏中消失，但会以指示它们不可用的方式显示。 例如，它们可能仅使用灰色阴影显示。

这是什么？ 只要QAction :: whatsThis属性已设置，仍然可以使用禁用的动作帮助。

当添加的所有小部件（使用QWidget :: addAction（））被禁用或不可见时，操作将被禁用。 当某个操作被禁用时，无法通过其快捷方式触发它。

默认情况下，此属性为true（启用操作）。

访问函数；

```cpp
bool isEnabled() const
void setEnabled(bool)
```

通知信号；

```cpp
void changed()
```


5、font : QFont



该属性保存动作的字体。

font属性用于呈现QAction上的文本集。 字体将被视为一个提示，因为在所有情况下都不会根据应用和样式查阅。
默认情况下，此属性包含应用程序的默认字体。

访问函数；

```cpp
QFont font() const
void setFont(const QFont &font)
```

通知信号；

```cpp
void changed()
```




6、icon : QIcon



该属性包含操作的图标。

在工具栏中，图标用作工具按钮图标; 在菜单中，它显示在菜单文本的左侧。 没有默认图标。

如果将一个空图标（QIcon :: isNull（）传入此函数，则会清除该动作的图标。

访问函数；

```cpp
QIcon icon() const
void setIcon(const QIcon &icon)
```

通知信号；

```cpp
void changed()
```




7、iconText : QString



该属性包含操作的描述性图标文本。

如果QToolBar :: toolButtonStyle设置为允许显示文本的值，则保留在此属性中的文本在相关工具按钮中显示为标签。

如果没有使用setText（）或setToolTip（）定义动作，它也用作菜单和工具提示中的默认文本，如果没有使用setIcon（）定义图标，也将用于工具栏按钮中。

如果图标文本未明确设置，则该操作的普通文本将用于图标文本。

默认情况下，该属性包含一个空字符串。

访问函数；

```cpp
QString iconText() const
void setIconText(const QString &text)
```

通知信号；

```cpp
void changed()
```




8、iconVisibleInMenu : bool



这个属性保持一个动作是否应该在菜单中显示一个图标。

在某些应用程序中，使用工具栏中的图标进行操作可能比较合理，但在菜单中不可以。 如果为true，则图标（如果有效）显示在菜单中，如果为false，则不显示。

默认是跟随是否为应用程序设置Qt :: AA_DontShowIconsInMenus属性。 显式设置此属性将覆盖该属性的存在（或缺失）。

例如：

```cpp
QApplication app(argc, argv);
  app.setAttribute(Qt::AA_DontShowIconsInMenus);  // Icons are *no longer shown* in menus
  // ...
  QAction *myAction = new QAction();
  // ...
  myAction->setIcon(SomeIcon);
  myAction->setIconVisibleInMenu(true);   // Icon *will* be shown in menus for *this* action.
```

这个属性是在Qt 4.4中引入的。

访问函数；

```cpp
bool isIconVisibleInMenu() const
void setIconVisibleInMenu(bool visible)
```

通知信号；

```cpp
void changed()
```




9、menuRole : MenuRole



该属性包含操作的菜单角色。

这表明该操作在Mac OS X上的应用程序菜单中扮演的角色。默认情况下，所有操作都具有TextHeuristicRole，这意味着该操作是基于其文本添加的（有关更多信息，请参阅QMenuBar）。

菜单角色只能在操作放入OS X的菜单栏之前更改（通常在显示第一个应用程序窗口之前）。

该属性在Qt 4.2中引入。

访问函数；

```cpp
MenuRole menuRole() const
void setMenuRole(MenuRole menuRole)
```

通知信号；

```cpp
void changed()
```




10、priority : Priority



该属性在用户界面中保存操作的优先级。

该属性可以设置为指示如何在用户界面中优先处理操作。

例如，当工具栏设置了Qt :: ToolButtonTextBesideIcon模式时，具有LowPriority的操作不会显示文本标签。

这个属性是在Qt 4.6中引入的。

访问函数；

```cpp
Priority priority() const
void setPriority(Priority priority)
```




11、shortcut : QKeySequence

该属性包含操作的主要快捷键。

在Qt :: Key和Qt :: Modifier中可以找到该属性的有效键码。 没有默认的快捷键。


访问函数；

```cpp
QKeySequence shortcut() const
void setShortcut(const QKeySequence &shortcut)
```

通知信号；

```cpp
void changed()
```




12、shortcutContext : Qt::ShortcutContext

该属性保存动作快捷方式的上下文。

这个属性的有效值可以在Qt :: ShortcutContext中找到。 默认值是Qt :: WindowShortcut。


访问函数；

```cpp
Qt::ShortcutContext shortcutContext() const
void setShortcutContext(Qt::ShortcutContext context)
```

通知信号；

```cpp
void changed()
```




13、statusTip : QString



该属性包含该操作的状态提示。

状态提示显示在动作的顶层父窗口小部件提供的所有状态栏中。

默认情况下，该属性包含一个空字符串。

访问函数；

```cpp
QString statusTip() const
void setStatusTip(const QString &statusTip)
```

通知信号；

```cpp
void changed()
```




14、text : QString

该属性包含操作的描述性文本。

如果操作添加到菜单中，则菜单选项将由图标（如果有），文本和快捷方式（如果有的话）组成。 如果文本未在构造函数中显式设置，或者使用setText（），则操作的描述图标文本将用作文本。 没有默认文字。


访问函数；

```cpp
QString text() const
void setText(const QString &text)
```

通知信号；

```cpp
void changed()
```




15、toolTip : QString



该属性包含操作的提示。

该文本用于工具提示。 如果未指定工具提示，则使用操作的文本。

默认情况下，此属性包含操作的文本。

访问函数；

```cpp
QString toolTip() const
void setToolTip(const QString &tip)
```

通知信号；

```cpp
void changed()
```




16、visible : bool



此属性保持是否可以看到该操作（例如，在菜单和工具栏中）。

如果可见，则可以看到该动作（例如，在菜单和工具栏中）并由用户选择; 如果可见是错误的，则该行为不能被用户看到或选择。

不可见的动作不会变灰; 他们根本不出现。

默认情况下，此属性为true（操作可见）。

访问函数；

```cpp
bool isVisible() const
void setVisible(bool)
```

通知信号；

```cpp
void changed()
```




17、whatsThis : QString

该属性包含操作的“这是什么？” 帮助文本。

“这是什么？” 文本用于提供操作的简要描述。 该文本可能包含丰富的文本。 没有默认的“这是什么？” 文本。


访问函数；

```cpp
QString whatsThis() const
void setWhatsThis(const QString &what)
```

通知信号；

```cpp
void changed()
```







三、成员函数

1、QAction :: QAction（QObject * parent = nullptr）

用父级构造一个动作。 如果父母是一个行动组，行动将被自动插入组中。





2、QAction::QAction(const QString &text, QObject *parent = nullptr)

用一些文本和父项构造一个动作。 如果父母是一个行动组，行动将被自动插入组中。

该动作使用剥离版本的文本（例如“＆Menu Option ...”变为“菜单选项”）作为工具按钮的描述性文本。 您可以通过使用setText（）设置特定的描述来覆盖此内容。 除非使用setToolTip（）指定不同的文本，否则相同的文本将用于工具提示。





3、QAction::QAction(const QIcon &icon, const QString &text, QObject *parent = nullptr)

用一个图标和一些文本和父项构造一个动作。 如果父母是一个行动组，行动将被自动插入组中。

该动作使用剥离版本的文本（例如“＆Menu Option ...”变为“菜单选项”）作为工具按钮的描述性文本。 您可以通过使用setText（）设置特定的描述来覆盖此内容。 除非使用setToolTip（）指定不同的文本，否则相同的文本将用于工具提示。





4、QAction::~QAction()

销毁对象并释放分配的资源。





5、QActionGroup *QAction::actionGroup() const

返回此操作的操作组。 如果没有动作组管理这个动作，则返回0。





6、void QAction::activate(ActionEvent event)

发送ActionEvent事件的相关信号。

基于操作的小部件使用此API来使QAction发出信号并发出它们自己的信号。





7、QList<QGraphicsWidget *> QAction::associatedGraphicsWidgets() const

返回此操作已添加到的小部件列表。

这个函数是在Qt 4.5中引入的。





8、QList<QWidget *> QAction::associatedWidgets() const

返回此操作已添加到的小部件列表。

这个函数是在Qt 4.2中引入的。




9、[signal] void QAction::changed()

动作发生变化时会发出此信号。 如果您只对给定小部件中的操作感兴趣，则可以观察使用QEvent :: ActionChanged发送的QWidget :: actionEvent（）。

注意：属性可见的通知程序信号。 通知信号用于属性检查。 启用属性的通知程序信号。 属性图标的通知器信号。 属性文本的通知程序信号。 通知者信号属性iconText。 属性工具提示的通知器信号。 通知程序信号用于属性状态提示。 通知者信号属性whatsThis。 属性字体的通知器信号。 属性快捷方式的通知器信号。 属性shortcutContext的通知程序信号。 属性autoRepeat的通知程序信号。 属性menuRole的通知器信号。 属性iconVisibleInMenu的通知程序信号。





10、QVariant QAction::data() const

返回在QAction :: setData中设置的用户数据。





11、[virtual protected] bool QAction::event(QEvent *e)

从QObject :: event（）重新实现。





12、[slot] void QAction::hover()

这是一个调用激活（悬停）的便利插槽。





13、[signal] void QAction::hovered()

当用户突出显示动作时发出此信号; 例如，当用户将光标悬停在菜单选项，工具栏按钮上或按动作的快捷键组合时。





14、bool QAction::isSeparator() const

如果此操作是分隔符操作，则返回true; 否则返回false。





15、QMenu *QAction::menu() const

返回此操作包含的菜单。 包含菜单的动作可用于创建具有子菜单的菜单项，或者插入到工具栏中以使用弹出菜单创建按钮。





16、QWidget *QAction::parentWidget() const

返回父窗口小部件。





17、void QAction::setActionGroup(QActionGroup *group)

将此操作组设置为组。 该操作将自动添加到组的操作列表中。

组内的行动将是相互排斥的。





18、void QAction::setData(const QVariant &userData)

将操作的内部数据设置为给定的userData。





19、[slot] void QAction::setDisabled(bool b)

这对启用属性来说是一个方便的功能，对于信号 - 插槽连接非常有用。 如果b为真，则操作被禁用; 否则启用。





20、void QAction::setMenu(QMenu *menu)

将此操作包含的菜单设置为指定的菜单。





21、void QAction::setSeparator(bool b)

如果b是真的，那么这个动作将被视为分隔符。

分隔符如何表示取决于它插入的小部件。 在大多数情况下，分隔符操作将忽略文本，子菜单和图标。





22、void QAction::setShortcuts(const QList<QKeySequence> &shortcuts)

将快捷方式设置为触发操作的快捷方式列表。 列表的第一个元素是主要的快捷方式。

这个函数是在Qt 4.2中引入的。





23、void QAction::setShortcuts(QKeySequence::StandardKey key)

根据密钥设置一个平台相关的快捷方式列表。 调用此函数的结果将取决于当前正在运行的平台。 请注意，此操作可分配多个快捷方式。 如果只需要主快捷方式，请改为使用setShortcut。

这个函数是在Qt 4.2中引入的。





24、QKeySequence QAction::shortcut() const

返回主要快捷方式。

注意：属性快捷方式的Getter函数。





25、QList<QKeySequence> QAction::shortcuts() const

返回快捷方式列表，主快捷方式作为列表的第一个元素。

这个函数是在Qt 4.2中引入的。





26、bool QAction::showStatusText(QWidget *widget = Q_NULLPTR)

通过将QStatusTipEvent发送到其父窗口小部件来更新指定窗口小部件的相关状态栏。 如果发送了事件，则返回true; 否则返回false。

如果指定了空小部件，则将该事件发送到该操作的父级。





27、[slot] void QAction::toggle()

这是一个检查属性的便利功能。 连接到它以将选中状态更改为相反状态。





28、[signal] void QAction::toggled(bool checked)



只要可检查操作改变其isChecked（）状态，就会发出此信号。 这可能是用户交互的结果，或者是因为调用了setChecked（）。

如果操作被选中，则检查为true;如果操作未选中，则检查为false。
注意：检查属性的通知程序信号。




29、[slot] void QAction::trigger()

这是一个调用激活（触发器）的便利插槽。





30、[signal] void QAction::triggered(bool checked = false)



该信号在用户激活动作时发出; 例如，当用户单击菜单选项，工具栏按钮或按动作的快捷键组合时，或者在调用trigger（）时

值得注意的是，当setChecked（）或toggle（）被调用时它不会被发射。
如果操作是可检查的，则检查操作是否被选中为true，如果操作未被检查则检查为false。



