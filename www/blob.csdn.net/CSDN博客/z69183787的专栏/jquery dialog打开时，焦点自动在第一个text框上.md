# jquery dialog打开时，焦点自动在第一个text框上 - z69183787的专栏 - CSDN博客
2013年12月26日 19:29:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5863
今天在用jqueryui的日历功能，datepicker时碰到一个问题。输入框在一个弹出的jquery dialog中，每次dialog(“open”)的时候，对话框的焦点自动focus到第一个<input type="text"/>框，正好这个input框绑定了datepicker，就自动把日历弹出来了。
google了一下，还真碰到正好也有这个问题的：
http://stackoverflow.com/questions/1202079/prevent-jquery-ui-dialog-from-setting-focus-to-first-textbox
里面有提到jquery对话框打开时，哪个元素获得焦点的机制：
http://jqueryui.com/upgrade-guide/1.10/#added-ability-to-specify-which-element-to-focus-on-open
在jqueryui 1.10.0之后，对话框打开时，第一个设置了autofocus的元素获得焦点。
在jqueryui 1.10.0之前：tabbable elements within the content area, tabbable elements within the button pane, the close button, and finally the dialog itself as a fallback。即：对话框主体内容中第一个支持tab键的元素；如果没找到，则查找button部分第一个支持tab键的元素；再没找到，则close按钮获得焦点；最后没有的话，整个对话框获得焦点
