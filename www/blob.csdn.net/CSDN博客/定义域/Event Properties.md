# Event Properties - 定义域 - CSDN博客
2016年02月22日 12:18:41[定义域](https://me.csdn.net/u010278882)阅读数：259标签：[事件](https://so.csdn.net/so/search/s.do?q=事件&t=blog)
个人分类：[笔记																[片段](https://blog.csdn.net/u010278882/article/category/5886909)](https://blog.csdn.net/u010278882/article/category/6086647)
# Event Properties
> 
FROM: [http://quirksmode.org/js/events_properties.html](http://quirksmode.org/js/events_properties.html)
  AUTHOR: Peter-Paul Koch
这里给出一些读取事件属性的示例。在访问事件属性方面存在非常严重的浏览器兼容问题。
只要我们想读取有关事件的信息，我们就会被淹没在大量的属性中，并且其中的大部分在大多数浏览器中并不能够正确运作。查看[事件兼容性表格](http://quirksmode.org/js/events_compinfo.html) 获取事件属性兼容性的概览，要查看详细信息，参考[W3C DOM 兼容性 - 事件](http://quirksmode.org/dom/w3c_events.html)。
我不会给出一份按字母排序的属性列表，因为那不会对你理解这些属性起到任何帮助 —— 因为那通常很容易让人感到困惑。取而代之，我编写了五个脚本来询问浏览器五个问题。
- 什么是事件类型?
- 哪个HTML元素是事件目标?
- 事件发生阶段，哪一个键盘的按键被按下?
- 事件发生阶段，哪一个鼠标按钮被按下?
- 事件发生阶段，鼠标位置在哪里?
请注意在下面这些脚本只能怪，我进行了严格的对象检测。在我使用这些对象和属性前，我首先创建了浏览器兼容的事件访问，然后检查每个属性是否得到支持。
## 什么是事件类型?
这是唯一一个有着跨浏览器答案的问题：使用 type 属性读取事件类型。
```java
function doSomething(e) {
    if(!e) var e = window.event;
    alert(e.type);
}
```
## 哪个HTML元素是事件目标?
W3C/Netscape 说是 target，微软说不，应该是 srcElement。这两个属性都返回发生事件的 HTML 元素。
```java
function doSomething(e) {
    var targ;
    if(!e) var e = window.event;
    if(e.target) targ = e.target;
    else if (e.srcElement) targ = e.srcElement;
    if(targ.nodeType == 3) // 处理 Safari bug
        targ = targ.parentNode;
}
```
最后的两行代码仅用于处理 Safari 浏览器。如果发生事件的元素包含文本，那么就是这个文本节点作为事件目标而不死元素。因此我们需要检测节点是的节点类型是不是 3，即文本节点。如果是文本节点的话，我们我转而获取其父节点，其父节点是 HTML 元素。
无论事件是通过捕获还是冒泡传递的，target/srcElement 总是发生事件的元素。
### 其他目标
还有一些其他的事件目标属性。我会在 [事件顺序](http://quirksmode.org/js/events_order.html) 那一节说明 currentTarget 属性，在[鼠标事件](http://quirksmode.org/js/events_mouse.html) 那一节说明 relatedTarget, fromElement 和 toElement 属性。
## 按下的是哪一个键?
这个问题的答案非常简单，首先通过 keyCode 属性读取按键的代码(a=65)。
如果需要获取用户按下的键的键值，可以通过 String.fromCharCode(keyCode) 方法来获取键值。
```java
function doSomething(e) {
    var code;
    if(!e) var e = window.event;
    if(e.keyCode) code = e.keyCode;
    else if(e.which) code = e.which;
    var character = String.fromCharCode(code);
    alert('Character was' + character);
}
```
此外，还有一些麻烦让键盘事件非常难以使用。比如，只要用户一直按着按键，keypress 事件就会持续产生。并且，在大多数浏览中，只要按键是按下的，keydown 事件就会一直产生。我并不确定这是一个好主意，但事实就是如此。
## 点下的是哪一个鼠标按钮?
有两种属性用于寻找鼠标的哪一个按钮被按下：which 和 button。请注意这些属性在 click 事件中不能正常工作。为了安全的检测鼠标按钮，你得使用 mousedown 和 mouseup 事件。
which 是老版的 Netscape 的属性。左键的值为1，中键（也就是鼠标滚轮）值为2，右键值为3。但是，这个属性的支持度很低（事实上，该属性也被用于键盘按键检测）。
再看看 button 属性吧，它的识别非常混乱。根据 W3C 的定义，值为
- 左键 - 0
- 中键 - 1
- 右键 - 2
根据微软的定义
- 左键 - 1
- 中键 - 4
- 右键 - 2
很显然，微软的定义比W3C的要好。0 应该表示“没有按钮按下”，用于其他值都是不符合逻辑的。
此外，只有微软定义的按钮值可以被组合使用，5 表示 “左键和中键”。W3C认为这种组合是理论不可行的：你永远也不可能知道左键是否被按下。
在我看来，W3C在定义 button 上犯了一些严重的错误。
### 右击
幸运的是，通常你只是想知道右键是否被按下。由于W3C和微软在这一点上达成一致，让右键按下是的键值为 2，这样你就可以检查右键的点击。
```java
function doSomething(e) {
    var rightclick;
    if(!e) var e = window.event;
    if(e.which) rightclick = (e.which == 3);
    else if(e.button) rightclick = (e.button == 2);
    alert('Rightclick: ' + rightclick); // true or false 
}
```
请注意，尽管 Mac 仅有一个鼠标按钮，但是 Mozilla 定义了一个 Ctrl + Click 时的 button 值为2，因为 Ctrl + Click 在 Mac 上也会导致上下文菜单。iCab 现在还不支持鼠标按钮属性，所以你不能检查 Opera 的右击。
## 鼠标位置
对于鼠标位置而言，情况就更加恶劣啦。尽管存在多于6对鼠标坐标属性对，但是没有一对是跨浏览器的。
这六对鼠标坐标属性对是（查看[事件兼容性表格](http://quirksmode.org/js/events_compinfo.html)或[W3C DOM 兼容性 - 事件](http://quirksmode.org/dom/w3c_events.html#mousepos)查看详细的兼容性情况）：
- clientX, clientY
- layerX, layerY
- offsetX, offsetX
- pageX, pageY
- screenX, screenY
- x, y
W3C在这个问题上的描述是含糊不清的。
screenX 和 screenY 属性是唯一一对跨浏览器的兼容的。他们给出鼠标相对整个屏幕的相对位置。很不幸，这些信息基本上没有什么用（因为我们想知道鼠标在浏览器文档上的位置）：因为除了你想在鼠标位置放置另一个窗口，否则你不会想要知道鼠标相对屏幕窗口的位置。
另外三对属性并不重要。查看[W3C DOM 兼容性 - 事件](http://quirksmode.org/dom/w3c_events.html#mousepos)页面的描述。
### 正确的脚本
这是用于检测鼠标位置的正确的脚本：
```java
function doSomething(e) {
    var posx = 0;
    var posy = 0;
    if(!e) var e = window.event;
    if(e.pageX || e.pageY) {
        posx = e.pageX;
        posy = e.pageY;
    } else if(e.clientX || e.clientY) {
        posx = e.clientX + document.body.scrollLeft
                + document.documentElement.scrollLeft;
        posx = e.clientY + document.body.scrollTop
                + docuemnt.documentElement.scrollTop;
    }
    // posx 和 posy 包含鼠标相对文档的位置
    // 现在可以利用这些位置信息进行处理啦
}
```
