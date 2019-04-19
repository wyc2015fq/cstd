# 微信小程序 js逻辑 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月04日 15:57:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：860
逻辑层将数据进行处理后发送给视图层，同时接受视图层的事件反馈。
在js的基础上：
- 增加 getApp 和 getCurrentPages 方法，分别用来获取
 App 实例和当前页面栈。
- 由于框架并非运行在浏览器中，所以 JavaScript 在 web 中一些能力都无法使用，如 document，window 等
App() 函数用来注册一个小程序。接受一个 object 参数，其指定小程序的生命周期函数等。
|onLaunch|Function|生命周期函数--监听小程序初始化|当小程序初始化完成时，会触发 onLaunch（全局只触发一次）|
|----|----|----|----|
|onShow|Function|生命周期函数--监听小程序显示|当小程序启动，或从后台进入前台显示，会触发 onShow|
|onHide|Function|生命周期函数--监听小程序隐藏|当小程序从前台进入后台，会触发 onHide|
|onError|Function|错误监听函数|当小程序发生脚本错误，或者 api 调用失败时，会触发 onError 并带上错误信息|
|其他|Any||开发者可以添加任意的函数或数据到 Object 参数中，用 this 可以访问|
当用户点击左上角关闭，或者按了设备 Home 键离开微信，小程序并没有直接销毁，而是进入了后台；当再次进入微信或再次打开小程序，又会从后台进入前台。需要注意的是：只有当小程序进入后台一定时间，或者系统资源占用过高，才会被真正的销毁。
全局的 getApp() 函数可以用来获取到小程序实例。
// other.jsvar
 appInstance = getApp()console.log(appInstance.globalData)
// I am global data
- 不要在定义于 App() 内的函数中调用 getApp() ，使用 this 就可以拿到
 app 实例。
- 不要在 onLaunch 的时候调用 getCurrentPages()，此时 page 还没有生成。
- 
如果需要全局的数据可以在 App() 中设置，如：App({
 globalData: 1})
页面
Page() 函数用来注册一个页面。接受一个 object 参数，其指定页面的初始数据、生命周期函数、事件处理函数等。
|[data](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/app-service/page.html#%E5%88%9D%E5%A7%8B%E5%8C%96%E6%95%B0%E6%8D%AE)|页面的初始数据,data 将会以 JSON 的形式由逻辑层传至渲染层，所以其数据必须是可以转成 JSON 的格式：字符串，数字，布尔值，对象，数组。|
|----|----|
|onLoad|生命周期函数--监听页面加载，只会调用一次,可以在 onLoad 中获取打开当前页面所调用的 query 参数|
|onReady|生命周期函数--监听页面初次渲染完成|
|onShow|生命周期函数--监听页面显示|
|onHide|生命周期函数--监听页面隐藏|
|onUnload|生命周期函数--监听页面卸载|
|onPullDownRefresh|页面相关事件处理函数--监听用户下拉动作,下拉刷新|
|onReachBottom|页面上拉触底事件的处理函数|
|onShareAppMessage|用户点击右上角转发，点击转发按钮的时候会调用此事件需要 return 一个 Object，用于自定义转发内容|
|onPageScroll|页面滚动触发事件的处理函数|
|其他|开发者可以添加任意的函数或数据到 object 参数（data）中，在页面的函数中用 this 可以访问|
对界面的设置如wx.setNavigationBarTitle请在onReady之后设置
如
...
onPageScroll:
function()
{
// Do something when page scroll
 }, // Event handler. viewTap:
function()
{
this.setData({ text:
'Set some data for updating view.'
 }, function()
{
// this is setData callback
 }) }, customData: { hi: 'MINA'
 }
onPullDownRefresh：需要在app.json的[window](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/config.html#window)选项中或[页面配置](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/config.html#pagejson)中开启enablePullDownRefresh。
onReachBottom
可以在app.json的[window](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/config.html#window)选项中或[页面配置](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/config.html#pagejson)中设置触发距离onReachBottomDistance。在触发距离内滑动期间，本事件只会被触发一次。
除了初始化数据和生命周期函数，Page 中还可以定义一些特殊的函数：事件处理函数。
<view
bindtap="viewTap">
 click me </view>
Page({ viewTap:
function()
{
console.log('view
 tap') }})

setData 函数用于将数据从逻辑层发送到视图层（异步），同时改变对应的 this.data 的值（同步）。
- **直接修改 this.data 而不调用 this.setData 是无法改变页面的状态的，还会造成数据不一致**。
模块化
可以将一些公共的代码抽离成为一个单独的 js 文件，作为一个模块。模块只有通过 module.exports 或者 exports 才能对外暴露接口。
推荐开发者采用 module.exports 来暴露模块接口
// common.jsfunctionsayHello(name)
{
console.log(`Hello
 ${name} !`)}functionsayGoodbye(name)
{
console.log(`Goodbye
 ${name} !`)}module.exports.sayHello
 = sayHelloexports.sayGoodbye = sayGoodbye

使用
var common = 
require('common.js')Page({ helloMINA: function() { common.sayHello('MINA') }, goodbyeMINA: function() { common.sayGoodbye('MINA') }})
 require 暂时不支持绝对路径

