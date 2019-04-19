# 如何防止重复发送Ajax请求 - 零度的博客专栏 - CSDN博客
2016年04月01日 10:27:03[零度anngle](https://me.csdn.net/zmx729618)阅读数：2527
                
做一个项目，里面需要js 异步请求的时候，把上一次的的请求取消，由于使用的是jquery,挡在手册里没有找到关于.abort()方法，在网上搜索了一下，在[http://ooxx.me/jquery-ajax-abort.orz](http://ooxx.me/jquery-ajax-abort.orz)看到有关于jquery
 里.abort()使用方法,直接用例子:
current_request = $.get('/events', { '七夕': '发春' },function(resp) { alert(resp); });
if(current_request) {current_request.abort();}
但是使用这个current_request.abort();方法取消的时候，实际上会触发$.ajax success事件，所以在success是的回调函数需要添加判断current_request是否存在，存在才执行回调函数
不推荐用外部变量锁定或修改按钮状态的方式，因为那样比较难：
- 
要考虑并理解 success, complete, error, timeout 这些事件的区别，并注册正确的事件，一旦失误，功能将不再可用；
- 
不可避免地比普通流程要要多注册一个 complete 事件；
- 
恢复状态的代码很容易和不相干的代码混合在一起；
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
