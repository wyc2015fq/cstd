# Qt图形视图框架（一） - fanyun的博客 - CSDN博客
2016年10月30日 20:49:19[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1726
http://blog.csdn.net/goodlixueyong/article/details/5935370
优点：处理多个图元，单击，拖动，选择图元
架构：一个场景，多个图元位于其中，通过视图显示
主要应用: 绘图软件，显示地图软件
当使用没有变换的视图观察场景时，场景中的一个单元对应屏幕上的一个像素
图元坐标通常以图元中心为原点，X轴正方向为向右，Y轴正方向为向下
场景坐标的原点在场景中心，X轴正方向为向右，Y轴正方向为向下
视图坐标以左上角为原点，X轴正方向为向右，Y轴正方向为向下
所有的鼠标事件最开始都是使用视图坐标
场景：图元的容器
1.提供管理很多图元的接口
2.传播事件到图元中
3.管理图元状态，例如选择和焦点处理
4.提供非转换的绘制功能，主要用于打印
QGraphicsScene scene;
QGraphicsRectItem *rect = scene.addRect(QRectF(0, 0, 100, 100)); // 添加图元
QGraphicsItem *item = scene.itemAt(50, 50); // 查询图元
// item == rect；
通过QGraphicsScene::setSelectionArea()可以选择场景的任一个图元，
QGraphicsScene::setSelectedItems()返回被选择的图元
设置焦点图元QGraphicsScene::setFocusItem(), setFocus(),
QGraphicsScene::focusItem(), 返回焦点图元
视图：
一个可视的子部件，可视化场景的内容
多个视图可以显示同一个场景
坐标转换：QGraphicsView::mapToScene(), QGraphicsView::mapFromScene()
图元：
支持鼠标事件，滚轮事件，上下文菜单事件
支持键盘输入焦点，按键事件
支持拖放
支持分组
冲突探测
提供坐标转换，图元与场景，图元与图元之间
利用QGraphicsItem::shape()和QGraphicsItem::collidesWith()
实现冲突探测，这2个函数都是虚函数
相关类：QGraphicsScene, QGraphicsItem, QGraphicsView
QGraphicsItem子类：
QGraphicsEllipseItem     provides an ellipse item
QGraphicsLineItem        provides a line item
QGraphicsPathItem        provides an arbitrary path item
QGraphicsPixmapItem      provides a pixmap item
QGraphicsPolygonItem     provides a polygon item
QGraphicsRectItem        provides a rectangular item
QGraphicsSimpleTextItem  provides a simple text label item
QGraphicsTextItem        provides an advanced text browser item
QGraphicsSvgItem         provides a SVG file item
QGraphicsScene:
拥有多个图元，包含三层：背景层，图元层，前景层
背景层和前景层可以使用QBrush绘制，也可以使用drawBackground(),drawForeground()实现
如果使用图片作为背景，可以用texture QBrush(pixmap)实现
前景层brush可以使用半透明的白色实现褪色效果，或者使用交叉模式实现网格重叠
场景可以告诉我们，哪些图元发生冲突，哪些图元被选择，哪些图元位于一个特定的点或者区域
每个图元可以是：1.顶级图元，场景是它的父亲；2.孩子，它的父亲是另一个图元，任何作用于父图元的转换
都将自动应用于它的孩子
2种分组方式：1.一个图元成为另一个图元的孩子； 2.使用QGraphicsItemGroup。使用分组，可以使位于同一个
组的所有图元的操作都相同
QGraphicsView:
是一个Widget，用于显示一个场景，提供滚动条功能和转换功能，可以缩放和旋转场景。
默认使用内建的2D画图引擎，可以使用OpenGL：在构造后，调用setViewport()
坐标系统：
使用3种坐标系统：viewport, scene, item
viewport: 位于QGraphicsView内部
scene: 逻辑坐标用于定位顶级图元
item: 与图元相关，以图元的(0,0)为中心，移动图元时，它的坐标不会改变
实践中，主要关注场景坐标（定位顶级图元）和图元坐标（定位子图元和绘制图元）
在图元自己的坐标系统里面绘图意味着我们不用担心它在场景中的位置和应用于它的坐标转换
Demo：
// 主要特点：
// 上下文菜单, 右键菜单
// copy->paste方法
//diagram.pro
TEMPLATE      = app
HEADERS       = diagramwindow.h /
link.h /
node.h /
propertiesdialog.h
SOURCES       = diagramwindow.cpp /
link.cpp /
main.cpp /
node.cpp /
propertiesdialog.cpp
FORMS         = propertiesdialog.ui
RESOURCES     = resources.qrc
//link.h
#ifndef LINK_H
#define LINK_H
#include <QGraphicsLineItem>
class Node;
class Link : public QGraphicsLineItem   // 如果使用信号和槽，采用多继承public QObject
{
public:
Link(Node *fromNode, Node *toNode);
~Link();
Node *fromNode() const;
Node *toNode() const;
void setColor(const QColor &color);
QColor color() const;
void trackNodes();   // 节点移动时，跟踪节点
private:
Node *myFromNode;   // 连线的2个节点
Node *myToNode;
};
#endif
//link.cpp
#include <QtGui>
#include "link.h"
#include "node.h"
Link::Link(Node *fromNode, Node *toNode)
{
myFromNode = fromNode;
myToNode = toNode;
myFromNode->addLink(this); // 节点增加连线，每个节点有任意多个连线
myToNode->addLink(this);
setFlags(QGraphicsItem::ItemIsSelectable);  // 连线可以被选择，然后删除
setZValue(-1);  // 在场景中显示的前后层次，因为连线是两个节点的中心，-1表示位于最后面，
// 节点覆盖了部分连线
setColor(Qt::darkRed);  // 设置线的颜色
trackNodes();
}
Link::~Link()
{
myFromNode->removeLink(this);  // 删除连线时，将删除它在节点中的记录
myToNode->removeLink(this);
}
Node *Link::fromNode() const
{
return myFromNode;
}
Node *Link::toNode() const
{
return myToNode;
}
void Link::setColor(const QColor &color)
{
setPen(QPen(color, 1.0));
}
QColor Link::color() const
{
return pen().color();
}
void Link::trackNodes()
{
// pos()返回节点在场景中或者父图元中的位置
setLine(QLineF(myFromNode->pos(), myToNode->pos()));
}
//node.h
#ifndef NODE_H
#define NODE_H
#include <QApplication>
#include <QColor>
#include <QGraphicsItem>
#include <QSet>
class Link;
class Node : public QGraphicsItem
{
Q_DECLARE_TR_FUNCTIONS(Node)   // 在此类中增加tr()功能，直接使用，而不需要QObject::tr()了
public:
Node();
~Node();
void setText(const QString &text);
QString text() const;
void setTextColor(const QColor &color);
QColor textColor() const;
void setOutlineColor(const QColor &color);
QColor outlineColor() const;
void setBackgroundColor(const QColor &color);
QColor backgroundColor() const;
void addLink(Link *link);
void removeLink(Link *link);
QRectF boundingRect() const;    // 重新实现，决定一个图元是否需要绘制，必须的
QPainterPath shape() const;     // 重新实现，返回图元的精确形状，
// 决定一个点是否在图元内，或者2个图元是否发生冲突
void paint(QPainter *painter,   // 重新实现，画图， 必须的
const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event); // 双击事件，修改节点的文本
QVariant itemChange(GraphicsItemChange change,   // 重新实现，图元变化时，相关的连线发生变化
const QVariant &value);      // 没有使用mouseMoveEvent(), 
// 是因为程序可以改变节点位置
private:
QRectF outlineRect() const;
int roundness(double size) const;
QSet<Link *> myLinks;
QString myText;
QColor myTextColor;
QColor myBackgroundColor;
QColor myOutlineColor;
};
#endif
//link.cpp
#include <QtGui>
#include "link.h"
#include "node.h"
Node::Node()
{
myTextColor = Qt::darkGreen;
myOutlineColor = Qt::darkBlue;
myBackgroundColor = Qt::white;
setFlags(ItemIsMovable | ItemIsSelectable); // 节点可以移动，被选择
}
Node::~Node()
{
foreach (Link *link, myLinks)  // 删除所有的连线，防止边界效应，不使用aDeleteAll()
delete link;
}
void Node::setText(const QString &text)
{
prepareGeometryChange();  // 改变节点内的文本时，矩形可能会发生变化
myText = text;
update();
}
QString Node::text() const
{
return myText;
}
void Node::setTextColor(const QColor &color)
{
myTextColor = color;
update();
}
QColor Node::textColor() const
{
return myTextColor;
}
void Node::setOutlineColor(const QColor &color)
{
myOutlineColor = color;
update();
}
QColor Node::outlineColor() const
{
return myOutlineColor;
}
void Node::setBackgroundColor(const QColor &color)
{
myBackgroundColor = color;
update();
}
QColor Node::backgroundColor() const
{
return myBackgroundColor;
}
void Node::addLink(Link *link)
{
myLinks.insert(link);   // 增加连线时，记录连线
}
void Node::removeLink(Link *link)
{
myLinks.remove(link);
}
QRectF Node::boundingRect() const  // View决定是否绘制矩形
{
const int Margin = 1;
return outlineRect().adjusted(-Margin, -Margin, +Margin, +Margin);
}
QPainterPath Node::shape() const   // View用于冲突探测
{
QRectF rect = outlineRect();
QPainterPath path;
path.addRoundRect(rect, roundness(rect.width()),
roundness(rect.height()));
return path;
}
// 绘制图元
void Node::paint(QPainter *painter,
const QStyleOptionGraphicsItem *option,
QWidget * /* widget */)
{
QPen pen(myOutlineColor);
if (option->state & QStyle::State_Selected) {  // 图元被选择
pen.setStyle(Qt::DotLine);
pen.setWidth(2);
}
painter->setPen(pen);
painter->setBrush(myBackgroundColor);
QRectF rect = outlineRect();
painter->drawRoundRect(rect, roundness(rect.width()),
roundness(rect.height()));
painter->setPen(myTextColor);
painter->drawText(rect, Qt::AlignCenter, myText);
}
// 双击节点，弹出标准输入对话框
void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
QString text = QInputDialog::getText(event->widget(),
tr("Edit Text"), tr("Enter new text:"),
QLineEdit::Normal, myText);
if (!text.isEmpty())
setText(text);
}
// 拖动节点时，调用此函数
QVariant Node::itemChange(GraphicsItemChange change,
const QVariant &value)
{
if (change == ItemPositionHasChanged) {
foreach (Link *link, myLinks)
link->trackNodes();
}
return QGraphicsItem::itemChange(change, value);
}
QRectF Node::outlineRect() const
{
const int Padding = 8;
QFontMetricsF metrics = qApp->font();
QRectF rect = metrics.boundingRect(myText);
rect.adjust(-Padding, -Padding, +Padding, +Padding);
rect.translate(-rect.center());
return rect;
}
int Node::roundness(double size) const
{
const int Diameter = 12;
return 100 * Diameter / int(size);
}
// diagramwindow.h
#ifndef DIAGRAMWINDOW_H
#define DIAGRAMWINDOW_H
#include <QMainWindow>
#include <QPair>
class QAction;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsView;
class Link;
class Node;
class DiagramWindow : public QMainWindow
{
Q_OBJECT
public:
DiagramWindow();
private slots:
void addNode();
void addLink();
void del();
void cut();
void copy();
void paste();
void bringToFront();
void sendToBack();
void properties();    // 弹出属性设置对话框
void updateActions(); // 更新菜单栏和工具栏的动作，哪些可用，哪些不可用
private:
typedef QPair<Node *, Node *> NodePair;
void createActions();
void createMenus();
void createToolBars();
void setZValue(int z);
void setupNode(Node *node);
Node *selectedNode() const;
Link *selectedLink() const;
NodePair selectedNodePair() const;
QMenu *fileMenu;
QMenu *editMenu;
QToolBar *editToolBar;
QAction *exitAction;
QAction *addNodeAction;
QAction *addLinkAction;
QAction *deleteAction;
QAction *cutAction;
QAction *copyAction;
QAction *pasteAction;
QAction *bringToFrontAction;
QAction *sendToBackAction;
QAction *propertiesAction;
QGraphicsScene *scene;
QGraphicsView *view;
int minZ; // sendToBack(), bringToFront()使用
int maxZ;
int seqNumber; // 唯一标示一个节点的文本
};
#endif
//digramwindow.cpp
#include <QtGui>
#include "diagramwindow.h"
#include "link.h"
#include "node.h"
#include "propertiesdialog.h"
DiagramWindow::DiagramWindow()
{
scene = new QGraphicsScene(0, 0, 600, 500); // 创建场景，起始点为(0,0), 宽600，高500
view = new QGraphicsView;
view->setScene(scene);    // 显示场景
view->setDragMode(QGraphicsView::RubberBandDrag); // 选择多个节点方式：1.按ctrl；2.设置橡皮筋方式
view->setRenderHints(QPainter::Antialiasing
| QPainter::TextAntialiasing);
view->setContextMenuPolicy(Qt::ActionsContextMenu); // 右键菜单
setCentralWidget(view);
minZ = 0;
maxZ = 0;
seqNumber = 0;
createActions();
createMenus();
createToolBars();
connect(scene, SIGNAL(selectionChanged()),
this, SLOT(updateActions()));
setWindowTitle(tr("Diagram"));
updateActions();
}
// 增加一个节点
void DiagramWindow::addNode()
{
Node *node = new Node;
node->setText(tr("Node %1").arg(seqNumber + 1));
setupNode(node);
}
void DiagramWindow::addLink()
{
NodePair nodes = selectedNodePair();
if (nodes == NodePair())
return;
Link *link = new Link(nodes.first, nodes.second);
scene->addItem(link);
}
// 删除选择的图元：首先删除连线，然后删除节点，以防止多次删除同一个连线
void DiagramWindow::del()
{
QList<QGraphicsItem *> items = scene->selectedItems();
QMutableListIterator<QGraphicsItem *> it(items);
while (it.hasNext()) {
Link *link = dynamic_cast<Link *>(it.next());
if (link) {
delete link;
it.remove();
}
}
qDeleteAll(items);
}
// 剪切操作：先复制，后删除
void DiagramWindow::cut()
{
Node *node = selectedNode();
if (!node)
return;
copy();
delete node;
}
// 拷贝操作：值得研究！！！
void DiagramWindow::copy()
{
Node *node = selectedNode();
if (!node)
return;
QString str = QString("Node %1 %2 %3 %4")
.arg(node->textColor().name())
.arg(node->outlineColor().name())
.arg(node->backgroundColor().name())
.arg(node->text());
QApplication::clipboard()->setText(str);
}
void DiagramWindow::paste()
{
QString str = QApplication::clipboard()->text();
QStringList parts = str.split(" ");
if (parts.count() >= 5 && parts.first() == "Node") {
Node *node = new Node;
node->setText(QStringList(parts.mid(4)).join(" ")); // 连接字符串列表
node->setTextColor(QColor(parts[1]));
node->setOutlineColor(QColor(parts[2]));
node->setBackgroundColor(QColor(parts[3]));
setupNode(node);
}
}
void DiagramWindow::bringToFront()
{
++maxZ;
setZValue(maxZ);  // 改变绘图顺序，首先绘制父图元，然后是子图元，根据子图元Z值的大小，
// 值最小，最先绘制，值最大，最后绘制
}
void DiagramWindow::sendToBack()
{
--minZ;
setZValue(minZ);
}
void DiagramWindow::properties()
{
Node *node = selectedNode();
Link *link = selectedLink();
if (node) {
PropertiesDialog dialog(node, this);
dialog.exec();
} else if (link) {
QColor color = QColorDialog::getColor(link->color(), this);
if (color.isValid())
link->setColor(color);
}
}
// 更新动作使能
void DiagramWindow::updateActions()
{
bool hasSelection = !scene->selectedItems().isEmpty();
bool isNode = (selectedNode() != 0);
bool isNodePair = (selectedNodePair() != NodePair());
cutAction->setEnabled(isNode);
copyAction->setEnabled(isNode);
addLinkAction->setEnabled(isNodePair);
deleteAction->setEnabled(hasSelection);
bringToFrontAction->setEnabled(isNode);
sendToBackAction->setEnabled(isNode);
propertiesAction->setEnabled(isNode);
foreach (QAction *action, view->actions())
view->removeAction(action);  // 删除右键菜单
foreach (QAction *action, editMenu->actions()) {
if (action->isEnabled())
view->addAction(action);  // 增加右键菜单
}
}
void DiagramWindow::createActions()
{
exitAction = new QAction(tr("E&xit"), this);
exitAction->setShortcut(tr("Ctrl+Q"));
connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
addNodeAction = new QAction(tr("Add &Node"), this);
addNodeAction->setIcon(QIcon(":/images/node.png"));
addNodeAction->setShortcut(tr("Ctrl+N"));
connect(addNodeAction, SIGNAL(triggered()), this, SLOT(addNode()));
addLinkAction = new QAction(tr("Add &Link"), this);
addLinkAction->setIcon(QIcon(":/images/link.png"));
addLinkAction->setShortcut(tr("Ctrl+L"));
connect(addLinkAction, SIGNAL(triggered()), this, SLOT(addLink()));
deleteAction = new QAction(tr("&Delete"), this);
deleteAction->setIcon(QIcon(":/images/delete.png"));
deleteAction->setShortcut(tr("Del"));
connect(deleteAction, SIGNAL(triggered()), this, SLOT(del()));
cutAction = new QAction(tr("Cu&t"), this);
cutAction->setIcon(QIcon(":/images/cut.png"));
cutAction->setShortcut(tr("Ctrl+X"));
connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
copyAction = new QAction(tr("&Copy"), this);
copyAction->setIcon(QIcon(":/images/copy.png"));
copyAction->setShortcut(tr("Ctrl+C"));
connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
pasteAction = new QAction(tr("&Paste"), this);
pasteAction->setIcon(QIcon(":/images/paste.png"));
pasteAction->setShortcut(tr("Ctrl+V"));
connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
bringToFrontAction = new QAction(tr("Bring to &Front"), this);
bringToFrontAction->setIcon(QIcon(":/images/bringtofront.png"));
connect(bringToFrontAction, SIGNAL(triggered()),
this, SLOT(bringToFront()));
sendToBackAction = new QAction(tr("&Send to Back"), this);
sendToBackAction->setIcon(QIcon(":/images/sendtoback.png"));
connect(sendToBackAction, SIGNAL(triggered()),
this, SLOT(sendToBack()));
propertiesAction = new QAction(tr("P&roperties..."), this);
connect(propertiesAction, SIGNAL(triggered()),
this, SLOT(properties()));
}
void DiagramWindow::createMenus()
{
fileMenu = menuBar()->addMenu(tr("&File"));
fileMenu->addAction(exitAction);
editMenu = menuBar()->addMenu(tr("&Edit"));
editMenu->addAction(addNodeAction);
editMenu->addAction(addLinkAction);
editMenu->addAction(deleteAction);
editMenu->addSeparator();
editMenu->addAction(cutAction);
editMenu->addAction(copyAction);
editMenu->addAction(pasteAction);
editMenu->addSeparator();
editMenu->addAction(bringToFrontAction);
editMenu->addAction(sendToBackAction);
editMenu->addSeparator();
editMenu->addAction(propertiesAction);
}
void DiagramWindow::createToolBars()
{
editToolBar = addToolBar(tr("Edit"));
editToolBar->addAction(addNodeAction);
editToolBar->addAction(addLinkAction);
editToolBar->addAction(deleteAction);
editToolBar->addSeparator();
editToolBar->addAction(cutAction);
editToolBar->addAction(copyAction);
editToolBar->addAction(pasteAction);
editToolBar->addSeparator();
editToolBar->addAction(bringToFrontAction);
editToolBar->addAction(sendToBackAction);
}
void DiagramWindow::setZValue(int z)
{
Node *node = selectedNode();
if (node)
node->setZValue(z);
}
void DiagramWindow::setupNode(Node *node)
{
node->setPos(QPoint(80 + (100 * (seqNumber % 5)),
80 + (50 * ((seqNumber / 5) % 7))));
scene->addItem(node);
++seqNumber;
scene->clearSelection();
node->setSelected(true);
bringToFront();
}
// 返回一个选择的节点
Node *DiagramWindow::selectedNode() const
{
QList<QGraphicsItem *> items = scene->selectedItems(); // 全部选择的节点
if (items.count() == 1) {
return dynamic_cast<Node *>(items.first());
} else {
return 0;
}
}
Link *DiagramWindow::selectedLink() const
{
QList<QGraphicsItem *> items = scene->selectedItems();
if (items.count() == 1) {
return dynamic_cast<Link *>(items.first());
} else {
return 0;
}
}
// 返回选择的节点对
DiagramWindow::NodePair DiagramWindow::selectedNodePair() const
{
QList<QGraphicsItem *> items = scene->selectedItems();
if (items.count() == 2) {
Node *first = dynamic_cast<Node *>(items.first());
Node *second = dynamic_cast<Node *>(items.last());
if (first && second)
return NodePair(first, second);
}
return NodePair();
}
// propertiesdialog.h
#ifndef PROPERTIESDIALOG_H
#define PROPERTIESDIALOG_H
#include "ui_propertiesdialog.h"
class Node;
class PropertiesDialog : public QDialog, private Ui::PropertiesDialog
{
Q_OBJECT
public:
PropertiesDialog(Node *node, QWidget *parent = 0);
private slots:
void on_buttonBox_accepted();    // 快速信号连接
void on_textColorButton_clicked();
void on_outlineColorButton_clicked();
void on_backgroundColorButton_clicked();
private:
void updateColorLabel(QLabel *label, const QColor &color);
void chooseColor(QLabel *label, QColor *color);
Node *node;
QColor textColor;
QColor outlineColor;
QColor backgroundColor;
};
#endif
//propertiesdialog.cpp
#include <QtGui>
#include "node.h"
#include "propertiesdialog.h"
PropertiesDialog::PropertiesDialog(Node *node, QWidget *parent)
: QDialog(parent)
{
setupUi(this);
this->node = node;
xSpinBox->setValue(int(node->x()));
ySpinBox->setValue(int(node->y()));
textLineEdit->setText(node->text());
textColor = node->textColor();
outlineColor = node->outlineColor();
backgroundColor = node->backgroundColor();
updateColorLabel(outlineColorLabel, outlineColor);
updateColorLabel(backgroundColorLabel, backgroundColor);
updateColorLabel(textColorLabel, textColor);
}
void PropertiesDialog::on_buttonBox_accepted()
{
node->setPos(xSpinBox->value(), ySpinBox->value());
node->setText(textLineEdit->text());
node->setOutlineColor(outlineColor);
node->setBackgroundColor(backgroundColor);
node->setTextColor(textColor);
node->update();
QDialog::accept();
}
void PropertiesDialog::on_textColorButton_clicked()
{
chooseColor(textColorLabel, &textColor);
}
void PropertiesDialog::on_outlineColorButton_clicked()
{
chooseColor(outlineColorLabel, &outlineColor);
}
void PropertiesDialog::on_backgroundColorButton_clicked()
{
chooseColor(backgroundColorLabel, &backgroundColor);
}
void PropertiesDialog::updateColorLabel(QLabel *label,
const QColor &color)
{
QPixmap pixmap(16, 16);
pixmap.fill(color);
label->setPixmap(pixmap);
}
void PropertiesDialog::chooseColor(QLabel *label, QColor *color)
{
QColor newColor = QColorDialog::getColor(*color, this);
if (newColor.isValid()) {
*color = newColor;
updateColorLabel(label, *color);
}
}
