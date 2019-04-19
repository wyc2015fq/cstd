# 小程序WXML 使用小结 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月05日 15:20:10[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1390
[数据绑定](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/data.html)
<view>
 {{message}} </view>
// page.jsPage({
data: { message:
'Hello MINA!' }})
组件属性(需要在双引号之内)
<view id="item-{{id}}"></view>
Page({ data: { id:
0 }})
控制属性(需要在双引号之内)
<view wx:if="{{condition}}"></view>
Page({ data: { condition:
true }})
关键字(需要在双引号之内)
true：boolean 类型的 true，代表真值。
false： boolean 类型的 false，代表假值。
<checkbox checked="{{false}}"></checkbox>
***特别注意：不要直接写 ******checked="false"***
可以在 {{}} 内进行简单的运算，支持的有如下几种方式：
三元运算
<view hidden="{{flag
 ? true : false}}"> Hidden
</view>
算数运算
<view>
 {{a + b}} + {{c}} + d </view>
Page({ data: { a:
1, b:
2, c:
3 }})
view中的内容为 3 + 3 + d
逻辑判断
<view wx:if="{{length
 > 5}}"></view>
字符串运算
<view>{{"hello"
 + name}}</view>
Page({ data:{ name:
'MINA' }})
数据路径运算
<view>{{object.key}}
 {{array[0]}}</view>
Page({ data: { object: { key:
'Hello ' }, array: ['MINA']
 }})
数组
<view
wx:for="{{[zero,
 1, 2, 3, 4]}}">
 {{item}} </view>
Page({ data: { zero:
0 }})
最终组合成数组[0,
 1, 2, 3, 4]。
对象
<template is="objectCombine"
 data="{{for: a, bar: b}}"></template>
Page({ data: { a:
1, b:
2 }})
最终组合成的对象是{for:
 1, bar: 2}

也可以用扩展运算符 ... 来将一个对象展开
<template is="objectCombine"
 data="{{...obj1, ...obj2, e: 5}}"></template>
Page({ data: { obj1: { a:
1, b:
2 }, obj2: { c:
3, d:
4 } }})
最终组合成的对象是 {a: 1, b: 2, c: 3, d: 4, e: 5}。
如果对象的 key 和 value 相同，也可以间接地表达。
<template is="objectCombine"
 data="{{foo, bar}}"></template>
Page({ data: { foo:
'my-foo', bar:
'my-bar' }})
最终组合成的对象是 {foo: 'my-foo', bar:'my-bar'}。

**注意：**上述方式可以随意组合，但是如有存在变量名相同的情况，后边的会覆盖前面，如：
<template is="objectCombine"
 data="{{...obj1, ...obj2, a, c: 6}}"></template>
Page({ data: { obj1: { a:
1, b:
2 }, obj2: { b:
3, c:
4 }, a:
5 }})
最终组合成的对象是 {a: 5, b: 3, c: 6}。
**注意：** 花括号和引号之间如果有空格，将最终被解析成为字符串
<view wx:for="{{[1,2,3]}}">
 {{item}}</view>
等同于
<view wx:for="{{[1,2,3]
+ ' '}}">
 {{item}}</view>

[列表渲染](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/list.html)
<view
wx:for="{{array}}"> {{item}} </view>
// page.jsPage({ data: { array: [1, 2, 3, 4, 5] }})
wx:for 绑定一个数组
默认数组的当前项的下标变量名默认为 index，数组当前项的变量名默认为 item
<view wx:for="{{array}}">
 {{index}}: {{item.message}}</view>
Page({ data: { array: [{ message:
'foo', }, { message:
'bar' }] }})
使用 wx:for-index 可以指定数组当前下标的变量名
使用 wx:for-item 可以指定数组当前元素的变量名
<view wx:for="{{array}}"
 wx:for-index="idx" wx:for-item="itemName">
 {{idx}}: {{itemName.message}}</view>
