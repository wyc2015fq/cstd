# QT动画框架 - u013366022的专栏 - CSDN博客
2016年08月26日 13:59:24[slitaz](https://me.csdn.net/u013366022)阅读数：363
状态机框架提供了创建和执行状态图的一些类．这些概念和表示都是基于Ｈarel状态图中的一些概念和记法．它也是UML状态图表的基础．状态机执行的语义是基于状态图XML(SCXML)．
   状态图提供了一种图形化的方式来对一个系统建模，从而反映它怎么响应外部触发．这是通过定义系统可能进入的一些状态以及系统怎么从一个状态转换到另一个状态（不同状态之间转变）来实现的．事件驱动系统的一个关键的特征（例如Qt应用程序）就是行为通常不仅取决于上次或当前事件，还取决于在它之前的一些事件．用状态图，这个信息非常容易表达．
   状态机框架提供了一套API以及一种执行模型，可有效地将状态图的元素和语义嵌入到Ｑt应用程序当中．该框架与Qt的元对象系统结合紧密：例如，不同状态之间的转变可由信号触发且状态可配置用于设置QObject的属性和方法．Qt的事件系统用于驱动状态机．
   状态机框架中的状态图是分层的．状态可嵌套在另一个状态内．状态机的当前配置包含一些当前活跃的状态．状态机中的一个有效的配置中的所有状态都有一个共同的祖先．
**状态机框架中的类**
qt提供了这些类来创建事件驱动的状态机．
|[**QAbstractState**](http://doc.qt.nokia.com/4.6/qabstractstate.html)|The base class of states of a QStateMachine|
|----|----|
|[**QAbstractTransition**](http://doc.qt.nokia.com/4.6/qabstracttransition.html)|The base class of transitions between QAbstractState objects|
|[**QEventTransition**](http://doc.qt.nokia.com/4.6/qeventtransition.html)|QObject-specific transition for Qt events|
|[**QFinalState**](http://doc.qt.nokia.com/4.6/qfinalstate.html)|Final state|
|[**QHistoryState**](http://doc.qt.nokia.com/4.6/qhistorystate.html)|Means of returning to a previously active substate|
|[**QKeyEventTransition**](http://doc.qt.nokia.com/4.6/qkeyeventtransition.html)|Transition for key events|
|[**QMouseEventTransition**](http://doc.qt.nokia.com/4.6/qmouseeventtransition.html)|Transition for mouse events|
|[**QSignalTransition**](http://doc.qt.nokia.com/4.6/qsignaltransition.html)|Transition based on a Qt signal|
|[**QState**](http://doc.qt.nokia.com/4.6/qstate.html)|General-purpose state for QStateMachine|
|[**QStateMachine**](http://doc.qt.nokia.com/4.6/qstatemachine.html)|Hierarchical finite state machine|
|[**QStateMachine::SignalEvent**](http://doc.qt.nokia.com/4.6/qstatemachine-signalevent.html)|Represents a Qt signal event|
|[**QStateMachine::WrappedEvent**](http://doc.qt.nokia.com/4.6/qstatemachine-wrappedevent.html)|Holds a clone of an event associated with a QObject|
**一个简单的状态机**
为了演示状态机API的核心功能，让我们来看一个小例子：一个状态机有三个状态s1,s2和s3．状态机由一个按钮来控制；当点击按钮时，状态机转换到另一个状态．刚开始时，状态机处于状态s1．该状态机的状态图如下所示：
![](http://hi.csdn.net/attachment/201012/29/0_1293639600K3oI.gif)
下面代码段显示了创建一个这样的状态机所需的代码．首先，我们创建一个状态机和一些状态：
QStateMachine machine;
QState *s1 = new QState();
QState *s2 = new QState();
QState *s3 = new QState();
然后，我们使用[QState::addTransition](http://doc.qt.nokia.com/4.6/qstate.html#addTransition)()函数创建转换：
s1->addTransition(button, SIGNAL(clicked()), s2);
s2->addTransition(button, SIGNAL(clicked()), s3);
  s3->addTransition(button, SIGNAL(clicked()), s1);
接下来，我们将这些状态加入状态机中并设置它的初始状态：
machine.addState(s1);
machine.addState(s2);
machine.addState(s3);
machine.setInitialState(s1);
最后，我们启动状态机：
machine.start();
状态是异步执行的，例如，它成为你的应用程序事件循环的一部分．
**在状态入口和出口做有意义的工作**
上面的状态机仅仅从一个状态转换到另一个状态，并没有执行任何操作．[QState::assignProperty](http://doc.qt.nokia.com/4.6/qstate.html#assignProperty)()函数可用于当进入某个状态时设置某个QObject的一个属性．在下面的代码段中，为每个状态指定了应当赋给QLabel的text属性的值．
s1->assignProperty(label, "text", "In state s1");
s2->assignProperty(label, "text", "In state s2");
s3->assignProperty(label, "text", "In state s3");
当进入了这些状态中的任何一个，标签的值就会相应地改变．　
当进入某个状态时，就会发出[QState::entered](http://doc.qt.nokia.com/4.6/qabstractstate.html#entered)()信号．当离开这个状态时，就会发出[QState::exited](http://doc.qt.nokia.com/4.6/qabstractstate.html#exited)()信号．在下面的代码段中，按钮的showMaximize()槽在进入状态s3时被调用．当退出状态s3时调用showMinimized()：
QObject::connect(s3,
 SIGNAL(entered()), button, SLOT(showMaximized()));
QObject::connect(s3, SIGNAL(exited()), button, SLOT(showMinimized()));
自定义的状态可以重新实现方法[QAbstractState::onEntry](http://doc.qt.nokia.com/4.6/qabstractstate.html#onEntry)()和[QAbstractState::onExit](http://doc.qt.nokia.com/4.6/qabstractstate.html#onExit)().
**完成的状态机**
前面部分定义的状态机从不完成．为了使一个状态机能够完成，它需要拥有一个顶层的最终状态（[QFinalState](http://doc.qt.nokia.com/4.6/qfinalstate.html)对象）．当状态机进入一个顶层最终状态时，该状态机将会释放[QStateMachine::finished](http://doc.qt.nokia.com/4.6/qstate.html#finished)()信号并停止．
在图中引入一个最终状态，所有你需要做的就是创建一个[QFinalState](http://doc.qt.nokia.com/4.6/qfinalstate.html)对象且使用它作为一个或多个转换的目标．
**通过对状态进行分组来共享转换**
假设我们想让用户能够通过点击Quit揿钮在任何时刻能够退出应用程序．为了完成这个目标，我们需要创建一个最终状态并将其作为与Quit按钮的clicked()信号相关联的转换的目标．我们可以从状态s1,s2,s3中添加一个转换；但是，这看起来像是多余的，并且，我们不得不记住从每个将来新加入的状态添加一个这样的转换．
我们可以通过将状态s1,s2,s3分组取得相同的行为（即点击Quit按钮将退出状态机，无论该状态机处于哪个状态）．这是通过创建一个新的顶层状态并使三个原先的状态成为新状态的孩子．如下图显示了新状态机．
![](http://hi.csdn.net/attachment/201012/29/0_1293639698i0Zr.gif)
三个原先的状态已经重命名为s11,s12和s13以反映它们现在已经是新的顶层状态s1的孩子．孩子状态隐含地继承它们的父状态的转换．这意味着现在增加一个从状态s1到最终状态s2的转换已经足够了．新加入s1的状态也将自动继承这个转换．
将状态分组的所有工作就是当创始状态时，指定合适的父状态．你也需要指定哪个子状态是初始状态（例如，哪个子状态将是进入父状态时应该处于的状态）．
QState *s1 = new QState();
QState *s11 = new QState(s1);
QState *s12 = new QState(s1);
QState *s13 = new QState(s1);
s1->setInitialState(s11);
machine.addState(s1);
QFinalState*s2
 = new QFinalState();
s1->addTransition(quitButton, SIGNAL(clicked()), s2);
machine.addState(s2);
QObject::connect(&machine,
 SIGNAL(finished()), QApplication::instance(), SLOT(quit()));
在本例子中，我们想让状态机完成后，应用程序退出，因此状态机的finished()信号连接到应用程序的quit()槽．
一个子状态可以覆盖一个继承过来的转换．例如，如下代码添加了一个转换，它有效地造成了当状态机处于状态s12时，Quit按钮将被忽略．
s12->addTransition(quitButton, SIGNAL(clicked()), s12);
一个转换可以将任何状态作为它的目标，例如，目标状态不一定要与源状态处于相同的层次．　
**使用历史状态来保存和恢复当前状态**
假设我们要增加一个“中断”机制到前面提到的例子当中；用户应该能够点击一个按钮使状态机执行一些不相关的任务，任务完成后状态机应该能够恢复到之前执行的任何任务。（例如，返回到旧状态，在此例子中s11,s12,s13中的一个）。
这样的行为很容易地使用历史状态建模。一个历史状态（QHistoryState对象）是一个伪状态，它代表父状态最后退出时所处的孩子状态。
一个历史状态创建为某个状态的孩子，用于为其记录当前的孩子状态；当状态机在运行时检测到有这样的一个状态存在时，它在父状态退出时自动地记录当前的孩子状态。到该历史状态的一个转变实际上是到状态机之前保存的子状态的转变。状态机自动地“转发”到真正孩子状态的转变。
下图显示了加入了中断机制后的状态机。
![](http://hi.csdn.net/attachment/201012/29/0_1293640004CLes.gif)
下面的代码显示了怎么去实现这种机制；在本例中，我们在进入s3时简单地显示一个信息框，然后通过历史状态立即返回到s1之前的孩子状态中。
QHistoryState *s1h = new QHistoryState(s1);
QState *s3 = new QState();
s3->assignProperty(label, "text", "In s3");
QMessageBox *mbox = new QMessageBox(mainWindow);
mbox->addButton(QMessageBox::Ok);
mbox->setText("Interrupted!");
mbox->setIcon(QMessageBox::Information);
QObject::connect(s3, SIGNAL(entered()), mbox, SLOT(exec()));
s3->addTransition(s1h);
machine.addState(s3);
s1->addTransition(interruptButton, SIGNAL(clicked()), s3);
**使用并行状态以避免状态的组合爆发**
假设你想要在一个状态机中建立一些相互排斥的属性。比如说，我们感兴趣的属性是Clean VS Dirty和Moving VS Not moving。需要采用四个互斥的状态和八个转变才能描述该状态机，并能在各个可能的组合中自由的移动。
![](http://hi.csdn.net/attachment/201012/29/0_1293640106hC3H.gif)
如果我们增加第三个属性（比如，Red VS Blue），状态的总数将会翻倍，到8个，且如果我们添加第四个属性（比如，Enclosed VS Convertible），状态的总数将再次翻倍到16个。
使用并行状态，状态的总数和转变数会随着属性的不断增加线性地增长，而不是指数地增长。而且，从并行状态中添加或移除状态不会影响它们的兄弟状态。
![](http://hi.csdn.net/attachment/201012/29/0_1293640289DMCM.gif)
为了创建一个并行状态组，传递[QState::ParallelStates](http://doc.qt.nokia.com/4.6/qstate.html#ChildMode-enum)到Qstate构造函数中。
QState *s1 = new QState(QState::ParallelStates);
*// s11 and s12 will be entered in parallel*
QState *s11 = new QState(s1);
QState *s12 = new QState(s1);
当一个并行状态组进入时，所有的子状态将会同时进入。每个子状态里的转变正常执行。但是，任何一个子状态可以执行存在于父状态中的一个转变。当这发生时，父状态以及所有的子状态将退出。
状态机框架的并行机制遵循如下一种交错的语义。所有并行操作将以单步，原子地进行，没有事件可以中断并行操作。但是，事件仍然会被顺序地处理，因为状态机本身是单线程的。举个例子：考虑这样的一个情形，有两个转变从相同的状态组中退出，并且它们的（退出）条件同时变为真。在这种情况下，被处理的事件中的后一个将不会产生任何效果，因为第一个事件已经促使状态机从并行状态中退出了。
**检测某个组合状态已经完成**
一个孩子状态可为最终状态（一个[QFinalState](http://doc.qt.nokia.com/4.6/qfinalstate.html)对象）。当进入最终状态时，父状态发出[QState::finished](http://doc.qt.nokia.com/4.6/qstate.html#finished)()信号。下图显示了一个组合状态s1，在进入最终状态之前执行一些处理：
![](http://hi.csdn.net/attachment/201012/29/0_12936403717m67.gif)
当进入s1的最终状态时，s1会自动地发出finished()。我们使用一个转变来促使这个事件触发一个状态改变：
s1->addTransition(s1, SIGNAL(finished()), s2);
在组合状态中使用最终状态是有用的，当你想隐藏一个组合状态的内部细节时；例如，位于该组合状态之外的世界只需能进入到该状态并在该状态完成了其工作时获得通知。在构建复杂的状态机（深度嵌套）时，这是一种非常强大的抽象和封装机制。（在以上例子中，当然你可以创建一个直接从s1的done状态开始的一个转变，而不依赖s1的finished()信号，但是，会造成s1的实现细节暴露并依赖它。）。
对于并行状态组，当所有孩子状态进入了最终状态时会发出[QState::finished](http://doc.qt.nokia.com/4.6/qstate.html#finished)()信号。
**无目标转变**
一个转变不需要一个目标状态。无目标的转变可与其他转变一样的方式被触发；不同之处在于当无目标转变被触发时，它不会造成任何状态的改变。这可以允许你在当状态机处于某个特定状态时，对信号或事件作出响应而不用离开那个状态。例如：
QStateMachine machine;
QState *s1 = new QState(&machine);
QPushButton button;
QSignalTransition *trans = new QSignalTransition(&button, SIGNAL(clicked()));
s1->addTransition(trans);
QMessageBox msgBox;
msgBox.setText("The button was clicked; carry on.");
QObject::connect(trans, SIGNAL(triggered()), &msgBox, SLOT(exec()));
machine.setInitialState(s1);
该信息框在每次按钮被点击时显示，但是状态机仍然处于当前状态(s1)。然而，如果目标状态显式地设置为s1，s1会退出并且每次点击的时候进入（例如，会发出[QAbstractState::entered](http://doc.qt.nokia.com/4.6/qabstractstate.html#entered)()和[QAbstractState::exited](http://doc.qt.nokia.com/4.6/qabstractstate.html#exited)()信号）。
**事件，转变和哨卫**
一个[QStateMachine](http://doc.qt.nokia.com/4.6/qstatemachine.html)运行在自己的事件循环里，对于信号转变（[QSignalTransition](http://doc.qt.nokia.com/4.6/qsignaltransition.html)对象），当它截获了相应地信号，[QStateMachine](http://doc.qt.nokia.com/4.6/qstatemachine.html)会自动地发送一个[QStateMachine::SignalEvent](http://doc.qt.nokia.com/4.6/qstatemachine-signalevent.html)到自身。类似地，对于[QObject](http://doc.qt.nokia.com/4.6/qobject.html)事件转变（[QEventTransition](http://doc.qt.nokia.com/4.6/qeventtransition.html)对象），会发送一个[QStateMachine::WrappedEvent](http://doc.qt.nokia.com/4.6/qstatemachine-wrappedevent.html)。
你可以使用[QStateMachine::postEvent](http://doc.qt.nokia.com/4.6/qstatemachine.html#postEvent)()将自己的事件发送到状态机。
当发送一个自定义的事件到状态机，你一般也拥有一个或更多个自定义的转变，这些转变可以由这种类型的事件触发。为了创建一个这样的转变，你要创建一个[QAbstractTransition](http://doc.qt.nokia.com/4.6/qabstracttransition.html)子类并重新实现[QAbstractTransition::eventTest](http://doc.qt.nokia.com/4.6/qabstracttransition.html#eventTest)()方法，在该方法中，你检测某个事件是否与你的事件类型匹配（也可以采用其他的判断规则，如事件对象的属性）。下面我们定义了自已的事件类型，StringEvent，用于向状态机中发送字符串：
struct StringEvent : public QEvent
{
StringEvent(const QString &val)
: QEvent(QEvent::Type(QEvent::User+1)),
value(val) {}
QString value;
};
接下来，我们定义一个转变，仅当事件的字符串与某个特定的字符串（一个哨卫转变）匹配时才触发它。
class StringTransition : public QAbstractTransition
{
public:
StringTransition(const QString &value)
: m_value(value) {}
protected:
virtual bool eventTest(QEvent *e) const
{
if (e->type() != QEvent::Type(QEvent::User+1))*// StringEvent*
return false;
StringEvent *se = static_cast<StringEvent*>(e);
return (m_value == se->value);
}
virtual void onTransition(QEvent *) {}
private:
QString m_value;
};
在eventTest()的重载中，我们首先检测了事件类型是否是我们想要的类型。如果是的，我们将事件转换为一个StringEvent并执行字符串比较操作。
如下是一个使用了自定义事件和转变的状态图：
![](http://hi.csdn.net/attachment/201012/29/0_1293640474iB9j.gif)
该状态图的实现代码如下：
QStateMachine machine;
QState *s1 = new QState();
QState *s2 = new QState();
QFinalState *done = new QFinalState();
StringTransition *t1 = new StringTransition("Hello");
t1->setTargetState(s2);
s1->addTransition(t1);
StringTransition *t2 = new StringTransition("world");
t2->setTargetState(done);
s2->addTransition(t2);
machine.addState(s1);
machine.addState(s2);
machine.addState(done);
machine.setInitialState(s1);
一旦状态机启动，我们可以将事件发送给它。
machine.postEvent(new StringEvent("Hello"));
machine.postEvent(new StringEvent("world"));
没有被任何相关的转变处理的事件将自动由状态处理。这对于分组状态和提供这样的事件的一个默认处理是有用的；例如，如下状态图：
![](http://hi.csdn.net/attachment/201012/29/0_1293640600JpnY.gif)
对于深度嵌套的状态图，你可以添加这样的“回退（fallback）”转变
**使用恢复策略自动地恢复属性**
在一些状态机中，在精力集中在对状态中的属性进行赋值是有用的，而不是当状态不再活跃时恢复它们。如果你知道当状态机进入某个状态时，并且在该状态下没有显式地给属性一个值，属性总是应该恢复到它的初始状态，你可以设置全局的策略为[QStateMachine::RestoreProperties](http://doc.qt.nokia.com/4.6/qstatemachine.html#RestorePolicy-enum)。
QStateMachine machine;
machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
当该策略设置了后，状态机会自动地恢复所有的属性。如果它进入了一个状态，而某个给定的属性没有设置，它会首先寻找祖先的层次结构以查看该属性是否已定义。如果是的，该属性会被恢复到最近祖先定义的值。如果不是，它会被恢复到初始值。
如下代码所示：
QStateMachine machine;
machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
QState *s1 = new QState();
s1->assignProperty(object, "fooBar", 1.0);
machine.addState(s1);
machine.setInitialState(s1);
QState *s2 = new QState();
machine.addState(s2);
比如说，属性fooBar在状态机启动时值为0.0。当机器处于状态s1，属性值会为1.0，因为该状态显示地设置了该属性的值。当该机器处于状态s2，没有显式地定义该属性，因此它会被隐式地恢复为0.0。
如果我们使用嵌套的状态，父状态为该属性定义了一个值，所有其后裔并没有显式地定义该属性的值。
QStateMachine machine;
machine.setGlobalRestorePolicy(QStateMachine::RestoreProperties);
QState *s1 = new QState();
s1->assignProperty(object, "fooBar", 1.0);
machine.addState(s1);
machine.setInitialState(s1);
QState *s2 = new QState(s1);
s2->assignProperty(object, "fooBar", 2.0);
s1->setInitialState(s2);
QState *s3 = new QState(s1);
这里，s1拥有两个孩子：s2和s3。当进入s2时，属性fooBar的值为2.0，因为该状态显式地定义了该值。当状态机处于状态s3时，该状态没有定义任何值，但是s1定义了属性的值为1.0，因此，这就是将被赋给fooBar的值。
**动画属性赋值**
状态机API与动画API的连接使得当在状态中设置动画属性时，自动地animating属性。比如，我们有如下代码：
QState *s1 = new QState();
QState *s2 = new QState();
s1->assignProperty(button, "geometry", QRectF(0, 0, 50, 50));
s2->assignProperty(button, "geometry", QRectF(0, 0, 100, 100));
s1->addTransition(button, SIGNAL(clicked()), s2);
这里，我们定义了用户界面的两个状态，在状态s1中，button小些，在状态s2中，button大些。如果我们点击按钮，从状态s1转换到状态s2，当给定的状态进入时，该按钮的几何属性可以立即设置。但是，如果我们想让转变更为流畅，需要构造一个[QPropertyAnimation](http://doc.qt.nokia.com/4.6/qpropertyanimation.html)对象并将其添加到转变对象中。
QState *s1 = new QState();
QState *s2 = new QState();
s1->assignProperty(button, "geometry", QRectF(0, 0, 50, 50));
s2->assignProperty(button, "geometry", QRectF(0, 0, 100, 100));
QSignalTransition *transition = s1->addTransition(button, SIGNAL(clicked()), s2);
transition->addAnimation(new QPropertyAnimation(button, "geometry"));
为属性添加了一个动画后，属性的赋值不再当进入状态时马上起效。相反地，动画在状态进入时开始播放并平滑地使属性赋值动起来。因为我们没有设置运行的起始值和结束值，这些将隐式地设置。动画的起始值将是动画开始时的当前值。
如果状态机的全局恢复策略设置为[QStateMachine::RestoreProperties](http://doc.qt.nokia.com/4.6/qstatemachine.html#RestorePolicy-enum)，也可以为恢复属性添加动画。
**检测某个状态下的所有属性**
当动画用于赋值时，一个状态不再定义当状态机进入该状态时的精确值。当动画正在运行时，属性可以拥有任何值，取决于动画。
在一些情况下，当能检测到某个属性被一个状态定义的实际值时是有用的。
比如，我们有如下代码：
QMessageBox *messageBox = new QMessageBox(mainWindow);
messageBox->addButton(QMessageBox::Ok);
messageBox->setText("Button geometry has been set!");
messageBox->setIcon(QMessageBox::Information);
QState *s1 = new QState();
QState *s2 = new QState();
s2->assignProperty(button, "geometry", QRectF(0, 0, 50, 50));
connect(s2, SIGNAL(entered()), messageBox, SLOT(exec()));
s1->addTransition(button, SIGNAL(clicked()), s2);
当button点击后，状态机将转换到状态s2，它会设置按钮的geometry属性，然后弹出一个信息框来提示用户geometry已经改变。
在正常情况下，没有使用动画时，该操作会以预期地方式执行。但是，如果在状态s1到s2的转变中为button的属性geometry定义了一个动画，该动画将在进入s2时启动，但是，在动画结束运行之前，geometry属性并不会到达它定义的值。在这种情况下，在button的geometry属性实际被设置之前，会弹出一个信息框。
为了确保信息框直到geometry达到它的最终值的时候才弹出，我们可以使用状态的propertiesAssigned()信号，当属性被赋予最终的值时，就会发出propertiesAssigned()信号。
QMessageBox *messageBox = new QMessageBox(mainWindow);
messageBox->addButton(QMessageBox::Ok);
messageBox->setText("Button geometry has been set!");
messageBox->setIcon(QMessageBox::Information);
QState *s1 = new QState();
QState *s2 = new QState();
s2->assignProperty(button, "geometry", QRectF(0, 0, 50, 50));
QState *s3 = new QState();
connect(s3, SIGNAL(entered()), messageBox, SLOT(exec()));
s1->addTransition(button, SIGNAL(clicked()), s2);
s2->addTransition(s2, SIGNAL(propertiesAssigned()), s3);
在该例子中，当button点击时，状态机进入s2，当仍然处于状态s2直到属性geometry被设置为QRect(0,
 0, 50, 50)。然后，它会转变到s3。当进入s3时，信息框会弹出。如果转变到s2有一个geometry属性的动画，那么状态机将会处于s2中直到动画完成。如果没有这样的动画，它会设置该属性并立即进入状态s3。
不管什么方式，当状态机处于状态s3，可以保证属性geometry已经被赋予了定义的值。如果全局恢复策略设置为[QStateMachine::RestoreProperties](http://doc.qt.nokia.com/4.6/qstatemachine.html#RestorePolicy-enum),该状态不会发出propertiesAssigned()信号，直到这些也被执行了。
在动画完成之前某个状态退出了会发生什么
如果一个状态有属性被赋值并且状态的转变过程中为该属性设置了动画，状态有可能在动画完成之前退出。这是可能发生的，特别当从状态的转变出来的一些转变不依赖于propertiesAssigned()信号。
状态机API保证一个被状态机赋值的属性：
——拥有显式赋给该属性的一个值
——是当前正被渐进到一个显式地赋予给该属性的值。
当一个状态在动画完成之前退出时，状态机的行为取决于转变的目标状态。如果目标状态显式地为属性赋予了一个值，不会采用另外的动作。属性将被赋予由目标状态定义的值。
如果目标状态没有赋予属性任何值，有两种选择：默认的，属性会被赋予它离开时的状态的值。但是，如果设置了全局恢复策略，优先采取这种选择，属性会像平常一样被恢复。
**默认动画**
正如早前所描述的一样，你可以添加动画到转变中以确保目标状态的属性赋值会被渐变。如果你想为某个给定的属性使用一个特定的动画而不管采用什么转变，你可以添加它作为状态机的一个默认的动画。
QState *s1 = new QState();
QState *s2 = new QState();
s2->assignProperty(object, "fooBar", 2.0);
s1->addTransition(s2);
QStateMachine machine;
machine.setInitialState(s1);
machine.addDefaultAnimation(new QPropertyAnimation(object, "fooBar"));
当状态机处于状态s2，状态机会为属性fooBar播放默认的动画，因为该属性由s2赋值。注意，显式地设置转变动画比默认动画优先级大。
