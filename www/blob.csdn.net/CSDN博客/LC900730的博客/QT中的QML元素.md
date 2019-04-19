# QT中的QML元素 - LC900730的博客 - CSDN博客
2017年08月20日 16:09:45[lc900730](https://me.csdn.net/LC900730)阅读数：190
## QML
QML实现并且扩展了ECMAScript，是一种说明性语言，用来描述基于Qt对象系统的用户界面。QML提供了高可读、声明式的 、类CSS的语法，支持结合动态属性绑定的ECMAScript表达式。 
Qt Quick模块是开发QML应用的标准库，提供了使用QML创建用户界面需要的一切东西，包含可视化类型、交互类型、动画、模型和视图、粒子特效与着色器
## 基本QML元素
```
Item：被其他QML元素继承的基本项
Component：导入时封装了QML元素
QtObject：仅容纳一个objectName属性的基本元素
```
## 图形
```
Rectangle：一个矩形元素
Image:屏幕上的一个位图
BorderImage：可以使用图片作为边缘
AnimatedImage:未播放动画而存储的一系列帧
Gradient：定义颜色渐变
GradientStop:为gradient定义一个颜色
SystemPalette：提供访问Qt调色板的接口
```
## 文件处理
Text：向屏幕上插入格式化 的文字 
TextInput：获取用户键盘输入 
TextEdit：显示多行可编辑的格式化文本 
IntValidator:验证整数 
DoubleValidator:验证实数 
RegExpValidator：字符串正则表达式验证器 
FontLoader:按照名称或URL加载字体
## 鼠标和交互区域
```
MouseArea:
Keys:提供一个带有附加属性的组件用于处理键盘输入
FocusScope：传递键盘焦点变化的元素
Flickable：提供实现“flicked”弹性效果的表面组件
Flipable：提供处理flipping(翻转)效果的表面组件
PinchArea：简单的挤压手势处理
```
## 定位器和重复器
Column：垂直排列子元素 
Row：水平排列子元素 
Grid：在网格中定位子元素 
Flow：按坐标定位子元素 
Repeater：使用一个模型创建多个组件
## Transformations
```
Scale:设置item的缩放行为
Rotation：设置item的选择行为
Translate：设置item的移动行为
```
## 状态States
State：定义对象和属性的一系列配置值 
PropertyChanges：描述状态中属性的变化 
StateGroup:包含一系列的状态和状态变换 
StateChangeScript:使脚本绑定到一个状态上 
ParentChange:在状态改变时重定义item的父项 
AnchorChanges:在状态中改变item的锚点
## 动画和过度
## QML与QWidget比较
有3种不同结构的QWidget
```
不能作为父部件的简单部件(QLabel,QCheckBox,QToolButton)
常作为其他部件的父部件(QGroupBox,QStackedWidget,QTabWidget)
由子部件组成的组合部件(QComboBox,QSpinBox,QTabWidget)
```