wx:for 可以嵌套，下边是一个九九乘法表
<view wx:for="{{[1,
 2, 3, 4, 5, 6, 7, 8, 9]}}" wx:for-item="i"><view wx:for="{{[1, 2, 3,
 4, 5, 6, 7, 8, 9]}}" wx:for-item="j"><view wx:if="{{i <= j}}">
 {{i}} * {{j}} = {{i * j}} </view></view></view>

也可以将 wx:for 用在<block/>标签上，以渲染一个包含多节点的结构块。例如：
<block wx:for="{{[1,
 2, 3]}}"><view> {{index}}:
</view><view> {{item}}
</view></block>

wx:key
如果列表中项目的位置会动态改变或者有新的项目添加到列表中，并且希望列表中的项目保持自己的特征和状态（如 <input/> 中的输入内容，<switch/> 的选中状态），需要使用 wx:key 来指定列表中项目的唯一的标识符。
wx:key 的值以两种形式提供
- 字符串，代表在 for 循环的 array 中 item 的某个 property，该 property 的值需要是列表中唯一的字符串或数字，且不能动态改变。
- 保留关键字 *this 代表在 for 循环中的 item 本身，这种表示需要 item 本身是一个唯一的字符串或者数字，如：
当数据改变触发渲染层重新渲染的时候，会校正带有 key 的组件，框架会确保他们被重新排序，而不是重新创建，以确保使组件保持自身的状态，并且提高列表渲染时的效率。
**如不提供 ****wx:key****，会报一个 ****warning****， 如果明确知道该列表是静态，或者不必关注其顺序，可以选择忽略。**
**示例代码：**
<switch wx:for="{{objectArray}}"
 wx:key="unique" style="display:
 block;"> {{item.id}}
</switch><button
 bindtap="switch">
 Switch </button>
<button
 bindtap="addToFront">
 Add to the front </button>
<switch
 wx:for="{{numberArray}}"
 wx:key="*this" style="display:
 block;"> {{item}}
</switch><button
 bindtap="addNumberToFront">
 Add to the front </button>
Page({ data: { objectArray: [ {id:
5, unique:
'unique_5'}, {id:
4, unique:
'unique_4'}, {id:
3, unique:
'unique_3'}, {id:
2, unique:
'unique_2'}, {id:
1, unique:
'unique_1'}, {id:
0, unique:
'unique_0'}, ], numberArray:
 [1,
2,
3,
4] },
switch:
function(e)
{
const length =
this.data.objectArray.length
for (let
 i = 0; i < length; ++i)
 { const x =
Math.floor(Math.random()
 * length) //向下取整const y =
Math.floor(Math.random()
 * length) const temp =
this.data.objectArray[x]
this.data.objectArray[x]
 = this.data.objectArray[y]
 //2个值交换一下 this.data.objectArray[y]
 = temp } this.setData({
 objectArray: this.data.objectArray
 }) },
addToFront:
function(e)
{
const length =
this.data.objectArray.length
this.data.objectArray = [{id:
 length, unique: 'unique_'
 + length}].concat(this.data.objectArray)
this.setData({ objectArray:
this.data.objectArray })
 },
addNumberToFront:
function(e){
this.data.numberArray = [
this.data.numberArray.length
 + 1 ].concat(this.data.numberArray)
this.setData({ numberArray:
this.data.numberArray })
 }})
**注意：**
当 wx:for 的值为字符串时，会将字符串解析成字符串数组
<view wx:for="array">
 {{item}}</view>
等同于
<view wx:for="{{['a','r','r','a','y']}}">
 {{item}}</view>
**注意：** 花括号和引号之间如果有空格，将最终被解析成为字符串
<view wx:for="{{[1,2,3]}}
 "> {{item}}</view>
等同于
<view wx:for="{{[1,2,3]
 + ' '}}" > {{item}}</view>

[条件渲染](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/conditional.html)
<view
 wx:if="{{view == 'WEBVIEW'}}">
 WEBVIEW </view><view
 wx:elif="{{view == 'APP'}}">
 APP </view><view
 wx:else="{{view == 'MINA'}}">
 MINA </view>
// page.jsPage({
 data: { view: 'MINA' }})
