
# React源码分析2 — 组件和对象的创建（createClass，createElement） - 谢杨易的博客 - CSDN博客

2017年02月18日 18:44:41[谢杨易](https://me.csdn.net/u013510838)阅读数：6641



# 1 组件的创建
React受大家欢迎的一个重要原因就是可以自定义组件。这样的一方面可以复用开发好的组件，实现一处开发，处处调用，另外也能使用别人开发好的组件，提高封装性。另一方面使得代码结构很清晰，组件间耦合减少，方便维护。ES5创建组件时，调用React.createClass()即可. ES6中使用class myComponent extends React.Component, 其实内部还是调用createClass创建组件。
组件创建我们可以简单类比为Java中ClassLoader加载class。下面来分析下createClass的源码，我们省去了开发阶段错误提示的相关代码，如propType的检查。（if (“development” !== ‘production’) {}代码段都不进行分析了，这些只在开发调试阶段调用）
```python
createClass:
```
```python
function
```
```python
(spec)
```
```python
{
```
```python
var
```
```python
Constructor =
```
```python
function
```
```python
(props, context, updater)
```
```python
{
```
```python
// 触发自动绑定
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
.__reactAutoBindPairs.length) {
        bindAutoBindMethods(
```
```python
this
```
```python
);
      }
```
```python
// 初始化参数
```
```python
this
```
```python
.props = props;
```
```python
this
```
```python
.context = context;
```
```python
this
```
```python
.refs = emptyObject;
```
```python
// 本组件对象的引用，可以利用它来调用组件的方法
```
```python
this
```
```python
.updater = updater || ReactNoopUpdateQueue;
```
```python
// 调用getInitialState()来初始化state变量
```
```python
this
```
```python
.state =
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
initialState =
```
```python
this
```
```python
.getInitialState ?
```
```python
this
```
```python
.getInitialState() :
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
.state = initialState;
    };
```
```python
// 继承父类
```
```python
Constructor.prototype =
```
```python
new
```
```python
ReactClassComponent();
    Constructor.prototype.constructor = Constructor;
    Constructor.prototype.__reactAutoBindPairs = [];
    injectedMixins.forEach(mixSpecIntoComponent.bind(
```
```python
null
```
```python
, Constructor));
    mixSpecIntoComponent(Constructor, spec);
```
```python
// 调用getDefaultProps，并挂载到组件类上。defaultProps是类变量，使用ES6写法时更清晰
```
```python
if
```
```python
(Constructor.getDefaultProps) {
      Constructor.defaultProps = Constructor.getDefaultProps();
    }
```
```python
// React中暴露给应用调用的方法，如render componentWillMount。
```
```python
// 如果应用未设置，则将他们设为null
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
methodName
```
```python
in
```
```python
ReactClassInterface) {
```
```python
if
```
```python
(!Constructor.prototype[methodName]) {
        Constructor.prototype[methodName] =
```
```python
null
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
Constructor;
  },
```
createClass主要做的事情有
定义构造方法Constructor，构造方法中进行props，refs等的初始化，并调用getInitialState来初始化state
调用getDefaultProps，并放在defaultProps类变量上。这个变量不属于某个单独的对象。可理解为static 变量
将React中暴露给应用，但应用中没有设置的方法，设置为null。
# 2 对象的创建
JSX中创建React元素最终会被babel转译为createElement(type, config, children), babel根据JSX中标签的首字母来判断是原生DOM组件，还是自定义React组件。如果首字母大写，则为React组件。这也是为什么ES6中React组件类名必须大写的原因。如下面代码
```python
<
```
```python
div
```
```python
className
```
```python
=
```
```python
"title"
```
```python
ref
```
```python
=
```
```python
"example"
```
```python
>
```
```python
<
```
```python
span
```
```python
>
```
```python
123
```
```python
</
```
```python
span
```
```python
>
```
```python
// 原生DOM组件，首字母小写
```
```python
<
```
```python
ErrorPage
```
```python
title
```
```python
=
```
```python
'123456'
```
```python
desc
```
```python
=
```
```python
{[]}
```
```python
/>
```
```python
// 自定义组件，首字母大写
```
```python
</
```
```python
div
```
```python
>
```
转译完后是
```python
React.createElement(
```
```python
'div'
```
```python
,
```
```python
// type,标签名,原生DOM对象为String
```
```python
{
            className:
```
```python
'title'
```
```python
,
            ref:
```
```python
'example'
```
```python
},
```
```python
// config，属性
```
```python
React.createElement(
```
```python
'span'
```
```python
,
```
```python
null
```
```python
,
```
```python
'123'
```
```python
),
```
```python
// children，子元素
```
```python
React.createElement(
```
```python
// type,标签名,React自定义组件的type不为String.
```
```python
// _errorPage2.default为从其他文件中引入的React组件
```
```python
_errorPage2.default,    
            {
                title:
```
```python
'123456'
```
```python
,
                desc: []
            }
        )
```
```python
// children，子元素
```
```python
)
```
下面来分析下createElement的源码
```python
ReactElement.createElement =
```
```python
function
```
```python
(type, config, children)
```
```python
{
```
```python
var
```
```python
propName;
```
```python
// 初始化参数
```
```python
var
```
```python
props = {};
```
```python
var
```
```python
key =
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
ref =
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
self =
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
source =
```
```python
null
```
```python
;
```
```python
// 从config中提取出内容，如ref key props
```
```python
if
```
```python
(config !=
```
```python
null
```
```python
) {
    ref = config.ref ===
```
```python
undefined
```
```python
?
```
```python
null
```
```python
: config.ref;
    key = config.key ===
```
```python
undefined
```
```python
?
```
```python
null
```
```python
:
```
```python
''
```
```python
+ config.key;
    self = config.__self ===
```
```python
undefined
```
```python
?
```
```python
null
```
```python
: config.__self;
    source = config.__source ===
```
```python
undefined
```
```python
?
```
```python
null
```
```python
: config.__source;
```
```python
// 提取出config中的prop，放入props变量中
```
```python
for
```
```python
(propName
```
```python
in
```
```python
config) {
```
```python
if
```
```python
(config.hasOwnProperty(propName) && !RESERVED_PROPS.hasOwnProperty(propName)) {
        props[propName] = config[propName];
      }
    }
  }
```
```python
// 处理children，挂到props的children属性下
```
```python
// 入参的前两个为type和config，后面的就都是children参数了。故需要减2
```
```python
var
```
```python
childrenLength =
```
```python
arguments
```
```python
.length -
```
```python
2
```
```python
;
```
```python
if
```
```python
(childrenLength ===
```
```python
1
```
```python
) {
```
```python
// 只有一个参数时，直接挂到children属性下，不是array的方式
```
```python
props.children = children;
  }
```
```python
else
```
```python
if
```
```python
(childrenLength >
```
```python
1
```
```python
) {
```
```python
// 不止一个时，放到array中，然后将array挂到children属性下
```
```python
var
```
```python
childArray =
```
```python
Array
```
```python
(childrenLength);
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
; i < childrenLength; i++) {
      childArray[i] =
```
```python
arguments
```
```python
[i +
```
```python
2
```
```python
];
    }
    props.children = childArray;
  }
```
```python
// 取出组件类中的静态变量defaultProps，并给未在JSX中设置值的属性设置默认值
```
```python
if
```
```python
(type && type.defaultProps) {
```
```python
var
```
```python
defaultProps = type.defaultProps;
```
```python
for
```
```python
(propName
```
```python
in
```
```python
defaultProps) {
```
```python
if
```
```python
(props[propName] ===
```
```python
undefined
```
```python
) {
        props[propName] = defaultProps[propName];
      }
    }
  }
```
```python
// 返回一个ReactElement对象
```
```python
return
```
```python
ReactElement(type, key, ref, self, source, ReactCurrentOwner.current, props);
};
```
下面来看ReactElement源码
```python
var
```
```python
ReactElement =
```
```python
function
```
```python
(type, key, ref, self, source, owner, props)
```
```python
{
```
```python
var
```
```python
element = {
```
```python
// This tag allow us to uniquely identify this as a React Element
```
```python
$$
```
```python
typeof
```
```python
: REACT_ELEMENT_TYPE,
```
```python
// ReactElement对象上的四个变量，特别关键
```
```python
type: type,
    key: key,
    ref: ref,
    props: props,
```
```python
// Record the component responsible for creating this element.
```
```python
_owner: owner
  };
```
```python
return
```
```python
element;
}
```
可以看到仅仅是给ReactElement对象内的成员变量赋值而已，不在赘述。
流程如如下
![Markdown](http://p1.bqimg.com/1949/366f3f29e30f2098.png)
# 3 组件对象初始化
在mountComponent()挂载组件中，会进行组件渲染，调用到instantiateReactComponent()方法。这个过程我们在React生命周期方法中再详细讲述，这里有个大体了解即可。instantiateReactComponent()根据ReactElement中不同的type字段，创建不同类型的组件对象。源码如下
```python
// 初始化组件对象，node是一个ReactElement对象，是节点元素在React中的表示
```
```python
function
```
```python
instantiateReactComponent
```
```python
(node)
```
```python
{
```
```python
var
```
```python
instance;
```
```python
var
```
```python
isEmpty = node ===
```
```python
null
```
```python
|| node ===
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
(isEmpty) {
```
```python
// 空对象
```
```python
instance = ReactEmptyComponent.create(instantiateReactComponent);
  }
```
```python
else
```
```python
if
```
```python
(
```
```python
typeof
```
```python
node ===
```
```python
'object'
```
```python
) {
```
```python
// 组件对象，包括DOM原生的和React自定义组件
```
```python
var
```
```python
element = node;
```
```python
// 根据ReactElement中的type字段区分
```
```python
if
```
```python
(
```
```python
typeof
```
```python
element.type ===
```
```python
'string'
```
```python
) {
```
```python
// type为string则表示DOM原生对象，比如div span等。可以参看上面babel转译的那段代码
```
```python
instance = ReactNativeComponent.createInternalComponent(element);
    }
```
```python
else
```
```python
if
```
```python
(isInternalComponentType(element.type)) {
```
```python
// 保留给以后版本使用，此处暂时不会涉及到
```
```python
instance =
```
```python
new
```
```python
element.type(element);
    }
```
```python
else
```
```python
{
```
```python
// React自定义组件
```
```python
instance =
```
```python
new
```
```python
ReactCompositeComponentWrapper(element);
    }
  }
```
```python
else
```
```python
if
```
```python
(
```
```python
typeof
```
```python
node ===
```
```python
'string'
```
```python
||
```
```python
typeof
```
```python
node ===
```
```python
'number'
```
```python
) {
```
```python
// 元素是一个string时，对应的比如<span>123</span> 中的123
```
```python
// 本质上它不是一个ReactElement，但为了统一，也按照同样流程处理，称为ReactDOMTextComponent
```
```python
instance = ReactNativeComponent.createInstanceForText(node);
  }
```
```python
else
```
```python
{
```
```python
// 无处理
```
```python
}
```
```python
// 初始化参数，这两个参数是DOM diff时用到的
```
```python
instance._mountIndex =
```
```python
0
```
```python
;
  instance._mountImage =
```
```python
null
```
```python
;
```
```python
return
```
```python
instance;
}
```
故我们可以看到有四种创建组件元素的方式，同时对应四种ReactElement
ReactEmptyComponent.create(), 创建空对象ReactDOMEmptyComponent
ReactNativeComponent.createInternalComponent(), 创建DOM原生对象 ReactDOMComponent
new ReactCompositeComponentWrapper(), 创建React自定义对象ReactCompositeComponent
ReactNativeComponent.createInstanceForText(), 创建文本对象 ReactDOMTextComponent
下面分别分析这几种对象，和创建他们的过程。
## ReactDOMEmptyComponent
由ReactEmptyComponent.create()创建，最终生成ReactDOMEmptyComponent对象，源码如下
```python
var
```
```python
emptyComponentFactory;
```
```python
var
```
```python
ReactEmptyComponentInjection = {
  injectEmptyComponentFactory:
```
```python
function
```
```python
(factory)
```
```python
{
```
```python
emptyComponentFactory = factory;
  }
};
```
```python
var
```
```python
ReactEmptyComponent = {
  create:
```
```python
function
```
```python
(instantiate)
```
```python
{
```
```python
return
```
```python
emptyComponentFactory(instantiate);
  }
};
ReactEmptyComponent.injection = ReactEmptyComponentInjection;
ReactInjection.EmptyComponent.injectEmptyComponentFactory(
```
```python
function
```
```python
(instantiate)
```
```python
{
```
```python
// 前面比较绕，关键就是这句话，创建ReactDOMEmptyComponent对象
```
```python
return
```
```python
new
```
```python
ReactDOMEmptyComponent(instantiate);
});
```
```python
// 各种null，就不分析了
```
```python
var
```
```python
ReactDOMEmptyComponent =
```
```python
function
```
```python
(instantiate)
```
```python
{
```
```python
this
```
```python
._currentElement =
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
._nativeNode =
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
._nativeParent =
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
._nativeContainerInfo =
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
._domID =
```
```python
null
```
```python
;
};
```
## ReactDOMComponent
由ReactNativeComponent.createInternalComponent()创建。这里注意原生组件不代表是DOM组件，而是React封装过的Virtual DOM对象。React并不直接操作原生DOM。
大家可以自己看ReactDOMComponent的源码。重点看下ReactDOMComponent.Mixin
```python
ReactDOMComponent.Mixin = {
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
},
  _createOpenTagMarkupAndPutListeners:
```
```python
function
```
```python
(transaction, props)
```
```python
{
```
```python
},
  _createContentMarkup:
```
```python
function
```
```python
(transaction, props, context)
```
```python
{
```
```python
},
  _createInitialChildren:
```
```python
function
```
```python
(transaction, props, context, lazyTree)
```
```python
{
```
```python
}
  receiveComponent:
```
```python
function
```
```python
(nextElement, transaction, context)
```
```python
{
```
```python
},
  updateComponent:
```
```python
function
```
```python
(transaction, prevElement, nextElement, context)
```
```python
{
```
```python
},
  _updateDOMProperties:
```
```python
function
```
```python
(lastProps, nextProps, transaction)
```
```python
{
```
```python
},
  _updateDOMChildren:
```
```python
function
```
```python
(lastProps, nextProps, transaction, context)
```
```python
{
```
```python
},
  getNativeNode:
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
},
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
},
  getPublicInstance:
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
}
}
```
其中暴露给外部的比较关键的是mountComponent，receiveComponen， updateComponent，unmountComponent。他们会引发React生命周期方法的调用，下一节再讲。
## ReactCompositeComponent
由new ReactCompositeComponentWrapper()创建，重点看下ReactCompositeComponentMixin
```python
var
```
```python
ReactCompositeComponentMixin = {
```
```python
// new对应的方法，创建ReactCompositeComponent对象
```
```python
construct:
```
```python
function
```
```python
(element)
```
```python
{
```
```python
},
  mountComponent,
```
```python
// 初始挂载组件时被调用，仅一次
```
```python
performInitialMountWithErrorHandling,
```
```python
// 和performInitialMount相近，只是多了错误处理
```
```python
performInitialMount,
```
```python
// 执行mountComponent的渲染阶段，会调用到instantiateReactComponent，从而进入初始化React组件的入口
```
```python
getNativeNode,
  unmountComponent,
```
```python
// 卸载组件，内存释放等工作
```
```python
receiveComponent,
  performUpdateIfNecessary,
  updateComponent,
```
```python
// setState后被调用，重新渲染组件
```
```python
attachRef,
```
```python
// 将ref指向组件对象，这样我们就可以利用它调用对象内的方法了
```
```python
detachRef,
```
```python
// 将组件的引用从全局对象refs中删掉，这样我们就不能利用ref找到组件对象了
```
```python
instantiateReactComponent，
```
```python
// 初始化React组件的入口，在mountComponent时的渲染阶段会被调用
```
```python
}
```
## ReactDOMTextComponent
由ReactNativeComponent.createInstanceForText()创建，我们也不细细分析了，主要入口代码如下，大家可以自行分析。
```python
var
```
```python
ReactDOMTextComponent =
```
```python
function
```
```python
(text)
```
```python
{
```
```python
this
```
```python
._currentElement = text;
```
```python
this
```
```python
._stringText =
```
```python
''
```
```python
+ text;
};
_assign(ReactDOMTextComponent.prototype, {
  mountComponent,
  receiveComponent,
  getNativeNode,
  unmountComponent
}
```

