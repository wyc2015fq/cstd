
# React源码分析5 — setState机制 - 谢杨易的博客 - CSDN博客

2017年03月02日 11:57:31[谢杨易](https://me.csdn.net/u013510838)阅读数：5919标签：[React																](https://so.csdn.net/so/search/s.do?q=React&t=blog)[setState																](https://so.csdn.net/so/search/s.do?q=setState&t=blog)[源码																](https://so.csdn.net/so/search/s.do?q=源码&t=blog)[刷新																](https://so.csdn.net/so/search/s.do?q=刷新&t=blog)[render																](https://so.csdn.net/so/search/s.do?q=render&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=刷新&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=源码&t=blog)个人分类：[前端																](https://blog.csdn.net/u013510838/article/category/6726259)
[
																								](https://so.csdn.net/so/search/s.do?q=源码&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=setState&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=setState&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=React&t=blog)

# 1 概述
React作为一门前端框架，虽然只是focus在MVVM中的View部分，但还是实现了View和model的绑定。修改数据的同时，可以实现View的刷新。这大大简化了我们的逻辑，只用关心数据流的变化，同时减少了代码量，使得后期维护也更加方便。这个特性则要归功于setState()方法。React中利用队列机制来管理state，避免了很多重复的View刷新。下面我们来从源码角度探寻下setState机制。
# 2 setState和replaceState
我们都知道setState是以修改和新增的方式改变state的，不会改变没有涉及到的state。而replaceState则用新的state完全替换掉老state。比如
```python
this
```
```python
.state = {
  title:
```
```python
"example"
```
```python
,
  desc:
```
```python
"this is an example for react"
```
```python
};
setState({
  title:
```
```python
"new example"
```
```python
});
console.log(
```
```python
"setState: "
```
```python
+
```
```python
JSON
```
```python
.stringify(
```
```python
this
```
```python
.state));
```
```python
// 1
```
```python
replaceState({
  title:
```
```python
"new example"
```
```python
})
console.log(
```
```python
"replaceState: "
```
```python
+
```
```python
JSON
```
```python
.stringify(
```
```python
this
```
```python
.state));
```
```python
// 2
```
打印如下
```python
setState: {
```
```python
"title"
```
```python
:
```
```python
"new example"
```
```python
,
```
```python
"desc"
```
```python
:
```
```python
"this is an example for react"
```
```python
}
replaceState: {
```
```python
"title"
```
```python
:
```
```python
"new example"
```
```python
}
```
可见，setState不会影响没有涉及到的state，而replaceState则是完完全全的替换。下面让我们进入源码来探寻究竟吧。
## setState
setState方法入口如下
```python
ReactComponent.prototype.setState =
```
```python
function
```
```python
(partialState, callback)
```
```python
{
```
```python
// 将setState事务放入队列中
```
```python
this
```
```python
.updater.enqueueSetState(
```
```python
this
```
```python
, partialState);
```
```python
if
```
```python
(callback) {
```
```python
this
```
```python
.updater.enqueueCallback(
```
```python
this
```
```python
, callback,
```
```python
'setState'
```
```python
);
  }
};
```
取名为partialState，有部分state的含义，可见只是影响涉及到的state，不会伤及无辜。enqueueSetState是state队列管理的入口方法，比较重要，我们之后再接着分析。
## replaceState
```python
replaceState:
```
```python
function
```
```python
(newState, callback)
```
```python
{
```
```python
this
```
```python
.updater.enqueueReplaceState(
```
```python
this
```
```python
, newState);
```
```python
if
```
```python
(callback) {
```
```python
this
```
```python
.updater.enqueueCallback(
```
```python
this
```
```python
, callback,
```
```python
'replaceState'
```
```python
);
  }
},
```
replaceState中取名为newState，有完全替换的含义。同样也是以队列的形式来管理的。
# 3 enqueueSetState
```python
enqueueSetState:
```
```python
function
```
```python
(publicInstance, partialState)
```
```python
{
```
```python
// 先获取ReactComponent组件对象
```
```python
var
```
```python
internalInstance = getInternalInstanceReadyForUpdate(publicInstance,
```
```python
'setState'
```
```python
);
```
```python
if
```
```python
(!internalInstance) {
```
```python
return
```
```python
;
    }
```
```python
// 如果_pendingStateQueue为空,则创建它。可以发现队列是数组形式实现的
```
```python
var
```
```python
queue = internalInstance._pendingStateQueue || (internalInstance._pendingStateQueue = []);
    queue.push(partialState);
```
```python
// 将要更新的ReactComponent放入数组中
```
```python
enqueueUpdate(internalInstance);
}
```
其中getInternalInstanceReadyForUpdate源码如下，解释都在代码注释中
```python
function
```
```python
getInternalInstanceReadyForUpdate
```
```python
(publicInstance, callerName)
```
```python
{
```
```python
// 从map取出ReactComponent组件,还记得mountComponent时把ReactElement作为key，将ReactComponent存入了map中了吧，ReactComponent是React组件的核心，包含各种状态，数据和操作方法。而ReactElement则仅仅是一个数据类。
```
```python
var
```
```python
internalInstance = ReactInstanceMap.
```
```python
get
```
```python
(publicInstance);
```
```python
if
```
```python
(!internalInstance) {
```
```python
return
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
internalInstance;
}
```
enqueueUpdate源码如下
```python
function
```
```python
enqueueUpdate
```
```python
(component)
```
```python
{
```
```python
ensureInjected();
```
```python
// 如果不是正处于创建或更新组件阶段,则处理update事务
```
```python
if
```
```python
(!batchingStrategy.isBatchingUpdates) {
    batchingStrategy.batchedUpdates(enqueueUpdate, component);
```
```python
return
```
```python
;
  }
```
```python
// 如果正在创建或更新组件,则暂且先不处理update,只是将组件放在dirtyComponents数组中
```
```python
dirtyComponents.push(component);
}
```
enqueueUpdate包含了React避免重复render的逻辑。mountComponent和updateComponent方法在执行的最开始，会调用到batchedUpdates进行批处理更新，此时会将isBatchingUpdates设置为true，也就是将状态标记为现在正处于更新阶段了。之后React以事务的方式处理组件update，事务处理完后会调用wrapper.close(), 而TRANSACTION_WRAPPERS中包含了RESET_BATCHED_UPDATES这个wrapper，故最终会调用RESET_BATCHED_UPDATES.close(), 它最终会将isBatchingUpdates设置为false。这个过程比较麻烦，想更清晰的了解的话，建议自行分析源码。
故getInitialState，componentWillMount， render，componentWillUpdate 中setState都不会引起updateComponent。但在componentDidMount和componentDidUpdate中则会。
```python
batchedUpdates:
```
```python
function
```
```python
(callback, a, b, c, d, e)
```
```python
{
```
```python
var
```
```python
alreadyBatchingUpdates = ReactDefaultBatchingStrategy.isBatchingUpdates;
```
```python
// 批处理最开始时，将isBatchingUpdates设为true，表明正在更新
```
```python
ReactDefaultBatchingStrategy.isBatchingUpdates =
```
```python
true
```
```python
;
```
```python
// The code is written this way to avoid extra allocations
```
```python
if
```
```python
(alreadyBatchingUpdates) {
    callback(a, b, c, d, e);
  }
```
```python
else
```
```python
{
```
```python
// 以事务的方式处理updates，后面详细分析transaction
```
```python
transaction.perform(callback,
```
```python
null
```
```python
, a, b, c, d, e);
  }
}
```
```python
var
```
```python
RESET_BATCHED_UPDATES = {
  initialize: emptyFunction,
  close:
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
// 事务批更新处理结束时，将isBatchingUpdates设为了false
```
```python
ReactDefaultBatchingStrategy.isBatchingUpdates =
```
```python
false
```
```python
;
  }
};
```
```python
var
```
```python
TRANSACTION_WRAPPERS = [FLUSH_BATCHED_UPDATES, RESET_BATCHED_UPDATES];
```
# 4 事务transaction
事务通过wrapper进行封装。一个wrapper包含一对initialize和close方法。比如RESET_BATCHED_UPDATES
```python
var
```
```python
RESET_BATCHED_UPDATES = {
```
```python
// 初始化调用
```
```python
initialize: emptyFunction,
```
```python
// 事务执行完成，close时调用
```
```python
close:
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
ReactDefaultBatchingStrategy.isBatchingUpdates =
```
```python
false
```
```python
;
  }
};
```
transcation被包装在wrapper中，比如
```python
var
```
```python
TRANSACTION_WRAPPERS = [FLUSH_BATCHED_UPDATES, RESET_BATCHED_UPDATES];
```
transaction是通过transaction.perform(callback, args…)方法进入的，它会先调用注册好的wrapper中的initialize方法，然后执行perform方法中的callback，最后再执行close方法。截取React代码中的一张图如下
![Markdown](http://p1.bpimg.com/573600/00288c9c094acee3.png)
下面分析transaction.perform(callback, args…)
```python
perform:
```
```python
function
```
```python
(method, scope, a, b, c, d, e, f)
```
```python
{
```
```python
var
```
```python
errorThrown;
```
```python
var
```
```python
ret;
```
```python
try
```
```python
{
```
```python
this
```
```python
._isInTransaction =
```
```python
true
```
```python
;
      errorThrown =
```
```python
true
```
```python
;
```
```python
// 先运行所有wrapper中的initialize方法
```
```python
this
```
```python
.initializeAll(
```
```python
0
```
```python
);
```
```python
// 再执行perform方法传入的callback
```
```python
ret = method.call(scope, a, b, c, d, e, f);
      errorThrown =
```
```python
false
```
```python
;
    }
```
```python
finally
```
```python
{
```
```python
try
```
```python
{
```
```python
if
```
```python
(errorThrown) {
```
```python
// 最后运行wrapper中的close方法
```
```python
try
```
```python
{
```
```python
this
```
```python
.closeAll(
```
```python
0
```
```python
);
          }
```
```python
catch
```
```python
(err) {}
        }
```
```python
else
```
```python
{
```
```python
// 最后运行wrapper中的close方法
```
```python
this
```
```python
.closeAll(
```
```python
0
```
```python
);
        }
      }
```
```python
finally
```
```python
{
```
```python
this
```
```python
._isInTransaction =
```
```python
false
```
```python
;
      }
    }
```
```python
return
```
```python
ret;
  },
  initializeAll:
```
```python
function
```
```python
(startIndex)
```
```python
{
```
```python
var
```
```python
transactionWrappers =
```
```python
this
```
```python
.transactionWrappers;
```
```python
// 遍历所有注册的wrapper
```
```python
for
```
```python
(
```
```python
var
```
```python
i = startIndex; i < transactionWrappers.length; i++) {
```
```python
var
```
```python
wrapper = transactionWrappers[i];
```
```python
try
```
```python
{
```
```python
this
```
```python
.wrapperInitData[i] = Transaction.OBSERVED_ERROR;
```
```python
// 调用wrapper的initialize方法
```
```python
this
```
```python
.wrapperInitData[i] = wrapper.initialize ? wrapper.initialize.call(
```
```python
this
```
```python
) :
```
```python
null
```
```python
;
      }
```
```python
finally
```
```python
{
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
.wrapperInitData[i] === Transaction.OBSERVED_ERROR) {
```
```python
try
```
```python
{
```
```python
this
```
```python
.initializeAll(i +
```
```python
1
```
```python
);
          }
```
```python
catch
```
```python
(err) {}
        }
      }
    }
  },
  closeAll:
```
```python
function
```
```python
(startIndex)
```
```python
{
```
```python
var
```
```python
transactionWrappers =
```
```python
this
```
```python
.transactionWrappers;
```
```python
// 遍历所有wrapper
```
```python
for
```
```python
(
```
```python
var
```
```python
i = startIndex; i < transactionWrappers.length; i++) {
```
```python
var
```
```python
wrapper = transactionWrappers[i];
```
```python
var
```
```python
initData =
```
```python
this
```
```python
.wrapperInitData[i];
```
```python
var
```
```python
errorThrown;
```
```python
try
```
```python
{
        errorThrown =
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
(initData !== Transaction.OBSERVED_ERROR && wrapper.close) {
```
```python
// 调用wrapper的close方法，如果有的话
```
```python
wrapper.close.call(
```
```python
this
```
```python
, initData);
        }
        errorThrown =
```
```python
false
```
```python
;
      }
```
```python
finally
```
```python
{
```
```python
if
```
```python
(errorThrown) {
```
```python
try
```
```python
{
```
```python
this
```
```python
.closeAll(i +
```
```python
1
```
```python
);
          }
```
```python
catch
```
```python
(e) {}
        }
      }
    }
```
```python
this
```
```python
.wrapperInitData.length =
```
```python
0
```
```python
;
  }
```
# 5 runBatchedUpdates更新组件
前面分析到enqueueUpdate中调用transaction.perform(callback, args…)后，小伙伴们肯定会发现，callback还是enqueueUpdate方法啊，那岂不是死循环了？不是说好的setState会调用updateComponent，从而自动刷新View的吗？别急，我们还是要先从transaction事务说起。
我们的wrapper中注册了两个wrapper，如下
```python
var
```
```python
TRANSACTION_WRAPPERS = [FLUSH_BATCHED_UPDATES, RESET_BATCHED_UPDATES];
```
RESET_BATCHED_UPDATES用来管理isBatchingUpdates状态，我们前面在分析setState是否立即生效时已经讲解过了。那FLUSH_BATCHED_UPDATES用来干嘛呢？
```python
var
```
```python
FLUSH_BATCHED_UPDATES = {
  initialize: emptyFunction,
  close: ReactUpdates.flushBatchedUpdates.bind(ReactUpdates)
};
```
```python
var
```
```python
flushBatchedUpdates =
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
// 循环遍历处理完所有dirtyComponents
```
```python
while
```
```python
(dirtyComponents.length || asapEnqueued) {
```
```python
if
```
```python
(dirtyComponents.length) {
```
```python
var
```
```python
transaction = ReactUpdatesFlushTransaction.getPooled();
```
```python
// close前执行完runBatchedUpdates方法，这是关键
```
```python
transaction.perform(runBatchedUpdates,
```
```python
null
```
```python
, transaction);
      ReactUpdatesFlushTransaction.release(transaction);
    }
```
```python
if
```
```python
(asapEnqueued) {
      asapEnqueued =
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
queue = asapCallbackQueue;
      asapCallbackQueue = CallbackQueue.getPooled();
      queue.notifyAll();
      CallbackQueue.release(queue);
    }
  }
};
```
FLUSH_BATCHED_UPDATES会在一个transaction的close阶段运行runBatchedUpdates，从而执行update。
```python
function
```
```python
runBatchedUpdates
```
```python
(transaction)
```
```python
{
```
```python
var
```
```python
len = transaction.dirtyComponentsLength;
  dirtyComponents.sort(mountOrderComparator);
```
```python
for
```
```python
(
```
```python
var
```
```python
i =
```
```python
0
```
```python
; i < len; i++) {
```
```python
// dirtyComponents中取出一个component
```
```python
var
```
```python
component = dirtyComponents[i];
```
```python
// 取出dirtyComponent中的未执行的callback,下面就准备执行它了
```
```python
var
```
```python
callbacks = component._pendingCallbacks;
    component._pendingCallbacks =
```
```python
null
```
```python
;
```
```python
var
```
```python
markerName;
```
```python
if
```
```python
(ReactFeatureFlags.logTopLevelRenders) {
```
```python
var
```
```python
namedComponent = component;
```
```python
if
```
```python
(component._currentElement.props === component._renderedComponent._currentElement) {
        namedComponent = component._renderedComponent;
      }
    }
```
```python
// 执行updateComponent
```
```python
ReactReconciler.performUpdateIfNecessary(component, transaction.reconcileTransaction);
```
```python
// 执行dirtyComponent中之前未执行的callback
```
```python
if
```
```python
(callbacks) {
```
```python
for
```
```python
(
```
```python
var
```
```python
j =
```
```python
0
```
```python
; j < callbacks.length; j++) {
        transaction.callbackQueue.enqueue(callbacks[j], component.getPublicInstance());
      }
    }
  }
}
```
runBatchedUpdates循环遍历dirtyComponents数组，主要干两件事。首先执行performUpdateIfNecessary来刷新组件的view，然后执行之前阻塞的callback。下面来看performUpdateIfNecessary。
```python
performUpdateIfNecessary:
```
```python
function
```
```python
(transaction)
```
```python
{
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
._pendingElement !=
```
```python
null
```
```python
) {
```
```python
// receiveComponent会最终调用到updateComponent，从而刷新View
```
```python
ReactReconciler.receiveComponent(
```
```python
this
```
```python
,
```
```python
this
```
```python
._pendingElement, transaction,
```
```python
this
```
```python
._context);
    }
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
._pendingStateQueue !==
```
```python
null
```
```python
||
```
```python
this
```
```python
._pendingForceUpdate) {
```
```python
// 执行updateComponent，从而刷新View。这个流程在React生命周期中讲解过
```
```python
this
```
```python
.updateComponent(transaction,
```
```python
this
```
```python
._currentElement,
```
```python
this
```
```python
._currentElement,
```
```python
this
```
```python
._context,
```
```python
this
```
```python
._context);
    }
  },
```
最后惊喜的看到了receiveComponent和updateComponent吧。receiveComponent最后会调用updateComponent，而updateComponent中会执行React组件存在期的生命周期方法，如componentWillReceiveProps， shouldComponentUpdate， componentWillUpdate，render, componentDidUpdate。 从而完成组件更新的整套流程。
# 6 总结
setState流程还是很复杂的，设计也很精巧，避免了重复无谓的刷新组件。它的主要流程如下
enqueueSetState将state放入队列中，并调用enqueueUpdate处理要更新的Component
如果组件当前正处于update事务中，则先将Component存入dirtyComponent中。否则调用batchedUpdates处理。
batchedUpdates发起一次transaction.perform()事务
开始执行事务初始化，运行，结束三个阶段
初始化：事务初始化阶段没有注册方法，故无方法要执行
运行：执行setSate时传入的callback方法，一般不会传callback参数
结束：更新isBatchingUpdates为false，并执行FLUSH_BATCHED_UPDATES这个wrapper中的close方法
FLUSH_BATCHED_UPDATES在close阶段，会循环遍历所有的dirtyComponents，调用updateComponent刷新组件，并执行它的pendingCallbacks, 也就是setState中设置的callback。

