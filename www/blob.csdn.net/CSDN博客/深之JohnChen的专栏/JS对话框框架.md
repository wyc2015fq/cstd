# JS对话框框架 - 深之JohnChen的专栏 - CSDN博客

2017年03月03日 23:43:14[byxdaz](https://me.csdn.net/byxdaz)阅读数：1694


         基本的对话框窗口是一个定位于视区中的覆盖层，同时通过一个 iframe 与页面内容分隔开（就像 select 元素）。它由一个标题栏和一个内容区域组成，且可以移动，调整尺寸，默认可通过 'x' 图标关闭。           

         常见的js对话框框架有Jquery UI Dialog、artDialog、lhgdialog。

         一：Jquery UI Dialog，对话框，是jQuery UI 非常重要的一个功能。它彻底的代替了JavaScript的alert()、prompt()等方法，也避免了新窗口或页面的繁杂冗余。

               中文帮助文档：[http://www.runoob.com/jqueryui/jqueryui-tutorial.html](http://www.runoob.com/jqueryui/jqueryui-tutorial.html)

               下载地址：[http://jqueryui.com/demos/dialog/](http://jqueryui.com/demos/dialog/)

jQuery UI是jQuery 自带的一个可选ＵＩ库，但是非常可惜，一些关键的组件没有包含进去，如TreeView, DataGrid。可以使用第三方UI库，比如EasyUI。

jQuery UI vs EasyUI: 二者冲突怎么办？

很不幸，jQueryUI和EasyUI的某些组件是命名冲突的，如 Dialog, Tabs等，我们之前的项目主要是基于jQueryUI,　如果使用EasyUI将重写以前的代码，后来我们采取了一种折中的方案：

进入http://jqueryui.com/download 定制jQuery组件，只保留jQueryUI中的Tabs, Dialog. （其他的如DatatimePicker等使用EasyUI重写）；这样我们会得到一个只有40K的jQueryUI文件，我们认为还是可以接受的。

将精简后的jQueryUI加载到EasyUI引用之后，这样EasyUI的Tabs和Dialog就会被复写。

        二：artDialog, 经典的网页对话框组件。
支持普通与 12 方向气泡状对话框
完善的焦点处理，自动焦点附加与回退
支持 ARIA 标准
面向未来：基于 HTML5 Dialog 的 API
支持标准与模态对话框
丰富且友好的编程接口
能自适应内容尺寸

                中文帮助文档：[http://aui.github.io/artDialog/doc/index.html](http://aui.github.io/artDialog/doc/index.html)

                下载地址：[https://github.com/aui/artDialog](https://github.com/aui/artDialog)

        三：lhgdialog,是一个功能强大且兼容面广的对话框组件，它拥有精致的界面与友好的接口。
为大型弹窗定制
本组件主要以iframe方式加载单独页面为主的弹出窗口，由其适用于后台管理和webOS类项目使用，独立的内容页更方便管理，页面的也不易受其它页面的影响，而且内容页可以是静态或动态的任何一种文件。
强大灵活的接口
由于本组件主要是用来制作大型的窗口，页面和窗口间的交互肯定会非常频繁，所以本组件提供了丰富的交互控制接口，可以非常方便的进行页面间的数据的传输。
细致的用户体验
智能无限级跨框架弹出，如果不是在输入状态，它支持Esc快捷键关闭；智能给按钮添加焦点；黄金比例垂直居中；采用九宫格的布局结构，css类钩子丰富，可以定制类似桌面软件般精致的皮肤...
跨平台兼容特性
兼容：IE6+、Firefox、Chrome、Safari、Opera以及iPad等移动设备。并且IE6下也能支持现代浏览器的静止定位(fixed)、覆盖下拉控件、alpha通道png背景。

                中文帮助文档：[http://www.lhgdialog.com/api/](http://www.lhgdialog.com/api/)

                下载地址：[http://code.google.com/p/lhgdialog/downloads/list](http://code.google.com/p/lhgdialog/downloads/list)

