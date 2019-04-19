# Vue.js 学习笔记 - zwvista - 博客园
## [Vue.js 学习笔记](https://www.cnblogs.com/zwvista/p/9656361.html)
### Vue.js
Vue.js 是时下最流行的前端 JavaScript 框架之一。
### 基本语法（HTML）
- 表达式采用 moustache 模板。
- 属性绑定用 v-bind:，简写为 :。
- 事件绑定用 v-on:，简写为 @。
- 值value的双向绑定用 v-model。
- 分支处理用 v-if, v-else。
- 循环处理用 v-for。
- 在绑定 class 时可以传入一个JS对象
该对象的字段名对应 class（注意class名内含横杠时需要用引号括起来）
而相应字段的值对应 Vue 实例的属性（布尔型）。
### 基本语法（JavaScript）
- 用 Vue 函数创建 Vue 实例，创建时传入一个可选项对象（options object）。
- 在可选项对象的 el 字段中声明元素节点。
- 在可选项对象的 data 字段中声明数据属性。
- 在可选项对象的 methods 字段中声明方法。
- 在可选项对象的 computed 字段中声明计算属性。
- 在可选项对象的 watch 字段中声明侦听属性。
- 可选项对象的 created, mounted 字段对应 lifecycle hook。
### 基本结构
```
<html>
<head>
  <script src="https://cdn.jsdelivr.net/npm/vue/dist/vue.js"></script>
  <meta charset="utf-8"/>
</head>
<body>
  <div id="app">
    {{ message }}
  </div>
  <script>
    var app = new Vue({
      el: '#app',
      data: {
        message: 'Hello Vue!'
      }
    });
    app.message = 'I have changed the data';
  </script>
</body>
</html>
```
这里 Vue 示例的创建可以写在页面最后的部分，也可以在 window.onload 事件中进行。
```
window.load = function() {
  var app = new Vue({
  // ...
  });
};
// 或者
$(function() {
  var app = new Vue({
  // ...
  });
});
```
### 基本语法（组件）
- 用 Vue.component 创建组件实例，创建时传入组件标签名以及一个可选项对象（options object）。
- 在可选项对象的 data 字段中声明数据属性。注意 data 字段返回函数。
- 在可选项对象的 template 字段中声明模板字符串。
- 在可选项对象的 props 字段中声明入口参数名。
- 在可选项对象的 methods 字段中声明方法。
- 在可选项对象的 computed 字段中声明计算属性。
- 在可选项对象的 watch 字段中声明侦听属性。
- 可选项对象的 created, mounted 字段对应 lifecycle hook。
### Vue CLI
```
# 安装 Vue CLI
$ npm install --global @vue/cli
# 创建 Vue 应用程序
$ vue create hello-world
# 进入程序所在路径
$ cd hello-world
# 安装依赖
$ npm install
# 启动应用程序
$ npm run serve
# http://localhost:8080/ 打开主页
```
### 基本语法（TypeScript）
[vue-class-component](https://github.com/vuejs/vue-class-component)
- 在类声明之前添加 @Component 修饰符，该修饰符可以带一个对象参数。
- data 部分变成了类中的属性。
- methods 部分变成了类中的方法。
- computed 部分变成了类中的 getter 和 setter。
- lifecycle hook 变成了类中的方法。
- 所有其他部分都照原样写在 @Component 修饰符的参数对象中。
### 基本语法（Vue文件）
Vue文件用于声明单文件组件。
- 在 *.vue 文件的`<template>`部分声明组件的专用模板
- 在 *.vue 文件的`<script>`部分声明组件的专用代码
- 在 *.vue 文件的`<style>`部分声明组件的专用CSS
### 基本语法（Vuex)
组件可以将自身的一部分或全部数据属性交给 Vuex 来管理。
通过这种方法可以在多个组件中共享数据（状态），以及使用浏览器插件跟踪状态的值。
- 用 Vuex.Store 创建 store（仓库） 实例，创建时传入一个可选项对象（options object）。
- 在可选项对象的 state 字段中声明状态属性。
- 在可选项对象的 getters 字段中声明读取状态属性的只读属性。
- 在可选项对象的 mutations 字段中声明设置以及更改状态属性的方法（必须是同步的方法）。
mutations 字段中的方法无法调用该字段中的其他方法。
- 在可选项对象的 actions 字段中声明设置以及更改状态属性的方法（可以是异步的方法）。
- actions 字段中的方法不能直接设置以及更改状态属性，但是
- 可以通过 commit 方法调用 mutations 字段中的方法。
- 可以通过 dispatch 方法调用 actions 字段中的其他方法。
### 在组件中使用 Vuex
- 在组件中的可选项对象中声明 store 字段将 store 对象引入组件。
- 在组件中引用 store 对象 state 字段中的 XXX 这个属性：
- this.store.state.XXX
- this.XXX（需要调用 mapState 方法将 store 对象 state 字段中的指定状态属性引入组件的 computed 字段中）
- 在组件中引用 store 对象 getters 字段中的 XXX 这个只读属性：
- this.store.getters.XXX
- this.XXX（需要调用 mapGetters 方法将 store 对象 getters 字段中的指定只读属性引入组件的 computed 字段中）
- 在组件中调用 store 对象 mutations 字段中的 XXX 这个方法：
- this.store.commit("XXX")
- this.XXX()（需要调用 mapMutations 方法将 store 对象 mutations 字段中的指定方法引入组件的 methods 字段中）
- 在组件中调用 store 对象 actions 字段中的 XXX 这个方法：
- this.store.dispatch("XXX")
- this.XXX()（需要调用 mapActions 方法将 store 对象 actions 字段中的指定方法引入组件的 methods 字段中）
