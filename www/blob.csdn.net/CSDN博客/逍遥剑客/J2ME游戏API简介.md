# J2ME游戏API简介 - 逍遥剑客 - CSDN博客
2006年11月30日 23:56:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4756
1 游戏API简介
    MIDP 2.0相对于1.0来说，最大的变化就是新添加了用于支持游戏的API，它们被放在javax.microedition.lcdui.game包中。游戏API包提供了一系列针对无线设备的游戏开发类。由于无线设备仅有有限的计算能力，因此许多API的目的在于提高Java游戏的性能，并且把原来很多需要手动编写的代码如屏幕双缓冲、图像剪裁等都交给API间接调用本地代码来实现。各厂家有相当大的自由来优化它们。
游戏API使用了MIDP的低级图形类接口（Graphics，Image，等等）。整个game包仅有5个Class：
GameCanvas
这个类是LCDUI的Canvas类的子类，为游戏提供了基本的“屏幕”功能。除了从Canvas继承下来的方法外，这个类还提供了游戏专用的功能，如查询当前游戏键状态的能力，同步图像输出；这些功能简化了游戏开发并提高了性能。
Layer
Layer类代表游戏中的一个可视化元素，例如Sprite或TiledLayer是它的子类；这个抽象类搭好了层(Layer)的基本框架并提供了一些基本的属性，如位置，大小，可视与否。出于优化的考虑，不允许直接产生Layer的子类（不能包外继承）。
LayerManager
对于有着许多Layer的游戏而言，LayerManager通过实现分层次的自动渲染，从而简化了游戏开发。它允许开发者设置一个可视窗口(View Window)，表示用户在游戏中可见的窗口； LayerManager自动渲染游戏中的Layer，从而实现期望的视图效果。
Sprite
Sprite又称“精灵”，也是一种Layer，可以显示一帧或多帧的连续图像。但所有的帧都是相同大小的，并且由一个Image对象提供。Sprite通过循环显示每一帧，可以实现任意顺序的动画；Sprite类还提供了许多变换（翻转和旋转）模式和碰撞检测方法，能大大简化游戏逻辑的实现。
TiledLayer
TiledLayer又称“砖块”，这个类允许开发者在不必使用非常大的Image对象的情况下创建一个大的图像内容。TiledLayer有许多单元格构成，每个单元格能显示由一个单一Image对象提供的一组贴图中的某一个贴图。单元格也能被动画贴图填充，动画贴图的内容能非常迅速地变化；这个功能对于动画显示非常大的一组单元格非常有用，例如一个充满水的动态区域。
    在游戏中，某些方法如果改变了Layer，LayerManager，Sprite和TiledLayer对象的状态，通常并不能立刻显示出视觉变化。因为这些状态仅仅存储在对象里，只有当随后调用我们自己的paint()方法时才会更新显示。这种模式非常适合游戏程序，因为在一个游戏循环中，一些对象的状态会更新，在每个循环的最后，整个屏幕才会被重绘。基于轮询也是现在视频游戏的基本结构。
2   GameCanvas的使用
    GameCanvas类提供了基本的游戏用户接口。除了从Canvas继承下来的特性（命令，输入事件等）以外，它还提供了专门针对游戏的功能，比如后备屏幕缓冲和键盘状态查询的能力。
    每个GameCanvas实例都会有一个为之创建的专用的缓冲区。因为每个GameCanvas实例都会有一个唯一的缓冲区。可以从GameCanvas实例获得其对应的Graphics对象，而且，只有对Graphics对象操作，才会修改缓冲区的内容。外部资源如其他的MIDlet或者系统级的通知都不会导致缓冲区内容被修改。该缓冲区在初始化时被填充为白色。
    缓冲区大小被设置为GameCanvas的最大尺度。然而，当请求填充时，可被填充的区域大小会受限于当前GameCanvas的尺度，一个存在的Ticker，Command等等都会影响到GameCanvas的大小。GameCanvas的当前大小可以通过调用getWidth和getHeight获得。 
    一个游戏可能提供自己的线程来运行游戏循环。一个典型的循环将检查输入，实现游戏逻辑，然后渲染更新后的用户界面。以下代码演示了一个典型的游戏循环的结构：
