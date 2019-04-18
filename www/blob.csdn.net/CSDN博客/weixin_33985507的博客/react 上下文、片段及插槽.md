# react 上下文、片段及插槽 - weixin_33985507的博客 - CSDN博客
2018年04月08日 09:03:16[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
## 1.上下文(Context)
react是单项数据流，数据是自上往下传递的。如果父组件想传递数据给孙组件，需要父传递到子，子再传递到孙。也可以用上下文解决。
在某些场景下，你想在整个组件树中传递数据，但却不想手动地在每一层传递属性。你可以直接在 React 中使用强大的”context” API解决上述问题。
react路由就是通过上下文实现的。
**上下文实现步骤**
```bash
1. 在父组件里定义 childContextTypes 子上下文类型
2. 在父组件里还要定义一个getChildContext用来返回上下文对象
3. 在要接收这些上下文对象的组件里写义contextTypes
复制代码
```
实现一个小功能，有header，main两个组件，分别包含title，content组件，公用一个颜色。
**父组件**
- childContextTypes
- getChildContext
**子组件**
- contextTypes
- this.context
```bash
import React, { Component } from 'react';
import ReactDOM from 'react-dom';
import PropTypes from 'prop-types';
/**
 * 1.  在父组件里定义 childContextTypes 子上下文类型
 * 2. 在父组件里还要定义一个getChildContext用来返回上下文对象
 * 3. 在要接收这些上下文对象的组件里写义contextTypes
 */
export default class HomePage extends Component {
    static childContextTypes = {
        color: PropTypes.string,
        setColor:PropTypes.func
    }
    getChildContext() {
        return {
            color: this.state.color,
            setColor:this.setColor
        }
    }
    //状态不能别人改，只能自己改。
    constructor() {
        super();
        this.state = { color: 'red' };
    }
    setColor = (color)=>{
        this.setState({color});
    }
    render() {
        return (
            <div>
                <Header />
                <Main />
            </div>
        )
    }
}
class Header extends Component {
    render() {
        return (
            <div>
                <Title />
            </div>
        )
    }
}
class Main extends Component {
    render() {
        return (
            <div>
                <Content />
            </div>
        )
    }
}
class Title extends Component {
    static contextTypes = {
        color: PropTypes.string
    }
    render() {
        //this.context
        return (
            <div>
                <h1 style={{ color: this.context.color }}>我是标题</h1>
            </div>
        )
    }
}
class Content extends Component {
    static contextTypes = {
        color: PropTypes.string,
        setColor: PropTypes.func
    }
    render() {
        console.log(this.context);
        return (
            <div>
                <h1 style={{ color: this.context.color }}>我是内容</h1>
                <button onClick={()=>this.context.setColor('green')}>变绿</button> 
                <button onClick={()=>this.context.setColor('yellow')}>变黄</button> 
            </div>
        )
    }
}
复制代码
```
## 2.片段(fragments)
React 中一个常见模式是为一个组件返回多个元素。 片段(fragments) 可以让你将子元素列表添加到一个分组中，并且不会在DOM中增加额外节点。
```bash
import React,{Component} from 'react';
class List extends Component{
    render(){
        return (
                { this.props.messages.map((item,key)=><li key={key}>{item}</li>)}
        )
    }
}
export default class Messages extends Component{
    constructor(){
        super();
        this.state = {messages:[1,2,3]};
    }
    render(){
        return (
            <ul>
               <List messages = {this.state.messages}/>
            </ul>
        )
    }
}
复制代码
```
将li的逻辑单独拎出来成立一个组件，但是这样会报错，react只能返回一个顶层元素，现在返回的是很多li。需要在他的外层添加一个顶层元素。
```bash
class List extends Component{
    render(){
        return (
            <div>
                { this.props.messages.map((item,key)=><li key={key}>{item}</li>)}
            </div>
        )
    }
}
复制代码
```
但是这样ul中将会有div。这样dom结构就乱了。
这时候就需要用到片段了：
```bash
class List extends Component{
    render(){
        return (
           <React.Fragment>
                { this.props.messages.map((item,key)=><li key={key}>{item}</li>)}
           </React.Fragment>
        )
    }
}
复制代码
```
<React.Fragment>标签不会形成新的dom元素。完美解决这个问题。
另一种更简单更形象写法，**但是**现在还不支持：
```bash
class List extends Component{
    render(){
        return (
           <>
                { this.props.messages.map((item,key)=><li key={key}>{item}</li>)}
           </>
        )
    }
}
复制代码
```
## 3.插槽(Portals)
Portals 提供了一种很好的方法，将子节点渲染到父组件 DOM 层次结构之外的 DOM 节点。
```bash
ReactDOM.createPortal(child, container)
复制代码
```
> - 第一个参数（child）是任何可渲染的 React 子元素，例如一个元素，字符串或 片段(fragment)
- 第二个参数（container）则是一个 DOM 元素
需求：页面中有很多组件，但每个组件都一个个弹窗功能。弹窗不应属于任何一个组件。
在index.html中放入模态框的根节点
```bash
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
    <div id="root"></div>
    <div id="modal-root" class="modal-root"></div>
</body>
</html>
复制代码
```
modal-root放入的为模态框组件，我们的其他组件在root中。
**模态框组件**
```bash
import React, {Component} from 'react';
import ReactDOM from 'react-dom';
import './Modal.css'
class Modal extends Component {
    constructor() {
        super();
        this.container = document.querySelector('#modal-root');
    }
    render() {
        return ReactDOM.createPortal(this.props.children, this.container);
    }
}
export default class ModelPage extends Component {
    constructor() {
        super();
        this.state = {show: false};
    }
    render() {
        return (
            <div>
                <button onClick={() => this.setState({show: !this.state.show})}>显示</button>
                {
                    this.state.show ? <Modal>
                        <div className="modal-container">
                            <div className="modal-content">
                                <h1>显示模态窗口</h1>
                            </div>
                        </div>
                    </Modal> : null
                }
            </div>
        )
    }
}
复制代码
```
## 4.错误边界(Error Boundaries)
部分 UI 中的 JavaScript 错误不应该破坏整个应用程序。 为了解决 React 用户的这个问题，React 16引入了一个 “错误边界(Error Boundaries)” 的新概念。
```bash
import React,{Component} from 'react';
class ErrorBoundary extends Component{
   constructor(){
       super();
       this.state = {hasError:false};
   }
   componentDidCatch(hasError){
    this.setState({hasError});
   }
   render(){
       if(this.state.hasError){
         return <div>此组件暂时无法显示</div>
       }
       return this.props.children
   }
}
class Todo extends Component{
    render(){
        return <div>{null.toString()}</div>
    }
}
export default class MyPage extends Component{
    render(){
        return (
            <ErrorBoundary>
                <Todo/>
            </ErrorBoundary>
        )
    }
}
复制代码
```
null.toString()不合法，会显示：此组件暂时无法显示。