<block wx:if="{{true}}"><view> view1
</view><view> view2
</view></block>
<block/> 并不是一个组件，它仅仅是一个包装元素，不会在页面中做任何渲染，只接受控制属性。
wx:if vs hidden
因为 wx:if 之中的模板也可能包含数据绑定，所有当 wx:if 的条件值切换时，框架有一个局部渲染的过程，因为它会确保条件块在切换时销毁或重新渲染。
同时 wx:if 也是**惰性的**，如果在初始渲染条件为 false，框架什么也不做，在条件第一次变成真的时候才开始局部渲染。
相比之下，hidden 就简单的多，组件始终会被渲染，只是简单的控制显示与隐藏。
一般来说，wx:if 有更高的切换消耗而 hidden 有更高的初始渲染消耗。因此，如果需要频繁切换的情景下，用 hidden 更好，如果在运行时条件不大可能改变则 wx:if 较好。


[模板](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/template.html)
<template 
name="staffName"> <view> FirstName: {{firstName}}, LastName: {{lastName}} </view></template><template is="staffName" data="{{...staffA}}"></template><template is="staffName"
 data="{{...staffB}}"></template><template is="staffName" data="{{...staffC}}"></template>
// page.jsPage({ data: { staffA: {firstName: 'Hulk', lastName: 'Hu'}, staffB: {firstName: 'Shang', lastName: 'You'}, staffC: {firstName: 'Gideon', lastName: 'Lin'} }})
可以在模板中定义代码片段，然后在不同的地方调用。
name 属性，作为模板的名字。然后在<template/>内定义代码片段
<template name="msgItem"><view><text> {{index}}: {{msg}}
</text><text> Time: {{time}}
</text></view></template>
使用模板：使用 is 属性，声明需要的使用的模板，
然后将模板所需要的 data 传入
<template is="msgItem"
 data="{{...item}}"/>
Page({ data: { item: { index:
0, msg:
'this is a template', time:
'2016-09-15' } }})

动态决定具体需要渲染哪个模板：
<template name="odd"><view> odd
</view></template><template
 name="even"><view> even
</view></template><block
 wx:for="{{[1, 2, 3, 4, 5]}}"><template is="{{item % 2
 == 0 ? 'even' : 'odd'}}"/></block>
模板拥有自己的作用域，只能使用 data 传入的数据以及模版定义文件中定义的<wxs
 /> 模块。
[事件](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html)
<view 
bindtap="add"> {{count}} </view>
Page({ data: { count: 1 }, add: function(e) { this.setData({ count: this.data.count + 1 }) }})
事件对象可以携带额外信息，如 id, dataset, touches。
如何 使用？
在组件中绑定一个事件处理函数。
如bindtap，当用户点击该组件的时候会在该页面对应的Page中找到相应的事件处理函数。
<view id="tapTest"
 data-hi="WeChat" bindtap="tapName">
 Click me! </view>
写上相应的事件处理函数，参数是event。
Page({ tapName:
function(event)
{
console.log(event) }})