// 从后备屏幕缓冲获得Graphics对象
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)Graphics g = getGraphics();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
while (true) ...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// 检查用户输入并更新位置，如果有需要
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int keyState = getKeyStates();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)if ((keyState & LEFT_PRESSED) !=0) ...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      sprite.move(-1, 0);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)elseif ((keyState & RIGHT_PRESSED) !=0) ...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      sprite.move(1, 0);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// 将背景清除成白色
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    g.setColor(0xFFFFFF);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    g.fillRect(0,0,getWidth(), getHeight());
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// 绘制Sprite（精灵）
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    sprite.paint(g);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)// 输出后备缓冲区的内容
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    flushGraphics();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
2.1   绘图
要创建一个新的GameCanvas实例，只能通过继承并调用父类的构造函数：
protected GameCanvas(boolean suppressKeyEvents)，
这将使为GameCanvas准备的一个新的缓冲区也被创建并在初始化时被填充为白色。
为了在GameCanvas上绘图，首先要获得Graphics对象来渲染GameCanvas：
protected Graphics getGraphics()
返回的Graphics对象将用于渲染属于这个GameCanvas的后备屏幕缓冲区(off-screen buffer)。 但是渲染结果不会立刻显示出来，直到调用flushGraphics()方法；输出缓冲区的内容也不会改变缓冲区的内容，即输出操作不会清除缓冲区的像素。
每次调用这个方法时，都会创建一个新的Graphics对象；对于每个GameCanvas实例，获得的多个Graphics对象都将渲染同一个后备屏幕缓冲区。因此，有必要在游戏启动前获得并存储Graphics对象，以便游戏运行时能反复使用。
刚创建的Graphics对象有以下属性：
l       渲染目标是这个GameCanvas的缓冲区；
l       渲染区域覆盖整个缓冲区；
l       当前颜色是黑色(black)；
l       字体和调用Font.getDefaultFont()返回的相同；
l       绘图模式为SOLID；
l       坐标系统的原点定位在缓冲区的左上角。
    在完成了绘图操作后，可以使用flushGraphics()方法将后备屏幕缓冲区的内容输出到显示屏上。输出区域的大小与GameCanvas的大小相同。输出操作不会改变后备屏幕缓冲区的内容。这个方法会直到输出操作完成后才返回，因此，当这个方法返回时，应用程序可以立刻对缓冲区进行下一帧的渲染。 
  如果GameCanvas当前没有被显示，或者系统忙而不能执行输出请求，这个方法不做任何事就立刻返回。
2.2   键盘
如果需要，开发者可以随时调用getKeyStates方法来查询键的状态。getKeyStates()获取游戏的物理键状态。返回值的每个比特位都表示设备上的一个特定的键。如果一个键对应的比特位的值为1，表示该键当前被按下，或者自上次调用此方法后到现在，至少被按下过一次。如果一个键对应的比特位的值为0，表示该键当前未被按下，并且自上次调用此方法后到现在从未被按下过。这种“闭锁行为(latching behavior)”保证一个快速的按键和释放总是能够在游戏循环中被捕获，不管循环有多慢。下面是获取游戏按键的示例：
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)// 获得键的状态并存储
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int keyState = getKeyStates();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
if ((keyState & LEFT_KEY) !=0) ...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      positionX--;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
elseif ((keyState & RIGHT_KEY) !=0) ...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)      positionX++;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)    }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
  调用这个方法的副作用是不能及时清除过期的状态。在一个getKeyStates调用后如果紧接着另一个调用，键的当前状态将取决于系统是否已经清除了上一次调用后的结果。
    某些设备可能无法直接访问键盘硬件，因此，这个方法可能是通过监视键的按下和释放事件来实现的，这会导致getKeyStates可能滞后于当前物理键的状态，因为时延取决于每个设备的性能。某些设备还可能没有探测多个键同时按下的能力。
    请注意，除非GameCanvas当前可见（通过调用Displayable.isShown()方法），否则此方法返回0。一旦GameCanvas变为可见，将初始化所有键为未按下状态(0)。 
