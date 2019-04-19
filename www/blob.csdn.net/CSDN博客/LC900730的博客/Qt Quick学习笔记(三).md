# Qt Quick学习笔记(三) - LC900730的博客 - CSDN博客
2017年08月20日 18:39:51[lc900730](https://me.csdn.net/LC900730)阅读数：145
### 第五章
浏览器对于ECMAScript是宿主环境，但不是唯一的宿主环境，其他非浏览器宿主环境如Flash与Director MX中的ActionScript，以及QML。
ECMAScript规定了如下内容:
- 语法
- 类型
- 语句
- 关键字
- 保留字
- 运算符
- 对象
ECMAScript仅仅是一个描述，定义了脚本语言所有属性、方法和对象。其他语言可以实现ECMAScript来作为功能的继承，如JavaScript那样。实现可以被扩展，包含特定于宿主环境的新特性。QML中就引入了信号与槽，还增加了动态属性绑定等非常有特定的功能。
QML有三个核心
- ECMAScript
- Qt对象系统
- Qt Quick标准库
在QML引入的信号，是有安全类型检测的，在定义信号的时候，可以使用特定的类型来定义参数，如signal colorChanged(color cl),这是作为宿主环境的QML与浏览器之类的宿主环境的一个不同之处。
### ECMAScript中的Undefined类型
Undefined类型只要一个值，是undefined。当声明的变量未初始化时，该变量的默认值是undefined。 
var temp； 
当函数没有明确的返回值，返回值也是undefined。
## 宿主环境Qt
QML除了实现ECMAScript规范，还提供了庞大的类库，有QML语音本身提供的，有Qt Quick提供的，还有其他扩展模块提供的(如多媒体、定位),甚至可以通过QML提供的机制自行添加。 
QML提供的宿主对象Qt 
Qt::quit(); 
Qt::argb(); 
Qt::Key_0; 
Qt是QML提供的一个全局宿主对象，整合了常用的属性、方法和枚举类型。
