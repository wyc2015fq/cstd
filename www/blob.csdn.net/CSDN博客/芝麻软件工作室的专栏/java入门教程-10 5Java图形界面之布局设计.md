
# java入门教程-10.5Java图形界面之布局设计 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月28日 08:43:05[seven-soft](https://me.csdn.net/softn)阅读数：1792


在界面设计中，一个容器要放置许多组件，为了美观，为组件安排在容器中的位置，这就是布局设计。java.awt中定义了多种布局类，每种布局类对应一种布局的策略。常用的有以下布局类：FlowLayout,依次放置组件。
BoarderLayout,将组件放置在边界上。
CardLayout,将组件像扑克牌一样叠放，而每次只能显示其中一个组件。
GridLayout,将显示区域按行、列划分成一个个相等的格子，组件依次放入这些格子中。
GridBagLayout,将显示区域划分成许多矩形小单元，每个组件可占用一个或多个小单元。
其中GridBagLayout能进行精细的位置控制，也最复杂，本教程暂不讨论这种布局策略，将在专题文章中进行详细讲解。
每个容器都有一个布局管理器，由它来决定如何安排放入容器内的的组件。布局管理器是实现LayoutManager接口的类。
## 一.FlowLayout布局 (JApplet,JPanel,JScrollPane默认布局)
FlowLayout布局是将其中的组件按照加入的先后顺序从左到右排列，一行满之后就转到一下行继续从左到右排列，每一行中的组件都居中排列。这是一种最简便的布局策略，一般用于组件不多的情况，当组件较多时，容器中的组件就会显得高低不平，各行长短不一。
FlowLayout是小应用程序和面板默认布局，FlowLayout布局的构造方法有：FlowLayout(),生成一个默认的FlowLayout布局。默认情况下，组件居中，间隙为5个像素。
FlowLayout(int aligment),设定每珩的组件的对齐方式。alignment取值可以为 FlowLayout.LEFT,FlowLayout.CENTER,FlowLayout.RIGHT。
FlowLayout(int aligment,int horz, int vert),设定对齐方式，并设定组件的水平间距horz和垂直间距vert，用超类Container的方法setLayout()为容器设定布局。例如，代码setLayout(new FlowLayout())为容器设定 FlowLayout布局。将组件加入容器的方法是add(组件名)。
## 二.BorderLayout布局(JWindow、JFrame,JDialog的默认布局)
BorderLayout布局策略是把容器内的空间简单划分为东“East”,西 “West”,南 “South”,北 “North”，中 “Center”五个区域。加入组件时，都应该指明把组件放在哪一个区域中。一个位置放一个组件。如果某个位置要加入多个组件，应先将要加入该位置的组件放放另一个容器中，然后再将这个容器加入到这个个位置。
BorderLayout布局的构造方法有：
(1) BorderLayout(),生成一个默认的BorderLayout布局。默认情况下，没有间隙。
(2) BorderLayout(int horz,int vert),设定组件之间的水平间距和垂直间距。
BorderLayout布局策略的设定方法是setLayout(new BorderLayout())。将组件加入到容器的方法是add(组件名，位置)，如果加入组件时没有指定位置，则默认为“中”位置。
BorderLayout布局是JWindow、JFrame,JDialog的默认布局。
【例 11-5】应用程序设有五个标签、分别放于窗口的东、西、南、北和中五个区域（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-5.txt)）。

## 三.GridLayout布局
GridLayout布局是把容器划分成若干行和列的网格状，行数和列数由程序控制，组件放在网格的小格子中。GridLayout布局的特点是组件定位比较精确。由于GridLayout布局中每个网格具有相同形状和大小，要求放入容器的组件也应保持相同的大小。
GridLayout布局的构造方法有：
(1) GridLayout(),生成一个单列的GridLayout布局。默认情况下，无间隙。
(2) GridLayout(int row,int col),设定一个有行row和列col的GridLayout布局。
(3) GridLayout(int row,int col,int horz,int vert),设定布局的行数和列数、组件的水平间距和垂直间距。
GridLayout布局以行为基准，当放置的组件个数超额时，自动增加列；反之，组件太少也会自动减少列，行数不变，组件按行优先顺序排列(根据组件自动增减列)。GridLayout布局的每个网格必须填入组件，如果希望某个网格为空白，可以用一个空白标签(add(new
 Label()))顶替。
【例 11-6】小应用程序先将若干个按钮和若干个标签放入JPanel中，然后将JPanel放入JScrollPane中，最后，将JScrollPane放入小程序的窗口中，程序所创建的JScrollPane总是带水平和垂直滚动条，滚动面板的可视范围小于面板的实际要求，可以移动滚动条的滑块显示面板原先不在可视范围内的区域（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-6.txt)）。
GridLayout布局要求所有组件的大小保持一致，这可能会使用界面外观不够美观。一个补救的办法是让一些小组件合并放在一个容器中，然后把这个容器作为组件，再放入到GridLayout布局中。这就是前面所说的容器嵌套。例如，容器A使用GridLayout布局，将容器均分为网格；另有容器B和C各放入若干组件后，把B和C分别作为组件添加到容器A中。容器B和C也可以设置为GridLayout布局，把自己分为若干网格，也可以设置成其他布局。这样，从外观来看，各组件的大小就有了差异。
## 四.CardLayout布局
采用CardLayout布局的容器虽可容纳多个组件，但是多个组件拥有同一个显示空间，某一时刻只能显示一个组件。就像一叠扑克牌每次只能显示最上面的一张一样，这个显示的组件将占据容器的全部空间。CardLayout布局设计步骤如下：
先创建CardLayout布局对象。然后，使用setLayout()方法为容器设置布局。最的，调用容器的add()方法将组件加入容器。CardLayout布局策略加入组件的方法是：
add(组件代号，组件);
其中组件代号是字符串，是另给的，与组件名无关。
例如，以下代码为一个JPanel容器设定CardLayout布局：
CardLayout myCard = new CardLayout();//创建CardLayout布局对象
JPanel p = new JPanel();//创建Panel对象
p.setLayout(myCard);
用CardLayout类提供的方法显示某一组件的方式有两种：
(1) 使用show(容器名，组件代号)形式的代码，指定某个容器中的某个组件显示。例如，以下代码指定容器p的组件代号k，显示这个组件：
myCard.show(p,k);
(2) 按组件加入容器的顺序显示组件。
first(容器)：例如，代码myCard.first(p);
last(容器)：例如 ， myCard.last(p);
next(容器)：例如，myCard.next(p);
previous(容器):myCard.previous(p);
【例11-7】小应用程序使用CardLayout布局，面板容器p使用CardLayout布局策略设置10个标签组件。窗口设有4个按钮，分别负责显示p的第一个组件、最后一个组件、当前组件的前一个组件和当前的组件的最后一个组件（[查看源文件](http://www.weixueyuan.net/uploads/code/java/rumen/11-7.txt)）。

## 五.null布局与setBounds方法
空布局就是把一个容器的布局设置为null布局。空布局采用setBounds()方法设置组件本身的大小和在容器中的位置：
setBounds(int x,int y,int width,int height)
组件所占区域是一个矩形，参数x,y是组件的左上角在容器中的位置坐标；参数weight,height是组件的宽和高。空布局安置组件的办法分两个步骤：先使用add()方法身容器添加组件。然后调用setBounds()方法设置组件在容器中的位置和组件本身的大小。与组件相关的其他方法：getSize().width,
getSize().height
setVgap(ing vgap)
setHgap(int hgap);

