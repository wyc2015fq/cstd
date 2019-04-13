
# React源码分析1 -- 框架 - 谢杨易的博客 - CSDN博客

2017年02月18日 18:42:01[谢杨易](https://me.csdn.net/u013510838)阅读数：4591



# 1 源码结构
我们分析的React源码version为16.0.0-alpha.3，源码目录如下图所示。
![Markdown](http://p1.bqimg.com/573600/b0eab78afb186c88.png)
含义如下
addons：插件，一些比较有用的工具，如transition动画
isomorphic: 同构，服务端在页面初次加载时，将所有方法渲染好，一次性交给客户端。这样可以减少Ajax
shared: 共用方法，一些utils
test: 测试方法
renderers: React代码核心，大部分功能实现代码都在其中
dom:
stack/client: 各种ReactComponent
stack/server: 服务端渲染方法
shared: CSSProperty, DOMProperty, 合成事件处理，DOM操作方法，如findDOMNode, setInnerHTML等。
fiber: 重写了React核心算法，架构进行了升级，未来可能会应用。
native: ReactNative, 跨平台实现Android和iOS
shared:
stack/reconciler: 协调器，包含自定义组件实现ReactCompositeComponent.js， setState机制，生命周期方法流程，DOM diff等
shared/event: 事件处理
fiber：实验代码，未来可能会应用
2 重要模块
React代码还是相当复杂的，我们需要深入理解重要模块的源码机制。后面会有几篇文章针对每个模块进行分析
元素和组件的创建：ReactElement元素是一个数据类，包含props refs key等变量。ReactComponent是一个控制类，包含组件状态，操作方法等，是React对内的一个很重要的类。它有不同的子类实现，如DOM原生组件ReactDOMComponent，React自定义组件ReactCompositeComponent，文本组件ReactDOMTextComponent。
React利用createClass()创建组件类对象，createElement()创建组件实例对象。JSX经过babel转译后，实际是调用createElement()创建实例对象。这部分代码分析参见[React源码分析2 — 组件和对象的创建（createClass，createElement）](http://blog.csdn.net/u013510838/article/details/55669769)
React组件插入DOM流程：创建了ReactElement和ReactComponent后，还需要将virtual DOM插入真实DOM中，这样浏览器才能渲染。React会利用virtual DOM生成HTML，然后将HTML插入父组件中。而root组件正好是我们在ReactDOM.render()方法中传入的DOM原生对象。这部分代码分析参见[React源码分析3 — React组件插入DOM流程](http://blog.csdn.net/u013510838/article/details/56677451)
React生命周期：React吸引人的一个地方在于，有比较清晰的生命周期调用方法。利用模板模式使得代码结构清晰而又不失灵活性。前端一直以来有个让人诟病的地方在于，大家代码风格各异，没有像Android那样比较清晰的流程方法，使得维护起来比较麻烦。React的出现大大解决了这个难题。这部分代码分析参见[React源码分析4 — React生命周期详解](http://blog.csdn.net/u013510838/article/details/58070092)
setState实现机制：作为一种前端流行框架，虽然React专注于MVVM中的View，但它也实现了一套View和数据绑定的方法。这个正是setState。同时，利用队列和transaction来管理setState，避免了一些重复无谓的界面更新。这部分代码分析参见[React源码分析5 — setState机制](http://blog.csdn.net/u013510838/article/details/59486772)
总之，作为当下前端最流行的框架，React源码还是值得我们细细分析的。从源码中我们也能学到很多优秀的设计模式，让我们的代码更加清晰好维护。文章中如有不正确的地方，欢迎指正！

