# React Native自带组件介绍 - 三少GG - CSDN博客
2016年07月29日 01:28:41[三少GG](https://me.csdn.net/scut1135)阅读数：1239
# ListView 组件介绍
[http://taobaofed.org/blog/2015/12/09/using-js-to-build-an-crossplatform-native-app-an-introduction-to-listview/](http://taobaofed.org/blog/2015/12/09/using-js-to-build-an-crossplatform-native-app-an-introduction-to-listview/)
## 分页
除了简单的渲染之外，另外一个要考虑的问题就是 当数据量很大的时候如何分页加载 。这种情形分两种情况考虑：
- 
数据一次性拿到，边滚动边加载
- 
数据不是一次性拿到，而是有可能分屏取数据
对于第一种情况，在 ListView 内部其实已经做了分页的处理：
- 
ListView 内部通过 `curRenderedRowsCount` 状态保存已渲染的行数；
- 
初始状态下，要加载的数据条数等于 `initialListSize`（默认为
 10 条）；
- 
在滚动时检测当前滚动的位置和最底部的距离，如果小于 `scrollRenderAheadDistance`(默认为
 1000)，就更新`curRenderedRowsCount` ，在它原有值基础上加 `pageSize` 个（默认为
 1 条）；
- 
由于属性变化，触发了 ListView 重新的 render 。在渲染过程中，`curRenderedRowsCount` 起到截断数据的作用，React
 的 diff 算法使得只有新加入的数据才会渲染到了界面上。
整个过程类似于 Web 端懒加载机制，即 每次在和底部的距离达到一个阈值时，加载接下来的 pageSize 个数据 。
对于第二种情况，ListView 提供了相关的属性：
- `onEndReachedThreshold`，在滚动即将到达底部时触发；
- `onEndReached`，在已经到达底部时触发；
我们可以在这两个方法中调用接口去拿数据，取到数据后再更新数据源。

