# vue通信、传值的多种方式 - weixin_33985507的博客 - CSDN博客
2018年10月14日 23:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
### 通过路由带参数进行传值
两个组件 `A`和`B`,`A`组件通过`query`把`orderId`传递给`B`组件（触发事件可以是点击事件、钩子函数等）。
```
this.$router.push({ path: '/conponentsB', query: { orderId: 123 } }) // 跳转到B
```
在`B`组件中获取`A`组件传递过来的参数。
```
this.$route.query.orderId
```
### 父子组件之间的传值
##### 父组件往子组件传值props
- 定义父组件，父组件传递`number`这个数值给子组件，如果传递的参数很多，推荐使用`json`数组`{ }`的形式。
- 子组件通过 props方法获取父组件传递过来的值。props中可以定义能接收的数据类型，如果不符合会报错。
- 子组件往父组件传值，通过emit事件。
```
<!-- parent.vue -->
<template>
  <div class="parent">
    <child @toParent="toChild" :number="number"></child>
  </div>
</template>
<script>
import child from './child.vue'
export default {
  data () {
    return {
      number: 666
    }
  },
  components: {
    child
  },
  methods: {
    toChild (data) {
      this.number = data
    }
  }
}
</script>
```
> 
父子组件传值，数据是异步请求，有可能数据渲染时报错。原因：异步请求时，数据还没有获取到但是此时已经渲染节点了。可以在 父组件需要传递数据的节点加上`v-if = false`，异步请求获取数据后，`v-if = true`。
```
<!-- child.vue -->
<template>
  <div class="child">
    <p @click="toParent">{{ number }}</p>
  </div>
</template>
<script>
export default {
  props: ['number'],
  methods: {
    toParent () {
      this.emit('toChild', '888')
    }
  }
}
</script>
```
### eventBus
发布者通过`eventBus`发布事件，订阅者通过`eventBus`订阅事件。当发布者发布事件时，订阅该事件的订阅者的事件处理方法将被调用。
在`src`目录下创建`eventBus.js`文件。
```
import Vue from 'vue'
 
const eventBus = new Vue()
 
export { eventBus }
```
`A`组件中通过触发事件或者其他什么，然后发射数据。
```
this.$emit('key','data-1232131232132131232')
```
B组件中接收A组件的数据。
```
this.$on('key',function(params){
    console.log(params);  // data-1232131232132131232
})
```
### vuex
安装 `Vuex`之后，让我们来创建一个 `store`。创建过程直截了当。仅需要提供一个初始 `state`对象和一些`mutation`：
```
// 如果在模块化构建系统中，请确保在开头调用了 Vue.use(Vuex)
const store = new Vuex.Store({
  state: {
    count: 0
  },
  mutations: {
    increment (state) {
      state.count++
    }
  }
})
```
现在，你可以通过 `store.state` 来获取状态对象，以及通过 `store.commit` 方法触发状态变更：
```
store.commit('increment')
console.log(store.state.count) // -> 1
```
再次强调，我们通过提交`mutation`的方式，而非直接改变 `store.state.count`。
#### 单一状态树
每个应用将仅仅包含一个 `store` 实例。单一状态树让我们能够直接地定位任一特定的状态片段，在调试的过程中也能轻易地取得整个当前应用状态的快照。
##### 在 Vue 组件中获得 Vuex 状态
`Vuex`通过`store`选项，提供了一种机制将状态从根组件“注入”到每一个子组件中（需调用 `Vue.use(Vuex)`）：
```
const app = new Vue({
  el: '#app',
  // 把 store 对象提供给 “store” 选项，这可以把 store 的实例注入所有的子组件
  store,
  components: { Counter },
  template: `
    <div class="app">
      <counter></counter>
    </div>
  `
})
```
通过在根实例中注册`store` 选项，该 store 实例会注入到根组件下的所有子组件中，且子组件能通过 this.$store 访问到。让我们更新下 Counter 的实现：
```
const Counter = {
  template: `<div>{{ count }}</div>`,
  computed: {
    count () {
      return this.$store.state.count
    }
  }
}
```
