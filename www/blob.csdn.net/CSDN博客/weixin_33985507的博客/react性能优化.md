# react性能优化 - weixin_33985507的博客 - CSDN博客
2019年01月08日 17:40:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 可复性高的组件
如果一个组件的渲染**只依赖于外界传进去的 `props`和自己的 `state`，而并不依赖于其他的外界的任何数据，**也就是说像纯函数一样，给它什么，它就吐出（渲染）什么出来。这种组件的复用性是最强的，别人使用的时候根本不用担心任何事情，只要看看 PropTypes 它能接受什么参数，然后把参数传进去控制它就行了。我们把这种组件叫做 `Pure Component`，因为它就像纯函数一样，可预测性非常强，对参数（props）以外的数据零依赖，也不产生副作用。这种组件也叫 `Dumb Component`，因为它们呆呆的，让它干啥就干啥。写组件的时候尽量写`Dumb Component`会提高我们的组件的可复用性。
## 避免重复渲染
【shouldComponentUpdate】
　　当一个组件的`props`或者`state`改变时，React通过比较新返回的元素和之前渲染的元素来决定是否有必要更新实际的DOM。当他们不相等时，React会更新DOM。
在一些情况下，组件可以通过重写这个生命周期函数`shouldComponentUpdate`来提升速度， 它是在重新渲染过程开始前触发的。 这个函数默认返回`true`，可使React执行更新：
```
shouldComponentUpdate(nextProps, nextState) {
  return true;
}
```
如果知道在某些情况下组件不需要更新，可以在`shouldComponentUpdate`内返回`false`来跳过整个渲染进程，该进程包括了对该组件和之后的内容调用`render()`指令。
如果想让组件只在`props.color`或者`state.count`的值变化时重新渲染，可以像下面这样设定`shouldComponentUpdate`。
```
shouldComponentUpdate(nextProps, nextState) {
    if (this.props.color !== nextProps.color) {
      return true;
    }
    if (this.state.count !== nextState.count) {
      return true;
    }
    return false;
  }
```
【pureComponent】
　　在以上代码中，`shouldComponentUpdate`只检查`props.color`和`state.count`的变化。如果这些值没有变化，组件就不会更新。**当组件变得更加复杂时，可以使用类似的模式来做一个“浅比较”，用来比较属性和值以判定是否需要更新组件**。这种模式十分常见，因此React提供了一个辅助对象来实现这个逻辑 - 继承自`React.PureComponent`。
```
class CounterButton extends React.PureComponent {
  constructor(props) {
    super(props);
    this.state = {count: 1};
  }
  render() {
    return (
      <button
        color={this.props.color}
        onClick={() => this.setState(state => ({count: state.count + 1}))}>
        Count: {this.state.count}
      </button>
    );
  }
}
```
大部分情况下，可以使用`React.PureComponent`而不必写自己的`shouldComponentUpdate`，它只做一个浅比较。但是由于浅比较会忽略属性或状态突变的情况，使用时需要特别注意。
## 避免突变
`PureComponent`将会在`this.props.words`的新旧值之间做一个简单的比较。由于代码中`words`数组在`WordAdder`的`handleClick`方法中被改变了，尽管数组中的实际单词已经改变，`this.props.words`的新旧值还是相等的，因此即便`ListOfWords`具有应该被渲染的新单词，它还是不会更新。
```
handleClick() {
    // This section is bad style and causes a bug
    const words = this.state.words;
    words.push('marklar');
    this.setState({words: words});
  }
```
避免此类问题最简单的方式是避免使用值可能会突变的属性或状态。例如，上面例子中的`handleClick`应该用`concat`重写成：
```
handleClick() {
  this.setState(prevState => ({
    words: prevState.words.concat(['marklar'])
  }));
}
```
数组方法是否突变，可以查看[JS总结的数组部分](https://www.jianshu.com/p/9e11e82c5d12)
也可以使用展开运算符，原理与concat一样，都是生成一个新数组，避免浅拷贝:
```
handleClick() {
  this.setState(prevState => ({
    words: [...prevState.words, 'marklar'],
  }));
};
```
也可以用相似的方式重写可以会突变的对象。
例如，假设有一个叫`colormap的对象`，我们想写一个把`colormap.right`改变成'`blue'的函数`，我们应该写：
```
function updateColorMap(colormap) {
  colormap.right = 'blue';
}
```
想要实现代码而不污染原始对象，我们可以使用Object.assign方法：
```
function updateColorMap(colormap) {
  return Object.assign({}, colormap, {right: 'blue'});
}
```
或者使用扩展运算符：
```
function updateColorMap(colormap) {
  return {...colormap, right: 'blue'};
}
```
【 immutable 】
JavaScript 中的对象一般是可变的（Mutable），当数据复杂后，这有很大的隐患，一般的做法是使用 shallowCopy（浅拷贝）或 deepCopy（深拷贝）来避免被修改，但这样做造成了CPU 和内存的浪费，`Immutable.js`可以很好地解决这些问题。
`Immutable Data` 是一旦创建就不能再被更改的数据。对 `Immutable`对象的任何修改或添加删除操作都会返回一个新的`Immutable 对象`。`Immutable` 实现的原理是 `Persistent Data Structure（持久化数据结构）`，也就是使用旧数据创建新数据时，要保证旧数据同时可用且不变。同时为了避免 deepCopy 把所有节点都复制一遍带来的性能损耗，**`Immutable`使用了 `Structural Sharing（结构共享）`，即如果对象树中一个节点发生变化，只修改这个节点和受它影响的父节点，其它节点则进行共享。**
```
const x = { foo: 'bar' };
const y = x;
y.foo = 'baz';
x === y; // true
```
虽然y被编辑了，但是由于它与x索引了相同的对象，这个比较会返回`true`。可以使用`immutable.js`实现类似效果：
```
const SomeRecord = Immutable.Record({ foo: null });
const x = new SomeRecord({ foo: 'bar' });
const y = x.set('foo', 'baz');
x === y; // false
```
在这个例子中，x突变后返回了一个新的索引，因此我们可以安全的确认x被改变了。
【Immutable 优点】
　　1.`Immutable`降低了`Mutable`带来的复杂度。
可变（Mutable）数据耦合了 Time 和 Value 的概念，造成了数据很难被回溯。
比如下面一段代码：
```
function touchAndLog(touchFn) {
  let data = { key: 'value' };
  touchFn(data);
  console.log(data.key); // 猜猜会打印什么？
}
```
在不查看`touchFn`的代码的情况下，因为不确定它对 data 做了什么，你是不可能知道会打印什么，但如果 data 是`Immutable`的呢，你可以很肯定的知道打印的是 `value`。
　　2. 节省内存
`Immutable.js` 使用了 `Structure Sharing`会尽量复用内存，甚至以前使用的对象也可以再次被复用，没有被引用的对象会被垃圾回收。
```
import { Map} from 'immutable';
let a = Map({
  select: 'users',
  filter: Map({ name: 'Cam' })
})
let b = a.set('select', 'people');
a === b; // false
a.get('filter') === b.get('filter'); // true
```
上面 a 和 b 共享了没有变化的 filter 节点。
【拥抱函数式编程】
`Immutable` 本身就是函数式编程中的概念，纯函数式编程比面向对象更适用于前端开发。**因为只要输入一致，输出必然一致，这样开发的组件更易于调试和组装。**
![15401334-f3028266c6522ada.png](https://upload-images.jianshu.io/upload_images/15401334-f3028266c6522ada.png)
【与 Redux 搭配使用】
　　由于 Redux 中内置的 `combineReducers`和 `reducer`中的`initialState` 都为原生的`Object 对象`，所以不能和 Immutable 原生搭配使用。
[Immutable中的API](https://segmentfault.com/a/1190000010676878)
[Immutable 详解及 React 中实践](https://github.com/camsong/blog/issues/3)
