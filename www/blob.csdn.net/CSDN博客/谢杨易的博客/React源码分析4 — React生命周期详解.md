
# React源码分析4 — React生命周期详解 - 谢杨易的博客 - CSDN博客

2017年02月27日 14:19:04[谢杨易](https://me.csdn.net/u013510838)阅读数：4283



# 1 React生命周期流程
![Markdown](http://p1.bpimg.com/573600/1d957ca6195ce7ca.png)
调用流程可以参看上图。分为实例化，存在期和销毁三个不同阶段。介绍生命周期流程的文章很多，相信大部分同学也有所了解，我们就不详细分析了。很多同学肯定有疑问，这些方法在React内部是在哪些方法中被调用的呢，他们触发的时机又是什么时候呢。下面我们来详细分析。
# 2 实例化生命周期
## getDefaultProps
在React.creatClass()初始化组件类时，会调用getDefaultProps()，将返回的默认属性挂载到defaultProps变量下。这段代码之前已经分析过了，参考[React源码分析2 — 组件和对象的创建（createClass，createElement）](http://blog.csdn.net/u013510838/article/details/55669769).
这里要提的一点是，初始化组件类只运行一次。可以把它简单类比为Java中的Class对象。初始化组件类就是ClassLoader加载Class对象的过程。类对象的初始化不要错误理解成了实例对象的初始化。一个React组件类可能会在JSX中被多次调用，产生多个组件对象，但它只有一个类对象，也就是类加载后getDefaultProps就不会再调用了。
## getInitialState
这个方法在创建组件实例对象的时候被调用，具体代码位于React.creatClass()的Constructor函数中。之前文章中已经分析了，参考[React源码分析2 — 组件和对象的创建（createClass，createElement）](http://blog.csdn.net/u013510838/article/details/55669769)。
每次创建React实例对象时，它都会被调用。
## mountComponent
componentWillMount，render，componentDidMount都是在mountComponent中被调用。在[React源码分析3 — React组件插入DOM流程](http://blog.csdn.net/u013510838/article/details/56677451)一文中，我们讲过mountComponent被调用的时机。它是在渲染新的ReactComponent中被调用的。输入ReactComponent，返回组件对应的HTML。把这个HTML插入到DOM中，就可以生成组件对应的DOM对象了。所以mountComponent尤其关键。
和Java中的多态一样，不同的React组件的mountComponent实现都有所区别。下面我们来重点分析React自定义组件类，也就是ReactCompositeComponent的mountComponent。
```python
mountComponent:
```
```python
function
```
```python
(transaction, nativeParent, nativeContainerInfo, context)
```
```python
{
```
```python
this
```
```python
._context = context;
```
```python
this
```
```python
._mountOrder = nextMountID++;
```
```python
this
```
```python
._nativeParent = nativeParent;
```
```python
this
```
```python
._nativeContainerInfo = nativeContainerInfo;
```
```python
// 做propTypes是否合法的判断，这个只在开发阶段有用
```
```python
var
```
```python
publicProps =
```
```python
this
```
```python
._processProps(
```
```python
this
```
```python
._currentElement.props);
```
```python
var
```
```python
publicContext =
```
```python
this
```
```python
._processContext(context);
```
```python
var
```
```python
Component =
```
```python
this
```
```python
._currentElement.type;
```
```python
// 初始化公共类
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._constructComponent(publicProps, publicContext);
```
```python
var
```
```python
renderedElement;
```
```python
// inst或者inst.render为空对应的是stateless组件，也就是无状态组件
```
```python
// 无状态组件没有实例对象，它本质上只是一个返回JSX的函数而已。是一种轻量级的React组件
```
```python
if
```
```python
(!shouldConstruct(Component) && (inst ==
```
```python
null
```
```python
|| inst.render ==
```
```python
null
```
```python
)) {
      renderedElement = inst;
      warnIfInvalidElement(Component, renderedElement);
      inst =
```
```python
new
```
```python
StatelessComponent(Component);
    }
```
```python
// 设置变量
```
```python
inst.props = publicProps;
    inst.context = publicContext;
    inst.refs = emptyObject;
    inst.updater = ReactUpdateQueue;
```
```python
this
```
```python
._instance = inst;
```
```python
// 存储实例对象的引用到map中，方便以后查找
```
```python
ReactInstanceMap.set(inst,
```
```python
this
```
```python
);
```
```python
// 初始化state，队列等
```
```python
var
```
```python
initialState = inst.state;
```
```python
if
```
```python
(initialState ===
```
```python
undefined
```
```python
) {
      inst.state = initialState =
```
```python
null
```
```python
;
    }
```
```python
this
```
```python
._pendingStateQueue =
```
```python
null
```
```python
;
```
```python
this
```
```python
._pendingReplaceState =
```
```python
false
```
```python
;
```
```python
this
```
```python
._pendingForceUpdate =
```
```python
false
```
```python
;
```
```python
var
```
```python
markup;
```
```python
if
```
```python
(inst.unstable_handleError) {
```
```python
// 挂载时出错，进行一些错误处理，然后performInitialMount，初始化挂载
```
```python
markup =
```
```python
this
```
```python
.performInitialMountWithErrorHandling(renderedElement, nativeParent, nativeContainerInfo, transaction, context);
    }
```
```python
else
```
```python
{
```
```python
// 初始化挂载
```
```python
markup =
```
```python
this
```
```python
.performInitialMount(renderedElement, nativeParent, nativeContainerInfo, transaction, context);
    }
```
```python
if
```
```python
(inst.componentDidMount) {
```
```python
// 调用componentDidMount，以事务的形式
```
```python
transaction.getReactMountReady().enqueue(inst.componentDidMount, inst);
    }
```
```python
return
```
```python
markup;
  },
```
mountComponent先做实例对象的props,state等初始化，然后调用performInitialMount初始化挂载，完成后调用componentDidMount。这个调用链还是很清晰的。下面我们重点来分析performInitialMountWithErrorHandling和performInitialMount
```python
performInitialMountWithErrorHandling:
```
```python
function
```
```python
(renderedElement, nativeParent, nativeContainerInfo, transaction, context)
```
```python
{
```
```python
var
```
```python
markup;
```
```python
var
```
```python
checkpoint = transaction.checkpoint();
```
```python
try
```
```python
{
```
```python
// 放到try-catch中，如果没有出错则调用performInitialMount初始化挂载。可见这里没有什么特别的操作，也就是做一些错误处理而已
```
```python
markup =
```
```python
this
```
```python
.performInitialMount(renderedElement, nativeParent, nativeContainerInfo, transaction, context);
    }
```
```python
catch
```
```python
(e) {
```
```python
// handleError，卸载组件，然后重新performInitialMount初始化挂载
```
```python
transaction.rollback(checkpoint);
```
```python
this
```
```python
._instance.unstable_handleError(e);
```
```python
if
```
```python
(
```
```python
this
```
```python
._pendingStateQueue) {
```
```python
this
```
```python
._instance.state =
```
```python
this
```
```python
._processPendingState(
```
```python
this
```
```python
._instance.props,
```
```python
this
```
```python
._instance.context);
      }
      checkpoint = transaction.checkpoint();
```
```python
this
```
```python
._renderedComponent.unmountComponent(
```
```python
true
```
```python
);
      transaction.rollback(checkpoint);
      markup =
```
```python
this
```
```python
.performInitialMount(renderedElement, nativeParent, nativeContainerInfo, transaction, context);
    }
```
```python
return
```
```python
markup;
  },
```
可见performInitialMountWithErrorHandling只是多了一层错误处理而已，关键还是在performInitialMount中。
```python
performInitialMount:
```
```python
function
```
```python
(renderedElement, nativeParent, nativeContainerInfo, transaction, context)
```
```python
{
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
if
```
```python
(inst.componentWillMount) {
```
```python
// render前调用componentWillMount
```
```python
inst.componentWillMount();
```
```python
// 将state提前合并，故在componentWillMount中调用setState不会触发重新render，而是做一次state合并。这样做的目的是减少不必要的重新渲染
```
```python
if
```
```python
(
```
```python
this
```
```python
._pendingStateQueue) {
        inst.state =
```
```python
this
```
```python
._processPendingState(inst.props, inst.context);
      }
    }
```
```python
// 如果不是stateless，即无状态组件，则调用render，返回ReactElement
```
```python
if
```
```python
(renderedElement ===
```
```python
undefined
```
```python
) {
      renderedElement =
```
```python
this
```
```python
._renderValidatedComponent();
    }
```
```python
// 得到组件类型，如空组件ReactNodeTypes.EMPTY，自定义React组件ReactNodeTypes.COMPOSITE，DOM原生组件ReactNodeTypes.NATIVE
```
```python
this
```
```python
._renderedNodeType = ReactNodeTypes.getType(renderedElement);
```
```python
// 由ReactElement生成ReactComponent，这个方法在之前讲解过。根据不同type创建不同Component对象
```
```python
// 参考 http://blog.csdn.net/u013510838/article/details/55669769
```
```python
this
```
```python
._renderedComponent =
```
```python
this
```
```python
._instantiateReactComponent(renderedElement);
```
```python
// 递归渲染，渲染子组件
```
```python
var
```
```python
markup = ReactReconciler.mountComponent(
```
```python
this
```
```python
._renderedComponent, transaction, nativeParent, nativeContainerInfo,
```
```python
this
```
```python
._processChildContext(context));
```
```python
return
```
```python
markup;
  },
```
performInitialMount中先调用componentWillMount()，再将setState()产生的state改变进行state合并，然后调用_renderValidatedComponent()返回ReactElement，它会调用render()方法。然后由ReactElement创建ReactComponent。最后进行递归渲染。下面来看renderValidatedComponent()
```python
_renderValidatedComponent:
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
renderedComponent;
    ReactCurrentOwner.current =
```
```python
this
```
```python
;
```
```python
try
```
```python
{
      renderedComponent =
```
```python
this
```
```python
._renderValidatedComponentWithoutOwnerOrContext();
    }
```
```python
finally
```
```python
{
      ReactCurrentOwner.current =
```
```python
null
```
```python
;
    }
    !(
```
```python
return
```
```python
renderedComponent;
  },
  _renderValidatedComponentWithoutOwnerOrContext:
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
// 调用render方法，得到ReactElement。JSX经过babel转译后其实就是createElement()方法。这一点在前面也讲解过
```
```python
var
```
```python
renderedComponent = inst.render();
```
```python
return
```
```python
renderedComponent;
  },
```
# 3 存在期生命周期
组件实例对象已经生成时，我们可以通过setState()来更新组件。setState机制后面会有单独文章分析，现在只用知道它会调用updateComponent()来完成更新即可。下面来分析updateComponent
```python
updateComponent:
```
```python
function
```
```python
(transaction, prevParentElement, nextParentElement, prevUnmaskedContext, nextUnmaskedContext
  )
```
```python
{
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
var
```
```python
willReceive =
```
```python
false
```
```python
;
```
```python
var
```
```python
nextContext;
```
```python
var
```
```python
nextProps;
```
```python
// context对象如果有改动,则检查propTypes等,这在开发阶段可以报错提醒
```
```python
if
```
```python
(
```
```python
this
```
```python
._context === nextUnmaskedContext) {
      nextContext = inst.context;
    }
```
```python
else
```
```python
{
      nextContext =
```
```python
this
```
```python
._processContext(nextUnmaskedContext);
      willReceive =
```
```python
true
```
```python
;
    }
```
```python
// 如果父元素类型相同,则跳过propTypes类型检查
```
```python
if
```
```python
(prevParentElement === nextParentElement) {
      nextProps = nextParentElement.props;
    }
```
```python
else
```
```python
{
      nextProps =
```
```python
this
```
```python
._processProps(nextParentElement.props);
      willReceive =
```
```python
true
```
```python
;
    }
```
```python
// 调用componentWillReceiveProps,如果通过setState进入的updateComponent，则没有这一步
```
```python
if
```
```python
(willReceive && inst.componentWillReceiveProps) {
      inst.componentWillReceiveProps(nextProps, nextContext);
    }
```
```python
// 提前合并state,componentWillReceiveProps中调用setState不会重新渲染,在此处做合并即可,因为后面也是要调用render的
```
```python
// 这样可以避免没必要的渲染
```
```python
var
```
```python
nextState =
```
```python
this
```
```python
._processPendingState(nextProps, nextContext);
```
```python
// 调用shouldComponentUpdate给shouldUpdate赋值
```
```python
// 如果通过forceUpdate进入的updateComponent，即_pendingForceUpdate不为空，则不用判断shouldComponentUpdate.
```
```python
var
```
```python
shouldUpdate =
```
```python
true
```
```python
;
```
```python
if
```
```python
(!
```
```python
this
```
```python
._pendingForceUpdate && inst.shouldComponentUpdate) {
      shouldUpdate = inst.shouldComponentUpdate(nextProps, nextState, nextContext);
    }
```
```python
// 如果shouldUpdate为true,则会执行渲染,否则不会
```
```python
this
```
```python
._updateBatchNumber =
```
```python
null
```
```python
;
```
```python
if
```
```python
(shouldUpdate) {
```
```python
this
```
```python
._pendingForceUpdate =
```
```python
false
```
```python
;
```
```python
// 执行更新渲染,后面详细分析
```
```python
this
```
```python
._performComponentUpdate(
        nextParentElement,
        nextProps,
        nextState,
        nextContext,
        transaction,
        nextUnmaskedContext
      );
    }
```
```python
else
```
```python
{
```
```python
// shouldUpdate为false,则不会更新渲染
```
```python
this
```
```python
._currentElement = nextParentElement;
```
```python
this
```
```python
._context = nextUnmaskedContext;
      inst.props = nextProps;
      inst.state = nextState;
      inst.context = nextContext;
    }
},
```
updateComponent中，先调用componentWillReceiveProps，然后合并setState导致的state变化。然后调用shouldComponentUpdate判断是否需要更新渲染。如果需要，则调用_performComponentUpdate执行渲染更新，下面接着分析performComponentUpdate。
```python
_performComponentUpdate:
```
```python
function
```
```python
(nextElement,nextProps,nextState,nextContext,transaction,
                                     unmaskedContext
  )
```
```python
{
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
// 判断是否已经update了
```
```python
var
```
```python
hasComponentDidUpdate =
```
```python
Boolean
```
```python
(inst.componentDidUpdate);
```
```python
var
```
```python
prevProps;
```
```python
var
```
```python
prevState;
```
```python
var
```
```python
prevContext;
```
```python
if
```
```python
(hasComponentDidUpdate) {
      prevProps = inst.props;
      prevState = inst.state;
      prevContext = inst.context;
    }
```
```python
// render前调用componentWillUpdate
```
```python
if
```
```python
(inst.componentWillUpdate) {
      inst.componentWillUpdate(nextProps, nextState, nextContext);
    }
```
```python
// state props等属性设置到内部变量inst上
```
```python
this
```
```python
._currentElement = nextElement;
```
```python
this
```
```python
._context = unmaskedContext;
    inst.props = nextProps;
    inst.state = nextState;
    inst.context = nextContext;
```
```python
// 内部会调用render方法，重新解析ReactElement并得到HTML
```
```python
this
```
```python
._updateRenderedComponent(transaction, unmaskedContext);
```
```python
// render后调用componentDidUpdate
```
```python
if
```
```python
(hasComponentDidUpdate) {
      transaction.getReactMountReady().enqueue(
        inst.componentDidUpdate.bind(inst, prevProps, prevState, prevContext),
        inst
      );
    }
},
```
_performComponentUpdate会调用componentWillUpdate，然后在调用updateRenderedComponent进行更新渲染，最后调用componentDidUpdate。下面来看看updateRenderedComponent中怎么调用render方法的。
```python
_updateRenderedComponent:
```
```python
function
```
```python
(transaction, context)
```
```python
{
```
```python
var
```
```python
prevComponentInstance =
```
```python
this
```
```python
._renderedComponent;
```
```python
var
```
```python
prevRenderedElement = prevComponentInstance._currentElement;
```
```python
// _renderValidatedComponent内部会调用render,得到ReactElement
```
```python
var
```
```python
nextRenderedElement =
```
```python
this
```
```python
._renderValidatedComponent();
```
```python
// 判断是否做DOM diff。React为了简化递归diff,认为组件层级不变,且type和key不变(key用于listView等组件,很多时候我们没有设置type)才update,否则先unmount再重新mount
```
```python
if
```
```python
(shouldUpdateReactComponent(prevRenderedElement, nextRenderedElement)) {
```
```python
// 递归updateComponent,更新子组件的Virtual DOM
```
```python
ReactReconciler.receiveComponent(prevComponentInstance, nextRenderedElement, transaction,
```
```python
this
```
```python
._processChildContext(context));
    }
```
```python
else
```
```python
{
```
```python
var
```
```python
oldNativeNode = ReactReconciler.getNativeNode(prevComponentInstance);
```
```python
// 不做DOM diff,则先卸载掉,然后再加载。也就是先unMountComponent,再mountComponent
```
```python
ReactReconciler.unmountComponent(prevComponentInstance,
```
```python
false
```
```python
);
```
```python
this
```
```python
._renderedNodeType = ReactNodeTypes.getType(nextRenderedElement);
```
```python
// 由ReactElement创建ReactComponent
```
```python
this
```
```python
._renderedComponent =
```
```python
this
```
```python
._instantiateReactComponent(nextRenderedElement);
```
```python
// mountComponent挂载组件,得到组件对应HTML
```
```python
var
```
```python
nextMarkup = ReactReconciler.mountComponent(
```
```python
this
```
```python
._renderedComponent, transaction,
```
```python
this
```
```python
._nativeParent,
```
```python
this
```
```python
._nativeContainerInfo,
```
```python
this
```
```python
._processChildContext(context));
```
```python
// 将HTML插入DOM中
```
```python
this
```
```python
._replaceNodeWithMarkup(oldNativeNode, nextMarkup, prevComponentInstance);
    }
},
_renderValidatedComponent:
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
renderedComponent;
    ReactCurrentOwner.current =
```
```python
this
```
```python
;
```
```python
try
```
```python
{
      renderedComponent =
```
```python
this
```
```python
._renderValidatedComponentWithoutOwnerOrContext();
    }
```
```python
finally
```
```python
{
      ReactCurrentOwner.current =
```
```python
null
```
```python
;
    }
```
```python
return
```
```python
renderedComponent;
},
_renderValidatedComponentWithoutOwnerOrContext:
```
```python
function
```
```python
()
```
```python
{
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
// 看到render方法了把，应该放心了把~
```
```python
var
```
```python
renderedComponent = inst.render();
```
```python
return
```
```python
renderedComponent;
},
```
和mountComponent中一样，updateComponent也是用递归的方式将各子组件进行update的。这里要特别注意的是DOM diff。DOM diff是React中渲染加速的关键所在，它会帮我们算出virtual DOM中真正变化的部分，并对这部分进行原生DOM操作。为了避免循环递归对比节点的低效率，React中做了假设，即只对层级不变，type不变，key不变的组件进行Virtual DOM更新。这其中的关键是shouldUpdateReactComponent，下面分析
```python
function
```
```python
shouldUpdateReactComponent
```
```python
(prevElement, nextElement)
```
```python
{
```
```python
var
```
```python
prevEmpty = prevElement ===
```
```python
null
```
```python
|| prevElement ===
```
```python
false
```
```python
;
```
```python
var
```
```python
nextEmpty = nextElement ===
```
```python
null
```
```python
|| nextElement ===
```
```python
false
```
```python
;
```
```python
if
```
```python
(prevEmpty || nextEmpty) {
```
```python
return
```
```python
prevEmpty === nextEmpty;
  }
```
```python
var
```
```python
prevType =
```
```python
typeof
```
```python
prevElement;
```
```python
var
```
```python
nextType =
```
```python
typeof
```
```python
nextElement;
```
```python
// React DOM diff算法
```
```python
// 如果前后两次为数字或者字符,则认为只需要update(处理文本元素)
```
```python
// 如果前后两次为DOM元素或React元素,则必须在同一层级内,且type和key不变(key用于listView等组件,很多时候我们没有设置type)才update,否则先unmount再重新mount
```
```python
if
```
```python
(prevType ===
```
```python
'string'
```
```python
|| prevType ===
```
```python
'number'
```
```python
) {
```
```python
return
```
```python
(nextType ===
```
```python
'string'
```
```python
|| nextType ===
```
```python
'number'
```
```python
);
  }
```
```python
else
```
```python
{
```
```python
return
```
```python
(
      nextType ===
```
```python
'object'
```
```python
&&
      prevElement.type === nextElement.type &&
      prevElement.key === nextElement.key
    );
  }
}
```
# 4 销毁
前面提到过，更新组件时，如果不满足DOM diff条件，会先unmountComponent, 然后再mountComponent，下面我们来分析下unmountComponent时都发生了什么事。和mountComponent的多态一样，不同type的ReactComponent也会有不同的unmountComponent行为。我们来分析下React自定义组件，也就是ReactCompositeComponent中的unmountComponent。
```python
unmountComponent:
```
```python
function
```
```python
(safely)
```
```python
{
```
```python
if
```
```python
(!
```
```python
this
```
```python
._renderedComponent) {
```
```python
return
```
```python
;
    }
```
```python
var
```
```python
inst =
```
```python
this
```
```python
._instance;
```
```python
// 调用componentWillUnmount
```
```python
if
```
```python
(inst.componentWillUnmount && !inst._calledComponentWillUnmount) {
      inst._calledComponentWillUnmount =
```
```python
true
```
```python
;
```
```python
// 安全模式下，将componentWillUnmount包在try-catch中。否则直接componentWillUnmount
```
```python
if
```
```python
(safely) {
```
```python
var
```
```python
name =
```
```python
this
```
```python
.getName() +
```
```python
'.componentWillUnmount()'
```
```python
;
        ReactErrorUtils.invokeGuardedCallback(name, inst.componentWillUnmount.bind(inst));
      }
```
```python
else
```
```python
{
        inst.componentWillUnmount();
      }
    }
```
```python
// 递归调用unMountComponent来销毁子组件
```
```python
if
```
```python
(
```
```python
this
```
```python
._renderedComponent) {
      ReactReconciler.unmountComponent(
```
```python
this
```
```python
._renderedComponent, safely);
```
```python
this
```
```python
._renderedNodeType =
```
```python
null
```
```python
;
```
```python
this
```
```python
._renderedComponent =
```
```python
null
```
```python
;
```
```python
this
```
```python
._instance =
```
```python
null
```
```python
;
    }
```
```python
// reset等待队列和其他等待状态
```
```python
this
```
```python
._pendingStateQueue =
```
```python
null
```
```python
;
```
```python
this
```
```python
._pendingReplaceState =
```
```python
false
```
```python
;
```
```python
this
```
```python
._pendingForceUpdate =
```
```python
false
```
```python
;
```
```python
this
```
```python
._pendingCallbacks =
```
```python
null
```
```python
;
```
```python
this
```
```python
._pendingElement =
```
```python
null
```
```python
;
```
```python
// reset内部变量,防止内存泄漏
```
```python
this
```
```python
._context =
```
```python
null
```
```python
;
```
```python
this
```
```python
._rootNodeID =
```
```python
null
```
```python
;
```
```python
this
```
```python
._topLevelWrapper =
```
```python
null
```
```python
;
```
```python
// 将组件从map中移除,还记得我们在mountComponent中将它加入了map中的吧
```
```python
ReactInstanceMap.remove(inst);
  },
```
可见，unmountComponent还是比较简单的，它就做三件事
调用componentWillUnmount()
递归调用unmountComponent(),销毁子组件
将内部变量置空，防止内存泄漏
# 5 总结
React自定义组件创建期，存在期，销毁期三个阶段的生命周期调用上面都讲完了。三个入口函数mountComponent，updateComponent，unmountComponent尤其关键。大家如果有兴趣，还可以自行分析ReactDOMEmptyComponent，ReactDOMComponent和ReactDOMTextComponent的这三个方法。
深入学习React生命周期源码可以帮我们理清各个方法的调用顺序，明白它们都是什么时候被调用的，哪些条件下才会被调用等等。阅读源码虽然有点枯燥，但能够大大加深对上层API接口的理解，并体会设计者设计这些API的良苦用心。

