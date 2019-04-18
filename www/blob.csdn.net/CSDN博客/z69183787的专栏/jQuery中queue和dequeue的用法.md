# jQuery中queue和dequeue的用法 - z69183787的专栏 - CSDN博客
2014年03月25日 16:53:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1606
jQuery中的queue和dequeue是一组很有用的方法，他们对于一系列需要按次序运行的函数特别有用。特别animate动画，ajax，以及timeout等需要一定时间的函数
queue和dequeue的过程主要是：
1，用queue把函数加入队列（通常是函数数组）
2，用dequeue将函数数组中的第一个函数取出，并执行(用shift()方法取出并执行）
也就意味着当再次执行dequeue的时候，得到的是另一个函数了
同时也意味着，如果不执行dequeue，那么队列中的下一个函数永远不会执行
对于一个元素上执行animate方法加动画，jQuery内部也会将其加入名为 fx 的函数队列
而对于多个元素要依次执行动画，则必须我们手动设置队列进行了。
一个例子，要两个div依次向左移动，[点击这里查看](http://shawphy.com/demo/queue.html)
- div{
- background:#aaa;
- width:18px;
- height:18px;
- position:absolute;
- top:10px;
- }
- <divid="block1"></div>
- <divid="block2"></div>
如果只是轮流移动次数较少，可以使用animate的回调函数来做，一个动画放在另一个动画的回调里边
比如
- $("#block1").animate({left:"+=100"},function(){
-     $("#block2").animate({left:"+=100"},function(){
-         $("#block1").animate({left:"+=100"},function(){
-             $("#block2").animate({left:"+=100"},function(){
-                 $("#block1").animate({left:"+=100"},function(){
- alert("动画结束");
- });
- });
- });
- });
- });
但这种方法当动画较多的时候简直是残忍。
此时利用queue和dequeue则显得简单很多：
- varFUNC=[
- function(){$("#block1").animate({left:"+=100"},aniCB);},
- function(){$("#block2").animate({left:"+=100"},aniCB);},
- function(){$("#block1").animate({left:"+=100"},aniCB);},
- function(){$("#block2").animate({left:"+=100"},aniCB);},
- function(){$("#block1").animate({left:"+=100"},aniCB);},
- function(){alert("动画结束")}
- ];
- varaniCB=function(){
-     $(document).dequeue("myAnimation");
- }
- $(document).queue("myAnimation",FUNC);
- aniCB();
1，我首先~~建议~~建立了一个函数数组，里边是一些列需要依次执行的动画
2，然后我定义了一个回调函数，用dequeue方法用来执行队列中的下一个函数
3，接着把这个函数数组放到document上的myAnimation的队列中（可以选择任何元素，我只是为了方便而把这个队列放在document上）
4，最后我开始执行队列中的第一个函数
这样做的好处在于函数数组是线性展开，增减起来非常方便。
而且，当不要要继续进行接下来动画的时候(比如用户点了某个按钮)，只需要清空那个队列即可。而要增加更多则只需要加入队列即可
- //清空队列
- $(document).queue("myAnimation",[]);
- //加一个新的函数放在最后
- $(document).queue("myAnimation",function(){alert("动画真的结束了！")});
以前发过一个wait插件，用于让动画之间可以暂停一段时间
[http://shawphy.com/2008/07/enabling-settimout-within-chained-functions-in-jquery.html](http://shawphy.com/2008/07/enabling-settimout-within-chained-functions-in-jquery.html)
可以看一下，他也就是利用了这个原理，默认在fx中插入一个timeout，放到队列中，直到timeout结束后才执行dequeue继续执行队列中的下一个函数。
这当然也可以用于ajax之类的方法，如果需要一系列ajax交互，每个ajax都希望在前一个结束之后开始，之前最原始的方法就是用回调函数，但这样太麻烦了。同样利用queue添加队列，每次ajax之后的回调中执行一次dequeue即可。
