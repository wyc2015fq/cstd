# 在VUE2.0中使用PostCSS - weixin_33985507的博客 - CSDN博客
2016年11月04日 03:56:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
*作者：滴滴公共前端团队 - J*
在写 vue 组件样式时，有时为了兼容需要不停的写 -webkit-，即使复制粘贴也不能提升我们码 css 的幸福感，又有时我们想使用一下 **BEM** 风格的命名，但是长长的名称外加分隔符敲起来十分烦人。
其实在 vue-loader 中仅需要简单的配置即可*拯救我们*。
**vue-loader 中 PostCSS 配置**
vue-loader 中使用了 PostCSS 来处理 css，如下图所示：
上图为 vue-loader 9.x、webpack 1.x 中配置方法
**autoprefixer 使用**
对于 autoprefixer 功能来说，使用起来则更为方便了。当我们使用 vue-cli (最新版)创建好 vue 项目时，在 **build/webpack.base.conf.js** 中可以看到 vue-loader 的中已经默认加入了 autoprefixer 的配置，如下图所示：
我们仅需要做的是配置好 autoprefixer 中的选项。
vue-cli 工具默认给出的是 ’**last 2 versions**’，即**主流浏览器的最新两个版本。**
> 
对于移动端来说很多 css3 属性需要加 -webkit- 前缀来说，这个配置目前是**不够的**。
可以根据我们的需求增加，如‘**Android >= 4.0**’(安卓大于4.0版本)，’ChromeAndroid>5%’(chrome 和 android webview 全球统计有超过 5% 的使用率)等等。
例如我们使用如下配置
vue 组件中 css 如下：
编译出来后会得到如下结果：
如此就可以放心的不写 -webkit- 了。
**postcss-cssnext 使用**
1. 首先使用 **npm install postcss-cssnext** 安装插件。
2. 在 vue-loader 的 postcss 中加入 postcss-cssnext。
注意
> 
cssnext 插件中**已经包含了 autoprefixer** ，所以这里不用再引入autoprefixer。
配置完成后，来试一试 **cssnext** 功能。
vue 组件中 css 如下：
编译后：
**postcss-bem 使用**
使用 bem 风格来对 css 进行命名时，名称虽然清晰，但是长名称外加分割线，写起来略微不爽。
在 vue 中使用 postcss 命名时，可以使用 postcss-bem 来简化我们的书写，而且配置起来也十分方法。
1.安装 bem 的插件 **npm install saladcss-bem**
2.在 vue-loader 的 postcss 中引入 **postcss-bem**
配置完成后，就可以用简化的方式来书写 bem 风格的 css 了
vue 组件中 css 如下：
编译后结果：
补充：
github.com/vuejs/vue-l… vue-loader对PostCSS使用介绍
[autoprefixer.github.io/](https://link.juejin.im?target=https%3A%2F%2Fautoprefixer.github.io%2F)  autoprefixer在线编译，可以用来测试browers配置是否正确
[github.com/ai/browsers…](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fai%2Fbrowserslist%23queries)  browserlist 参数详解
[cssnext.io/usage/](https://link.juejin.im?target=http%3A%2F%2Fcssnext.io%2Fusage%2F)  cssnext使用
[github.com/saladcss/sa…](https://link.juejin.im?target=https%3A%2F%2Fgithub.com%2Fsaladcss%2Fsaladcss-bem)  posts-bem插件 
欢迎关注**DDFE**公众号
微信搜索公众号“DDFE”或扫描下面的二维码
