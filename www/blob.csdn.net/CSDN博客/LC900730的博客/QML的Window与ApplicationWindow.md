# QML的Window与ApplicationWindow - LC900730的博客 - CSDN博客
2017年08月31日 19:14:07[lc900730](https://me.csdn.net/LC900730)阅读数：2438
### Qt Quick中ApplicationWindow与Window
ApplicationWindow需要导入QtQuick.Controls  
Window需要导入QtQuick.Window 。 
默认不可见，需要设置visible:true才可见。 
主要区别就是ApplicationWindow提供了简单的方式创建程序窗口，因为其有属性menuBar、toolBar、Tabview等属性，可以方便快速创建一个丰富的窗口。
```
ApplicationWindow {
      id: window
      visible: true
      menuBar: MenuBar {
          Menu { MenuItem {...} }
          Menu { MenuItem {...} }
      }
      toolBar: ToolBar {
          RowLayout {
              anchors.fill: parent
              ToolButton {...}
          }
      }
      TabView {
          id: myContent
          anchors.fill: parent
          ...
      }
  }
```