可以看到log出来的信息大致如下：
{"type":"tap","timeStamp":895,"target":
 { "id":
"tapTest",
"dataset": {
 "hi":"WeChat" }},"currentTarget":
 { "id":
"tapTest",
"dataset": {
"hi":"WeChat"
 }},"detail": {
"x":53,
"y":14},"touches":[{
"identifier":0,
"pageX":53,
"pageY":14,
"clientX":53,
"clientY":14}],"changedTouches":[{
"identifier":0,
"pageX":53,
"pageY":14,
"clientX":53,
"clientY":14}]}
事件分为冒泡事件和非冒泡事件：
- 冒泡事件：当一个组件上的事件被触发后，该事件会向父节点传递。
- 非冒泡事件：当一个组件上的事件被触发后，该事件不会向父节点传递。
冒泡事件列表
|类型|触发条件|
|----|----|
|touchstart|手指触摸动作开始|
|touchmove|手指触摸后移动|
|touchcancel|手指触摸动作被打断，如来电提醒，弹窗|
|touchend|手指触摸动作结束|
|tap|手指触摸后马上离开|
|longpress|手指触摸后，超过350ms再离开，如果指定了事件回调函数并触发了这个事件，tap事件将不被触发|
|longtap|手指触摸后，超过350ms再离开（推荐使用longpress事件代替）|
|transitionend|会在 WXSS transition 或 wx.createAnimation 动画结束后触发|
|animationstart|会在一个 WXSS animation 动画开始时触发|
|animationiteration|会在一个 WXSS animation 一次迭代结束时触发|
|animationend|会在一个 WXSS animation 动画完成时触发|
**除上表之外的其他组件自定义事件如无特殊声明都是非冒泡事件，如**[**<form/>**](https://mp.weixin.qq.com/debug/wxadoc/dev/component/form.html)**的****submit****事件，**[**<input/>**](https://mp.weixin.qq.com/debug/wxadoc/dev/component/input.html)**的****input****事件，**[**<scroll-view/>**](https://mp.weixin.qq.com/debug/wxadoc/dev/component/scroll-view.html)**的****scroll****事件，(详见各个**[**组件**](https://mp.weixin.qq.com/debug/wxadoc/dev/component/)**)**
事件绑定的写法同组件的属性，以 key、value 的形式。
key 以bind或catch开头，然后跟上事件的类型，如bindtap、catchtouchstart
bind和catch后也可以紧跟一个冒号，其含义不变，如bind:tap、、catch:touchstart。
value 是一个字符串，需要在对应的 Page 中定义同名的函数。不然当触发事件的时候会报错。
bind型事件绑定不会阻止冒泡事件向上冒泡，catch型事件绑定可以阻止冒泡事件向上冒泡。
如在下边这个例子中，点击 inner view 会先后调用handleTap3和handleTap2(因为tap事件会冒泡到
 middle view，而 middle view 阻止了 tap 事件冒泡，不再向父节点传递)，点击 middle view 会触发handleTap2，点击
 outer view 会触发handleTap1。
<view id="outer"
 bindtap="handleTap1">
 outer view <view id="middle"
 catchtap="handleTap2">
 middle view <view id="inner"
 bindtap="handleTap3">
 inner view </view></view></view>
触摸类事件支持捕获阶段。捕获阶段位于冒泡阶段之前
且在捕获阶段中，事件到达节点的顺序与冒泡阶段恰好相反。
需要在捕获阶段监听事件时，可以采用capture-bind、capture-catch关键字，后者将中断捕获阶段和取消冒泡阶段。
在下面的代码中，点击 inner view 会先后调用handleTap2、handleTap4、handleTap3、handleTap1。
<view id="outer"
 bind:touchstart="handleTap1"
 capture-bind:touchstart="handleTap2">
 outer view <view id="inner"
 bind:touchstart="handleTap3"
 capture-bind:touchstart="handleTap4">
 inner view </view></view>
如果将上面代码中的第一个capture-bind改为capture-catch，将只触发handleTap2。
<view id="outer"
 bind:touchstart="handleTap1"
 capture-catch:touchstart="handleTap2">
 outer view <view id="inner"
 bind:touchstart="handleTap3"
 capture-bind:touchstart="handleTap4">
 inner view </view></view>
当组件触发事件时，逻辑层绑定该事件的处理函数会收到一个事件对象。
**BaseEvent****基础事件对象****属性列表：**
|属性|说明|
|----|----|
|[type](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#type)|事件类型|
|[timeStamp](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#timeStamp)|事件生成时的时间戳，页面打开到触发事件所经过的毫秒数。|
|[target](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#target)|触发事件的组件的一些属性值集合|
|[currentTarget](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#currenttarget)|当前组件的一些属性值集合|
其中target,触发事件的源组件。
|属性|说明|
|----|----|
|id|事件源组件的id|
|tagName|当前组件的类型|
|[dataset](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#dataset)|事件源组件上由data-开头的自定义属性组成的集合|
currentTarget为事件绑定的当前组件
|属性|说明|
|----|----|
|id|当前组件的id|
|tagName|当前组件的类型|
|[dataset](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#dataset)|当前组件上由data-开头的自定义属性组成的集合|
**说明： target 和 currentTarget 可以参考上例中，点击 inner view 时，****handleTap3**** 收到的事件对象 target 和 currentTarget 都是 inner，而 ****handleTap2**** 收到的事件对象 target 就是****inner****，currentTarget 就是 middle。**
**其中dateset:**
在组件中可以定义数据，这些数据将会通过事件传递给 SERVICE。 书写方式： 以data-开头，多个单词由连字符-链接，不能有大写(大写会自动转成小写)如data-element-type，最终在
 event.currentTarget.dataset 中会将连字符转成驼峰elementType。
**示例：**
<view data-alpha-beta="1"
 data-alphaBeta="2" bindtap="bindViewTap">
 DataSet Test </view>
Page({ bindViewTap:function(event){
 event.currentTarget.dataset.alphaBeta === 1// - 会转为驼峰写法 event.currentTarget.dataset.alphabeta
 === 2// 大写会转为小写 }})
**CustomEvent****自定义事件对象****属性列表（继承 BaseEvent）：**
|属性|说明|
|----|----|
|[detail](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#detail)|额外的信息|
"detail":
 { "x":53,
"y":14},
自定义事件所携带的数据，如表单组件的提交事件会携带用户的输入，媒体的错误事件会携带错误信息，详见[组件](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/wxml/wxml-component.md)定义中各个事件的定义。
点击事件的detail 带有的
 x, y 同 pageX, pageY 代表距离文档左上角的距离。

**TouchEvent****触摸事件对象****属性列表（继承 BaseEvent）：**
|属性|说明|
|----|----|
|[touches](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#touches)|触摸事件，当前停留在屏幕中的触摸点信息的数组|
|[changedTouches](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html#changedTouches)|触摸事件，当前变化的触摸点信息的数组|
**特殊事件： ****<canvas/>**** 中的触摸事件不可冒泡，所以没有 currentTarget。**
其中
"touches":[{
"identifier":0,
"pageX":53,
"pageY":14,
"clientX":53,
"clientY":14}],
touches 是一个数组，每个元素为一个 Touch 对象（canvas 触摸事件中携带的 touches 是 CanvasTouch 数组）。 表示当前停留在屏幕上的触摸点。
Touch 对象
|属性|说明|
|----|----|
|identifier|触摸点的标识符|
|pageX, pageY|距离文档左上角的距离，文档的左上角为原点 ，横向为X轴，纵向为Y轴|
|clientX, clientY|距离页面可显示区域（屏幕除去导航条）左上角距离，横向为X轴，纵向为Y轴|
CanvasTouch 对象
|属性|说明|
|----|----|
|identifier|触摸点的标识符|
|x, y|距离 Canvas 左上角的距离，Canvas 的左上角为原点 ，横向为X轴，纵向为Y轴|
"changedTouches":[{
"identifier":0,
"pageX":53,
"pageY":14,
"clientX":53,
"clientY":14}]
changedTouches 数据格式同 touches。 表示有变化的触摸点，如从无变有（touchstart），位置变化（touchmove），从有变无（touchend、touchcancel）。

WXML 提供两种文件引用方式import和include。
import
import可以在该文件中使用目标文件定义的template，如：
在 item.wxml 中定义了一个叫item的template：<template
 name="item"><text>{{text}}</text></template>
在 index.wxml 中引用了 item.wxml，就可以使用item模板：
<import src="item.wxml"/><template
 is="item" data="{{text:
 'forbar'}}"/>
import 的作用域
只会 import 目标文件中定义的 template，而不会 import 目标文件 import 的 template。即不会递归引用
**如：C import B，B import A，在C中可以使用B定义的****template****，在B中可以使用A定义的****template****，****但是C不能使用A定义的****template**。
include
include 可以将目标文件**除了**<template/><wxs/>外的整个代码引入，相当于是拷贝到 include 位置，如：
<!-- index.wxml --><include
 src="header.wxml"/><view>
 body </view><include
 src="footer.wxml"/>
<!-- header.wxml --><view>
 header </view>
<!-- footer.wxml --><view>
 footer </view>
