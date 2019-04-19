# Flux/Redux架构初步 - 三少GG - CSDN博客
2016年07月22日 20:26:36[三少GG](https://me.csdn.net/scut1135)阅读数：1571
最佳文章：
# 1. [https://segmentfault.com/a/1190000005356568](https://segmentfault.com/a/1190000005356568)
# [实例讲解基于 React+Redux 的前端开发流程](https://segmentfault.com/a/1190000005356568)
[](https://segmentfault.com/a/1190000005356568)
NOTE：从对象的包含关系上讲，reducer 是store的一部分，但在逻辑上我们把它分出来，这样会比较容易理解整个redux流程。
2. [http://www.lijianfei.cn/2016/04/20/RN-Rudex-Exp/](http://www.lijianfei.cn/2016/04/20/RN-Rudex-Exp/)
# 近期 React-Native With Redux 开发的一点心得
###################################################
https://github.com/react-guide/redux-tutorial-cn
||// 章节 0 - introduction.js|// 章节 0 - introduction.js|
|----|----|----|
|// 章节 0 - introduction.js| | |
||// 为什么要有这个教程呢？| |
||// 在我尝试学习 Redux 的时候，我发现之前的阅读过的一些文章加上个人的经验，| |
||// 让我对 flux 产生了一些错误的认知。| |
||// 当然，我不是说那些关于 flux 的文章写得不好，只是我没能正确地领会其中的概念。| |
||// 到头来，我只是对着各种 flux 框架（Reflux、Flummox、FB Flux）的文档照猫画虎，| |
||// 并试着把它们和之前了解到的理论概念联系起来。| |
||// 等我用了 Redux 之后，我才发现原来flux比我想象的要简单很多。| |
||// 这些都归功于 Redux 通过优良的设计来减少样板文件，而其它框架则是为了减少样板文件却又引入了很多新的代码。| |
||// 我现在觉得用通过 Redux 来学习 flux 比通过其他框架高效得多。| |
||// 这就是为什么我想分享给大家，用我自己的话来说，| |
||// 通过关注 Redux 的用法来理解 flux 的概念。| |
||| |
||// 你一定已经看过这张著名的 flux 的单向数据流图了。| |
||| |
||| |
||// 在这个教程里，我们会一步步地向你介绍上图里的各个概念。| |
||// 我们会把这些概念分成单独的章节来介绍它们存在的意义和作用，| |
||// 而不会笼统地介绍整张数据流图。| |
||// 在最后，当我们理解了每一个概念后，我们会发现这张图真是意义深远啊！| |
||| |
||// 在我们开始之前，我们先聊下一 flux 存在的意义以及我们为什么需要它。| |
||// 假设我们正在构建一个网站应用，那么这个网站应用会由什么组成呢？| |
||// 1) 模板/HTML = View| |
||// 2) 填充视图的数据 = Model| |
||// 3) 获取数据、将所有视图组装在一起、响应用户事件、| |
||// 数据操作等等的逻辑 = Controller| |
||| |
||// 这是我们熟知的非常典型的 MVC，但它和 flux 的概念其实是很像的，| |
||// 只是在某些表述上有些小小的不同：| |
||// - Model 看起来像 Store| |
||// - 用户事件、数据操作以及它们的处理程序看起来像| |
||// "action creators" -> action -> dispatcher -> callback| |
||// - View 看起来像 React view (或者其他什么的)| |
||| |
||// 所以，flux 就只是一个新名词么？不全是，但是新名词是很重要的，| |
||// 因为通过引入这些新术语我们可以更准确地表述各种专业术语。| |
||// 举一个例子，获取数据是一个 action，一个点击是一个 action，| |
||// 一个 input 变化也是一个 action 等等。我们都已经习惯了从我们的应用里分发 action，| |
||// 只是以不同的方式称呼它们。 不同于直接修改 Model 和 View，| |
||// Flux 确保所有 action 首先通过一个 dispatcher，| |
||// 然后再是 store，最后通知所有的 store 监听器。| |
||| |
||// 为了弄清楚 MVC 和 flux 的不同，| |
||// 我们举一个典型的 MVC 应用的用例：| |
||// 一个典型的 MVC 应用的流程大致上是这样的：| |
||// 1) 用户点击按钮 A| |
||// 2) 点击按钮 A 的处理程序触发 Model A 的改变| |
||// 3) Model A 的改变处理程序触发 Model B 的改变| |
||// 4) Model B 的改变处理程序触发 View B 的改变并重新渲染自身| |
||| |
||// 在这样的一个环境里，当应用出错的时候快速地定位 bug 来源是一件非常困难的事情。| |
||// 这是因为每个 View 可以监视任何的 Model，| |
||// 并且每个 Model 可以监视其它所有 Model，所以数据会从四面八方涌来，并且被许多源（view 或者 model）改变。| |
||| |
||// 当我们用 flux 以及它的单向数据流的时候，上面的例子就会变成这样子：| |
||// 1) 用户点击按钮 A| |
||// 2) 点击按钮A的处理程序会触发一个被分发的 action，并改变 Store A| |
||// 3) 因为其它的 Store 也被这个 action 通知了，所以 Store B 也会对相同的 action 做出反应| |
||// 4) View B 因为 Store A 和 Store B 的改变而收到通知，并重新渲染| |
||| |
||// 来看一下我们是如何避免 Store A 和 Store B 直接相关联的。| |
||// Store 只能被 action 修改，别无他选。| |
||// 并且当所有 Store 响应了 action 后，最终所有 View 都会更新。由此可见，数据总是沿一个方向进行流动：| |
||// action -> store -> view -> action -> store -> view -> action -> ...| |
||| |
###################################################
[http://f8-app.liaohuqiu.net/tutorials/building-the-f8-app/data/](http://f8-app.liaohuqiu.net/tutorials/building-the-f8-app/data/)
### 从 Flux 到 Redux
Redux 是一个实现了 Flux 架构，但又从 Flux 中剥离的框架。[react-redux
 包提供的官方的数据绑定实现](https://github.com/reactjs/react-redux) 使得和 React 的集成变得很简单。
Redux 中没有 Dispatcher，并且对于整个 app 的 `state`，只有一个
 Store。
那么 Redux 中的数据流到底是怎样的？详细的部分我们下面后阐述，现在让我们看看大致的流程：
- 
React 可以触发 Action，比如按钮点击按钮。
- 
Action 是对象，包含一个类型以及相关的数据，通过 Store 的 `dispatch()` 函数发送到
 Store。
- 
Store 接收 Action 的数据并将其连同当前的 `state` 树（[`state` 树](https://egghead.io/lessons/javascript-redux-the-single-immutable-state-tree) 是包含所有 `state` 的一种特殊的数据结构，是一个单一的对象）发给 Reducer。
- Reducer 是一个 [pure
 function](http://redux.js.org/docs/basics/Reducers.html#handling-actions)，它接收一个之前的 `state` 和一个
 Action；并基于此 Action 将会产生的影响，返回一个新的 `state`。一个
 app 可以包含一个 Reducer，但大部分的 app 最后会包含多个，每个处理 `state` 中不同的部分，[下文](http://f8-app.liaohuqiu.net/#reducers) 会提到。
- 
Store 接收到新的 `state`，并替换当前的。
- 
当 `state` 变化时，Store 触发 [事件](http://redux.js.org/docs/api/Store.html#subscribe)。
- 
任何 [订阅了事件](http://redux.js.org/docs/api/Store.html#subscribe) 的组件 [从
 Store 中提取新的 `state`](http://redux.js.org/docs/api/Store.html#getState)。
- 
组件使用新的 `state` 进行更新。
简单起见，这个流程可用下图表示：
你可以看到数据随着一个很清晰的单项路径流动，没有重叠，没有反方向的数据流。这图也展示了 app 的每一部分可以多么清晰地分开：
- 
Store 只关心所只有的 `state`；
- 
View 中的组件，只关心显示数据和触发 Action；
- 
Action 只关注 `state` 中的某些数据发生变化了，并包含了这些数据；
- 
Reducer 只关注旧的状态并将 Action 放入到 `state` 中。
一切都是模块化的，非常优雅。当阅读这样的代码的时候，表意非常明显，很容易理解。
和 Flux 相比，这有另外的一些好处：
- 
Action 是唯一一个可以导致 `state` 变化的途径，这也将这流程从
 UI 组件中归集起来。另外，因为 Action 都是由 Reducer 妥当排序的，这也防止了条件竞争。
- `state` 大体上是不可变的。创建一系列的 `state` ，每个 `state` 代表每个个体的变化。这给我们了一个
 app 中清晰并且易于追溯的 `state` 历史。

Redux流程：
![](http://139.196.14.76/uploads/default/optimized/1X/8aa93769b05ad73c4b132953a382364df740f2da_1_660x500.png)
各部件功能简介：
- State
一个应用的唯一一个数据对象，是纯数据，只有reducer可修改，其他只读，所有数据均存储在state中（与应用状态一一对应，可理解为应用的快照）
一个组件可以只监听其所关心的state中特定的key下的数据更新，性能较好。
- Action
操作的抽象，Action是一个数据对象，描述即将改变state的意图。它是将数据放入Store中的唯一途径。
- Reducer
reducer接收上一个state和Action，并输出下一个state，并且不能带副作用。是修改Store的途径。
- Store
Store维护state对象，并进行Action的分发和接收View的监听。
- middleware
扩展的一种方式，通常用于拦截特殊的Action，例如异步Action，并重新分发新的标准Action。
优势：
1. 由于Action有统一的入口，所有对数据的修改都有迹可循，
2. 同时由于其函数式变成无副作用的特点，可以单独对各部分进行测试，为以后推行单测提供便利和可能。
3. 另外，单一Store和reducer无副作用使下面的时间旅行成为可能。
不足：
编码要从命令式转为函数式，思路要转变。
生态圈：
redux的一个很大的优点是生态圈很完善，特别是redux-devtools，对开发调试提供了很大的方便
[http://camsong.github.io/redux-in-chinese/docs/introduction/Ecosystem.html4](http://camsong.github.io/redux-in-chinese/docs/introduction/Ecosystem.html)
[https://github.com/gaearon/redux-devtools1](https://github.com/gaearon/redux-devtools)
下面链接展示redux-devtools的时间旅行（time travel）的特性，记录/回放任意操作。
（不知道怎么放gif图。。。）
![](https://camo.githubusercontent.com/d61984306d27d5e0739efc2d57c56ba7aed7996c/68747470733a2f2f73332e616d617a6f6e6177732e636f6d2f662e636c2e6c792f6974656d732f3269314c3147316e3161316833793161324f31772f53637265656e2532305265636f7264696e67253230323031352d30382d3034253230617425323030372e3435253230504d2e676966)
![](https://camo.githubusercontent.com/c2c0ba1ad82d003b5386404ae09c00763d73510c/687474703a2f2f692e696d6775722e636f6d2f72764352394f512e706e67)

