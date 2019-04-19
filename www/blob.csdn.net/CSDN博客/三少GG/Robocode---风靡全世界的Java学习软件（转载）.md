# Robocode---风靡全世界的Java学习软件（转载） - 三少GG - CSDN博客
2009年11月21日 17:47:00[三少GG](https://me.csdn.net/scut1135)阅读数：1289
原文地址：[http://bbs.cfan.com.cn/thread-174922-1-1.html](http://bbs.cfan.com.cn/thread-174922-1-1.html)
Robocode 是一个很容易使用的机器人战斗仿真器，可以在所有支持 Java 2 的平台上运行。您创建一个机器人，把它放到战场上，然后让它同其他开发者们创建的机器人对手拼死战斗到底。Robocode 里有一些预先做好的机器人对手让你入门，但一旦您不再需要它们，就可以把您自己创建的机器人加入到正在世界范围内形成的某个联盟里去和世界最强手对阵。
每个 Robocode 参加者都要利用 Java 语言元素创建他或她的机器人，这样就使从初学者到高级黑客的广大开发者都可以参与这一娱乐活动。初级的 Java 的开发者们可以学习一些基础知识：调用 API 代码、阅读 Javadoc、继承、内部类、事件处理等等。高级开发者们可以在构建“最优品种”的软件机器人全球竞赛中提高他们的编程技巧。在本文中，我们将介绍
 Robocode，并指导您从构建您平生第一个 Robocode 机器人开始征服世界。我们还将 看一下迷人的“后台”机制，正是它使得 Robocode 起作用。 
下载并安装 Robocode 
Robocode 是 Mathew Nelson 的智慧之作，他是 IBM Internet 部门 Advanced Technology 的软件工程师。请首先访问IBM alphaWorks Robocode页面。在这个页面上，您可以找到 Robocode
系统最新的可执行文件。这个分发包是一个自包含的安装文件，在下载该分发包之后，您就可以使用下面的命令行在您的系统上安装这个软件包（当然，我们假定您的机器上已经预安装了 Java VM（JDK 1.3.x））：
java -jar robocode-setup.jar 
在安装过程中，Robocode 将问您是否要使用这个外部的 Java VM 来编译机器人。您也可以选择使用作为 Robocode 分发包一部分而提供的 Jikes 编译器。
安装完成后，您可以通过 shell 脚本（robocode.sh）、批处理文件（robocode.bat）或桌面上的图标来启动 Robocode 系统。此时，战场将会出现。在此，您可以通过菜单调用 Robot Editor 和 compiler。
Robocode 系统组件 
当您激活 Robocode 时，将看到两个相关的 GUI 窗口，这两个窗口构成了 Robocode 的 IDE： 
战场 
Robot Editor 
图 1 展示了处于工作状态的战场和 Robot Editor。 
图 1. Robocode IDE 
战场是机器人之间进行战斗直至分出胜负的场地。主要的仿真引擎被置于其中，并且允许您在这里创建战斗、保存战斗以及打开新建的或现有的战斗。通过界面区域内的控件，您可以暂停或继续战斗、终止战斗、消灭任何机器人个体或获取任何机器人的统计数据。此外，您可以在此屏幕上激活 Robot Editor。
Robot Editor 是一个定制的文本编辑器，它可以用于编辑生成机器人的 Java 源文件。在它的菜单里集成了 Java 编译器（用于编译机器人代码）以及定制的 Robot 打包器。由 Robot Editor 创建并成功编译的所有机器人都会处于战场上一个部署就绪的位置。
Robocode 里的每个机器人都由一个或多个 Java 类构成。这些类可以被压缩成一个 JAR 包。为此，Robocode 的最新版本提供了一个可以在战场 GUI 窗口中激活的“Robot Packager”。
对 Robocode 机器人的详细分析 
在写这篇文章时，Robocode 机器人是一个图形化的坦克。图 2 是一个典型的 Robocode 机器人的图解。 
图 2. 对 Robocode 机器人的详细分析 
请注意，机器人有一门可以旋转的炮，炮上面的雷达也是可以旋转的。机器人坦克车（Vehicle）、炮（Gun）以及雷达（Radar）都可以单独旋转，也就是说，在任何时刻，机器人坦克车、炮以及雷达都可以转向不同的方向。缺省情况下，这些方向是一致的，都指向坦克车运动的方向。
Robot 命令 
Robocode 机器人的命令集都收录在 Robocode API Javadoc 中。您将会发现这些命令都是 robocode.Robot 类的公共方法。在这一部分，我们将分类讨论每个可用的命令。
移动机器人、炮和雷达 
让我们从移动机器人及其装备的基本命令开始： 
turnRight(double degree) 和 turnLeft(double degree) 使机器人转过一个指定的角度。 
ahead(double distance) 和 back(double distance) 使机器人移动指定的像素点距离；这两个方法在机器人碰到墙或另外一个机器人时即告完成。
turnGunRight(double degree) 和 turnGunLeft(double degree) 使炮可以独立于坦克车的方向转动。 
turnRadarRight(double degree) 和 turnRadarLeft(double degree) 使炮上面的雷达转动，转动的方向也独立于炮的方向（以及坦克车的方向）。
这些命令都是在执行完毕后才把控制权交还给程序。此外，转动坦克车的时候，除非通过调用下列方法分别指明炮（和雷达）的方向，否则炮（和雷达）的指向也将移动。 
setAdjustGunForRobotTurn(boolean flag)：如果 flag 被设置成 true，那么坦克车转动时，炮保持原来的方向。
setAdjustRadarForRobotTurn(boolean flag)：如果 flag 被设置成 true，那么坦克车（和炮）转动时，雷达会保持原来的方向。
setAdjustRadarForGunTurn(boolean flag)：如果 flag 被设置成 true，那么炮转动时，雷达会保持原来的方向。而且，它执行的动作如同调用了 setAdjustRadarForRobotTurn(true)。
获取关于机器人的信息 
有许多方法可以得到关于机器人的信息。下面简单列举了常用的方法调用： 
getX() 和 getY() 可以捕捉到机器人当前的坐标。 
getHeading()、getGunHeading() 和 getRadarHeading() 可以得出坦克车、炮或雷达当前的方向，该方向是以角度表示的。 
getBattleFieldWidth() 和 getBattleFieldHeight() 可以得到当前这一回合的战场尺寸。 
射击命令 
一旦您掌握了移动机器人以及相关的武器装备的方法，您就该考虑射击和控制损害的任务了。每个机器人在开始时都有一个缺省的“能量级别”，当它的能量级别减小到零的时候，我们就认为这个机器人已经被消灭了。射击的时候，机器人最多可以用掉三个能量单位。提供给炮弹的能量越多，对目标机器人所造成的损害也就越大。 fire(double power) 和 fireBullet(double power) 用来发射指定能量（火力）的炮弹。调用的 fireBullet() 版本返回 robocode.Bullet 对象的一个引用，该引用可以用于高级机器人。
事件 
每当机器人在移动或转动时，雷达一直处于激活状态，如果雷达检测到有机器人在它的范围内，就会触发一个事件。作为机器人创建者，您有权选择处理可能在战斗中发生的各类事件。基本的 Robot 类中包括了所有这些事件的缺省处理程序。但是，您可以覆盖其中任何一个“什么也不做的”缺省处理程序，然后实现您自己的定制行为。下面是一些较为常用的事件：
ScannedRobotEvent。通过覆盖 onScannedRobot() 方法来处理 ScannedRobotEvent；当雷达检测到机器人时，就调用该方法。
HitByBulletEvent。通过覆盖 onHitByBullet() 方法来处理 HitByBulletEvent；当机器人被炮弹击中时，就调用该方法。 
HitRobotEvent。通过覆盖 onHitRobot() 方法来处理 HitRobotEvent；当您的机器人击中另外一个机器人时，就调用该方法。 
HitWallEvent。通过覆盖 onHitWall() 方法来处理 HitWallEvent；当您的机器人撞到墙时，就调用该方法。 
我们只需要知道这些就可以创建一些相当复杂的机器人了。您可以通过战场的帮助菜单或 Robot Editor 的帮助菜单访问 Javadoc 中其余的 Robocode API。
现在，我们该把学到的知识付诸实践了。 
创建机器人 
要创建一个新的机器人，请启动 Robot Editor 并选择 File->New->Robot。系统将会提示您输入机器人的名称，这个名称将成为 Java 类名。请您在提示符处输入 DWStraight。接下来，系统还会提示您输入一个独一无二的包前缀，它将用作存放机器人（还可能有相关的 Java 文件）的包的名称。请在该提示符处输入 dw。
Robot Editor 就会显示您要控制这个机器人需要编写的 Java 代码。清单 1 是您将会看到的代码的一个示例： 
清单 1. Robocode 生成的 Robot 代码 package dw;
import robocode.*; 
public class DWStraight extends Robot 
{ 
... // <> 
public void run() { 
... // <> 
while(true) { 
... // <> 
} 
} 
... // <> 
public void onScannedRobot(ScannedRobotEvent e) { 
fire(1); 
} 
} 
突出显示的区域就是我们添加控制机器人的代码的地方： 
Area 1 
我们可以在这片空白里声明类作用域变量并设置这些变量的值。这些变量可以在机器人的 run() 方法内以及其他一些您可能创建的助手方法内使用。 
Area 2 
战斗管理器调用 run() 方法激活机器人。典型情况下，run() 方法包括两个区域（即在清单 1 中指出的 Area 2 和 Area 3），您可以在这两块空白里添加代码。您在 Area 2 中加入的代码每个机器人实例只运行一次。这部分代码通常用于使机器人先处于一种预设状态后再开始执行重复行为。
Area 3 
这是典型的 run() 方法实现的第二部分。在此，我们将在无限 while 循环内对机器人可能执行的重复行为进行编程。 
Area 4 
您可以在这一区域内添加机器人在 run() 逻辑内使用的助手方法。您也可以在此添加您想要覆盖的任何事件处理程序。例如，清单 1 里的代码处理 ScannedRobot 事件，每当雷达检测到机器人的时候，只是直接向其发射炮弹。
我们对第一个机器人（DWStraight）的代码的更新如清单 2 中红色标记所示。 
清单 2. DWStraight 机器人代码的增加部分 package dw;
import robocode.*; 
public class DWStraight extends Robot 
{ 
public void run() { 
turnLeft(getHeading()); 
while(true) { 
ahead(1000); 
turnRight(90); 
} 
} 
public void onScannedRobot(ScannedRobotEvent e) { 
fire(1); 
} 
public void onHitByBullet(HitByBulletEvent e) { 
turnLeft(180); 
} 
}  
下面我们逐区地描述这个第一个机器人将做些什么： 
Area 1 
我们没有在这个机器人的程序中指定任何类作用域变量。 
Area 2 
为了使机器人处于已知的状态，我们通过 turnLeft(getHeading()) 使它转到 0 度的方向。 
Area 3 
在这个重复性的部分，我们使用语句 ahead(1000) 让机器人尽其所能向前移动到最远的地方。当机器人撞到墙或其他机器人时，就会停下来。接着，我们通过 turnRight(90) 使它向右转。在重复执行这一行为时，机器人基本上是在沿着墙按顺时针方向移动。
Area 4 
在此，除处理自动生成的 ScannedRobot 事件并向被发现的机器人直接射击之外，我们还会检测 HitByBullet 事件，并且让机器人在被击中的时候转过 180 度（沿顺时针方向或逆时针方向）。
编译以及测试机器人 
在 Robot Editor 菜单上选择 Compiler->Compile 编译您的机器人代码。现在我们可以尝试第一回合的战斗了。切换回战场并选择菜单上的 Battle->New，将会出现一个类似于图 3 中所示的对话框。
图 3. New Battle 对话框 
请先将我们的机器人 dw.DWStraight 添加到战斗中，然后再添加一个对手机器人，比如 sample.SittingDuck。单击 Finish，战斗就开始了。不可否认，同 SittingDuck 战斗并不怎么有趣，但是您可以目睹这个叫做 DWStraight 的机器人在缺省情况下的行为。试试 sample 文件夹里的其他机器人，看看 DWStraight 同这些机器人的战斗情况如何。
当您准备开始研究另外一个机器人的代码时，请先看看随参考资料中的代码分发包一起提供的 dw.DWRotater 这个机器人的代码。在缺省情况下，这个机器人将会： 
移动到战场中心 
一直转动它的炮，直到检测到机器人 
每次尝试以不同的角度在离被检测到的机器人前方不远的地方射击 
每当它被另外一个机器人击中时，它都会迅速的来回移动 
这段代码简单易懂，所以我们在这里就不做分析了，但是我鼓励您试验一下。Robocode 中的 sample 包还提供了许多其他机器人的代码。 
附加的机器人支持类 
随着您设计机器人的水平的提高，机器人的代码主体将充分增长。对这些代码的一种模块化处理方法是把代码分解成独立的 Java 类，然后通过打包器把这些 Java 类打包成一个单独的包（JAR 文件），并将它包括在您的机器人分发包内。Robocode 将自动在它的 robots 目录下的包里找到 robot 类。
其他 Robot 子类 
任何人都可以创建 Robot 子类并添加用于构建机器人的新功能。Robocode 提供了一个叫做 AdvancedRobot 的 Robot 子类，它允许异步 API 调用。虽然对 AdvancedRobot 类的描述超出了本文的范围，但我鼓励您在掌握了基本的 Robot 类的操作后，试验一下这个高级类。
设计 Robocode 的目的 
我碰见了 Robocode 的创建者 Mathew Nelson，向他请教创建 Robocode 最初的设计目的。Mat 所说的是：“编写 Robocode 的一部分目的是为了向世界证明：象‘Java 比较慢’以及‘Java 不可以用来写游戏’之类的论断不再正确。我认为我证明了这一点。”
战斗仿真器的体系结构 
通过“在后台”对 Robocode 进行分析，我们发现复杂的仿真引擎既具高性能（为了以现实的速度生成战斗）又具灵活性（使创建复杂的机器人逻辑不存在障碍）。特别感谢 Robocode 的创建者 Mathew Nelson 无私的提供了仿真引擎体系结构的内部信息。
利用 Java 平台进行设计 
图 4 中所示的仿真引擎利用的是大多数现代的 Java VM 都提供的非抢占式线程技术，并结合使用了 JDK GUI 和 2D 图形库提供的生成功能。
图 4. Robocode 仿真引擎体系结构 
请注意，所仿真的每个机器人都在它自己的 Java 线程上，它可以在任何可适用的地方利用 VM 本地线程映射机制。战斗管理器线程是系统的控制器：它安排仿真并驱动图形化的生成子系统。图形化的生成子系统本身是基于 Java 2D 和 AWT 的。
松散的线程耦合 
为了减少共享资源可能带来的问题（以及有可能随之出现的死锁或阻塞仿真引擎），战斗管理器线程和机器人线程之间的耦合应当非常松散。为了实现这种松散耦合，每个机器人线程都将有属于自己的事件队列。获取及处理这些事件都是在每个机器人自己的线程内进行。这种基于线程的队列有效地消除了战斗管理器线程和机器人线程之间（或机器人线程本身之间）可能存在的任何争用。
Robocode 内部结构 
您可以把 Robocode 仿真器引擎看作是一个仿真器程序，该程序在运行时会使用一些插件（定制机器人）；这些插件可以利用已有的 API（robocode.Robot 类的方法）。实际上，每个机器人都是一个独立的 Java 线程，同时 run() 方法内包含了每个线程上将要执行的逻辑。
在任何时候，机器人线程都可以调用由它的父类 robocoode.Robot 类所提供的 API。典型情况下，这将通过调用 Object.wait() 阻塞机器人线程。
战斗管理器线程 
战斗管理器线程管理机器人、炮弹及它们在战场上的生成。仿真“时钟”是根据战场上生成的帧的数目来标记的。用户可以调整真实的帧的速度。 
在一个典型的回合中，战斗管理器线程唤醒每个机器人线程，然后等待机器人完成它的一轮战斗(即，再次调用一个阻塞 API）。等待的间隔时间通常是几十毫秒，即使是最复杂的机器人，使用现今典型的系统速度进行策略安排和计算，也只要 1 到 2 毫秒的时间。
以下是战斗管理器线程执行的逻辑的伪代码：
while (round is not over) do 
call the rendering subsystem to draw robots, bullets, explosions 
for each robot do 
wake up the robot 
wait for it to make a blocking call, up to a max time interval 
end for 
clear all robot event queue 
move bullets, and generate event into robots' event queue if applicable 
move robots, and generate event into robots' event queue if applicable 
do battle housekeeping and generate event into robots' event queue 
if applicable 
delay for frame rate if necessary 
end do 
请注意，在 for 循环内部，战斗管理器线程的等待时间不会超过最大的时间间隔。如果机器人线程没有及时调用阻塞 API（典型情况下是由于一些应用程序逻辑错误或无限循环），那么，它将继续进行战斗。生成一个 SkippedTurnEvent 并将其加入机器人事件队列中，用来通知高级机器人。
可替换的生成子系统 
AWT 和 Java 2D 线程就是当前实现中的生成子系统，它从战斗管理器中获取命令并生成战场。它同系统的其余部分是完全分离的。我们可以预见到，在这个生成子系统将来的修订版中，它可以被替换掉（比如，用 3-D 生成器）。在当前的实现中，只要 Robocode 应用程序被最小化，生成就禁用了，这可以以更快的速度进行仿真。
Robocode 的未来 
通过 alphaWorks Robocode 站点上的一个讨论组（请参阅参考资料），Mathew Nelson 可以同 Robocode 用户社区保持紧密的反馈联系。许多反馈都并入了真实的代码中。Mathew 已计划即将要进行的一些改进有：
通过不同的物体和障碍来定制战场地图 
基于团队的战斗 
对联赛或联盟的集成支持 
用户可选择坦克车体／炮／雷达／武器的样式 
挡不住的 Robocode 风潮 
对于一个从 2001 年 7 月 12 日出现在公众面前的项目，Robocode 的出名简直让人吃惊。尽管最新的可用版本还不到 1.0（在写这篇文章时是版本 0.98.2），但它已经是全世界的大学校园以及公司的 PC 机上颇受欢迎的娱乐活动了。Robocode 联盟（或 roboleagues）正如雨后春笋般出现，在这些联盟里，人们通过因特网让自己定制的作品相互较量。大学教授们一直在挖掘 Robocode 的教育特性，并且已经把它纳入了大学里的计算机科学课程。在因特网上，Robocode 用户组、讨论列表、FAQ、教程和
 Webring 随处可见。 
显然，Robocode 已经填补了大众化的寓教于乐领域的空白 — 它为学生们和熬夜的工程师们提供简便、有趣、非胁迫却富竞争力的方式，释放他们的创造力，而且有可能实现他们征服世界的梦想。
参考资料 
请单击文章顶部或底部的讨论参与本文的讨论论坛。 
请下载本文所主要描述的两个简单的机器人的源代码[http://www-900.ibm.com/developer ... bocode/source.zip。](http://www-900.ibm.com/developerWorks/cn/java/j-robocode/source.zip%E3%80%82)
Mathew Nelson 是 Robocode 的创建者，他维护着官方 Robocode 站点。这应当是所有关心 Robocode 的人的第一站。在此，您还可以参加由 Mathew Nelson 主持的 讨论组，管理员就是 Mathew Nelson。您还可以看看 to-do list，这个列表是对请求功能的“持续更新”的列表。
为了联盟游戏，RoboLeague 正在与 Robocode 一起同时开发。 
您可以在 Robocode 资源库找到另外一个很出色的 Robocode 站点，上面有机器人上载和下载区域和一个活跃的讨论论坛。您可以在那里下载到一些非常高级的机器人以及它们的源代码，以促进您的 Robocode 学习。
Diverman 是一个活跃的 Robocode 热衷者，运作着他自己的 Robocode Fanatics 站点。这个站点的特色是可以容易地上载和下载机器人,并可以进行讨论和民意测验。
如果您已经是 Yahoo 成员，那么可以通过 Yahoo Robocode 电子部落分享 Robocode 信息。
