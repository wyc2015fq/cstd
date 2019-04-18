# 浅谈Vue.js - z69183787的专栏 - CSDN博客
2016年09月07日 16:57:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5376
> 
作为一名Vue.js的忠实用户，我想有必要写点文章来歌颂这一门美好的语言了，我给它的总体评价是“简单却不失优雅，小巧而不乏大匠”，下面将围绕这句话给大家介绍Vue.js，希望能够激发你对Vue.js的兴趣。
## Vue.js简介
Vue.js的作者为Evan You（尤雨溪），任职于Google Creative Lab，虽然是Vue是一个个人项目，但在发展前景上个人认为绝不输于Google的AngularJs，下面我会将Vue与Angular（Angular 1.0+版本）做一些简单的比较。
Vue的主要特点就和它官网（[http://cn.vuejs.org/](http://cn.vuejs.org/)）所介绍的那样：
（1） 简洁 （2） 轻量 （3）快速 （4） 数据驱动 （5） 模块友好 （6） 组件化
## 简单
下面看一段Angular的实现双向绑定的代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// html
<body ng-app="myApp">
    <div ng-controller="myCtrl">
        <p>{{ note }}</p>
        <input type="text" ng-model="note">
    </div>
</body>
// js
var myModule = angular.module('myApp', []);
myModule.controller('myCtrl', ['$scopp', function($scope) {
    $scope.note = '';
]);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
然后再看一下Vue的代码
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// html
<body>
    <div id="app">
        <p>{{ note }}</p>
        <input type="text" v-model="note">
    </div>
</body>
// js
var vm = new Vue({
    el: '#app',
    data: {
        note: ''
    }
})
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
相比较而言我个人认为Vue的代码编写风格更加简洁，并且通俗易懂。
## 不失优雅
Vue虽然是一个比较轻量级的框架，简单轻量的同时还非常的人性化，其提供的API也是非常的容易理解，同时也提供了一些很便捷的指令和属性。
例如：
（1）绑定click事件
```
<a v-on:click="doSomething"></a>
```
可以简写为：
```
<a @click="doSomething"></a>
```
(2) 绑定动态属性
```
<a v-bind:href="url"></a>
```
可以简写为：
```
<a :href="url"></a>
```
(3) 便捷的修饰符
```
<!-- 阻止单击事件冒泡 -->
<a @click.stop="doSomething"></a>
<!-- 只在按下回车键的时候触发事件 -->
<input @keyup.enter="submit">
```
(4) 实用的参数特性
```
<!-- debounce 设置一个最小的延时 -->
<input v-model="note" debounce="500">
<!-- 在 "change" 而不是 "input" 事件中更新数据 -->
<input v-model="msg" lazy>
```
怎么样，是不是感觉优雅极了。
## 小巧
说起小巧，那应该首先要关注下Vue的源码大小，Vue的成产版本（即min版）源码仅为72.9kb，官网称gzip压缩后只有25.11kb，相比Angular的144kb缩小了一半。
小巧的一种好处就是可以让用户更自由的选择相应的解决方案，在配合其他库方面它给了用户更大的空间。
如Vue的核心默认是不包含路由和 Ajax 功能，但是如果项目中需要路由和AJAX，可以直接使用Vue提供的官方库Vue-router及第三方插件vue-resource，同时你也可以使用其他你想要使用的库或插件，如jQuery的AJAX等。
是不是感觉非常的灵活。
## 不乏大匠
Vue虽然小巧，但是“麻雀虽小五脏俱全”，在构建大型应用的时候也是得心应手。
(1) 模块化
结合一些第三方模块构建工具，如CommonJS、RequireJS或者SeaJs，可以轻松实现代码的模块化。
但是在这里小编不推荐使用上述构建工具，直接使用ES6的模块化功能，再结合Webpack进行相应打包是目前最热门的方案。
不了解ES6模块功能的可以详见：[http://es6.ruanyifeng.com/#docs/module](http://es6.ruanyifeng.com/#docs/module)
在今后的文章中，我也会对其进行介绍，包括Webpack的配置。
(2) 组件化
Vue的组件化功能可谓是它的一大亮点，通过将页面上某一组件的html、CSS、js代码放入一个.vue的文件中进行管理可以大大提高代码的维护性。
例如：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// App.vue
<template>
    <div class="box" v-text="note"></div>
</template>
<script>
export default {
    data () {
        return {
            note: '这是一个组件的html模板！'
        }
    }
}
</script>
<style scoped>
.box {
    color: #000;
}
</style>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我们还可以在组件里写一些预处理语言：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// App.vue
<template lang='jade'>
    div(class="box" v-text="text")
</template>
<script>
export default {
    data () {
        return {
            note: '这是一个组件的html模板！'
        }
    }
}
</script>
<style lang="stylus">
.box 
    color: #000
</style>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
当然这样写我们是需要通过webpack来进行打包的，推荐使用Webpack + vue-loader的方式，同时使用ES6语法，需要安装babel来进行转换。因为文章为浅谈Vue.js，所以这里不做深入介绍。
(3) 路由
和Angular一样，Vue也具有它的路由功能。通过路由功能，我们可以实现各个组件的按需加载，轻松构建单页应用。下面是一个简单的路由配置文件：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// router.js
'use strict'
export default function(router) {
    router.map({
        '/': {
            component: function (resolve) {
              require(['./components/Foo.vue'], resolve)
            }
        },
        '/foo': {
            component: function (resolve) {
              require(['./components/Foo.vue'], resolve)
            }
        },
        '/bar': {
            component: function (resolve) {
              require(['./components/Bar.vue'], resolve)
            }
        }
    })
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
如需查看具体的路由配置及使用，移步官方提供的文档：[http://vuejs.github.io/vue-router/zh-cn/index.html](http://vuejs.github.io/vue-router/zh-cn/index.html)
## 总结
> 
个人认为前端的一些技术都是融会贯通的，学习一门语言或者框架本身并不是为了学习它的技术，最重要的是学习它的思维，只有思维层面得到了延伸，学习其他技术的时候会发现得心应手。Vue带给我们的是前端一种解决问题的新的思维。
原创文章，转载请注明来自[一个萝卜一个坑 -博客园[http://www.cnblogs.com/luozhihao]](http://www.cnblogs.com/luozhihao/)
本文地址：[http://www.cnblogs.com/luozhihao/p/5329440.html](http://www.cnblogs.com/luozhihao/p/5329440.html)
本文同步发表于：[https://segmentfault.com/a/1190000004704498](https://segmentfault.com/a/1190000004704498)
