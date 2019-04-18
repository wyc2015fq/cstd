# React Native 中 component 生命周期 - z69183787的专栏 - CSDN博客
2017年02月15日 16:09:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：424
个人分类：[前端-React](https://blog.csdn.net/z69183787/article/category/6289905)
[React Native](http://lib.csdn.net/base/reactnative)中的component跟[Android](http://lib.csdn.net/base/android)中的activity，fragment等一样，存在生命周期，下面先给出component的生命周期图
![这里写图片描述](https://img-blog.csdn.net/20160703034722480)
## getDefaultProps
`object getDefaultProps()`
执行过一次后，被创建的类会有缓存，映射的值会存在`this.props`,前提是这个prop不是父组件指定的 
这个方法在对象被创建之前执行，因此不能在方法内调用`this.props` ，另外，注意任何`getDefaultProps()`返回的对象在实例中共享，不是复制
## getInitialState
`object getInitialState()`
控件加载之前执行，返回值会被用于state的初始化值
## componentWillMount
`void componentWillMount()`
执行一次，在初始化`render`之前执行，如果在这个方法内调用`setState`，`render()`知道state发生变化，并且只执行一次
## render
`ReactElement render()`
render的时候会调用`render()`会被调用 
调用`render()`方法时，首先检查`this.props`和`this.state`返回一个子元素，子元素可以是DOM组件或者其他自定义复合控件的虚拟实现 
如果不想渲染可以返回null或者false，这种场景下，[React](http://lib.csdn.net/base/react)渲染一个`<noscript>`标签，当返回null或者false时，`ReactDOM.findDOMNode(this)`返回null 
`render()`方法是很纯净的，这就意味着不要在这个方法里初始化组件的state，每次执行时返回相同的值，不会读写DOM或者与服务器交互，如果必须如服务器交互，在`componentDidMount()`方法中实现或者其他生命周期的方法中实现，保持`render()`方法纯净使得服务器更准确，组件更简单
## componentDidMount
`void componentDidMount()`
在初始化render之后只执行一次，在这个方法内，可以访问任何组件，`componentDidMount()`方法中的子组件在父组件之前执行
从这个函数开始，就可以和 JS 其他框架交互了，例如设置计时 setTimeout 或者 setInterval，或者发起网络请求
## shouldComponentUpdate
```
boolean shouldComponentUpdate(
  object nextProps, object nextState
)
```
- 1
- 2
- 3
- 1
- 2
- 3
这个方法在初始化`render`时不会执行，当props或者state发生变化时执行，并且是在`render`之前，当新的`props`或者`state`不需要更新组件时，返回false
```java
shouldComponentUpdate: function(nextProps, nextState) {
  return nextProps.id !== this.props.id;
}
```
- 1
- 2
- 3
- 1
- 2
- 3
当`shouldComponentUpdate`方法返回false时，讲不会执行`render()`方法，`componentWillUpdate`和`componentDidUpdate`方法也不会被调用
默认情况下，`shouldComponentUpdate`方法返回true防止`state`快速变化时的问题，但是如果·`state`不变，`props`只读，可以直接覆盖`shouldComponentUpdate`用于比较`props`和`state`的变化，决定UI是否更新，当组件比较多时，使用这个方法能有效提高应用性能
## componentWillUpdate
```
void componentWillUpdate(
  object nextProps, object nextState
)
```
- 1
- 2
- 3
- 1
- 2
- 3
当`props`和`state`发生变化时执行，并且在`render`方法之前执行，当然初始化render时不执行该方法，需要特别注意的是，在这个函数里面，你就不能使用`this.setState`来修改状态。这个函数调用之后，就会把`nextProps`和`nextState`分别设置到`this.props`和`this.state`中。紧接着这个函数，就会调用`render()`来更新界面了
## componentDidUpdate
```
void componentDidUpdate(
  object prevProps, object prevState
)
```
- 1
- 2
- 3
- 1
- 2
- 3
组件更新结束之后执行，在初始化`render`时不执行
## componentWillReceiveProps
```
void componentWillReceiveProps(
  object nextProps
)
```
- 1
- 2
- 3
- 1
- 2
- 3
当`props`发生变化时执行，初始化`render`时不执行，在这个回调函数里面，你可以根据属性的变化，通过调用`this.setState()`来更新你的组件状态，旧的属性还是可以通过`this.props`来获取,这里调用更新状态是安全的，并不会触发额外的`render`调用
```java
componentWillReceiveProps: function(nextProps) {
  this.setState({
    likesIncreasing: nextProps.likeCount > this.props.likeCount
  });
}
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
## componentWillUnmount
`void componentWillUnmount()`
当组件要被从界面上移除的时候，就会调用`componentWillUnmount()`,在这个函数中，可以做一些组件相关的清理工作，例如取消计时器、网络请求等
## 总结
React Native的生命周期就介绍完了，其中最上面的虚线框和右下角的虚线框的方法一定会执行，左下角的方法根据`props``state`是否变化去执行，其中建议只有在`componentWillMount`,`componentDidMount`,`componentWillReceiveProps`方法中可以修改`state`值
英文地址：[https://facebook.github.io/react/docs/component-specs.html#lifecycle-methods](https://facebook.github.io/react/docs/component-specs.html#lifecycle-methods)
