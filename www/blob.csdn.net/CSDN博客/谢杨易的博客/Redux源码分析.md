
# Redux源码分析 - 谢杨易的博客 - CSDN博客

2017年07月20日 23:45:01[谢杨易](https://me.csdn.net/u013510838)阅读数：1496



# 1 redux使用步骤
React仅仅是一个前端View框架库，可以看做是MVC里面的V。没有解决组件间通信，MVC分离，数据共享等问题。Redux的出现使得这些都不是问题。使用Redux也比较简单，步骤大概如下
编写React Component，这里不涉及Redux
编写reducer，它接收一个state和action，返回一个新的state
createStore(reducer) 得到store对象，它是Redux的核心对象。包含的主要API如下
getState(), 得到store中保存的state对象
dispatch(action), 发送action消息给store，它会调用reducer更新state，并回调subscribe的监听器
subcribe(listener), 注册监听器，dispatch触发时会回调
编写React Component中的事件回调，比如onClick。使用store.dispatch(action)发送消息，reducer此时会得到调用，更新state。
其中createStore(reducer)比较关键，下面来分析它的源码
2 createStore()源码分析
functioncreateStore(reducer, preloadedState, enhancer){// reducer, preloadedState, enhancer 入参检查letcurrentReducer = reducerletcurrentState = preloadedState// createStore传入的state会存储为初始stateletcurrentListeners = []letnextListeners = currentListenersletisDispatching =false// 判断是否能触发nextListenersfunctionensureCanMutateNextListeners(){if(nextListeners === currentListeners) {
      nextListeners = currentListeners.slice()
    }
  }// 返回store中存储的statefunctiongetState(){returncurrentState
  }// 注册事件,加入到listener数组中。每次dispatch调用时，会回调注册的listenerfunctionsubscribe(listener){letisSubscribed =true// 存储listenerensureCanMutateNextListeners()
    nextListeners.push(listener)// 取消订阅事件returnfunctionunsubscribe(){if(!isSubscribed) {return}
      isSubscribed =falseensureCanMutateNextListeners()// 找到要取消订阅的listener,然后从数组中删除,以后就不会回调到它了constindex = nextListeners.indexOf(listener)
      nextListeners.splice(index,1)
    }
  }// 发送消息action。主要两件事// 1. 调用reducer更新state// 2. 回调subscribe注册的listenersfunctiondispatch(action){if(isDispatching) {thrownewError('Reducers may not dispatch actions.')
    }try{
      isDispatching =true// 调用reducer来更新state, 传入当前state和actioncurrentState = currentReducer(currentState, action)
    }finally{
      isDispatching =false}// 回调subscribe的listeners, dispatch会触发回调listenerconstlisteners = currentListeners = nextListenersfor(leti =0; i < listeners.length; i++) {constlistener = listeners[i]
      listener()
    }returnaction
  }// createStore时触发一次dispatch, 一般会调用到我们reducer里面的default分支,此时返回默认state,更新currentStatedispatch({ type: ActionTypes.INIT })return{
    dispatch,
    subscribe,
    getState,
  }源码分析中对一些不是很关键的地方做了省略，我们重点关注dispatch， subscribe， getState三个API即可。其中dispatch稍微麻烦点，它主要做两件事
