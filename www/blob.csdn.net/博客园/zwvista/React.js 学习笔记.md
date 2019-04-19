# React.js 学习笔记 - zwvista - 博客园
## [React.js 学习笔记](https://www.cnblogs.com/zwvista/p/9662128.html)
### React.js
React.js 是时下最流行的前端 JavaScript 框架之一。
### 创建工程
```
# 安装 CLI
$ npm install -g create-react-app
# 创建新的应用程序
$ create-react-app my-app --scripts-version=react-scripts-ts
# 进入程序所在路径
$ cd my-app
# 编译并运行
$ npm start
# http://localhost:3000 打开主页
```
### 组件
- 使用 ReactDOM.render 方法渲染组件。
该方法带有两个参数：一个React元素（组件）和一个DOM元素。
- 使用 JSX（类似 XML的标签）描述组件。
- 顶层标签只能有一个。
- 使用 {} 在 JSX 中插入表达式。
- JSX 本身是一个表达式。
- JSX 中属性的值可以用 ""，也可以用 {}。
### 函数形式组件
- 组件分为函数形式组件和类形式组件。函数形式组件相当于只有 render 方法的类形式组件。
- 函数形式组件接收唯一一个参数（props），返回 React 元素。
- 父标签通过属性向子标签传递参数。子标签的属性名对应于组件参数 props 的属性。
### 类形式组件
- 组件的类型为 React.Component。
- 类形式组件的构造器接收唯一一个参数（props）。
类形式组件的其他方法通过 this.props 引用这个参数。
- 类形式组件的 render 方法返回 React 元素。
- 类形式组件的 lifecycle 方法：
componentDidMount
componentWillUnmount
- 类形式组件在构造器中初始化状态属性 this.state（或者在类当中不使用构造器直接初始化属性 state）。
类形式组件的其他方法通过 this.state 引用状态，通过调用 this.setState 方法来改变状态更新组件。
- 组件中事件处理器有3种写法：
使用 bind 将 this 绑定到事件处理器。
使用箭头函数初始化事件处理器。
在标签中将箭头函数作为事件属性的值。
- 组件模板中没有 if 语句，需要使用 && || ?: 这些逻辑运算符来做条件渲染。
- 组件模板中没有 for 语句，需要使用 array.map 这种函数型编程手段来将数组展开成多个标签。
### 基本结构
```
<html>
<head>
  <script crossorigin src="https://unpkg.com/react@16/umd/react.production.min.js"></script>
  <script crossorigin src="https://unpkg.com/react-dom@16/umd/react-dom.production.min.js"></script>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/babel-standalone/6.26.0/babel.min.js"></script>
  <meta charset="utf-8"/>
</head>
<body>
  <div id="root"></div>
  
<script type="text/babel">
function Welcome(props) {
  return <h1>Hello, {props.name}</h1>;
}
const element = <Welcome name="Sara" />;
ReactDOM.render(
  element,
  document.getElementById('root')
);
</script>
</body>
</html>
```
