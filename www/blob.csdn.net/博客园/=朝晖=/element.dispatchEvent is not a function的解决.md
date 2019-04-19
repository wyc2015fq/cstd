# element.dispatchEvent is not a function的解决 - =朝晖= - 博客园
# [element.dispatchEvent is not a function的解决](https://www.cnblogs.com/dhcn/p/7100306.html)
Firebug中的出错提示：
```
element.dispatchEvent is not a function  
 element.dispatchEvent(event);                       prototype.js (第 5734 行)
```
出错原因：同时引用了prototype和jQuery，我这儿引用的版本，prototype是1.7，jQuery测试了1.4.3和1.6.1版本。
解决办法请参考：[http://beutelevision.com/blog2/2009/08/28/using-jquery-and-prototype-together-and-avoiding-the-dreaded-elementdispatchevent-error/](http://beutelevision.com/blog2/2009/08/28/using-jquery-and-prototype-together-and-avoiding-the-dreaded-elementdispatchevent-error/)
申明：这个方法我也没测。
