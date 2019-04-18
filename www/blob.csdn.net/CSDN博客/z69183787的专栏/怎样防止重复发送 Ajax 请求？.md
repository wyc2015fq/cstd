# 怎样防止重复发送 Ajax 请求？ - z69183787的专栏 - CSDN博客
2014年05月09日 16:18:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5495
最近在做一个项目，里面需要js 异步请求的时候，把上一次的的请求取消，由于使用的是jquery,挡在手册里没有找到关于.abort()方法，在网上搜索了一下，在[http://ooxx.me/jquery-ajax-abort.orz](http://ooxx.me/jquery-ajax-abort.orz)看到有关于jquery 里.abort()使用方法,直接用例子:
current_request = $.get('/events', { '七夕': '发春' },function(resp) { alert(resp); });
if(current_request) {current_request.abort();}
但是使用这个current_request.abort();方法取消的时候，实际上会触发$.ajax success事件，所以在success是的回调函数需要添加判断current_request是否存在，存在才执行回调函数
不推荐用外部变量锁定或修改按钮状态的方式，因为那样比较难：
- 要考虑并理解 success, complete, error, timeout 这些事件的区别，并注册正确的事件，一旦失误，功能将不再可用；
- 不可避免地比普通流程要要多注册一个 complete 事件；
- 恢复状态的代码很容易和不相干的代码混合在一起；
我推荐用主动查询状态的方式（A、B，jQuery 为例）或工具函数的方式（C、D）来去除重复操作，并提供一些例子作为参考：
A. 独占型提交
只允许同时存在一次提交操作，并且直到本次提交完成才能进行下一次提交。
```
module.submit = function() {
  if (this.promise_.state() === 'pending') {
    return
  }
  return this.promise_ = $.post('/api/save')
}
```
B. 贪婪型提交
无限制的提交，但是以最后一次操作为准；亦即需要尽快给出最后一次操作的反馈，而前面的操作结果并不重要。
```
module.submit = function() {
  if (this.promise_.state() === 'pending') {
    this.promise_.abort()
  }
  // todo
}
```
比如某些应用的条目中，有一些进行类似「喜欢」或「不喜欢」操作的二态按钮。如果按下后不立即给出反馈，用户的目光焦点就可能在那个按钮上停顿许久；如果按下时即时切换按钮的状态，再在程序上用 abort 来实现积极的提交，这样既能提高用户体验，还能降低服务器压力，皆大欢喜。
C. 节制型提交
无论提交如何频繁，任意两次有效提交的间隔时间必定会大于或等于某一时间间隔；即以一定频率提交。
```
module.submit = throttle(, function() {
  // todo
})
```
如果客户发送每隔100毫秒发送过来10次请求，此模块将只接收其中6个（每个在时间线上距离为150毫秒）进行处理。
这也是解决查询冲突的一种可选手段，比如以知乎草稿举例，仔细观察可以发现：
编辑器的 blur 事件会立即触发保存；
保存按钮的 click 事件也会立即触发保存；
但是存在一种情况会使这两个事件在数毫秒内连续发生——当焦点在编辑器内部，并且直接去点击保存按钮——这时用 throttle 来处理是可行的。
另外还有一些事件处理会很频繁地使用 throttle，如： resize、scroll、mousemove。
D. 懒惰型提交
任意两次提交的间隔时间，必须大于一个指定时间，才会促成有效提交；即不给休息不干活。
```
module.submit = debounce(, function() {
  // todo
})
```
还是以知乎草稿举例，当在编辑器内按下 ctrl + s 时，可以手动保存草稿；如果你连按，程序会表示不理解为什么你要连按，只有等你放弃连按，它才会继续。
============
更多记忆中的例子
方式 C 和 方式 D 有时更加通用，比如这些情况：
- 游戏中你捡到一把威力强大的高速武器，为了防止你的子弹在屏幕上打成一条直线，可以 throttle 来控制频率；
- 在弹幕型游戏里，为了防止你把射击键夹住来进行无脑游戏，可以用 debounce 来控制频率；
- 在编译任务里，守护进程监视了某一文件夹里所有的文件（如任一文件的改变都可以触发重新编译，一次执行就需要2秒），但某种操作能够瞬间造成大量文件改变（如 git checkout），这时一个简单的 debounce 可以使编译任务只执行一次。
而方式 C 甚至可以和方式 B 组合使用，比如自动完成组件（Google 首页的搜索就是）：
- 当用户快速输入文本时（特别是打字能手），可以 throttle keypress 事件处理函数，以指定时间间隔来提取文本域的值，然后立即进行新的查询；
- 当新的查询需要发送，但上一个查询还没返回结果时，可以 abort 未完成的查询，并立即发送新查询；
----- update 2013-01-08 -----
E. 记忆型
```
var scrape = memoize(function(url) {
  return $.post('/scraper', { 'url': url })
})
```
对于同样的参数，其返回始终结果是恒等的——每次都将返回同一对象。
应用例子有编辑器，如粘贴内容时抓取其中的链接信息，memoize 用以保证同样的链接不会抓取两次。
----- update 2013-03-27 -----
F. 累积型
前几天处理自动完成事件时得到这个函数，发现也可以用在处理连续事件上，它能够把连续的多次提交合并为一个提交，比如：
```
var request = makePile(, function() {
    $.post('/', { list: JSON.stringify([].slice.call(arguments)) })
})
// 连续发送五次 
request({a:}), request({a:}), request({a:}), request({a:}), request({a:})
/* post =>
list:[{"a":1},{"a":2},{"a":3},{"a":4},{"a":5}]
 */
```
样例实现：
```
var makePile = function(count, onfilter, onvalue) {
  var values = [], id = function(value) { return value }
  return function(value) {
    values.push((onvalue || id).apply(this, arguments))
    if (values.length === count) {
      onfilter.apply(this, values)
      values = []
    }
  }
}
```
----- update 2013-04-16 -----
另一种累积是按时间而不是次数，比如应用在行为统计上，可能在瞬间收集到数十上百类似的行为，这时可以用上面 pile 的结构加上 debounce 来防止大批重复请求（但又不丢失任何统计）：
```
var trackFactory = function(delay, action) {
  var params = [], slice = [].slice
  var touch = debounce(delay, function() {
    if (params.length) {
      action(params)
      params = []
    }
  })
  return function() {
    params.push(slice.call(arguments))
    touch()
  }
}
var track = trackFactory(, function(params) {
  // send tracking request
})
```
G. 采样型
这是最近重构时联想到的，一种和上面都不同的去重操作，可以应用在自动加载（timeline）行为控制上：
```
autoload.listen(feeds, 'next', sample(3, function() {
  this.enable()
}))
```
如果 sample 是固化的选择函数（n 选 1），它这实际上会这样工作：
```
O-O-X-O-O-X
```
但「自动加载」的应用可能想要的是（两次自动，一次手动）：
```
X-X-O-X-X-O
```
对于这种情况，可以定义作为配置的选择函数来实现控制：
```
options { sample: (n) => n % 3 !== 0 }
```
即每个下一次加载完成之后， 每三次有两次对下一次加载实行自动加载。
