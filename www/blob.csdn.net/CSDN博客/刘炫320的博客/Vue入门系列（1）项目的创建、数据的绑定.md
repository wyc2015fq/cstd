# Vue入门系列（1）项目的创建、数据的绑定 - 刘炫320的博客 - CSDN博客
2019年01月02日 21:35:20[刘炫320](https://me.csdn.net/qq_35082030)阅读数：91
Vue也是做微信小程序时得来的兴趣。因为微信小程序的前端做的确实太便捷。深入了解后，发现Vue就是长这样子，并且只有略微的不同。因此打算先入手Vue练练手。
# 1.创建简单的实验项目
和往常一样，我们只需要在项目文件夹中建立三个文件:
- index.css //样式文件
- index.html //主文件
- index.js //页面逻辑文件
这样一个简单的项目架构就做成了。我们只需要在html文件中写出我们正常的展示代码：
```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8" />
    <title>Hello World</title>
    <!-- 这个Vue的引用一定要在页面的最前面 -->
    <script src="https://cdn.jsdelivr.net/npm/vue/dist/vue.js"></script>
</head>
<body>
	<!-- 目标id -->
    <div id="app">
    	<!-- 最熟悉的数据绑定的大括号在此 -->
        <p>{{ message }}</p>
    </div>
    <!--而自己的这个引用要在页面的最后面 -->
    <script src="index.js"></script> 
</body>
</html>
```
在js文件中写我们的逻辑代码：
```
var app=new Vue({
	/*el是元素id,同html界面中一致，不要忘记#*/
    el: '#app',
    /*这里的data不仅仅是个成员，而是数据绑定，你可以直接引用出它的成员*/
    data: {
      message: 'Hello Vue.js!'
    }
  })
 /*直接修改数据*/
app.message="Hello World!"
```
至此，简单的Vue项目就搭建完成了，内部包含了一些细节介绍，使用过微信小程序的人对此都不会陌生。但是这里我们希望能讨论的更加细致一些。
# 2. 数据绑定
## 2.1 简单插值
所谓的简单插值，就是使用大括号在html页面内嵌我们js中的变量，从而使得前端界面“千人千面”。
各种插值方法总结如下：
```
<!-- 1.简单插值 -->
<span>Message: {{ msg }}</span>
<!-- 2.一次性赋值-->
<span v-once>这个将不会改变: {{ msg }}</span>
<!-- 3. 真正嵌入html代码 -->
<p>Using v-html directive: <span v-html="rawHtml"></span></p>
<!-- 4. 绑定元素属性(id、href等) -->
<div v-bind:id="dynamicId"></div>
<!--5. 表达式绑定-->
	<!-- a.普通运算-->
	{{ number + 1 }}
	<!-- b.选择运算-->
	{{ ok ? 'YES' : 'NO' }}
	<!-- c.函数运算-->
	{{ message.split('').reverse().join('') }}
	<!-- d.元素属性运算-->
	<div v-bind:id="'list-' + id"></div>
<!-- 6. 判断语句-->
<p v-if="seen">现在你看到我了</p>
<!-- 7. 控制语句-->
<li v-for="item in items">
    {{ item.message }}
  </li>
<!-- 8. 指令-->
<a v-on:click="doSomething">...</a>
```
## 2.2 计算属性与侦听器
为了方便数据绑定，当一个数据属性需要计算获得时（求和，翻转等操作），可以使用计算属性。
```
<!-- html里面的内容 -->
<div id="example">
  <p>Original message: "{{ message }}"</p>
  <p>Computed reversed message: "{{ reversedMessage }}"</p>
</div>
```
```
//js里面的内容
var vm = new Vue({
  el: '#example',
  data: {
    message: 'Hello'
  },
  computed: {
    // 计算属性的 getter
    reversedMessage: function () {
      // `this` 指向 vm 实例
      return this.message.split('').reverse().join('')
    }
  }
})
```
上面就是一个计算属性使用的例子，看起来是比较简单的，其实它就是一个getter，setter的封装，默认使用getter接口，如果你想用setter，自行添加即可。
```
// ...
computed: {
  fullName: {
    // getter
    get: function () {
      return this.firstName + ' ' + this.lastName
    },
    // setter
    set: function (newValue) {
      var names = newValue.split(' ')
      this.firstName = names[0]
      this.lastName = names[names.length - 1]
    }
  }
}
// ...
```
那么我们可以看到，其实计算属性只是把表达式放到js文件中了，其实还是可以直接在页面上嵌入表达式，它们之间的区别在于，计算属性会有一个计算缓存，即如果参与计算的属性都没有发生值得变化，那么不会重复计算，尤其是一些大数据量的统计，使用计算变量可以极大减少运算负担。
侦听器主要是监听元素的变化后的结果，就相当于传统意义上的事件。主要实现方式有两种：
```
<!-- 第一种，传统的侦听器在内部-->
var watchExampleVM = new Vue({
  el: '#watch-example',
  data: {
    question: '',
    answer: 'I cannot give you an answer until you ask a question!'
  },
  watch: {
    // 如果 `question` 发生改变，这个函数就会运行
    question: function (newQuestion, oldQuestion) {
      this.answer = 'Waiting for you to stop typing...'
      this.debouncedGetAnswer()
    }
  }
})
<!-- 第二种，传统的侦听器在外部-->
// 键路径
vm.$watch('a.b.c', function (newVal, oldVal) {
  // 做点什么
})
// 函数
vm.$watch(
  function () {
    return this.a + this.b
  },
  function (newVal, oldVal) {
    // 做点什么
  }
)
```
另外提及一点：不要过量的使用侦听器，如果只是自己的属性随着自己的属性而改变，优先考虑结果变量，也就是使用计算属性。
# 3. 小结
我们这次主要学习的是Vue的样子，可以说是初次见面，还是请多多指教。在内容上，我们主要搭建了一个建议的Vue应用，并简要的介绍了页面创建与数据绑定的一些知识，其他知识下回分解。我们想要通过Vue学习的是整个前端的思考方式和方法，尽管有很多想法5-10年前就已经在我们每个编程人员心中产生，但是知道最近，才将想法落到实地。最后献上Vue的生命周期图，更多详情还请参阅[官方网站](https://cn.vuejs.org/v2/guide/computed.html)。
![在这里插入图片描述](https://cn.vuejs.org/images/lifecycle.png)
