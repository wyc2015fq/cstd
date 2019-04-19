# 小程序组件与api - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月08日 09:13:16[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：395
通过组合基础组件进行快速开发。
- 组件是视图层的基本组成单元。
所有组件都有的属性：
|属性名|描述|注解|
|----|----|----|
|id|组件的唯一标示|保持整个页面唯一|
|class|组件的样式类|在对应的 WXSS 中定义的样式类|
|style|组件的内联样式|可以动态设置的内联样式|
|hidden|组件是否显示|所有组件默认显示|
|data-*|自定义属性|组件上触发的事件时，会发送给事件处理函数|
|bind* / catch*|组件的事件|[详见事件](https://mp.weixin.qq.com/debug/wxadoc/dev/framework/view/wxml/event.html)|
组件分为以下七大类：
**1，视图容器(View Container)：**
|组件名|说明|
|----|----|
|[view](https://mp.weixin.qq.com/debug/wxadoc/dev/component/view.html)|视图容器|
|[scroll-view](https://mp.weixin.qq.com/debug/wxadoc/dev/component/scroll-view.html)|可滚动视图容器|
|[swiper](https://mp.weixin.qq.com/debug/wxadoc/dev/component/swiper.html)|滑块视图容器|
**swiper可以做轮播图**
movable-view：可移动的视图容器，在页面中可以拖拽滑动
cover-view：覆盖在原生组件之上的视图
**2，基础内容(Basic Content)：**
|组件名|说明|
|----|----|
|[icon](https://mp.weixin.qq.com/debug/wxadoc/dev/component/icon.html)|图标|
|[text](https://mp.weixin.qq.com/debug/wxadoc/dev/component/text.html)|文字|
|[progress](https://mp.weixin.qq.com/debug/wxadoc/dev/component/progress.html)|进度条|
rich-text：富文本
**3，表单(Form)：**
|标签名|说明|
|----|----|
|[button](https://mp.weixin.qq.com/debug/wxadoc/dev/component/button.html)|按钮|
|[form](https://mp.weixin.qq.com/debug/wxadoc/dev/component/form.html)|表单|
|[input](https://mp.weixin.qq.com/debug/wxadoc/dev/component/input.html)|输入框|
|[checkbox](https://mp.weixin.qq.com/debug/wxadoc/dev/component/checkbox.html)|多项选择器|
|[radio](https://mp.weixin.qq.com/debug/wxadoc/dev/component/radio.html)|单项选择器|
|[picker](https://mp.weixin.qq.com/debug/wxadoc/dev/component/picker.html)|列表选择器，如时间选择器，日期选择器|
|[picker-view](https://mp.weixin.qq.com/debug/wxadoc/dev/component/picker-view.html)|内嵌列表选择器|
|[slider](https://mp.weixin.qq.com/debug/wxadoc/dev/component/slider.html)|滚动选择器|
|[switch](https://mp.weixin.qq.com/debug/wxadoc/dev/component/switch.html)|开关选择器|
|[label](https://mp.weixin.qq.com/debug/wxadoc/dev/component/label.html)|标签|
textarea：多行输入框。
**4，导航(Navigation)：**
|组件名|说明|
|----|----|
|[navigator](https://mp.weixin.qq.com/debug/wxadoc/dev/component/navigator.html)|应用链接，页面链接，如跳转到新页面|
**5，多媒体(Media)：**
|组件名|说明|
|----|----|
|[audio](https://mp.weixin.qq.com/debug/wxadoc/dev/component/audio.html)|音频|
|[image](https://mp.weixin.qq.com/debug/wxadoc/dev/component/image.html)|图片|
|[video](https://mp.weixin.qq.com/debug/wxadoc/dev/component/video.html)|视频，带弹幕功能|
camera：系统相机。
live-player：实时音视频播放。
live-pusher：实时音视频录制
**6地图(Map)：**
|组件名|说明|
|----|----|
|[map](https://mp.weixin.qq.com/debug/wxadoc/dev/component/map.html)|地图|
**7画布(Canvas)：**
|组件名|说明|
|----|----|
|[canvas](https://mp.weixin.qq.com/debug/wxadoc/dev/component/canvas.html)|画布|
**客服会话：**
|组件名|说明|
|----|----|
|[contact-button](https://mp.weixin.qq.com/debug/wxadoc/dev/component/contact-button.html)|进入客服会话按钮|
web-view：网页容器，网页中可使用[JSSDK
 1.3.0](https://res.wx.qq.com/open/js/jweixin-1.3.0.js)提供的接口
wx.on 开头的 API 是监听某个事件发生的API接口，接受一个 CALLBACK 函数作为参数。当该事件触发时，会调用 CALLBACK 函数。
如未特殊约定，其他 API 接口都接受一个OBJECT作为参数。
OBJECT中可以指定success, fail, complete（都是函数）来接收接口调用结果。
**网络 API **
**媒体 API **
**文件 API **
**本地数据 API **
**位置 API**
**设备 API **
**界面 API **
**WXML节点信息 API **
**开放接口：登录，发起微信支付等**