调用reducer更新state
回调subscribe注册的listeners
3 React-redux 使用步骤
Redux已经足够优秀了，但需要用户手动调用setState来更新view，也没有建立store上的state数据和React Component之间联系，而React-redux框架很好的解决了这个问题。它的使用步骤如下
编写UI组件，也就是React Component。这部分和Redux无关
编写mapStateToProps,     mapDispatchToProps,然后connect(mapStateToProps,     mapDispatchToProps)(UI组件)来生成容器组件。容器组件包含了UI，数据和逻辑。
编写reducers，完成控制逻辑。这部分和只是用Redux相同
应用顶层做整合
createStore(reducers) 生成store
使用Provider包装顶层组件，如下
ReactDOM.render(<Providerstore={store}><App/></Provider>,
document.getElementById('root')
);
步骤不复杂，下面我们来分析下connect源码
4 connect源码分析
connectAPI为connect([mapStateToProps], [mapDispatchToProps], [mergeProps], [options]), 后两个参数不是很关键，我们源码分析中省略它们，这样以最简洁的方式分析主流程
functionconnect(mapStateToProps, mapDispatchToProps){constinitMapStateToProps = match(mapStateToProps, mapStateToPropsFactories,'mapStateToProps')constinitMapDispatchToProps = match(mapDispatchToProps, mapDispatchToPropsFactories,'mapDispatchToProps')returnconnectHOC(selectorFactory, {// used in error messagesmethodName:'connect',// used to compute Connect's displayName from the wrapped component's displayName.getDisplayName: name => `Connect(${name})`,// mapStateToProps为空时，不能自动处理View刷新，此时需要用户自己setState来更新viewshouldHandleStateChanges:Boolean(mapStateToProps),// 参数传递到selectorFactoryinitMapStateToProps,
    initMapDispatchToProps,
  })
}connect方法没做什么事情，调用的connectHOC方法，connectHOC默认实现为connectAdvanced。connectAdvanced最终返回经过包装的React组件，也就是Redux所说的容器型组件。它对React组件生命周期方法进行了一些处理，源码如下。
classConnect extends Component {
      constructor(props, context) {
        super(props, context)this.version = versionthis.state = {}this.renderCount =0this.store = props[storeKey] || context[storeKey]this.propsMode =Boolean(props[storeKey])this.setWrappedInstance =this.setWrappedInstance.bind(this)this.initSelector()this.initSubscription()
      }
      componentDidMount() {// shouldHandleStateChanges: Boolean(mapStateToProps)// 当mapStateToProps为空时，不能自动处理View刷新，此时需要用户自己setState来更新viewif(!shouldHandleStateChanges)return// 关键在selector.run()方法中，它会将mapStateToProps mapDispatchToProps等扩展的数据添加到React Component的props上。具体看后面makeSelectorStateful的分析this.subscription.trySubscribe()this.selector.run(this.props)// 如果props有改变，shouldComponentUpdate会为true，此时forupdate强制刷新viewif(this.selector.shouldComponentUpdate)this.forceUpdate()
      }
      componentWillReceiveProps(nextProps) {// 具体代码看后面的makeSelectorStateful，主要是判断props有没有更新，如果更新则刷新viewthis.selector.run(nextProps)
      }// 判断是否取消掉view的刷新，React生命周期方法// selector.shouldComponentUpdate在makeSelectorStateful进行赋值的// 如果前后两次props有改变，则设置为true，此时不会拦截view的刷新shouldComponentUpdate() {returnthis.selector.shouldComponentUpdate
      }// unmount卸载前的调用componentWillUnmount() {if(this.subscription)this.subscription.tryUnsubscribe()this.subscription =nullthis.notifyNestedSubs = noopthis.store =nullthis.selector.run = noopthis.selector.shouldComponentUpdate =false}// wrappedInstance为未经过包装的React组件，也就是Redux所说的UI组件getWrappedInstance() {returnthis.wrappedInstance
      }
      setWrappedInstance(ref) {this.wrappedInstance = ref
      }// 比较关键，具体分析可以看后面对makeSelectorStateful的单独分析initSelector() {// selectorFactory很复杂，大概就是将stateProps，dispatchProps和原先就有的props合并。constsourceSelector = selectorFactory(this.store.dispatch, selectorFactoryOptions)// run方法在props有变化时，将shouldComponentUpdate设置为true，React生命周期时可以刷新view。另外将mapStateToProps mapDispatchToProps等扩展的数据添加到React Component的props上。this.selector = makeSelectorStateful(sourceSelector,this.store)this.selector.run(this.props)
      }
      onStateChange() {this.selector.run(this.props)if(!this.selector.shouldComponentUpdate) {this.notifyNestedSubs()
        }else{this.componentDidUpdate =this.notifyNestedSubsOnComponentDidUpdatethis.setState(dummyState)
        }
      }
      addExtraProps(props) {// 省略不关键的代码，基本就是将props返回。constwithExtras = { ...props }returnwithExtras
      }// Redux封装了React Component，重写了render方法，对props进行了增强render() {constselector =this.selector
        selector.shouldComponentUpdate =falseif(selector.error) {throwselector.error
        }else{// WrappedComponent是connect()(reactComponent)中传入的React Component// Redux对React Component进行了封装，render方法最后调用React的createElement 将props传入returncreateElement(WrappedComponent,this.addExtraProps(selector.props))
        }
      }
    }
}initSelector()方法调用makeSelectorStateful获取selector，并调用它的run方法
functionmakeSelectorStateful(sourceSelector, store){constselector = {
    run:functionrunComponentSelector(props){try{// 得到props， selector为我们包装的React Component对象constnextProps = sourceSelector(store.getState(), props)if(nextProps !== selector.props || selector.error) {// props有变化，则shouldComponentUpdate为true，会刷新viewselector.shouldComponentUpdate =true// 将mapStateToProps mapDispatchToProps等扩展的数据添加到React Component的props上selector.props = nextProps
          selector.error =null}
      }catch(error) {
        selector.shouldComponentUpdate =trueselector.error = error
      }
    }
  }returnselector
}connect方法源码十分复杂，我们省略了很多不关键的地方。从源码中我们可以看出
mapStateToProps，mapDispatchToProps，最终会经过selectorFactory，与React组件本身的props进行合并。
mapStateToProps注册了subscribe，这样每次dispatch时，回调完reducer后，就会调用mapStateToProps，将新的state传递过来。此时再将state合并到组件的props上，会引起props的改变，从而触发一次forceUpdate，从而自动刷新view
connect方法对React组件进行了包装，返回一个容器型组件。

