# react-native 之导入(import)、导出(export)深刻解析 - DEVELOPER - CSDN博客





2018年08月21日 12:24:02[学术袁](https://me.csdn.net/u012827205)阅读数：1543
所属专栏：[react-native](https://blog.csdn.net/column/details/23827.html)









最近每天业余时间，抽空读了2小时的书，把`JavaScript 忍者秘籍（第二版）`通篇过了一边。想想为了**react-native**我还是蛮拼的。之前刚学习**react-native**的时候，晓得是以**JavaScript**作为语言基础的。然后边学习**react-native**边学习**JavaScript**，然后边记录学习心得。初学状态虽有一股浓重的好奇心、求知劲头和不知疲倦，但是了解到的知识毕竟有限，知识结构也不会那么完整。只有慢慢的积累，知识才能越加丰富。因此，这里就为之前不完整的JavaScript在ES6上的记录使用，做下补充，深刻解析下在我使用过程中遇到得疑惑。

# 导入和导出功能

## 先看俩个例子,使用两种形式的导入导出

### 例[1]

```php
// Counter.js
export function increment() {
  return {
    type: 'increment'
  };
}

export function decrement() {
  return {
    type: 'decrement'
  };
}

export function incrementIfOdd() {
  return (dispatch, getState) => {
    const { counter } = getState();

    if (counter.get('counter') % 2 === 0) {
      return;
    }

    dispatch(increment());
  };
}
```

`Counter.js` 使用方式 
**第一步，导入：**
`import {increment, decrement, incrementIfOdd} as actions from '../../src/actions/Counter.js;`
*当然也可以这样写：*`import * as actions from '../../src/actions/Counter.js;`
**第二步，调用：**`const incre = actions.increment();`
## 例[2]

![这里写图片描述](https://img-blog.csdn.net/20180821112811845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**第一步，导入：**
`import NewJohson from '../../src/actions/CompareJohson.js;`
`import {compareJohsons } from '../../src/actions/CompareJohson.js;`
*当然也可以这样写：*`import NewJohson，{compareJohsons } from '../../src/actions/CompareJohson.js;`
`{compareJohsons }`而这种`{...}`导入方式，一般存在情况是在**非默认**导出情况下的，属性和方法命名方式。以起别名来达到以一种**映射数组**的功能再调用；而**默认的导出模式**的导出一般在**class**的命名方式上。
**第二补，调用：**
`const johson= new compareJohsons("johson");`
`compareJohsons ();`
**以上，是JavaScript ES6版本的代码模块化**

#### *在这里我只想要去解释一个我疑惑过的问题——**`默认导出`与`普通导出`的区别？？**

**默认导出** 类似`export default class Johson{}`，象CompareJohson.js中的第2行代码； 
**普通导出**类似 `export function compare(val1, val2)`，象CompareJohson.js中的第8行代码； 
**总结：**两者字面上的区别就是`default` ，但是在使用上是很不相同的。怎么解释呢？从上面代码对比着看，我们可以为默认导出的做自定义导入的命名，即并不一定使用导出时的命名。 

从上面代码对比截图体会下，默认导出方式的不同； 
![这里写图片描述](https://img-blog.csdn.net/20180821133732104?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**并附：**有时候使用上，导出时使用`default`默认导出方式，可以不声明属性名，而在导入时进行声明。比如

```
//ScreenStyle.js
export default StyleSheet.create({
    image_header_left: {
        height: 24,
        width: 24,
        marginLeft: 8,
    },
    image_header_right: {
        height: 24,
        width: 24,
        marginRight: 8,
    },
    ...
    image_report_button: {
        height: 56,
        width: 56,
        marginRight: 16
    }

});
```

使用方式上，依旧是先导入，再调用： 

导入：`import Styles from "./styles/ScreenStyle";`  这里可以看出，**默认模式下**可以在导出时**不声明**属性名，而在导入时进行**自定义**声明。 
**使用上**![这里写图片描述](https://img-blog.csdn.net/20180821122148379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### *另一个疑惑点——**导出、导入重命名（起别名）**？？

`起别名`基本上回出现在两个地方，**导出时起别名**和**导入时起别名**。 
**导出时起别名**![这里写图片描述](https://img-blog.csdn.net/201808211320212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**导入时起别名**![这里写图片描述](https://img-blog.csdn.net/20180821131326355?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

起别名目的是1，以我们熟悉、易记的方式内容命名；2，化繁为简，将要导入的内容以**映射数组**的方式进行调用； 

在上面我简单写的两种方式上，在关键字export表达式方式进行重命名（起别名）以及关键字import表达式方式进行重命名（起别名），最终的结果是在调用时，原始名称不能使用，别名则起作用。 

完结！









