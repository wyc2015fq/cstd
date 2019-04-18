# 在 Vue 中是使用插槽 - weixin_33985507的博客 - CSDN博客
2019年02月11日 15:28:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 属性传值
先写一段简单的代码
```
<div id="root">
    <child content="<p>Dell</p>"></child>
</div>
Vue.component('child', {
    props: {
        content:String
    },
    template: `<div>
                <p>hello</p>
                <div v-html="this.content"></div>
              </div>`,
})
let vm = new Vue({
    el: '#root'
})
```
这种写法有两个问题：
- `p`标签外面会有一层`div`，这个`div`是无法去掉的，有人会想，能不能用`template`代替，其实是不可以的，在这里模版站位符是不能用的。
- 如果`content`传递的内容很多，代码就会变得很难阅读。
当我的子组件有一部分内容，是根据父组件传递过来的`dom`进行显示的话，这时候可以换一种语法。
## 插槽`slot`
```
<div id="root">
    <child>
        <p>Dell</p>     //这种语法看起来非常像，用子组件，我向里插入内容
    </child>
</div>
Vue.component('child', {
    props: {
        content:String
    },
    template: `<div>
                <p>hello</p>
                <slot></slot>       //slot 标签显示的内容就是父组件向子组件插入进来的内容
              </div>`,
})
let vm = new Vue({
    el: '#root'
})
```
` <p>Dell</p>`这种语法看起来非常像，用子组我向里插入内容，所以我们把它叫做插槽。
`slot`标签显示的内容就是父组件向子组件插入进来的内容。
通过插槽可以更方便的向子组件传递`dom`元素，同时子组件只需通过`slot`来使用就可以了。
## `slot`其他功能
如果子组件里没有`dom`元素，可以让它显示默认内容，如下：
`<slot>默认内容</slot>`
## 具名插槽
如果现在有个需求是，`header`和`footer`是由外部引入的该怎么弄呢？如下
```
<div id="root">
    <body-content>
        <div class="header" slot="header">header</div>
        <div class="footer" slot="footer">footer</div>
    </body-content>
</div>
Vue.component('body-content', {
    props: {
        content:String
    },
    template: `<div>
                <slot name="header"></slot>
                <div class="content">content</p>
                <slot name="footer"></slot>
              </div>`,
})
let vm = new Vue({
    el: '#root'
})
```
`slot`标签`name`属性对应的是组件中`slot`属性，通过这种写法，可以在调用子组件时，一次性传递多个区域的`dom`结构，在子组件里通过具名插槽来分别使用不同部分的`dom`结构
