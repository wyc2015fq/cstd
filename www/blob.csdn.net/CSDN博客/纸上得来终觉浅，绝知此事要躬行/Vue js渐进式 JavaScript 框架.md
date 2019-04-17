# Vue.js渐进式 JavaScript 框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月05日 01:08:40[boonya](https://me.csdn.net/boonya)阅读数：1218








**目录**

[什么是Vue.js](#%E4%BB%80%E4%B9%88%E6%98%AFVue.js)

[概念](#%E6%A6%82%E5%BF%B5)

[资源](#%E8%B5%84%E6%BA%90%C2%A0)

[快速搭建框架](#%E5%BF%AB%E9%80%9F%E6%90%AD%E5%BB%BA%E6%A1%86%E6%9E%B6)

[开发环境](#%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83)

[实践](#%E5%AE%9E%E8%B7%B5)

## 什么是Vue.js

### 概念

中文教程：[http://doc.vue-js.com/](http://doc.vue-js.com/)

Vue.js（读音 /vjuː/, 类似于 **view**） 是一套构建用户界面的 **渐进式框架**。与其他重量级框架不同的是，Vue 采用自底向上增量开发的设计。Vue 的核心库只关注视图层，并且非常容易学习，非常容易与其它库或已有项目整合。另一方面，Vue 完全有能力驱动采用[单文件组件](http://doc.vue-js.com/v2/guide/single-file-components.html)和[Vue生态系统支持的库](http://github.com/vuejs/awesome-vue#libraries--plugins)开发的复杂单页应用。

Vue.js 的目标是通过尽可能简单的 API 实现**响应的数据绑定**和**组合的视图组件**。

如果你是有经验的前端开发者，想知道 Vue.js 与其它库/框架的区别，查看[对比其它框架](http://doc.vue-js.com/v2/guide/comparison.html)。

![](https://img-blog.csdnimg.cn/20181204225449161.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### 资源 

Github:[https://github.com/vuejs/vue](https://github.com/vuejs/vue)

Examples:[https://vuejs.org/v2/examples/](https://vuejs.org/v2/examples/)

## 快速搭建框架

### 开发环境

1.安装webpack
`npm install -g webpack`
2.安装vue-cli

vue-cli是什么？vue-cli 是vue.js的脚手架，用于自动生成vue.js模板工程的。

**安装vue-cli：**
`npm install -g vue-cli`
**使用vue-cli构建项目**
`vue init webpack your-project-name  //创建一个基于webpack模板的名为your-project-name的项目`
**目前可用的模板包括：**
- browserify –全功能的Browserify + vueify，包括热加载，静态检测，单元测试。
- browserify-simple–一个简易的Browserify + vueify，以便于快速开始。
- webpack–全功能的Webpack + vueify，包括热加载，静态检测，单元测试。
- webpack-simple–一个简易的Webpack + vueify，以便于快速开始。

**安装项目依赖：**

```
cd your-project-name    //进入项目目录
npm install        //安装项目依赖
npm run dev        //运行项目
npm run build      //发布项目
```



   此时在浏览器打开：localhost:8080即可看到欢迎页。

3.安装element-ui

[http://element-cn.eleme.io/#/zh-CN](http://element-cn.eleme.io/#/zh-CN)

Element，一套为开发者、设计师和产品经理准备的基于 Vue 2.0 的桌面端组件库.

**安装element-ui:**
`npm install element-ui -S`
**添加引用配置：**

在main.js文件中加入

```
import ElementUI from 'element-ui'
import 'element-ui/lib/theme-chalk/index.css'


Vue.use(ElementUI)
```

4.修改App.vue

```
<template>
  <div id="app">
    <img src="./assets/logo.png">
    <h1>{{ msg }}</h1>
    <el-button @click.native="startHacking">Let's start Vue element-ui</el-button>
    <div class="block">
    <span class="demonstration">显示默认颜色</span>
    <span class="wrapper">
    <el-button type="success">成功按钮</el-button>
    <el-button type="warning">警告按钮</el-button>
    <el-button type="danger">危险按钮</el-button>
    <el-button type="info">信息按钮</el-button>
    </span>
    </div>
    <br/>
    <div class="block">
      <span class="demonstration">hover 显示颜色</span>
    <span class="wrapper">
    <el-button :plain="true" type="success">成功按钮</el-button>
    <el-button :plain="true" type="warning">警告按钮</el-button>
    <el-button :plain="true" type="danger">危险按钮</el-button>
    <el-button :plain="true" type="info">信息按钮</el-button>
    </span>
    </div>
  </div>
</template>
<script>
export default {
  data () {
    return {
      msg: 'Use Vue 2.0 Today!'
    }
  },

  methods: {
    startHacking () {
      this.$notify({
        title: 'It Works',
        message: 'We have laid the groundwork for you. Now it\'s your time to build something epic!',
        duration: 6000
      })
    }
  }
}
</script>

<style>
body {
  font-family: Helvetica, sans-serif;
}
</style>
```

如果显示中文乱码，请将App.vue文件另存为选择UTF-8保存。

参考[https://www.cnblogs.com/dwj88/p/7606224.html](https://www.cnblogs.com/dwj88/p/7606224.html)效果：

![](https://img-blog.csdnimg.cn/20181205010756181.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

### 实践

参考github地址：[https://github.com/yqrong/vvproject](https://github.com/yqrong/vvproject)

![](https://img-blog.csdnimg.cn/20181205011722117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181205011435417.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181205011531680.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20181205011607438.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181205010617920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

下载地址：[https://pan.baidu.com/s/1C7a24Lw9xVPq9rAZHpmQVw](https://pan.baidu.com/s/1C7a24Lw9xVPq9rAZHpmQVw)





