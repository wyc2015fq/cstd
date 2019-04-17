# react-native 之移动端开发九问 - DEVELOPER - CSDN博客





2018年06月05日 22:47:23[学术袁](https://me.csdn.net/u012827205)阅读数：262
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









在分析这些疑惑之前，我先就用语言描述两个概念 
`react`**是一种用于构建用户界面的 JavaScript 库；**
`javaScript`**是一种基于原型编程、多范式的动态脚本语言，并且支持面向对象、命令式和声明式（如函数式编程）风格。**

使用原生方式写移动应用之后，我们换用`react native` 写，中觉得会怪怪的，怪在哪里？实现业务的方式不同了！！ 

在这里总结几条：
```
1， 如何在`react native`的组件中声明变量并进行使用？
2， var、const和let都能定义变量，有什么不同？
3， 数组这种数据结构和他的一些方法使用方式？
4， 如何进行组件之间的方法回调实现？
5， 如何获取一个组件的实例对象并进行使用？
6， RN组件：Text、TextInput等
7， Js中的一个类Promise是做什么的，await 操作符又是干啥的？
8， 如何在不同的页面进行消息通知？
9， 如何在组件与组件之间进行数据or消息的传递？
```

## `1`如何声明变量并进行使用？

在`react native` 的组件中进行声明变量 
![变量声明图片](https://img-blog.csdn.net/20180612142441169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第一种方式，**

使用`const` 声明了一个对象（也可说变量），该对象即类似于`java` 中的一个`bean类`，也类似于一种`数据结构Map`。使用方式 `'showToast'==ACTION_NOTIFY[toast]`、 `'showToast'==ACTION_NOTIFY.toast` 获取`key`对应的`value`值。即可以用在当前的js文件中，也可以被导入到其他的js文件中使用； 
**第二种方式**，
```
this.state = {
     selectedTab: 'Home',
     theme:this.props.theme,
 };
```

通过这种方式，声明了一个组件的内部状态数据变量。并通过对state种内容的改变而使得组件进行重新渲染。 

调用方式`this.state.selectedTab` 拿到`'home'`字符串； 

使用方式`this.setState({selectedTab:'Mine'});` 更改`selectedTab` 对应的值。 
**第三种方式，**

直接使用`this.listener`，声明了一个变量，属于全局作用域范围的变量。等价于在组件中的构造方法中进行的声明方式
```
constructor(props) {
    this.listener = DeviceEventEmitter.addListener(..);
}
```

她们的不同之处在于被初始化的顺序不同，构造方法先，componentDieMount方法中的初始化后。 
**第四种方式，**
![这里写图片描述](https://img-blog.csdn.net/20180612144441928?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在方法中进行声明，并在方法内进行使用，`let`的作用域是属于块儿区域范围。比如上`render`方法中使用`let`定义一个`组件变量data`，然后使用在本方法中 
![这里写图片描述](https://img-blog.csdn.net/20180612144718906?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## `2`var、const和let都能定义变量，有什么不同？

`var`的定义 `var 变量名1 [= value1 [, 变量名2 [, 变量名3... [, 变量名N]]]];`
`var`进行变量声明，无论发生在何处，都在执行任何代码之前进行处理。用var声明的变量的作用域是它当前的执行上下文，它可以是嵌套的函数，也可以是声明在任何函数外的变量。

`let`的定义`let 变量名1 [= value1] [, 变量名2[= value2]] [, ..., 变量名N [= valueN]];`
`let`允许你声明一个作用域被限制在块级中的变量、语句或者表达式。与var关键字不同的是，`var`声明的变量只能是全局或者整个函数块的。

```java
function varTest() {
  var x = 1;
  if (true) {
    var x = 2;  // 同样的变量!
    console.log(x);  // 2
  }
  console.log(x);  // 2
}
```

```java
function letTest() {
  let x = 1;
  if (true) {
    let x = 2;  // 不同的变量
    console.log(x);  // 2
  }
  console.log(x);  // 1
}
```

**而且需要注意：**在相同的函数或块作用域内重新声明同一个变量会引发`SyntaxError`

```
if (x) {
  let foo;
  let foo; // TypeError thrown.
}
```

`const`的定义`const 变量名1 = value1 [, 变量名2 = value2 [, ... [, 变量名N = valueN]]];`
`const`声明创建一个常量，其作用域可以是全局或本地声明的块。 与var变量不同，全局常量不会变为窗口对象的属性。简单的描述就是通过`const`定义过的常量，其指向内存地址的引用不能变。

```
// 定义常量MY_FAV并赋值7
const MY_FAV = 7;

// 再次赋值报错
MY_FAV = 20;

// 尝试重新声明会报错 
const MY_FAV = 20;

//  MY_FAV 保留给上面的常量，这个操作会失败
var MY_FAV = 20; 

// 也会报错
let MY_FAV = 20;
```

## `3`数组这种数据结构和他的一些方法使用方式？

`阮一峰`老师在这里有比较详细的解释，[点击这里查看详解](http://es6.ruanyifeng.com/#docs/array)

## `4`如何进行组件之间的方法回调实现？

这一块儿，我觉得在移动应用开发过程中是非常重要的。就像java类中的接口回调一样，用处那么的广泛和实用！ 

java中实现接口回调目的，就是在一个类中将具体的实现所产生的结果，或者处理过程，暴露在需要使用的别的类中。（纯属个人理解） 

而在`react native` 也是这样的。发生在两个`component组件`中的事件，**并且这种回调仅属于B组件是视图A（组件A）的一个子视图的情况。**请看 

基于下面列表的业务逻辑是这样的 
![这里写图片描述](https://img-blog.csdn.net/20180612153403732?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

一个列表页面`A.js`，里边使用的是`listview`，而每一个`item`，作为组件`B.js`，我单独的给抽了出来。现在实现的功能是在`js类B`中点击了搜藏的`星按钮`操作，在`js类A`中进行回调并处理该点击逻辑。 

接下来让我们看下怎么在两个`component组件` 中进行方法的回调！！
**`js类B`中点击事件触发的方法**
![这里写图片描述](https://img-blog.csdn.net/2018061215394774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**`js类A`中通过`js类B`中点击事件的触发所作出的反应**
![这里写图片描述](https://img-blog.csdn.net/20180612154125175?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

解释一下，在`js类A`中对`<RespositoryTrendingCell />` 自定义的组件设定了一个`组件属性`` onFavoritePressed={(item, isFavorite) => this.onFavoritePressed(item, isFavorite)}`
即给设定的属性`onFavoritePressed`配置了一个方法，作为引用的指向。且所指向的方法有两个参数`(item, isFavorite)` ，这两个参数是从`js类B`中传递过来并赋值到方法（函数）`this.onFavoritePressed(item, isFavorite)}`中。而在`js类B`中使用方法`this.props.onFavoritePressed` 则纯粹是组件自身对自己的属性调用方式。而且是正宗的方式，对于组件自身的属性都可以通过方式`this.props.属性名`进行调用。且属性名无论变量或者方法。

## `5`如何获取一个组件的实例对象并进行使用？

如果放到java语言中，获取一个类的实例，。。。`new` 嘛！使用new关键字语法就解决了~ 

在`react-native` 中在`导入使用`一个`class类`文件的时候，获取该类实例也是通过`new创建`。对于组件，react支持一个ref属性，该属性可以添加到任何的组件上。 
**例子1：**在`TouchableOpacity组件`中定义了一个`ref属性`
![这里写图片描述](https://img-blog.csdn.net/20180612161941540?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后就可以在该js类中使用 
![这里写图片描述](https://img-blog.csdn.net/20180612162056727?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

代码逻辑所实现的功能是，点击图片弹出menu对话框显示在组件Image的下方。
**例子2：** 视图A中的菜单组件被点击逻辑 
![这里写图片描述](https://img-blog.csdn.net/20180612162534682?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180612162617602?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

组件`<MoreMenu/>`中定义了属性ref，使得可以在视图A中拿到`<MoreMenu/>`实例并调用其方法，来实现需求逻辑。
**例子3：**Toast使用 

在一个`js`中 

导入·`import Toast, {DURATION} from 'react-native-easy-toast';`

在当前组件中载入toast组件，像这样
```xml
<Toast ref="toast"/>
/*<Toast ref={toast=>this.toast=toast}/>*/
```

如此就能依靠`ref属性`方便使用`Toast`。==>>`this.refs.toast.show(..)`

## `7`Js中的一个方法类Promise是做什么的，await 操作符又是干啥的？

从语法上说，`Promise` 是一个对象，由他可以`获取异步操作的消息（结果）`。Promise 是`异步编程`的一种解决方案。 
![这里写图片描述](https://img-blog.csdn.net/20180612164303926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这是一个能`返回json结果`的方法，先是从本地查找并获取，本地没有数据或者过时则从网络上重新获取json数据，如果失败、出错会重新进行网络请求，并返回请求到的json数据结果。其中便使用到了`Promise`。
![这里写图片描述](https://img-blog.csdn.net/20180612164855341?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`await`  操作符用于等待一个`Promise` 对象所做的**操作结果**。它只能在异步函数 `async function` 中使用，并返回`promise.resolve`或者`promise.reject`的结果，并不是一个promise对象。**比如**上面图片中，返回的是一个字符串的数组对象。
## `8`如何在不同的页面进行消息通知？

在不同的页面，在`react-native`中也叫做`component`之间，非父组件与子组件item的关系。是一种平等级别的关系。 

她们之间的信息传递类似于Android中页面之间通过`EventBus`进行的消息传递，通知！而在`react-native`中使用的是`react-native`中的一个组件`DeviceEventEmitter` 可以通过注册监听，发送通知来实现！ 
![这里写图片描述](https://img-blog.csdn.net/20180612170013567?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在接收消息页面进行监听的注册，添加一个监听，实时监听消息的通知！使用方式上通常是在组件生命周期``componentDidMount <已装载>中开始进行注册，而在生命周期``componentWillUnmount<已卸载>中移除监听。 

然后在需要发送通知的地方，发送通知``//发送通知，通知修改主题
DeviceEventEmitter.emit('ACTION_BASE', ACTION_NOTIFY.changeTheme, ThemeFactory.createTheme(ThemeFlags[key]))

通过发送一个通知，通知让在接收消息的页面，执行更换主题的操作逻辑。从而完成页面（组件）之间的消息通知功能。``9如何在组件与组件之间进行数据or消息的传递？

组件之间进行数据的传递，同等级别组件之间使用``DeviceEventEmitter能实现，父子级别组件之间可以通过``组件属性实现。而``props则可以应用在各种级别的组件之间，实现数据传递。 
![这里写图片描述](https://img-blog.csdn.net/20180612171725276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这种方式实现了当前``A页面组件跳转到另外一个``B页面的时候，向下级页面``传递数据的功能。传递的数据内容都包含在``params对象中。通过``...this.props数组的扩展将``A页面组件的所有属性继承给了``B页面组件。并且在``A页面组件中动态的为``B页面组件新增了属性``menuType和``theme。由此在``B页面组件中可以直接通过``this.props.menuType 和 this.props.theme来得到新增的属性内容数据。 

以上是基于等级别的组件之间数据传递，这样传递过去的属性内容，如``menuType可以在构造方法中立即获取并赋值给定义的变量。但是在父子组件之间传递数据，就不同了！？不同的地方在传递过去的props属性内容在子组件构造方法中并不能直接拿得到。props属性内容被传递也即是组件的props属性内容发生了改变，此时第一个接收到回调的方法是在子组件生命周期方法componentWillReceiveProps(nextProps)中。为什么呢？其实最大区别就是通过跳转二级页面组件是要经历装载、渲染的过程，必然会访问到props属性内容。而引用组件（父子关系）则对于子组件而言，当你由操作去改变props属性内容时其实已经经历过了装载和渲染的过程。这时子组件props的改变只会触发方法``componentWillReceiveProps。

由此看下面一些代码罗理解下 

在js欢迎页面跳转到主页面HomePage.js，并传递了一个props属性内容叫做theme 
![这里写图片描述](https://img-blog.csdn.net/20180612174351409?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在HomePage.js页面接收，并赋值给其状态属性theme 
![这里写图片描述](https://img-blog.csdn.net/20180612174646710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

并将该状态属性theme通过组件属性传递（props属性内容新增属性）方式进行传递； 

注意：这里看到``{...this.props}扩展属性，通过这种方式，组件之间(无论级别)，子组件可以继承父组件属性并可实现栈内跳转。如跳转的this.props.navigator. 
![这里写图片描述](https://img-blog.csdn.net/20180612174854747?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时将触发子组件的方法回调，然后将新的值赋值给该组件的状态变量，由于props的变化必然会引起组件的重新渲染。因此在组件重新进行渲染的时候便能够使用状态属性最新值展示给用户。 
![这里写图片描述](https://img-blog.csdn.net/20180612180542311?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)















