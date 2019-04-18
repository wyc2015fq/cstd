# React+Router4 实战教程-1.React 简介 - weixin_33985507的博客 - CSDN博客
2018年03月28日 14:48:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
# ReactJS介绍
## 简介
ReactJS是为了解决**构建随着时间数据不断变化的大规模应用程序**而设计的**用来构建用户界面的JavaScript库**，**是MVC中的V（视图）**
## React设计思想
> 
React设计思想很独特，属于革命性创新，性能独特，代码逻辑却非常简单。很有可能会成为将来Web开发的主流工具。这个项目本身也越来越大，**从最早的UI引擎变成了一整套前后端通吃的Web App解决方案。**其衍生出来的**React Native项目，希望用Web APP的方式去native app，从而实现同一组人只需写一次UI，就能同时运行在服务器，浏览器和手机。随着在2015年发布了Android版本，把web和原声平台的js开发技术扩展到了Android平台，以后很有可能出现同一组人只需要写一次UI，就能运行在三端（service，mobile，browser）**
React两个主要思想：简单，表达能力
### 简单
仅仅只要表达出你的应用程序在任一个时间点应该长的样子，然后当底层的数据变了，React 会自动处理所有用户界面的更新
### 表达能力
当数据变化了，React 概念上是类似点击了更新的按钮，但仅会更新变化的部分。
## 注意事项：
- React 不是一个完整的MVC框架，最多可以认为是MVC中View；
- React的服务器端Render能力只能算是一个锦上添花的功能，不是其核心出发点
- React不是一个新的模板语言，JSX只是一个表象，没有JSX的React也能工作。
# React特性与原理
## 特性
- 采用单向数据流，易追踪数据
- 将HTML，JS结合起来构建组件，组件状态和结果意义对应起来，实现模块化开发
- 创建了高性能的**虚拟DOM**，避免直接操作DOM。即组件不是真实的DOM节点，而是存在于内存中的一种数据结构。所有的DOM变动都先在虚拟DOM上发生，然后再将实际发生变动的部分反映在真实DOM上。（**DOM diff算法**）
## 原理
### 旧模式问题
> 
Web开发中总需要将变化的数据实时反映到UI上，就需要对DOM进行操作。旧模式下**频繁or复杂的DOM操作通常是性能瓶颈产生的原因**===>>**如何高性能地进行复杂DOM操作也成为衡量一个Web前端开发人员的重要指标。**，React则是直接改造了，在浏览器端用JavaScript实现了一套DOM AP，引入了一套虚拟DOM的机制。
### 虚拟DOM（react 精髓）
虚拟DOM的精髓所在有两点：性能和抽象
- 
通过js对象模拟原生DOM（js对象性能比DOM性能高），再加上DOM diff算法，极大地提升了DOM操作的性能。
> 
所有的DOM构造都是通过虚拟DOM进行，每当数据变化时，React都会重新构建整个DOM树，然后React将当前整个DOM树和上一次的DOM树进行对比，得到DOM结构的区别，然后仅仅将需要变化的部分进行实际的浏览器DOM更新;
React**能够批处理虚拟DOM的刷新**，在一个事件循环（Event Loop）内的两次数据变化会被合并，例如你连续的先将节点内容从A变成B，然后又从B变成A，React会认为UI不发生任何变化，而如果通过手动控制，这种逻辑通常是极其复杂的。
**每一次都需要构造完整的虚拟DOM树**，但是因为虚拟DOM是内存数据，性能是极高的，而**对实际DOM进行操作的仅仅是Diff部分**，因而能达到提高性能的目的。这样，在保证性能的同时，开发者将不再需要关注某个数据的变化如何更新到一个或多个具体的DOM元素，而只需要关心在任意一个数据状态下，整个界面是如何Render的。
- 虚拟DOM最大的意义是**对DOM进行了一层抽象**，而且这个抽象在我们使用React时，感觉不是特别明显，毕竟我们再js中写的标签跟原生的没有什么区别，并且最终都被渲染成了DOM对象。**这也恰恰说明react抽象了DOM的具体实现**。在浏览器中，虚拟DOM最终编译成了DOM；在IOS中虚拟DOM完全可以编译成oc中的组件，在Android中完全可以编译成Android对应的UI组件。
![11285338-3fc74bea4d52ba04](https://upload-images.jianshu.io/upload_images/11285338-3fc74bea4d52ba04)
这里写图片描述
> - JS业务逻辑代码完全可以借助Node移植到任意平台上
- 虚拟DOM在不同平台上的抽象有不同实现，不能完全移植，但是用法基本一致
- 最底层的UI调用我们就不用关心了
**以上这些也是React Native的实现，这是一次学习，到处编写的体验**
# JSX语法
JSX 语法就是：**HTML 语言直接写在 JavaScript 语言之中，不加任何引号，它允许 HTML 与 JavaScript 的混写**
> 
**注意：在使用webstorm的时候，js要做下修改才不会报错，setting里的language 里的js要选成 JSX Harmony**
## 语法规则
**遇到 HTML 标签（以 < 开头），就用 HTML 规则解析；遇到代码块（以 { 开头），就用 JavaScript 规则解析，允许在模板中插入变量：**
```
ReactDOM.render(
      <h1>Hello, world!</h1>,
      document.getElementById('example')
    );
```
# ReactJS组件
React **都是关于构建可复用的组件**。事实上，通过 React 唯一要做的事情就是构建组件。得益于其良好的封装性，组件使代码复用、测试和关注分离，更加简单。====》》》**这也就引出了组件化开发**
## 基本组件例子
React 允许将代码封装成组件，然后像插入普通 HTML 标签一样，在网页中插入这个组件：
```
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>demo4</title>
    <script type="text/javascript" src="../build/react.js"></script>
    <script type="text/javascript" src="../build/react-dom.js"></script>
    <script type="text/javascript" src="../build/browser.min.js"></script>
</head>
<body>
 <div id="example"></div>
 <script type="text/babel">
    //组件类的第一个字母需要大写，否则会报错
    var HelloMessage = React.createClass({
        //所有组件必须有自己的render方法，用于输入组件
        render : function(){
        return <div><h1>I am {this.props.name}</h1><h2>My age is:{this.props.age}</h2></div>;
        }
    });
    ReactDOM.render(
        <HelloMessage name="sqliang" age="23"/>,
        document.getElementById('example')
    );
 </script>
</body>
</html>
```
## 组件化
React推荐以组件的方式去重新思考UI构成，将UI上每一个功能相对独立的模块定义成组件，然后将小的组件通过组合或者嵌套的方式构成大的组件，最终完成整体UI的构建。**MVC让我们实现了表现，数据，控制的分离，开发中从技术的角度对UI进行了拆分；React启发开发者从功能的角度出发，将UI分成不同的组件，每个组件都独立封装**
![11285338-f2bab1f654ff5ce9](https://upload-images.jianshu.io/upload_images/11285338-f2bab1f654ff5ce9)
这里写图片描述
### 组件的特征：
- 可组合，一个组件可以和其它组件一起使用，或者嵌套在另一个组件内部
- 可重用，每个组件都有独立功能，可被使用在多个UI场景
- 可维护，每个小的组件只包含自身的逻辑，更容易被理解和维护
# React学习
> 
真正学会React是一个漫长的过程，因为它不仅仅是一个库，一个框架，而是一个庞大的体系，想要学习它，发挥它的威力，整个技术栈都要配合它来改造，需要学习一整套解决方案，从后端到前端，都是全新的模式。使用React后，就会发现采用它的整个技术栈是合理的选择
