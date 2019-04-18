# React和Redux的连接react-redux - z69183787的专栏 - CSDN博客
2016年08月01日 16:03:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：602
[http://leozdgao.me/reacthe-reduxde-qiao-jie-react-redux/](http://leozdgao.me/reacthe-reduxde-qiao-jie-react-redux/)
之前一直在探索React相关的东西，手上有个SPA项目，于是准备上Redux试试水。Redux本身和React并没有之间的关联，它是一个通用Javscript App模块，用做App State的管理。要在React的项目中使用Redux，比较好的方式是借助react-redux这个库来做连接，这里的意思是，并不是没有react-redux，这两个库就不弄一起用了，而是说react-redux提供了一些封装，一种更科学的代码组织方式，让我们更舒服地在React的代码中使用Redux。
之前仅通过Redux文档来了解react-redux，在一段时间的实践后准备翻一翻源代码，顺便做些相关的总结。我看的代码的npm版本为`v4.0.0`，也就是说使用的React版本是0.14.x。
react-redux提供两个关键模块：Provider和connect。
## Provider
Provider这个模块是作为整个App的容器，在你原有的App Container的基础上再包上一层，它的工作很简单，就是接受Redux的store作为props，并将其声明为context的属性之一，子组件可以在声明了`contextTypes`之后可以方便的通过`this.context.store`访问到store。不过我们的组件通常不需要这么做，将store放在context里，是为了给下面的connect用的。
这个是Provider的使用示例：
```xml
// config app root
const history = createHistory()
const root = (
  <Provider store={store} key="provider">
    <Router history={history} routes={routes} />
  </Provider>
)
// render
ReactDOM.render(
  root,
  document.getElementById('root')
)
```
## connect
这个模块是算是真正意义上连接了Redux和React，正好它的名字也叫connect。
先考虑Redux是怎么运作的：首先store中维护了一个state，我们dispatch一个action，接下来reducer根据这个action更新state。
映射到我们的React应用中，store中维护的state就是我们的app state，一个React组件作为View层，做两件事：render和响应用户操作。于是connect就是将store中的必要数据作为props传递给React组件来render，并包装action creator用于在响应用户操作时dispatch一个action。
好了，详细看看connect这个模块做了什么。先从它的使用来说，它的API如下：
```java
```java
connect([mapStateToProps], [mapDispatchToProps], [mergeProps], [options])
```
```
mapStateToProps是一个函数，返回值表示的是需要merge进props的state。默认值为`() => ({})`，即什么都不传。
```java
```java
(state, props) => ({  }) // 通常会省略第二个参数
```
```
mapDispatchToProps是可以是一个函数，返回值表示的是需要merge仅props的actionCreators，这里的actionCreator应该是已经被包装了dispatch了的，推荐使用redux的bindActionCreators函数。
```java
```java
(dispatch, props) => ({ // 通常会省略第二个参数
 ...bindActionCreators({
   ...ResourceActions
 }, dispatch)
})
```
```
更方便的是可以直接接受一个对象，此时connect函数内部会将其转变为函数，这个函数和上面那个例子是一模一样的。
mergeProps用于自定义merge流程，下面这个是默认流程，parentProps值的就是组件自身的props，可以发现如果组件的props上出现同名，会被覆盖。
```java
```java
(stateProps, dispatchProps, parentProps) => ({
  ...parentProps,
  ...stateProps,
  ...dispatchProps
})
```
```
options共有两个开关：pure代表是否打开优化，详细内容下面会提，默认为true，withRef用来给包装在里面的组件一个ref，可以通过getWrappedInstance方法来获取这个ref，默认为false。
connect返回一个函数，它接受一个React组件的构造函数作为连接对象，最终返回连接好的组件构造函数。
然后几个问题：
- React组件如何响应store的变化？
- 为什么connect选择性的merge一些props，而不是直接将整个state传入？
- pure优化的是什么？
我们把connect返回的函数叫做Connector，它返回的是内部的一个叫Connect的组件，它在包装原有组件的基础上，还在内部监听了Redux的store的变化，为了让被它包装的组件可以响应store的变化:
```java
```java
trySubscribe() {
  if (shouldSubscribe && !this.unsubscribe) {
    this.unsubscribe = this.store.subscribe(::this.handleChange)
    this.handleChange()
  }
}
handleChange () {
  this.setState({
    storeState: this.store.getState()
  })
}
```
```
但是通常，我们connect的是某个Container组件，它并不承载所有App state，然而我们的handler是响应所有state变化的，于是我们需要优化的是：当storeState变化的时候，仅在我们真正依赖那部分state变化时，才重新render相应的React组件，那么什么是我们真正依赖的部分？就是通过`mapStateToProps`和`mapDispatchToProps`得到的。
具体优化的方式就是在`shouldComponentUpdate`中做检查，如果只有在组件自身的props改变，或者`mapStateToProps`的结果改变，或者是`mapDispatchToProps`的结果改变时`shouldComponentUpdate`才会返回true，检查的方式是进行shallowEqual的比较。
所以对于某个reducer来说：
```java
```java
export default (state = {}, action) => {
  return { ...state } // 返回的是一个新的对象，可能会使组件reRender
  // return state // 可能不会使得组件reRender
}
```
```
另外在connect的时候，要谨慎map真正需要的state或者actionCreators到props中，以避免不必要的性能损失。
最后，根据connect的API我们发现可以使用ES7 decorator功能来配合React ES6的写法：
```java
```java
@connect(
  state => ({
    user: state.user,
    resource: state.resource
  }),
  dispatch => ({
    ...bindActionCreators({
      loadResource: ResourceActions.load
    }, dispatch)
  })
)
export default class Main extends Component {
}
```
```
OK，结束了。
[推荐拓展阅读](/sign_in)
著作权归作者所有
如果觉得我的文章对您有用，请随意打赏。您的支持将鼓励我继续创作！
[¥ 打赏支持](#pay-modal)
[喜欢](/sign_in)
21
分享到微博
[分享到微信](#share-weixin-modal)
更多分享- [下载长微博图片](http://cwb.assets.jianshu.io/notes/images/2373218/weibo/image_0343087f4d4b.jpg)
- ![Tweibo](http://baijii-common.b0.upaiyun.com/social_icons/32x32/tweibo.png) 分享到腾讯微博
- ![Qzone](http://baijii-common.b0.upaiyun.com/social_icons/32x32/qzone.png) 分享到QQ空间
- ![Twitter](http://baijii-common.b0.upaiyun.com/social_icons/32x32/twitter.png) 分享到Twitter
- ![Facebook](http://baijii-common.b0.upaiyun.com/social_icons/32x32/facebook.png) 分享到Facebook
- ![Google plus](http://baijii-common.b0.upaiyun.com/social_icons/32x32/google_plus.png) 分享到Google+
- ![Douban](http://baijii-common.b0.upaiyun.com/social_icons/32x32/douban.png) 分享到豆瓣
×
##### 打开微信“扫一扫”，打开网页后点击屏幕右上角分享按钮
![Tiny](http://baijii-common.b0.upaiyun.com/loaders/tiny.gif)
![Tiny](http://baijii-common.b0.upaiyun.com/loaders/tiny.gif)
×
### 喜欢的用户
- ![](http://upload.jianshu.io/users/upload_avatars/2581025/de8773aa225d?imageMogr/thumbnail/90x90/quality/100)[突然累了](/users/ffeaec42ac4a)
2016.07.26 15:00
- ![](http://upload.jianshu.io/users/upload_avatars/1726248/9d9712f03f02.jpg?imageMogr/thumbnail/90x90/quality/100)[篮球不止是个球](/users/a05533f0e293)
2016.07.22 16:28
- ![](http://upload.jianshu.io/users/upload_avatars/19275/17a1cd389ba8.jpg?imageMogr/thumbnail/90x90/quality/100)[溪离欣洛](/users/xxkqi4)
2016.07.22 00:35
- ![](http://cdn-qn0.jianshu.io/assets/default_avatar/6-c3a19bc117a55e66cd3be55684d8191c.jpg)[Hustlion](/users/196669949439)
2016.07.18 08:20
- ![](http://cdn-qn0.jianshu.io/assets/default_avatar/2-65a6e6d032559f3c53475eb61d7ea573.jpg)[psx900910](/users/31dc9fae773c)2016.06.30 09:32
- ![](http://upload.jianshu.io/users/upload_avatars/1620367/2e560cb6e8e0?imageMogr/thumbnail/90x90/quality/100)[金生kingcent](/users/9d94dcbe454a)2016.06.22 19:47
- ![](http://upload.jianshu.io/users/upload_avatars/2165028/44159ace2220.jpg?imageMogr/thumbnail/90x90/quality/100)[Fanhehe](/users/a23b6bdaaf51)
2016.05.30 13:40
- ![](http://cdn-qn0.jianshu.io/assets/default_avatar/8-77a9d346e43ea9a5ea58086d74909362.jpg)[WadeHuang](/users/e5a9d912e5c5)2016.05.24 18:02
- ![](http://upload.jianshu.io/users/upload_avatars/671285/dad05f802ae6.jpg?imageMogr/thumbnail/90x90/quality/100)[MoonBall](/users/7cdfa98987d1)
2016.05.10 14:04
- ![](http://upload.jianshu.io/users/upload_avatars/1347666/c3e2021a70ea?imageMogr/thumbnail/90x90/quality/100)[田爽](/users/7a493c3e81f9)
2016.04.17 09:37
- ![](http://cdn-qn0.jianshu.io/assets/default_avatar/7-a82e998966cef5ab3eeda59b4297b619.jpg)[lyzaijs](/users/cedc44bed4ad)
2016.04.15 11:25
- ![](http://upload.jianshu.io/users/upload_avatars/1848511/e5c4cd211158.jpg?imageMogr/thumbnail/90x90/quality/100)[林呵呵不见了](/users/e17e4fbafd87)
2016.04.03 13:37
- ![](http://upload.jianshu.io/users/upload_avatars/3110/fad7d01ff5cc?imageMogr/thumbnail/90x90/quality/100)[雁门员外](/users/wb6cTU)
2016.03.04 18:13
- ![](http://upload.jianshu.io/users/upload_avatars/1098960/95c452ebcb64?imageMogr/thumbnail/90x90/quality/100)[瑶柱活到老供楼到老](/users/433bfbeaea09)2016.02.29 11:46
- ![](http://upload.jianshu.io/users/upload_avatars/1237865/5e3e1c81356d?imageMogr/thumbnail/90x90/quality/100)[特大号废柴](/users/9aab2811130e)
2016.02.26 11:31
3条评论 （ 
按时间正序· 按时间倒序· 按喜欢排序 ） [添加新评论](/sign_in)
![100](http://upload.jianshu.io/users/upload_avatars/3110/fad7d01ff5cc?imageMogr/thumbnail/90x90/quality/100)
[雁门员外](/users/wb6cTU)
2 楼 · [2016.03.04 18:09](/p/94c988cf11f3/comments/1615512#comment-1615512)
看了你的文章很受启发，冒昧请教一个问题：
containers\app\App.js(61): return {actions: bindActionCreators({fetchProfile, logout, getCurrentDevice, clearDeviceInformation, getGeoLocation}, dispatch)};
containers\explore\Explore.js(81): return {actions: bindActionCreators({getGeoLocation, getNearPlaces, getNearCheckIn, clearGetNearPlacesErrors, fetchAllUsers, followUser, unfollowUser}, dispatch)};
containers\home\Home.js(131): return {actions: bindActionCreators({getGeoLocation, checkIn, createPlace}, dispatch)};
containers\login\Login.js(77): return {actions: bindActionCreators({login}, dispatch)};
containers\person\Person.js(75): return {actions: bindActionCreators({fetchAllUsers}, dispatch)};
containers\profile\Profile.js(178): return {actions: bindActionCreators({editProfile, getAllDevicesIfNeeded, deleteDevice, clearSaveSuccessMessage}, dispatch)};
containers\record\Record.js(150): return {actions: bindActionCreators({getCheckIn, deleteCheckIn, editCheckIn, cancelEditCheckIn, updateCheckIn}, dispatch)};
containers\register\Register.js(114): return {actions: bindActionCreators({register}, dispatch)};
上面是一个工程中的多个container的bindActionCreators中的具体实例。
我的疑问是，多个不同的container绑定了相同的action那么因action的动作导致了store的修改后，redux如果知道该render那个container呢？
或者说你的理解：
文／leozdgao（简书作者）
原文链接：[http://www.jianshu.com/p/94c988cf11f3](http://www.jianshu.com/p/94c988cf11f3)
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
当storeState变化的时候，仅在我们真正依赖那部分state变化时，才重新render相应的React组件，那么什么是我们真正依赖的部分？就是通过mapStateToProps和mapDispatchToProps得到的。
具体优化的方式就是在shouldComponentUpdate中做检查，如果只有在组件自身的props改变，或者mapStateToProps的结果改变，或者是mapDispatchToProps的结果改变时shouldComponentUpdate才会返回true，检查的方式是进行shallowEqual的比较。
文／leozdgao（简书作者）
原文链接：http://www.jianshu.com/p/94c988cf11f3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
