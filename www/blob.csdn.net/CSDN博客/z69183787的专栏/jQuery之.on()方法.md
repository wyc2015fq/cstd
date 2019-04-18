# jQuery之.on()方法 - z69183787的专栏 - CSDN博客
2014年05月04日 17:00:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2788
还不是完全清楚如何使用.on()进行jQuery事件绑定的同学先看这里[http://api.jquery.com/on/](http://api.jquery.com/on/)
jQuery绑定事件的方法有几种，推荐使用.on()方法绑定，原因有两点：
### 1.on()方法可以绑定动态添加到页面元素的事件
比如动态添加到页面的DOM元素，用.on()方法绑定的事件不需要关心注册该事件的元素何时被添加进来，也不需要重复绑定。有的同学可能习惯于用.bind()、.live()或.delegate()，查看源码就会发现，它们实际上调用的都是.on()方法，并且.live()方法在jQuery1.9版本已经被移除。
```
```
bind:
```
`function`
```
(
 types, data, fn ) {
```
`    ``return``this`
```
.on(
 types,
```
`null`
```
,
 data, fn );
```
`},`
```
live:
```
`function`
```
(
 types, data, fn ) {
```
`    `
```
jQuery(
```
`this`
```
.context
 ).on( types,
```
`this`
```
.selector,
 data, fn );
```
`    ``return``this``;`
`},`
```
delegate:
```
`function`
```
(
 selector, types, data, fn ) {
```
`    ``return``this`
```
.on(
 types, selector, data, fn );
```
`}`
```
移除.on()绑定的事件用.off()方法。
### 2.on()方法绑定事件可以提升效率
很多文章都提到了利用事件冒泡和代理来提升事件绑定的效率，大多都没列出具体的差别，所以为了求证，我做一个小测试。
假设页面添加了5000个li，用chrome开发者工具Profiles测试页面载入时间。
普通绑定（姑且这么称呼它）
```
`$(``'li'``).click(``function``(){`
`    ``console.log(``this``)`
`});`
```
绑定过程的执行时间
![2013-08-13_190358](http://images.cnitblog.com/blog/551140/201308/14154950-49a3f81486494d82b63b6462d21e3bca.png)
普通绑定相当于在5000li上面分别注册click事件，内存占用约4.2M，绑定时间约为72ms。
.on()绑定
```
`$(document).on(``'click'`
```
,
```
`'li'`
```
,
```
`function``(){`
`    ``console.log(``this``)`
`})`
```
绑定过程的执行时间
![2013-08-13_191010](http://images.cnitblog.com/blog/551140/201308/14154950-4086085a6037494cb2821aa973a12431.png)
.on()绑定利用事件代理，只在document上注册了一个click事件，内存占用约2.2M，绑定时间约为1ms。
## .on()源码分析
.on()方法分析包含其调用的两个主要方法： 
.add()进行事件注册
.dispatch()进行事件代理
```
```
/*
 jQuery 1.10.2 */
```
```
on:
```
`function`
```
(
 types, selector, data, fn,
```
`/*INTERNAL*/`
```
one
 ) {
```
`    ``var`
```
type,
 origFn;
```
`    `
```
//
 Types can be a map of types/handlers
```
`    ``if`
```
(
```
`typeof`
```
types
 ===
```
`"object"`
```
)
 {
```
`        `
```
//
 ( types-Object, selector, data )
```
`        ``if`
```
(
```
`typeof`
```
selector
 !==
```
`"string"`
```
)
 {
```
`            `
```
//
 ( types-Object, data )
```
`            `
```
data
 = data || selector;
```
`            `
```
selector
 = undefined;
```
`        ``}`
`        `
```
//
 遍历types对象，针对每一个属性绑定on()方法
```
`        `
```
//
 将types[type]作为fn传入
```
`        ``for`
```
(
 type
```
`in`
```
types
 ) {
```
`            ``this`
```
.on(
 type, selector, data, types[ type ], one );
```
`        ``}`
`        ``return``this``;`
`    ``}`
`    `
```
//
 参数修正
```
`    `
```
//
 jQuery这种参数修正的方法很好
```
`    `
```
//
 可以兼容多种参数形式
```
`    `
```
//
 可见在灵活调用的背后做了很多处理
```
`    ``if`
```
(
 data ==
```
`null`
```
&&
 fn ==
```
`null`
```
)
 {
```
`        `
```
//
 ( types, fn )
```
`        `
```
fn
 = selector;
```
`        `
```
data
 = selector = undefined;
```
`    `
```
}
```
`else``if`
```
(
 fn ==
```
`null`
```
)
 {
```
`        ``if`
```
(
```
`typeof`
```
selector
 ===
```
`"string"`
```
)
 {
```
`            `
```
//
 ( types, selector, fn )
```
`            `
```
fn
 = data;
```
`            `
```
data
 = undefined;
```
`        `
```
}
```
`else``{`
`            `
```
//
 ( types, data, fn )
```
`            `
```
fn
 = data;
```
`            `
```
data
 = selector;
```
`            `
```
selector
 = undefined;
```
`        ``}`
`    ``}`
`    ``if`
```
(
 fn ===
```
`false`
```
)
 {
```
`        `
```
//
 fn传入false时，阻止该事件的默认行为
```
`        `
```
//
 function returnFalse() {return false;}
```
`        `
```
fn
 = returnFalse;
```
`    `
```
}
```
`else``if`
```
(
 !fn ) {
```
`        ``return``this``;`
`    ``}`
`    `
```
//
 one()调用on()
```
`    ``if`
```
(
 one === 1 ) {
```
`        `
```
origFn
 = fn;
```
`        `
```
fn
 =
```
`function`
```
(
 event ) {
```
`            `
```
//
 Can use an empty set, since event contains the info
```
`            `
```
//
 用一个空jQuery对象，这样可以使用.off方法，
```
`            `
```
//
 并且event带有remove事件需要的信息
```
`            `
```
jQuery().off(
 event );
```
`            ``return`
```
origFn.apply(
```
`this`
```
,
 arguments );
```
`        ``};`
`        `
```
//
 Use same guid so caller can remove using origFn
```
`        `
```
//
 事件删除依赖于guid
```
`        `
```
fn.guid
 = origFn.guid || ( origFn.guid = jQuery.guid++ );
```
`    ``}`
`    `
```
//
 这里调用jQuery的each方法遍历调用on()方法的jQuery对象
```
`    `
```
//
 如$('li').on(...)则遍历每一个li传入add()
```
`    `
```
//
 推荐使用$(document).on()或者集合元素的父元素
```
`    ``return``this`
```
.each(
```
`function`
```
()
 {
```
`        `
```
jQuery.event.add(
```
`this`
```
,
 types, fn, data, selector );
```
`    ``});`
`},`
```
//
 事件注册
```
```
add:
```
`function`
```
(
 elem, types, handler, data, selector ) {
```
`    ``var`
```
tmp,
 events, t, handleObjIn,
```
`        `
```
special,
 eventHandle, handleObj,
```
`        `
```
handlers,
 type, namespaces, origType,
```
`        `
```
elemData
 = jQuery._data( elem );
```
`    `
```
//
 Don't attach events to noData or
```
`    `
```
//
 text/comment nodes (but allow plain objects)
```
`    `
```
//
 不符合绑定条件的节点
```
`    ``if`
```
(
 !elemData ) {
```
`        ``return``;`
`    ``}`
`    `
```
//
 Caller can pass in an object of custom data in lieu of the handler
```
`    `
```
//
 传入的handler为事件对象
```
`    ``if`
```
(
 handler.handler ) {
```
`        `
```
handleObjIn
 = handler;
```
`        `
```
handler
 = handleObjIn.handler;
```
`        `
```
selector
 = handleObjIn.selector;
```
`    ``}`
`    `
```
//
 Make sure that the handler has a unique ID,
```
`    `
```
//
 used to find/remove it later
```
`    `
```
//
 为handler分配一个ID，用于之后的查找或删除
```
`    ``if`
```
(
 !handler.guid ) {
```
`        `
```
handler.guid
 = jQuery.guid++;
```
`    ``}`
`    `
```
//
 Init the element's event structure and main handler,
```
`    `
```
//
 if this is the first
```
`    `
```
//
 初始化events结构
```
`    ``if`
```
(
 !(events = elemData.events) ) {
```
`        `
```
events
 = elemData.events = {};
```
`    ``}`
`    ``if`
```
(
 !(eventHandle = elemData.handle) ) {
```
`        `
```
eventHandle
 = elemData.handle =
```
`function`
```
(
 e ) {
```
`            `
```
//
 Discard the second event of a jQuery.event.trigger() and
```
`            `
```
//
 when an event is called after a page has unloaded
```
`            ``return``typeof`
```
jQuery
 !== core_strundefined &&
```
`            `
```
(!e
 || jQuery.event.triggered !== e.type) ?
```
`                `
```
jQuery.event.dispatch.apply(
 eventHandle.elem, arguments ) :
```
`                ``undefined;`
`        ``};`
`        `
```
//
 Add elem as a property of the handle fn
```
`        `
```
//
 to prevent a memory leak with IE non-native events
```
`        `
```
//
 添加elem为eventHandle的属性，防止IE非本地事件的内存泄露？
```
`        `
```
//
 搜索整个源码，只有110行用到了eventHandle.elem
```
`        `
```
eventHandle.elem
 = elem;
```
`    ``}`
`    `
```
//
 Handle multiple events separated by a space
```
`    `
```
//
 处理多个以空格分隔的事件类型
```
`    `
```
types
 = ( types ||
```
`""`
```
).match(
 core_rnotwhite ) || [
```
`""``];`
`    `
```
t
 = types.length;
```
`    ``while`
```
(
 t-- ) {
```
`        `
```
tmp
 = rtypenamespace.exec( types[t] ) || [];
```
`        `
```
type
 = origType = tmp[1];
```
`        `
```
//
 存储所有命名空间
```
`        `
```
namespaces
 = ( tmp[2] ||
```
`""`
```
).split(
```
`"."``).sort();`
`        `
```
//
 There *must* be a type, no attaching namespace-only handlers
```
`        ``if`
```
(
 !type ) {
```
`            ``continue``;`
`        ``}`
`        `
```
//
 If event changes its type,
```
`        `
```
//
 use the special event handlers for the changed type
```
`        `
```
//
 对于改变了事件类型的特殊事件
```
`        `
```
special
 = jQuery.event.special[ type ] || {};
```
`        `
```
//
 If selector defined, determine special event api type,
```
`        `
```
//
 otherwise given type
```
`        `
```
type
 = ( selector ? special.delegateType : special.bindType ) || type;
```
`        `
```
//
 Update special based on newly reset type
```
`        `
```
special
 = jQuery.event.special[ type ] || {};
```
`        `
```
//
 handleObj is passed to all event handlers
```
`        `
```
handleObj
 = jQuery.extend({
```
`            `
```
type:
 type,
```
`            `
```
origType:
 origType,
```
`            `
```
data:
 data,
```
`            `
```
handler:
 handler,
```
`            `
```
guid:
 handler.guid,
```
`            `
```
selector:
 selector,
```
`            `
```
needsContext:
 selector && jQuery.expr.match.needsContext.test( selector ),
```
`            `
```
namespace:
 namespaces.join(
```
`"."``)`
`        `
```
},
 handleObjIn );
```
`        `
```
//
 Init the event handler queue if we're the first
```
`        `
```
//
 初始化handler队列，只初始化一次
```
`        ``if`
```
(
 !(handlers = events[ type ]) ) {
```
`            `
```
handlers
 = events[ type ] = [];
```
`            `
```
handlers.delegateCount
 = 0;
```
`            `
```
//
 Only use addEventListener/attachEvent
```
`            `
```
//
 if the special events handler returns false
```
`            ``if`
```
(
 !special.setup ||
```
`            `
```
special.setup.call(
 elem, data, namespaces, eventHandle ) ===
```
`false`
```
)
 {
```
`                `
```
//
 Bind the global event handler to the element
```
`                `
```
//
 二级DOM事件/IE事件模型
```
`                `
```
//
 eventHandle会调用jQuery.event.dispatch进行事件代理
```
`                ``if`
```
(
 elem.addEventListener ) {
```
`                    `
```
elem.addEventListener(
 type, eventHandle,
```
`false``);`
`                `
```
}
```
`else``if`
```
(
 elem.attachEvent ) {
```
`                    `
```
elem.attachEvent(
```
`"on"`
```
+
 type, eventHandle );
```
`                ``}`
`            ``}`
`        ``}`
`        ``if`
```
(
 special.add ) {
```
`            `
```
special.add.call(
 elem, handleObj );
```
`            ``if`
```
(
 !handleObj.handler.guid ) {
```
`                `
```
handleObj.handler.guid
 = handler.guid;
```
`            ``}`
`        ``}`
`        `
```
//
 Add to the element's handler list, delegates in front
```
`        ``if`
```
(
 selector ) {
```
`            `
```
handlers.splice(
 handlers.delegateCount++, 0, handleObj );
```
`        `
```
}
```
`else``{`
`            `
```
handlers.push(
 handleObj );
```
`        ``}`
`        `
```
//
 Keep track of which events have ever been used,
```
`        `
```
//
 for event optimization
```
`        `
```
//
 跟踪每个事件是否被使用过，为了事件优化
```
`        `
```
jQuery.event.global[
 type ] =
```
`true``;`
`    ``}`
`    `
```
//
 Nullify elem to prevent memory leaks in IE
```
`    `
```
//
 将变量置空，防止循环引用导致IE内存泄露
```
`    `
```
elem
 =
```
`null``;`
`},`
```
//
 事件代理
```
```
dispatch:
```
`function`
```
(
 event ) {
```
`    `
```
//
 Make a writable jQuery.Event from the native event object
```
`    `
```
//
 jQuery定义的event对象，兼容标准事件模型与IE事件模型
```
`    `
```
event
 = jQuery.event.fix( event );
```
`    ``var`
```
i,
 ret, handleObj, matched, j,
```
`        `
```
handlerQueue
 = [],
```
`        `
```
args
 = core_slice.call( arguments ),
```
`        `
```
handlers
 = ( jQuery._data(
```
`this`
```
,
```
`"events"`
```
)
 || {} )[ event.type ] || [],
```
`        `
```
special
 = jQuery.event.special[ event.type ] || {};
```
`    `
```
//
 Use the fix-ed jQuery.Event rather than the (read-only) native event
```
`    `
```
//
 使用jQuery.Event代替浏览器的event
```
`    `
```
args[0]
 = event;
```
`    `
```
//
 事件的代理节点，比如document
```
`    `
```
event.delegateTarget
 =
```
`this``;`
`    `
```
//
 Call the preDispatch hook for the mapped type,
```
`    `
```
//
 and let it bail if desired
```
`    ``if`
```
(
 special.preDispatch &&
```
`        `
```
special.preDispatch.call(
```
`this`
```
,
 event ) ===
```
`false`
```
)
 {
```
`        ``return``;`
`    ``}`
`    `
```
//
 Determine handlers
```
`    `
```
//
 遍历事件发生节点至代理节点之间的所有节点
```
`    `
```
//
 匹配每一个发生节点=?绑定节点
```
`    `
```
handlerQueue
 = jQuery.event.handlers.call(
```
`this`
```
,
 event, handlers );
```
`    `
```
//
 Run delegates first; they may want to stop propagation beneath us
```
`    `
```
i
 = 0;
```
`    `
```
//
 遍历匹配的节点，并且没有被阻止冒泡
```
`    ``while`
```
(
 (matched = handlerQueue[ i++ ]) && !event.isPropagationStopped() ) {
```
`        `
```
event.currentTarget
 = matched.elem;
```
`        `
```
j
 = 0;
```
`        ``while`
```
(
 (handleObj = matched.handlers[ j++ ]) &&
```
`         `
```
!event.isImmediatePropagationStopped()
 ) {
```
`            `
```
//
 Triggered event must either 1) have no namespace, or
```
`            `
```
//
 2) have namespace(s) a subset or equal to those
```
`            `
```
//
 in the bound event (both can have no namespace).
```
`            ``if`
```
(
 !event.namespace_re ||
```
`             `
```
event.namespace_re.test(
 handleObj.namespace ) ) {
```
`                `
```
event.handleObj
 = handleObj;
```
`                `
```
event.data
 = handleObj.data;
```
`                `
```
//
 传入绑定事件的具体节点，调用事件发生函数
```
`                `
```
ret
 = ( (jQuery.event.special[ handleObj.origType ] || {}).handle ||
```
`                 `
```
handleObj.handler
 )
```
`                        `
```
.apply(
 matched.elem, args );
```
`                ``if`
```
(
 ret !== undefined ) {
```
`                    ``if`
```
(
 (event.result = ret) ===
```
`false`
```
)
 {
```
`                        ``event.preventDefault();`
`                        ``event.stopPropagation();`
`                    ``}`
`                ``}`
`            ``}`
`        ``}`
`    ``}`
`    `
```
//
 Call the postDispatch hook for the mapped type
```
`    ``if`
```
(
 special.postDispatch ) {
```
`        `
```
special.postDispatch.call(
```
`this`
```
,
 event );
```
`    ``}`
`    ``return``event.result;`
`}`
```
