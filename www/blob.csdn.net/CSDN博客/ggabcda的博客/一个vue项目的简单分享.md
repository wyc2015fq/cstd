# 一个vue项目的简单分享 - ggabcda的博客 - CSDN博客





2017年09月12日 16:58:03[ggabcda](https://me.csdn.net/ggabcda)阅读数：495








回首用vue已经2个多月了，今年7月底根据vue社区提供的api写了一个小移动端的小dom

通过这个项目也让我更深入的了解了vue（组件之间的通讯，计算属性，数据绑定、数据驱动....），用数据驱动型的框架做项目好处就是基本上不需要自己操作dom，框架会通过数据改变帮你最优的操作,也让你开发更加迅速

vue数据的更新：** 获取数据（转换数据） →绑定事件→渲染dom →监听事件→改变数据（计算）→重新绑定事件→改变dom**

首先vue会遍历一遍你定义的 data 然后通过 这个ES5的属性（Object.defineProperty）把你的data全部转为 getter/setter。

每个组件都有一个自己的 观察数据的 实例对象（watcher ） 当你的数据改变是 这个实例对象会得到一个通知，然后重新计算更新

当然我只是从官网文档copy了一小部分，vue还在内部做了很多优化我们看不见，比如异步更新，等等



**vuex**： vuex是vue的状态管理器 就行 angular的状态机 react 的 redux。

他提供了 一个（或者多个state）

**getter**： 获取vuex的state。

**mutation**：设置vuex的state（最好通过这个设置才能让vue观察到vuex数据的绑定，建议别图省事）。

**action**： 大量或者需要异步提交的 数据可以通过 action (甚至能通过这个去发ajax请求，因为他是异步的）

**分割 state**： Vuex 允许我们将 store 分割成模块（module）。每个模块拥有自己的 state、mutation、action、getter、甚至是嵌套子模块——从上至下进行同样方式的分割( getter、action可以通过rootState 获取上游的 state但mutation似乎不行)

**vuex的使用环境**：（跨组件数据通信多，频繁，或者想通过vue更加直观的观察数据，因为数据传递有时候会不好追踪，尤其是双向改变的时候。当数据传递多的时候更是）

如果不用vuex的话：

**父组件向子组件:**传递用 props传递，然后子组件就可以拿到数据

**子组件向父组件:**传递通过this.$emit（‘事件名’， data）然后父组件监听事件拿到传过来的数据

**非父子组件:** 创建一个 空vue实例 new Vue() 然后 用这个实例 派发事件让别的组件去监听（用的少，基本上遇到这个就是vuex）



**vue-router：**

组件内定义一个<router-view><router-view/>标签 该组件旗下所有的子路由都会进入这个 标签内（可以定义多个，单页面多路由操作）

路由钩子: 可以创建全局钩子，或者单个路由的独享钩子 每个钩子都有三个参数（to, from, next）

**to:** 即将要进入的目标 路由对象 可以通过这个参数获取 对面路由传的参数和 路径等等

**from：**没用过

**next：**想要离开这个路由就要这些这个参数（他是一个方法）,可以通过他跳到别的路由或者中断路由

ps: 甚至可以通过 watch 去观察路由 做一些不可描述的事情

**编程式的导航:** this.push（{}） this.go() 指哪打哪

**传递路由参数获取参数:** 如果是通过 router.push的话 加一个query参数(它是一个对象），进入的如有通过 this.$router.query.***获取

**动态路由：** 可以设置动态路径参数，达到数据不同但组件同一个的效果

**懒加载：** vue的异步组件 加 code splitting feature （简单粗暴）

const *** = () => import('@/views/***')

**路由动画：** 给路由包裹一个 <transition name='***'></transition> 通过给name设置class

***-enterr: 初始状态

***-leave-active:离开变成什么样

***-enter-active： 进来时变化成什么样

可以通过 watch 观察router 然后动态 设置name 设置不一样的动画。官网有例子



还有很多，明天再写吧~~

附上github地址： https://github.com/Xudaobin/VueCommunity

项目用到的技术：vue+vuex+vue-route+vue-cli+axios+vue-lazyload+better-scroll

欢迎各路大神 指出问题



