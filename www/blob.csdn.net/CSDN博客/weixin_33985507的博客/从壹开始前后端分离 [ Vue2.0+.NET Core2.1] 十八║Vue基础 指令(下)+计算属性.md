# 从壹开始前后端分离 [ Vue2.0+.NET Core2.1] 十八║Vue基础: 指令(下)+计算属性 - weixin_33985507的博客 - CSDN博客
2018年09月07日 23:44:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
# 回顾
今天来晚辣，给公司做了一个小项目，一个瀑布流+动态视频控制的DEMO，有需要的可以联系我，公司的项目就不对外展示了（一个后端程序员真的要干前端了哈哈哈）。
> 
书接上文，昨天正式的开始了Vue的代码的学习，简单的通过一些假的数据来展示了下个人博客的首页列表，不知道大家是否还记得昨天讲的什么，如果不太清楚呢，可以再回顾下《[从壹开始前后端分离 [ Vue2.0+.NET Core2.1] 十七 ║Vue基础：使用Vue.js 来画博客首页（一）](https://www.jianshu.com/p/067493c96a53)》，我们主要说到了，Vue的核心语法是什么，MVVM体现在哪些方面，如何简单安装Vue环境（通过直接引用Vue.js文件的形式），以及常用的十天指令的前五个，并且通过讲解，咱们做了一个小DEMO，就是一个个人的博客系统的首页（这里是盗取网友的一个样式，因为我看着着实比较喜欢哈哈）。其实主要记住一点，用Vue这类MVVM框架开发，一定要摆脱之前的DOM操作的习惯，改成操作数据来控制View，如果你Vue这一块会的话，那学习微信小程序开发就是分分钟的事儿，嗯~~~
关于后边的实战环节，我还没有确定要用什么样式的博客，如果大家看到有好的，可以留言下，咱们以它为基础可以扩展，如果不行的话，我就自己写一个简单的吧，当然还是那句话，我只是一个抛砖引玉的作用，也给看到这篇文章的小伙伴们一丢丢的动力，这个时候要说下QQ群里的小伙伴，都已经开始用Vue，配合着前边的教程和自己的.Net Core项目进行开发页面了，我感觉也是很开心的！至少可以在平时的时间，一起学点儿东西也是不错的。虽然不能手把手吧，但是一些建议还是尽量给提问题的小伙伴的。哈哈，{{ 强硬收回话题 }}，今天我们接着上一篇的内容，继续往下走，主要是：把 **基本指令 **介绍完，说下**计算属性和侦听器**，**Class 与 Style 绑定**，主要是这三部分，在博客页面上设计 添加文章，删除文章，筛选文章等功能。
# 零、今天完成左下角浅蓝色的部分
![13652172-ed1619e08ea32ea4.png](https://upload-images.jianshu.io/upload_images/13652172-ed1619e08ea32ea4.png)
image
# 一、VUE 常用的一些指令总结 （ 下 ）
### **1、补充下 v-once 指令 —— 禁止修改**
看名字就可以知道，这个是 一次 的意思，也就是说在第一次渲染以后，后期的无论数据的如何修改都不会影响该节点，只渲染元素和组件一次。随后的重新渲染，元素/组件及其所有的子节点将被视为静态内容并跳过。这可以用于优化更新性能。
注意：但是，我想说的是，这个指令一般情况不要使用，除非是含有大量的静态数据，不想每次加载的时候占用时间，如果过多的使用该指令会出现很多意想不到的问题，因为数据的不刷新，不适合刚入门的小伙伴使用。
### **2、v-bind （ : ）指令 —— 动态属性**
在昨天的博客首页的联系中，我们其实已经用到了这个指令，大家应该没有注意到，就是文章列表陪着 href 属性的时候。
`v-bind` 指令的作用和用法，它用于动态绑定DOM元素的属性，比较常见的比如：<a>标签的href属性，<img>标签的src属性。
```
<ul class="post-list non-style-list">
                <li v-for='item in list' class="post-list-item">
                    <!--这里用到了 v-bind 指令-->
                    <a v-bind:href="'https://www.cnblogs.com/laozhang-is-phi/p/'+ item.id +'.html'">{{item.name}}</a>
                    <!--还可以这样写 这里的linkUrl 是一个变量-->
                    <a :href="linkUrl">{{item.name}}</a>
                    <span class="post-list-date">({{item.date}})</span>
                </li>
            </ul>
```
还可以：动态地绑定一个或多个特性，或一个组件 prop 到表达式（这里要记得，是**动态的绑定，就是指在特性中存在变量**）。
```
<!-- 绑定一个有属性的对象 -->
<div v-bind="{ id: someProp, 'other-attr': otherProp }"></div>
<!-- 通过 prop 修饰符绑定 DOM 属性 -->
<div v-bind:text-content.prop="text"></div>
```
### **3、v-on （ @ ）指令——事件触发**
绑定事件监听器。事件类型由参数指定。表达式可以是一个方法的名字或一个内联语句，如果没有修饰符也可以省略。
用在普通元素上时，只能监听[原生 DOM 事件](https://developer.mozilla.org/zh-CN/docs/Web/Events)。用在自定义元素组件上时，也可以监听子组件触发的自定义事件。相当于绑定事件的监听器，绑定的事件触发了，可以指定事件的处理函数。
我们可以简单说个栗子，在我们的博客首页的头像上，增加一个点击事件（就是之前的click事件），
```
<div  v-on:click="alert('我是老张的哲学的头像')">  </div>
//注意 v-on 可以用@代替，比如
//<div  @:click="alert('我是老张的哲学的头像')">  </div>
```
在我们的页面里，我们可以用来触发：添加、筛选功能
**a、**新建一个 input 标签，添加一个 回车 事件
```
<input @keydown.enter="addArticle" type="text" class="edit" placeholder="按回车添加文章">
```
**b、**在 vue 实例的 methods 中，统一添加我们的 addArticle 方法。
```
//我们的方法都统一写到这里
 methods: { //添加事件
 addArticle: function () { //将文章存入list数组，注意 this指向！ //将数据反转
                    this.list = this.list.reverse(); this.list.push(this.task); this.list = this.list.reverse(); //存入list[]后，重置task
                    this.task = {
                        name: '',//内容为空
                        id: 1000,
                        date: " Just Now ",
                        finished: false,//未完成
                        deleted: false//未删除
 }
                }
            },
```
**c、**这个时候，差最后一步，就是获取 input 的指（这个时候可千万不要再像以前那样，根据id来获取结果了）
还记得咱们前几帐将基本语法和Vue的核心功能的时候，说到了其中的一个很大的特性就是数据驱动 —— 双向数据绑定，不仅我们可以给 Data 赋值，还可以通过在 DOM 操作的时候，将指获取到 Data，没错就是下边的这个指令，v-model。
****4、v-model 指令 —— 双向数据绑定****
这是一个我认为很重要，也是经常使用到的指令，主要是表单操作，它可以很容易的实现表单控件和数据的双向绑定，相对以前的手动更新DOM，这个上边也说到了。
在之前的 input 输入框中，添加 v-model 指令
```
<input @keydown.enter="addArticle" type="text" class="edit" v-model="task.name" placeholder="按回车添加文章">
```
这个时候，我们的博客添加的功能就好了（当然现在是最低端最low的，只是为了讲解 v-model 指令用，博客添加到时候会用 express 后台管理）。
![13652172-c292f84e933cf741.gif](https://upload-images.jianshu.io/upload_images/13652172-c292f84e933cf741.gif)
image
**好啦，常用的 vue 指令已经讲解完成，还有其他的一些不常用的几个大家可以用到的时候了解下。**
# 二、计算属性 Computed
### **1、计算属性的原理**
在模板内使用表达式很便利，但是设计它们的初衷是用于简单运算的。在模板中放入太多的逻辑会让模板过重且难以维护。例如：
```
<div id="example"> {{ message.split('').reverse().join('') }} </div>
```
在这个地方，模板不再是简单的声明式逻辑。你必须看一段时间才能意识到，这里是想要显示变量 `message` 的翻转字符串。当你想要在模板中多次引用此处的翻转字符串时，就会更加难以处理，然后如果大量的使用这样的表达式，会使得整个页面不仅不好看，还很繁重。
所以，对于任何复杂逻辑，你都应当使用计算属性。
就比如上边的栗子，我们就可以写成这样：
```
<div id="example">
    //1、这里是我们在 computed中定义的值，而不是在data中
  <p>message: "{{ reversedMessage }}"</p>
</div>
var vm = new Vue({
  el: '#example',
  data: {
    message: 'Hello' },
  computed: { // 计算属性的 getter
 reversedMessage: function () { // 注意 ！`this` 指向 vm 实例
      return this.message.split('').reverse().join('')
    }
  }
})
```
这样看起来就清晰明了，减轻页面的负重。这里你可以会好奇，这就像一个data的中间件一样，不用做任何的其他操作，都可以实现这个逻辑，就好像data的影子一样，没错！计算属性就是一个getter器。
你可以像绑定普通属性一样在模板中绑定计算属性。Vue 知道 `vm.reversedMessage` 依赖于 `vm.message`，因此当 `vm.message` 发生改变时，所有依赖 `vm.reversedMessage` 的绑定也会更新。而且最妙的是我们已经以声明的方式创建了这种依赖关系：计算属性的 getter 函数是没有副作用 (side effect) 的，这使它更易于测试和理解。
### **2、知道了他的原理和如何使用，那么我们就可以在我们的项目中使用 计算属性 来达到我们的动态查询文章的功能**
我们首先添加一个计算属性来过滤我们的文章list数据
```
//通过计算属性过滤数据
 computed: {
                listSearch: function () { //为什么要存这个this呢，因为filter过滤器会改变this的指向
                    let that = this; return this.list.filter(function (item) { //简单的 判断文章name是否包含 input中的值，因为双向绑定，所以也就是 task.name
                        return item.name.indexOf(that.$data.task.name) >= 0;
                    });
                }
            }
```
接下来，我们就需要把我们的计算属性 listSearch 替换掉view中的 list，从而达到过滤：
```
<li v-for='item in listSearch' class="post-list-item">
      <span class="post-list-date">({{item.date}})</span>
  </li>
```
最后我们可以看看效果：
![13652172-e9164fcbb646f3ae.gif](https://upload-images.jianshu.io/upload_images/13652172-e9164fcbb646f3ae.gif)
image
注意：计算属性默认只有 getter ，不过在需要时你也可以提供一个 setter ：
```
computed: {
  fullName: { // getter
    get: function () { return this.firstName + ' ' + this.lastName
    }, // setter
    set: function (newValue) { var names = newValue.split(' ') this.firstName = names[0] this.lastName = names[names.length - 1]
    }
  }
}
```
现在再运行 `vm.fullName = '老张 哲学'` 时，setter 会被调用，`vm.firstName` 和 `vm.lastName` 也会相应地被更新。
# 三、侦听器 （不建议多使用）
虽然计算属性在大多数情况下更合适，但有时也需要一个自定义的侦听器。这就是为什么 Vue 通过 `watch` 选项提供了一个更通用的方法，来响应数据的变化。当需要在数据变化时**执行异步或开销较大的操作**时，这个方式是最有用的（**这个要强调，是异步操作，或者开销较大的操作**）。
在这里，我们监听下我们的 input 输入的数据变化，也就是 task.name 的值
一般的写法是这样的：
```
new Vue({
  data: {
   author: "老张的哲学",
    task: {
                    name: '',//内容为空
                    id: 100,
                    date: " Just Now ",
                    finished: false,//未完成
                    deleted: false//未删除
 },
  },
  watch: {
    author: function (newval, oldVal) {
      console.log("author 变化辣，")
    }
  }
})
```
但是在我们的栗子中，是监听一个对象的中某个属性，也就是 task.name，
所以我们就会这么写：
```
watch: {
     task.name() { //这里面可以执行一旦监听的值发生变化你想做的操作
 }
 },
```
但是，这样写是不符合规则的，必须是一个变量，因此会报错：
![13652172-3c04bf487bb9d8c1.png](https://upload-images.jianshu.io/upload_images/13652172-3c04bf487bb9d8c1.png)
image
所以我们就需要还是用到 计算属性来定义，还记得计算属性是干什么的么，它就像一个数据的中间件，把原始数据再封装一下，
那正好，我们可以把 task.name 给封装下，最终会是这样的：
```
new Vue({
  data: {
   author: "老张的哲学",
    task: {
            name: '',//内容为空
            id: 100,
            date: " Just Now ",
            finished: false,//未完成
            deleted: false//未删除
 },
  },
  watch: {
    author: function (newval, oldVal) {
      console.log("author 变化辣，")
    }，
    nameCpt() : function (newval, oldVal) {
      console.log("task.name 变化辣，")
    }，
  }, //通过计算属性来操作我们需要用到的任何数据
 computed: {
      nameCpt() { return this.task.name
      }
  }
})
```
注意：虽然Vue 提供了一种更通用的方式来观察和响应 Vue 实例上的数据变动：侦听属性。`但是`监听是特别浪费资源的，当我们有一些数据需要随着其它数据变动而变动时，我们很容易滥用 `watch，`因此通常更好的做法是使用计算属性而不是命令式的 `watch` 回调。
# 四、匆匆结语
今天时间晚了些，动态Class 与 Style 绑定没有说到，那我们就下次再说吧！今天呢，我们主要说了常用的指令，主要的是 v-model、v-bind、v-on三个指令，然后还说了计算属性和侦听器，我在开发的过程中，计算属性是使用较多的，但是某些时候，watch 侦听器会发挥不一样的作用！好啦，下次咱们继续说说 **动态Class 与 Style 绑定 **和 很重要的 **生命周期讲解 **吧**。**
# 五、CODE
[https://github.com/anjoy8/Blog.Vue](http://www.cnblogs.com/%20%20https://github.com/anjoy8/Blog.Vue)
QQ群：
867095512     (blod.core)
