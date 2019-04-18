# 实现一个EventTarget类 - weixin_33985507的博客 - CSDN博客
2019年02月15日 15:09:48[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# EventTarget
`EventTarget`是一个由可以接收事件的对象实现的接口，并且可以为它们创建侦听器。
[`Element`](https://developer.mozilla.org/zh-CN/docs/Web/API/Element)，[`document`](https://developer.mozilla.org/zh-CN/docs/Web/API/Document) 和 [`window`](https://developer.mozilla.org/zh-CN/docs/Web/API/Window) 是最常见的事件目标，但是其他对象也可以是事件目标，比如[`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest)，[`AudioNode`](https://developer.mozilla.org/zh-CN/docs/Web/API/AudioNode)，[`AudioContext`](https://developer.mozilla.org/zh-CN/docs/Web/API/AudioContext) 等等。
许多事件目标（包括元素，文档和 window）还支持通过 `on...` 属性和属性设置[事件处理程序](https://developer.mozilla.org/zh-CN/docs/Web/Guide/DOM/Events/Event_handlers)。
## 构造函数[节](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget#%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
- [`EventTarget()`](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget/EventTarget)
创建一个新的 `EventTarget` 对象实例。
## 方法[节](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget#Methods)
- [`EventTarget.addEventListener()`](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget/addEventListener)
在EventTarget上注册特定事件类型的事件处理程序。
- [`EventTarget.removeEventListener()`](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget/removeEventListener)
EventTarget中删除事件侦听器。
- [`EventTarget.dispatchEvent()`](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget/dispatchEvent)
将事件分派到此EventTarget。
### Mozilla chrome 代码的其他方法[节](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget#Mozilla_chrome_%E4%BB%A3%E7%A0%81%E7%9A%84%E5%85%B6%E4%BB%96%E6%96%B9%E6%B3%95)
Mozilla扩展，供JS实现的事件目标使用以 实现 on* 属性。另见 [WebIDL bindings](https://developer.mozilla.org/en-US/docs/Mozilla/WebIDL_bindings) 绑定。
- void **setEventHandler**(DOMString type, EventHandler handler)
- EventHandler **getEventHandler**(DOMString type)
## 示例[节](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget#%E7%A4%BA%E4%BE%8B)
### EventTarget 的简单实现[节](https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget#EventTarget_%E7%9A%84%E7%AE%80%E5%8D%95%E5%AE%9E%E7%8E%B0)
```
function EventTarget() {
    this.listeners = {};
}
Object.assign(EventTarget.prototype, {
    // listeners: null,
    // prefix:"on",
    addEventListener: function (type, callback) {
        if (!(type in this.listeners)) {
            this.listeners[type] = [];
        }
        this.listeners[type].push(callback);
    },
    removeEventListener: function (type, callback) {
        if (type in this.listeners) {
            let stack = this.listeners[type];
            let index = stack.indexOf(callback);
            console.log(index);
            //可能会添加多个
            while (index !== -1) {
                stack.splice(index, 1);
                index = stack.indexOf(callback);
            }
        }
    },
    dispatchEvent: function (event) {
        if (event.type in this.listeners) {
            let stack = this.listeners[event.type];
            event.target = this;
            stack.forEach(callback => {
                callback.call(this,event);
            })
        }
    }
});
let target = new EventTarget();
let remove = (e) => {console.log(e)};
target.addEventListener("test",remove);
target.addEventListener("test",remove);
target.addEventListener("test",(e) => {console.log(e)});
target.removeEventListener("test",remove);
target.dispatchEvent({type:"test"});
```
当单击这个例子中的按钮时， this 和 currentTarget 都等于 document.body，因为事件处理程 序是注册到这个元素上的。然而， target 元素却等于按钮元素，因为它是 click 事件真正的目标。由 于按钮上并没有注册事件处理程序，结果 click 事件就冒泡到了 document.body，在那里事件才得到 了处理。
```
document.body.onclick = function(event){
    alert(event.currentTarget === document.body); //true
    alert(this === document.body); //true
    alert(event.target === document.getElementById("myBtn")); //true
};
```
所有问事件的target和currentTarget区别的问题，都是在变相的问事件委托。
因为子元素将事件委托到父级的时候，event的target指向的还是子元素，而currentTarget指的是父元素
```
<ul>
    <li>w</li>
    <li>m</li>
    <li>x</li>
</ul>
//子元素将事件委托给了父元素
document.querySelector('ul').addEventListener('click',(e) => {
    let text = e.target.innerText || e.target.textContent;
    swicth(text){
        case 'w':{
            console.info('this first li');
            break;
        }
        case 'm':{
            console.info('this second li');
            break;
        }
        case 'x':{
            console.info('this third li');
            break;
        }
        default:{
            new Error('No such Element')
        }
    }
},false)
```
事件模拟
事件创建
参数事件类型字符串UIEvents
document.createEvent(EventTypeString)
- UIEvents：一般化的 UI 事件。 鼠标事件和键盘事件都继承自 UI 事件。 DOM3 级中是 UIEvent；
- MouseEvents：一般化的鼠标事件。 DOM3 级中是 MouseEvent；
- MutationEvents：一般化的 DOM 变动事件。 DOM3 级中是 MutationEvent；
- HTMLEvents：一般化的 HTML 事件。没有对应的 DOM3 级事件（HTML 事件被分散到其他类 别中);
- KeyboardEvents :DOM3级中添加
```
var btn = document.getElementById("myBtn");
//创建事件对象
var event = document.createEvent("MouseEvents");
//初始化事件对象
event.initMouseEvent("click", true, true, document.defaultView, 0, 0, 0, 0, 0,false, false, false, false, 0, null);
//添加事件监听
btn.addEventListener("click",e => console.log(e))
//触发事件dispatch中动态将this绑定到event.target上，也就是btn
btn.dispatchEvent(event);
//执行监听函数
//e
```
