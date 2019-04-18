# Vue组件 - weixin_33985507的博客 - CSDN博客
2018年12月06日 15:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
组件（Component）是Vue.js 最强大的功能之一。
组件可以扩展出HTMl元素，封装可重用的代码。是可复用的 Vue 实例。
# 组件的注册
全局注册
```
// html 代码
<div id="app">
  <my-component></my-component>
</div>
// js 代码
Vue.component('my-component', {
  template: '<div>A component!</div>'
})
var app = new Vue({
  el: '#app',
  data: {
       
  } 
});
```
局部注册
```
// html 代码
<div id="app">
  <my-component></my-component>
</div>
// js 代码
// 组件中的data必须是个函数
var Child = {
  template: '<div>A component!</div>',
  data: function() {
      return {
            name: "gao",
      }
}};
new Vue({
  // ...
  components: {
    // <my-component> 将只在父组件模板中可用
    'my-component': Child
  }
})
```
组件中的 data methods
```
// js 代码
Vue.component('child', {
    template: `<div><button @click="on_click()">{{msg}}</button></div>`,
    data: function () {
        return {
            msg: "点我",
        }
    },
    methods: {
        on_click(){
            alert(123)
        }
    }
});
new Vue({
    el: "#app",
})
```
子组件的注册
```
<script>
    var my_component = {
        template: `<div><h1>{{msg}}</h1></div>`,
        data(){
            return{
                msg: "这是子组件"
            }
        }
    };
    var global_component = {
        template: `<div>
                        <h1>{{msg}}</h1>
                        <button @click="on_click">点我</button>
                        <my_component></my_component>
                    </div>`,
        data(){
            return {
                msg: "全局组件"
            }
        },
        methods: {
            on_click() {
                alert("123")
            }
        },
        components:{
            my_component:my_component,
        }
    };
    const app = new Vue({
        el: "#app",
        data: {
        },
        components: {
            global_component: global_component,
            // my_component: my_component,
        }
    });
</script>
```
# 组件之间的通信
父子组件通信
```
// html 代码
<div id="app">
    <child username="gaoxin"></child>
</div>
// js 代码
Vue.component('child', {
    template: `<a :href="'/user/'+ username">{{username}}</a>`,
    props: ["username"],
});
var app = new Vue({
    el: "#app",
    data:{
        name: "@gaoxin"
    }
});
```
app.emit触发，回调函数接收事件触发器额外参数。
app.$emit(event, [args....])  触发当前实例上的事件，额外参数传给监听器的callback回调函数。
子父组件通信
```
// html 代码
<div id="app">
    <parent></parent>
</div>
// js 代码
Vue.component('parent',{
    template: `
        <div>
            <child @show_balance="show"></child>
            <p v-if="active">您的余额998</p>
        </div>
    `,
    data: function () {
        return {
            active: false,
        }
    },
    methods: {
        show: function(data){
            this.active=true;
            console.log(data)
        }
    }
});
Vue.component('child', {
    template: `<div><button @click="on_click()">{{msg}}</button></div>`,
    data: function () {
        return {
            msg: "显示余额",
        }
    },
    methods: {
        on_click(){
            // alert(123)
            this.$emit('show_balance', {q:1,b:2})
        }
    }
});
```
平行组件之间的通信，汉化需要一个中间调度器，在组件加载完成之后去监听调度器时间，回调函数接受数据。
```
// html 代码
<div id="app">
    <whh></whh>
    <shh></shh>
</div>
// js 代码
var Event = new Vue()
Vue.component('whh',{
    template: `
        <div>
            我说: <input @keyup="on_change" v-model="i_said">
        </div>
    `,
    data: function () {
        return {
            i_said: '',
        }
    },
    methods: {
        on_change: function () {
            Event.$emit("whh_said_something", this.i_said)
        }
    }
});
Vue.component('shh', {
    template: `
        <div>
            花花说:{{whh_said}}
        </div>
    `,
    data: function () {
        return {
            whh_said: '',
        }
    },
    mounted: function () {
        var me = this
        Event.$on('whh_said_something', function (data) {
            me.whh_said = data
        })
    }
});
```
# 混合Mixins
重复功能和数据的存储器，可以覆盖Mixin是的内容。
Mixins
```
// 点击显示和隐藏  提示框的显示和隐藏
// html 代码
<div id="app">
    <PopUp></PopUp>
    <ToolTip></ToolTip>
</div>
// js 代码
var base = {
     data: function () {
        return {
            visible: false,
        }
    },
    methods: {
        show: function () {
            this.visible = true
        },
        hide: function () {
            this.visible = false
        }
    }
}
Vue.component('popup', {
    template:`
        <div>
        <button @click="show">PopUp show</button>
        <button @click="hide">PopUp hide</button>
        <div v-if="visible"><p>hello everybody</p></div>
        </div>
    `,
    mixins: [base],
    data: function () {
        return {
            visible: true,
        }
    }
});
Vue.component('tooltip', {
    template: `
        <div>
        <div @mouseenter="show" @mouseleave="hide">ToolTip</div>
        <div v-if="visible"><p>ToolTip</p></div>
        </div>
    `,
    mixins: [base]
});
new Vue({
    el: "#app",
})
```
# 插槽 Slot
插槽是一套内容分发的 API，在组件中，<slot>作为 内容承载分发的出口
```
// html 代码
<div id="app">
    <panel>
        <div slot="title"> HELLO</div>
        <div slot="content">hello</div>
        
    </panel>
    <panel></panel>
    <panel></panel>
</div>
<template id="panel-tpl">
    <div class="panel">
        <div class="title">
            <slot name="title"></slot>
        </div>
        <div class="content">
            <slot name="content"></slot>
        </div>
        <!--<div class="content">Failure is probably the fortification in your pole. It is like a peek your wallet as the thief, when you are thinking how to spend several hard-won lepta,</div>-->
        <div class="footer">
            <slot name="footer">更多信息</slot>
        </div>
    </div>
</template>
// js 代码
Vue.component('panel', {
    template: '#panel-tpl',
});
new Vue({
    el: "#app",
})
```
