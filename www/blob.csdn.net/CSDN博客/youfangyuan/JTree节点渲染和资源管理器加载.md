# JTree节点渲染和资源管理器加载 - youfangyuan - CSDN博客
2012年12月22日 01:29:50[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：3410
前面已经把树的基本使用、树的事件和树的数据取得写完了,并看了一个Sun提供的最简单的树的例子,这一专题我们讲两个方面,一个是使用树的Renderer和Editor构造一棵我们自己的树;一个树使用SwingWorker完成一棵资源管理器树的加载。（文/ [zeyuphoenix](http://www.blogjava.net/zeyuphoenix/)）
先看渲染的例子:
![JTree-- 树(节点渲染和资源管理器加载)(一)_25540](http://www.pin5i.com/attachment.aspx?attachmentid=25540)
![JTree-- 树(节点渲染和资源管理器加载)(一)_25541](http://www.pin5i.com/attachment.aspx?attachmentid=25541)
我们为我们的树增加了可用与否、可见与否和节点图片的属性,当然你也可以自己添加想要的属性,比如顺序等.
整个实现的过程很清晰,也不复杂,我们首先实现我们自己的TreeNode,它继承于DefaultMutableTreeNode,我们在里面添加自己的属性;然后是实现树的节点的Renderer和Editor,在Renderer里我们设置可用也否,选择状态,节点图片等;在Editor里我们设置树的容器布局.最后构造树时使用setRenderer和setEditor就可以了,需要注意的是如果仅仅是呈现,设置Renderer就可以了,如果除了呈现还有操作的话,必须都要设置.
先看我们自己的TreeNode类, 继承于DefaultMutableTreeNode,
- 
/**
- 
- 
* the tree node that I rewrite it.
- 
- 
*/
- 
- 
publicclass MyTreeNode extends DefaultMutableTreeNode {
复制代码
我们添加了属性：
- 
    /** is select or not. */
- 
- 
    privatebooleanisSelected = false;
复制代码
节点的选择状态
- 
    /** is enable. */
- 
- 
    privatebooleanenabled = false;
复制代码
节点使用与否
- 
    /** is visible or not. */
- 
- 
    privatebooleanisVisible = false;
复制代码
节点可见与否
- 
    /** it's icon. */
- 
- 
    private Icon icon = null;
- 
- 
    /** icon name. */
- 
- 
    private String iconName = null;
复制代码
节点图片和图片名字
然后是构造函数,我们初始化属性,或使用默认的:
- 
public MyTreeNode() {
- 
- 
    this(null, true, false, true, true, null);
- 
- 
}
- 
- 
public MyTreeNode(Object userObject, boolean allowsChildren,
- 
- 
    boolean isSelected, boolean enabled, boolean isVisible, Icon icon) {
- 
- 
    super(userObject, allowsChildren);
- 
- 
    this.isSelected = isSelected;
- 
- 
    this.enabled = enabled;
- 
- 
    this.isVisible = isVisible;
- 
- 
    this.icon = icon;
- 
- 
      setSelectionMode(TreeSelectionModel.DISCONTIGUOUS_TREE_SELECTI
- 
- 
ON);
- 
- 
}
复制代码
然后就是一些设置和取得属性的方法,这个类就相当于一个Bean.
然后就是Rnederer了,我们这里继承于DefaultTreeCellRenderer
- 
publicclass MyTreeRenderer extends DefaultTreeCellRenderer {
复制代码
然后复写它的getTreeCellRendererComponent方法:
- 
@Override
- 
- 
public Component getTreeCellRendererComponent(JTree tree, Object value, boolean sel, boolean expanded, boolean leaf, int row,
- 
- 
    boolean hasFocus) {
复制代码
然后设置节点的属性:
首先是显示文本:
- 
String stringValue = tree.convertValueToText(value, sel, expanded, leaf, row, hasFocus);
- 
- 
setText(stringValue);
复制代码
然后是可用与否:
- 
      boolean nodeIsEnabled = ((MyTreeNode) value).isEnabled();
- 
- 
      boolean isEnabled = (treeIsEnabled && nodeIsEnabled);
- 
- 
      setEnabled(isEnabled);
复制代码
然后是是图片
- 
Icon icon = ((MyTreeNode) value).getIcon();
- 
- 
setIcon(icon);
复制代码
还有一些基本的属性,比如背景色等:
- 
setForeground(getTextSelectionColor());
复制代码
再来需要看的类就是Editor了,它继承于DefaultTreeCellEditor:
- 
publicclass MyCellEditor extends DefaultTreeCellEditor {
复制代码
因为我们使用的是JLabel作为显示控件,所以我们的主要实现是布局处理,对于事件可以不需要考虑.我们复习了DefaultTreeCellEditor的默认布局：
- 
    /**
- 
- 
    * Container responsible for placing the editingComponent.
- 
- 
    */
- 
- 
    privateclass MyEditorContainer extends
- 
- 
          DefaultTreeCellEditor.EditorContainer {
复制代码
复写它的doLayout方法：
- 
@Override
- 
- 
publicvoid doLayout() {
复制代码
取得节点的边缘和大小:
- 
r = tree.getBounds(r);
- 
- 
eSize.width = r.width - (offset * n);
- 
- 
editingComponent.setLocation(offset, 0);
复制代码
设置节点的位置和大小:
- 
setSize(new Dimension(eSize.width + offset, cSize.height));
复制代码
最后就是使用了,很简单和以前一样,直接使用JTree的设置方法就可以了:
- 
      MyTreeRenderer renderer = new MyTreeRenderer();
- 
- 
      tree.setCellRenderer(renderer);
- 
- 
      tree.setEditable(true);
- 
- 
      tree.setCellEditor(new MyCellEditor(tree,
- 
- 
          (DefaultTreeCellRenderer) tree.getCellRenderer()));
复制代码
这次我们用树实现一个比较综合的例子,做一个类似Windows的资源管理器,先看Windows的,如下图：
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x1.jpg)
接着就是我们的实现了,路是一步一步走的,先看最基础的实现,这个例子是我在网上看到的,虽然简单,起码是一种思路:
效果如下图:
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x22.jpg)
先说说这个实现的思路吧,首先创建树,再创建一个根节点,然后取得盘符,把盘符放置在根节点下面,然后增加树的监听,当树的节点展开时,取得当前节点下的所有文件和文件夹,加入到节点下,刷新树,展开到指定位置就可以了.
先是创建根节点的：
**public** DefaultMutableTreeNode createRootNode(){ 
      File dir = **new** File("."); 
      DefaultMutableTreeNode rootNode = **new** DefaultMutableTreeNode(ROOT_NAME); 
**for**(**int** i
 = 0; i < dir.*listRoots*().length; i++){ 
**if**(dir.*listRoots*()[i].isDirectory()){ 
              String rootPath = dir.*listRoots*()[i].getPath(); 
**this**.treeNode = **new** DefaultMutableTreeNode(rootPath); 
              rootNode.add(**this**.treeNode); 
**this**.treeNode = **null**; 
          } 
      } 
**return** rootNode; 
    }
然后是增加监听,这里我们监听展开事件:
**this**.tree.addTreeExpansionListener(this); 
然后是处理事件：
@Override
**publicvoid** treeExpanded(TreeExpansionEvent event) { 
先取得选择的节点对象:
**this**.selectNode
=(DefaultMutableTreeNode)event.getPath().getLastPathComponent();
然后取得节点的绝对路径:
    String path = event.getPath().toString(); 
然后根据路径在节点下添加子节点:
**publicvoid** addTreeNode(DefaultMutableTreeNode
 node, File dir) {
**if** (node == **null** ||
 dir == **null**) {
**return**;
       }
**if** (!dir.isDirectory()) {
**return**;
       }
**if** (!node.isRoot()) {
// get all files in node
           File file[] = dir.listFiles();
**for** (**int** i
 = 0; i < file.length; i++) {
// hidden is not show
**if** (file[i].isDirectory() && !file[i].isHidden()) {
// create node
**this**.treeNode = **new** DefaultMutableTreeNode(dir.list()[i]);
// add to tree
                  ((DefaultTreeModel) **this**.jt.getModel()).insertNodeInto(
treeNode, node, node.getChildCount());
**this**.treeNode = **null**;
              }
           }
       }
    }
同样的收起事件也要处理：
@Override
**publicvoid** treeCollapsed(TreeExpansionEvent event) { 
最后把树放置在JScrollPane上就可以了.
**this**.jscroolpane.setViewportView(**this**.tree); 
这样一个简单的资源管理树就完成了,下面我们说说它的问题:
①图片和外观和Windows相差太大
这个我们可以通过设置L&F和通过前面写的Renderer那样设置新的图片解决,不是大问题.
②文件夹里文件多时展开会很慢,会导致界面假死
这个我们可以自己写一个缓加载的TreeNode,让它继承于DefaultMutableTreeNode,在它里面定义加载标示,然后使用SwingWorker或者多线程方式使Tree平稳加载,虽然麻烦,但是也可以解决.
③Tree点击假死时,用户会以为出现问题,胡乱点击会加载多个事件
这个问题其实是Swing事件机制的问题,其实是没办法解决的,因为总会存在耗时的操作的,不等待是不可能的.但我们可以做更好的用户体验来避免这个问题,这里我想到的解决办法是在Tree上绘制一层GlassPane,屏蔽所有事件,提示用户,等加载完成后,取消GlassPane界面.
④只有我的电脑的基本文件,没有网上邻居之类的
这个问题很难解决,涉及到网上邻居就存在网络的问题了,还需要网络连接和扫描,开始我的思路是使用Apache的commons-client做,后来发现有人给出了更好的办法,使用Java的JFileChooser类,Java已经实现了很多我们需要实现的.
⑤取得的资源管理树的子目录是乱序的
这个很好解决,使我们的TreeNode实现Comparable接口就可以了.
为了解决这五个问题我们做的改进版:
首先我们解决问题一,看看我们的代码:
节点的图片的样式问题我们可以设置Renderer,又因为这些图片可以在JFileChooser的UI中取得,我们先参照JFileChooser的UI做一个FileView类:
// ***********************
// * FileView operations *
// ***********************
**protectedclass** BasicFileView **extends**FileView {
复写它的方法:
@Override
**public** String getName(File f)
 {
// Note: Returns display name rather than file name
           String fileName = **null**;
**if** (f != **null**)
 {
              fileName = chooser.getFileSystemView().getSystemDisplayName(f);
           }
**return** fileName;
       }
这个是显示名字.
@Override
**public** String getDescription(File
 f) {
**return** f.getName();
       }
这个是描述
@Override
**public** String getTypeDescription(File
 f) {
           String type = chooser.getFileSystemView().getSystemTypeDescription(
                  f);
**if** (type == **null**)
 {
**if** (f.isDirectory()) {
                  type = directoryDescriptionText;
              } **else** {
                  type = fileDescriptionText;
              }
           }
**return** type;
       }
这个是文件类别
@Override
**public** Icon getIcon(File f) {
这个是图片表示.
这样我们构建这个FileView之后我们需要的图片和名字就都可以取得了.
然后是我们的Renderer了：
**privateclass** FileSystemTreeRenderer **extends** DefaultTreeCellRenderer
 {
复写它的方法,设置我们从FileView取得图片和名字：
@Override
**public** Component getTreeCellRendererComponent(JTree tree, Object value,** boolean** sel, **boolean**expanded, **boolean** leaf, **int** row,
**boolean** hasFocus) {
     setText(getFileView(chooser).getName(node.getFile()));
     setIcon(getFileView(chooser).getIcon(node.getFile()));
然后设置到树上：
tree.setCellRenderer(**new** FileSystemTreeRenderer());
看看效果:
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x3.jpg)
是不是和Windows的很接近了,设置L&F,如下图:
UIManager.*setLookAndFeel*(UIManager.*getSystemLookAndFeelClassName*());
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x4.jpg)
然后解决问题二,我们不能用树的原始节点了,用我们自己构造的,继承于它：
**publicabstractclass** LazyMutableTreeNode **extends** DefaultMutableTreeNode
 {
增加一个属性：
/** is node load. */
**privateboolean**loaded = **false**;
提供一个虚方法给子类实现:
**protectedabstractvoid** loadChildren();
然后是我们的实现:
**privateclass** FileTreeNode **extends** LazyMutableTreeNode
 {
复写它的方法,非load不允许加载:
@Override
**publicboolean** isLeaf()
 {
**if** (!isLoaded()) {
**returnfalse**;
           } **else** {
**returnsuper**.isLeaf();
           }
       }
还有它的现实名字：
@Override
**public** String toString() {
**return**chooser.getFileSystemView().getSystemDisplayName(
                  (File) getUserObject());
        }
实现虚方法：
@Override
**protectedvoid** loadChildren()
 {
           FileTreeNode[] nodes = getChildren();
**for** (**int** i
 = 0, c = nodes.length; i < c; i++) {
              add(nodes[i]);
           }
       }
这样问题二就解决了,同时也可以在这里解决我们的问题五,使我们的TreeNode实现Comparable接口:
**privateclass** FileTreeNode **extends** LazyMutableTreeNode **implements**
           Comparable<Object> {
然后实现方法：
@Override
**publicint** compareTo(Object
 o) {
**if** (!(o **instanceof** FileTreeNode))
 {
**return** 1;
           }
**return** getFile().compareTo(((FileTreeNode)
 o).getFile());
       }
最后在我们使用时：
// sort directories, FileTreeNode implements Comparable
FileTreeNode[] result = (FileTreeNode[]) nodes
              .toArray(**new**FileTreeNode[0]);
       Arrays.*sort*(result);
nodes.add(**new** FileTreeNode(result[i]));
这样我们加入的节点文件夹就都是排序的了.
然后我们解决问题四,三比较麻烦留在最后:
构建这个组件时,我们先构建JFileChooser
JFileChooser chooser = **new** JFileChooser();
增加监听：
**protectedvoid** installListeners()
 {
tree.addTreeSelectionListener(**new** SelectionListener());
chooser.getActionMap().put("refreshTree", **new** UpdateAction());
chooser.getInputMap(JComponent.*WHEN_ANCESTOR_OF_FOCUSED_COMPONENT*).put(
              KeyStroke.*getKeyStroke*("F5"), "refreshTree");
chooser.addPropertyChangeListener(**new** ChangeListener());
    }
在监听中展开树时,使用JFileChooser的方法:
/**
     * tree node select change.
     */
**privateclass** SelectionListener **implements** TreeSelectionListener
 {
@Override
**publicvoid** valueChanged(TreeSelectionEvent
 e) {
           getApproveSelectionAction()
                  .setEnabled(tree.getSelectionCount()
 > 0);
           setSelectedFiles();
// the current directory is the one currently selected
           TreePath currentDirectoryPath = tree.getSelectionPath();
**if** (currentDirectoryPath != **null**)
 {
              File currentDirectory = ((FileTreeNode) currentDirectoryPath
                     .getLastPathComponent()).getFile();
chooser.setCurrentDirectory(currentDirectory);
           }
       }
    }
这样我们所有的目录结构就不需要自己去循环构建了,使用JFileChooser为我们提供好的就可以了,如下图,网上邻居也有了,问题四完成了：
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x5.jpg)
最后我们来解决问题三,为什么会假死,是因为文件夹多或者网速慢导致的,解决办法当然是多线程,但是多线程在Swing里容易出现线程不安全,因为它不在ADT上,这里我们使用SwingWorker,监听树的展开事件:
tree.addTreeExpansionListener(**new** TreeExpansion());
处理它：
**privateclass**TreeExpansion**implements** TreeExpansionListener
 {
@Override
**publicvoid** treeCollapsed(TreeExpansionEvent
 event) {
       }
@Override
**publicvoid** treeExpanded(TreeExpansionEvent
 event) {
// ensure children gets expanded later
**if** (event.getPath() != **null**)
 {
           Object lastElement = event.getPath().getLastPathComponent();
**if** (lastElement **instanceof** FileTreeNode
 && useNodeQueue)
**if** (((FileTreeNode)
 lastElement).isLoaded()) {
慢主要是在这里的处理,我们把它放在SwingWorker里面:
**new** WorkerQueue(node, tree, glassPane).execute();
然后看这个类:
**privatestaticfinalclass** WorkerQueue **extends**
           SwingWorker<Void, FileTreeNode> {
复写它的方法,处理我们的TreeNode添加事件:
@Override
**protected** Void doInBackground() **throws** Exception
 {
glassPanel.setVisible(**true**);
**for** (Enumeration<?> e = node.children();
 e.hasMoreElements();) {
           publish((FileTreeNode) e.nextElement());
       }
**returnnull**;
    }
@Override
**protectedvoid** process(List<FileTreeNode>
 chunks) {
**for** (FileTreeNode fileTreeNode
 : chunks) {
           fileTreeNode.getChildCount();
       }
    }
@Override
**protectedvoid** done()
 {
glassPanel.setVisible(**false**);
tree.repaint();
    }
然后是处理我们在展开节点时屏蔽所有的鼠标点击并给以用户提示,这里我们自己绘制一个Component,把它设置为GlassPane,屏蔽所有事件:
/**
 */
**publicclass** GlassPane **extends** JComponent
 {
屏蔽所有事件,只能获得焦点：
// blocks all user input
    addMouseListener(**new** MouseAdapter()
 {
    });
    addMouseMotionListener(**new** MouseMotionAdapter()
 {
    });
    addKeyListener(**new** KeyAdapter() {
    });
    setFocusTraversalKeysEnabled(**false**);
    addComponentListener(**new** ComponentAdapter()
 {
**publicvoid** componentShown(ComponentEvent
 evt) {
           requestFocusInWindow();
       }
    });
然后是绘制：
@Override
**protectedvoid** paintComponent(Graphics
 g) {
先绘制整体背景：
// gets the current clipping area
       Rectangle clip = g.getClipBounds();
// sets a 65% translucent composite
       AlphaComposite alpha = AlphaComposite.*SrcOver*.derive(0.65f);
       Composite composite = g2.getComposite();
       g2.setComposite(alpha);
// fills the background
       g2.setColor(getBackground());
       g2.fillRect(clip.x, clip.y,
 clip.width, clip.height);
       g2.setComposite(composite);
然后绘制一张提示图片,本来想绘制一个滚动的等待图标![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x6.jpg)的,实在是没心情写了,随便Google了张图片放上去了.
**if** (image == **null**)
 {
**try** {
image = ImageIO.*read*(getClass().getResource("wait2.jpg"));
       } **catch** (IOException ex) {
           ex.printStackTrace();
       }
    }
    g.drawImage(image, getWidth() / 2 - 40, getHeight()
 / 2
       - 80, 120, 120, **null**);
通过设置画面的GlassPane就可以了
    Component glassPane = **new** GlassPane();
    frame.getRootPane().setGlassPane(glassPane);
最终效果如下图:
![](http://www.blogjava.net/images/blogjava_net/zeyuphoenix/x7.jpg)
到此为止,关于树的操作基本就完成了,下面再开个专题讲下有CheckBox的JTree,至于JTree的拖拽,因为和其它Component基本是一致的,就留在以后拖拽专题一起写了,总之,JTree还是不算复杂的一个组件.


