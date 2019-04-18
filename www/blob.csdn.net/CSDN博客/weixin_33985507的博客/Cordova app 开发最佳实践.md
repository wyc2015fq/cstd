# Cordova app 开发最佳实践 - weixin_33985507的博客 - CSDN博客
2017年06月17日 14:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 1）SPA是你的朋友
首先，Cordova 应用程序应该采用 **SPA**(单页面应用) 设计。 松散定义，*SPA*是从对网页发起请求开始运行的客户端应用。用户先加载一组最初资源（HTML，CSS 和 JavaScript脚本），进一步的更新（显示新视图，加载数据）通过 AJAX 来完成。SPAs 通常用在比较复杂的客户端应用上。GMail 是一个很好的例子，在加载完 GMail 之后，邮件的视图、编辑、组织都通过更新 DOM 来完成，而不是离开当前页面去加载一个全新的页面。
使用 SPA 可以帮你更高效地组织你的应用，对 Cordova 应用来说也有特定的好处。Cordova 应用必须等到 `deviceready` 事件触发之后才能使用插件。如果不使用 SPA，当用户从一个页面点击跳转到另一个页面，就必须等待 `deviceready` 事件再次触发才能使用插件。当应用变得越来越大，这点很容易被忽略。
即使你选择不用 Corodova，在创建应用时如果不使用单页面结构也会有严重的性能影响。这是因为页面之间导航跳转需要加载脚本，资源等等，即使这些资源被缓存下来，仍然会存在性能问题。
在 Cordova 应用里面可以使用的一些 SPA 库：
- [AngularJS](https://link.jianshu.com?t=http://angularjs.org/)
- [EmberJS](https://link.jianshu.com?t=http://emberjs.com/)
- [Backbone](https://link.jianshu.com?t=http://backbonejs.org/)
- [KendoUI](https://link.jianshu.com?t=http://www.telerik.com/kendo-ui)
- [Monaca](https://link.jianshu.com?t=http://monaca.mobi/en/)
- [ReactJS](https://link.jianshu.com?t=http://facebook.github.io/react/)
- [Sencha Touch](https://link.jianshu.com?t=http://www.sencha.com/products/touch/)
- [jQuery Mobile](https://link.jianshu.com?t=http://jquerymobile.com/)
还有很多很多......
## 2）性能考虑
考虑下面的问题以提高移动应用性能：
**Click VS Touch**
你可能犯的最大也是最简单的错误就是使用 `click` 事件，虽然在手机上用得好好的，但在大部分的设备上，为了把它和 `touch`、`hold` 事件区分开，会强制延迟 300ms。使用 `touchstart` 或者 `touchend` 事件会得到极大的改善，300ms 听起来不多，但可能会导致不流畅的 UI 更新和行为。你可能会考虑到在非 webkit 的浏览器上不支持 `touch` 事件，看[CanIUse](https://link.jianshu.com?t=http://caniuse.com/#search=touch)。为了处理这些限制，你可以 checkout 像 HandJS 和 Fastclick 这样的库。
**CSS转换 VS DOM操作**
使用硬件加速CSS转换比使用JavaScript创建动画要好很多。请看后面的资源清单。
**网络抽风**
好吧，网络并不总是抽风，但即使好的手机网络也会有延迟，甚至比你想象中还要严重。一个每 30s 下载 500 行 JSON 数据的桌面应用，在手机上或者电池猪上会更慢。要记住：Cordova 应用有多种方法持久化数据，比如：LocalStorage 和 file system。在本地缓存数据并弄清来回发送的数据的数量，这在你的应用使用蜂窝网络连接时尤其重要。
**其它性能方面的文章和资源**
- ["You half assed it"](https://link.jianshu.com?t=http://sintaxi.com/you-half-assed-it)
- ["Top Ten Performance Tips for PhoneGap and Hybrid Apps"](https://link.jianshu.com?t=http://coenraets.org/blog/2013/10/top-10-performance-techniques-for-phonegap-and-hybrid-apps-slides-available/)
- ["Fast Apps and Sites with JavaScript"](https://link.jianshu.com?t=https://channel9.msdn.com/Events/Build/2013/4-313)
## 3）识别并处理离线状态
从上面关于网络的技巧可看到，不仅可能会遇到慢网络，应用还可能会进入离线状态。你的应用应该智能地处理这种情况。否则，人们会认为你的应用死掉了。这很容易处理（Crodova 支持监听 `offline`和 `online`事件），应用绝对没有理由在离线状态下不给出好的响应。确保测试（看下面的测试章节）并确保测试从一种状态切换到另一种状态应用是如何处理的。
注意，`online`、`offline`事件以及网络连接 API 不是完美的。可能需要依赖使用 XHR 请求来查看设备是真的离线或在线。最后，确保为网络问题增加技术支持表单。实际上，Apple 商店（或者其它商店）会拒绝没有恰当处理离线／在线状态的应用。关于这个主题，请看 ["Is This Thing On?"](https://link.jianshu.com?t=http://blogs.telerik.com/appbuilder/posts/13-04-23/is-this-thing-on-%28part-1%29) 。
译自 ：[Best Practices Cordova app development](https://link.jianshu.com?t=http://cordova.apache.org/docs/en/latest/guide/next/index.html#best-practices-cordova-app-development)
