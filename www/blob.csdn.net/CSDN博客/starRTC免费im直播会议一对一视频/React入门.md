# React入门 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年09月07日 14:42:11[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：76
个人分类：[react](https://blog.csdn.net/elesos/article/category/8022874)
A JavaScript library for building user interfaces
[https://reactjs.org/](https://reactjs.org/)
React 起源于 Facebook 的内部项目
从最早的UI引擎变成了一整套前后端通吃的 Web App 解决方案。衍生的 React Native 项目，目标更是宏伟，希望用写 Web App 的方式去写 Native App。
React 可以在浏览器运行，也可以在服务器运行，但是本教程只涉及浏览器。一方面是为了尽量保持简单，另一方面 React 的语法是一致的，服务器的用法与浏览器差别不大。
HTML 模板
结构大致如下。
<!DOCTYPE html> <html> <head> <script src="../build/react.js"></script> <script src="../build/react-dom.js"></script> <script src="../build/browser.min.js"></script> </head> <body> <div id="example"></div> <script type="text/babel"> // ** Our code goes here! ** </script> </body> </html>
Browser.js 的作用是将 JSX 语法转为 JavaScript 语法
ReactDOM.render()
用于将模板转为 HTML 语言，并插入指定的 DOM 节点。
ReactDOM.render( <h1>Hello, world!</h1>, document.getElementById('example') );
将一个 h1 标题，插入 example 节点
JSX 语法
上一节的代码， HTML 语言直接写在 JavaScript 语言之中，不加任何引号，这就是 [JSX 的语法](http://facebook.github.io/react/docs/displaying-data.html#jsx-syntax)，
var names = ['Alice', 'Emily', 'Kate']; ReactDOM.render( <div> { names.map(function (name) { return <div>Hello, {name}!</div> }) } </div>, document.getElementById('example') );
遇到 HTML 标签（以 < 开头），就用 HTML 规则解析；遇到代码块（以 { 开头），就用 JavaScript 规则解析。
var arr = [ <h1>Hello world!</h1>, <h2>React is awesome</h2>, ]; ReactDOM.render( <div>{arr}</div>, document.getElementById('example') );
组件
React.createClass 方法用于生成一个组件类
var HelloMessage = React.createClass({ render: function() { return <h1>Hello {this.props.name}</h1>; } }); ReactDOM.render( <HelloMessage name="John" />, document.getElementById('example') );
变量 HelloMessage 就是一个组件类。插入 <HelloMessage /> 时，会自动生成 HelloMessage 的一个实例，所有组件类都必须有自己的 render 方法，用于输出组件。
组件类的第一个字母必须大写, 组件类只能包含一个顶层标签
this.props.children
组件的所有子节点
ol:有序列表
可以用 React.Children.map 来遍历子节点
PropTypes
有时，我们需要一种机制，验证别人使用组件时，提供的参数是否符合要求。组件的PropTypes属性，就是用来验证组件实例的属性是否符合要求的
获取真实的DOM节点
组件并不是真实的 DOM 节点，可用 ref 属性获取
this.state
getInitialState 方法用于定义初始状态
this.setState 方法就修改状态值，每次修改以后，自动调用 this.render 方法，再次渲染组件。
this.props 表示那些一旦定义，就不再改变的特性，而 this.state 是会随着用户互动而产生变化的特性。
表单
通过 event.target.value 读取用户输入的值
组件的生命周期
三个状态：
- Mounting：已插入真实 DOM
- Updating：正在被重新渲染
- Unmounting：已移出真实 DOM
每个状态都提供了两种处理函数，will 函数在进入状态之前调用，did 函数在进入状态之后调用，三种状态共计五种处理函数。
- componentWillMount()
- componentDidMount()
- componentWillUpdate(object nextProps, object nextState)
- componentDidUpdate(object prevProps, object prevState)
- componentWillUnmount()
此外，React 还提供两种特殊状态的处理函数。
- componentWillReceiveProps(object nextProps)：已加载组件收到新的参数时调用
- shouldComponentUpdate(object nextProps, object nextState)：组件判断是否重新渲染时调用
Ajax
组件的数据来源，通常是通过 Ajax 请求从服务器获取，可以使用 componentDidMount 方法设置 Ajax 请求，等到请求成功，再用 this.setState 方法重新渲染 UI
可以把一个Promise对象传入组件,如果Promise对象正在抓取数据（pending状态），组件显示"正在加载"；如果Promise对象报错（rejected状态），组件显示报错信息；如果Promise对象抓取数据成功（fulfilled状态），组件显示获取的数据。
[http://www.ruanyifeng.com/blog/2015/03/react.html](http://www.ruanyifeng.com/blog/2015/03/react.html)
