# react+redux教程（一）connect、applyMiddleware、thunk、webpackHotMiddleware - z69183787的专栏 - CSDN博客
2016年07月17日 21:49:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：810
http://www.cnblogs.com/lewis617/p/5145073.html;
今天，我们通过解读官方示例代码（counter）的方式来学习react+redux。
# 例子
![](http://images2015.cnblogs.com/blog/814069/201601/814069-20160120134123062-158958063.gif)
这个例子是官方的例子，计数器程序。前两个按钮是加减，第三个是如果当前数字是奇数则加一，第四个按钮是异步加一（延迟一秒）。
源代码：[https://github.com/lewis617/react-redux-tutorial/tree/master/redux-examples/counter](https://github.com/lewis617/react-redux-tutorial/tree/master/redux-examples/counter)
# 组件
components/Counter.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import React, { Component, PropTypes } from 'react'
class Counter extends Component {
  render() {
    //从组件的props属性中导入四个方法和一个变量
    const { increment, incrementIfOdd, incrementAsync, decrement, counter } = this.props;
    //渲染组件，包括一个数字，四个按钮
    return (
      <p>
        Clicked: {counter} times
        {' '}
        <button onClick={increment}>+</button>
        {' '}
        <button onClick={decrement}>-</button>
        {' '}
        <button onClick={incrementIfOdd}>Increment if odd</button>
        {' '}
        <button onClick={() => incrementAsync()}>Increment async</button>
      </p>
    )
  }
}
//限制组件的props安全
Counter.propTypes = {
  //increment必须为fucntion,且必须存在
  increment: PropTypes.func.isRequired,
  incrementIfOdd: PropTypes.func.isRequired,
  incrementAsync: PropTypes.func.isRequired,
  decrement: PropTypes.func.isRequired,
  //counter必须为数字，且必须存在
  counter: PropTypes.number.isRequired
};
export default Counter
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上述代码，我们干了几件事：
- 从props中导入变量和方法
- 渲染组件
有的同学可能会急于想知道props的方法和变量是怎么来，下面我们继续解读。
# 容器
containers/App.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import Counter from '../components/Counter'
import * as CounterActions from '../actions/counter'
//将state.counter绑定到props的counter
function mapStateToProps(state) {
  return {
    counter: state.counter
  }
}
//将action的所有方法绑定到props上
function mapDispatchToProps(dispatch) {
  return bindActionCreators(CounterActions, dispatch)
}
//通过react-redux提供的connect方法将我们需要的state中的数据和actions中的方法绑定到props上
export default connect(mapStateToProps, mapDispatchToProps)(Counter)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
看到这里，很多刚接触redux同学可能已经晕了，我来图解下redux的流程。
![](https://img-blog.csdn.net/20160801152020537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
state就是数据，组件就是数据的呈现形式，action是动作，action是通过reducer来更新state的。
上述代码，我们干了几件事：
- 把state的counter值绑定到props上
- 把四个action创建函数绑定到props上
# connect
那么为什么就绑定上去了呢？因为有connect这个方法。这个方法是如何实现的，或者我们该怎么用这个方法呢？connect这个方法的用法，可以直接看[api文档](http://camsong.github.io/redux-in-chinese/docs/react-redux/api.html)。我也可以简单描述一下：
- 第一个参数，必须是function，作用是绑定state的指定值到props上面。这里绑定的是counter
- 第二个参数，可以是function，也可以是对象，作用是绑定action创建函数到props上。
- 返回值，是绑定后的组件
这里还有很多种其他写法，我喜欢在第二个参数绑定一个对象，即
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import Counter from '../components/Counter'
import * as CounterActions from '../actions/counter'
//将state.counter绑定到props的counter
function mapStateToProps(state) {
  return {
    counter: state.counter
  }
}
//通过react-redux提供的connect方法将我们需要的state中的数据和actions中的方法绑定到props上
export default connect(mapStateToProps, CounterActions)(Counter)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
还可以不写第二个参数，后面用dispatch来触发action的方法，即
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import Counter from '../components/Counter'
import * as CounterActions from '../actions/counter'
//将state.counter绑定到props的counter
function mapStateToProps(state) {
  return {
    counter: state.counter
  }
}
//通过react-redux提供的connect方法将我们需要的state中的数据绑定到props上
export default connect(mapStateToProps)(Counter)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
后面在组件中直接使用dispatch()来触发action创建函数。
# action和reducer两个好基友负责更新state
actions/counter.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
export const INCREMENT_COUNTER = 'INCREMENT_COUNTER'
export const DECREMENT_COUNTER = 'DECREMENT_COUNTER'
//导出加一的方法
export function increment() {
  return {
    type: INCREMENT_COUNTER
  }
}
//导出减一的方法
export function decrement() {
  return {
    type: DECREMENT_COUNTER
  }
}
//导出奇数加一的方法，该方法返回一个方法，包含dispatch和getState两个参数，dispatch用于执行action的方法，getState返回state
export function incrementIfOdd() {
  return (dispatch, getState) => {
    //获取state对象中的counter属性值
    const { counter } = getState()
    //偶数则返回
    if (counter % 2 === 0) {
      return
    }
    //没有返回就执行加一
    dispatch(increment())
  }
}
//导出一个方法,包含一个默认参数delay,返回一个方法,一秒后加一
export function incrementAsync(delay = 1000) {
  return dispatch => {
    setTimeout(() => {
      dispatch(increment())
    }, delay)
  }
}
//这些方法都导出,在其他文件导入时候,使用import * as actions 就可以生成一个actions对象包含所有的export
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
reducers/counter.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { INCREMENT_COUNTER, DECREMENT_COUNTER } from '../actions/counter'
//reducer其实也是个方法而已,参数是state和action,返回值是新的state
export default function counter(state = 0, action) {
  switch (action.type) {
    case INCREMENT_COUNTER:
      return state + 1
    case DECREMENT_COUNTER:
      return state - 1
    default:
      return state
  }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
reducers/index.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { combineReducers } from 'redux'
import counter from './counter'
//使用redux的combineReducers方法将所有reducer打包起来
const rootReducer = combineReducers({
  counter
})
export default rootReducer
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上述代码我们干了几件事:
- 写了四个action创建函数
- 写了reducer用于更新state
- 将所有reducer(这里只有一个)打包成一个reducer
看到这里，有很多初次接触redux的同学可能已经晕了，怎么那么多概念？为了形象直观，我们在开发工具（react dev tools）上看看这些state，props什么的：
action的方法和state的变量是不是都绑定上去了啊。state怎么看呢？这个需要借助redux的开发工具，也可以通过Connect(Counter)组件的State来查看redux那颗全局唯一的状态树：
那个storeState就是全局唯一的状态树。我们可以看到只有一个counter而已。
![](https://img-blog.csdn.net/20160801152114319?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 注册store
store/configureStore.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import { createStore, applyMiddleware } from 'redux'
import thunk from 'redux-thunk'
import reducer from '../reducers'
//applyMiddleware来自redux可以包装 store 的 dispatch
//thunk作用是使action创建函数可以返回一个function代替一个action对象
const createStoreWithMiddleware = applyMiddleware(
  thunk
)(createStore)
export default function configureStore(initialState) {
  const store = createStoreWithMiddleware(reducer, initialState)
  //热替换选项
  if (module.hot) {
    // Enable Webpack hot module replacement for reducers
    module.hot.accept('../reducers', () => {
      const nextReducer = require('../reducers')
      store.replaceReducer(nextReducer)
    })
  }
  return store
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
index.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import React from 'react'
import { render } from 'react-dom'
import { Provider } from 'react-redux'
import App from './containers/App'
import configureStore from './store/configureStore'
const store = configureStore()
render(
  <Provider store={store}>
    <App />
  </Provider>,
  document.getElementById('root')
)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上述代码，我们干了几件事：
- 用中间件使action创建函数可以返回一个function代替一个action对象
- 如果在热替换状态（Webpack hot module replacement）下，允许替换reducer
- 导出store
- 将store放进provider
- 将provider放在组件顶层，并渲染
# applyMiddleware、thunk
applyMiddleware来自redux可以包装 store 的 dispatch()
thunk作用使action创建函数可以返回一个function代替一个action对象
# 服务
server.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var webpack = require('webpack')
var webpackDevMiddleware = require('webpack-dev-middleware')
var webpackHotMiddleware = require('webpack-hot-middleware')
var config = require('./webpack.config')
var app = new (require('express'))()
var port = 3000
var compiler = webpack(config)
app.use(webpackDevMiddleware(compiler, { noInfo: true, publicPath: config.output.publicPath }))
app.use(webpackHotMiddleware(compiler))
app.get("/", function(req, res) {
  res.sendFile(__dirname + '/index.html')
})
app.listen(port, function(error) {
  if (error) {
    console.error(error)
  } else {
    console.info("==> ��  Listening on port %s. Open up http://localhost:%s/ in your browser.", port, port)
  }
})
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
webpack.config.js
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var path = require('path')
var webpack = require('webpack')
module.exports = {
  devtool: 'cheap-module-eval-source-map',
  entry: [
    'webpack-hot-middleware/client',
    './index'
  ],
  output: {
    path: path.join(__dirname, 'dist'),
    filename: 'bundle.js',
    publicPath: '/static/'
  },
  plugins: [
    new webpack.optimize.OccurenceOrderPlugin(),
    new webpack.HotModuleReplacementPlugin(),
    new webpack.NoErrorsPlugin()
  ],
  module: {
    loaders: [
      {
        test: /\.js$/,
        loaders: [ 'babel' ],
        exclude: /node_modules/,
        include: __dirname
      }
    ]
  }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
npm start 后执行node server ，触发webpack。webpack插件功能如下：
- OccurenceOrderPlugin的作用是给经常使用的模块分配最小长度的id。
- HotModuleReplacementPlugin是热替换，热替换和dev-server的hot有什么区别？不用刷新页面，可用于生产环境。
- NoErrorsPlugin用于保证编译后的代码永远是对的，因为不对的话会自动停掉。
# webpackHotMiddleware
server.js中webpackHotMiddleware的用法是参考[官网](https://www.npmjs.com/package/webpack-hot-middleware)的，没有为什么，express中间件就是在请求后执行某些操作。
